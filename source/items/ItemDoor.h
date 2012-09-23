
#pragma once

#include "ItemHandler.h"
#include "../World.h"

class cItemDoorHandler : public cItemHandler
{
public:
	cItemDoorHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return (m_ItemID == E_ITEM_WOODEN_DOOR) ? E_BLOCK_WOODEN_DOOR : E_BLOCK_IRON_DOOR;
	}

};