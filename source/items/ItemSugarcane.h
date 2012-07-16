#pragma once

#include "Item.h"
#include "../cWorld.h"

class cItemSugarcaneHandler : public cItemHandler
{
public:
	cItemSugarcaneHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return E_BLOCK_SUGARCANE;
	}

	virtual NIBBLETYPE GetBlockMeta(NIBBLETYPE a_ItemMeta) override
	{
		return 0;	//Not grown yet
	}
};