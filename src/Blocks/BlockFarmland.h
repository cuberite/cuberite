
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

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::Dirt);
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		auto Self = a_Chunk.GetBlock(a_RelPos);

		if (IsWaterInNear(a_Chunk, a_RelPos))
		{
			// Water was found, set block meta to 7
			a_Chunk.FastSetBlock(a_RelPos, Block::Farmland::Farmland(7));
			return;
		}

		auto Moisture = Block::Farmland::Moisture(Self);
		// Water wasn't found, de-hydrate block:
		if (Moisture > 0)
		{
			a_Chunk.FastSetBlock(a_RelPos, Block::Farmland::Farmland(--Moisture));
			return;
		}

		// Farmland too dry. If nothing is growing on top, turn back to dirt:
		auto UpperBlock = cChunkDef::IsValidHeight(a_RelPos.y + 1) ? a_Chunk.GetBlock(a_RelPos.addedY(1)) : Block::Air::Air();
		switch (UpperBlock.Type())
		{
			case BlockType::Beetroots:
			case BlockType::Carrots:
			case BlockType::MelonStem:
			case BlockType::AttachedMelonStem:
			case BlockType::Potatoes:
			case BlockType::PumpkinStem:
			case BlockType::AttachedPumpkinStem:
			case BlockType::Wheat:
			{
				// Produce on top, don't revert
				break;
			}
			default:
			{
				a_Chunk.SetBlock(a_RelPos, Block::Dirt::Dirt());
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
		auto UpperBlock = a_ChunkInterface.GetBlock(a_BlockPos.addedY(1));
		if (cBlockInfo::FullyOccupiesVoxel(UpperBlock))
		{
			a_ChunkInterface.SetBlock(a_BlockPos, Block::Dirt::Dirt());
		}
	}





	/** Returns true if there's either a water source block close enough to hydrate the specified position, or it's raining there. */
	static bool IsWaterInNear(const cChunk & a_Chunk, const Vector3i a_RelPos)
	{
		if (a_Chunk.IsWeatherWetAt(a_RelPos.addedY(1)))
		{
			// Rain hydrates farmland, too
			return true;
		}

		const auto WorldPos = a_Chunk.RelativeToAbsolute(a_RelPos);

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
		auto Blocks = Area.GetBlocks();
		for (size_t i = 0; i < NumBlocks; i++)
		{
			if (Blocks[i].Type() == BlockType::Water)
			{
				return true;
			}
		}  // for i - BlockTypes[]

		return false;
	}





	virtual bool CanSustainPlant(BlockState a_Plant) const override
	{
		switch (a_Plant.Type())
		{
			case BlockType::Beetroots:
			case BlockType::Wheat:
			case BlockType::Carrots:
			case BlockType::Potatoes:
			case BlockType::MelonStem:
			case BlockType::AttachedMelonStem:
			case BlockType::PumpkinStem:
			case BlockType::AttachedPumpkinStem:
				return true;
			default: return false;
		}
	}
} ;
