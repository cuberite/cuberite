
#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"
#include "../Root.h"
#include "../Bindings/PluginManager.h"





/** Handler used for all types of dirt and grassblock.
TODO: Split the Grassblock handler away from this class. */
class cBlockDirtHandler :
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockDirtHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		if (a_BlockMeta == E_META_DIRT_COARSE)
		{
			// Drop the coarse block (dirt, meta 1)
			return cItem(E_BLOCK_DIRT, 1, E_META_DIRT_COARSE);
		}
		else
		{
			return cItem(E_BLOCK_DIRT, 1, E_META_DIRT_NORMAL);
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
			BLOCKTYPE above = a_Chunk.GetBlock(a_RelX, a_RelY + 1, a_RelZ);

			// Grass turns back to dirt when the block above it is not transparent or water.
			// It does not turn to dirt when a snow layer is above.
			if ((above != E_BLOCK_SNOW) &&
				(!cBlockInfo::IsTransparent(above) || IsBlockWater(above)))
			{
				a_Chunk.FastSetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_DIRT, E_META_DIRT_NORMAL);
				return;
			}

			NIBBLETYPE light = std::max(a_Chunk.GetBlockLight(a_RelX, a_RelY + 1, a_RelZ), a_Chunk.GetTimeAlteredLight(a_Chunk.GetSkyLight(a_RelX, a_RelY + 1, a_RelZ)));
			// Source block is not bright enough to spread
			if (light < 9)
			{
				return;
			}

		}

		// Grass spreads to adjacent dirt blocks:
		auto & rand = GetRandomProvider();
		for (int i = 0; i < 2; i++)  // Pick two blocks to grow to
		{
			int OfsX = rand.RandInt(-1, 1);
			int OfsY = rand.RandInt(-3, 1);
			int OfsZ = rand.RandInt(-1, 1);

			BLOCKTYPE  DestBlock;
			NIBBLETYPE DestMeta;
			if (!cChunkDef::IsValidHeight(a_RelY + OfsY))
			{
				// Y Coord out of range
				continue;
			}
			Vector3i pos(a_RelX + OfsX, a_RelY + OfsY, a_RelZ + OfsZ);
			auto chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(pos);
			if (chunk == nullptr)
			{
				// Unloaded chunk
				continue;
			}
			chunk->GetBlockTypeMeta(pos, DestBlock, DestMeta);
			if ((DestBlock != E_BLOCK_DIRT) || (DestMeta != E_META_DIRT_NORMAL))
			{
				// Not a regular dirt block
				continue;
			}
			auto abovePos = pos.addedY(1);
			BLOCKTYPE above = chunk->GetBlock(abovePos);
			NIBBLETYPE light = std::max(chunk->GetBlockLight(abovePos), chunk->GetTimeAlteredLight(chunk->GetSkyLight(abovePos)));
			if ((light > 4)  &&
				cBlockInfo::IsTransparent(above) &&
				(!IsBlockLava(above)) &&
				(!IsBlockWaterOrIce(above))
			)
			{
				auto absPos = chunk->RelativeToAbsolute(pos);
				if (!cRoot::Get()->GetPluginManager()->CallHookBlockSpread(*chunk->GetWorld(), absPos.x, absPos.y, absPos.z, ssGrassSpread))
				{
					chunk->FastSetBlock(pos, E_BLOCK_GRASS, 0);
				}
			}
		}  // for i - repeat twice
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		switch (m_BlockType)
		{
			case E_BLOCK_DIRT: return 10;
			case E_BLOCK_GRASS: return 1;
			default:
			{
				ASSERT(!"Unhandled blocktype in dirt handler!");
				return 0;
			}
		}
	}
} ;




