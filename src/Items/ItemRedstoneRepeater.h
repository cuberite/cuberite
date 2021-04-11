
#pragma once

#include "ItemHandler.h"
#include "../Blocks/BlockRedstoneRepeater.h"





class cItemRedstoneRepeaterHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemRedstoneRepeaterHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool IsPlaceable() override
	{
		return true;
	}





	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_REDSTONE_REPEATER_OFF, cBlockRedstoneRepeaterHandler::YawToMetaData(a_Player.GetYaw()));
	}
} ;




