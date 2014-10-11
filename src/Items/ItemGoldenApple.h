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
		// Feed the player:
		FoodInfo Info = GetFoodInfo();
		a_Player->Feed(Info.FoodLevel, Info.Saturation);

		// Add the effects:
		a_Player->AddEntityEffect(cEntityEffect::effAbsorption, 2400, 0);
		a_Player->AddEntityEffect(cEntityEffect::effRegeneration, 100, 1);

		// When the apple is a 'notch apple', give extra effects:
		if (a_Item->m_ItemDamage >= E_META_GOLDEN_APPLE_ENCHANTED)
		{
			a_Player->AddEntityEffect(cEntityEffect::effRegeneration, 600, 4);
			a_Player->AddEntityEffect(cEntityEffect::effResistance, 6000, 0);
			a_Player->AddEntityEffect(cEntityEffect::effFireResistance, 6000, 0);
		}

		a_Player->GetInventory().RemoveOneEquippedItem();
		return true;
	}


	virtual FoodInfo GetFoodInfo(void) override
	{
		return FoodInfo(4, 9.6);
	}


	virtual bool GetEatEffect(cEntityEffect::eType & a_EffectType, int & a_EffectDurationTicks, short & a_EffectIntensity, float & a_Chance) override
	{
		return false;
	}

};




