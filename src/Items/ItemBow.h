
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
	
	
	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		ASSERT(a_Player != NULL);
		
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
		ASSERT(a_Player != NULL);
		
		int BowCharge = a_Player->FinishChargingBow();
		double Force = (double)BowCharge / 20;
		Force = (Force * Force + 2 * Force) / 3;  // This formula is used by the 1.6.2 client
		if (Force < 0.1)
		{
			// Too little force, ignore the shot
			return;
		}
		if (Force > 1)
		{
			Force = 1;
		}
		
		// Create the arrow entity:
		cArrowEntity * Arrow = new cArrowEntity(*a_Player, Force * 2);
		if (Arrow == NULL)
		{
			return;
		}
		if (!Arrow->Initialize(*a_Player->GetWorld()))
		{
			delete Arrow;
			Arrow = NULL;
			return;
		}
		a_Player->GetWorld()->BroadcastSpawnEntity(*Arrow);
		a_Player->GetWorld()->BroadcastSoundEffect("random.bow", (int)a_Player->GetPosX() * 8, (int)a_Player->GetPosY() * 8, (int)a_Player->GetPosZ() * 8, 0.5, (float)Force);

		if (!a_Player->IsGameModeCreative())
		{
			a_Player->UseEquippedItem();
		}
	}
} ;





