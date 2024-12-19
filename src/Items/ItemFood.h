
#pragma once

#include "ItemHandler.h"





class cItemFoodHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	constexpr cItemFoodHandler(Item a_ItemType, FoodInfo a_FoodInfo):
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

	~cItemFoodHandler() = default;
};

class cItemSimpleFoodHandler final:
	public cItemFoodHandler
{
	using cItemFoodHandler::cItemFoodHandler;
};
