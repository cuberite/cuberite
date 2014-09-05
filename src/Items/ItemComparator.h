
#pragma once

#include "ItemHandler.h"
#include "../Blocks/BlockRedstoneRepeater.h"





class cItemComparatorHandler :
	public cItemHandler
{
public:
	cItemComparatorHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}

	virtual bool IsPlaceable(void) override
	{
		return true;
	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = E_BLOCK_INACTIVE_COMPARATOR;
		a_BlockMeta = cBlockRedstoneRepeaterHandler::RepeaterRotationToMetaData(a_Player->GetYaw());
		return true;
	}
} ;




