
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ArmorStand.h"
#include "Player.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "../World.h"





cArmorStand::cArmorStand(Vector3d a_Pos, double a_Yaw):
	cEntity(etArmorStand, a_Pos.x, a_Pos.y, a_Pos.z, 0.5, 0.9875),
	m_IsVisible(true),
	m_CustomName(""),
	m_CustomNameAlwaysVisible(false),
	m_TicksSinceLastDamaged(100),
	m_IsSmall(false),
	m_IsMarker(false),
	m_HasArms(false),
	m_HasBasePlate(true),
	m_HeadRotation(0.0, 0.0, 0.0),
	m_BodyRotation(0.0, 0.0, 0.0),
	m_LeftArmRotation(-10.0, 0.0, -10.0),
	m_RightArmRotation(-15.0, 0.0, 10.0),
	m_LeftLegRotation(-1.0, 0.0, -1.0),
	m_RightLegRotation(1.0, 0.0, 1.0),
	m_LeftHand(E_BLOCK_AIR),
	m_RightHand(E_BLOCK_AIR),
	m_Helmet(E_BLOCK_AIR),
	m_ChestPlate(E_BLOCK_AIR),
	m_Leggings(E_BLOCK_AIR),
	m_Boots(E_BLOCK_AIR)
{
	SetMaxHealth(1);
	SetHealth(1);
	SetYaw(a_Yaw);
	SetGravity(-16.0f);
	SetAirDrag(0.02f);
}





void cArmorStand::OnRightClicked(cPlayer & a_Player)
{
	if (IsMarker() || a_Player.GetEquippedItem().IsEmpty())  // Disallow interaction with marker and prevent acting if no item
	{
		return;
	}

	short ItemType = a_Player.GetEquippedItem().m_ItemType;

	if ((ItemType == E_ITEM_NAME_TAG) && !a_Player.GetEquippedItem().m_CustomName.empty())  // Add a name to the armor stand
	{
		SetCustomName(a_Player.GetEquippedItem().m_CustomName);
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
}





void cArmorStand::OnClickedAt(cPlayer & a_Player, Vector3f a_TargetPos, eHand a_Hand)
{
	if (IsMarker() || a_Player.GetEquippedItem().IsEmpty())  // Disallow interaction with marker and prevent acting if no item
	{
		return;
	}

	if (a_Hand == eHand::hMain)
	{
		short ItemType = a_Player.GetEquippedItem().m_ItemType;

		if (ItemCategory::IsArmor(ItemType))  // OR find the best place for the item on the armor stand
		{
			if (ItemCategory::IsHelmet(ItemType))
			{
				if (!GetEquippedHelmet().IsEmpty())  // That way, we don't remove the item from the player
				{
					return;
				}
				SetEquippedHelmet(a_Player.GetEquippedItem());
			}
			else if (ItemCategory::IsChestPlate(ItemType))
			{
				if (!GetEquippedChestplate().IsEmpty())
				{
					return;
				}
				SetEquippedChestplate(a_Player.GetEquippedItem());
			}
			else if (ItemCategory::IsLeggings(ItemType))
			{
				if (!GetEquippedLeggings().IsEmpty())
				{
					return;
				}
				SetEquippedLeggings(a_Player.GetEquippedItem());
			}
			else if (ItemCategory::IsBoots(ItemType))
			{
				if (!GetEquippedBoots().IsEmpty())
				{
					return;
				}
				SetEquippedBoots(a_Player.GetEquippedItem());
			}
			else if (ItemCategory::IsTool(ItemType) && HasArms() && GetEquippedWeapon().IsEmpty())
			{
				SetEquippedWeapon(a_Player.GetEquippedItem());
			}
			else if (GetOffHandEquipedItem().IsEmpty() && HasArms())
			{
				SetOffHandEquipedItem(a_Player.GetEquippedItem());
			}
			else  // No place on the armor
			{
				return;  // Don't stole the item from the player if we haven't used it
			}
		}
		else if (GetOffHandEquipedItem().IsEmpty() && HasArms())
		{
			SetOffHandEquipedItem(a_Player.GetEquippedItem());
		}
		else  // No place on the armor
		{
			return;  // Don't stole the item from the player if we haven't used it
		}

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
}





void cArmorStand::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}
	if (m_TicksSinceLastDamaged < 100)
	{
		++m_TicksSinceLastDamaged;
	}
	HandlePhysics(a_Dt, a_Chunk);
	BroadcastMovementUpdate();
}





bool cArmorStand::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (IsMarker())  // Disallow interaction with marker
	{
		return false;
	}

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPlayer() && static_cast<cPlayer *>(a_TDI.Attacker)->IsGameModeCreative())
	{
		Destroy();
		SetInvulnerableTicks(0);
		return false;
	}
	m_World->BroadcastEntityMetadata(*this);

	if ((a_TDI.Attacker != nullptr) && a_TDI.Attacker->IsPawn())
	{
		if (a_TDI.Attacker->IsPlayer() && (m_TicksSinceLastDamaged >= 10))  // Needs two hit in 0.5s to destroy
		{
			GetWorld()->BroadcastEntityStatus(*this, esArmorStandHit);
			m_TicksSinceLastDamaged = 0;
		}
		else
		{
			Destroy();

			cItems Pickup;
			if (!m_LeftHand.IsEmpty())
			{
				Pickup.push_back(m_LeftHand);
			}
			if (!m_RightHand.IsEmpty())
			{
				Pickup.push_back(m_RightHand);
			}
			if (!m_Helmet.IsEmpty())
			{
				Pickup.push_back(m_Helmet);
			}
			if (!m_ChestPlate.IsEmpty())
			{
				Pickup.push_back(m_ChestPlate);
			}
			if (!m_Leggings.IsEmpty())
			{
				Pickup.push_back(m_Leggings);
			}
			if (!m_Boots.IsEmpty())
			{
				Pickup.push_back(m_Boots);
			}
			Pickup.push_back(cItem(E_ITEM_ARMOR_STAND));
			GetWorld()->SpawnItemPickups(Pickup, GetPosX(), GetPosY() + (GetHeight() / 2), GetPosZ());
		}
	}
	return false;
}





