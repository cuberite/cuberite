
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

#define GET_DOOR_TYPE(DoorType) \
	BlockType = BlockType::DoorType;     break;

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
			case Item::BambooDoor:               GET_DOOR_TYPE(BambooDoor)
			case Item::CherryDoor:               GET_DOOR_TYPE(CherryDoor)
			case Item::CopperDoor:               GET_DOOR_TYPE(CopperDoor)
			case Item::MangroveDoor:             GET_DOOR_TYPE(MangroveDoor)
			case Item::ExposedCopperDoor:        GET_DOOR_TYPE(ExposedCopperDoor)
			case Item::WeatheredCopperDoor:      GET_DOOR_TYPE(WeatheredCopperDoor)
			case Item::OxidizedCopperDoor:       GET_DOOR_TYPE(OxidizedCopperDoor)
			case Item::WaxedCopperDoor:          GET_DOOR_TYPE(WaxedCopperDoor)
			case Item::WaxedExposedCopperDoor:   GET_DOOR_TYPE(WaxedExposedCopperDoor)
			case Item::WaxedWeatheredCopperDoor: GET_DOOR_TYPE(WaxedWeatheredCopperDoor)
			case Item::WaxedOxidizedCopperDoor:  GET_DOOR_TYPE(WaxedOxidizedCopperDoor)
			case Item::PaleOakDoor:              GET_DOOR_TYPE(PaleOakDoor)
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
			BlockState UpperReplacedBlock;
			if (!World.GetBlock(UpperBlockPosition, UpperReplacedBlock))
			{
				return false;
			}

			if (!cBlockHandler::For(UpperReplacedBlock.Type()).DoesIgnoreBuildCollision(World, a_HeldItem, UpperBlockPosition, UpperReplacedBlock, a_ClickedBlockFace, false))
			{
				return false;
			}
		}

		// Get the coords of the neighboring blocks:
		auto Facing = RotationToBlockFace(a_Player.GetYaw(), true);
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

#define PLACE_DOOR(DoorType) \
	{\
		return a_Player.PlaceBlock(a_PlacePosition, DoorType::DoorType(Facing, DoorType::Half::Lower, HingeRight ? DoorType::Hinge::Right : DoorType::Hinge::Left, false, false)) && \
		a_Player.PlaceBlock(UpperBlockPosition, DoorType::DoorType(Facing, DoorType::Half::Upper, HingeRight ? DoorType::Hinge::Right : DoorType::Hinge::Left, false, false));\
	}\

		switch (BlockType)
		{
			case BlockType::AcaciaDoor:               PLACE_DOOR(AcaciaDoor)
			case BlockType::BirchDoor:                PLACE_DOOR(BirchDoor)
			case BlockType::DarkOakDoor:              PLACE_DOOR(DarkOakDoor)
			case BlockType::JungleDoor:               PLACE_DOOR(JungleDoor)
			case BlockType::OakDoor:                  PLACE_DOOR(OakDoor)
			case BlockType::SpruceDoor:               PLACE_DOOR(SpruceDoor)
			case BlockType::WarpedDoor:               PLACE_DOOR(WarpedDoor)
			case BlockType::IronDoor:                 PLACE_DOOR(IronDoor)
			case BlockType::BambooDoor:               PLACE_DOOR(BambooDoor)
			case BlockType::CherryDoor:               PLACE_DOOR(CherryDoor)
			case BlockType::CopperDoor:               PLACE_DOOR(CopperDoor)
			case BlockType::MangroveDoor:             PLACE_DOOR(MangroveDoor)
			case BlockType::ExposedCopperDoor:        PLACE_DOOR(ExposedCopperDoor)
			case BlockType::WeatheredCopperDoor:      PLACE_DOOR(WeatheredCopperDoor)
			case BlockType::OxidizedCopperDoor:       PLACE_DOOR(OxidizedCopperDoor)
			case BlockType::WaxedCopperDoor:          PLACE_DOOR(WaxedCopperDoor)
			case BlockType::WaxedExposedCopperDoor:   PLACE_DOOR(WaxedExposedCopperDoor)
			case BlockType::WaxedWeatheredCopperDoor: PLACE_DOOR(WaxedWeatheredCopperDoor)
			case BlockType::WaxedOxidizedCopperDoor:  PLACE_DOOR(WaxedOxidizedCopperDoor)
			case BlockType::PaleOakDoor:              PLACE_DOOR(PaleOakDoor)
			default: return false;
		}
		return true;
	}




	virtual bool IsPlaceable(void) const override
	{
		return true;
	}
} ;
