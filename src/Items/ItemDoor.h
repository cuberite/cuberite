
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockDoor.h"





class cItemDoorHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemDoorHandler(int a_ItemType):
		Super(a_ItemType)
	{

	}




	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
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
			case E_ITEM_WOODEN_DOOR:   BlockType = BlockType::OakDoor;     break;
			case E_ITEM_IRON_DOOR:     BlockType = BlockType::IronDoor;    break;
			case E_ITEM_SPRUCE_DOOR:   BlockType = BlockType::SpruceDoor;  break;
			case E_ITEM_BIRCH_DOOR:    BlockType = BlockType::BirchDoor;   break;
			case E_ITEM_JUNGLE_DOOR:   BlockType = BlockType::JungleDoor;  break;
			case E_ITEM_DARK_OAK_DOOR: BlockType = BlockType::DarkOakDoor; break;
			case E_ITEM_ACACIA_DOOR:   BlockType = BlockType::AcaciaDoor;  break;
			default: UNREACHABLE("Unhandled door type");
		}

		const auto & World = *a_Player.GetWorld();
		const auto UpperBlockPosition = a_PlacePosition.addedY(1);

		// Check the block that will get replaced by the door:
		{
			BLOCKTYPE TopType;
			NIBBLETYPE TopMeta;
			World.GetBlockTypeMeta(UpperBlockPosition, TopType, TopMeta);

			if (!cBlockHandler::For(TopType).DoesIgnoreBuildCollision(World, a_HeldItem, UpperBlockPosition, TopMeta, a_ClickedBlockFace, false))
			{
				return false;
			}
		}

		// Get the coords of the neighboring blocks:
		auto Facing = RotationToBlockFace(a_Player.GetYaw());
		Vector3i RelDirToOutside = cBlockDoorHandler::GetRelativeDirectionToOutside(LowerBlock);
		Vector3i LeftNeighborPos = RelDirToOutside;
		LeftNeighborPos.TurnCW();
		LeftNeighborPos.Move(a_PlacePosition);
		Vector3i RightNeighborPos = RelDirToOutside;
		RightNeighborPos.TurnCCW();
		RightNeighborPos.Move(a_PlacePosition);

		// Decide whether the hinge is on the left (default) or on the right:
		auto LeftNeighborBlock = World.GetBlock(LeftNeighborPos);
		auto RightNeighborBlock = World.GetBlock(RightNeighborPos);
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
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, AcaciaDoor::AcaciaDoor(Facing, AcaciaDoor::Half::Lower, HingeRight ? AcaciaDoor::Hinge::Right : AcaciaDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    AcaciaDoor::AcaciaDoor(Facing, AcaciaDoor::Half::Upper, HingeRight ? AcaciaDoor::Hinge::Right : AcaciaDoor::Hinge::Left, false, false));
				break;
			}
			case BlockType::BirchDoor:
			{
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, BirchDoor::BirchDoor(Facing, BirchDoor::Half::Lower, HingeRight ? BirchDoor::Hinge::Right : BirchDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    BirchDoor::BirchDoor(Facing, BirchDoor::Half::Upper, HingeRight ? BirchDoor::Hinge::Right : BirchDoor::Hinge::Left, false, false));
				break;
			}
			case BlockType::CrimsonDoor:
			{
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, CrimsonDoor::CrimsonDoor(Facing, CrimsonDoor::Half::Lower, HingeRight ? CrimsonDoor::Hinge::Right : CrimsonDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    CrimsonDoor::CrimsonDoor(Facing, CrimsonDoor::Half::Upper, HingeRight ? CrimsonDoor::Hinge::Right : CrimsonDoor::Hinge::Left, false, false));
				break;
			}
			case BlockType::DarkOakDoor:
			{
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, CrimsonDoor::CrimsonDoor(Facing, CrimsonDoor::Half::Lower, HingeRight ? CrimsonDoor::Hinge::Right : CrimsonDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    CrimsonDoor::CrimsonDoor(Facing, CrimsonDoor::Half::Upper, HingeRight ? CrimsonDoor::Hinge::Right : CrimsonDoor::Hinge::Left, false, false));
				break;
			}
			case BlockType::JungleDoor:
			{
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, JungleDoor::JungleDoor(Facing, JungleDoor::Half::Lower, HingeRight ? JungleDoor::Hinge::Right : JungleDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    JungleDoor::JungleDoor(Facing, JungleDoor::Half::Upper, HingeRight ? JungleDoor::Hinge::Right : JungleDoor::Hinge::Left, false, false));
				break;
			}
			case BlockType::OakDoor:
			{
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, OakDoor::OakDoor(Facing, OakDoor::Half::Lower, HingeRight ? OakDoor::Hinge::Right : OakDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    OakDoor::OakDoor(Facing, OakDoor::Half::Upper, HingeRight ? OakDoor::Hinge::Right : OakDoor::Hinge::Left, false, false));
				break;
			}
			case BlockType::SpruceDoor:
			{
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, SpruceDoor::SpruceDoor(Facing, SpruceDoor::Half::Lower, HingeRight ? SpruceDoor::Hinge::Right : SpruceDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    SpruceDoor::SpruceDoor(Facing, SpruceDoor::Half::Upper, HingeRight ? SpruceDoor::Hinge::Right : SpruceDoor::Hinge::Left, false, false));
				break;
			}
			case BlockType::WarpedDoor:
			{
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, WarpedDoor::WarpedDoor(Facing, WarpedDoor::Half::Lower, HingeRight ? WarpedDoor::Hinge::Right : WarpedDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    WarpedDoor::WarpedDoor(Facing, WarpedDoor::Half::Upper, HingeRight ? WarpedDoor::Hinge::Right : WarpedDoor::Hinge::Left, false, false));
				break;
			}
			case BlockType::IronDoor:
			{
				a_BlocksToSet.emplace_back(a_PlacedBlockPos, IronDoor::IronDoor(Facing, IronDoor::Half::Lower, HingeRight ? IronDoor::Hinge::Right : IronDoor::Hinge::Left, false, false));
				a_BlocksToSet.emplace_back(UpperBlockPos,    IronDoor::IronDoor(Facing, IronDoor::Half::Upper, HingeRight ? IronDoor::Hinge::Right : IronDoor::Hinge::Left, false, false));
				break;
			}
			default: return false;
		}
		return true;

		// Set the blocks:
		return a_Player.PlaceBlocks(
		{
			{ a_PlacePosition, BlockType, LowerBlockMeta },
			{ UpperBlockPosition, BlockType, UpperBlockMeta }
		});
	}




	virtual bool IsPlaceable(void) override
	{
		return true;
	}
} ;
