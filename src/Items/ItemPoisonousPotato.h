
#pragma once

#include "ItemFood.h"





class cItemPoisonousPotatoHandler :
	public cItemFoodHandler
{
	typedef cItemFoodHandler super;

public:
	cItemPoisonousPotatoHandler()
		: super(E_ITEM_POISONOUS_POTATO, FoodInfo(2, 1.2))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		if (!super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		if (GetRandomProvider().RandBool(0.6))
		{
			a_Player->AddEntityEffect(cEntityEffect::effPoison, 100, 0);
		}

		return true;
	}

};
