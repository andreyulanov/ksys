#include "kmessagemodel.h"
#include <QXmppQt5/QXmppMessage.h>
#include <QSqlQuery>
#include <QSqlError>

QSqlQuery KMessageModel::message2InsertQuery(const QXmppMessage &message)
{
    QSqlQuery query(*database);
    query.prepare("INSERT INTO " + table_name + columns_values);
    bindValues(query, message);
    return query;
}

void KMessageModel::bindValues(QSqlQuery& query, const QXmppMessage& message) const
{
    query.bindValue(":body", message.body());
    query.bindValue(":e2eeFallbackBody", message.e2eeFallbackBody());
    query.bindValue(":subject", message.subject());
    query.bindValue(":thread", message.thread());
    query.bindValue(":parentThread", message.parentThread());
    query.bindValue(":type", message.type());
    query.bindValue(":outOfBandUrl", message.outOfBandUrl());
    query.bindValue(":xhtml", message.xhtml());
    query.bindValue(":stamp", message.stamp());
    query.bindValue(":receiptRequested", message.isReceiptRequested());
    query.bindValue(":receiptId", message.receiptId());
    query.bindValue(":attentionRequested", message.isAttentionRequested());
    query.bindValue(":mucInvitationJid", message.mucInvitationJid());
    query.bindValue(":mucInvitationPassword", message.mucInvitationPassword());
    query.bindValue(":mucInvitationReason", message.mucInvitationReason());
    query.bindValue(":private", message.isPrivate());
    query.bindValue(":carbonForwarded", message.isCarbonForwarded());
    query.bindValue(":replaceId", message.replaceId());
    query.bindValue(":markable", message.isMarkable());
    query.bindValue(":markerId", message.markedId());
    query.bindValue(":markedThread", message.markedThread());
    query.bindValue(":marker", message.marker());
    query.bindValue(":stanzaId", message.stanzaId());
    query.bindValue(":stanzaIdBy", message.stanzaIdBy());
    query.bindValue(":originId", message.originId());
    query.bindValue(":attachId", message.attachId());
    query.bindValue(":mixUserJid", message.mixUserJid());
    query.bindValue(":mixUserNick", message.mixUserNick());
    query.bindValue(":encryptionMethod", message.encryptionMethod());
    query.bindValue(":encryptionMethodNs", message.encryptionMethodNs());
    query.bindValue(":encryptionName", message.encryptionName());
    query.bindValue(":isSpoiler", message.isSpoiler());
    query.bindValue(":spoilerHint", message.spoilerHint());
    query.bindValue(":isFallback", message.isFallback());

}

QXmppMessage *KMessageModel::MessageFiller::getMessage() const
{
    return message;
}

void KMessageModel::MessageFiller::setMessage(QXmppMessage *newMessage)
{
    message = newMessage;
}

QString KMessageModel::MessageFiller::field2Name(Field field)
{
    switch (field) {
        case Body:      return "body";
        case E2eeFallbackBody:  return "e2eeFallbackBody";
        case Subject:   return "subject";
        case Thread:    return "thread";
        case ParentThread:      return "parentThread";
        case Type:      return "type";
        case OutOfBandUrl:      return "outOfBandUrl";
        case Xhtml:     return "xhtml";
        case Stamp:     return "stamp";
        case ReceiptRequested:  return "receiptRequested";
        case ReceiptId: return "receiptId";
        case AttentionRequested:        return "attentionRequested";
        case MucInvitationJid:  return "mucInvitationJid";
        case MucInvitationPassword:     return "mucInvitationPassword";
        case MucInvitationReason:       return "mucInvitationReason";
        case Private:   return "private";
        case CarbonForwarded:   return "carbonForwarded";
        case ReplaceId: return "replaceId";
        case Markable:  return "markable";
        case MarkerId:  return "markerId";
        case MarkedThread:      return "markedThread";
        case Marker:    return "marker";
        case StanzaId:  return "stanzaId";
        case StanzaIdBy:        return "stanzaIdBy";
        case OriginId:  return "originId";
        case AttachId:  return "attachId";
        case MixUserJid:        return "mixUserJid";
        case MixUserNick:       return "mixUserNick";
        case EncryptionMethod:  return "encryptionMethod";
        case EncryptionMethodNs:        return "encryptionMethodNs";
        case EncryptionName:    return "encryptionName";
        case IsSpoiler: return "isSpoiler";
        case SpoilerHint:       return "spoilerHint";
        case IsFallback:        return "isFallback";
    default: //impossible
        qWarning() << "Unknown field";
        return QString();
    }
}

