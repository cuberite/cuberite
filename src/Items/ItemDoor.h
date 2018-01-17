
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockDoor.h"





class cItemDoorHandler :
	public cItemHandler
{
public:
	cItemDoorHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}


	virtual bool GetBlocksToPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		sSetBlockVector & a_BlocksToSet
	) override
	{
		// Vanilla only allows door placement while clicking on the top face of the block below the door:
		if (a_BlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		// Door (bottom block) can be placed in Y range of [1, 254]:
		if ((a_BlockY < 1) || (a_BlockY >= cChunkDef::Height - 2))
		{
			return false;
		}

		// The door needs a compatible block below it:
		if (!cBlockDoorHandler::CanBeOn(a_World.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ), a_World.GetBlockMeta(a_BlockX, a_BlockY - 1, a_BlockZ)))
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
			default:
			{
				ASSERT(!"Unhandled door type");
				return false;
			}
		}

		// Check the two blocks that will get replaced by the door:
		BLOCKTYPE LowerBlockType = a_World.GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		BLOCKTYPE UpperBlockType = a_World.GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ);
		if (
			!cBlockDoorHandler::CanReplaceBlock(LowerBlockType) ||
			!cBlockDoorHandler::CanReplaceBlock(UpperBlockType))
		{
			return false;
		}

		// Get the coords of the neighboring blocks:
		NIBBLETYPE LowerBlockMeta = cBlockDoorHandler::PlayerYawToMetaData(a_Player.GetYaw());
		Vector3i RelDirToOutside = cBlockDoorHandler::GetRelativeDirectionToOutside(LowerBlockMeta);
		Vector3i LeftNeighborPos = RelDirToOutside;
		LeftNeighborPos.TurnCW();
		LeftNeighborPos.Move(a_BlockX, a_BlockY, a_BlockZ);
		Vector3i RightNeighborPos = RelDirToOutside;
		RightNeighborPos.TurnCCW();
		RightNeighborPos.Move(a_BlockX, a_BlockY, a_BlockZ);

		// Decide whether the hinge is on the left (default) or on the right:
		NIBBLETYPE UpperBlockMeta = 0x08;
		BLOCKTYPE LeftNeighborBlock = a_World.GetBlock(LeftNeighborPos);
		BLOCKTYPE RightNeighborBlock = a_World.GetBlock(RightNeighborPos);
		/*
		// DEBUG:
		LOGD("Door being placed at {%d, %d, %d}", a_BlockX, a_BlockY, a_BlockZ);
		LOGD("RelDirToOutside: {%d, %d, %d}", RelDirToOutside.x, RelDirToOutside.y, RelDirToOutside.z);
		LOGD("Left neighbor at {%d, %d, %d}: %d (%s)", LeftNeighborPos.x, LeftNeighborPos.y, LeftNeighborPos.z, LeftNeighborBlock, ItemTypeToString(LeftNeighborBlock).c_str());
		LOGD("Right neighbor at {%d, %d, %d}: %d (%s)", RightNeighborPos.x, RightNeighborPos.y, RightNeighborPos.z, RightNeighborBlock, ItemTypeToString(RightNeighborBlock).c_str());
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
		a_BlocksToSet.emplace_back(a_BlockX, a_BlockY, a_BlockZ, BlockType, LowerBlockMeta);
		a_BlocksToSet.emplace_back(a_BlockX, a_BlockY + 1, a_BlockZ, BlockType, UpperBlockMeta);
		return true;
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}
} ;
