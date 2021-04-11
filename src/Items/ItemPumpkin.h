
#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockPumpkin.h"





class cItemPumpkinHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// First try spawning a snow golem or an iron golem:
		if (TrySpawnGolem(a_Player, a_PlacePosition))
		{
			// The client thinks that they placed the pumpkin, let them know it's been replaced:
			a_Player.SendBlocksAround(a_PlacePosition.x, a_PlacePosition.y, a_PlacePosition.z);
			return true;
		}

		// No golem at these coords, place the block normally:
		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_PUMPKIN, cBlockPumpkinHandler::YawToMetaData(a_Player.GetYaw()));
	}





	/** Spawns a snow / iron golem if the shape matches the recipe, supposing that the block placed at the specified coords is a pumpkin.
	Returns true if the golem blocks are removed (for spawning), false if the recipe is not matched. */
	bool TrySpawnGolem(cPlayer & a_Player, const Vector3i a_PumpkinPos)
	{
		// A golem can't form with a pumpkin below level 2 or above level 255:
		if ((a_PumpkinPos.y < 2) || (a_PumpkinPos.y >= cChunkDef::Height))
		{
			return false;
		}

		auto & World = *a_Player.GetWorld();

		// Decide which golem to try spawning based on the block below the placed pumpkin:
		switch (World.GetBlock(a_PumpkinPos.addedY(-1)))
		{
			case E_BLOCK_SNOW_BLOCK: return TrySpawnSnowGolem(World, a_Player, a_PumpkinPos);
			case E_BLOCK_IRON_BLOCK: return TrySpawnIronGolem(World, a_Player, a_PumpkinPos);
			default:
			{
				// No golem here:
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
		ASSERT(a_World.GetBlock(a_PumpkinPos.addedY(-1)) == E_BLOCK_SNOW_BLOCK);

		// Need one more snow block 2 blocks below the pumpkin:
		if (a_World.GetBlock(a_PumpkinPos.addedY(-2)) != E_BLOCK_SNOW_BLOCK)
		{
			return false;
		}

		// Try to place air blocks where the original recipe blocks were:
		if (
			!a_Player.PlaceBlocks(
			{
				{ a_PumpkinPos,            E_BLOCK_AIR, 0 },  // Head
				{ a_PumpkinPos.addedY(-1), E_BLOCK_AIR, 0 },  // Torso
				{ a_PumpkinPos.addedY(-2), E_BLOCK_AIR, 0 }	  // Legs
			})
		)
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
		ASSERT(a_World.GetBlock(a_PumpkinPos.addedY(-1)) == E_BLOCK_IRON_BLOCK);

		// Need one more iron block 2 blocks below the pumpkin:
		if (a_World.GetBlock(a_PumpkinPos.addedY(-2)) != E_BLOCK_IRON_BLOCK)
		{
			return false;
		}

		// Check the two arm directions (X, Z) using a loop over two sets of offset vectors:
		auto BodyPos = a_PumpkinPos.addedY(-1);
		static const Vector3i ArmOffsets[] =
		{
			{1, 0, 0},
			{0, 0, 1},
		};
		for (size_t i = 0; i < ARRAYCOUNT(ArmOffsets); i++)
		{
			// If the arm blocks don't match, bail out of this loop repetition:
			if (
				(a_World.GetBlock(BodyPos + ArmOffsets[i]) != E_BLOCK_IRON_BLOCK) ||
				(a_World.GetBlock(BodyPos - ArmOffsets[i]) != E_BLOCK_IRON_BLOCK)
			)
			{
				continue;
			}

			// Try to place air blocks where the original recipe blocks were:
			if (
				!a_Player.PlaceBlocks(
				{
					{ a_PumpkinPos,            E_BLOCK_AIR, 0 },  // Head
					{ BodyPos,                 E_BLOCK_AIR, 0 },  // Torso
					{ BodyPos.addedY(-1),      E_BLOCK_AIR, 0 },  // Legs
					{ BodyPos + ArmOffsets[i], E_BLOCK_AIR, 0 },  // Arm
					{ BodyPos - ArmOffsets[i], E_BLOCK_AIR, 0 }   // Arm
				})
			)
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
