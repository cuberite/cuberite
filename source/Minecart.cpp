
// Minecart.cpp

// Implements the cMinecart class representing a minecart in the world

#include "Globals.h"
#include "Minecart.h"
#include "World.h"
#include "ClientHandle.h"
#include "Player.h"





cMinecart::cMinecart(ePayload a_Payload, double a_X, double a_Y, double a_Z) :
	super(etMinecart, a_X, a_Y, a_Z, 0.98, 0.7),
	m_Payload(a_Payload)
{
}




void cMinecart::Initialize(cWorld * a_World)
{
	super::Initialize(a_World);
	a_World->BroadcastSpawnEntity(*this);
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





void cMinecart::Tick(float a_Dt, cChunk & a_Chunk)
{
	// TODO: the physics
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cEmptyMinecart:

cEmptyMinecart::cEmptyMinecart(double a_X, double a_Y, double a_Z) :
	super(mpNone, a_X, a_Y, a_Z)
{
}





void cEmptyMinecart::OnRightClicked(cPlayer & a_Player)
{
	if (m_Attachee != NULL)
	{
		if (m_Attachee->GetUniqueID() == a_Player.GetUniqueID())
		{
			// This player is already sitting in, they want out.
			a_Player.Detach();
			return;
		}
		
		if (m_Attachee->IsPlayer())
		{
			// Another player is already sitting in here, cannot attach
			return;
		}
		
		// Detach whatever is sitting in this minecart now:
		m_Attachee->Detach();
	}
	
	// Attach the player to this minecart
	a_Player.AttachTo(this);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMinecartWithChest:

cMinecartWithChest::cMinecartWithChest(double a_X, double a_Y, double a_Z) :
	super(mpChest, a_X, a_Y, a_Z)
{
}





void cMinecartWithChest::SetSlot(int a_Idx, const cItem & a_Item)
{
	ASSERT((a_Idx >= 0) && (a_Idx < ARRAYCOUNT(m_Items)));
	
	m_Items[a_Idx] = a_Item;
}





void cMinecartWithChest::OnRightClicked(cPlayer & a_Player)
{
	// Show the chest UI window to the player
	// TODO
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMinecartWithFurnace:

cMinecartWithFurnace::cMinecartWithFurnace(double a_X, double a_Y, double a_Z) :
	super(mpFurnace, a_X, a_Y, a_Z)
{
}





void cMinecartWithFurnace::OnRightClicked(cPlayer & a_Player)
{
	// Try to power the furnace with whatever the player is holding
	// TODO
}





