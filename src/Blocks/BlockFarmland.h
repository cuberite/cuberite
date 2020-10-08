
// BlockFarmland.h

// Declares the cBlcokFarmlandHandler representing the block handler for farmland





#pragma once

#include "BlockHandler.h"
#include "../BlockArea.h"





class cBlockFarmlandHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(E_BLOCK_DIRT, 1, 0);
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		auto BlockMeta = a_Chunk.GetMeta(a_RelPos);

		if (IsWaterInNear(a_Chunk, a_RelPos))
		{
			// Water was found, set block meta to 7
			a_Chunk.FastSetBlock(a_RelPos, m_BlockType, 7);
			return;
		}

		// Water wasn't found, de-hydrate block:
		if (BlockMeta > 0)
		{
			a_Chunk.FastSetBlock(a_RelPos, E_BLOCK_FARMLAND, --BlockMeta);
			return;
		}

		// Farmland too dry. If nothing is growing on top, turn back to dirt:
		auto UpperBlock = cChunkDef::IsValidHeight(a_RelPos.y + 1) ? a_Chunk.GetBlock(a_RelPos.addedY(1)) : E_BLOCK_AIR;
		switch (UpperBlock)
		{
			case E_BLOCK_BEETROOTS:
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
				a_Chunk.SetBlock(a_RelPos, E_BLOCK_DIRT, 0);
				break;
			}
		}
	}





	virtual void OnNeighborChanged(cChunkInterface & a_ChunkInterface, Vector3i a_BlockPos, eBlockFace a_WhichNeighbor) const override
	{
		// Don't care about any neighbor but the one above us (fix recursion loop in #2213):
		if (a_WhichNeighbor != BLOCK_FACE_YP)
		{
			return;
		}

		// Don't care about anything if we're at the top of the world:
		if (a_BlockPos.y >= cChunkDef::Height)
		{
			return;
		}

		// Check whether we should revert to dirt:
		auto upperBlock = a_ChunkInterface.GetBlock(a_BlockPos.addedY(1));
		if (cBlockInfo::FullyOccupiesVoxel(upperBlock))
		{
			a_ChunkInterface.SetBlock(a_BlockPos, E_BLOCK_DIRT, 0);
		}
	}





	/** Returns true if there's either a water source block close enough to hydrate the specified position, or it's raining there. */
	static bool IsWaterInNear(const cChunk & a_Chunk, const Vector3i a_RelPos)
	{
		const auto WorldPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		if (a_Chunk.GetWorld()->IsWeatherWetAtXYZ(WorldPos))
		{
			// Rain hydrates farmland, too
			return true;
		}

		// Search for water in a close proximity:
		// Ref.: https://minecraft.gamepedia.com/Farmland#Hydration
		// TODO: Rewrite this to use the chunk and its neighbors directly
		cBlockArea Area;
		if (!Area.Read(*a_Chunk.GetWorld(), WorldPos - Vector3i(4, 0, 4), WorldPos + Vector3i(4, 1, 4)))
		{
			// Too close to the world edge, cannot check surroundings
			return false;
		}

		size_t NumBlocks = Area.GetBlockCount();
		BLOCKTYPE * BlockTypes = Area.GetBlockTypes();
		for (size_t i = 0; i < NumBlocks; i++)
		{
			if (IsBlockWater(BlockTypes[i]))
			{
				return true;
			}
		}  // for i - BlockTypes[]

		return false;
	}





	virtual bool CanSustainPlant(BLOCKTYPE a_Plant) const override
	{
		return (
			(a_Plant == E_BLOCK_BEETROOTS) ||
			(a_Plant == E_BLOCK_CROPS) ||
			(a_Plant == E_BLOCK_CARROTS) ||
			(a_Plant == E_BLOCK_POTATOES) ||
			(a_Plant == E_BLOCK_MELON_STEM) ||
			(a_Plant == E_BLOCK_PUMPKIN_STEM)
		);
	}
} ;
