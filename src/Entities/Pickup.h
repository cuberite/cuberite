
#pragma once

#include "Entity.h"
#include "../Item.h"





class cPlayer;





// tolua_begin
class cPickup :
	public cEntity
{
	// tolua_end

	using Super = cEntity;

public:  // tolua_export

	CLASS_PROTODEF(cPickup)

	cPickup(Vector3d a_Pos, const cItem & a_Item, bool IsPlayerCreated, Vector3f a_Speed = Vector3f(), int a_LifetimeTicks = 6000, bool a_CanCombine = true);

	cItem &       GetItem(void)       {return m_Item; }  // tolua_export
	const cItem & GetItem(void) const {return m_Item; }

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	bool CollectedBy(cPlayer & a_Dest);  // tolua_export

	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;

	virtual bool DoesPreventBlockPlacement(void) const override { return false; }

	/** Returns whether this pickup is allowed to combine with other similar pickups */
	bool CanCombine(void) const { return m_bCanCombine; }  // tolua_export

	/** Sets whether this pickup is allowed to combine with other similar pickups */
	void SetCanCombine(bool a_CanCombine) { m_bCanCombine = a_CanCombine; }  // tolua_export

	/** Returns the number of ticks that this entity has existed */
	int GetAge(void) const { return std::chrono::duration_cast<cTickTime>(m_Timer).count(); }     // tolua_export

	/** Set the number of ticks that this entity has existed */
	void SetAge(int a_Age) { m_Timer = cTickTime(a_Age); }  // tolua_export

	/** Returns the number of ticks that this pickup should live for */
	int GetLifetime(void) const { return std::chrono::duration_cast<cTickTime>(m_Lifetime).count(); }  // tolua_export

	/** Set the number of ticks that this pickup should live for */
	void SetLifetime(int a_Lifetime) { m_Lifetime = cTickTime(a_Lifetime); }  // tolua_export

	/** Returns true if the pickup has already been collected */
	bool IsCollected(void) const { return m_bCollected; }  // tolua_export

	/** Returns true if created by player (i.e. vomiting), used for determining picking-up delay time */
	bool IsPlayerCreated(void) const { return m_bIsPlayerCreated; }  // tolua_export

private:

	/** The number of ticks that the entity has existed / timer between collect and destroy; in msec */
	std::chrono::milliseconds m_Timer;

	cItem m_Item;

	bool m_bCollected;

	bool m_bIsPlayerCreated;

	bool m_bCanCombine;

	std::chrono::milliseconds m_Lifetime;
};  // tolua_export
