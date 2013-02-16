
#pragma once

#include "ItemHandler.h"


class cItemFoodHandler : public cItemHandler
{
public:
	cItemFoodHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool IsFood() override
	{
		return true;
	}

	virtual FoodInfo GetFoodInfo() override
	{
		switch(m_ItemType)
		{
		case E_ITEM_BREAD:
			return FoodInfo(5, 6.f);
		case E_ITEM_COOKIE:
			return FoodInfo(2, 0.4f);
		case E_ITEM_MELON_SLICE:
			return FoodInfo(2, 1.2f);
		case E_ITEM_RAW_CHICKEN:
			return FoodInfo(2, 1.2f, 30);
		case E_ITEM_COOKED_CHICKEN:
			return FoodInfo(6, 7.2f);
		case E_ITEM_RAW_BEEF:
		case E_ITEM_RAW_PORKCHOP:
			return FoodInfo(3, 1.8f);
		case E_ITEM_STEAK:
		case E_ITEM_COOKED_PORKCHOP:
			return FoodInfo(8, 12.8f);
		case E_ITEM_RAW_FISH:
			return FoodInfo(2, 1.2f);
		case E_ITEM_COOKED_FISH:
			return FoodInfo(5, 6.f);
		case E_ITEM_RED_APPLE:
			return FoodInfo(4, 2.4f);
		case E_ITEM_GOLDEN_APPLE:
			return FoodInfo(4, 9.6f);
		case E_ITEM_ROTTEN_FLESH:
			return FoodInfo(4, 0.8f, 80);
		case E_ITEM_SPIDER_EYE:
			return FoodInfo(2, 3.2f, 100);
		}
		return FoodInfo(0, 0.f);
	}
	
};