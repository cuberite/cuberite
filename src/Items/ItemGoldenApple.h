
#pragma once

#include "ItemFood.h"





class cItemGoldenAppleHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemGoldenAppleHandler():
		Super(Item::GoldenApple, FoodInfo(4, 9.6))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		cItemHandler::EatItem(a_Player, a_Item);

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		// Enchanted golden apples have stronger effects:
		if (a_Item->m_ItemType == Item::EnchantedGoldenApple)
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

};
