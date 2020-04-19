
#pragma once

#include "ItemHandler.h"





class cItemLeavesHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemLeavesHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		bool res = Super::GetPlacementBlockTypeMeta(
			a_World, a_Player,
			a_PlacedBlockPos,
			a_ClickedBlockFace,
			a_CursorPos,
			a_BlockType, a_BlockMeta
		);
		a_BlockMeta = a_BlockMeta | 0x4;  // 0x4 bit set means this is a player-placed leaves block, not to be decayed
		return res;
	}
} ;




