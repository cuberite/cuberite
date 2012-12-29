
#pragma once

#include "ItemHandler.h"

class cItemBrewingStandHandler : public cItemHandler
{
public:
	cItemBrewingStandHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return E_BLOCK_BREWING_STAND;
	}

};