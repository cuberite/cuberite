
#pragma once

#include "PassiveAggressiveMonster.h"
#include "../UUID.h"


class cEntity;



class cWolf:
	public cPassiveAggressiveMonster
{
	using Super = cPassiveAggressiveMonster;

public:

	cWolf();

	CLASS_PROTODEF(cWolf)

	void NotifyAlliesOfFight(cPawn * a_Opponent);
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void OnRightClicked(cPlayer & a_Player) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;
	virtual void TickFollowPlayer();
	virtual bool Attack(std::chrono::milliseconds a_Dt) override;

	// Get functions
	bool    IsSitting     (void) const override { return m_IsSitting; }
	bool    IsTame        (void) const override { return m_IsTame; }
	bool    IsBegging     (void) const { return m_IsBegging; }
	bool    IsAngry       (void) const { return m_IsAngry; }
	AString GetOwnerName  (void) const { return m_OwnerName; }
	cUUID   GetOwnerUUID  (void) const { return m_OwnerUUID; }
	int     GetCollarColor(void) const { return m_CollarColor; }

	// Set functions
	void SetIsSitting  (bool a_IsSitting)   { m_IsSitting = a_IsSitting; }
	void SetIsTame     (bool a_IsTame)      { m_IsTame = a_IsTame; }
	void SetIsBegging  (bool a_IsBegging)   { m_IsBegging = a_IsBegging; }
	void SetIsAngry    (bool a_IsAngry)     { m_IsAngry = a_IsAngry; }
	void SetCollarColor(int a_CollarColor)  { m_CollarColor = a_CollarColor; }
	void SetOwner      (const AString & a_NewOwnerName, const cUUID & a_NewOwnerUUID)
	{
		m_OwnerName = a_NewOwnerName;
		m_OwnerUUID = a_NewOwnerUUID;
	}

	/** Notfies the wolf of a nearby fight.
	The wolf may then decide to attack a_Opponent.
	If a_IsPlayerInvolved is true, then the player whose UUID is a_PlayerUUID is fighting a_Opponent
	If false, then a wolf owned by the player whose UUID is a_PlayerUUID is fighting a_Opponent
	@param a_PlayerUUID The UUID of the fighting player, or the UUID of the owner whose wolf is fighting.
	@param a_Opponent The opponent who is being faught.
	@param a_IsPlayerInvolved Whether the fighter a player or a wolf. */
	void ReceiveNearbyFightInfo(const cUUID & a_PlayerUUID, cPawn * a_Opponent, bool a_IsPlayerInvolved);

	virtual void InStateIdle(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	/** Called after the baby is born, allows the baby to inherit the parents' properties (color, etc.) */
	virtual void InheritFromParents(cWolf & a_Parent1, cWolf & a_Parent2);

	/** Returns the partner which the monster is currently mating with. */
	cWolf * GetPartner(void) const { return m_LovePartner; }

	/** Start the mating process. Causes the monster to keep bumping into the partner until m_MatingTimer reaches zero. */
	void EngageLoveMode(cWolf * a_Partner);

	/** Finish the mating process. Called after a baby is born. Resets all breeding related timers and sets m_LoveCooldown to 20 minutes. */
	void ResetLoveMode();

	/** Returns whether the monster has just been fed and is ready to mate. If this is "true" and GetPartner isn't "nullptr", then the monster is mating. */
	bool IsInLove() const { return (m_LoveTimer > 0); }

	/** Returns whether the monster is tired of breeding and is in the cooldown state. */
	bool IsInLoveCooldown() const { return (m_LoveCooldown > 0); }

protected:

	bool    m_IsSitting;
	bool    m_IsTame;
	bool    m_IsBegging;
	bool    m_IsAngry;
	AString m_OwnerName;
	cUUID   m_OwnerUUID;
	int     m_CollarColor;
	int     m_NotificationCooldown;

	/** The monster's breeding partner. */
	cWolf * m_LovePartner;

	/** If above 0, the monster is in love mode, and will breed if a nearby monster is also in love mode. Decrements by 1 per tick till reaching zero. */
	int m_LoveTimer;

	/** If above 0, the monster is in cooldown mode and will refuse to breed. Decrements by 1 per tick till reaching zero. */
	int m_LoveCooldown;

	/** The monster is engaged in mating, once this reaches zero, a baby will be born. Decrements by 1 per tick till reaching zero, then a baby is made and ResetLoveMode() is called. */
	int m_MatingTimer;
} ;




