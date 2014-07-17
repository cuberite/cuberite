
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

	CLASS_PROTODEF(cPickup);

	cPickup(double a_PosX, double a_PosY, double a_PosZ, const cItem & a_Item, bool IsPlayerCreated, float a_SpeedX = 0.f, float a_SpeedY = 0.f, float a_SpeedZ = 0.f);

	cItem &       GetItem(void)       {return m_Item; }  // tolua_export
	const cItem & GetItem(void) const {return m_Item; }

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;

	bool CollectedBy(cPlayer * a_Dest);  // tolua_export

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;

	/** Returns the number of ticks that this entity has existed */
	int GetAge(void) const { return (int)(m_Timer / 50); }     // tolua_export

	/** Set the number of ticks that this entity has existed */
	void SetAge(int a_Age) { m_Timer = (float)(a_Age * 50); }  // tolua_export

	/** Returns true if the pickup has already been collected */
	bool IsCollected(void) const { return m_bCollected; }  // tolua_export

	/** Returns true if created by player (i.e. vomiting), used for determining picking-up delay time */
	bool IsPlayerCreated(void) const { return m_bIsPlayerCreated; }  // tolua_export

private:

	/** The number of ticks that the entity has existed / timer between collect and destroy; in msec */
	float m_Timer;

	cItem m_Item;

	bool m_bCollected;

	bool m_bIsPlayerCreated;
};  // tolua_export




