
// Minecart.cpp

// Implements the cMinecart class representing a minecart in the world

#include "Globals.h"
#include "Minecart.h"
#include "World.h"
#include "ClientHandle.h"





cMinecart::cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z) :
	super(etMinecart, a_X, a_Y, a_Z),
	m_Payload(a_Payload)
{
}




void cMinecart::Initialize(cWorld * a_World)
{
	super::Initialize(a_World);
	a_World->BroadcastSpawn(*this);
}





void cMinecart::SpawnOn(cClientHandle & a_ClientHandle)
{
	char Type = 0;
	switch (m_Payload)
	{
		case mpNone:    Type = 10; break;
		case mpChest:   Type = 11; break;
		case mpFurnace: Type = 12; break;
		default:
		{
			ASSERT(!"Unknown payload, cannot spawn on client");
			return;
		}
	}
	a_ClientHandle.SendSpawnVehicle(*this, Type);
}





void cMinecart::Tick(float a_Dt, MTRand & a_TickRandom)
{
	// TODO: the physics
}




