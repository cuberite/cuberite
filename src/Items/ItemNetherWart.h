
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemNetherWartHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemNetherWartHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// Only allow planting nether wart onto the top side of the block:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return true;
		}

		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_NETHER_WART, 0);
	}
} ;
