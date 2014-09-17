
#pragma once

#include "ItemHandler.h"





class cItemMushroomSoupHandler :
	public cItemHandler
{
	typedef cItemHandler super;
	
public:
	cItemMushroomSoupHandler(int a_ItemType)
		: super(a_ItemType)
	{
	}


	virtual bool IsFood(void) override
	{
		return true;
	}


	virtual FoodInfo GetFoodInfo(void) override
	{
		return FoodInfo(6, 7.2);
	}


	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		if (!super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		// Return a bowl to the inventory
		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().AddItem(cItem(E_ITEM_BOWL), true, true);
		}
		return true;
	}

};




