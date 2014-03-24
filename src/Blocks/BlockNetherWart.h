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
#include "../MersenneTwister.h"
#include "../World.h"





/// Common class that takes care of carrots, potatoes and wheat
class cBlockNetherWartHandler :
	public cBlockHandler
{
public:
	cBlockNetherWartHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_Meta) override
	{
		MTRand rand;

		if (a_Meta == 0x7)
		{
			// Is fully grown, drop the entire produce:
			a_Pickups.push_back(cItem(E_ITEM_NETHER_WART, 1 + (int)(rand.randInt(2) + rand.randInt(2)) / 2, 0));
		}
		else
		{
			a_Pickups.push_back(cItem(E_ITEM_NETHER_WART));
		}
	}

	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta     = a_Chunk.GetMeta      (a_RelX, a_RelY, a_RelZ);

		if (Meta < 7)
		{
			a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_NETHER_WART, ++Meta);
		}
	}

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_SOULSAND));
	}
} ;
