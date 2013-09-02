
#pragma once

#include "Entity.h"
#include "../Item.h"





class cPlayer;





// tolua_begin
class cPickup :
	public cEntity
{
	// tolua_end
	typedef cEntity super;
	
public:
	CLASS_PROTODEF(cPickup);

	cPickup(int a_MicroPosX, int a_MicroPosY, int a_MicroPosZ, const cItem & a_Item, float a_SpeedX = 0.f, float a_SpeedY = 0.f, float a_SpeedZ = 0.f);	// tolua_export
	
	cItem &       GetItem(void)       {return m_Item; }								// tolua_export
	const cItem & GetItem(void) const {return m_Item; }

	virtual void SpawnOn(cClientHandle & a_ClientHandle) override;
	
	virtual bool CollectedBy(cPlayer * a_Dest);					// tolua_export

	virtual void Tick(float a_Dt, cChunk & a_Chunk) override;
	
	/// Returns the number of ticks that this entity has existed
	int GetAge(void) const { return (int)(m_Timer / 50); }  // tolua_export
	
	/// Returns true if the pickup has already been collected
	bool IsCollected(void) const { return m_bCollected; }  // tolua_export
	
private:
	Vector3d   m_ResultingSpeed;	 //Can be used to modify the resulting speed for the current tick ;)

	Vector3d   m_WaterSpeed;

	/// The number of ticks that the entity has existed / timer between collect and destroy; in msec
	float m_Timer;

	cItem m_Item;

	bool m_bCollected;
};  // tolua_export




