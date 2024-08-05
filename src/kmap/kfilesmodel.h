#ifndef KFILESMODEL_H
#define KFILESMODEL_H

#include <QXmppQt5/QXmppFileShare.h>
#include "kabstractdblistmodel.h"

class KMessageFileConnectionModel;

/// Models stores all downloaded files.
///
/// Evey file in this model is ready for work.
/// Only the first httpSource is stored in database.
class KFilesModel : public KAbstractDbListModel
{
    Q_OBJECT
public:
    explicit KFilesModel(QObject *parent = nullptr);
    enum Roles {
       dataRole = Qt::UserRole + 1,
       dbIdRole,
       descriptionRole,
       httpSourceRole
    };

    // QAbstractItemModel interface
    virtual int rowCount(const QModelIndex &parent) const override;
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QVariant data(int id, int role) const;
    virtual QHash<int, QByteArray> roleNames() const override;

protected:
    // KAbstractDbListModel interface
    bool loadFromDatabase() override;
    bool createTable() override;
    static const inline QString table_name = "files";

private:
    QHash<QModelIndex, int> index2db_id;
    bool saveSharedFile(const QXmppFileShare&, const QBitArray&);

friend KMessageFileConnectionModel;
};


#endif // KFILESMODEL_H
