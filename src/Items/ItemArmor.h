
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemArmorHandler :
	public cItemHandler
{
public:
	cItemArmorHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		int SlotNum = -1;
		if (ItemCategory::IsHelmet(a_Item.m_ItemType))
		{
			SlotNum = 0;
		}
		else if (ItemCategory::IsChestPlate(a_Item.m_ItemType))
		{
			SlotNum = 1;
		}
		else if (ItemCategory::IsLeggings(a_Item.m_ItemType))
		{
			SlotNum = 2;
		}
		else if (ItemCategory::IsBoots(a_Item.m_ItemType))
		{
			SlotNum = 3;
		}
		
		if (!a_Player->GetInventory().GetArmorSlot(SlotNum).IsEmpty())
		{
			return false;
		}
		
		a_Player->GetInventory().SetArmorSlot(SlotNum, a_Item.CopyOne());
		a_Player->GetInventory().SetHotbarSlot(a_Player->GetInventory().GetEquippedSlotNum(), cItem());
		return true;
	}

} ;




