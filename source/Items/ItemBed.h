
#pragma once

#include "ItemHandler.h"
#include "../World.h"

class cItemBedHandler : public cItemHandler
{
public:
	cItemBedHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return E_BLOCK_BED;
	}
};