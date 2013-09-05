
// Protocol14x.h

/*
Interfaces to the 1.4.x protocol classes representing these protocols:
- cProtocol142:
	- release 1.4.2 protocol (#47)
	- release 1.4.4 protocol (#49) - the same protocol class is used, because the only difference is in a packet that MCServer doesn't implement yet (ITEM_DATA)
	- release 1.4.5 protocol (same as 1.4.4)
- cProtocol146:
	- release 1.4.6 protocol (#51)
*/





#pragma once

#include "Protocol132.h"





class cProtocol142 :
	public cProtocol132
{
	typedef cProtocol132 super;
	
public:
	cProtocol142(cClientHandle * a_Client);

	// Sending commands (alphabetically sorted):
	virtual void SendPickupSpawn        (const cPickup & a_Pickup) override;
	virtual void SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendTimeUpdate         (Int64 a_WorldAge, Int64 a_TimeOfDay) override;

	// Specific packet parsers:
	virtual int ParseLocaleViewDistance(void) override;
} ;





class cProtocol146 :
	public cProtocol142
{
	typedef cProtocol142 super;
	
public:
	cProtocol146(cClientHandle * a_Client);
	
	virtual void SendPickupSpawn      (const cPickup & a_Pickup) override;
	virtual void SendSpawnFallingBlock(const cFallingBlock & a_FallingBlock) override;
	virtual void SendSpawnObject      (const cEntity & a_Entity, char a_ObjectType, int a_ObjectData, Byte a_Yaw, Byte a_Pitch) override;
	virtual void SendSpawnVehicle     (const cEntity & a_Vehicle, char a_VehicleType, char a_VehicleSubType) override;
} ;




