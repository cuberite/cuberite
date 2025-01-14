
#pragma once

#include "ItemSeeds.h"
#include "../World.h"





class cItemFoodSeedsHandler final:
	public cItemSeedsHandler
{
	using Super = cItemSeedsHandler;

public:

	constexpr cItemFoodSeedsHandler(Item a_ItemType, FoodInfo a_FoodInfo):
		Super(a_ItemType),
		m_FoodInfo(a_FoodInfo)
	{

	}


	virtual bool IsFood(void) const override
	{
		return true;
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) const override
	{
		UNUSED(a_Item);
		return m_FoodInfo;
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) const override
	{
		if (!Super::EatItem(a_Player, a_Item))
		{
			return false;
		}

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		return true;
	}

protected:
	FoodInfo m_FoodInfo;

};
