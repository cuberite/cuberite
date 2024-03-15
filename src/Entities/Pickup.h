
#pragma once

#include "Entity.h"
#include "../Item.h"





class cPawn;





// tolua_begin
class cPickup :
	public cEntity
{
	// tolua_end

	using Super = cEntity;

public:  // tolua_export

	CLASS_PROTODEF(cPickup)

	cPickup(Vector3d a_Position, cItem && a_Item, Vector3d a_Speed, cTickTime a_CollectionDelay, cTickTime a_Lifetime);

	cItem &       GetItem(void)       {return m_Item; }  // tolua_export
	const cItem & GetItem(void) const {return m_Item; }

	bool CollectedBy(cEntity & a_Dest);  // tolua_export

	static bool TryCombineWithQueuedEntity(cEntity & a_Entity, const cBoundingBox & a_CombineBounds, cItem & a_Item, const char a_MaxStackSize);

	/** Returns whether this pickup is allowed to combine with other similar pickups */
	bool CanCombine(void) const { return m_IsCombinable; }  // tolua_export

	/** Sets whether this pickup is allowed to combine with other similar pickups */
	void SetCanCombine(bool a_CanCombine) { m_IsCombinable = a_CanCombine; }  // tolua_export

	/** Returns the number of ticks that this entity has existed */
	int GetAge(void) const { LOGWARNING("GetAge is deprecated, use GetTicksAlive");  return m_TicksAlive; }     // tolua_export

	/** Set the number of ticks that this entity has existed */
	void SetAge(int a_Age) { LOGWARNING("SetAge is deprecated, use SetLifetime"); m_TicksAlive = a_Age; }  // tolua_export

	/** Returns the number of ticks that this pickup should live for */
	int GetLifetime(void) const { return std::chrono::duration_cast<cTickTime>(m_RemainingLifetime).count(); }  // tolua_export

	/** Set the number of ticks that this pickup should live for */
	void SetLifetime(int a_Lifetime) { m_RemainingLifetime = cTickTime(a_Lifetime); }  // tolua_export

	/** Returns true if the pickup has already been collected */
	bool IsCollected(void) const { return m_IsCollected; }  // tolua_export

	/** Returns true if created by player (i.e. vomiting), used for determining picking-up delay time */
	bool IsPlayerCreated(void) const { LOGWARNING("IsPlayerCreated is deprecated");  return false; }  // tolua_export

private:

	void OnCollectedBy(cPawn & a_Collector, char a_CollectionCount);

	void TryCombineWithPickupsInWorld();

	// cEntity overrides:
	virtual bool DoesPreventBlockPlacement(void) const override { return false; }
	virtual bool DoTakeDamage(TakeDamageInfo & a_TDI) override;
	virtual void OnAddToWorld(cWorld & a_World) override;
	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	virtual void Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk) override;

	cItem m_Item;

	/** How much time left until the pickup can be picked up.
	Two seconds if player created the pickup (vomiting), half a second if anything else, but plugin-customisable. */
	std::chrono::milliseconds m_RemainingCollectionDelay;

	/** You have thirty seconds to live. - Medic TF2 */
	std::chrono::milliseconds m_RemainingLifetime;

	bool m_IsCollected;
	bool m_IsCombinable;
};  // tolua_export
