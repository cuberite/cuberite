
#pragma once

#include "ItemFood.h"





class cItemSpiderEyeHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemSpiderEyeHandler():
		Super(E_ITEM_SPIDER_EYE, FoodInfo(2, 3.2))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		if (!Super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		a_Player->AddEntityEffect(cEntityEffect::effPoison, 100, 0);

		return true;
	}

};
