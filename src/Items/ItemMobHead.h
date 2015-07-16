
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../BlockEntities/MobHeadEntity.h"





class cItemMobHeadHandler :
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemMobHeadHandler(int a_ItemType) :
		super(a_ItemType)
	{
	}


	virtual bool OnPlayerPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ
	) override
	{
		// Cannot place a head at "no face" and from the bottom:
		if ((a_BlockFace == BLOCK_FACE_NONE) || (a_BlockFace == BLOCK_FACE_BOTTOM))
		{
			return true;
		}
		auto placedX = a_BlockX, placedY = a_BlockY, placedZ = a_BlockZ;
		AddFaceDirection(placedY, placedY, placedZ, a_BlockFace);

		// If the placed head is a wither, try to spawn the wither first:
		if (a_EquippedItem.m_ItemDamage == E_META_HEAD_WITHER)
		{
			if (TrySpawnWitherAround(a_World, a_Player, placedX, placedY, placedZ))
			{
				return true;
			}
			// Wither not created, proceed with regular head placement
		}

		cItem itemCopy(a_EquippedItem);  // Make a copy in case this is the player's last head item and OnPlayerPlace removes it
		if (!super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ))
		{
			return false;
		}
		RegularHeadPlaced(a_World, a_Player, itemCopy, placedX, placedY, placedZ, a_BlockFace);
		return true;
	}


	/** Called after placing a regular head block with no mob spawning.
	Adjusts the mob head entity based on the equipped item's data. */
	void RegularHeadPlaced(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	)
	{
		// Use a callback to set the properties of the mob head block entity:
		class cCallback : public cBlockEntityCallback
		{
			cPlayer & m_Player;
			eMobHeadType m_HeadType;
			NIBBLETYPE m_BlockMeta;
			
			virtual bool Item(cBlockEntity * a_BlockEntity)
			{
				if (a_BlockEntity->GetBlockType() != E_BLOCK_HEAD)
				{
					return false;
				}
				auto MobHeadEntity = static_cast<cMobHeadEntity *>(a_BlockEntity);

				int Rotation = 0;
				if (m_BlockMeta == 1)
				{
					Rotation = FloorC(m_Player.GetYaw() * 16.0f / 360.0f + 0.5f) & 0x0f;
				}

				MobHeadEntity->SetType(m_HeadType);
				MobHeadEntity->SetRotation(static_cast<eMobHeadRotation>(Rotation));
				MobHeadEntity->GetWorld()->BroadcastBlockEntity(MobHeadEntity->GetPosX(), MobHeadEntity->GetPosY(), MobHeadEntity->GetPosZ());
				return false;
			}
		
		public:
			cCallback (cPlayer & a_CBPlayer, eMobHeadType a_HeadType, NIBBLETYPE a_BlockMeta) :
				m_Player(a_CBPlayer),
				m_HeadType(a_HeadType),
				m_BlockMeta(a_BlockMeta)
			{}
		};
		cCallback Callback(a_Player, static_cast<eMobHeadType>(a_EquippedItem.m_ItemDamage), static_cast<NIBBLETYPE>(a_BlockFace));
		a_World.DoWithBlockEntityAt(a_BlockX, a_BlockY, a_BlockZ, Callback);
	}


	/** Spawns a wither if the wither skull placed at the specified coords completes wither's spawning formula.
	Returns true if the wither was created. */
	bool TrySpawnWitherAround(
		cWorld & a_World, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ
	)
	{
		// No wither can be created at Y < 2 - not enough space for the formula:
		if (a_BlockY < 2)
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
		for (size_t i = 0; i < ARRAYCOUNT(RelCoords); ++i)
		{
			if (TrySpawnWitherAt(
				a_World, a_Player,
				a_BlockX, a_BlockY, a_BlockZ,
				RelCoords[i].x, RelCoords[i].z
			))
			{
				return true;
			}
		}  // for i - Coords[]

		return false;
	}


	/** Tries to spawn a wither at the specified offset from the placed head block.
	PlacedHead coords are used to override the block query - at those coords the block is not queried from the world,
	but assumed to be a head instead.
	Offset is used to shift the image around the X and Z axis.
	Returns true iff the wither was created successfully. */
	bool TrySpawnWitherAt(
		cWorld & a_World, cPlayer & a_Player,
		int a_PlacedHeadX, int a_PlacedHeadY, int a_PlacedHeadZ,
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
				a_World, a_Player, ImageWitherX, ARRAYCOUNT(ImageWitherX),
				a_PlacedHeadX, a_PlacedHeadY, a_PlacedHeadZ,
				a_OffsetX, a_OffsetZ
			) ||
			TrySpawnWitherFromImage(
				a_World, a_Player, ImageWitherZ, ARRAYCOUNT(ImageWitherZ),
				a_PlacedHeadX, a_PlacedHeadY, a_PlacedHeadZ,
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
		cWorld & a_World, cPlayer & a_Player, const sSetBlock * a_Image, size_t a_ImageCount,
		int a_PlacedHeadX, int a_PlacedHeadY, int a_PlacedHeadZ,
		int a_OffsetX, int a_OffsetZ
	)
	{
		// Check each block individually; simultaneously build the SetBlockVector for clearing the blocks:
		sSetBlockVector AirBlocks;
		AirBlocks.reserve(a_ImageCount);
		for (size_t i = 0; i < a_ImageCount; i++)
		{
			// Get the absolute coords of the image:
			int BlockX = a_PlacedHeadX + a_OffsetX + a_Image[i].GetX();
			int BlockY = a_PlacedHeadY + a_Image[i].GetY();
			int BlockZ = a_PlacedHeadZ + a_OffsetZ + a_Image[i].GetZ();

			// If the query is for the placed head, short-circuit-evaluate it:
			if ((BlockX == a_PlacedHeadX) && (BlockY == a_PlacedHeadY) && (BlockZ == a_PlacedHeadZ))
			{
				if (a_Image[i].m_BlockType != E_BLOCK_HEAD)
				{
					return false;  // Didn't match
				}
				continue;  // Matched, continue checking the rest of the image
			}

			// Query the world block:
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			if (!a_World.GetBlockTypeMeta(BlockX, BlockY, BlockZ, BlockType, BlockMeta))
			{
				// Cannot query block, assume unloaded chunk, fail to spawn the wither
				return false;
			}

			// Compare the world block:
			if (BlockType != a_Image[i].m_BlockType)
			{
				return false;
			}

			// If it is a mob head, check the correct head type using the block entity:
			if (BlockType == E_BLOCK_HEAD)
			{
				class cHeadCallback: public cBlockEntityCallback
				{
					virtual bool Item(cBlockEntity * a_Entity) override
					{
						ASSERT(a_Entity->GetBlockType() == E_BLOCK_HEAD);
						cMobHeadEntity * MobHead = static_cast<cMobHeadEntity *>(a_Entity);
						m_IsWitherHead = (MobHead->GetType() == SKULL_TYPE_WITHER);
						return true;
					}
				public:
					cHeadCallback(void):
						m_IsWitherHead(false)
					{
					}
					bool m_IsWitherHead;
				} callback;
				a_World.DoWithBlockEntityAt(BlockX, BlockY, BlockZ, callback);
				if (!callback.m_IsWitherHead)
				{
					return false;
				}
			}
			// Matched, continue checking
			AirBlocks.emplace_back(BlockX, BlockY, BlockZ, E_BLOCK_AIR, 0);
		}  // for i - a_Image

		// All image blocks matched, try replace the image with air blocks:
		if (!a_Player.PlaceBlocks(AirBlocks))
		{
			return false;
		}

		// Spawn the wither:
		int BlockX = a_PlacedHeadX + a_OffsetX;
		int BlockZ = a_PlacedHeadZ + a_OffsetZ;
		a_World.SpawnMob(static_cast<double>(BlockX) + 0.5, a_PlacedHeadY - 2, static_cast<double>(BlockZ) + 0.5, mtWither, false);
		AwardSpawnWitherAchievement(a_World, BlockX, a_PlacedHeadY - 2, BlockZ);
		return true;
	}


	/** Awards the achievement to all players close to the specified point. */
	void AwardSpawnWitherAchievement(cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		class cPlayerCallback : public cPlayerListCallback
		{
			Vector3f m_Pos;

			virtual bool Item(cPlayer * a_Player)
			{
				// If player is close, award achievement:
				double Dist = (a_Player->GetPosition() - m_Pos).Length();
				if (Dist < 50.0)
				{
					a_Player->AwardAchievement(achSpawnWither);
				}
				return false;
			}

		public:
			cPlayerCallback(const Vector3f & a_Pos) : m_Pos(a_Pos) {}
		} PlayerCallback(Vector3f(static_cast<float>(a_BlockX), static_cast<float>(a_BlockY), static_cast<float>(a_BlockZ)));
		a_World.ForEachPlayer(PlayerCallback);
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

	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = E_BLOCK_HEAD;
		a_BlockMeta = BlockFaceToBlockMeta(a_BlockFace);
		return true;
	}
} ;




