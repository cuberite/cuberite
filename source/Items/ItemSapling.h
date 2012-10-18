
#pragma once

#include "ItemHandler.h"





class cItemSaplingHandler : public cItemHandler
{
public:
	cItemSaplingHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual NIBBLETYPE GetBlockMeta(short a_ItemDamage) override
	{
		// Only the lowest 3 bits are important
		return (NIBBLETYPE)(a_ItemDamage & 0x07);
	}
} ;




