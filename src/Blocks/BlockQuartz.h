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





class cBlockQuartzHandler : public cBlockHandler
{
public:
	cBlockQuartzHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = (NIBBLETYPE)(a_Player->GetEquippedItem().m_ItemDamage);
		if (Meta != E_META_QUARTZ_PILLAR) // Check if the block is a pillar block.
		{
			a_BlockMeta = Meta;
			return true;
		}

		a_BlockMeta = BlockFaceToMetaData(a_BlockFace, Meta);
		return true;
	}

	inline static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace, NIBBLETYPE a_QuartzMeta)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return a_QuartzMeta; // Top or bottom, just return original
			}

			case BLOCK_FACE_ZP:
			case BLOCK_FACE_ZM:
			{
				return 0x4; // North or south
			}

			case BLOCK_FACE_XP:
			case BLOCK_FACE_XM:
			{
				return 0x3; // East or west
			}

			default:
			{
				ASSERT(!"Unhandled block face!");
				return a_QuartzMeta; // No idea, give a special meta (all sides the same)
			}
		}
	}
} ;
