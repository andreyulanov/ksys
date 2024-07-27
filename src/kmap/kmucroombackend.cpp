#include "kmucroombackend.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

KMucRoomsController::KMucRoomsController(QObject *parent)
    : QObject{parent}
{
    // nothing to do
}

void KMucRoomsController::setRoomJid(QString _room_jid)
{
    if (room_jid != _room_jid)
    {
        room_jid = _room_jid;
        roomJidChanged();
    }
    if (!isJidValid())
    {
        invalidJid();
    }
}
QString KMucRoomsController::roomJid()
{
    return room_jid;
}
bool KMucRoomsController::isJidValid()
{
    return jid_regexp.match(room_jid).hasMatch();
}
void KMucRoomsController::add()
{
    if (!isJidValid())
    {
        invalidJid();
        return;
    }
    addRoom(room_jid);
}

KMucRoomsModel::KMucRoomsModel(QXmppMucManager* manager,
                               KMucRoomsController* controller,
                               QObject *parent):
    KAbstractDbListModel(parent)
{
    setManager(manager);
    setController(controller);
}

int KMucRoomsModel::rowCount(const QModelIndex&) const
{
    return rooms.size();
}

QXmppMucRoom* KMucRoomsModel::roomByIndex(const QModelIndex &index) const
{
    int indx = index.row();
    if (!index.isValid() || indx >= rooms.length() || indx < 0) return nullptr;
    return rooms.at(indx);
}
QVariant KMucRoomsModel::data(const QModelIndex &index, int role) const
{
    QXmppMucRoom* room = roomByIndex(index);
    if (room == nullptr) {
        qWarning() << "KMucRoomsModel: Invalid index";
        return QVariant();
    }
    switch (role) {
    case MucRoomRole:
    {
        QVariant result;
        result.setValue(room);
        return result;
    }
    default:
        qWarning() << "KMucRoomsModel::data: Invalid role";
        return QVariant();
    }
}

QHash<int, QByteArray> KMucRoomsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[MucRoomRole] = "mucRoom";

    return roles;
}

void KMucRoomsModel::setManager(QXmppMucManager* _manager)
{
    manager = _manager;
}

void KMucRoomsModel::setController(KMucRoomsController* controller)
{
    if (controller == nullptr) return;
    connect(controller, &KMucRoomsController::addRoom,
            this, &KMucRoomsModel::roomAddedSlot);
}


QXmppMucRoom* KMucRoomsModel::createRoom(const QString& room_jid)
{
    int row_to_add = rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), row_to_add, row_to_add);
    qDebug() << "KMucRoomsModel: adding row number" << row_to_add;
    QXmppMucRoom* room = manager->addRoom(room_jid);
    rooms.append(room);
    room->requestConfiguration();
    connect(room, &QXmppMucRoom::allowedActionsChanged,
            this, &KMucRoomsModel::updateRoomSlot);
    connect(room, &QXmppMucRoom::nameChanged,
            this, &KMucRoomsModel::updateRoomSlot);
    connect(room, &QXmppMucRoom::nickNameChanged,
            this, &KMucRoomsModel::updateRoomSlot);
    connect(room, &QXmppMucRoom::subjectChanged,
            this, &KMucRoomsModel::updateRoomSlot);
    endInsertRows();
    return room;
}
bool KMucRoomsModel::insertRoomToDatabase(QXmppMucRoom* room)
{
    if (noDatabaseMode()) return false;
    QSqlQuery query(*database);
    query.prepare("INSERT INTO " + table_name + "(jid, nickname, password)"
                  "VALUES (:jid, :nickname, :password)");
    query.bindValue(":jid", room->jid());
    query.bindValue(":nickname", room->nickName());
    query.bindValue(":password", room->password()); /// FIXME: We should store passwords in more secure way...
    if (!query.exec())
    {
        qCritical() << "Unable to save a MUC room:"
                    << query.lastError().text();
        return false;
    }
    return true;
}

bool KMucRoomsModel::updateRoomInDatabase(QXmppMucRoom* room)
{
    if (noDatabaseMode()) return false;
    QSqlQuery query(*database);
    query.prepare("UPDATE" + table_name + "nickname = :nickname," +
                                          "password = :password)" +
                  "WHERE jid = :jid");
    query.bindValue(":jid", room->jid());
    query.bindValue(":nickname", room->nickName());
    query.bindValue(":password", room->password());
    if (!query.exec())
    {
        qCritical() << "Unable to update a MUC room:"
                    << query.lastError().text();
        return false;
    }
    return true;
}

