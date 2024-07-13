#ifndef KMUCMANAGER_H
#define KMUCMANAGER_H
#include <QXmppQt5/QXmppMucManager.h>

/// Wrapper over QXmppMucManager with some utilities
class KMUCManager : public QXmppMucManager
{
public:
    /// Automatic reaction on invitation to MUC
    enum InvitationReaction
    {
        Add = 0x0001, 				///< Add MUC room to the list on an invitation
        // AddFromRoster = 0x0002, ///< Add room if the inviter presents in roster
        IgnoreInvitaion = 0x0000 	///< Do nothing
    };
    enum AddingReaction
    {
        Join = 0x0001, 			///< If room added join it
        IgnoreAdding = 0x0000	///< Do noting
    };
    KMUCManager(InvitationReaction _invitation_reaction = IgnoreInvitaion,
                AddingReaction _adding_reaction = IgnoreAdding);
    void setInvitationReaction(InvitationReaction);
    void setAddingReaction(AddingReaction);
    //void discoveryRoomInfo(const QString& jid);
    /// Searches through the jids of added rooms.
    ///
    /// \return nullptr means no room has been found.
    /// If there are more then one rome with the same jid will be returnd one of them.
    QXmppMucRoom* getRoomByJid(const QString& jid);
private:
    InvitationReaction invitation_reaction;
    AddingReaction adding_reacton;
private slots:
    void slotInvitationReaction(const QString &roomJid, const QString &inviter, const QString &reason);
    void slotRoomAdded(QXmppMucRoom *room);
};

#endif // KMUCMANAGER_H
