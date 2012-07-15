#pragma once

#include "Item.h"
#include "../cWorld.h"

class cItemDoorHandler : public cItemHandler
{
public:
	cItemDoorHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool IsPlaceable()
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType()
	{
		return (m_ItemID == E_ITEM_WOODEN_DOOR) ? E_BLOCK_WOODEN_DOOR : E_BLOCK_IRON_DOOR;
	}

};