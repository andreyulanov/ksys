#include <math.h>
#include <QApplication>
#include <QStandardPaths>
#include <QScreen>
#include <QDir>
#include <QDebug>
#include <QProcess>
#include <QStandardPaths>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlEngine>
#include <QQmlContext>
#include <QSqlDatabase>

#include "krenderwidget.h"
#include "kautoscroll.h"
#include "kcontrols.h"
#include "keditwidget.h"
#include "kpack.h"
#include "kclassmanager.h"
#include "kposgenerator.h"
#include "ktrackmanager.h"
#include "knewobjectwidget.h"
#include "kpackfetcher.h"
#include "kscalelabel.h"
#include "ksettings.h"
#include "kstoragemanager.h"
#ifdef BUILD_WITH_XMPP
  #include "kmucroombackend.h"
  #include "kxmppclient.h"
  #include "kmessagemodel.h"
  #include "kfilesmodel.h"
  #include "kmessagefileconnectionmodel.h"
//  #include "krosterwidget.h"
  #include <kportableobjectsender.h>
  #include <QXmppQt5/QXmppClient.h>
  #include <QXmppQt5/QXmppLogger.h>
  #include <QXmppQt5/QXmppRosterManager.h>
  #include <QXmppQt5/QXmppMamManager.h>
#endif

#ifdef BUILD_WITH_SENSORS
  #include <QGeoPositionInfoSource>
  #include <QGeoCoordinate>
  #include "kpositionlabel.h"
  #include "kheading.h"
#endif