KMessageModel::MessageFiller::Field KMessageModel::MessageFiller::name2Field(const QString& name)
{
    if (name == "body") return Body;
    if (name == "e2eeFallbackBody") return E2eeFallbackBody;
    if (name == "subject") return Subject;
    if (name == "thread") return Thread;
    if (name == "parentThread") return ParentThread;
    if (name == "type") return Type;
    if (name == "outOfBandUrl") return OutOfBandUrl;
    if (name == "xhtml") return Xhtml;
    if (name == "stamp") return Stamp;
    if (name == "receiptRequested") return ReceiptRequested;
    if (name == "receiptId") return ReceiptId;
    if (name == "attentionRequested") return AttentionRequested;
    if (name == "mucInvitationJid") return MucInvitationJid;
    if (name == "mucInvitationPassword") return MucInvitationPassword;
    if (name == "mucInvitationReason") return MucInvitationReason;
    if (name == "private") return Private;
    if (name == "carbonForwarded") return CarbonForwarded;
    if (name == "replaceId") return ReplaceId;
    if (name == "markable") return Markable;
    if (name == "markerId") return MarkerId;
    if (name == "markedThread") return MarkedThread;
    if (name == "marker") return Marker;
    if (name == "stanzaId") return StanzaId;
    if (name == "stanzaIdBy") return StanzaIdBy;
    if (name == "originId") return OriginId;
    if (name == "attachId") return AttachId;
    if (name == "mixUserJid") return MixUserJid;
    if (name == "mixUserNick") return MixUserNick;
    if (name == "encryptionMethod") return EncryptionMethod;
    if (name == "encryptionMethodNs") return EncryptionMethodNs;
    if (name == "encryptionName") return EncryptionName;
    if (name == "isSpoiler") return IsSpoiler;
    if (name == "spoilerHint") return SpoilerHint;
    if (name == "isFallback") return IsFallback;
    qWarning() << "Unknown field name" << name;
    return Invalid;
}

KMessageModel::MessageFiller::MessageFiller(QXmppMessage *message)
{
    setMessage(message);
}

// template<typename T>
// T KMessageModel::MessageFiller::get(Field field)
// {
//     switch (field)
//     {
//         case Body: return message->body();
//         case E2eeFallbackBody: return message->e2eeFallbackBody();
//         case Subject: return message->subject();
//         case Thread: return message->thread();
//         case ParentThread: return message->parentThread();
//         case Type: return message->type();
//         case OutOfBandUrl: return message->outOfBandUrl();
//         case Xhtml: return message->xhtml();
//         case Stamp: return message->stamp();
//         case ReceiptRequested: return message->isReceiptRequested();
//         case ReceiptId: return message->receiptId();
//         case AttentionRequested: return message->isAttentionRequested();
//         case MucInvitationJid: return message->mucInvitationJid();
//         case MucInvitationPassword: return message->mucInvitationPassword();
//         case MucInvitationReason: return message->mucInvitationReason();
//         case Private: return message->isPrivate();
//         case CarbonForwarded: return message->isCarbonForwarded();
//         case ReplaceId: return message->replaceId();
//         case Markable: return message->isMarkable();
//         case MarkerId: return message->markedId();
//         case MarkedThread: return message->markedThread();
//         case Marker: return message->marker();
//         case StanzaId: return message->stanzaId();
//         case StanzaIdBy: return message->stanzaIdBy();
//         case OriginId: return message->originId();
//         case AttachId: return message->attachId();
//         case MixUserJid: return message->mixUserJid();
//         case MixUserNick: return message->mixUserNick();
//         case EncryptionMethod: return message->encryptionMethod();
//         case EncryptionMethodNs: return message->encryptionMethodNs();
//         case EncryptionName: return message->encryptionName();
//         case IsSpoiler: return message->isSpoiler();
//         case SpoilerHint: return message->spoilerHint();
//         case IsFallback: return message->isFallback();
//         default:
//         qWarning() << "Trying to get Invalid value";
//     }
// }

