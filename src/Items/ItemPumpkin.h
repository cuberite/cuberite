
#pragma once

#include "ItemHandler.h"





class cItemPumpkinHandler:
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemPumpkinHandler(void):
		super(E_BLOCK_PUMPKIN)
	{
	}


	virtual bool OnPlayerPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ
	) override
	{
		// First try spawning a snow golem or an iron golem:
		int PlacedBlockX = a_BlockX;
		int PlacedBlockY = a_BlockY;
		int PlacedBlockZ = a_BlockZ;
		AddFaceDirection(PlacedBlockX, PlacedBlockY, PlacedBlockZ, a_BlockFace);
		if (TrySpawnGolem(a_World, a_Player, PlacedBlockX, PlacedBlockY, PlacedBlockZ))
		{
			// The client thinks that they placed the pumpkin, let them know it's been replaced:
			a_Player.SendBlocksAround(PlacedBlockX, PlacedBlockY, PlacedBlockZ);
			return true;
		}

		// No golem at these coords, place the block normally:
		return super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
	}


	/** Spawns a snow / iron golem if the shape matches the recipe, supposing that the block placed at the specified coords is a pumpkin.
	Returns true if the golem blocks are removed (for spawning), false if the recipe is not matched. */
	bool TrySpawnGolem(cWorld & a_World, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		// A golem can't form with a pumpkin below level 2 or above level 255
		if ((a_BlockY < 2) || (a_BlockY >= cChunkDef::Height))
		{
			return false;
		}

		// Decide which golem to try spawning based on the block below the placed pumpkin:
		switch (a_World.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ))
		{
			case E_BLOCK_SNOW_BLOCK: return TrySpawnSnowGolem(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ);
			case E_BLOCK_IRON_BLOCK: return TrySpawnIronGolem(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ);
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
	bool TrySpawnSnowGolem(cWorld & a_World, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		// Need one more snow block 2 blocks below the pumpkin:
		if (a_World.GetBlock(a_BlockX, a_BlockY - 2, a_BlockZ) != E_BLOCK_SNOW_BLOCK)
		{
			return false;
		}

		// Try to place air blocks where the original recipe blocks were:
		sSetBlockVector AirBlocks;
		AirBlocks.emplace_back(a_BlockX, a_BlockY,     a_BlockZ, E_BLOCK_AIR, 0);  // Head
		AirBlocks.emplace_back(a_BlockX, a_BlockY - 1, a_BlockZ, E_BLOCK_AIR, 0);  // Torso
		AirBlocks.emplace_back(a_BlockX, a_BlockY - 2, a_BlockZ, E_BLOCK_AIR, 0);  // Legs
		if (!a_Player.PlaceBlocks(AirBlocks))
		{
			return false;
		}

		// Spawn the golem:
		a_World.SpawnMob(static_cast<double>(a_BlockX) + 0.5, a_BlockY - 2, static_cast<double>(a_BlockZ) + 0.5, mtSnowGolem, false);
		return true;
	}


	/** Spawns an iron golem if the shape matches the recipe, supposing that the block placed at the specified coords is a pumpkin.
	Returns true if the golem blocks are removed (for spawning), false if the recipe is not matched.
	Assumes that the block below the specified block has already been checked and is an iron block. */
	bool TrySpawnIronGolem(cWorld & a_World, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		// Need one more iron block 2 blocks below the pumpkin:
		if (a_World.GetBlock(a_BlockX, a_BlockY - 2, a_BlockZ) != E_BLOCK_IRON_BLOCK)
		{
			return false;
		}

		// Check the two arm directions (X, Z) using a loop over two sets of offset vectors:
		static const Vector3i ArmOffsets[] =
		{
			{1, 0, 0},
			{0, 0, 1},
		};
		for (size_t i = 0; i < ARRAYCOUNT(ArmOffsets); i++)
		{
			// If the arm blocks don't match, bail out of this loop repetition:
			if (
				(a_World.GetBlock(a_BlockX + ArmOffsets[i].x, a_BlockY - 1, a_BlockZ + ArmOffsets[i].z) != E_BLOCK_IRON_BLOCK) ||
				(a_World.GetBlock(a_BlockX - ArmOffsets[i].x, a_BlockY - 1, a_BlockZ - ArmOffsets[i].z) != E_BLOCK_IRON_BLOCK)
			)
			{
				continue;
			}

			// Try to place air blocks where the original recipe blocks were:
			sSetBlockVector AirBlocks;
			AirBlocks.emplace_back(a_BlockX,                   a_BlockY,     a_BlockZ,                   E_BLOCK_AIR, 0);  // Head
			AirBlocks.emplace_back(a_BlockX,                   a_BlockY - 1, a_BlockZ,                   E_BLOCK_AIR, 0);  // Torso
			AirBlocks.emplace_back(a_BlockX,                   a_BlockY - 2, a_BlockZ,                   E_BLOCK_AIR, 0);  // Legs
			AirBlocks.emplace_back(a_BlockX + ArmOffsets[i].x, a_BlockY - 1, a_BlockZ + ArmOffsets[i].z, E_BLOCK_AIR, 0);  // Arm
			AirBlocks.emplace_back(a_BlockX - ArmOffsets[i].x, a_BlockY - 1, a_BlockZ - ArmOffsets[i].z, E_BLOCK_AIR, 0);  // Arm
			if (!a_Player.PlaceBlocks(AirBlocks))
			{
				return false;
			}

			// Spawn the golem:
			a_World.SpawnMob(static_cast<double>(a_BlockX) + 0.5, a_BlockY - 2, static_cast<double>(a_BlockZ) + 0.5, mtIronGolem, false);
			return true;
		}  // for i - ArmOffsets[]

		// Neither arm offset matched, this thing is not a complete golem
		return false;
	}
};
