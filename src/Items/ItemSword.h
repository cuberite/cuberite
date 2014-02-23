
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
	
	inline static bool IsSword(int id)
	{
		switch (id)
		{
			case E_ITEM_WOODEN_SWORD:
			case E_ITEM_STONE_SWORD:
			case E_ITEM_IRON_SWORD:
			case E_ITEM_GOLD_SWORD:
			case E_ITEM_DIAMOND_SWORD:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}
} ;