template<>
void KMessageModel::MessageFiller::set<QString>(Field field, QVariant qvariant)
{
    if (!qvariant.isValid() || qvariant.isNull() || qvariant.canConvert<QString>()) return;
    QString value(qvariant.value<QString>());
    switch (field) {
    case Body: return message->setBody(value); break;
    case E2eeFallbackBody: return message->setE2eeFallbackBody(value); break;
    case Subject: return message->setSubject(value); break;
    case Thread: return message->setThread(value); break;
    case ParentThread: return message->setParentThread(value); break;
    case OutOfBandUrl: return message->setOutOfBandUrl(value); break;
    case Xhtml: return message->setXhtml(value); break;
    case ReceiptId: return message->setReceiptId(value); break;
    case MucInvitationJid: return message->setMucInvitationJid(value); break;
    case MucInvitationPassword: return message->setMucInvitationPassword(value); break;
    case MucInvitationReason: return message->setMucInvitationReason(value); break;
    case ReplaceId: return message->setReplaceId(value); break;
    case MarkerId: return message->setMarkerId(value); break;
    case MarkedThread: return message->setMarkedThread(value); break;
    case StanzaId: return message->setStanzaId(value); break;
    case StanzaIdBy: return message->setStanzaIdBy(value); break;
    case OriginId: return message->setOriginId(value); break;
    case AttachId: return message->setAttachId(value); break;
    case MixUserJid: return message->setMixUserJid(value); break;
    case MixUserNick: return message->setMixUserNick(value); break;
    case EncryptionMethodNs: return message->setEncryptionMethodNs(value); break;
    case EncryptionName: return message->setEncryptionName(value); break;
    case SpoilerHint: return message->setSpoilerHint(value); break;
    default:
        qWarning() << "Trying to set Invalid value or invalid type";
    }
}

template<>
void KMessageModel::MessageFiller::set<bool>(Field field, QVariant qvariant)
{
    if (!qvariant.isValid() || qvariant.isNull() || qvariant.canConvert<bool>()) return;
    bool value = qvariant.value<bool>();
    switch (field) {
    case ReceiptRequested: return message->setReceiptRequested(value); break;
    case AttentionRequested: return message->setAttentionRequested(value); break;
    case Private: return message->setPrivate(value); break;
    case CarbonForwarded: return message->setCarbonForwarded(value); break;
    case Markable: return message->setMarkable(value); break;
    case IsSpoiler: return message->setIsSpoiler(value); break;
    case IsFallback: return message->setIsFallback(value); break;
    default:
        qWarning() << "Trying to set Invalid value or invalid type";
    }
}
template<>
void KMessageModel::MessageFiller::set<QDateTime>(Field field, QVariant qvariant)
{
    if (!qvariant.isValid() || qvariant.isNull() || qvariant.canConvert<int>()) return;
    QDateTime dt;
    dt.setTime_t(qvariant.value<int>());
    switch (field) {
    case Stamp: return message->setStamp(dt); break;
    default:
        qWarning() << "Trying to set Invalid value or invalid type";
    }
}
template<>
void KMessageModel::MessageFiller::set<QXmppMessage::EncryptionMethod>(Field field, QVariant qvariant)
{
    if (!qvariant.isValid() || qvariant.isNull() || qvariant.canConvert<int>()) return;
    QXmppMessage::EncryptionMethod value = static_cast<QXmppMessage::EncryptionMethod>(qvariant.value<int>());
    switch (field) {
        case EncryptionMethod: return message->setEncryptionMethod(value); break;
    default:
        qWarning() << "Trying to set Invalid value or invalid type";
    }
}

template<>
void KMessageModel::MessageFiller::set<QXmppMessage::Marker>(Field field, QVariant qvariant)
{
    if (!qvariant.isValid() || qvariant.isNull() || qvariant.canConvert<int>()) return;
    QXmppMessage::Marker value = static_cast<QXmppMessage::Marker>(qvariant.value<int>());
    switch (field) {
        case Marker: return message->setMarker(value); break;
    default:
        qWarning() << "Trying to set Invalid value or invalid type";
    }
}

template<>
void KMessageModel::MessageFiller::set<QXmppMessage::Type>(Field field, QVariant qvariant)
{
    if (!qvariant.isValid() || qvariant.isNull() || qvariant.canConvert<int>()) return;
    QXmppMessage::Type value = static_cast<QXmppMessage::Type>(qvariant.value<int>());
    switch (field) {
        case Type: return message->setType(value); break;
    default:
        qWarning() << "Trying to set Invalid value or invalid type";
    }
}
KMessageModel::KMessageModel(QXmppClient* client,
                           QObject *parent)
    : KAbstractDbListModel{parent}
{
    setClient(client);
}

