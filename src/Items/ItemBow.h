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

// ItemBow.h

// Declares the cItemBowHandler class representing the itemhandler for bows





#pragma once

#include "../Entities/ProjectileEntity.h"





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
		if (!Arrow->Initialize(a_Player->GetWorld()))
		{
			delete Arrow;
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





