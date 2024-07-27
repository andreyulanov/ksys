#ifndef KMUCROOMBACKEND_H
#define KMUCROOMBACKEND_H

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include <QXmppQt5/QXmppMucManager.h>
#include <QRegularExpression>
#include <QtSql/QSqlDatabase>
#include <QMetaType>

#include "kabstractdblistmodel.h"

/// \brief Contorller of MUC rooms.
class KMucRoomsController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString room_jid READ roomJid WRITE setRoomJid NOTIFY roomJidChanged)

public:
    explicit KMucRoomsController(QObject *parent = nullptr);
    QString roomJid();
    void setRoomJid(QString _room_jid);
    /// \brief Add a new room.
    /// Do not use this function if a room with _room_jid is already in QXmppMucManager::rooms()
    Q_INVOKABLE void add();
signals:
    void roomJidChanged();
    /// User entered incorrect Jid
    void invalidJid();
    /// User adds a MUC room
    void addRoom(const QString& room_jid);
private:
    QString room_jid;
    bool isJidValid();
    inline static const QRegularExpression jid_regexp{"[^@/<>'\"]*@[^@/<>'\"]*(/[^@/<>'\"])*"}; // FIXME
    // this regex far from the perfect one.
    // For more see https://datatracker.ietf.org/doc/html/rfc6122#section-2
};


/// \brief Model that contains chat rooms.
///
/// TODO: Model stores room pointers inside itself, but there
class KMucRoomsModel : public KAbstractDbListModel
{
    Q_OBJECT

public:
    enum Roles {
        MucRoomRole = Qt::UserRole + 1,
    };

    /// \brief Constructs a new KMucRoomsModel.
    ///
    /// _manager and _databse must be ready for work
    KMucRoomsModel(QXmppMucManager* _manager,
                   KMucRoomsController* _controller = nullptr,
                   QObject *parent = nullptr);

    virtual int rowCount(const QModelIndex&) const;
    Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QHash<int, QByteArray> roleNames() const;

    void setManager(QXmppMucManager* _manager);
    /// Sets the controller.
    ///
    /// Actually this function only do signal-slot connection.
    void setController(KMucRoomsController*);
    /// Loads chats from database and adds them to the manager.
    bool loadFromDatabase();
    /// Create table of chats in the database.
    bool createTable();
private:
    QXmppMucManager* manager = nullptr;
    /// if database == nullptr then the model will not store data in a database
    QVector<QXmppMucRoom*> rooms;
    inline static const QString table_name = "MUC_rooms";

    /// Leave the room and remove it from the database
    bool removeRoom(QXmppMucRoom*);
    /// Returns room's index in the rooms vetor, O(n)
    int roomIndex(QXmppMucRoom* room);
    QXmppMucRoom* roomByIndex(const QModelIndex &index) const;
    // Create a new room but do not save it to the database.
    QXmppMucRoom* createRoom(const QString& room_jid);
    // destry a room but do not remove it from the database.
    bool destroyRoom(QXmppMucRoom* room);
    // Insert a new room to the database.
    bool insertRoomToDatabase(QXmppMucRoom* room);
    // Update existing room in the database.
    bool updateRoomInDatabase(QXmppMucRoom* room);
    // Remove room to the database.
    bool removeRoomFromDatabase(QXmppMucRoom* room);

private slots:
    /// Adds room to the model and the database.
    QXmppMucRoom* roomAddedSlot(const QString& room_jid);
    /// Update the room.
    void updateRoomSlot();
};


#endif // KMUCROOMBACKEND_H
