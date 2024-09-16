
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockDoor.h"





class cItemDoorHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		// Vanilla only allows door placement while clicking on the top face of the block below the door:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		// Get the block type of the door to place:
		BlockType BlockType;
		switch (m_ItemType)
		{
			case Item::OakDoor:      BlockType = BlockType::OakDoor;     break;
			case Item::IronDoor:     BlockType = BlockType::IronDoor;    break;
			case Item::SpruceDoor:   BlockType = BlockType::SpruceDoor;  break;
			case Item::BirchDoor:    BlockType = BlockType::BirchDoor;   break;
			case Item::JungleDoor:   BlockType = BlockType::JungleDoor;  break;
			case Item::DarkOakDoor:  BlockType = BlockType::DarkOakDoor; break;
			case Item::AcaciaDoor:   BlockType = BlockType::AcaciaDoor;  break;
			default:
			{
				UNREACHABLE("Unhandled door type");
				BlockType = BlockType::Air;
				break;
			}
		}

		const auto & World = *a_Player.GetWorld();
		const auto UpperBlockPosition = a_PlacePosition.addedY(1);

		// Check the block that will get replaced by the door:
		{
			auto UpperReplacedBlock = World.GetBlock(UpperBlockPosition);

			if (!cBlockHandler::For(UpperReplacedBlock.Type()).DoesIgnoreBuildCollision(World, a_HeldItem, UpperBlockPosition, UpperReplacedBlock, a_ClickedBlockFace, false))
			{
				return false;
			}
		}

		// Get the coords of the neighboring blocks:
		auto Facing = RotationToBlockFace(a_Player.GetYaw());
		Vector3i RelDirToOutside = cBlockDoorHandler::GetRelativeDirectionToOutside(World.GetBlock(a_PlacePosition));
		Vector3i LeftNeighborPos = RelDirToOutside;
		LeftNeighborPos.TurnCW();
		LeftNeighborPos.Move(a_PlacePosition);
		Vector3i RightNeighborPos = RelDirToOutside;
		RightNeighborPos.TurnCCW();
		RightNeighborPos.Move(a_PlacePosition);

		// Decide whether the hinge is on the left (default) or on the right:
		auto LeftNeighborBlock = World.GetBlock(LeftNeighborPos);
		auto RightNeighborBlock = World.GetBlock(RightNeighborPos);
		bool HingeRight = false;
		/*
		// DEBUG:
		FLOGD("Door being placed at {0}", a_PlacePosition);
		FLOGD("RelDirToOutside: {0}", RelDirToOutside);
		FLOGD("Left neighbor at {0}: {1} ({2})", LeftNeighborPos, LeftNeighborBlock, ItemTypeToString(LeftNeighborBlock));
		FLOGD("Right neighbor at {0}: {1} ({2})", RightNeighborPos, RightNeighborBlock, ItemTypeToString(RightNeighborBlock));
		*/

		if (
			cBlockDoorHandler::IsBlockDoor(LeftNeighborBlock) ||     // The block to the left is a door block
			(
				!cBlockInfo::IsSolid(LeftNeighborBlock) &&           // Prioritize hinge on the left side
				cBlockInfo::IsSolid(RightNeighborBlock) &&           // The block to the right is solid...
				!cBlockDoorHandler::IsBlockDoor(RightNeighborBlock)  // ... but not a door
			)
		)
		{
			// DEBUG: LOGD("Setting hinge to right side");
			HingeRight = true;
		}

		using namespace Block;

		switch (BlockType)
		{
			case BlockType::AcaciaDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, AcaciaDoor::AcaciaDoor(Facing, AcaciaDoor::Half::Lower, HingeRight ? AcaciaDoor::Hinge::Right : AcaciaDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition,    AcaciaDoor::AcaciaDoor(Facing, AcaciaDoor::Half::Upper, HingeRight ? AcaciaDoor::Hinge::Right : AcaciaDoor::Hinge::Left, false, false) }
				});
			}
			case BlockType::BirchDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, BirchDoor::BirchDoor(Facing, BirchDoor::Half::Lower, HingeRight ? BirchDoor::Hinge::Right : BirchDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition,    BirchDoor::BirchDoor(Facing, BirchDoor::Half::Upper, HingeRight ? BirchDoor::Hinge::Right : BirchDoor::Hinge::Left, false, false) }
				});
			}
			case BlockType::CrimsonDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, CrimsonDoor::CrimsonDoor(Facing, CrimsonDoor::Half::Lower, HingeRight ? CrimsonDoor::Hinge::Right : CrimsonDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition,    CrimsonDoor::CrimsonDoor(Facing, CrimsonDoor::Half::Upper, HingeRight ? CrimsonDoor::Hinge::Right : CrimsonDoor::Hinge::Left, false, false) }
				});
			}
			case BlockType::DarkOakDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, CrimsonDoor::CrimsonDoor(Facing, CrimsonDoor::Half::Lower, HingeRight ? CrimsonDoor::Hinge::Right : CrimsonDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition,    CrimsonDoor::CrimsonDoor(Facing, CrimsonDoor::Half::Upper, HingeRight ? CrimsonDoor::Hinge::Right : CrimsonDoor::Hinge::Left, false, false) }
				});
			}
			case BlockType::JungleDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, JungleDoor::JungleDoor(Facing, JungleDoor::Half::Lower, HingeRight ? JungleDoor::Hinge::Right : JungleDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition,    JungleDoor::JungleDoor(Facing, JungleDoor::Half::Upper, HingeRight ? JungleDoor::Hinge::Right : JungleDoor::Hinge::Left, false, false) }
				});
			}
			case BlockType::OakDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, OakDoor::OakDoor(Facing, OakDoor::Half::Lower, HingeRight ? OakDoor::Hinge::Right : OakDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition,    OakDoor::OakDoor(Facing, OakDoor::Half::Upper, HingeRight ? OakDoor::Hinge::Right : OakDoor::Hinge::Left, false, false) }
				});
			}
			case BlockType::SpruceDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, SpruceDoor::SpruceDoor(Facing, SpruceDoor::Half::Lower, HingeRight ? SpruceDoor::Hinge::Right : SpruceDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition,    SpruceDoor::SpruceDoor(Facing, SpruceDoor::Half::Upper, HingeRight ? SpruceDoor::Hinge::Right : SpruceDoor::Hinge::Left, false, false) }
				});
			}
			case BlockType::WarpedDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, WarpedDoor::WarpedDoor(Facing, WarpedDoor::Half::Lower, HingeRight ? WarpedDoor::Hinge::Right : WarpedDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition,    WarpedDoor::WarpedDoor(Facing, WarpedDoor::Half::Upper, HingeRight ? WarpedDoor::Hinge::Right : WarpedDoor::Hinge::Left, false, false) }
				});
			}
			case BlockType::IronDoor:
			{
				return a_Player.PlaceBlocks(
				{
					{ a_PlacePosition, IronDoor::IronDoor(Facing, IronDoor::Half::Lower, HingeRight ? IronDoor::Hinge::Right : IronDoor::Hinge::Left, false, false) },
					{ UpperBlockPosition, IronDoor::IronDoor(Facing, IronDoor::Half::Upper, HingeRight ? IronDoor::Hinge::Right : IronDoor::Hinge::Left, false, false) }
				});
			}
			default: return false;
		}
		return true;
	}




	virtual bool IsPlaceable(void) const override
	{
		return true;
	}
} ;
