
#pragma once

#include "ItemHandler.h"
#include "../Blocks/BlockRedstoneRepeater.h"





class cItemRepeaterHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemRepeaterHandler(Item a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool CommitPlacement(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, Block::Repeater::Repeater(0, RotationToBlockFace(a_Player.GetYaw()), false, false));
	}





	virtual bool IsPlaceable() override
	{
		return true;
	}
} ;




