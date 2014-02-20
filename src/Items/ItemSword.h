
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
	
	virtual float GetDestroySpeed(BLOCKTYPE a_BlockType) override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_COBWEB:
			{
				return 15.0F;
			}
			case E_BLOCK_COCOA_POD:
			case E_BLOCK_YELLOW_FLOWER:
			case E_BLOCK_RED_ROSE:
			case E_BLOCK_REEDS:
			case E_BLOCK_BIG_FLOWER:
			case E_BLOCK_DEAD_BUSH:
			case E_BLOCK_TALL_GRASS:
			case E_BLOCK_VINES:
			case E_BLOCK_LEAVES:
			case E_BLOCK_NEW_LEAVES:
			case E_BLOCK_MELON:
			case E_BLOCK_PUMPKIN:
			{
				return 1.5F;
			}
			default:
			{
				return 1.0F;
			}
		}
	}
} ;




