#pragma once

#include "ItemHandler.h"



#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cSimplePlaceableItemHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

	virtual bool IsPlaceable(void) const override
	{
		return true;
	}


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		return a_Player.PlaceBlock(a_PlacePosition, BlockItemConverter::FromItem(a_HeldItem.m_ItemType));
	}
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

