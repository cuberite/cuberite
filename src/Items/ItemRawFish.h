
#pragma once

#include "ItemFood.h"





class cItemRawFishHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemRawFishHandler():
		Super(E_ITEM_RAW_FISH, FoodInfo(0, 0))
	{
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) override
	{
		static const FoodInfo RawFishInfos[] =
		{
			FoodInfo(2, 0.4),  // Raw fish
			FoodInfo(2, 0.2),  // Raw salmon
			FoodInfo(1, 0.2),  // Clownfish
			FoodInfo(1, 0.2),  // Pufferfish
		};

		if (a_Item->m_ItemDamage >= static_cast<short>(ARRAYCOUNT(RawFishInfos)))
		{
			LOGWARNING("Unknown raw fish type '%d'", a_Item->m_ItemDamage);
			return FoodInfo(0, 0);
		}
		return RawFishInfos[a_Item->m_ItemDamage];
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		if (!Super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		if (a_Item->m_ItemDamage == E_META_RAW_FISH_PUFFERFISH)
		{
			a_Player->AddEntityEffect(cEntityEffect::effHunger, 20 * 15, 2);
			a_Player->AddEntityEffect(cEntityEffect::effNausea, 20 * 15, 1);
			a_Player->AddEntityEffect(cEntityEffect::effPoison, 20 * 60, 3);
		}


		return true;
	}

};
