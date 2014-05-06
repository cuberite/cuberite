
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemSwordHandler :
	public cItemHandler
{
public:
	cItemSwordHandler(int a_ItemType)
	: cItemHandler(a_ItemType)
	{

	}
	
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		return (a_BlockType == E_BLOCK_COBWEB);
	}

	virtual bool CanRepairWithRawMaterial(short a_ItemType) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_SWORD:  return (a_ItemType == E_BLOCK_PLANKS);
			case E_ITEM_STONE_SWORD:   return (a_ItemType == E_BLOCK_COBBLESTONE);
			case E_ITEM_IRON_SWORD:    return (a_ItemType == E_ITEM_IRON);
			case E_ITEM_GOLD_SWORD:    return (a_ItemType == E_ITEM_GOLD);
			case E_ITEM_DIAMOND_SWORD: return (a_ItemType == E_ITEM_DIAMOND);
		}
		return false;
	}
} ;




