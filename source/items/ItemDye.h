
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"

class cItemDyeHandler : public cItemHandler
{
public:
	cItemDyeHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		// TODO: Handle coloring the sheep, too (OnItemUseOnEntity maybe)
		// Handle growing the plants:
		if (a_Item->m_ItemHealth == E_META_DYE_WHITE)
		{
			if(a_World->GrowPlant(a_X, a_Y, a_Z, true))
			{
				if (a_Player->GetGameMode() == eGameMode_Survival)
				{
					cItem Item(a_Item->m_ItemID, 1, a_Item->m_ItemHealth);
					a_Player->GetInventory().RemoveItem(Item);
					return true;
				}
			}
		}
		return false;
	}
};