QVariant KMessageModel::data(const QModelIndex& index, int role) const
{
    QXmppMessage* message = getMessageByIndex(index);
    if (message == nullptr)
    {
        qWarning() << "Got nullptr message";
        return QVariant();
    }
    QVariant result;
    switch (role) {
    case fromRole:
        result = QVariant(message->from());
    case toRole:
        result = QVariant(message->to());
    case bodyRole:
        result = QVariant(message->body());
    default:
        qWarning() << "KMessageModel::data: Invalid role";
        result = QVariant();
    }
    delete message;
    return result;
}

int KMessageModel::rowCount(const QModelIndex&) const
{
    QSqlQuery query(*database);
    query.prepare("SELECT COUNT(1) FROM" + table_name);
    if (!query.exec())
    {
        qCritical() << "Unable to count rows" << query.lastError().text();
        return 0;
    }
    return 0;
}

QHash<int, QByteArray> KMessageModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[fromRole]	= "from";
    roles[toRole] 	= "to";
    roles[bodyRole]	= "body";
    //TODO add more roles
    return roles;
}

void KMessageModel::setClient(QXmppClient* _client)
{
    if (client != nullptr)
    {
        disconnect(client, &QXmppClient::messageReceived,
                   this, &KMessageModel::addMessage);
    }
    client = _client;
    if (client != nullptr)
    {
        connect(client, &QXmppClient::messageReceived,
                this, &KMessageModel::addMessage);
    }
}

bool KMessageModel::addMessage(const QXmppMessage& message)
{
    QSqlQuery query = message2InsertQuery(message);
    if (!query.exec())
    {
        qWarning() << "Failed to add a message:" << query.lastError().text();
        return false;
    }
    return true;
}

bool KMessageModel::loadFromDatabase()
{
    return false; //FIXME
}

bool KMessageModel::createTable()
{
    if (noDatabaseMode()) return false;
    QSqlQuery query(*database);
    query.prepare("CREATE TABLE IF NOT EXISTS\"" + table_name + "\" ("
                    "dbId 					INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,"
                    "Body                   TEXT,"
                    "E2eeFallbackBody       TEXT,"
                    "Subject                TEXT,"
                    "Thread                 TEXT,"
                    "ParentThread           TEXT,"
                    "Type                   INTEGER,"
                    "OutOfBandUrl           TEXT,"
                    "Xhtml                  TEXT,"
                    "Stamp                  TEXT,"
                    "ReceiptRequested       INTEGER,"
                    "ReceiptId              TEXT,"
                    "AttentionRequested     INTEGER,"
                    "MucInvitationJid       TEXT,"
                    "MucInvitationPassword  TEXT,"
                    "MucInvitationReason    TEXT,"
                    "Private                INTEGER,"
                    "CarbonForwarded        TEXT,"
                    "ReplaceId              TEXT,"
                    "Markable               INTEGER,"
                    "MarkerId               TEXT,"
                    "MarkedThread           TEXT,"
                    "Marker                 INTEGER,"
                    "StanzaId               TEXT,"
                    "StanzaIdBy             TEXT,"
                    "OriginId               TEXT,"
                    "AttachId               TEXT,"
                    "MixUserJid             TEXT,"
                    "MixUserNick            TEXT,"
                    "EncryptionMethod       INTEGER,"
                    "EncryptionMethodNs     TEXT,"
                    "EncryptionName         TEXT,"
                    "IsSpoiler              INTEGER,"
                    "SpoilerHint            TEXT,"
                    "IsFallback             INTEGER"
                    ")");
    if (!query.exec())
    {
        qCritical() << "Unable to create messages table:" << query.lastError().text();
        return false;
    }
    return true;
}

QXmppMessage* KMessageModel::getMessageByIndex(const QModelIndex& index) const
{
    if (!index.isValid()) return nullptr;
    QSqlQuery query(*database);
    int dbId = index.row();
    query.prepare("SELECT * FROM" + table_name +
                  "WHERE dbId = :dbId");
    query.bindValue(":dbId",dbId);
    if (!query.exec())
    {
        qWarning() << "Unable to load a message:" << query.lastError().text();
        return nullptr;
    }
    if (query.next())
    {
        return new QXmppMessage(query2Message(query));
    }
    else
    {
        qWarning() << "Can't find a message with dbId" << dbId;
        return nullptr;
    }
    return nullptr; // unreachable
}

