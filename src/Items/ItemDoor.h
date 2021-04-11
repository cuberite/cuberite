
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




	virtual bool IsPlaceable(void) override
	{
		return true;
	}




	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// Vanilla only allows door placement while clicking on the top face of the block below the door:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		// Get the block type of the door to place:
		BLOCKTYPE BlockType;
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_DOOR:   BlockType = E_BLOCK_OAK_DOOR;      break;
			case E_ITEM_IRON_DOOR:     BlockType = E_BLOCK_IRON_DOOR;     break;
			case E_ITEM_SPRUCE_DOOR:   BlockType = E_BLOCK_SPRUCE_DOOR;   break;
			case E_ITEM_BIRCH_DOOR:    BlockType = E_BLOCK_BIRCH_DOOR;    break;
			case E_ITEM_JUNGLE_DOOR:   BlockType = E_BLOCK_JUNGLE_DOOR;   break;
			case E_ITEM_DARK_OAK_DOOR: BlockType = E_BLOCK_DARK_OAK_DOOR; break;
			case E_ITEM_ACACIA_DOOR:   BlockType = E_BLOCK_ACACIA_DOOR;   break;
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
		NIBBLETYPE LowerBlockMeta = cBlockDoorHandler::YawToMetaData(a_Player.GetYaw());
		Vector3i RelDirToOutside = cBlockDoorHandler::GetRelativeDirectionToOutside(LowerBlockMeta);
		Vector3i LeftNeighborPos = RelDirToOutside;
		LeftNeighborPos.TurnCW();
		LeftNeighborPos.Move(a_PlacePosition);
		Vector3i RightNeighborPos = RelDirToOutside;
		RightNeighborPos.TurnCCW();
		RightNeighborPos.Move(a_PlacePosition);

		// Decide whether the hinge is on the left (default) or on the right:
		NIBBLETYPE UpperBlockMeta = 0x08;
		BLOCKTYPE LeftNeighborBlock = World.GetBlock(LeftNeighborPos);
		BLOCKTYPE RightNeighborBlock = World.GetBlock(RightNeighborPos);

		/*
		// DEBUG:
		FLOGD("Door being placed at {0}", a_PlacePosition);
		FLOGD("RelDirToOutside: {0}", RelDirToOutside);
		FLOGD("Left neighbor at {0}: {1} ({2})", LeftNeighborPos, LeftNeighborBlock, ItemTypeToString(LeftNeighborBlock));
		FLOGD("Right neighbor at {0}: {1} ({2})", RightNeighborPos, RightNeighborBlock, ItemTypeToString(RightNeighborBlock));
		*/

		if (
			cBlockDoorHandler::IsDoorBlockType(LeftNeighborBlock) ||   // The block to the left is a door block
			(
				!cBlockInfo::IsSolid(LeftNeighborBlock) &&               // Prioritize hinge on the left side
				cBlockInfo::IsSolid(RightNeighborBlock) &&               // The block to the right is solid...
				!cBlockDoorHandler::IsDoorBlockType(RightNeighborBlock)  // ... but not a door
			)
		)
		{
			// DEBUG: LOGD("Setting hinge to right side");
			UpperBlockMeta = 0x09;  // Upper block | hinge on right
		}

		// Set the blocks:
		return a_Player.PlaceBlocks(
		{
			{ a_PlacePosition, BlockType, LowerBlockMeta },
			{ UpperBlockPosition, BlockType, UpperBlockMeta }
		});
	}
} ;
