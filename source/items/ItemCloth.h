#pragma once

#include "Item.h"


class cItemClothHandler : public cItemHandler
{
public:
	cItemClothHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}
	virtual NIBBLETYPE GetBlockMeta(char a_ItemMeta) override
	{
		return a_ItemMeta;
	}
};