QXmppMessage KMessageModel::query2Message(const QSqlQuery& query) const
{
    QXmppMessage message;
    KMessageModel::MessageFiller filler(&message);
    filler.set<QString>	(KMessageModel::MessageFiller::Body, query.value("body"));
    filler.set<QString>	(KMessageModel::MessageFiller::E2eeFallbackBody, query.value("e2eeFallbackBody"));
    filler.set<QString>	(KMessageModel::MessageFiller::Subject, query.value("subject"));
    filler.set<QString>	(KMessageModel::MessageFiller::Thread, query.value("thread"));
    filler.set<QString>	(KMessageModel::MessageFiller::ParentThread, query.value("parentThread"));
    filler.set<QXmppMessage::Type>	(KMessageModel::MessageFiller::Type, query.value("type"));
    filler.set<QString>	(KMessageModel::MessageFiller::OutOfBandUrl, query.value("outOfBandUrl"));
    filler.set<QString>	(KMessageModel::MessageFiller::Xhtml, query.value("xhtml"));
    filler.set<QString>	(KMessageModel::MessageFiller::Stamp, query.value("stamp"));
    filler.set<bool>	(KMessageModel::MessageFiller::ReceiptRequested, query.value("receiptRequested"));
    filler.set<QString>	(KMessageModel::MessageFiller::ReceiptId, query.value("receiptId"));
    filler.set<bool>	(KMessageModel::MessageFiller::AttentionRequested, query.value("attentionRequested"));
    filler.set<QString>	(KMessageModel::MessageFiller::MucInvitationJid, query.value("mucInvitationJid"));
    filler.set<QString>	(KMessageModel::MessageFiller::MucInvitationPassword, query.value("mucInvitationPassword"));
    filler.set<QString>	(KMessageModel::MessageFiller::MucInvitationReason, query.value("mucInvitationReason"));
    filler.set<bool>	(KMessageModel::MessageFiller::Private, query.value("private"));
    filler.set<QString>	(KMessageModel::MessageFiller::CarbonForwarded, query.value("carbonForwarded"));
    filler.set<QString>	(KMessageModel::MessageFiller::ReplaceId, query.value("replaceId"));
    filler.set<bool>	(KMessageModel::MessageFiller::Markable, query.value("markable"));
    filler.set<QString>	(KMessageModel::MessageFiller::MarkerId, query.value("markerId"));
    filler.set<QString>	(KMessageModel::MessageFiller::MarkedThread, query.value("markedThread"));
    filler.set<QXmppMessage::Marker>	(KMessageModel::MessageFiller::Marker, query.value("marker"));
    filler.set<QString>	(KMessageModel::MessageFiller::StanzaId, query.value("stanzaId"));
    filler.set<QString>	(KMessageModel::MessageFiller::StanzaIdBy, query.value("stanzaIdBy"));
    filler.set<QString>	(KMessageModel::MessageFiller::OriginId, query.value("originId"));
    filler.set<QString>	(KMessageModel::MessageFiller::AttachId, query.value("attachId"));
    filler.set<QString>	(KMessageModel::MessageFiller::MixUserJid, query.value("mixUserJid"));
    filler.set<QString>	(KMessageModel::MessageFiller::MixUserNick, query.value("mixUserNick"));
    filler.set<QXmppMessage::EncryptionMethod>	(KMessageModel::MessageFiller::EncryptionMethod, query.value("encryptionMethod"));
    filler.set<QString>	(KMessageModel::MessageFiller::EncryptionMethodNs, query.value("encryptionMethodNs"));
    filler.set<QString>	(KMessageModel::MessageFiller::EncryptionName, query.value("encryptionName"));
    filler.set<bool>	(KMessageModel::MessageFiller::IsSpoiler, query.value("isSpoiler"));
    filler.set<QString>	(KMessageModel::MessageFiller::SpoilerHint, query.value("spoilerHint"));
    filler.set<bool>	(KMessageModel::MessageFiller::IsFallback, query.value("isFallback"));
    //****************************************************************************************************//
    // message.setMixInvitation(const std::optional< QXmppMixInvitation > &mixInvitation)
    // message.setTrustMessageElement(const std::optional< QXmppTrustMessageElement > &trustMessageElement)
    // message.setReaction(const std::optional< QXmppMessageReaction > &reaction)
    // message.setSharedFiles(const QVector< QXmppFileShare > &sharedFiles)
    // message.setOutOfBandUrls(const QVector< QXmppOutOfBandUrl > &urls)
    // message.setBitsOfBinaryData(const QXmppBitsOfBinaryDataList &bitsOfBinaryData)
    return message;
}
