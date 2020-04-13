
#pragma once

#include "ItemFood.h"





class cItemRawChickenHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemRawChickenHandler():
		Super(E_ITEM_RAW_CHICKEN, FoodInfo(2, 1.2))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		if (!Super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		if (GetRandomProvider().RandBool(0.3))
		{
			a_Player->AddEntityEffect(cEntityEffect::effHunger, 600, 0);
		}

		return true;
	}

};
