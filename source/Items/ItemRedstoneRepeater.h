
#pragma once

#include "ItemHandler.h"





class cItemRedstoneRepeaterHandler : public cItemHandler
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

	virtual BLOCKTYPE GetBlockType() override
	{
		return ::E_BLOCK_REDSTONE_REPEATER_OFF;
	}

	virtual NIBBLETYPE GetBlockMeta(short a_ItemMeta) override
	{
		return 0;
	}
} ;




