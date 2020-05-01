
// Boat.cpp

// Implements the cBoat class representing a boat in the world

#include "Globals.h"
#include "Boat.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "Player.h"





cBoat::cBoat(Vector3d a_Pos, eMaterial a_Material) :
	Super(etBoat, a_Pos, 0.98, 0.7),
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
}





void cBoat::BroadcastMovementUpdate(const cClientHandle * a_Exclude)
{
	// Process packet sending every two ticks
	if (GetWorld()->GetWorldAge() % 2 != 0)
	{
		return;
	}

	Vector3i Diff = (GetPosition() * 32.0).Floor() - (m_LastSentPosition * 32.0).Floor();

	if (Diff.HasNonZeroLength())  // Have we moved?
	{
		if ((abs(Diff.x) <= 127) && (abs(Diff.y) <= 127) && (abs(Diff.z) <= 127))  // Limitations of a Byte
		{
			m_World->BroadcastEntityRelMove(*this, Vector3<Int8>(Diff), a_Exclude);
		}
		else
		{
			// Too big a movement, do a teleport
			m_World->BroadcastTeleportEntity(*this, a_Exclude);
		}
		m_LastSentPosition = GetPosition();
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
	a_Player.AttachTo(this);
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





cBoat::eMaterial cBoat::ItemToMaterial(const cItem & a_Item)
{
	switch (a_Item.m_ItemType)
	{
		case E_ITEM_BOAT:          return bmOak;
		case E_ITEM_SPRUCE_BOAT:   return bmSpruce;
		case E_ITEM_BIRCH_BOAT:    return bmBirch;
		case E_ITEM_JUNGLE_BOAT:   return bmJungle;
		case E_ITEM_ACACIA_BOAT:   return bmAcacia;
		case E_ITEM_DARK_OAK_BOAT: return bmDarkOak;
		default:
		{
			LOGWARNING("%s: Item type not handled %d.", __FUNCTION__, a_Item.m_ItemType);
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
		case bmOak:     return cItem(E_ITEM_BOAT);
		case bmSpruce:  return cItem(E_ITEM_SPRUCE_BOAT);
		case bmBirch:   return cItem(E_ITEM_BIRCH_BOAT);
		case bmJungle:  return cItem(E_ITEM_JUNGLE_BOAT);
		case bmAcacia:  return cItem(E_ITEM_ACACIA_BOAT);
		case bmDarkOak: return cItem(E_ITEM_DARK_OAK_BOAT);
	}
	UNREACHABLE("Unsupported boat material");
}




