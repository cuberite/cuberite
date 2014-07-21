
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"

class cItemPickaxeHandler :
	public cItemHandler
{
public:
	cItemPickaxeHandler(int a_ItemType)
	: cItemHandler(a_ItemType)
	{

	}

	char PickaxeLevel()
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_PICKAXE:  return 1;
			case E_ITEM_GOLD_PICKAXE:    return 1;
			case E_ITEM_STONE_PICKAXE:   return 2;
			case E_ITEM_IRON_PICKAXE:    return 3;
			case E_ITEM_DIAMOND_PICKAXE: return 4;

			default: return 0;
		}
	}
	
	virtual bool CanHarvestBlock(BLOCKTYPE a_BlockType) override
	{
		switch (a_BlockType)
		{
			case E_BLOCK_OBSIDIAN:
			{
				return PickaxeLevel() >= 4;
			}
			
			case E_BLOCK_DIAMOND_BLOCK:
			case E_BLOCK_DIAMOND_ORE:
			case E_BLOCK_GOLD_BLOCK:
			case E_BLOCK_GOLD_ORE:
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			case E_BLOCK_EMERALD_ORE:
			{
				return PickaxeLevel() >= 3;
			}
			
			case E_BLOCK_IRON_BLOCK:
			case E_BLOCK_IRON_ORE:
			case E_BLOCK_LAPIS_ORE:
			case E_BLOCK_LAPIS_BLOCK:
			{
				return PickaxeLevel() >= 2;
			}
			
			case E_BLOCK_ANVIL:
			case E_BLOCK_ENCHANTMENT_TABLE:
			case E_BLOCK_FURNACE:
			case E_BLOCK_LIT_FURNACE:
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
			case E_BLOCK_COBBLESTONE_WALL:
			case E_BLOCK_STONE_BRICK_STAIRS:
			case E_BLOCK_NETHER_BRICK_STAIRS:
			case E_BLOCK_CAULDRON:
			{
				return PickaxeLevel() >= 1;
			}
		}
		return false;
	}

	virtual bool CanRepairWithRawMaterial(short a_ItemType) override
	{
		switch (m_ItemType)
		{
			case E_ITEM_WOODEN_PICKAXE:  return (a_ItemType == E_BLOCK_PLANKS);
			case E_ITEM_STONE_PICKAXE:   return (a_ItemType == E_BLOCK_COBBLESTONE);
			case E_ITEM_IRON_PICKAXE:    return (a_ItemType == E_ITEM_IRON);
			case E_ITEM_GOLD_PICKAXE:    return (a_ItemType == E_ITEM_GOLD);
			case E_ITEM_DIAMOND_PICKAXE: return (a_ItemType == E_ITEM_DIAMOND);
		}
		return false;
	}
} ;




