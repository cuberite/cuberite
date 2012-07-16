#pragma once

#include "Item.h"


class cItemWoodHandler : public cItemHandler
{
public:
	cItemWoodHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{
	}
	virtual NIBBLETYPE GetBlockMeta(NIBBLETYPE a_ItemMeta) override
	{
		return a_ItemMeta;
	}
};