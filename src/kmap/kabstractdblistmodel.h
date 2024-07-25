#ifndef KABSTRACTDBLISTMODEL_H
#define KABSTRACTDBLISTMODEL_H

#include <QAbstractListModel>
#include <QtSql/QSqlDatabase>

/// \brief Abstaract list model that stores data in a database.
class KAbstractDbListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit KAbstractDbListModel(QObject *parent = nullptr);
    /// Sets the databse.
    ///
    ///	Database mast be ready for openning.
    /// If database == nullptr or database can not be opened
    ///  couthen the model will not store data in a database.
    /// The class is usless untill you have set the database.
    void setDatabase(QSqlDatabase*);
    bool noDatabaseMode() const {return (database == nullptr);}

protected:
    /// Loads chats from database and adds them to the manager.
    virtual bool loadFromDatabase() = 0;
    /// Create table of chats in the database.
    virtual bool createTable() = 0;
    QSqlDatabase* database = nullptr;
};

#endif // KABSTRACTDBLISTMODEL_H
