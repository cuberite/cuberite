#pragma once
#include "Item.h"
#include "../cWorld.h"
#include "../cPlayer.h"

class cItemPickaxeHandler : public cItemHandler
{
public:
	cItemPickaxeHandler(int a_ItemID)
	: cItemHandler(a_ItemID)
	{

	}

	char PickaxeLevel()
	{
		switch(m_ItemID)
		{
		case E_ITEM_WOODEN_PICKAXE:
		case E_ITEM_GOLD_PICKAXE:
			return 1;
		case E_ITEM_STONE_PICKAXE:
			return 2;
		case E_ITEM_IRON_PICKAXE:
			return 3;
		case E_ITEM_DIAMOND_PICKAXE:
			return 4;
		default:
			return 0;
		}
	}
	
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockID)
	{
		switch(a_BlockID)
		{
		case E_BLOCK_OBSIDIAN:
			return PickaxeLevel() >= 4;
		case E_BLOCK_DIAMOND_BLOCK:
		case E_BLOCK_DIAMOND_ORE:
		case E_BLOCK_GOLD_BLOCK:
		case E_BLOCK_GOLD_ORE:
		case E_BLOCK_REDSTONE_ORE:
		case E_BLOCK_REDSTONE_ORE_GLOWING:
		case E_BLOCK_EMERALD_ORE:
			return PickaxeLevel() >= 3;
		case E_BLOCK_IRON_BLOCK:
		case E_BLOCK_IRON_ORE:
		case E_BLOCK_LAPIS_ORE:
		case E_BLOCK_LAPIS_BLOCK:
			return PickaxeLevel() >= 2;
		case E_BLOCK_COAL_ORE:
		case E_BLOCK_STONE:
		case E_BLOCK_COBBLESTONE:
		case E_BLOCK_END_STONE:
		case E_BLOCK_MOSSY_COBBLESTONE:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_SANDSTONE:
		case E_BLOCK_STONE_BRICKS:
		case E_BLOCK_NETHER_BRICK:
		case E_BLOCK_NETHERRACK:
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_DOUBLE_STONE_SLAB:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_BRICK:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_NETHER_BRICK_STAIRS:
			return PickaxeLevel() >= 1;
		}
	}
};