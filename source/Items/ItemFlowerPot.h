
#pragma once

#include "ItemHandler.h"

class cItemFlowerPotHandler : public cItemHandler
{
public:
	cItemFlowerPotHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return E_BLOCK_FLOWER_POT;
	}

};