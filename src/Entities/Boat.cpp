
// Boat.cpp

// Implements the cBoat class representing a boat in the world

#include "Globals.h"
#include "Boat.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../ClientHandle.h"
#include "Player.h"





cBoat::cBoat(Vector3d a_Pos, eMaterial a_Material) :
	Super(etBoat, a_Pos, 1.375, 0.5625),
	m_Material(a_Material),
	m_LeftPaddleUsed(false),
	m_RightPaddleUsed(false),
	m_ShouldShakeForwards(true)
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





bool cBoat::DoTakeDamage(TakeDamageInfo & TDI)
{
	if (
		const auto Attacker = TDI.Attacker;

		(Attacker != nullptr) &&
		Attacker->IsPlayer() &&
		static_cast<cPlayer *>(Attacker)->IsGameModeCreative()
	)
	{
		TDI.FinalDamage = GetMaxHealth();  // Instant hit for creative
		return Super::DoTakeDamage(TDI);
	}

	if (!Super::DoTakeDamage(TDI))
	{
		return false;
	}

	m_World->BroadcastEntityMetadata(*this);  // Tell the client to play the shaking animation
	m_ShouldShakeForwards = !m_ShouldShakeForwards;  // The next shake goes the opposite direction

	SetInvulnerableTicks(2);  // Make rapid attacks have more of an effect
	return true;
}





void cBoat::OnRightClicked(cPlayer & a_Player)
{
	Super::OnRightClicked(a_Player);

	if (m_Attachee != nullptr)
	{
		if (m_Attachee == &a_Player)
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





void cBoat::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	if (
		(a_Killer != nullptr) &&
		a_Killer->IsPlayer() &&
		!static_cast<cPlayer *>(a_Killer)->IsGameModeCreative()  // No drops for creative
	)
	{
		a_Drops.Add(MaterialToItem(m_Material));
	}
}





void cBoat::KilledBy(TakeDamageInfo & a_TDI)
{
	Super::KilledBy(a_TDI);
	Destroy();
}





void cBoat::HandlePhysics(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	const auto Position = POS_TOINT;
	const auto InWater = IsBlockWater(m_World->GetBlock(Position));
	const auto DtSec = std::chrono::duration_cast<std::chrono::duration<double>>(a_Dt).count();

	// A real boat floats.
	// Propel to top water block and sit slightly beneath the waterline:
	if (InWater)
	{
		// Counteract gravity and provide a small upwards force:
		SetSpeedY((-m_Gravity + 1) * DtSec);
	}

	auto Speed = GetSpeed();
	ApplyFriction(Speed, 0.9, DtSec);  // Slowly decrease the speed
	SetSpeed(Speed);

	Super::HandlePhysics(a_Dt, a_Chunk);

	const auto AbovePosition = Position.addedY(1);
	const auto OnSurface = !IsBlockWater(m_World->GetBlock(AbovePosition));
	if (InWater && OnSurface)
	{
		SetSpeedY(0);
		SetPosY(Position.y + 0.52);
	}

	BroadcastMovementUpdate();
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





float cBoat::GetDamageTaken(void) const
{
	return GetMaxHealth() - GetHealth();
}





bool cBoat::ShouldShakeForwards(void) const
{
	return m_ShouldShakeForwards;
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
