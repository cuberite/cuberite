
#pragma once

#include "ItemHandler.h"





class cItemSaplingHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemSaplingHandler(int a_ItemType):
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
			a_PlacedBlockPos, a_ClickedBlockFace,
			a_CursorPos,
			a_BlockType, a_BlockMeta
		);

		// Allow only the lowest 3 bits (top bit is for growth):
		a_BlockMeta = a_BlockMeta & 0x07;
		return res;
	}
} ;




