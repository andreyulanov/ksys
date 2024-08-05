#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QModelIndex>
#include <QXmppQt5/QXmppHttpFileSource.h>
#include "kfilesmodel.h"

KFilesModel::KFilesModel(QObject *parent)
    : KAbstractDbListModel{parent}
{
    //nothing to do...
}

int KFilesModel::rowCount(const QModelIndex&) const
{
    return index2db_id.size();
}

QVariant KFilesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !index2db_id.contains(index)) return QVariant();
    return data(index2db_id[index], role);
}
QVariant KFilesModel::data(int id, int role) const
{
    QString column_name;
    switch (role) {
    case dataRole:	column_name = "data"; break;
    case descriptionRole: column_name = "description"; break;
    case httpSourceRole: column_name = "httpSource"; break;
    default: return QVariant();
    };

    QSqlQuery query(*database);
    query.prepare("SELECT :column_name FROM :table_name"
                  "WHERE id = :id");
    query.bindValue(":column_name", column_name);
    query.bindValue(":table_name", table_name);
    query.bindValue(":id", id);
    if (query.exec())
    {
        qCritical() << "Failed to load file:" << query.lastError().text();
        return QVariant();
    }
    if (query.next())
    {
        return query.value(column_name);
    }
    qWarning() << "Can't find a file with id" << id;
    return QVariant();
}

QHash<int, QByteArray> KFilesModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[descriptionRole]	= "description";
    roles[dataRole] 		= "data";
    roles[dbIdRole]			= "dbId";
    return roles;
}

bool KFilesModel::loadFromDatabase()
{
    index2db_id.clear();
    if (noDatabaseMode())
    {
        qWarning() << "No data have been loaded due to the noDatabaseMode";
        return false;
    }
    // Load from db
    QSqlQuery query(*database);
    query.prepare("SELECT id FROM \"" + table_name + "\"");
    if (!query.exec())
    {
        qCritical() << "Unable to load files table:" << query.lastError().text();
        return false;
    }
    for (int row = 0; query.next(); row++)
    {
        index2db_id.insert(index(row), query.value("id").toInt());
    }
    return true;
}

bool KFilesModel::createTable()
{
    if (noDatabaseMode())
    {
        qWarning() << "No table have been created due to the noDatabaseMode";
        return false;
    }
    QSqlQuery query(*database);
    query.prepare("CREATE TABLE IF NOT EXISTS \"" + table_name + "\"("
                    "\"id\"						INTEGER PRIMARY KEY NOT NULL UNIQUE,"
                    "\"descritption\"			TEXT,"
                    "\"httpSource\"				TEXT,"
                    "\"data\"					BLOB"
                    ")");
    if (!query.exec())
    {
        qCritical() << "Unable to create files table:" << query.lastError().text();
        return false;
    }
    return true;
}


bool KFilesModel::saveSharedFile(const QXmppFileShare &file_share, const QBitArray &data)
{
    int row_to_add = rowCount(QModelIndex());
    beginInsertRows(QModelIndex(), row_to_add, row_to_add);
    if (noDatabaseMode())
    {
        qWarning() << "No table have been created due to the noDatabaseMode";
        endInsertRows();
        return false;
    }
    QSqlQuery query(*database);
    query.prepare("INSERT INTO TABLE :table_name "
                  "(descritption, data, httpSource)"
                  "VALUES"
                  "(:descritption, :data, :httpSource);"
    );
    query.bindValue(":table_name", table_name);
    query.bindValue(":description", file_share.disposition());
    if (file_share.httpSources().isEmpty())
        query.bindValue(":httpSource", QVariant()); // set NULL source
    else
        query.bindValue(":httpSource", file_share.httpSources().first().url());
    query.bindValue(":data", data);
    if (!query.exec())
    {
        qCritical() << "Unable to insert a row into files table:" << query.lastError().text();
        endInsertRows();
        return false;
    }
    QSqlQuery id_query("SELECT last_insert_rowid()", *database);
    if (!id_query.exec())
    {
        qCritical() << "Can't get last insert row id:" << query.lastError().text();
        endInsertRows();
        return false;
    }
    int row = rowCount(QModelIndex());
    int id = id_query.value(0).toInt();
    index2db_id.insert(index(row), id);
    endInsertRows();
    return true;
}
