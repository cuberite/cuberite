
// Boat.cpp

// Implements the cBoat class representing a boat in the world

#include "Globals.h"
#include "Boat.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "Player.h"





cBoat::cBoat(double a_X, double a_Y, double a_Z) :
	super(etBoat, a_X, a_Y, a_Z, 0.98, 0.7),
	m_LastDamage(0), m_ForwardDirection(0),
	m_DamageTaken(0.0f), m_Type(0),
	m_RightPaddleUsed(false), m_LeftPaddleUsed(false)
{
	SetMass(20.0f);
	SetGravity(-16.0f);
	SetAirDrag(0.05f);
	SetMaxHealth(6);
	SetHealth(6);
}




void cBoat::SpawnOn(cClientHandle & a_ClientHandle)
{
	a_ClientHandle.SendSpawnVehicle(*this, 1);
}





bool cBoat::DoTakeDamage(TakeDamageInfo & TDI)
{
	m_LastDamage = 10;
	if (!super::DoTakeDamage(TDI))
	{
		return false;
	}

	m_World->BroadcastEntityMetadata(*this);

	if (GetHealth() <= 0)
	{
		if (TDI.Attacker != nullptr)
		{
			if (TDI.Attacker->IsPlayer())
			{
				cItems Pickups;
				Pickups.Add(cItem(E_ITEM_BOAT));
				m_World->SpawnItemPickups(Pickups, GetPosX(), GetPosY(), GetPosZ(), 0, 0, 0, true);
			}
		}
		Destroy(true);
	}
	return true;
}





void cBoat::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);

	if (m_Attachee != nullptr)
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





void cBoat::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
	BroadcastMovementUpdate();

	SetSpeed(GetSpeed() * 0.97);  // Slowly decrease the speed

	if ((POSY_TOINT < 0) || (POSY_TOINT >= cChunkDef::Height))
	{
		return;
	}

	if (IsBlockWater(m_World->GetBlock(POSX_TOINT, POSY_TOINT, POSZ_TOINT)))
	{
		if (GetSpeedY() < 2)
		{
			AddSpeedY(0.2);
		}
	}

	if (GetLastDamage() > 0)
	{
		SetLastDamage(GetLastDamage() - 1);
	}

	// Broadcast any changes in position
	m_World->BroadcastEntityMetadata(*this);
}





void cBoat::HandleSpeedFromAttachee(float a_Forward, float a_Sideways)
{
	if (GetSpeed().Length() > 7.5)
	{
		return;
	}

	Vector3d ToAddSpeed = m_Attachee->GetLookVector() * (a_Sideways * 0.4) ;
	ToAddSpeed.y = 0;

	AddSpeed(ToAddSpeed);
}




void cBoat::SetLastDamage(int TimeSinceLastHit)
{
	m_LastDamage = TimeSinceLastHit;
}





void cBoat::UpdatePaddles(bool a_RightPaddleUsed, bool a_LeftPaddleUsed)
{
	m_RightPaddleUsed = a_RightPaddleUsed;
	m_LeftPaddleUsed = a_LeftPaddleUsed;

	m_World->BroadcastEntityMetadata(*this);
}
