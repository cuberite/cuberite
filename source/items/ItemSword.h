
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"

class cItemSwordHandler : public cItemHandler
{
public:
	cItemSwordHandler(int a_ItemID)
	: cItemHandler(a_ItemID)
	{

	}
	
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockID) override
	{
		return a_BlockID == E_BLOCK_COBWEB;
	}
};