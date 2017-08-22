
#pragma once

#include "PassiveAggressiveMonster.h"


class cEntity;



class cWolf :
    public cPassiveAggressiveMonster
{
    typedef cPassiveAggressiveMonster super;

public:
    cWolf(void);

    CLASS_PROTODEF(cWolf)

    void NotifyAlliesOfFight(cPawn * a_Opponent);
    virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
    virtual void OnRightClicked(cPlayer & a_Player) override;
    virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
    virtual void TickFollowPlayer();
    //virtual bool Attack(std::chrono::milliseconds a_Dt) override;

    // Get functions
    bool    IsSitting     (void) const override { return m_IsSitting; }
    bool    IsTame        (void) const override { return m_IsTame; }
    bool    IsBegging     (void) const { return m_IsBegging; }
    bool    IsAngry       (void) const { return m_IsAngry; }
    AString GetOwnerName  (void) const { return m_OwnerName; }
    AString GetOwnerUUID  (void) const { return m_OwnerUUID; }
    int     GetCollarColor(void) const { return m_CollarColor; }

    // Set functions
    void SetIsSitting  (bool a_IsSitting)   { m_IsSitting = a_IsSitting; }
    void SetIsTame     (bool a_IsTame)      { m_IsTame = a_IsTame; }
    void SetIsBegging  (bool a_IsBegging)   { m_IsBegging = a_IsBegging; }
    void SetIsAngry    (bool a_IsAngry)     { m_IsAngry = a_IsAngry; }
    void SetCollarColor(int a_CollarColor)  { m_CollarColor = a_CollarColor; }
    void SetOwner      (const AString & a_NewOwnerName, const AString & a_NewOwnerUUID)
    {
        m_OwnerName = a_NewOwnerName;
        m_OwnerUUID = a_NewOwnerUUID;
    }

    /** Notfies the wolf of a nearby fight.
    The wolf may then decide to attack a_Opponent.
    If a_IsPlayer is true, then the player whose ID is a_PlayerID is fighting a_Opponent
    If false, then a wolf owned by the player whose ID is a_PlayerID is fighting a_Opponent
    @param a_PlayerID The ID of the fighting player, or the ID of the owner whose wolf is fighting.
    @param a_Opponent The opponent who is being faught.
    @param a_IsPlayerInvolved Whether the fighter a player or a wolf. */
    void ReceiveNearbyFightInfo(AString a_PlayerID, cPawn * a_Opponent, bool a_IsPlayerInvolved);

    virtual void InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

protected:

    bool    m_IsSitting;
    bool    m_IsTame;
    bool    m_IsBegging;
    bool    m_IsAngry;
    AString m_OwnerName;
    AString m_OwnerUUID;
    int     m_CollarColor;
    int     m_NotificationCooldown;
} ;




