
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ArmorStand.h"
#include "Player.h"
#include "../ClientHandle.h"
#include "../Chunk.h"
#include "../World.h"





cArmorStand::cArmorStand(Vector3d a_Pos, double a_Yaw):
	Super(etArmorStand, a_Pos, 0.5f, 0.9875f),
	m_IsVisible(true),
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
	m_RightLegRotation(1.0, 0.0, 1.0)
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





void cArmorStand::OnClickedAt(cPlayer & a_Player, Vector3f a_TargetPos, bool a_UsedMainHand)
{
	// Disallow interaction if it's a marker
	if (IsMarker())
	{
		return;
	}

	cItem EquippedItem = a_Player.GetEquippedItem();

	// Maybe trying to get back an item
	if (EquippedItem.IsEmpty())
	{
		const bool Small = IsSizeSmall();
		float PosCheck = Small ? a_TargetPos.y * 2.0f : a_TargetPos.y;

		const cItem DummyEmptyItem;
		cItem Item;

		if (!GetEquippedBoots().IsEmpty()
			&& (PosCheck >= 0.1f)
			&& (PosCheck < 0.1f + (Small ? 0.8f : 0.45f)))
		{
			Item = GetEquippedBoots();
			SetEquippedBoots(DummyEmptyItem);
		}
		else if (!GetEquippedLeggings().IsEmpty()
			&& (PosCheck >= 0.4f)
			&& (PosCheck < 0.4f + (Small ? 1.0f : 0.8f)))
		{
			Item = GetEquippedLeggings();
			SetEquippedLeggings(DummyEmptyItem);
		}
		else if (!GetEquippedChestplate().IsEmpty()
			&& (PosCheck >= 0.9f + (Small ? 0.3f : 0))
			&& (PosCheck < 0.9f + (Small ? 1.0f : 0.7f)))
		{
			Item = GetEquippedChestplate();
			SetEquippedChestplate(DummyEmptyItem);
		}
		else if (!GetEquippedHelmet().IsEmpty()
			&& (PosCheck >= 1.6f))
		{
			Item = GetEquippedHelmet();
			SetEquippedHelmet(DummyEmptyItem);
		}

		if (!Item.IsEmpty() && (a_Player.IsGameModeSurvival()))
		{
			cItems Items;
			Items.emplace_back(Item);

			GetWorld()->SpawnItemPickups(Items, GetPosX(), GetPosY() + (GetHeight() / 2), GetPosZ(), 5);
		}

		return;
	}

	if (a_UsedMainHand)
	{
		short ItemType = EquippedItem.m_ItemType;

		// Trying to add Armor to stand
		if (ItemCategory::IsArmor(ItemType))
		{
			if (ItemCategory::IsHelmet(ItemType))
			{
				if (!GetEquippedHelmet().IsEmpty())
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
			else
			{
				return;
			}
		}
		else if (ItemCategory::IsTool(ItemType) && HasArms() && GetEquippedWeapon().IsEmpty())
		{
			SetEquippedWeapon(a_Player.GetEquippedItem());
		}
		else if (GetOffHandEquipedItem().IsEmpty() && HasArms())
		{
			SetOffHandEquipedItem(a_Player.GetEquippedItem());
		}
		else
		{
			return;
		}

		if (!a_Player.IsGameModeCreative())
		{
			a_Player.GetInventory().RemoveOneEquippedItem();
		}
	}
}





void cArmorStand::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
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
	// Disallow interaction with marker
	if (IsMarker())
	{
		return false;
	}

	m_World->BroadcastEntityMetadata(*this);

	if (a_TDI.Attacker != nullptr
		&& (a_TDI.Attacker->IsPawn()))
	{
		cPlayer * Player;

		if (a_TDI.Attacker->IsPlayer())
		{
			Player = static_cast<cPlayer *>(a_TDI.Attacker);

			if (Player == nullptr)
			{
				return false;
			}

			// Needs two hit in 0.5s to destroy in survival
			if (Player->IsGameModeSurvival()
				&& (m_TicksSinceLastDamaged >= 10))
			{
				m_TicksSinceLastDamaged = 0;
				return false;
			}
			else if (Player->IsGameModeCreative())
			{
				Destroy();
				SetInvulnerableTicks(0);
				return false;
			}
		}

		Destroy();

		cItems Pickups;

		if (!m_LeftHand.IsEmpty())
		{
			Pickups.push_back(m_LeftHand);
		}

		if (!m_RightHand.IsEmpty())
		{
			Pickups.push_back(m_RightHand);
		}

		if (!m_Helmet.IsEmpty())
		{
			Pickups.push_back(m_Helmet);
		}

		if (!m_ChestPlate.IsEmpty())
		{
			Pickups.push_back(m_ChestPlate);
		}

		if (!m_Leggings.IsEmpty())
		{
			Pickups.push_back(m_Leggings);
		}

		if (!m_Boots.IsEmpty())
		{
			Pickups.push_back(m_Boots);
		}

		Pickups.push_back(cItem(E_ITEM_ARMOR_STAND));

		GetWorld()->SpawnItemPickups(Pickups, GetPosX(), GetPosY() + (GetHeight() / 2), GetPosZ());
	}

	return false;
}





void cArmorStand::SpawnOn(cClientHandle & a_ClientHandle)  // Should got any rotation and should replace non-solid blocks like snow when placed
{
	a_ClientHandle.SendSpawnEntity(*this);
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





void cArmorStand::SetSizeNormal()
{
	SetSize(0.25f, 0.9875f);
	m_IsSmall = false;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetSizeSmall()
{
	SetSize(0.25f / 2, 0.9875f / 2);
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
		SetSize(0, 0);
		if (m_World != nullptr)
		{
			m_World->BroadcastEntityMetadata(*this);
		}
	}
	else  // Else, do recalculation of size
	{
		if (m_IsSmall)
		{
			SetSizeSmall();
		}
		else
		{
			SetSizeNormal();
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





void cArmorStand::SetHeadRotation(Vector3f a_HeadRotation)
{
	m_HeadRotation = a_HeadRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetBodyRotation(Vector3f a_BodyRotation)
{
	m_BodyRotation = a_BodyRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetLeftArmRotation(Vector3f a_LeftArmRotation)
{
	m_LeftArmRotation = a_LeftArmRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetRightArmRotation(Vector3f a_RightArmRotation)
{
	m_RightArmRotation = a_RightArmRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetLeftLegRotation(Vector3f a_LeftLegRotation)
{
	m_LeftLegRotation = a_LeftLegRotation;
	if (m_World != nullptr)
	{
		m_World->BroadcastEntityMetadata(*this);
	}
}





void cArmorStand::SetRightLegRotation(Vector3f a_RightLegRotation)
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
