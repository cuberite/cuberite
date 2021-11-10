
#pragma once

#include "ItemHandler.h"





class cItemRedstoneDustHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemRedstoneDustHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_REDSTONE_WIRE, 0);
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}
} ;




