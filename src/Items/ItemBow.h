
#pragma once

#include "../Entities/ArrowEntity.h"
#include "ItemHandler.h"





class cItemBowHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemBowHandler(void):
		Super(E_ITEM_BOW)
	{
	}





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		ASSERT(a_Player != nullptr);

		// Check if the player has an arrow in the inventory, or is in Creative:
		if (!(a_Player->IsGameModeCreative() || a_Player->GetInventory().HasItems(cItem(E_ITEM_ARROW))))
		{
			return false;
		}

		a_Player->StartChargingBow();
		return true;
	}



	virtual void OnItemShoot(cPlayer * a_Player, const Vector3i a_BlockPos, eBlockFace a_BlockFace) override
	{
		// Actual shot - produce the arrow with speed based on the number of ticks that the bow was charged
		UNUSED(a_BlockPos);
		ASSERT(a_Player != nullptr);

		int BowCharge = a_Player->FinishChargingBow();
		double Force = static_cast<double>(BowCharge) / 20.0;
		Force = (Force * Force + 2.0 * Force) / 3.0;  // This formula is used by the 1.6.2 client
		if (Force < 0.1)
		{
			// Too little force, ignore the shot
			return;
		}
		Force = std::min(Force, 1.0);

		// Does the player have an arrow?
		if (!a_Player->IsGameModeCreative() && !a_Player->GetInventory().HasItems(cItem(E_ITEM_ARROW)))
		{
			return;
		}

		// Create the arrow entity:
		auto Arrow = std::make_unique<cArrowEntity>(*a_Player, Force * 2);
		auto ArrowPtr = Arrow.get();
		if (!ArrowPtr->Initialize(std::move(Arrow), *a_Player->GetWorld()))
		{
			return;
		}
		a_Player->GetWorld()->BroadcastSoundEffect(
			"entity.arrow.shoot",
			a_Player->GetPosition(),
			0.5,
			static_cast<float>(Force)
		);
		if (!a_Player->IsGameModeCreative())
		{
			if (a_Player->GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchInfinity) == 0)
			{
				a_Player->GetInventory().RemoveItem(cItem(E_ITEM_ARROW));
			}
			else
			{
				ArrowPtr->SetPickupState(cArrowEntity::psNoPickup);
			}

			a_Player->UseEquippedItem();
		}
		if (a_Player->GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchFlame) > 0)
		{
			ArrowPtr->StartBurning(100);
		}
	}
} ;
