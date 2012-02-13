
#pragma once

#include "cEntity.h"




class cPacket_PickupSpawn;
class cPlayer;
class cItem;





class cPickup : public cEntity			//tolua_export
{										//tolua_export
public:
	CLASS_PROTOTYPE();

	cPickup(int a_X, int a_Y, int a_Z, const cItem & a_Item, float a_SpeedX = 0.f, float a_SpeedY = 0.f, float a_SpeedZ = 0.f);	//tolua_export
	cPickup(cPacket_PickupSpawn * a_PickupSpawnPacket);				//tolua_export
	~cPickup();														//tolua_export

	cItem * GetItem() { return m_Item; }								//tolua_export

	virtual cPacket * GetSpawnPacket(void) const override;
	
	virtual bool CollectedBy( cPlayer* a_Dest );					//tolua_export

	void Tick(float a_Dt);
	void HandlePhysics(float a_Dt);
	
private:

	Vector3f * m_Speed;
	Vector3f * m_ResultingSpeed;	 //Can be used to modify the resulting speed for the current tick ;)

	Vector3f * m_WaterSpeed;
	bool       m_bOnGround;
	bool       m_bReplicated;

	float m_Timer;

	cItem* m_Item;

	bool m_bCollected;
};  //tolua_export




