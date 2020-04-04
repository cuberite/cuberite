
#pragma once

#include "ItemFood.h"





class cItemRottenFleshHandler :
	public cItemFoodHandler
{
	typedef cItemFoodHandler super;

public:
	cItemRottenFleshHandler()
		: super(E_ITEM_ROTTEN_FLESH, FoodInfo(4, 0.8))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		if (!super::EatItem(a_Player, a_Item))
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
