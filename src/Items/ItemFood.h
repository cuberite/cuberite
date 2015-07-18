
#pragma once

#include "ItemHandler.h"





class cItemFoodHandler :
	public cItemHandler
{
	typedef cItemHandler super;
	
public:
	cItemFoodHandler(int a_ItemType, short a_ItemDamage = 0) :
		super(a_ItemType),
		m_ItemDamage(a_ItemDamage)
	{
	}


	virtual bool IsFood(void) override
	{
		return true;
	}


	virtual FoodInfo GetFoodInfo(void) override
	{
		switch (m_ItemType)
		{
			// Please keep alpha-sorted.
			case E_ITEM_BAKED_POTATO:     return FoodInfo(5, 7.2);
			case E_ITEM_BREAD:            return FoodInfo(5, 6);
			// Carrots handled in ItemSeeds
			case E_ITEM_COOKED_CHICKEN:   return FoodInfo(6, 7.2);
			case E_ITEM_COOKED_FISH:
			{
				if (m_ItemDamage == E_META_COOKED_FISH_SALMON)
				{
					return FoodInfo(6, 9.6);
				}
				else
				{
					return FoodInfo(5, 6);
				}
			}
			case E_ITEM_COOKED_MUTTON:    return FoodInfo(6, 9.6);
			case E_ITEM_COOKED_PORKCHOP:  return FoodInfo(8, 12.8);
			case E_ITEM_COOKED_RABBIT:    return FoodInfo(5, 6);
			case E_ITEM_COOKIE:           return FoodInfo(2, 0.4);
			// Golden apple handled in ItemGoldenApple
			case E_ITEM_GOLDEN_CARROT:    return FoodInfo(6, 14.4);
			case E_ITEM_MELON_SLICE:      return FoodInfo(2, 1.2);
			case E_ITEM_POISONOUS_POTATO: return FoodInfo(2, 1.2);
			// Potatoes handled in ItemSeeds
			case E_ITEM_PUMPKIN_PIE:      return FoodInfo(8, 4.8);
			case E_ITEM_RABBIT_STEW:      return FoodInfo(10, 12);
			case E_ITEM_RED_APPLE:        return FoodInfo(4, 2.4);
			case E_ITEM_RAW_BEEF:         return FoodInfo(3, 1.8);
			case E_ITEM_RAW_CHICKEN:      return FoodInfo(2, 1.2);
			case E_ITEM_RAW_FISH:
			{
				switch (m_ItemDamage)
				{
					default:
					case E_META_RAW_FISH_SALMON:     return FoodInfo(2, 0.4);
					case E_META_RAW_FISH_CLOWNFISH:
					case E_META_RAW_FISH_PUFFERFISH: return FoodInfo(1, 0.2);
				}
			}
			case E_ITEM_RAW_MUTTON:       return FoodInfo(2, 1.2);
			case E_ITEM_RAW_PORKCHOP:     return FoodInfo(3, 1.8);
			case E_ITEM_RAW_RABBIT:       return FoodInfo(3, 1.8);
			case E_ITEM_ROTTEN_FLESH:     return FoodInfo(4, 0.8);
			case E_ITEM_SPIDER_EYE:       return FoodInfo(2, 3.2);
			case E_ITEM_STEAK:            return FoodInfo(8, 12.8);
		}
		LOGWARNING("%s: Unknown food item (%d), returning zero nutrition", __FUNCTION__, m_ItemType);
		return FoodInfo(0, 0.f);
	}

	virtual bool AddEatEffects(cPlayer & a_Player) override
	{
		float Chance = cFastRandom().NextFloat();
		switch (m_ItemType)
		{
			case E_ITEM_RAW_CHICKEN:
			{
				if (0.3f <= Chance)
				{
					return true;  // Not apply effect
				}
			}
			case E_ITEM_ROTTEN_FLESH:
			{
				if (0.8f > Chance)
				{
					a_Player.AddEntityEffect(cEntityEffect::effHunger, 30 * 20, 0);
				}
				return true;
			}
			case E_ITEM_POISONOUS_POTATO:
			{
				if (0.6f <= Chance)
				{
					return true;  // Not apply effect
				}
			}
			case E_ITEM_SPIDER_EYE:
			{
				a_Player.AddEntityEffect(cEntityEffect::effPoison, 4 * 20, 0);
				return true;
			}
			case E_ITEM_RAW_FISH:
			{
				if (m_ItemDamage == E_META_RAW_FISH_PUFFERFISH)
				{
					a_Player.AddEntityEffect(cEntityEffect::effNausea, 15 * 20, 1);
					a_Player.AddEntityEffect(cEntityEffect::effPoison, 60 * 20, 3);
					a_Player.AddEntityEffect(cEntityEffect::effHunger, 15 * 20, 2);
					return true;
				}
			}
		}
		return false;
	}

	protected:
		short m_ItemDamage;
};




