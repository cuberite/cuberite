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





class cItemRedstoneDustHandler : public cItemHandler
{
public:
	cItemRedstoneDustHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool IsPlaceable(void) override
	{
		return true;
	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (!cBlockInfo::FullyOccupiesVoxel(a_World->GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ))) // Some solid blocks, such as cocoa beans, are not suitable for dust
		{
			return false;
		}

		a_BlockType = E_BLOCK_REDSTONE_WIRE;
		a_BlockMeta = 0;
		return true;
	}
} ;




