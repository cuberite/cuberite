
#pragma once

#include "Entity.h"
#include "../Item.h"





class cPlayer;





// tolua_begin
class cPickup :
	public cEntity
{
	typedef cEntity super;
	
public:
	// tolua_end

	CLASS_PROTODEF(cPickup)

	cPickup(CreateEntityInfo a_Info, const cItem & a_Item, bool IsPlayerCreated);

	cItem &       GetItem(void)       {return m_Item; }  // tolua_export
	const cItem & GetItem(void) const {return m_Item; }

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	bool CollectedBy(cPlayer * a_Dest);  // tolua_export

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	/** Returns true if the pickup has already been collected */
	bool IsCollected(void) const { return m_bCollected; }  // tolua_export

	/** Returns true if created by player (i.e. vomiting), used for determining picking-up delay time */
	bool IsPlayerCreated(void) const { return m_bIsPlayerCreated; }  // tolua_export

private:

	int m_DestroyTimer;

	cItem m_Item;

	bool m_bCollected;

	bool m_bIsPlayerCreated;
};  // tolua_export




