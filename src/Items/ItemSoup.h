
#pragma once

#include "ItemFood.h"
#include "../World.h"





class cItemSoupHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemSoupHandler(int a_ItemType, FoodInfo a_FoodInfo):
		Super(a_ItemType, a_FoodInfo)
	{

	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		// Skip over food handler, which does removal for us.
		if (!cItemHandler::EatItem(a_Player, a_Item))
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->ReplaceOneEquippedItemTossRest(cItem(E_ITEM_BOWL));
		}

		return true;
	}

};
