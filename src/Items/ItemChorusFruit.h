
#pragma once

#include "ItemFood.h"
#include "../Entities/Pawn.h"





class cItemChorusFruitHandler final:
	public cItemFoodHandler
{
	using Super = cItemFoodHandler;

public:

	constexpr cItemChorusFruitHandler(Item a_ItemType) :
		Super(a_ItemType, FoodInfo(4, 2.4))
	{
	}

	virtual bool EatItem(cPlayer * a_Player, cItem * a_Item) const override
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
