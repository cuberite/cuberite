
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemNetherWartHandler final:
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;





	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		// Only allow planting nether wart onto the top side of the block:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return true;
		}

		return a_Player.PlaceBlock(a_PlacePosition, Block::NetherWart::NetherWart());
	}
} ;
