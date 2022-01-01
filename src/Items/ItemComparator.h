
#pragma once

#include "ItemHandler.h"
#include "../Blocks/BlockComparator.h"





class cItemComparatorHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_INACTIVE_COMPARATOR, cBlockComparatorHandler::YawToMetaData(a_Player.GetYaw()));
	}


	virtual bool IsPlaceable(void) const override
	{
		return true;
	}
} ;
