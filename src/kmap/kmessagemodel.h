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
        inline static const QMap<Field, QString> field_2_name_map{
            {Body,                  "body"},
            {E2eeFallbackBody,      "e2eeFallbackBody"},
            {Subject,               "subject"},
            {Thread,                "thread"},
            {ParentThread,          "parentThread"},
            {Type,                  "type"},
            {OutOfBandUrl,          "outOfBandUrl"},
            {Xhtml,                 "xhtml"},
            {Stamp,                 "stamp"},
            {ReceiptRequested,      "receiptRequested"},
            {ReceiptId,             "receiptId"},
            {AttentionRequested,    "attentionRequested"},
            {MucInvitationJid,      "mucInvitationJid"},
            {MucInvitationPassword, "mucInvitationPassword"},
            {MucInvitationReason,   "mucInvitationReason"},
            {Private,               "private"},
            {CarbonForwarded,       "carbonForwarded"},
            {ReplaceId,             "replaceId"},
            {Markable,              "markable"},
            {MarkerId,              "markerId"},
            {MarkedThread,          "markedThread"},
            {Marker,                "marker"},
            {StanzaId,              "stanzaId"},
            {StanzaIdBy,            "stanzaIdBy"},
            {OriginId,              "originId"},
            {AttachId,              "attachId"},
            {MixUserJid,            "mixUserJid"},
            {MixUserNick,           "mixUserNick"},
            {EncryptionMethod,      "encryptionMethod"},
            {EncryptionMethodNs,    "encryptionMethodNs"},
            {EncryptionName,        "encryptionName"},
            {IsSpoiler,             "isSpoiler"},
            {SpoilerHint,           "spoilerHint"},
            {IsFallback,            "isFallback"},
        };
        inline static const QMap<QString, Field> name_2_field_map{
            {"body",						 Body},
            {"e2eeFallbackBody",			 E2eeFallbackBody},
            {"subject",						 Subject},
            {"thread",						 Thread},
            {"parentThread",				 ParentThread},
            {"type",						 Type},
            {"outOfBandUrl",				 OutOfBandUrl},
            {"xhtml",						 Xhtml},
            {"stamp",						 Stamp},
            {"receiptRequested",			 ReceiptRequested},
            {"receiptId",					 ReceiptId},
            {"attentionRequested",			 AttentionRequested},
            {"mucInvitationJid",			 MucInvitationJid},
            {"mucInvitationPassword",		 MucInvitationPassword},
            {"mucInvitationReason",			 MucInvitationReason},
            {"private",						 Private},
            {"carbonForwarded",				 CarbonForwarded},
            {"replaceId",					 ReplaceId},
            {"markable",					 Markable},
            {"markerId",					 MarkerId},
            {"markedThread",				 MarkedThread},
            {"marker",						 Marker},
            {"stanzaId",					 StanzaId},
            {"stanzaIdBy",					 StanzaIdBy},
            {"originId",					 OriginId},
            {"attachId",					 AttachId},
            {"mixUserJid",					 MixUserJid},
            {"mixUserNick",					 MixUserNick},
            {"encryptionMethod",			 EncryptionMethod},
            {"encryptionMethodNs",			 EncryptionMethodNs},
            {"encryptionName",				 EncryptionName},
            {"isSpoiler",					 IsSpoiler},
            {"spoilerHint",					 SpoilerHint},
            {"isFallback",					 IsFallback},
        };
    };
};

#endif // KMESSAGEMODEL_H
