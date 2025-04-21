
#pragma once

#include "ItemHandler.h"





class cItemTripwireHookHandler final  :
	public cSimplePlaceableItemHandler
{
	using Super = cSimplePlaceableItemHandler;

public:

	using Super::Super;

private:


	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) const override
	{
		if ((a_ClickedBlockFace == BLOCK_FACE_YP) || (a_ClickedBlockFace == BLOCK_FACE_YM))
		{
			return false;
		}

		return a_Player.PlaceBlock(a_PlacePosition, Block::TripwireHook::TripwireHook(false, RotationToBlockFace(a_Player.GetYaw()), false));
	}
};
