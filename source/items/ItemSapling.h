
#pragma once

#include "Item.h"





class cItemSaplingHandler : public cItemHandler
{
public:
	cItemSaplingHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual NIBBLETYPE GetBlockMeta(short a_ItemDamage) override
	{
		// Only the lowest 3 bits are important
		return (NIBBLETYPE)(a_ItemDamage & 0x07);
	}
} ;




