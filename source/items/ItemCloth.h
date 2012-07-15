#pragma once

#include "Item.h"


class cItemClothHandler : public cItemHandler
{
public:
	cItemClothHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}
	virtual char GetBlockMeta(char a_ItemMeta)
	{
		return a_ItemMeta;
	}
};