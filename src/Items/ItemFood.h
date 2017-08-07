
#pragma once

#include "ItemHandler.h"





class cItemFoodHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemFoodHandler(int a_ItemType)
		: super(a_ItemType)
	{
	}


	virtual bool IsFood(void) override
	{
		return true;
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
		static const FoodInfo CookedFishInfos[] =
		{
			FoodInfo(5, 6.0),  // Cooked fish
			FoodInfo(6, 9.6),  // Cooked salmon
		};
		static const short NumRawFishInfos = sizeof(RawFishInfos) / sizeof(FoodInfo);
		static const short NumCookedFishInfos = sizeof(CookedFishInfos) / sizeof(FoodInfo);

		switch (m_ItemType)
		{
			// Please keep alpha-sorted.
			case E_ITEM_BAKED_POTATO:     return FoodInfo(5, 6);
			case E_ITEM_BEETROOT:         return FoodInfo(1, 1.2);
			case E_ITEM_BEETROOT_SOUP:    return FoodInfo(6, 7.2);
			case E_ITEM_BREAD:            return FoodInfo(5, 6);
			// Carrots handled in ItemSeeds
			case E_ITEM_CHORUS_FRUIT:     return FoodInfo(4, 2.4);
			case E_ITEM_COOKED_CHICKEN:   return FoodInfo(6, 7.2);
			case E_ITEM_COOKED_FISH:
			{
				if (a_Item->m_ItemDamage >= NumCookedFishInfos)
				{
					LOGWARNING("Unknown cooked fish type '%d'", a_Item->m_ItemDamage);
					return FoodInfo(0, 0);
				}
				return CookedFishInfos[a_Item->m_ItemDamage];
			}
			case E_ITEM_COOKED_MUTTON:    return FoodInfo(6, 9.6);
			case E_ITEM_COOKED_PORKCHOP:  return FoodInfo(8, 12.8);
			case E_ITEM_COOKED_RABBIT:    return FoodInfo(5, 6);
			case E_ITEM_COOKIE:           return FoodInfo(2, 0.5);
			// Golden apple handled in ItemGoldenApple
			case E_ITEM_GOLDEN_CARROT:    return FoodInfo(6, 14.4);
			case E_ITEM_MELON_SLICE:      return FoodInfo(2, 1.2);
			case E_ITEM_MUSHROOM_SOUP:    return FoodInfo(6, 7.2);
			case E_ITEM_POISONOUS_POTATO: return FoodInfo(2, 1.2);
			// Potatoes handled in ItemSeeds
			case E_ITEM_PUMPKIN_PIE:      return FoodInfo(8, 4.8);
			case E_ITEM_RABBIT_STEW:      return FoodInfo(10, 12);
			case E_ITEM_RED_APPLE:        return FoodInfo(4, 2.4);
			case E_ITEM_RAW_BEEF:         return FoodInfo(3, 1.8);
			case E_ITEM_RAW_CHICKEN:      return FoodInfo(2, 1.2);
			case E_ITEM_RAW_FISH:
			{
				if (a_Item->m_ItemDamage >= NumRawFishInfos)
				{
					LOGWARNING("Unknown raw fish type '%d'", a_Item->m_ItemDamage);
					return FoodInfo(0, 0);
				}
				return RawFishInfos[a_Item->m_ItemDamage];
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

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		if (!super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		switch (m_ItemType)
		{
			case E_ITEM_BEETROOT_SOUP:
			case E_ITEM_MUSHROOM_SOUP:
			case E_ITEM_RABBIT_STEW:
			{
				// Return a bowl to the inventory
				if (!a_Player->IsGameModeCreative())
				{
					a_Player->GetInventory().AddItem(cItem(E_ITEM_BOWL));
				}
				break;
			}
			case E_ITEM_RAW_FISH:
			{
				if (a_Item->m_ItemDamage == E_META_RAW_FISH_PUFFERFISH)
				{
					a_Player->AddEntityEffect(cEntityEffect::effHunger, 20 * 15, 2);
					a_Player->AddEntityEffect(cEntityEffect::effNausea, 20 * 15, 1);
					a_Player->AddEntityEffect(cEntityEffect::effPoison, 20 * 60, 3);
				}
				break;
			}
			case E_ITEM_RAW_CHICKEN:
			{
				if (GetRandomProvider().RandBool(0.3))
				{
					a_Player->AddEntityEffect(cEntityEffect::effHunger, 600, 0);
				}
				break;
			}
			case E_ITEM_ROTTEN_FLESH:
			{
				if (GetRandomProvider().RandBool(0.8))
				{
					a_Player->AddEntityEffect(cEntityEffect::effHunger, 600, 0);
				}
				break;
			}
			case E_ITEM_SPIDER_EYE:
			{
				a_Player->AddEntityEffect(cEntityEffect::effPoison, 100, 0);
				break;
			}
			case E_ITEM_POISONOUS_POTATO:
			{
				if (GetRandomProvider().RandBool(0.6))
				{
					a_Player->AddEntityEffect(cEntityEffect::effPoison, 100, 0);
				}
				break;
			}
		}
		return true;
	}

};




