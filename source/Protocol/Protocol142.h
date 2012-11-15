
// Protocol142.h

// Interfaces to the cProtocol142 class representing the release 1.4.2 protocol (#47)
// The same protocol class is used for 1.4.4 too, because the only difference is in a packet that MCServer doesn't implement yet (ITEM_DATA)





#pragma once

#include "Protocol132.h"





class cProtocol142 :
	public cProtocol132
{
	typedef cProtocol132 super;
public:

	cProtocol142(cClientHandle * a_Client);
	virtual ~cProtocol142();

	// Sending commands (alphabetically sorted):
	virtual void SendPickupSpawn        (const cPickup & a_Pickup) override;
	virtual void SendSoundParticleEffect(int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendTimeUpdate         (Int64 a_WorldAge, Int64 a_TimeOfDay) override;

	// Specific packet parsers:
	virtual int ParseLocaleViewDistance(void) override;
} ;




