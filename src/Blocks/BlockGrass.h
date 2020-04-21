
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

		if (!cBlockGrassHandler::TrySurvive(a_Chunk, a_RelPos))
		{
			return;
		}

		// Grass spreads to adjacent dirt blocks:
		for (unsigned i = 0; i < 2; i++)  // Pick two blocks to grow to
		{
			auto & Random = GetRandomProvider();
			int OfsX = Random.RandInt(-1, 1);
			int OfsY = Random.RandInt(-3, 1);
			int OfsZ = Random.RandInt(-1, 1);

			cBlockGrassHandler::TryThrive(a_Chunk, a_RelPos + Vector3i(OfsX, OfsY, OfsZ));
		}  // for i - repeat twice
	}





	/** Check if conditions are favourable to a grass block at the given position.
	If they are not, the grass dies and is turned to dirt.
	Returns whether conditions are so good that the grass can try to spread to neighbours. */
	static bool TrySurvive(cChunk & a_Chunk, const Vector3i a_RelPos)
	{
		const auto AbovePos = a_RelPos.addedY(1);
		if (!cChunkDef::IsValidHeight(AbovePos.y))
		{
			return true;
		}

		// Grass turns back to dirt when the block Above it is not transparent or water.
		// It does not turn to dirt when a snow layer is Above.
		const auto Above = a_Chunk.GetBlock(AbovePos);
		if (
			(Above != E_BLOCK_SNOW) &&
			(!cBlockInfo::IsTransparent(Above) || IsBlockWater(Above)))
		{
			a_Chunk.FastSetBlock(a_RelPos, E_BLOCK_DIRT, E_META_DIRT_NORMAL);
			return false;
		}

		// Make sure that there is enough light at the source block to spread
		const auto Light = std::max(a_Chunk.GetBlockLight(AbovePos), a_Chunk.GetSkyLightAltered(AbovePos));
		return (Light >= 9);
	}





	/** Attempt to spread grass to a block at the given position. */
	static void TryThrive(cChunk & a_Chunk, Vector3i a_RelPos)
	{
		if (!cChunkDef::IsValidHeight(a_RelPos.y))
		{
			// Y Coord out of range
			return;
		}

		auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(a_RelPos);
		if (Chunk == nullptr)
		{
			// Unloaded chunk
			return;
		}

		BLOCKTYPE  DestBlock;
		NIBBLETYPE DestMeta;
		Chunk->GetBlockTypeMeta(a_RelPos, DestBlock, DestMeta);

		if ((DestBlock != E_BLOCK_DIRT) || (DestMeta != E_META_DIRT_NORMAL))
		{
			// Not a regular dirt block
			return;
		}

		const auto AbovePos = a_RelPos.addedY(1);
		const auto Above = Chunk->GetBlock(AbovePos);
		const auto Light = std::max(Chunk->GetBlockLight(AbovePos), Chunk->GetSkyLightAltered(AbovePos));

		if (
			(Light > 4) &&
			cBlockInfo::IsTransparent(Above) &&
			!IsBlockLava(Above) &&
			!IsBlockWaterOrIce(Above)
		)
		{
			const auto AbsPos = Chunk->RelativeToAbsolute(a_RelPos);
			if (!cRoot::Get()->GetPluginManager()->CallHookBlockSpread(*Chunk->GetWorld(), AbsPos.x, AbsPos.y, AbsPos.z, ssGrassSpread))
			{
				Chunk->FastSetBlock(a_RelPos, E_BLOCK_GRASS, 0);
			}
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 1;
	}
} ;
