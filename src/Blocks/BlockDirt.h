
#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"





/// Handler used for both dirt and grass
class cBlockDirtHandler :
	public cBlockHandler
{
public:
	cBlockDirtHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	
	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_DIRT, 1, 0));
	}
	
	
	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		if (m_BlockType != E_BLOCK_GRASS)
		{
			return;
		}
		
		// Grass becomes dirt if there is something on top of it:
		if (a_RelY < cChunkDef::Height - 1)
		{
			BLOCKTYPE Above = a_Chunk.GetBlock(a_RelX, a_RelY + 1, a_RelZ);
			if ((!cBlockInfo::IsTransparent(Above) && !cBlockInfo::IsOneHitDig(Above)) || IsBlockWater(Above))
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_DIRT, E_META_DIRT_NORMAL);
				return;
			}
		}
		
		// Grass spreads to adjacent dirt blocks:
		cFastRandom rand;
		for (int i = 0; i < 2; i++)  // Pick two blocks to grow to
		{
			int OfsX = rand.NextInt(3, a_RelX) - 1;  // [-1 .. 1]
			int OfsY = rand.NextInt(5, a_RelY) - 3;  // [-3 .. 1]
			int OfsZ = rand.NextInt(3, a_RelZ) - 1;  // [-1 .. 1]
	
			BLOCKTYPE  DestBlock;
			NIBBLETYPE DestMeta;
			if ((a_RelY + OfsY < 0) || (a_RelY + OfsY >= cChunkDef::Height - 1))
			{
				// Y Coord out of range
				continue;
			}
			int BlockX = a_RelX + OfsX;
			int BlockY = a_RelY + OfsY;
			int BlockZ = a_RelZ + OfsZ;
			cChunk * Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(BlockX, BlockZ);
			if (Chunk == NULL)
			{
				// Unloaded chunk
				continue;
			}
			Chunk->GetBlockTypeMeta(BlockX, BlockY, BlockZ, DestBlock, DestMeta);
			if ((DestBlock != E_BLOCK_DIRT) || (DestMeta != E_META_DIRT_NORMAL))
			{
				// Not a regular dirt block
				continue;
			}

			BLOCKTYPE AboveDest;
			NIBBLETYPE AboveMeta;
			Chunk->GetBlockTypeMeta(BlockX, BlockY + 1, BlockZ, AboveDest, AboveMeta);
			if ((cBlockInfo::IsOneHitDig(AboveDest) || cBlockInfo::IsTransparent(AboveDest)) && !IsBlockWater(AboveDest))
			{
				if (!cRoot::Get()->GetPluginManager()->CallHookBlockSpread((cWorld*) &a_WorldInterface, BlockX * cChunkDef::Width, BlockY, BlockZ * cChunkDef::Width, ssGrassSpread))
				{
					Chunk->FastSetBlock(BlockX, BlockY, BlockZ, E_BLOCK_GRASS, 0);
				}
			}
		}  // for i - repeat twice
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.gravel";
	}
} ;




