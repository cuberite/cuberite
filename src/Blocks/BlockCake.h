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





class cBlockCakeHandler :
	public cBlockHandler
{
public:
	cBlockCakeHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

		if (!a_Player->Feed(2, 0.1))
		{
			return;
		}
		
		if (Meta >= 5)
		{
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
		}
		else
		{
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta + 1);
		}
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Give nothing
	}

	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




