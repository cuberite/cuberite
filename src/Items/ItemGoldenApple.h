
#pragma once

#include "ItemFood.h"





class cItemGoldenAppleHandler :
	public cItemFoodHandler
{
	typedef cItemFoodHandler super;

public:
	cItemGoldenAppleHandler()
		: super(E_ITEM_GOLDEN_APPLE)
	{
	}


	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		super::EatItem(a_Player, a_Item);

		// Enchanted golden apples have stronger effects:
		if (a_Item->m_ItemDamage >= E_META_GOLDEN_APPLE_ENCHANTED)
		{
			a_Player->AddEntityEffect(cEntityEffect::effAbsorption, 2400, 3);
			a_Player->AddEntityEffect(cEntityEffect::effRegeneration, 400, 1);
			a_Player->AddEntityEffect(cEntityEffect::effResistance, 6000, 0);
			a_Player->AddEntityEffect(cEntityEffect::effFireResistance, 6000, 0);
			return true;
		}

		a_Player->AddEntityEffect(cEntityEffect::effAbsorption, 2400, 0);
		a_Player->AddEntityEffect(cEntityEffect::effRegeneration, 100, 1);

		return true;
	}


	virtual FoodInfo GetFoodInfo(const cItem * a_Item) override
	{
		UNUSED(a_Item);
		return FoodInfo(4, 9.6);
	}

};
