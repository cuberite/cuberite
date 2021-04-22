
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





	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// Only allow planting seeds from the top side of the block:
		if ((a_ClickedBlockFace != BLOCK_FACE_TOP) || (a_PlacedBlockPos.y <= 0))
		{
			return false;
		}

		// Only allow placement on farmland
		if (a_World->GetBlock(a_PlacedBlockPos.addedY(-1)) != E_BLOCK_FARMLAND)
		{
			return false;
		}

		// Get the produce block based on the seed item:
		a_BlockMeta = 0;
		switch (m_ItemType)
		{
			case E_ITEM_BEETROOT_SEEDS: a_BlockType = E_BLOCK_BEETROOTS;    return true;
			case E_ITEM_CARROT:         a_BlockType = E_BLOCK_CARROTS;      return true;
			case E_ITEM_MELON_SEEDS:    a_BlockType = E_BLOCK_MELON_STEM;   return true;
			case E_ITEM_POTATO:         a_BlockType = E_BLOCK_POTATOES;     return true;
			case E_ITEM_PUMPKIN_SEEDS:  a_BlockType = E_BLOCK_PUMPKIN_STEM; return true;
			case E_ITEM_SEEDS:          a_BlockType = E_BLOCK_CROPS;        return true;
			default:                    a_BlockType = E_BLOCK_AIR;          return true;
		}
	}
} ;




