
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"





class cItemDyeHandler :
	public cItemHandler
{
public:
	cItemDyeHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		// TODO: Handle coloring the sheep, too (OnItemUseOnEntity maybe)
		// Handle growing the plants:
		if (a_Item.m_ItemDamage == E_META_DYE_WHITE)
		{
			if (a_World->GrowRipePlant(a_BlockX, a_BlockY, a_BlockZ, true))
			{
				if (a_Player->GetGameMode() != eGameMode_Creative)
				{
					cItem Item(a_Item.m_ItemType, 1, a_Item.m_ItemDamage);
					a_Player->GetInventory().RemoveItem(Item);
					return true;
				}
			}
		}
		return false;
	}
} ;




