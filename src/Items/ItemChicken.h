
#pragma once

#include "ItemFood.h"





class cItemChickenHandler final:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	constexpr cItemChickenHandler(Item a_ItemType):
		Super(a_ItemType, FoodInfo(2, 1.2))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) const override
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