using namespace kmath;

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  QSysInfo si;
  auto     is_device = si.productType().toLower().contains("android");

  auto   screen          = QGuiApplication::screens().first();
  QSize  screen_size_pix = screen->availableSize();
  QSizeF screen_size_mm  = screen->physicalSize();

  auto   physical_diag_mm = sqrt(pow(screen_size_mm.width(), 2) +
                                 pow(screen_size_mm.height(), 2));
  auto   pixel_diag       = sqrt(pow(screen_size_pix.width(), 2) +
                                 pow(screen_size_pix.height(), 2));
  double pixel_size_mm    = physical_diag_mm / pixel_diag;

  if (!is_device)
    pixel_size_mm /= 2;

  KEditWidget editw;
  editw.setFixedSize(screen_size_pix);
  editw.move((screen_size_pix.width() - editw.width()) / 2,
             (screen_size_pix.height() - editw.height()) / 2);

  KNewObjectWidget newobjw;
  newobjw.setFixedSize(screen_size_pix);

  QString     mmc_path;
  QStringList dir_list;
  if (is_device)
  {
    QStringList dir_list = QStandardPaths::standardLocations(
        QStandardPaths::AppDataLocation);
    for (auto dir: dir_list)
    {
      if (QDir(dir + "/packs").exists())
      {
        mmc_path = dir;
        break;
      }
    }
  }
  else
  {
    if (argc > 1)
      mmc_path = argv[1];
    else
    {
      mmc_path = QStandardPaths::writableLocation(
          QStandardPaths::AppDataLocation);
      if (mmc_path.isEmpty())
      {
        qCritical("Standard app's data diricory is not writable");
      }
    }
  }
  kStorageManager storage_man(mmc_path);

  KRenderWidget::Settings mapw_settings;
  mapw_settings.map_dir                 = storage_man.packsPath();
  mapw_settings.pixel_size_mm           = pixel_size_mm;
  mapw_settings.window_size             = screen_size_pix;
  mapw_settings.render_window_size_coef = 2;
  mapw_settings.update_interval_ms      = 100;
  mapw_settings.max_zoom_speed          = 1.5;

  KRenderWidget      renderw(mapw_settings);
  KPackFetcher       map_fetcher(mapw_settings.map_dir,
                                 renderw.getWorldPack());
  KClassManager      user_class_man(storage_man.classPath());
  KTrackManager      track_man(storage_man.tracksPath());
  KFreeObjectManager object_man(storage_man.objectsPath(),
                                pixel_size_mm);
  KAutoScroll        auto_scroll;

  QObject::connect(&map_fetcher, &KPackFetcher::fetched,
                   [&renderw](QString map_path)
                   {
                     renderw.addMap(map_path, true);
                     renderw.render();
                   });

  QDir        dir(storage_man.classPath());
  QStringList filters;
  filters << "*.json";
  dir.setNameFilters(filters);

  auto path_list = dir.entryList();
  for (auto path: path_list)
    user_class_man.loadClasses(storage_man.classPath() + "/" + path,
                               storage_man.classPath());

  QObject::connect(&editw, &KEditWidget::saveTrack, &track_man,
                   &KTrackManager::saveTrack);

  KGeoCoor start_lat_lon = KGeoCoor::fromDegs(59.9769195, 30.3642851);

  QObject::connect(&renderw, &KRenderWidget::mousePressed,
                   &auto_scroll, &KAutoScroll::stop);
  QObject::connect(&renderw, &KRenderWidget::mousePressed,
                   &object_man,
                   &KFreeObjectManager::startMovingPoint);
  QObject::connect(&renderw, &KRenderWidget::mouseMoved, &auto_scroll,
                   &KAutoScroll::accumulate);
  QObject::connect(&renderw, &KRenderWidget::mouseReleased,
                   &auto_scroll, &KAutoScroll::start);
  QObject::connect(&renderw, &KRenderWidget::mouseReleased,
                   &object_man, &KFreeObjectManager::stopMovingPoint);
  QObject::connect(&renderw, &KRenderWidget::tapped, &object_man,
                   &KFreeObjectManager::onTapped);
  QObject::connect(&renderw, &KRenderWidget::pinchStarted,
                   &auto_scroll, &KAutoScroll::stop);
  QObject::connect(&renderw, &KRenderWidget::startedRender,
                   &map_fetcher, &KPackFetcher::requestRect);
  QObject::connect(&auto_scroll, &KAutoScroll::scroll, &renderw,
                   &KRenderWidget::scroll);

  QObject::connect(&renderw, &KRenderWidget::mouseMoved, &object_man,
                   &KFreeObjectManager::movePoint);
  double edge_mm        = 15;
  double step_mm        = 30;
  double button_size_mm = 15;
  if (is_device)
  {
    edge_mm        = 7;
    step_mm        = 20;
    button_size_mm = 7;
  }

  KControls::Settings control_settings;
  control_settings.map_widget     = &renderw;
  control_settings.edge_mm        = edge_mm;
  control_settings.step_mm        = step_mm;
  control_settings.button_size_mm = button_size_mm;
  control_settings.pixel_size_mm  = pixel_size_mm;

  KControls controls(control_settings);
  QObject::connect(&controls, &KControls::zoomIn, &auto_scroll,
                   &KAutoScroll::stop);
  QObject::connect(&controls, &KControls::zoomIn, &renderw,
                   &KRenderWidget::zoomIn);
  QObject::connect(&controls, &KControls::zoomOut, &auto_scroll,
                   &KAutoScroll::stop);
  QObject::connect(&controls, &KControls::zoomOut, &renderw,
                   &KRenderWidget::zoomOut);
  QObject::connect(&controls, &KControls::scrollTo, &renderw,
                   &KRenderWidget::scrollTo);
  QObject::connect(&controls, &KControls::switchRecording, &track_man,
                   &KTrackManager::onSwitchRecording);
  QObject::connect(&controls, &KControls::acceptObject, &object_man,
                   &KFreeObjectManager::acceptObject);
  QObject::connect(&track_man, &KTrackManager::switchRecording,
                   [&track_man, &editw]()
                   {
                     if (!track_man.isRecording())
                       editw.show(
                           "Enter track name",
                           QDateTime::currentDateTime().toString(
                               "yyyy-MM-dd,hh-mm-ss"));
                   });
  QObject::connect(&controls, &KControls::isRecording, &track_man,
                   &KTrackManager::isRecording);
  QObject::connect(&controls, &KControls::selectClass, &newobjw,
                   &KNewObjectWidget::show);
  QObject::connect(&controls, &KControls::removeObject, &object_man,
                   &KFreeObjectManager::removeObject);

  QObject::connect(&newobjw, &KNewObjectWidget::getUserClassImageList,
                   &user_class_man,
                   &KClassManager::getClassImageList);
  QObject::connect(&newobjw, &KNewObjectWidget::selectedClass,
                   &object_man, &KFreeObjectManager::createObject);
  QObject::connect(&newobjw, &KNewObjectWidget::getClassById,
                   &user_class_man, &KClassManager::getClassById);

  QObject::connect(&renderw, &KRenderWidget::zoomFinished, &controls,
                   &KControls::checkZoomRepeat);
  QObject::connect(&renderw, &KRenderWidget::mouseMoved, &controls,
                   &KControls::onMouseMoved);
  QObject::connect(&renderw, &KRenderWidget::movedCenterTo, &controls,
                   &KControls::setCurrCoor);

  KScaleLabel scale_label(&renderw);
  QObject::connect(&scale_label, &KScaleLabel::getMip, &renderw,
                   &KRenderWidget::getMip, Qt::DirectConnection);
  QObject::connect(&renderw, &KRenderWidget::modified, &scale_label,
                   &KScaleLabel::update);
  auto pos  = QPointF{0, renderw.height() - 5.0 / pixel_size_mm};
  auto size = QSizeF{20.0 / pixel_size_mm, 5.0 / pixel_size_mm};
  scale_label.setFixedSize(size.toSize());
  scale_label.move(pos.toPoint());

