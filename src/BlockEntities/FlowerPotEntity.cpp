
// FlowerPotEntity.cpp

// Implements the cFlowerPotEntity class representing a single flower pot in the world

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "json/json.h"
#include "FlowerPotEntity.h"
#include "../Entities/Player.h"
#include "../Item.h"





cFlowerPotEntity::cFlowerPotEntity(int a_BlockX, int a_BlockY, int a_BlockZ, cWorld * a_World) :
	super(E_BLOCK_FLOWER_POT, a_BlockX, a_BlockY, a_BlockZ, a_World)
{
}





// It don't do anything when 'used'
void cFlowerPotEntity::UsedBy(cPlayer * a_Player)
{
	if (IsItemInPot())
	{
		return;
	}
	
	cItem SelectedItem = a_Player->GetInventory().GetEquippedItem();
	if (IsFlower(SelectedItem.m_ItemType, SelectedItem.m_ItemDamage))
	{
		m_Item = SelectedItem.CopyOne();
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}
		m_World->BroadcastBlockEntity(m_PosX, m_PosY, m_PosZ, a_Player->GetClientHandle());
	}
}





void cFlowerPotEntity::SendTo(cClientHandle & a_Client)
{
	a_Client.SendUpdateBlockEntity(*this);
}





void cFlowerPotEntity::Destroy(void)
{
	// Drop the contents as pickups:
	if (!m_Item.IsEmpty())
	{
		ASSERT(m_World != NULL);
		cItems Pickups;
		Pickups.Add(m_Item);
		m_World->SpawnItemPickups(Pickups, m_PosX + 0.5, m_PosY + 0.5, m_PosZ + 0.5);
		
		m_Item.Empty();
	}
}





bool cFlowerPotEntity::LoadFromJson(const Json::Value & a_Value)
{
	m_PosX = a_Value.get("x", 0).asInt();
	m_PosY = a_Value.get("y", 0).asInt();
	m_PosZ = a_Value.get("z", 0).asInt();

	m_Item = cItem();
	m_Item.FromJson(a_Value.get("Item", 0));

	return true;
}





void cFlowerPotEntity::SaveToJson(Json::Value & a_Value)
{
	a_Value["x"] = m_PosX;
	a_Value["y"] = m_PosY;
	a_Value["z"] = m_PosZ;
	
	Json::Value Item;
	m_Item.GetJson(Item);
	a_Value["Item"] = Item;
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
			return (m_ItemData == (short) 2);
		}
		default:
		{
			return false;
		}
	}
}




