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
#include "../Blocks/BlockBed.h"





class cItemBedHandler :
	public cItemHandler
{
public:
	cItemBedHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
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
		if (a_BlockFace != BLOCK_FACE_TOP)
		{
			// Can only be placed on the floor
			return false;
		}

		a_BlockMeta = cBlockBedHandler::RotationToMetaData(a_Player->GetYaw());
		
		// Check if there is empty space for the foot section:
		Vector3i Direction = cBlockBedHandler::MetaDataToDirection(a_BlockMeta);
		if (a_World->GetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z) != E_BLOCK_AIR)
		{
			return false;
		}

		a_BlockType = E_BLOCK_BED;
		return true;
	}
} ;




