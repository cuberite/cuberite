
// FlowerPotEntity.cpp

// Implements the cFlowerPotEntity class representing a single flower pot in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "FlowerPotEntity.h"
#include "../Entities/Player.h"
#include "../ClientHandle.h"
#include "../Item.h"





cFlowerPotEntity::cFlowerPotEntity(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World):
	Super(a_BlockType, a_BlockMeta, a_BlockX, a_BlockY, a_BlockZ, a_World)
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
		m_World->SpawnItemPickups(Pickups, m_PosX + 0.5, m_PosY + 0.5, m_PosZ + 0.5);

		m_Item.Empty();
	}
}





void cFlowerPotEntity::CopyFrom(const cBlockEntity & a_Src)
{
	Super::CopyFrom(a_Src);
	auto & src = reinterpret_cast<const cFlowerPotEntity &>(a_Src);
	m_Item = src.m_Item;
}





bool cFlowerPotEntity::UsedBy(cPlayer * a_Player)
{
	if (IsItemInPot())
	{
		return false;
	}

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
