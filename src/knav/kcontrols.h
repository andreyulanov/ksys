#ifndef KCONTROLS_H
#define KCONTROLS_H

#include <QPushButton>
#include <QLineEdit>
#include <QElapsedTimer>
#include "kmap.h"
#include "kfindwidget.h"

class KControls: public QObject
{
  Q_OBJECT

  enum ZoomMode
  {
    None,
    In,
    Out
  };

  enum OrientMode
  {
    North,
    Heading
  };

  bool        centering_enabled = false;
  OrientMode  orient_mode       = North;
  double      heading           = 0;
  ZoomMode    zoom_mode         = None;
  QPushButton find;
  QPushButton zoom_in;
  QPushButton zoom_out;
  QPushButton center_position;
  QPushButton orient;
  QPushButton record;
  QPushButton add;
  QPushButton ok;
  QPushButton ok_and_send;
  QPixmap     orient_pixmap;
  KGeoCoor    curr_coor;
  QWidget*    findw = nullptr;

  static void initButton(QPushButton* b, QPixmap image_path,
                         QPoint pos, double size_mm);
  void        setIcon(QPushButton* b, QPixmap pm);
  void        onFind();
  void        onZoomIn();
  void        onZoomOut();
  void        onZoomReleased();

signals:
  void zoomIn();
  void zoomOut();
  void zoomReleased();
  void scrollTo(const KGeoCoor&);
  void setRotation(double);
  void changedFindString(const QString&);
  void switchRecording();
  bool isRecording();
  void selectShape();
  void acceptObject();
  void enableRotation();
  void disableRotation();
  void sendOnSave();

public:
  KControls(QWidget* w, QWidget* _findw, double edge_mm,
            double step_mm, double button_size_mm);
  void setCurrCoor(const KGeoCoor&);
  void update();
  void updatePosition(const KGeoCoor&);
  void updateHeading(double);
  void checkZoomRepeat();
  void enableCentering();
  void onSwitchRecording();
  void onMouseMoved();
  void switchOrient();
  bool hasNorthOrientation();
  void finishEdit();
};

#endif  // KCONTROLS_H
