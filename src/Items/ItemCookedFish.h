
#pragma once

#include "ItemFood.h"





class cItemCookedFishHandler final:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	constexpr cItemCookedFishHandler(Item a_ItemType):
		Super(a_ItemType, FoodInfo(0, 0))
	{
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) const override
	{
		switch (a_Item->m_ItemType)
		{
			case Item::CookedCod:return FoodInfo(5, 6);
			case Item::CookedSalmon: return FoodInfo(6, 9.6);
			default: LOGWARNING("Unknown cooked fish type "); return FoodInfo(0, 0);
		}
	}

};
