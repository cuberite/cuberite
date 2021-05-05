
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../BlockEntities/MobHeadEntity.h"





class cItemMobHeadHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemMobHeadHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// Cannot place a head at "no face" and from the bottom:
		if ((a_ClickedBlockFace == BLOCK_FACE_NONE) || (a_ClickedBlockFace == BLOCK_FACE_BOTTOM))
		{
			return false;
		}

		// If the placed head is a wither, try to spawn the wither first:
		if (a_HeldItem.m_ItemDamage == E_META_HEAD_WITHER)
		{
			if (TrySpawnWitherAround(a_Player, a_PlacePosition))
			{
				return true;
			}
			// Wither not created, proceed with regular head placement
		}

		if (!a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_HEAD, BlockFaceToBlockMeta(a_ClickedBlockFace)))
		{
			return false;
		}

		RegularHeadPlaced(a_Player, a_HeldItem, a_PlacePosition, a_ClickedBlockFace);
		return true;
	}





	/** Called after placing a regular head block with no mob spawning.
	Adjusts the mob head entity based on the equipped item's data. */
	void RegularHeadPlaced(const cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace)
	{
		const auto HeadType = static_cast<eMobHeadType>(a_HeldItem.m_ItemDamage);
		const auto BlockMeta = static_cast<NIBBLETYPE>(a_ClickedBlockFace);

		// Use a callback to set the properties of the mob head block entity:
		a_Player.GetWorld()->DoWithBlockEntityAt(a_PlacePosition, [&a_Player, HeadType, BlockMeta](cBlockEntity & a_BlockEntity)
		{
			ASSERT(a_BlockEntity.GetBlockType() == E_BLOCK_HEAD);

			auto & MobHeadEntity = static_cast<cMobHeadEntity &>(a_BlockEntity);

			int Rotation = 0;
			if (BlockMeta == 1)
			{
				Rotation = FloorC(a_Player.GetYaw() * 16.0f / 360.0f + 0.5f) & 0x0f;
			}

			MobHeadEntity.SetType(HeadType);
			MobHeadEntity.SetRotation(static_cast<eMobHeadRotation>(Rotation));
			return false;
		});
	}





	/** Spawns a wither if the wither skull placed at the specified coords completes wither's spawning formula.
	Returns true if the wither was created. */
	bool TrySpawnWitherAround(cPlayer & a_Player, const Vector3i a_BlockPos)
	{
		// No wither can be created at Y < 2 - not enough space for the formula:
		if (a_BlockPos.y < 2)
		{
			return false;
		}

		// Check for all relevant wither locations:
		static const Vector3i RelCoords[] =
		{
			{ 0, 0,  0},
			{ 1, 0,  0},
			{-1, 0,  0},
			{ 0, 0,  1},
			{ 0, 0, -1},
		};

		for (auto & RelCoord : RelCoords)
		{
			if (TrySpawnWitherAt(
				*a_Player.GetWorld(), a_Player,
				a_BlockPos,
				RelCoord.x, RelCoord.z
			))
			{
				return true;
			}
		}  // for i - RelCoords[]

		return false;
	}


	/** Tries to spawn a wither at the specified offset from the placed head block.
	PlacedHead coords are used to override the block query - at those coords the block is not queried from the world,
	but assumed to be a head instead.
	Offset is used to shift the image around the X and Z axis.
	Returns true iff the wither was created successfully. */
	bool TrySpawnWitherAt(
		cWorld & a_World, cPlayer & a_Player,
		Vector3i a_PlacedHeadPos,
		int a_OffsetX, int a_OffsetZ
	)
	{
		// Image for the wither at the X axis:
		static const sSetBlock ImageWitherX[] =
		{
			{-1,  0, 0, E_BLOCK_HEAD,     0},
			{ 0,  0, 0, E_BLOCK_HEAD,     0},
			{ 1,  0, 0, E_BLOCK_HEAD,     0},
			{-1, -1, 0, E_BLOCK_SOULSAND, 0},
			{ 0, -1, 0, E_BLOCK_SOULSAND, 0},
			{ 1, -1, 0, E_BLOCK_SOULSAND, 0},
			{-1, -2, 0, E_BLOCK_AIR,      0},
			{ 0, -2, 0, E_BLOCK_SOULSAND, 0},
			{ 1, -2, 0, E_BLOCK_AIR,      0},
		};

		// Image for the wither at the Z axis:
		static const sSetBlock ImageWitherZ[] =
		{
			{ 0,  0, -1, E_BLOCK_HEAD,     0},
			{ 0,  0,  0, E_BLOCK_HEAD,     0},
			{ 0,  0,  1, E_BLOCK_HEAD,     0},
			{ 0, -1, -1, E_BLOCK_SOULSAND, 0},
			{ 0, -1,  0, E_BLOCK_SOULSAND, 0},
			{ 0, -1,  1, E_BLOCK_SOULSAND, 0},
			{ 0, -2, -1, E_BLOCK_AIR,      0},
			{ 0, -2,  0, E_BLOCK_SOULSAND, 0},
			{ 0, -2,  1, E_BLOCK_AIR,      0},
		};

		// Try to spawn the wither from each image:
		return (
			TrySpawnWitherFromImage(
				a_World, a_Player, ImageWitherX,
				a_PlacedHeadPos,
				a_OffsetX, a_OffsetZ
			) ||
			TrySpawnWitherFromImage(
				a_World, a_Player, ImageWitherZ,
				a_PlacedHeadPos,
				a_OffsetX, a_OffsetZ
			)
		);
	}





	/** Tries to spawn a wither from the specified image at the specified offset from the placed head block.
	PlacedHead coords are used to override the block query - at those coords the block is not queried from the world,
	but assumed to be a head instead.
	Offset is used to shift the image around the X and Z axis.
	Returns true iff the wither was created successfully. */
	bool TrySpawnWitherFromImage(
		cWorld & a_World, cPlayer & a_Player, const sSetBlock (& a_Image)[9],
		Vector3i a_PlacedHeadPos,
		int a_OffsetX, int a_OffsetZ
	)
	{
		std::array<Vector3i, 9> PositionsToClear;

		// Check each block individually:
		for (size_t i = 0; i != std::size(a_Image); i++)
		{
			// The absolute coords of the block in the image to check.
			const Vector3i Block(
				a_PlacedHeadPos.x + a_OffsetX + a_Image[i].GetX(),
				a_PlacedHeadPos.y + a_Image[i].GetY(),
				a_PlacedHeadPos.z + a_OffsetZ + a_Image[i].GetZ()
			);

			// If the query is for the head the player is about to place (remember, it hasn't been set into the world yet), short-circuit-evaluate it:
			if (Block == a_PlacedHeadPos)
			{
				if (a_Image[i].m_BlockType != E_BLOCK_HEAD)
				{
					return false;  // Didn't match.
				}

				PositionsToClear[i] = Block;
				continue;  // Matched, continue checking the rest of the image.
			}

			// Query the world block:
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			if (!a_World.GetBlockTypeMeta(Block, BlockType, BlockMeta))
			{
				// Cannot query block, assume unloaded chunk, fail to spawn the wither
				return false;
			}

			// Compare the world block:
			if (BlockType != a_Image[i].m_BlockType)
			{
				return false;
			}

			// If it is a mob head, check it's a wither skull using the block entity:
			if (
				(BlockType == E_BLOCK_HEAD) &&
				!a_World.DoWithBlockEntityAt(Block, [&](cBlockEntity & a_BlockEntity)
				{
					ASSERT(a_BlockEntity.GetBlockType() == E_BLOCK_HEAD);

					return static_cast<cMobHeadEntity &>(a_BlockEntity).GetType() == SKULL_TYPE_WITHER;
				})
			)
			{
				return false;
			}

			// Matched, continue checking:
			PositionsToClear[i] = Block;
		}  // for i - a_Image

		// All image blocks matched, try replace the image with air blocks:
		if (
			!a_Player.PlaceBlocks(
			{
				{ PositionsToClear[0], E_BLOCK_AIR, 0 },
				{ PositionsToClear[1], E_BLOCK_AIR, 0 },
				{ PositionsToClear[2], E_BLOCK_AIR, 0 },
				{ PositionsToClear[3], E_BLOCK_AIR, 0 },
				{ PositionsToClear[4], E_BLOCK_AIR, 0 },
				{ PositionsToClear[5], E_BLOCK_AIR, 0 },
				{ PositionsToClear[6], E_BLOCK_AIR, 0 },
				{ PositionsToClear[7], E_BLOCK_AIR, 0 },
				{ PositionsToClear[8], E_BLOCK_AIR, 0 },
			})
		)
		{
			return false;
		}

		// Spawn the wither:
		int BlockX = a_PlacedHeadPos.x + a_OffsetX;
		int BlockZ = a_PlacedHeadPos.z + a_OffsetZ;
		a_World.SpawnMob(static_cast<double>(BlockX) + 0.5, a_PlacedHeadPos.y - 2, static_cast<double>(BlockZ) + 0.5, mtWither, false);
		AwardSpawnWitherAchievement(a_World, {BlockX, a_PlacedHeadPos.y - 2, BlockZ});
		return true;
	}





	/** Awards the achievement to all players close to the specified point. */
	void AwardSpawnWitherAchievement(cWorld & a_World, Vector3i a_BlockPos)
	{
		Vector3f Pos(a_BlockPos);
		a_World.ForEachPlayer([=](cPlayer & a_Player)
			{
				// If player is close, award achievement:
				double Dist = (a_Player.GetPosition() - Pos).Length();
				if (Dist < 50.0)
				{
					a_Player.AwardAchievement(CustomStatistic::AchSpawnWither);
				}
				return false;
			}
		);
	}





	/** Converts the block face of the placement (which face of the block was clicked to place the head)
	into the block's metadata value. */
	static NIBBLETYPE BlockFaceToBlockMeta(int a_BlockFace)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_TOP: return 0x01;  // On ground (rotation provided in block entity)
			case BLOCK_FACE_XM:  return 0x04;  // west wall, facing east
			case BLOCK_FACE_XP:  return 0x05;  // east wall, facing west
			case BLOCK_FACE_ZM:  return 0x02;  // north wall, facing south
			case BLOCK_FACE_ZP:  return 0x03;  // south wall, facing north
			default:
			{
				ASSERT(!"Unhandled block face");
				return 0;
			}
		}
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}
} ;




