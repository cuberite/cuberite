
#pragma once

#include "ItemFood.h"
#include "../Mobs/Enderman.h"





class cItemChorusFruitHandler:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	cItemChorusFruitHandler():
		Super(E_ITEM_CHORUS_FRUIT, FoodInfo(4, 2.4))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) override
	{
		cItemHandler::EatItem(a_Player, a_Item);

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->GetInventory().RemoveOneEquippedItem();
		}

		LOGD("Eating chorus fruit.");
		// Attempt to find a teleport destination
		Vector3i Destination;
		if (AttemptTeleport(*a_World, a_Player->GetPosition(), 8, 2, 16, Destination))
		{
			a_Player->TeleportToCoords(Destination.x, Destination.y, Destination.z);
			// TODO: send particle effects
		}

		return true;
	}

};
