/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
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


	virtual FoodInfo GetFoodInfo(void) override
	{
		switch(m_ItemType)
		{
			// Please keep alpha-sorted.
			case E_ITEM_BAKED_POTATO:     return FoodInfo(6, 7.2);
			case E_ITEM_BREAD:            return FoodInfo(5, 6);
			// Carrots handled in ItemSeeds
			case E_ITEM_COOKED_CHICKEN:   return FoodInfo(6, 7.2);
			case E_ITEM_COOKED_FISH:      return FoodInfo(5, 6);
			case E_ITEM_COOKED_PORKCHOP:  return FoodInfo(8, 12.8);
			case E_ITEM_COOKIE:           return FoodInfo(2, 0.4);
			case E_ITEM_GOLDEN_APPLE:     return FoodInfo(4, 9.6);
			case E_ITEM_GOLDEN_CARROT:    return FoodInfo(6, 14.4);
			case E_ITEM_MELON_SLICE:      return FoodInfo(2, 1.2);
			case E_ITEM_MUSHROOM_SOUP:    return FoodInfo(6, 7.2);
			case E_ITEM_POISONOUS_POTATO: return FoodInfo(2, 1.2, 60);
			// Potatoes handled in ItemSeeds
			case E_ITEM_PUMPKIN_PIE:      return FoodInfo(8, 4.8);
			case E_ITEM_RAW_BEEF:         return FoodInfo(3, 1.8);
			case E_ITEM_RAW_CHICKEN:      return FoodInfo(2, 1.2, 30);
			case E_ITEM_RAW_FISH:         return FoodInfo(2, 1.2);
			case E_ITEM_RAW_PORKCHOP:     return FoodInfo(3, 1.8);
			case E_ITEM_RED_APPLE:        return FoodInfo(4, 2.4);
			case E_ITEM_ROTTEN_FLESH:     return FoodInfo(4, 0.8, 80);
			case E_ITEM_SPIDER_EYE:       return FoodInfo(2, 3.2, 100);
			case E_ITEM_STEAK:            return FoodInfo(8, 12.8);
		}
		LOGWARNING("%s: Unknown food item (%d), returning zero nutrition", __FUNCTION__, m_ItemType);
		return FoodInfo(0, 0.f);
	}
	
};




