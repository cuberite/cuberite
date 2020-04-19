
#pragma once

#include "ItemHandler.h"





class cItemStringHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemStringHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = E_BLOCK_TRIPWIRE;
		a_BlockMeta = 0;
		return true;
	}
};




