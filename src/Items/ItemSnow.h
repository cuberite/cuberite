
#pragma once

#include "ItemHandler.h"





class cItemSnowHandler final :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		auto OldBlock = a_Player.GetWorld()->GetBlock(a_PlacePosition);
		auto LayerCount = Block::Snow::Layers(OldBlock);
		// Check if incrementing existing snow height:
		if (OldBlock.Type() == BlockType::Snow)
		{
			ASSERT(LayerCount < 7);  // BlockSnow.h ensures that if we replace a snow layer, it won't be at max height.
			LayerCount++;
		}
		else
		{
			// First time placement:
			LayerCount = 0;
		}

		return a_Player.PlaceBlock(a_PlacePosition, Block::Snow::Snow(LayerCount));
	}
};
