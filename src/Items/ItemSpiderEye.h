
#pragma once

#include "ItemFood.h"





class cItemSpiderEyeHandler final:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	constexpr cItemSpiderEyeHandler(int a_ItemType):
		Super(a_ItemType, FoodInfo(2, 3.2))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) const override
	{
		if (!Super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		a_Player->AddEntityEffect(cEntityEffect::effPoison, 100, 0);

		return true;
	}

};
