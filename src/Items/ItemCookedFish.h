
#pragma once

#include "ItemFood.h"





class cItemCookedFishHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemCookedFishHandler():
		Super(E_ITEM_COOKED_FISH, FoodInfo(0, 0))
	{
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) override
	{
		static const FoodInfo CookedFishInfos[] =
		{
			FoodInfo(5, 6.0),  // Cooked fish
			FoodInfo(6, 9.6),  // Cooked salmon
		};

		if (a_Item->m_ItemDamage >= static_cast<short>(ARRAYCOUNT(CookedFishInfos)))
		{
			LOGWARNING("Unknown cooked fish type '%d'", a_Item->m_ItemDamage);
			return FoodInfo(0, 0);
		}
		return CookedFishInfos[a_Item->m_ItemDamage];
	}

};
