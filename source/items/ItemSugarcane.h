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

	virtual bool IsPlaceable()
	{
		return true;
	}

	virtual bool AllowBlockOnTop()
	{
		return false;
	}
	
	virtual BLOCKTYPE GetBlockType()
	{
		return E_BLOCK_SUGARCANE;
	}

	virtual char GetBlockMeta(char a_ItemMeta)
	{
		return 0;	//Not grown yet
	}
};