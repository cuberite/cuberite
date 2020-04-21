
#pragma once

#include "BlockHandler.h"
#include "../FastRandom.h"
#include "../Root.h"
#include "../Bindings/PluginManager.h"





class cBlockGrassHandler :
	public cBlockHandler
{
	using super = cBlockHandler;

public:

	cBlockGrassHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		if (!ToolHasSilkTouch(a_Tool))
		{
			return cItem(E_BLOCK_DIRT, 1, 0);
		}
		return cItem(E_BLOCK_GRASS, 1, 0);
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) override
	{
		if (!a_Chunk.GetWorld()->IsChunkLighted(a_Chunk.GetPosX(), a_Chunk.GetPosZ()))
		{
			a_Chunk.GetWorld()->QueueLightChunk(a_Chunk.GetPosX(), a_Chunk.GetPosZ());
			return;
		}
		auto AbovePos = a_RelPos.addedY(1);
		if (cChunkDef::IsValidHeight(AbovePos.y))
		{
			// Grass turns back to dirt when the block Above it is not transparent or water.
			// It does not turn to dirt when a snow layer is Above.
			auto Above = a_Chunk.GetBlock(AbovePos);
			if (
				(Above != E_BLOCK_SNOW) &&
				(!cBlockInfo::IsTransparent(Above) || IsBlockWater(Above)))
			{
				a_Chunk.FastSetBlock(a_RelPos, E_BLOCK_DIRT, E_META_DIRT_NORMAL);
				return;
			}

			// Make sure that there is enough light at the source block to spread
			auto light = std::max(a_Chunk.GetBlockLight(AbovePos), a_Chunk.GetTimeAlteredLight(a_Chunk.GetSkyLight(AbovePos)));
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
			auto Pos = a_RelPos + Vector3i(OfsX, OfsY, OfsZ);
			if (!cChunkDef::IsValidHeight(Pos.y))
			{
				// Y Coord out of range
				continue;
			}
			auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(Pos);
			if (Chunk == nullptr)
			{
				// Unloaded chunk
				continue;
			}
			Chunk->GetBlockTypeMeta(Pos, DestBlock, DestMeta);
			if ((DestBlock != E_BLOCK_DIRT) || (DestMeta != E_META_DIRT_NORMAL))
			{
				// Not a regular dirt block
				continue;
			}
			auto AboveDestPos = Pos.addedY(1);
			auto AboveBlockType = Chunk->GetBlock(AboveDestPos);
			auto Light = std::max(Chunk->GetBlockLight(AboveDestPos), Chunk->GetTimeAlteredLight(Chunk->GetSkyLight(AboveDestPos)));
			if ((Light > 4)  &&
				cBlockInfo::IsTransparent(AboveBlockType) &&
				(!IsBlockLava(AboveBlockType)) &&
				(!IsBlockWaterOrIce(AboveBlockType))
			)
			{
				auto AbsPos = Chunk->RelativeToAbsolute(Pos);
				if (!cRoot::Get()->GetPluginManager()->CallHookBlockSpread(*Chunk->GetWorld(), AbsPos.x, AbsPos.y, AbsPos.z, ssGrassSpread))
				{
					Chunk->FastSetBlock(Pos, E_BLOCK_GRASS, 0);
				}
			}
		}  // for i - repeat twice
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 1;
	}
} ;




