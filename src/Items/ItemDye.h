
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemDyeHandler :
	public cItemHandler
{
public:
	cItemDyeHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		// Handle growing the plants:
		if (a_Item.m_ItemDamage == E_META_DYE_WHITE)
		{
			if (a_World->GrowRipePlant(a_BlockX, a_BlockY, a_BlockZ, true))
			{
				// Particle effects are in GrowRipePlant
				if (!a_Player->IsGameModeCreative())
				{
					a_Player->GetInventory().RemoveOneEquippedItem();
					return true;
				}
			}
		}
		return false;
	}
} ;




