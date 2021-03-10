
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





	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BlockState & a_Block
	) override
	{
		a_Block = Block::Repeater::Repeater(0, RotationToBlockFace(a_Player->GetYaw()), false, false);
		return true;
	}
} ;




