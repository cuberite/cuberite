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
#include "../World.h"
#include "../Entities/Player.h"





class cItemSpawnEggHandler : public cItemHandler
{
public:
	cItemSpawnEggHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}


	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		if (a_BlockFace < 0)
		{
			return false;
		}
		
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

		if (a_BlockFace == BLOCK_FACE_YM)
		{
			a_BlockY--;
		}

		if (a_World->SpawnMob(a_BlockX + 0.5, a_BlockY, a_BlockZ + 0.5, (cMonster::eType)(a_Item.m_ItemDamage)) >= 0)
		{
			if (!a_Player->IsGameModeCreative())
			{
				// The mob was spawned, "use" the item:
				a_Player->GetInventory().RemoveOneEquippedItem();
			}
			return true;
		}
		
		return false;
	}
} ;




