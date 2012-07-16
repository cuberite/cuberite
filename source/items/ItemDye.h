#pragma once
#include "Item.h"
#include "../cWorld.h"
#include "../cPlayer.h"

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
					a_Player->GetInventory().RemoveItem(cItem(a_Item->m_ItemID, 1, a_Item->m_ItemHealth));
					return true;
				}
			}
		}
		return false;
	}
};