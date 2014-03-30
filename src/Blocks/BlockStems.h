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
#include "../World.h"





class cBlockStemsHandler :
	public cBlockHandler
{
public:
	cBlockStemsHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		int ItemType = (m_BlockType == E_BLOCK_MELON_STEM) ? E_ITEM_MELON_SEEDS : E_ITEM_PUMPKIN_SEEDS;
		a_Pickups.push_back(cItem(ItemType, 1, 0));
	}

	
	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		NIBBLETYPE Meta = a_Chunk.GetMeta(a_RelX, a_RelY, a_RelZ);
		if (Meta >= 7)
		{
			// Grow the produce:
			int BlockX = a_RelX + a_Chunk.GetPosX() * cChunkDef::Width;
			int BlockZ = a_RelZ + a_Chunk.GetPosZ() * cChunkDef::Width;
			a_Chunk.GetWorld()->GrowMelonPumpkin(BlockX, a_RelY, BlockZ, m_BlockType);
		}
		else
		{
			// Grow the stem:
			a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, m_BlockType, Meta + 1);
		}
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		return ((a_RelY > 0) && (a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ) == E_BLOCK_FARMLAND));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




