
#pragma once

#include "ItemFood.h"





class cItemCookedFishHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemCookedFishHandler():
		Super(Item::CookedCod, FoodInfo(0, 0))
	{
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) override
	{
		switch (a_Item->m_ItemType)
		{
			case Item::CookedCod:return FoodInfo(5, 6);
			case Item::CookedSalmon: return FoodInfo(6, 9.6);
			default: LOGWARNING("Unknown cooked fish type "); return FoodInfo(0, 0);
		}
	}

};
