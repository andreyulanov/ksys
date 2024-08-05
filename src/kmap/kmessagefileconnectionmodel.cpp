#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include "kmessagefileconnectionmodel.h"
#include "kmessagemodel.h"
#include "kfilesmodel.h"

KMessageFileConnectionModel::KMessageFileConnectionModel(QObject *parent)
    : KAbstractDbListModel{parent}
{

}
bool KMessageFileConnectionModel::createTable()
{
    if (noDatabaseMode())
    {
        qWarning() << "No table have been created due to the noDatabaseMode";
        return false;
    }
    QSqlQuery query(*database);
    query.prepare("CREATE TABLE IF NOT EXISTS \"" + table_name + "\"("
                    "message_id		INTEGER REFERENCES \"" + KMessageModel::table_name + "\"(dbId),"
                    "file_id		INTEGER REFERENCES \"" + KFilesModel::table_name + "\"(id)"
                  ");");
    if (!query.exec())
    {
        qCritical() << "Unable to create files table:" << query.lastError().text();
        return false;
    }
    return true;
}

bool KMessageFileConnectionModel::addConnecion(int message_id, int file_id)
{
    //FIXME: add beginInsertRow etc
    if (noDatabaseMode()) return false;
    QSqlQuery query(*database);
    query.prepare("INSERT INTO :table_name "
                  "(message_id, file_id)"
                  "VALUES"
                  "(:message_id, :file_id)");
    query.bindValue(":table_name", table_name);
    query.bindValue(":message_id", message_id);
    query.bindValue(":file_id", file_id);
    if (!query.exec())
    {
        qCritical() << "No connection added due to the error:" << query.lastError().text();
        return false;
    }
    return true;
}

int KMessageFileConnectionModel::rowCount(const QModelIndex &parent) const
{
    return 0; //FIXME
}

QVariant KMessageFileConnectionModel::data(const QModelIndex &index, int role) const
{
    return QVariant(); //FIXME
}

bool KMessageFileConnectionModel::loadFromDatabase()
{
    //nothing to do
}