#ifdef BUILD_WITH_SENSORS
  KPositionLabel   position_label(&renderw);
  QGeoPositionInfo info;
  info.setCoordinate({59.9769195, 30.3642851});
  position_label.updateGeoPosition(info);
  QObject::connect(&renderw, &KRenderWidget::modified,
                   &position_label, &KPositionLabel::update);
  QObject::connect(&renderw, &KRenderWidget::paintUserObjects,
                   &track_man, &KTrackManager::paint,
                   Qt::DirectConnection);
  QObject::connect(&renderw, &KRenderWidget::paintUserObjects,
                   &object_man, &KFreeObjectManager::paint,
                   Qt::DirectConnection);
  QObject::connect(&renderw, &KRenderWidget::canScroll, &object_man,
                   &KFreeObjectManager::canScroll,
                   Qt::DirectConnection);
  QObject::connect(&track_man, &KTrackManager::deg2pix, &renderw,
                   &KRenderWidget::deg2pix, Qt::DirectConnection);
  QObject::connect(&object_man, &KFreeObjectManager::deg2pix,
                   &renderw, &KRenderWidget::deg2pix,
                   Qt::DirectConnection);
  QObject::connect(&object_man, &KFreeObjectManager::deg2scr,
                   &renderw, &KRenderWidget::deg2scr,
                   Qt::DirectConnection);
  QObject::connect(&object_man, &KFreeObjectManager::scr2deg,
                   &renderw, &KRenderWidget::scr2deg,
                   Qt::DirectConnection);
  QObject::connect(&position_label, &KPositionLabel::deg2scr,
                   &renderw, &KRenderWidget::deg2scr,
                   Qt::DirectConnection);

  QObject::connect(&track_man, &KTrackManager::updated, &renderw,
                   &KRenderWidget::renderUserObjects);
  QObject::connect(&object_man, &KFreeObjectManager::updated,
                   &renderw, &KRenderWidget::renderUserObjects);
  QObject::connect(&object_man, &KFreeObjectManager::startEdit,
                   &controls, &KControls::startEdit);
  QObject::connect(&object_man, &KFreeObjectManager::finishEdit,
                   &controls, &KControls::finishEdit);

  QGeoPositionInfoSource* geo =
      QGeoPositionInfoSource::createDefaultSource(&renderw);
  if (geo)
  {
    QObject::connect(geo, &QGeoPositionInfoSource::positionUpdated,
                     &position_label,
                     &KPositionLabel::updateGeoPosition);
    QObject::connect(geo, &QGeoPositionInfoSource::positionUpdated,
                     [&controls](const QGeoPositionInfo& geo)
                     {
                       auto c = geo.coordinate();
                       controls.updatePosition(KGeoCoor::fromDegs(
                           c.latitude(), c.longitude()));
                       controls.update();
                     });

    QObject::connect(
        geo, &QGeoPositionInfoSource::positionUpdated,
        [&track_man](const QGeoPositionInfo& geo)
        {
          auto c = geo.coordinate();
          if (track_man.isRecording())
            track_man.addPoint(KPosition{
                KGeoCoor::fromDegs(c.latitude(), c.longitude()),
                static_cast<float>(c.altitude()),
                KDateTime(QDateTime::currentDateTime())});
        });

    geo->startUpdates();
  }

  QMagnetometer    magnetometer(&renderw);
  KHeadingProvider heading_provider(&magnetometer);
  QObject::connect(&magnetometer, &QMagnetometer::readingChanged,
                   &heading_provider, &KHeadingProvider::update);
  QObject::connect(&heading_provider,
                   &KHeadingProvider::headingChanged, &position_label,
                   &KPositionLabel::updateHeading);
  magnetometer.setReturnGeoValues(true);
  magnetometer.start();

  KPosGenerator pos_gen(start_lat_lon,
                        KGeoCoor::fromDegs(0.00001, 0.00001));
  if (!is_device)
  {
    QObject::connect(&pos_gen, &KPosGenerator::generated_pos,
                     &track_man, &KTrackManager::addPoint);
    QObject::connect(&pos_gen, &KPosGenerator::generated_coor,
                     &controls, &KControls::updatePosition);
    QObject::connect(&pos_gen, &KPosGenerator::generated_coor,
                     &position_label,
                     &KPositionLabel::updatePosition);
    QObject::connect(&pos_gen, &KPosGenerator::generated_angle,
                     &heading_provider,
                     &KHeadingProvider::headingChanged);
  }

