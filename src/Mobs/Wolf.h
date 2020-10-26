
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

	virtual void InheritFromParents(cMonster * a_Parent1, cMonster * a_Parent2) override;
	virtual void GetBreedingItems(cItems & a_Items) override
	{
		a_Items.Add(E_ITEM_RAW_BEEF);
		a_Items.Add(E_ITEM_STEAK);
		a_Items.Add(E_ITEM_RAW_PORKCHOP);
		a_Items.Add(E_ITEM_COOKED_PORKCHOP);
		a_Items.Add(E_ITEM_RAW_CHICKEN);
		a_Items.Add(E_ITEM_COOKED_CHICKEN);
		a_Items.Add(E_ITEM_RAW_MUTTON);
		a_Items.Add(E_ITEM_COOKED_MUTTON);
		a_Items.Add(E_ITEM_RAW_RABBIT);
		a_Items.Add(E_ITEM_COOKED_RABBIT);
		a_Items.Add(E_ITEM_ROTTEN_FLESH);
	}


protected:

	bool    m_IsSitting;
	bool    m_IsTame;
	bool    m_IsBegging;
	bool    m_IsAngry;
	AString m_OwnerName;
	cUUID   m_OwnerUUID;
	int     m_CollarColor;
	int     m_NotificationCooldown;
} ;




