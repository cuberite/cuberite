#pragma once

#include "ItemHandler.h"





class cItemSnowHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		BLOCKTYPE Block;
		NIBBLETYPE Meta;
		a_Player.GetWorld()->GetBlockTypeMeta(a_PlacePosition, Block, Meta);

		// Check if incrementing existing snow height:
		if (Block == E_BLOCK_SNOW)
		{
			ASSERT(Meta < 7);  // BlockSnow.h ensures that if we replace a snow layer, it won't be at max height.
			Meta++;
		}
		else
		{
			// First time placement:
			Meta = 0;
		}

		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_SNOW, Meta);
	}
};
