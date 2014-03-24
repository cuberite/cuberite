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

// BlockFarmland.h

// Declares the cBlcokFarmlandHandler representing the block handler for farmland





#pragma once

#include "BlockHandler.h"
#include "../BlockArea.h"





class cBlockFarmlandHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
	
public:
	cBlockFarmlandHandler(void) :
		super(E_BLOCK_FARMLAND)
	{
	}


	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		bool Found = false;
		
		EMCSBiome Biome = a_Chunk.GetBiomeAt(a_RelX, a_RelZ);
		if (a_Chunk.GetWorld()->IsWeatherWet() && !IsBiomeNoDownfall(Biome))
		{
			// Rain hydrates farmland, too, except in Desert biomes.
			Found = true;
		}
		else
		{
			// Search for water in a close proximity:
			// Ref.: http://www.minecraftwiki.net/wiki/Farmland#Hydrated_Farmland_Tiles
			// TODO: Rewrite this to use the chunk and its neighbors directly
			cBlockArea Area;
			int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
			int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
			if (!Area.Read(a_Chunk.GetWorld(), BlockX - 4, BlockX + 4, a_RelY, a_RelY + 1, BlockZ - 4, BlockZ + 4))
			{
				// Too close to the world edge, cannot check surroundings; don't tick at all
				return;
			}

			int NumBlocks = Area.GetBlockCount();
			BLOCKTYPE * BlockTypes = Area.GetBlockTypes();
			for (int i = 0; i < NumBlocks; i++)
			{
				if (
					(BlockTypes[i] == E_BLOCK_WATER) ||
					(BlockTypes[i] == E_BLOCK_STATIONARY_WATER)
				)
				{
					Found = true;
					break;
				}
			}  // for i - BlockTypes[]
		}
		
		NIBBLETYPE BlockMeta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		
		if (Found)
		{
			// Water was found, hydrate the block until hydration reaches 7:
			if (BlockMeta < 7)
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, m_BlockType, ++BlockMeta);
			}
			return;
		}

		// Water wasn't found, de-hydrate block:
		if (BlockMeta > 0)
		{
			a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_FARMLAND, --BlockMeta);
			return;
		}
		
		// Farmland too dry. If nothing is growing on top, turn back to dirt:
		switch (a_Chunk.GetBlock(a_RelX, a_RelY + 1, a_RelZ))
		{
			case E_BLOCK_CROPS:
			case E_BLOCK_POTATOES:
			case E_BLOCK_CARROTS:
			case E_BLOCK_MELON_STEM:
			case E_BLOCK_PUMPKIN_STEM:
			{
				// Produce on top, don't revert
				break;
			}
			default:
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_DIRT, 0);
				break;
			}
		}
	}
} ;




