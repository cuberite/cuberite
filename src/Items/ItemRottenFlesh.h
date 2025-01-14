
#pragma once

#include "ItemFood.h"





class cItemRottenFleshHandler final:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	constexpr cItemRottenFleshHandler(Item a_ItemType):
		Super(a_ItemType, FoodInfo(4, 0.8))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) const override
	{
		if (!Super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		if (GetRandomProvider().RandBool(0.8))
		{
			a_Player->AddEntityEffect(cEntityEffect::effHunger, 600, 0);
		}

		return true;
	}

};
