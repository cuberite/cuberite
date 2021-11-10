#pragma once

#include "ItemHandler.h"





class cSimplePlaceableItemHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cSimplePlaceableItemHandler(Item a_ItemType) :
		Super(a_ItemType)
	{
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, BlockItemConverter::FromItem(a_HeldItem.m_ItemType));
	}
};
