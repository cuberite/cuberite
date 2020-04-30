
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
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = E_BLOCK_REDSTONE_REPEATER_OFF;
		a_BlockMeta = cBlockRedstoneRepeaterHandler::YawToMetaData(a_Player->GetYaw());
		return true;
	}
} ;




