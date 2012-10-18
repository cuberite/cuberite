
#pragma once

#include "ItemHandler.h"





class cItemLeavesHandler :
	public cItemHandler
{
public:
	cItemLeavesHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}
	
	virtual NIBBLETYPE GetBlockMeta(short a_ItemDamage) override
	{
		return (NIBBLETYPE)(a_ItemDamage & 0x0f) | 0x4;	//0x4 bit set means this is a player-placed leaves block, not to be decayed
	}
} ;




