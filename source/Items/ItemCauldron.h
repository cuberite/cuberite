
#pragma once

#include "ItemHandler.h"

class cItemCauldronHandler : public cItemHandler
{
public:
	cItemCauldronHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return E_BLOCK_CAULDRON;
	}

};