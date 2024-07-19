#include "kabstractdblistmodel.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

KAbstractDbListModel::KAbstractDbListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    //nothing to do
}

void KAbstractDbListModel::setDatabase(QSqlDatabase* _database)
{
    qDebug() << "Setting up a database...";
    if (_database == nullptr)
    {
        qWarning() << "KMucRoomsModel: database points to null";
        database = nullptr;
        return;
    }
    database = _database;
    if (!database->isOpen())
        database->open();
    if (database->isOpenError())
    {
        qCritical() << "KMucRoomsModel: database openned with error:"
                    << database->lastError().text();
        database = nullptr;
    }
    else
    {
        if (!createTable()) qWarning() << "Failed to create table in the database!";
        if (!loadFromDatabase()) qWarning() << "Failed to load from the database!";
    }
}
