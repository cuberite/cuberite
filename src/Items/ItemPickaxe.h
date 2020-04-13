
#pragma once

#include "ItemHandler.h"



class cItemPickaxeHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemPickaxeHandler(int a_ItemType):
		Super(a_ItemType)
	{

	}



	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		switch (a_Action)
		{
			case dlaAttackEntity:       return 2;
			case dlaBreakBlock:         return 1;
			case dlaBreakBlockInstant:  return 0;
		}
		UNREACHABLE("Unsupported durability loss action");
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
		// NOTICE: Make sure to update cItemHandler::CanHarvestBlock() if adding new blocks here!
		switch (a_BlockType)
		{
			case E_BLOCK_OBSIDIAN:
			{
				return PickaxeLevel() >= 4;
			}

			case E_BLOCK_DIAMOND_BLOCK:
			case E_BLOCK_DIAMOND_ORE:
			case E_BLOCK_EMERALD_ORE:
			case E_BLOCK_GOLD_BLOCK:
			case E_BLOCK_GOLD_ORE:
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
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
			case E_BLOCK_BLACK_GLAZED_TERRACOTTA:
			case E_BLOCK_BLOCK_OF_COAL:
			case E_BLOCK_BLOCK_OF_REDSTONE:
			case E_BLOCK_BLUE_GLAZED_TERRACOTTA:
			case E_BLOCK_BONE_BLOCK:
			case E_BLOCK_BREWING_STAND:
			case E_BLOCK_BRICK:
			case E_BLOCK_BRICK_STAIRS:
			case E_BLOCK_BROWN_GLAZED_TERRACOTTA:
			case E_BLOCK_CAULDRON:
			case E_BLOCK_COAL_ORE:
			case E_BLOCK_COBBLESTONE:
			case E_BLOCK_COBBLESTONE_STAIRS:
			case E_BLOCK_COBBLESTONE_WALL:
			case E_BLOCK_CONCRETE:
			case E_BLOCK_CYAN_GLAZED_TERRACOTTA:
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB:
			case E_BLOCK_DOUBLE_STONE_SLAB:
			case E_BLOCK_ENCHANTMENT_TABLE:
			case E_BLOCK_END_BRICKS:
			case E_BLOCK_END_STONE:
			case E_BLOCK_FURNACE:
			case E_BLOCK_GRAY_GLAZED_TERRACOTTA:
			case E_BLOCK_GREEN_GLAZED_TERRACOTTA:
			case E_BLOCK_HARDENED_CLAY:
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_HOPPER:
			case E_BLOCK_IRON_BARS:
			case E_BLOCK_IRON_DOOR:
			case E_BLOCK_IRON_TRAPDOOR:
			case E_BLOCK_LIGHT_BLUE_GLAZED_TERRACOTTA:
			case E_BLOCK_LIGHT_GRAY_GLAZED_TERRACOTTA:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_LIT_FURNACE:
			case E_BLOCK_LIME_GLAZED_TERRACOTTA:
			case E_BLOCK_MAGENTA_GLAZED_TERRACOTTA:
			case E_BLOCK_MOB_SPAWNER:
			case E_BLOCK_MOSSY_COBBLESTONE:
			case E_BLOCK_MAGMA:
			case E_BLOCK_NETHER_BRICK:
			case E_BLOCK_NETHER_BRICK_STAIRS:
			case E_BLOCK_NETHER_BRICK_FENCE:
			case E_BLOCK_NETHER_QUARTZ_ORE:
			case E_BLOCK_NETHERRACK:
			case E_BLOCK_OBSERVER:
			case E_BLOCK_ORANGE_GLAZED_TERRACOTTA:
			case E_BLOCK_PRISMARINE_BLOCK:
			case E_BLOCK_PINK_GLAZED_TERRACOTTA:
			case E_BLOCK_PURPLE_GLAZED_TERRACOTTA:
			case E_BLOCK_PURPUR_BLOCK:
			case E_BLOCK_PURPUR_DOUBLE_SLAB:
			case E_BLOCK_PURPUR_PILLAR:
			case E_BLOCK_PURPUR_SLAB:
			case E_BLOCK_PURPUR_STAIRS:
			case E_BLOCK_QUARTZ_BLOCK:
			case E_BLOCK_QUARTZ_STAIRS:
			case E_BLOCK_RED_GLAZED_TERRACOTTA:
			case E_BLOCK_RED_NETHER_BRICK:
			case E_BLOCK_RED_SANDSTONE:
			case E_BLOCK_RED_SANDSTONE_STAIRS:
			case E_BLOCK_SANDSTONE:
			case E_BLOCK_SANDSTONE_STAIRS:
			case E_BLOCK_STONE:
			case E_BLOCK_STONE_BRICKS:
			case E_BLOCK_STONE_BRICK_STAIRS:
			case E_BLOCK_STONE_PRESSURE_PLATE:
			case E_BLOCK_STONE_SLAB:
			case E_BLOCK_WHITE_GLAZED_TERRACOTTA:
			case E_BLOCK_YELLOW_GLAZED_TERRACOTTA:
			{
				return PickaxeLevel() >= 1;
			}
		}
		return Super::CanHarvestBlock(a_BlockType);
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


	virtual float GetBlockBreakingStrength(BLOCKTYPE a_Block) override
	{
		if (!IsBlockMaterialIron(a_Block) && (a_Block != E_BLOCK_ANVIL) && !IsBlockMaterialRock(a_Block))
		{
			return Super::GetBlockBreakingStrength(a_Block);
		}
		else
		{
			switch (m_ItemType)
			{
				case E_ITEM_WOODEN_PICKAXE:  return 2.0f;
				case E_ITEM_STONE_PICKAXE:   return 4.0f;
				case E_ITEM_IRON_PICKAXE:    return 6.0f;
				case E_ITEM_GOLD_PICKAXE:    return 12.0f;
				case E_ITEM_DIAMOND_PICKAXE: return 8.0f;
			}
		}
		ASSERT(!"Something is wrong here... Maybe they are pickaxes out of a new material?");
		return 1.0f;
	}

} ;
