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





class cItemShearsHandler :
	public cItemHandler
{
public:
	cItemShearsHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}
	
	
	virtual bool IsTool(void) override
	{
		return true;
	}
	
	
	virtual bool OnDiggingBlock(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		if ((Block == E_BLOCK_LEAVES) || (Block == E_BLOCK_NEW_LEAVES))
		{
			cItems Drops;
			Drops.push_back(cItem(Block, 1, a_World->GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) & 0x03));
			a_World->SpawnItemPickups(Drops, a_BlockX, a_BlockY, a_BlockZ);

			a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			a_Player->UseEquippedItem();
			return true;
		}
		return false;
	}


	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_COBWEB:
			case E_BLOCK_VINES:
			case E_BLOCK_LEAVES:
			case E_BLOCK_NEW_LEAVES:
			{
				return true;
			}
		}  // switch (a_BlockType)
		return false;
	}
} ;




