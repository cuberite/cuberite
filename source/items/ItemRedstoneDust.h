#pragma once
#include "Item.h"


class cItemRedstoneDustHandler : public cItemHandler
{
public:
	cItemRedstoneDustHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{
	}

	virtual bool IsPlaceable()
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType()
	{
		return E_BLOCK_REDSTONE_WIRE;
	}

	virtual NIBBLETYPE GetBlockMeta(char a_ItemMeta) override
	{
		return 0;
	}
};