#pragma once

#include "Item.h"


class cItemSaplingHandler : public cItemHandler
{
public:
	cItemSaplingHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual char GetBlockMeta(char a_ItemMeta)
	{
		//Only the first 2 bits are important
		return a_ItemMeta & 3;
	}
};