
// FlowerPotEntity.cpp

// Implements the cFlowerPotEntity class representing a single flower pot in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "FlowerPotEntity.h"
#include "../Entities/Player.h"
#include "../ClientHandle.h"
#include "../Item.h"





cFlowerPotEntity::cFlowerPotEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_Pos, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_Pos, a_World)
{
	ASSERT(a_BlockType == E_BLOCK_FLOWER_POT);
}





void cFlowerPotEntity::Destroy(void)
{
	// Drop the contents as pickups:
	if (!m_Item.IsEmpty())
	{
		ASSERT(m_World != nullptr);
		cItems Pickups;
		Pickups.Add(m_Item);
		m_World->SpawnItemPickups(Pickups, Vector3d(0.5, 0.5, 0.5) + m_Pos);

		m_Item.Empty();
	}
}





void cFlowerPotEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = static_cast<const cFlowerPotEntity &>(a_Src);
	m_Item = src.m_Item;
}





bool cFlowerPotEntity::UsedBy(cPlayer * a_Player)
{
	if (IsItemInPot())
	{
		return false;
	}

	a_Player->GetStatManager().AddValue(Statistic::PotFlower);

	cItem SelectedItem = a_Player->GetInventory().GetEquippedItem();
	if (IsFlower(SelectedItem.m_ItemType, SelectedItem.m_ItemDamage))
	{
		m_Item = SelectedItem.CopyOne();
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}
		m_World->BroadcastBlockEntity(GetPos(), a_Player->GetClientHandle());
	}
	return true;
}





void cFlowerPotEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





bool cFlowerPotEntity::IsFlower(short m_ItemType, short m_ItemData)
{
	switch (m_ItemType)
	{
		case E_BLOCK_DANDELION:
		case E_BLOCK_FLOWER:
		case E_BLOCK_CACTUS:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_SAPLING:
		case E_BLOCK_DEAD_BUSH:
		{
			return true;
		}
		case E_BLOCK_TALL_GRASS:
		{
			return (m_ItemData == static_cast<short>(2));
		}
		default:
		{
			return false;
		}
	}
}
