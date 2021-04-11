#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockPumpkin.h"





class cItemJackOLanternHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// Re-use the pumpkin converter for lanterns:
		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_JACK_O_LANTERN, cBlockPumpkinHandler::YawToMetaData(a_Player.GetYaw()));
	}
};
