
#pragma once

#include "ItemHandler.h"





class cItemRedstoneDustHandler : public cItemHandler
{
public:
	cItemRedstoneDustHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return E_BLOCK_REDSTONE_WIRE;
	}

	virtual NIBBLETYPE GetBlockMeta(short a_ItemDamage) override
	{
		return 0;
	}
} ;




