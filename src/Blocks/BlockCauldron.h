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

#include "BlockHandler.h"





class cBlockCauldronHandler :
	public cBlockHandler
{
public:
	cBlockCauldronHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_CAULDRON, 1, 0));
	}

	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		char Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		switch (a_Player->GetEquippedItem().m_ItemType)
		{
			case E_ITEM_WATER_BUCKET:
			{
				if (Meta < 3)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, 3);
					if (!a_Player->IsGameModeCreative())
					{
						a_Player->GetInventory().RemoveOneEquippedItem();
						cItem NewItem(E_ITEM_BUCKET, 1);
						a_Player->GetInventory().AddItem(NewItem);
					}
				}
				break;
			}
			case E_ITEM_GLASS_BOTTLE:
			{
				if (Meta > 0)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, --Meta);
					a_Player->GetInventory().RemoveOneEquippedItem();
					cItem NewItem(E_ITEM_POTIONS, 1, 0);
					a_Player->GetInventory().AddItem(NewItem);
				}
				break;
			}
		}
	}

	virtual bool IsUseable() override
	{
		return true;
	}
} ;




