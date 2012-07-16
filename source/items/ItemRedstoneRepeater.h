#pragma once
#include "Item.h"


class cItemRedstoneRepeaterHandler : public cItemHandler
{
public:
	cItemRedstoneRepeaterHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{
	}

	virtual bool IsPlaceable()
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType()
	{
		return ::E_BLOCK_REDSTONE_REPEATER_OFF;
	}

	virtual NIBBLETYPE GetBlockMeta(char a_ItemMeta) override
	{
		return 0;
	}
};