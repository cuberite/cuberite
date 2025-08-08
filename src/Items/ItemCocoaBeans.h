
#pragma once

#include "ItemHandler.h"




class cCocoaBeansHandler final :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:

	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, Vector3i a_PlacePosition, eBlockFace a_ClickedBlockFace, Vector3i a_CursorPosition) const override
	{
		return a_Player.PlaceBlock(a_PlacePosition, Block::Cocoa::Cocoa(0, ReverseBlockFace(a_ClickedBlockFace)));
	}
};




