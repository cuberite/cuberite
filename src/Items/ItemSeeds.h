
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemSeedsHandler :
	public cItemHandler
{
public:
	cItemSeedsHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable(void) override
	{
		return true;
	}

	virtual bool IsFood(void) override
	{
		switch (m_ItemType)  // Special cases, both a seed and food
		{
			case E_ITEM_CARROT:
			case E_ITEM_POTATO: return true;
			default:            return false;
		}
	}

	virtual FoodInfo GetFoodInfo(const cItem * a_Item) override
	{
		UNUSED(a_Item);
		switch (m_ItemType)
		{
			case E_ITEM_CARROT: return FoodInfo(3, 3.6);
			case E_ITEM_POTATO: return FoodInfo(1, 0.6);
			default:            return FoodInfo(0, 0);
		}
	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (a_BlockFace != BLOCK_FACE_TOP)
		{
			// Only allow planting seeds from the top side of the block
			return false;
		}

		// Only allow placement on farmland
		int X = a_BlockX;
		int Y = a_BlockY;
		int Z = a_BlockZ;
		AddFaceDirection(X, Y, Z, a_BlockFace, true);
		if (a_World->GetBlock(X, Y, Z) != E_BLOCK_FARMLAND)
		{
			return false;
		}

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




