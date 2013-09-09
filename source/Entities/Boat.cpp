
// Boat.cpp

// Implements the cBoat class representing a boat in the world

#include "Globals.h"
#include "Boat.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "Player.h"





cBoat::cBoat(double a_X, double a_Y, double a_Z) :
	super(etBoat, a_X, a_Y, a_Z, 0.98, 0.7)
{
	SetMass(20.f);
	SetMaxHealth(6);
	SetHealth(6);
}




void cBoat::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnVehicle(*this, 1);
}





void cBoat::DoTakeDamage(TakeDamageInfo & TDI)
{
	super::DoTakeDamage(TDI);

	if (GetHealth() == 0)
	{
		Destroy(true);
	}
}





void cBoat::OnRightClicked(cPlayer & a_Player)
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
		
		// Detach whatever is sitting in this boat now:
		m_Attachee->Detach();
	}
	
	// Attach the player to this boat
	a_Player.AttachTo(this);
}





void cBoat::HandlePhysics(float a_Dt, cChunk & a_Chunk)
{
	super::HandlePhysics(a_Dt, a_Chunk);
	BroadcastMovementUpdate();
}




