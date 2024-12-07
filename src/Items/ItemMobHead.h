
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../BlockEntities/MobHeadEntity.h"





class cItemMobHeadHandler final :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;





	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		using namespace Block;

		switch (a_Player.GetEquippedItem().m_ItemType)
		{
			case Item::SkeletonSkull:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Player.PlaceBlock(a_PlacePosition, SkeletonSkull::SkeletonSkull(false, RotationToFineFace(a_Player.GetYaw())));
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Player.PlaceBlock(a_PlacePosition, SkeletonWallSkull::SkeletonWallSkull(RotationToBlockFace(a_Player.GetYaw()), false));
						break;
					}
					default: return false;
				}
				break;
			}
			case Item::WitherSkeletonSkull:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Player.PlaceBlock(a_PlacePosition, WitherSkeletonSkull::WitherSkeletonSkull(false, RotationToFineFace(a_Player.GetYaw())));
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Player.PlaceBlock(a_PlacePosition, WitherSkeletonWallSkull::WitherSkeletonWallSkull(RotationToBlockFace(a_Player.GetYaw()), false));
						break;
					}
					default: return false;
				}
				break;
			}
			case Item::ZombieHead:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Player.PlaceBlock(a_PlacePosition, ZombieHead::ZombieHead(false, RotationToFineFace(a_Player.GetYaw())));
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Player.PlaceBlock(a_PlacePosition, ZombieWallHead::ZombieWallHead(RotationToBlockFace(a_Player.GetYaw()), false));
						break;
					}
					default: return false;
				}
				break;
			}
			case Item::PlayerHead:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Player.PlaceBlock(a_PlacePosition, PlayerHead::PlayerHead(false, RotationToFineFace(a_Player.GetYaw())));
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Player.PlaceBlock(a_PlacePosition, PlayerWallHead::PlayerWallHead(RotationToBlockFace(a_Player.GetYaw()), false));
						break;
					}
					default: return false;
				}
				break;
			}
			case Item::CreeperHead:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Player.PlaceBlock(a_PlacePosition, CreeperHead::CreeperHead(false, RotationToFineFace(a_Player.GetYaw())));
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Player.PlaceBlock(a_PlacePosition, CreeperWallHead::CreeperWallHead(RotationToBlockFace(a_Player.GetYaw()), false));
						break;
					}
					default: return false;
				}
				break;
			}
			case Item::DragonHead:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Player.PlaceBlock(a_PlacePosition, CreeperHead::CreeperHead(false, RotationToFineFace(a_Player.GetYaw())));
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Player.PlaceBlock(a_PlacePosition, CreeperWallHead::CreeperWallHead(RotationToBlockFace(a_Player.GetYaw()), false));
						break;
					}
					default: return false;
				}
				break;
			}
			case Item::PiglinHead:
			{
				switch (a_ClickedBlockFace)
				{
					case BLOCK_FACE_YP:
					{
						a_Player.PlaceBlock(a_PlacePosition, PiglinHead::PiglinHead(false, RotationToFineFace(a_Player.GetYaw())));
						break;
					}
					case BLOCK_FACE_XM:
					case BLOCK_FACE_XP:
					case BLOCK_FACE_ZM:
					case BLOCK_FACE_ZP:
					{
						a_Player.PlaceBlock(a_PlacePosition, PiglinWallHead::PiglinWallHead(RotationToBlockFace(a_Player.GetYaw()), false));
						break;
					}
					default: return false;
				}
				break;
			}
			default:
			{
				ASSERT(!"UNHANDLED MOB HEAD TYPE IN ITEMMOBHEAD");
			}
		}
		return true;
	}





	/** Called after placing a regular head block with no mob spawning.
	Adjusts the mob head entity based on the equipped item's data. */
	void RegularHeadPlaced(const cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace) const
	{
		// Use a callback to set the properties of the mob head block entity:
		a_Player.GetWorld()->DoWithBlockEntityAt(a_PlacePosition, [&a_Player, a_HeldItem, a_ClickedBlockFace](cBlockEntity & a_BlockEntity)
		{
			switch (a_BlockEntity.GetBlockType())
			{
				case BlockType::CreeperHead:
				case BlockType::CreeperWallHead:
				case BlockType::DragonHead:
				case BlockType::DragonWallHead:
				case BlockType::PlayerHead:
				case BlockType::PlayerWallHead:
				case BlockType::SkeletonSkull:
				case BlockType::SkeletonWallSkull:
				case BlockType::WitherSkeletonSkull:
				case BlockType::WitherSkeletonWallSkull:
				case BlockType::ZombieHead:
				case BlockType::ZombieWallHead:
				case BlockType::PiglinHead:
				case BlockType::PiglinWallHead:
					break;
				default: ASSERT(false); return false;
			}

			auto & MobHeadEntity = static_cast<cMobHeadEntity &>(a_BlockEntity);

			int Rotation = 0;
			if (a_ClickedBlockFace == BLOCK_FACE_TOP)
			{
				Rotation = FloorC(a_Player.GetYaw() * 16.0f / 360.0f + 0.5f) & 0x0f;
			}

			MobHeadEntity.SetType(a_HeldItem.m_ItemType);
			MobHeadEntity.SetRotation(static_cast<eMobHeadRotation>(Rotation));
			return false;
		});
	}





	/** Spawns a wither if the wither skull placed at the specified coords completes wither's spawning formula.
	Returns true if the wither was created. */
	bool TrySpawnWitherAround(cPlayer & a_Player, const Vector3i a_BlockPos) const
	{
		// No wither can be created at Y < 2 - not enough space for the formula:
		if (a_BlockPos.y < 2)
		{
			return false;
		}

		static constexpr std::array<Vector3i, 4> SoulSandOffsetX =
		{
			Vector3i( 0, -1, 0),
			Vector3i(-1, -1, 0),
			Vector3i( 1, -1, 0),
			Vector3i( 0, -2, 0)
		};

		static constexpr std::array<Vector3i, 4> SoulSandOffsetZ =
		{
			Vector3i( 0, -1, 0),
			Vector3i(-1, -1, 0),
			Vector3i( 1, -1, 0),
			Vector3i( 0, -2, 0)
		};

		static constexpr std::array<Vector3i, 5> XHeadCoords =
		{
			Vector3i(-2, 0,  0),
			Vector3i(-1, 0,  0),
			Vector3i( 0, 0,  0),
			Vector3i( 1, 0,  0),
			Vector3i( 2, 0,  0),
		};

		static constexpr std::array<Vector3i, 5> ZHeadCoords =
		{
			Vector3i(0,  0, -2),
			Vector3i(0,  0, -1),
			Vector3i(0,  0,  0),
			Vector3i(0,  0,  1),
			Vector3i(0,  0,  2),
		};

		static constexpr std::array<Vector3i, 7> XAirPos =
		{
			Vector3i(-1,  0, 0),
			Vector3i( 0,  0, 0),
			Vector3i( 1,  0, 0),
			Vector3i(-1, -1, 0),
			Vector3i( 0, -1, 0),
			Vector3i( 1, -1, 0),
			Vector3i( 0, -2, 0)
		};

		static constexpr std::array<Vector3i, 7> ZAirPos =
		{
			Vector3i(0,  0, -1),
			Vector3i(0,  0,  0),
			Vector3i(0,  0,  1),
			Vector3i(0, -1, -1),
			Vector3i(0, -1,  0),
			Vector3i(0, -1,  1),
			Vector3i(0, -2,  0)
		};

		cBlockArea Area;
		Area.Read(*a_Player.GetWorld(), a_BlockPos - Vector3i(2, 2, 2), a_BlockPos + Vector3i(2, 0, 2), cBlockArea::baBlocks);

		auto RelHeadPos = Vector3i(3, 2, 3);
		Vector3i CenterHeadPos;

		if (FindCenterHead(Area, XHeadCoords, CenterHeadPos, RelHeadPos))
		{
			if (!ValidateSoulSand(Area, SoulSandOffsetX, RelHeadPos))
			{
				return false;
			}
			if (!ReplaceAir(a_Player, XAirPos, CenterHeadPos - RelHeadPos + a_BlockPos))
			{
				return false;
			}
		}
		else
		{
			if (!FindCenterHead(Area, ZHeadCoords, CenterHeadPos, RelHeadPos))
			{
				return false;
			}
			if (!ValidateSoulSand(Area, SoulSandOffsetZ, RelHeadPos))
			{
				return false;
			}
			if (!ReplaceAir(a_Player, ZAirPos, CenterHeadPos - RelHeadPos + a_BlockPos))
			{
				return false;
			}
		}


		// Spawn the wither:
		Vector3d WitherPos = CenterHeadPos - RelHeadPos + a_BlockPos + Vector3d(0.5, -2, 0.5);
		a_Player.GetWorld()->SpawnMob(WitherPos.x, WitherPos.y, WitherPos.z, mtWither, false);
		AwardSpawnWitherAchievement(a_Player.GetWorld(), CenterHeadPos - RelHeadPos + a_BlockPos);
		return true;
	}

	static bool FindCenterHead(cBlockArea & a_Area, const std::array<Vector3i, 5> & a_Offsets, Vector3i & a_CenterHeadPos, Vector3i a_StartPos)
	{
		int HeadCount = 0;
		for (const auto & Offset : a_Offsets)
		{
			switch (a_Area.GetRelBlock(a_StartPos + Offset).Type())
			{
				case BlockType::WitherSkeletonSkull: HeadCount += 1; break;
				default: HeadCount = 0;
			}
			if (HeadCount == 2)
			{
				a_CenterHeadPos = a_StartPos + Offset;
			}
		}
		return (HeadCount >= 3);
	}

	static bool ValidateSoulSand(cBlockArea & a_Area, const std::array<Vector3i, 4> a_Offsets, Vector3i a_StartPos)
	{
		for (const auto & Offset : a_Offsets)
		{
			if (a_Area.GetRelBlock(a_StartPos + Offset).Type() != BlockType::SoulSand)
			{
				return false;
			}
		}
		return true;
	}

	static bool ReplaceAir(cPlayer & a_Player, const std::array<Vector3i, 7> a_Offsets, Vector3i a_StartPos)
	{
		for (const auto & Offset : a_Offsets)
		{
			if (!a_Player.PlaceBlock(a_StartPos + Offset, Block::Air::Air()))
			{
				return false;
			}
		}
		return true;
	}





	/** Awards the achievement to all players close to the specified point. */
	void AwardSpawnWitherAchievement(cWorld * a_World, Vector3i a_BlockPos) const
	{
		Vector3f Pos(a_BlockPos);
		a_World->ForEachPlayer([=](cPlayer & a_Player)
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
} ;




