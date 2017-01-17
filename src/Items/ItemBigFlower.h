
// ItemBigFlower.h

// Declares the cItemBigFlower class representing the cItemHandler for big flowers





#pragma once

#include "ItemHandler.h"





class cItemBigFlowerHandler:
	public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemBigFlowerHandler(void):
		super(E_BLOCK_BIG_FLOWER)
	{
	}


	virtual bool GetBlocksToPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		sSetBlockVector & a_BlocksToSet
	) override
	{
		// Can only be placed on the floor:
		if (a_BlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		a_BlocksToSet.emplace_back(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_BIG_FLOWER, a_EquippedItem.m_ItemDamage & 0x07);
		if (a_BlockY < cChunkDef::Height - 1)
		{
			a_BlocksToSet.emplace_back(a_BlockX, a_BlockY + 1, a_BlockZ, E_BLOCK_BIG_FLOWER, (a_EquippedItem.m_ItemDamage & 0x07) | 0x08);
		}
		return true;
	}
};




