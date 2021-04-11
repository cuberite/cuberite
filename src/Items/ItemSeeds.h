
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemSeedsHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemSeedsHandler(int a_ItemType):
		Super(a_ItemType)
	{

	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// Only allow planting seeds from the top side of the block:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		BLOCKTYPE BlockType;

		// Get the produce block based on the seed item:
		switch (m_ItemType)
		{
			case E_ITEM_BEETROOT_SEEDS: BlockType = E_BLOCK_BEETROOTS;    break;
			case E_ITEM_CARROT:         BlockType = E_BLOCK_CARROTS;      break;
			case E_ITEM_MELON_SEEDS:    BlockType = E_BLOCK_MELON_STEM;   break;
			case E_ITEM_POTATO:         BlockType = E_BLOCK_POTATOES;     break;
			case E_ITEM_PUMPKIN_SEEDS:  BlockType = E_BLOCK_PUMPKIN_STEM; break;
			case E_ITEM_SEEDS:          BlockType = E_BLOCK_CROPS;        break;
			default:                    UNREACHABLE("Unsupported seed type");
		}

		return a_Player.PlaceBlock(a_PlacePosition, BlockType, 0);
	}
} ;




