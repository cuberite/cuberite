
#pragma once
#include "Item.h"





class cItemRedstoneRepeaterHandler : public cItemHandler
{
public:
	cItemRedstoneRepeaterHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
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