QXmppMucRoom* KMucRoomsModel::roomAddedSlot(const QString& room_jid)
{
    QXmppMucRoom* room = createRoom(room_jid);
    if (!insertRoomToDatabase(room)) // clean up if failed to save
    {
        int row = rowCount(QModelIndex()) - 1;
        beginRemoveRows(QModelIndex(), row, row);
        removeRows(row,1);
        endRemoveRows();
        return nullptr;
    }
    return room;
}

bool KMucRoomsModel::loadFromDatabase()
{
    qDebug() << "Loading MUC Rooms table";
    if (noDatabaseMode()) return false;
    QSqlQuery query(*database);
    query.exec("SELECT jid, nickname, password from " + table_name);
    if (query.lastError().isValid())
    {
        qDebug() << "Failed to load rooms:" << query.lastError();
        return false;
    }
    while (query.next())
    {
        qDebug() << "Room loaded from the database :" << query.value(0).toString();
        QXmppMucRoom* room = createRoom(query.value(0).toString());
        if (!query.value(1).isNull()) room->setNickName(query.value(1).toString());
        if (!query.value(2).isNull()) room->setPassword(query.value(2).toString());
    }
    return true;
}

bool KMucRoomsModel::createTable()
{
    qDebug() << "Creating MUC Rooms table";
    if (noDatabaseMode()) return false;
    QSqlQuery query(*database);
    query.prepare("CREATE TABLE IF NOT EXISTS\"" + table_name + "\" ("
        "\"jid\"	TEXT NOT NULL UNIQUE,"
        "\"nickname\"	TEXT,"
        "\"password\"	TEXT,"
        "PRIMARY KEY(\"jid\")"
    ")");
    if (!query.exec())
    {
        qCritical() << "Unable to create MUC rooms table:" << query.lastError().text();
        return false;
    }
    return true;
}

bool KMucRoomsModel::removeRoomFromDatabase(QXmppMucRoom* room)
{
    qDebug() << "Removing room" << room->jid() << "from database";
    if (noDatabaseMode()) return true;
    QSqlQuery query(*database);
    query.prepare("DELETE FROM MUC_rooms WHERE jid = :jid;");
    query.bindValue(":jid", room->jid());
    if (!query.exec())
    {
        qWarning() << "Failed to remove room from the database:" << query.lastError().text();
        return false;
    }
    return true;
}

bool KMucRoomsModel::removeRows(int row, int count, const QModelIndex &parent)
{
    int last_row = row + count - 1;
    beginRemoveRows(parent, row, last_row);
    if (row < 0 || last_row  >= rowCount(parent))
    {
        qWarning() << "KMucRoomsModel::removeRows: index out of range." << Qt::endl
                   << "It tries to remove" << count << "rows from" << row << Qt::endl
                   << "But there are only" << manager->rooms().length() << "of them.";
        endRemoveRows();
        return false;
    }
    bool result = true;
    QXmppMucRoom* room_to_remove = nullptr;
    for (int i = row; i <= last_row; i++)
    {
        room_to_remove = roomByIndex(index(i));
        if (room_to_remove == nullptr)
        {
            qWarning() << "Something went wrong, trying to remove nullptr muc room";
            result = false;
            break;
        }
        else
        {
            result &= removeRoomFromDatabase(room_to_remove);
            if (room_to_remove->isJoined()) room_to_remove->leave();
        }
    }
    rooms.remove(row,count);
    endRemoveRows();
    return result;
}

int KMucRoomsModel::roomIndex(QXmppMucRoom* room)
{
    int row_count = rowCount(QModelIndex());
    for (int i = 0; i < row_count; i++)
    {
        if (rooms[i] == room) return i;
    }
    return -1;
}

void KMucRoomsModel::updateRoomSlot()
{
    //TODO: I use QObject::sender here for simplicity, but it's a bad practice. I should improve it.
    QXmppMucRoom* room = (QXmppMucRoom *) sender();
    qDebug() << "Updating room" << room->jid();
    updateRoomInDatabase(room);
    QModelIndex indx =  index(roomIndex(room));
    //TODO: I emmit signal without vector of changed roles.
    // It cause all roles to be updated.
    dataChanged(indx, indx);
}
