
// ItemBow.h

// Declares the cItemBowHandler class representing the itemhandler for bows





#pragma once

#include "../Entities/ArrowEntity.h"





class cItemBowHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemBowHandler(void) :
		super(E_ITEM_BOW)
	{
	}



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
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



	virtual void OnItemShoot(cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		// Actual shot - produce the arrow with speed based on the ticks that the bow was charged
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
		auto Arrow = cpp14::make_unique<cArrowEntity>(*a_Player, Force * 2);
		auto ArrowPtr = Arrow.get();
		if (!ArrowPtr->Initialize(std::move(Arrow), *a_Player->GetWorld()))
		{
			return;
		}
		a_Player->GetWorld()->BroadcastSoundEffect(
			"entity.arrow.shoot",
			a_Player->GetPosX(),
			a_Player->GetPosY(),
			a_Player->GetPosZ(),
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
				Arrow->SetPickupState(cArrowEntity::psNoPickup);
			}


			a_Player->UseEquippedItem();
		}

		if (a_Player->GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchFlame) > 0)
		{
			Arrow->StartBurning(100);
		}
	}
} ;





