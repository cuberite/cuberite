
#pragma once

#include "ItemHandler.h"





class cItemLeavesHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemLeavesHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}
} ;




