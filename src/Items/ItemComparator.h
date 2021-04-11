
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





	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_INACTIVE_COMPARATOR, cBlockComparatorHandler::YawToMetaData(a_Player.GetYaw()));
	}
} ;




