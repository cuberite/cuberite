
#pragma once

#include "ItemFood.h"





class cItemRawFishHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemRawFishHandler():
		Super(Item::Cod, FoodInfo(0, 0))
	{
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) override
	{
		switch (a_Item->m_ItemType)
		{
			case Item::Cod:return FoodInfo(2, 0.4);
			case Item::Salmon: return FoodInfo(2, 0.2);
			case Item::TropicalFish:return FoodInfo(1, 0.2);
			case Item::Pufferfish:return FoodInfo(1, 0.2);
			default: LOGWARNING("Unknown raw fish type "); return FoodInfo(0, 0);
		}
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		if (!Super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		if (a_Item->m_ItemType == Item::Pufferfish)
		{
			a_Player->AddEntityEffect(cEntityEffect::effHunger, 20 * 15, 2);
			a_Player->AddEntityEffect(cEntityEffect::effNausea, 20 * 15, 1);
			a_Player->AddEntityEffect(cEntityEffect::effPoison, 20 * 60, 3);
		}


		return true;
	}

};
