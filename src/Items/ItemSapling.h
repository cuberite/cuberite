
#pragma once

#include "ItemHandler.h"





class cItemSaplingHandler : public cItemHandler
{
	typedef cItemHandler super;

public:
	cItemSaplingHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		bool res = super::GetPlacementBlockTypeMeta(
			a_World, a_Player,
			a_BlockX, a_BlockY, a_BlockZ, a_BlockFace,
			a_CursorX, a_CursorY, a_CursorZ,
			a_BlockType, a_BlockMeta
		);
		// Only the lowest 3 bits are important
		a_BlockMeta = a_BlockMeta & 0x7;
		return res;
	}
} ;




