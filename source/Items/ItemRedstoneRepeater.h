
#pragma once

#include "ItemHandler.h"
#include "../Simulator/RedstoneSimulator.h"





class cItemRedstoneRepeaterHandler :
	public cItemHandler
{
public:
	cItemRedstoneRepeaterHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = E_BLOCK_REDSTONE_REPEATER_OFF;
		a_BlockMeta = cRedstoneSimulator::RepeaterRotationToMetaData(a_Player->GetRotation());
		return true;
	}
} ;




