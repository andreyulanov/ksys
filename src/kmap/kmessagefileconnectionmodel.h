#ifndef KMESSAGEFILECONNECTIONMODEL_H
#define KMESSAGEFILECONNECTIONMODEL_H

#include "kabstractdblistmodel.h"
#include <QObject>

/// Usless class.
///
/// The only it's purpose is to create a connection table and add connections.
class KMessageFileConnectionModel : public KAbstractDbListModel
{
    Q_OBJECT
public:
    explicit KMessageFileConnectionModel(QObject *parent = nullptr);
    bool createTable() override;
    bool addConnecion(int message_id, int file_id);
private:
    static const inline QString table_name = "files_messages_connections";

    // QAbstractItemModel interface
public:
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;

    // KAbstractDbListModel interface
protected:
    virtual bool loadFromDatabase() override;
};

#endif // KMESSAGEFILECONNECTIONMODEL_H
