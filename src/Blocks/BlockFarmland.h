
// BlockFarmland.h

// Declares the cBlcokFarmlandHandler representing the block handler for farmland





#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../BlockArea.h"
#include "../Chunk.h"
#include "../ClientHandle.h"





class cBlockFarmlandHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	/** Turns farmland into dirt.
	Will first check for any colliding entities and teleport them to a higher position.
	*/
	static void TurnToDirt(cChunk & a_Chunk, Vector3i a_AbsPos)
	{
		auto RelPos = cChunkDef::AbsoluteToRelative(a_AbsPos);
		TurnToDirt(a_Chunk, a_AbsPos, RelPos);
	}





	/** Turns farmland into dirt.
	Will first check for any colliding entities and teleport them to a higher position.
	*/
	static void TurnToDirt(cChunk & a_Chunk, const Vector3i a_AbsPos, const Vector3i a_RelPos)
	{
		// Use cBlockInfo::GetBlockHeight when it doesn't break trampling for
		// mobs and older clients anymore
		static const auto FarmlandHeight = 0.9375;
		static const auto FullHeightDelta = 0.0625;

		a_Chunk.ForEachEntityInBox(
			cBoundingBox(Vector3d(0.5, FarmlandHeight, 0.5) + a_AbsPos, 0.5, FullHeightDelta),
			[&](cEntity & Entity)
			{
				const auto GroundHeight = a_AbsPos.y + 1;

				// A simple IsOnGround isn't enough. It will return true when
				// e.g. a piston pushes a farmland block into an entity's head.
				// Maybe it's also possible than an entity is falling, it's
				// still not on the ground, but it's less than 0.0625 blocks
				// higher than the farmland block
				if ((Entity.GetPosY() < a_AbsPos.y + FarmlandHeight) || (Entity.GetPosY() >= GroundHeight))
				{
					return false;
				}

				// Players need a packet that will update their position
				if (Entity.IsPlayer())
				{
					const auto HeightIncrease = GroundHeight - Entity.GetPosY();
					const auto Player = static_cast<const cPlayer *>(&Entity);

					Player->GetClientHandle()->SendPlayerMoveLook(Vector3d(0.0, HeightIncrease, 0.0), 0.0f, 0.0f, true);
				}

				Entity.SetPosY(GroundHeight);

				return false;
			});

		a_Chunk.SetBlock(a_RelPos, BlockType::Dirt);
	}





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
		auto UpperBlock = cChunkDef::IsValidHeight(a_RelPos.addedY(1)) ? a_Chunk.GetBlock(a_RelPos.addedY(1)) : Block::Air::Air();
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
				auto AbsPos = a_Chunk.RelativeToAbsolute(a_RelPos);
				TurnToDirt(a_Chunk, AbsPos, a_RelPos);
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
		// TODO: fix for signs and slabs (possibly more blocks) - they should destroy farmland
		auto UpperBlock = a_ChunkInterface.GetBlock(a_BlockPos.addedY(1));
		if (cBlockInfo::FullyOccupiesVoxel(UpperBlock))
		{
			// Until the fix above is done, this line should also suffice:
			// a_ChunkInterface.SetBlock(a_BlockPos, E_BLOCK_DIRT, 0);
			a_ChunkInterface.DoWithChunkAt(a_BlockPos, [&](cChunk & Chunk)
			{
				TurnToDirt(Chunk, a_BlockPos);
				return true;
			});
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
		// Ref.: https://minecraft.wiki/w/Farmland#Hydration
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
