
#pragma once

#include "ItemHandler.h"
#include "../World.h"

class cItemDoorHandler : public cItemHandler
{
public:
	cItemDoorHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool IsPlaceable() override
	{
		return true;
	}

	virtual BLOCKTYPE GetBlockType() override
	{
		return (m_ItemType == E_ITEM_WOODEN_DOOR) ? E_BLOCK_WOODEN_DOOR : E_BLOCK_IRON_DOOR;
	}

};