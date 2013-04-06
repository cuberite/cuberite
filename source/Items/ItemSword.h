
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"

class cItemSwordHandler : public cItemHandler
{
public:
	cItemSwordHandler(int a_ItemType)
	: cItemHandler(a_ItemType)
	{

	}
	
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		return (a_BlockType == E_BLOCK_COBWEB);
	}
};