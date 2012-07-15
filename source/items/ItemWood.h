#pragma once

#include "Item.h"


class cItemWoodHandler : public cItemHandler
{
public:
	cItemWoodHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{
	}
	virtual char GetBlockMeta(char a_ItemMeta)
	{
		return a_ItemMeta;
	}
};