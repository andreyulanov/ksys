#ifndef KMESSAGEMODEL_H
#define KMESSAGEMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QXmppQt5/QXmppClient.h>
#include <QtSql/QSqlDatabase>
#include "kabstractdblistmodel.h"

class KMessageModel : public KAbstractDbListModel
{
    Q_OBJECT
public:
    explicit KMessageModel(QXmppClient* client,
                           QObject *parent = nullptr);
    enum Roles {
        fromRole = Qt::UserRole + 1,
        toRole,
        bodyRole
    };

    virtual int rowCount(const QModelIndex&) const override;
    // Q_INVOKABLE virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
    /// \todo Refactor to get all data in one sql request.
    virtual QVariant data(const QModelIndex &index, int role) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

       /// Sets the client.
    void setClient(QXmppClient*);
public slots:
    bool addMessage(const QXmppMessage&);

protected:
    virtual bool loadFromDatabase() override;
    virtual bool createTable() override;

private:
    QXmppClient* client = nullptr;
    void bindValues(QSqlQuery& query, const QXmppMessage& message) const;
    inline static const QString table_name = "messages";
    /// Gets message by index.
    ///
    /// \todo refactor pointers to std_optional
    QXmppMessage* getMessageByIndex(const QModelIndex&) const;
    /// Loads a message from sql query.
    ///
    /// If query is empty return QXmppMessage()
    QXmppMessage query2Message(const QSqlQuery&) const;
    /// Creates SQL qery to insert a message.
    QSqlQuery message2InsertQuery(const QXmppMessage&);
    inline static const QString columns_values = "("
                   "body,"
                   "e2eeFallbackBody,"
                   "subject,"
                   "thread,"
                   "parentThread,"
                   "type,"
                   "outOfBandUrl,"
                   "xhtml,"
                   "stamp,"
                   "receiptRequested,"
                   "receiptId,"
                   "attentionRequested,"
                   "mucInvitationJid,"
                   "mucInvitationPassword,"
                   "mucInvitationReason,"
                   "private,"
                   "carbonForwarded,"
                   "replaceId,"
                   "markable,"
                   "markerId,"
                   "markedThread,"
                   "marker,"
                   "stanzaId,"
                   "stanzaIdBy,"
                   "originId,"
                   "attachId,"
                   "mixUserJid,"
                   "mixUserNick,"
                   "encryptionMethod,"
                   "encryptionMethodNs,"
                   "encryptionName,"
                   "isSpoiler,"
                   "spoilerHint,"
                   "isFallback"
                   ") VALUES ("
                   ":body,"
                   ":e2eeFallbackBody,"
                   ":subject,"
                   ":thread,"
                   ":parentThread,"
                   ":type,"
                   ":outOfBandUrl,"
                   ":xhtml,"
                   ":stamp,"
                   ":receiptRequested,"
                   ":receiptId,"
                   ":attentionRequested,"
                   ":mucInvitationJid,"
                   ":mucInvitationPassword,"
                   ":mucInvitationReason,"
                   ":private,"
                   ":carbonForwarded,"
                   ":replaceId,"
                   ":markable,"
                   ":markerId,"
                   ":markedThread,"
                   ":marker,"
                   ":stanzaId,"
                   ":stanzaIdBy,"
                   ":originId,"
                   ":attachId,"
                   ":mixUserJid,"
                   ":mixUserNick,"
                   ":encryptionMethod,"
                   ":encryptionMethodNs,"
                   ":encryptionName,"
                   ":isSpoiler,"
                   ":spoilerHint,"
                   ":isFallback"
                   ")";

    class MessageFiller
    {
        QXmppMessage* message = nullptr;
    public:
        MessageFiller(QXmppMessage*);
        enum Field {
            Body,
            E2eeFallbackBody,
            Subject,
            Thread,
            ParentThread,
            Type,
            OutOfBandUrl,
            Xhtml,
            Stamp,
            ReceiptRequested,
            ReceiptId,
            AttentionRequested,
            MucInvitationJid,
            MucInvitationPassword,
            MucInvitationReason,
            Private,
            CarbonForwarded,
            ReplaceId,
            Markable,
            MarkerId,
            MarkedThread,
            Marker,
            StanzaId,
            StanzaIdBy,
            OriginId,
            AttachId,
            MixUserJid,
            MixUserNick,
            EncryptionMethod,
            EncryptionMethodNs,
            EncryptionName,
            IsSpoiler,
            SpoilerHint,
            IsFallback,
            Invalid	//The "Invalid" MUST be the last element in the enum
        };
        QString field2Name(Field);
        Field name2Field(const QString &);
        template<typename T>
        void set(Field, QVariant);
        // template<typename T>
        // T get(Field);
        QXmppMessage *getMessage() const;
        void setMessage(QXmppMessage *newMessage);
        inline static const QString table_name = "messages";
    };
};

#endif // KMESSAGEMODEL_H
