
#pragma once

#include "ItemHandler.h"
#include "../Blocks/BlockComparator.h"





class cItemComparatorHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemComparatorHandler(int a_ItemType):
		cItemHandler(a_ItemType)
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
		a_BlockType = E_BLOCK_INACTIVE_COMPARATOR;
		a_BlockMeta = cBlockComparatorHandler::YawToMetaData(a_Player->GetYaw());
		return true;
	}
} ;




