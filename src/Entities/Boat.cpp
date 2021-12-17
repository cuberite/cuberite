
// Boat.cpp

// Implements the cBoat class representing a boat in the world

#include "Globals.h"
#include "Boat.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "Player.h"





class cBoatCollisionCallback
{
public:

	cBoatCollisionCallback(cBoat & a_Boat, cEntity * a_Attachee) :
		m_Boat(a_Boat), m_Attachee(a_Attachee)
	{
	}

	bool operator()(cEntity & a_Entity)
	{
		// Checks if boat is empty and if given entity is a mob:
		if ((m_Attachee == nullptr) && a_Entity.IsMob())
		{
			// If so attach and stop iterating:
			a_Entity.AttachTo(m_Boat);
			return true;
		}

		return false;
	}

protected:

	cBoat & m_Boat;
	cEntity * m_Attachee;
};





cBoat::cBoat(Vector3d a_Pos, eMaterial a_Material) :
	Super(etBoat, a_Pos, 1.375f, 0.5625f),
	m_LastDamage(0), m_ForwardDirection(0),
	m_DamageTaken(0.0f), m_Material(a_Material),
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
	a_ClientHandle.SendSpawnEntity(*this);
	a_ClientHandle.SendEntityMetadata(*this);  // Boat colour
}





void cBoat::BroadcastMovementUpdate(const cClientHandle * a_Exclude)
{
	// Cannot use super::BroadcastMovementUpdate here, broadcasting position when not
	// expected by the client breaks things. See https://github.com/cuberite/cuberite/pull/4488

	// Process packet sending every two ticks:
	if ((GetWorld()->GetWorldTickAge() % 2_tick) != 0_tick)
	{
		return;
	}

	Vector3i Diff = (GetPosition() * 32.0).Floor() - (m_LastSentPosition * 32.0).Floor();
	if (Diff.HasNonZeroLength())  // Have we moved?
	{
		m_World->BroadcastEntityPosition(*this, a_Exclude);
		m_LastSentPosition = GetPosition();
		m_bDirtyOrientation = false;
	}
}





bool cBoat::DoTakeDamage(TakeDamageInfo & TDI)
{
	m_LastDamage = 10;
	if (!Super::DoTakeDamage(TDI))
	{
		return false;
	}

	m_World->BroadcastEntityMetadata(*this);

	if ((TDI.Attacker != nullptr) && (TDI.Attacker->IsPlayer()))
	{
		cPlayer * Destroyer = static_cast<cPlayer *>(TDI.Attacker);
		if (Destroyer->IsGameModeCreative())
		{
			Destroy();
			return true;
		}
	}

	if (GetHealth() <= 0)
	{
		if (TDI.Attacker != nullptr)
		{
			if (TDI.Attacker->IsPlayer())
			{
				cItems Pickups;
				Pickups.Add(MaterialToItem(m_Material));
				m_World->SpawnItemPickups(Pickups, GetPosX(), GetPosY(), GetPosZ(), 0, 0, 0, true);
			}
		}
		Destroy();
	}
	return true;
}





void cBoat::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

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
	a_Player.AttachTo(*this);
}





void cBoat::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
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

	if (m_World->GetBlock(POS_TOINT).Type() == BlockType::Water)
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

	// Tell the client to play the shaking animation
	m_World->BroadcastEntityMetadata(*this);
}





void cBoat::UpdatePaddles(bool a_RightPaddleUsed, bool a_LeftPaddleUsed)
{
	// Avoid telling client what it already knows since it may reset animation 1.13+
	const bool Changed = (m_RightPaddleUsed != a_RightPaddleUsed) || (m_LeftPaddleUsed != a_LeftPaddleUsed);

	m_RightPaddleUsed = a_RightPaddleUsed;
	m_LeftPaddleUsed = a_LeftPaddleUsed;

	if (Changed)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





cBoat::eMaterial cBoat::ItemToMaterial(const cItem & a_Item)
{
	switch (a_Item.m_ItemType)
	{

		case Item::AcaciaBoat:  return bmAcacia;
		case Item::BirchBoat:   return bmBirch;
		case Item::DarkOakBoat: return bmDarkOak;
		case Item::JungleBoat:  return bmJungle;
		case Item::OakBoat:     return bmOak;
		case Item::SpruceBoat:  return bmSpruce;
		default:
		{
			FLOGWARNING("{}: Item type not handled {}.", __FUNCTION__, a_Item.m_ItemType);
			return cBoat::bmOak;
		}
	}
}





AString cBoat::MaterialToString(eMaterial a_Material)
{
	switch (a_Material)
	{
		case bmOak:     return "oak";
		case bmSpruce:  return "spruce";
		case bmBirch:   return "birch";
		case bmJungle:  return "jungle";
		case bmAcacia:  return "acacia";
		case bmDarkOak: return "dark_oak";
	}
	UNREACHABLE("Unsupported boat material");
}





cBoat::eMaterial cBoat::StringToMaterial(const AString & a_Material)
{
	if (a_Material == "oak")
	{
		return bmOak;
	}
	else if (a_Material == "spruce")
	{
		return bmSpruce;
	}
	else if (a_Material == "birch")
	{
		return bmBirch;
	}
	else if (a_Material == "jungle")
	{
		return bmJungle;
	}
	else if (a_Material == "acacia")
	{
		return bmAcacia;
	}
	else if (a_Material == "dark_oak")
	{
		return bmDarkOak;
	}
	else
	{
		return bmOak;
	}
}





cItem cBoat::MaterialToItem(eMaterial a_Material)
{
	switch (a_Material)
	{
		case bmOak:     return cItem(Item::OakBoat);
		case bmSpruce:  return cItem(Item::SpruceBoat);
		case bmBirch:   return cItem(Item::BirchBoat);
		case bmJungle:  return cItem(Item::JungleBoat);
		case bmAcacia:  return cItem(Item::AcaciaBoat);
		case bmDarkOak: return cItem(Item::DarkOakBoat);
	}
	UNREACHABLE("Unsupported boat material");
}





void cBoat::HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	/** Special version of cEntity::HandlePhysics(...) function for boats, checks if mobs
	colliding with the boat can be attached and does if that's the case, then returns to
	normal physics calcualtions */

	// Calculate boat's bounding box, run collision callback on all entities in said box
	cBoatCollisionCallback BoatCollisionCallback(*this, m_Attachee);
	Vector3d BoatPosition = GetPosition();
	cBoundingBox bbBoat(
		Vector3d(BoatPosition.x, floor(BoatPosition.y), BoatPosition.z), GetWidth() / 2, GetHeight());
	m_World->ForEachEntityInBox(bbBoat, BoatCollisionCallback);

	// Return to calculating physics normally
	Super::HandlePhysics(a_Dt, a_Chunk);
}