void cArmorStand::SpawnOn(cClientHandle & a_ClientHandle)  // Should got any rotation and should replace any of non-solid blocks like snow
{
	a_ClientHandle.SendSpawnObject(*this, 78, 0, static_cast<Byte>(GetYaw()), static_cast<Byte>(GetPitch()));
	a_ClientHandle.SendEntityMetadata(*this);
	a_ClientHandle.SendEntityEquipment(*this, 0, GetEquippedWeapon());
	a_ClientHandle.SendEntityEquipment(*this, 1, GetEquippedBoots());
	a_ClientHandle.SendEntityEquipment(*this, 2, GetEquippedLeggings());
	a_ClientHandle.SendEntityEquipment(*this, 3, GetEquippedChestplate());
	a_ClientHandle.SendEntityEquipment(*this, 4, GetEquippedHelmet());
	a_ClientHandle.SendEntityEquipment(*this, 5, GetOffHandEquipedItem());  // For compatibility issues, left hand was added at the end and not at the ID 1 like wants the protocol since 1.9
}





void cArmorStand::SetVisible(bool a_IsVisible)
{
	m_IsVisible = a_IsVisible;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





bool cArmorStand::IsInvisible() const
{
	return !m_IsVisible;
}





void cArmorStand::SetCustomName(const AString & a_CustomName)
{
	m_CustomName = a_CustomName;

	// The maximal length is 64
	if (a_CustomName.length() > 64)
	{
		m_CustomName = a_CustomName.substr(0, 64);
	}

	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetCustomNameAlwaysVisible(bool a_CustomNameAlwaysVisible)
{
	m_CustomNameAlwaysVisible = a_CustomNameAlwaysVisible;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetNormal()
{
	SetHeight(0.9875);
	SetWidth(0.25);
	m_IsSmall = false;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetSmall()
{
	SetHeight(0.9875/2);
	SetWidth(0.25/2);
	m_IsSmall = true;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetIsMarker(bool a_IsMarker)
{
	m_IsMarker = a_IsMarker;
	if (a_IsMarker)  // By default set size of marker to null
	{
		SetHeight(0);
		SetWidth(0);
		if (m_World != nullptr)
		{
			m_World->BroadcastEntityMetadata(*this);
		}
	}
	else  // Else, do recalculation of size
	{
		if (m_IsSmall)
		{
			SetSmall();
		}
		else
		{
			SetNormal();
		}
	}
}





void cArmorStand::SetHasArms(bool a_HasArms)
{
	m_HasArms = a_HasArms;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetHasBasePlate(bool a_HasBasePlate)
{
	m_HasBasePlate = a_HasBasePlate;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetHeadRotation(Vector3d a_HeadRotation)
{
	m_HeadRotation = a_HeadRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetBodyRotation(Vector3d a_BodyRotation)
{
	m_BodyRotation = a_BodyRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetLeftArmRotation(Vector3d a_LeftArmRotation)
{
	m_LeftArmRotation = a_LeftArmRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetRightArmRotation(Vector3d a_RightArmRotation)
{
	m_RightArmRotation = a_RightArmRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetLeftLegRotation(Vector3d a_LeftLegRotation)
{
	m_LeftLegRotation = a_LeftLegRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetRightLegRotation(Vector3d a_RightLegRotation)
{
	m_RightLegRotation = a_RightLegRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetEquippedWeapon(const cItem & a_RightHand)
{
	m_RightHand = a_RightHand;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityEquipment(*this, 0, a_RightHand);
	}
}





void cArmorStand::SetOffHandEquipedItem(const cItem & a_LeftHand)
{
	m_LeftHand = a_LeftHand;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityEquipment(*this, 5, a_LeftHand);  // See above (In SpawnOn) about the slot ID
	}
}





void cArmorStand::SetEquippedHelmet(const cItem & a_Helmet)
{
	m_Helmet = a_Helmet;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityEquipment(*this, 4, a_Helmet);
	}
}





void cArmorStand::SetEquippedChestplate(const cItem & a_ChestPlate)
{
	m_ChestPlate = a_ChestPlate;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityEquipment(*this, 3, a_ChestPlate);
	}
}





void cArmorStand::SetEquippedLeggings(const cItem & a_Leggings)
{
	m_Leggings = a_Leggings;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityEquipment(*this, 2, a_Leggings);
	}
}





void cArmorStand::SetEquippedBoots(const cItem & a_Boots)
{
	m_Boots = a_Boots;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityEquipment(*this, 1, a_Boots);
	}
}
