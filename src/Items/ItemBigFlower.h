
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
		// Can only be placed on dirt:
		if ((a_BlockY <= 0) || !IsBlockTypeOfDirt(a_World.GetBlock(a_BlockX, a_BlockY - 1, a_BlockZ)))
		{
			return false;
		}

		// Needs at least two free blocks to build in
		if (a_BlockY >= cChunkDef::Height - 1)
		{
			return false;
		}

		BLOCKTYPE TopType;
		NIBBLETYPE TopMeta;
		a_World.GetBlockTypeMeta(a_BlockX, a_BlockY + 1, a_BlockZ, TopType, TopMeta);
		cChunkInterface ChunkInterface(a_World.GetChunkMap());

		if (!BlockHandler(TopType)->DoesIgnoreBuildCollision(ChunkInterface, { a_BlockX, a_BlockY + 1, a_BlockZ }, a_Player, TopMeta))
		{
			return false;
		}

		a_BlocksToSet.emplace_back(a_BlockX, a_BlockY,     a_BlockZ, E_BLOCK_BIG_FLOWER, a_EquippedItem.m_ItemDamage & 0x07);
		a_BlocksToSet.emplace_back(a_BlockX, a_BlockY + 1, a_BlockZ, E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_TOP);
		return true;
	}
};
