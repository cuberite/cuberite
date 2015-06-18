
#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"
#include "Root.h"
#include "Bindings/PluginManager.h"



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
		if (a_BlockMeta == E_META_DIRT_COARSE)
		{
			// Drop the coarse block (dirt, meta 1)
			a_Pickups.Add(E_BLOCK_DIRT, 1, E_META_DIRT_COARSE);
		}
		else
		{
			a_Pickups.Add(E_BLOCK_DIRT, 1, E_META_DIRT_NORMAL);
		}
	}
	
	
	virtual void OnUpdate(cChunkInterface & cChunkInterface, cWorldInterface & a_WorldInterface, cBlockPluginInterface & a_PluginInterface, cChunk & a_Chunk, int a_RelX, int a_RelY, int a_RelZ) override
	{
		if (m_BlockType != E_BLOCK_GRASS)
		{
			return;
		}

		// Make sure that there is enough light at the source block to spread
		if (!a_Chunk.GetWorld()->IsChunkLighted(a_Chunk.GetPosX(), a_Chunk.GetPosZ()))
		{
			a_Chunk.GetWorld()->QueueLightChunk(a_Chunk.GetPosX(), a_Chunk.GetPosZ());
			return;
		}
		else if ((a_RelY < cChunkDef::Height - 1))
		{
			NIBBLETYPE light = std::max(a_Chunk.GetBlockLight(a_RelX, a_RelY + 1, a_RelZ), a_Chunk.GetTimeAlteredLight(a_Chunk.GetSkyLight(a_RelX, a_RelY + 1, a_RelZ)));
			// Grass turns back to dirt when light levels are below 5
			if (light < 5)
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_DIRT, E_META_DIRT_NORMAL);
				return;
			}
			// Source block is not bright enough to spread
			if (light < 9)
			{
				return;
			}
			
		}

		// Grass spreads to adjacent dirt blocks:
		cFastRandom rand;
		for (int i = 0; i < 2; i++)  // Pick two blocks to grow to
		{
			int OfsX = rand.NextInt(3) - 1;  // [-1 .. 1]
			int OfsY = rand.NextInt(5) - 3;  // [-3 .. 1]
			int OfsZ = rand.NextInt(3) - 1;  // [-1 .. 1]
	
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
			if (Chunk == nullptr)
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

			NIBBLETYPE light = std::max(a_Chunk.GetBlockLight(BlockX, BlockY + 1, BlockZ), a_Chunk.GetTimeAlteredLight(a_Chunk.GetSkyLight(BlockX, BlockY + 1, BlockZ)));
			// Grass does not spread to blocks with a light level less than 5
			if (light > 4)
			{
				if (!cRoot::Get()->GetPluginManager()->CallHookBlockSpread(*Chunk->GetWorld(), Chunk->GetPosX() * cChunkDef::Width + BlockX, BlockY, Chunk->GetPosZ() * cChunkDef::Width + BlockZ, ssGrassSpread))
				{
					Chunk->FastSetBlock(BlockX, BlockY, BlockZ, E_BLOCK_GRASS, 0);
				}
			}
		}  // for i - repeat twice
	}
} ;




