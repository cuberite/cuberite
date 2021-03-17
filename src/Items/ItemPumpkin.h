
#pragma once

#include "ItemHandler.h"





class cItemPumpkinHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemPumpkinHandler():
		Super(E_BLOCK_PUMPKIN)
	{
	}





	virtual bool OnPlayerPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) override
	{
		// First try spawning a snow golem or an iron golem:
		auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		if (TrySpawnGolem(a_World, a_Player, PlacePos))
		{
			// The client thinks that they placed the pumpkin, let them know it's been replaced:
			a_Player.SendBlocksAround(PlacePos.x, PlacePos.y, PlacePos.z);
			return true;
		}

		// No golem at these coords, place the block normally:
		return Super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_ClickedBlockPos, a_ClickedBlockFace, a_CursorPos);
	}





	/** Spawns a snow / iron golem if the shape matches the recipe, supposing that the block placed at the specified coords is a pumpkin.
	Returns true if the golem blocks are removed (for spawning), false if the recipe is not matched. */
	bool TrySpawnGolem(cWorld & a_World, cPlayer & a_Player, const Vector3i a_PumpkinPos)
	{
		// A golem can't form with a pumpkin below level 2 or above level 255
		if ((a_PumpkinPos.y < 2) || (a_PumpkinPos.y >= cChunkDef::Height))
		{
			return false;
		}

		// Decide which golem to try spawning based on the block below the placed pumpkin:
		switch (a_World.GetBlock(a_PumpkinPos.addedY(-1)).Type())
		{
			case BlockType::SnowBlock: return TrySpawnSnowGolem(a_World, a_Player, a_PumpkinPos);
			case BlockType::IronBlock: return TrySpawnIronGolem(a_World, a_Player, a_PumpkinPos);
			default:
			{
				// No golem here
				return false;
			}
		}
	}





	/** Spawns a snow golem if the shape matches the recipe, supposing that the block placed at the specified coords is a pumpkin.
	Returns true if the golem blocks are removed (for spawning), false if the recipe is not matched.
	Assumes that the block below the specified block has already been checked and is a snow block. */
	bool TrySpawnSnowGolem(cWorld & a_World, cPlayer & a_Player, const Vector3i a_PumpkinPos)
	{
		ASSERT(a_PumpkinPos.y > 1);
		ASSERT(a_World.GetBlock(a_PumpkinPos.addedY(-1)).Type() == BlockType::SnowBlock);

		// Need one more snow block 2 blocks below the pumpkin:
		if (a_World.GetBlock(a_PumpkinPos.addedY(-2)).Type() != BlockType::SnowBlock)
		{
			return false;
		}

		// Try to place air blocks where the original recipe blocks were:
		sSetBlockVector AirBlocks;
		AirBlocks.emplace_back(a_PumpkinPos,            Block::Air::Air());  // Head
		AirBlocks.emplace_back(a_PumpkinPos.addedY(-1), Block::Air::Air());  // Torso
		AirBlocks.emplace_back(a_PumpkinPos.addedY(-2), Block::Air::Air());  // Legs
		if (!a_Player.PlaceBlocks(AirBlocks))
		{
			return false;
		}

		// Spawn the golem:
		auto GolemPos = Vector3d(a_PumpkinPos) + Vector3d(0.5, -2, 0.5);
		a_World.SpawnMob(GolemPos.x, GolemPos.y, GolemPos.z, mtSnowGolem, false);
		return true;
	}





	/** Spawns an iron golem if the shape matches the recipe, supposing that the block placed at the specified coords is a pumpkin.
	Returns true if the golem blocks are removed (for spawning), false if the recipe is not matched.
	Assumes that the block below the specified block has already been checked and is an iron block. */
	bool TrySpawnIronGolem(cWorld & a_World, cPlayer & a_Player, const Vector3i a_PumpkinPos)
	{
		ASSERT(a_PumpkinPos.y > 1);
		ASSERT(a_World.GetBlock(a_PumpkinPos.addedY(-1)).Type() == BlockType::IronBlock);

		// Need one more iron block 2 blocks below the pumpkin:
		if (a_World.GetBlock(a_PumpkinPos.addedY(-2)).Type() != BlockType::IronBlock)
		{
			return false;
		}

		// Check the two arm directions (X, Z) using a loop over two sets of offset vectors:
		auto BodyPos = a_PumpkinPos.addedY(-1);
		static const std::array<Vector3i, 2> ArmOffsets =
		{
			Vector3i(1, 0, 0),
			Vector3i(0, 0, 1)
		};
		for (const auto & Offset : ArmOffsets)
		{
			// If the arm blocks don't match, bail out of this loop repetition:
			if (
				(a_World.GetBlock(BodyPos + Offset).Type() != BlockType::IronBlock) ||
				(a_World.GetBlock(BodyPos - Offset).Type() != BlockType::IronBlock)
			)
			{
				continue;
			}

			// Try to place air blocks where the original recipe blocks were:
			sSetBlockVector AirBlocks;
			AirBlocks.emplace_back(a_PumpkinPos,       Block::Air::Air());  // Head
			AirBlocks.emplace_back(BodyPos,            Block::Air::Air());  // Torso
			AirBlocks.emplace_back(BodyPos.addedY(-1), Block::Air::Air());  // Legs
			AirBlocks.emplace_back(BodyPos + Offset,   Block::Air::Air());  // Arm
			AirBlocks.emplace_back(BodyPos - Offset,   Block::Air::Air());  // Arm
			if (!a_Player.PlaceBlocks(AirBlocks))
			{
				return false;
			}

			// Spawn the golem:
			auto GolemPos = Vector3d(a_PumpkinPos) + Vector3d(0.5, -2, 0.5);
			a_World.SpawnMob(GolemPos.x, GolemPos.y, GolemPos.z, mtIronGolem, false);
			return true;
		}  // for i - ArmOffsets[]

		// Neither arm offset matched, this thing is not a complete golem
		return false;
	}
};