#endif

  //открывает окно с картой
  // renderw.show();
  renderw.setViewPoint(start_lat_lon, 1);

#ifdef BUILD_WITH_XMPP
  /// TODO: Improve setting up
  QString aliceJid      = "knav.alice@macaw.me";
  QString alicePassword = "very-secure-password-for-knav-alice";
  QString bobJid        = "knav.bob@macaw.me";
  QString bobPassword   = "very-secure-password-for-knav-bob";
  // QString jidResource	 	= String("QXmpp");
  QString jidResource = "flowerpot";
  QString proxy       = "proxy.macaw.me"; // FIXME: find proxy with discovery manager

  KXmppClient client(storage_man.objectsPath(), proxy); client.logger()->setLogFilePath(storage_man.logsPath() +
                                  "/client.log");
  client.logger()->setLoggingType(QXmppLogger::FileLogging);

  // some database stuff
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(storage_man.databasePath());

  KMucRoomsController muc_controller;
  KMucRoomsModel muc_rooms_model(client.findExtension<QXmppMucManager>(),
                                 &muc_controller);
  if (!db.open())
  {
      qWarning() << "Error: connection with database failed";
  }
  else
  {
      qDebug() << "Database: connection ok";
      muc_rooms_model.setDatabase(&db);
  }

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QQuickView view(QUrl("qrc:/Main.qml"));
  view.connect(view.engine(), &QQmlEngine::quit, &a, &QCoreApplication::quit);
  if (view.status() == QQuickView::Error)
  {
      auto errors = view.errors();

      for (qsizetype i = 0; i < errors.size(); i++)
      {
          qDebug() << errors[i].toString();
      }
      qDebug() << "Failed with QQuickView::Error";
      return -1;
  }
  QObject *root_item = (QObject*) view.rootObject();
  QObject::connect(root_item, SIGNAL(connectToServer(QString, QString)),
                   &client, SLOT(reconnectToServer(QString, QString)));

  QQmlContext* root_context = view.engine()->rootContext();
  root_context->setContextProperty("kClient",&client);
  root_context->setContextProperty("_mucRoomsModel", &muc_rooms_model);
  root_context->setContextProperty("_mucBackEnd", &muc_controller);

  KSettings k_settings;
  root_context->setContextProperty("_kSettings", &k_settings);

  if (k_settings.autologin() && k_settings.thereIsASavedAccount())
  {
    client.reconnectToServer(k_settings.jid(), k_settings.password());
  }

  //Message archive (MAM and database)

  //QXmppMamManager mam_manager;
  //client.addExtension(&mam_manager);

  KMessageModel message_model(&client);
  message_model.setDatabase(&db);

  KFilesModel files_model;
  files_model.setDatabase(&db);

  KMessageFileConnectionModel message_file_connection_model;
  message_file_connection_model.setDatabase(&db);

  view.show();

  return a.exec();
}

#endif
