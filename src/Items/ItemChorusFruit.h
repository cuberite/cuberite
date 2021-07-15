
#pragma once

#include "ItemFood.h"
#include "../Entities/Pawn.h"





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

		// Attempt to find a teleport destination
		Vector3d Destination;
		cWorld * World = a_Player->GetWorld();
		if (cPawn::FindTeleportDestination(*World, 2, 16, Destination, a_Player->GetPosition(), 8))
		{
			// Broadcast sound effect to _pre-teleport_ location, then teleport player.
			World->BroadcastSoundEffect("item.chorus_fruit.teleport", a_Player->GetPosition(), 1, 1);
			a_Player->TeleportToCoords(Destination.x, Destination.y, Destination.z);
		}
		return true;
	}

};
