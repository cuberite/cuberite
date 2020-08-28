
// LootTables.h

#pragma once

#include "../LootTableProvider.h"

/*
This files contains all default loot tables
*/

class cAbandonedMineshaftLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			3, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{}, E_ITEM_GOLDEN_APPLE, 20},
				{{}, E_ITEM_NAME_TAG, 30},
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_BOOK, 10},
				{{}, E_ITEM_IRON_PICKAXE, 5},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_INGOT, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "9"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_REDSTONE, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "9"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_LAPIS_LAZULI, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_DIAMOND, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "3"}}}, E_ITEM_COAL, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BREAD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_MELON_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_PUMPKIN_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_BEETROOT_SEEDS, 10},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_RAIL, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_POWERED_RAIL, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_DETECTOR_RAIL, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_ACTIVATOR_RAIL, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "16"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_TORCH, 15},
			},
		},
	};

}

class cBuriedTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			2, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_TNT, 5},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_EMERALD, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_DIAMOND, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_PRISMARINE_CRYSTALS, 5},
			},
		},
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cDesertPyramidLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			4, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_DIAMOND, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_GOLD_INGOT, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_EMERALD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_BONE, 25},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_SPIDER_EYE, 25},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "3"}}}, E_ITEM_ROTTEN_FLESH, 25},
				{{}, E_ITEM_SADDLE, 20},
				{{}, E_ITEM_IRON_HORSE_ARMOR, 15},
				{{}, E_ITEM_GOLDEN_HORSE_ARMOR, 10},
				{{}, E_ITEM_DIAMOND_HORSE_ARMOR, 5},
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_BOOK, 20},
				{{}, E_ITEM_GOLDEN_APPLE, 20},
				{{}, E_ITEM_ENCHANTED_GOLDEN_APPLE, 2},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BONE, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GUNPOWDER, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_ROTTEN_FLESH, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_STRING, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_SAND, 10},
			},
		},
	};

}

class cEndCityTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {2, 6},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_DIAMOND, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_IRON_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_GOLD_INGOT, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_EMERALD, 2},
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BEETROOT_SEEDS, 5},
				{{}, E_ITEM_SADDLE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_DIAMOND_SWORD, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_DIAMOND_BOOTS, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_DIAMOND_CHESTPLATE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_DIAMOND_LEGGINGS, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_DIAMOND_HELMET, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_DIAMOND_PICKAXE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_DIAMOND_SHOVEL, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_IRON_SWORD, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_IRON_BOOTS, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_IRON_CHESTPLATE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_IRON_LEGGINGS, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_IRON_HELMET, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_IRON_PICKAXE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}{"type", "minecraft:uniform"}{"min", "20"}}}, E_ITEM_IRON_SHOVEL, 3},
			},
		},
	};

}

class cIglooChestLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_APPLE, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_COAL, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_NUGGET, 10},
				{{}, E_ITEM_STONE_AXE, 2},
				{{}, E_ITEM_ROTTEN_FLESH, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_WHEAT, 10},
			},
		},
		{
			{
			},
		},
	};

}

class cJungleTempleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {2, 6},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_DIAMOND, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_GOLD_INGOT, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BAMBOO, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_EMERALD, 2},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_BONE, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "3"}}}, E_ITEM_ROTTEN_FLESH, 16},
				{{}, E_ITEM_SADDLE, 3},
			},
		},
	};

}

class cJungleTempleDispenserLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {1, 2},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_ARROW, 30},
			},
		},
	};

}

class cNetherBridgeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {2, 4},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_DIAMOND, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_INGOT, 15},
				{{}, E_ITEM_GOLDEN_SWORD, 5},
				{{}, E_ITEM_GOLDEN_CHESTPLATE, 5},
				{{}, E_ITEM_FLINT_AND_STEEL, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "3"}}}, E_ITEM_NETHER_WART, 5},
				{{}, E_ITEM_SADDLE, 10},
				{{}, E_ITEM_GOLDEN_HORSE_ARMOR, 8},
				{{}, E_ITEM_IRON_HORSE_ARMOR, 5},
				{{}, E_ITEM_DIAMOND_HORSE_ARMOR, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_OBSIDIAN, 2},
			},
		},
	};

}

class cPillagerOutpostLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {2, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "3"}}}, E_ITEM_WHEAT, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_POTATO, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "3"}}}, E_ITEM_CARROT, 5},
			},
		},
		{
			{
			},
		},
		{
			{
				{{}, E_ITEM_EXPERIENCE_BOTTLE, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_STRING, 4},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_ARROW, 4},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_TRIPWIRE_HOOK, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 3},
			},
		},
	};

}

class cShipwreckMapLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			3, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_PAPER, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_FEATHER, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BOOK, 5},
			},
		},
	};

}

class cShipwreckSupplyLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {3, 10},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "12"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_PAPER, 8},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_POTATO, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_POISONOUS_POTATO, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_CARROT, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "21"}{"type", "minecraft:uniform"}{"min", "8"}}}, E_ITEM_WHEAT, 7},
				{{{LootTable::eFunctionType::SetStewEffect, {}}, E_ITEM_SUSPICIOUS_STEW, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_COAL, 6},
				{{{LootTable::eFunctionType::SetCount, {{"max", "24"}{"type", "minecraft:uniform"}{"min", "5"}}}, E_ITEM_ROTTEN_FLESH, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_PUMPKIN, 2},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BAMBOO, 2},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GUNPOWDER, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "1"}}{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_LEATHER_HELMET, 3},
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_LEATHER_CHESTPLATE, 3},
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_LEATHER_LEGGINGS, 3},
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_LEATHER_BOOTS, 3},
			},
		},
	};

}

class cShipwreckTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {2, 5},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 90},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_EMERALD, 40},
				{{}, E_ITEM_DIAMOND, 5},
				{{}, E_ITEM_EXPERIENCE_BOTTLE, 5},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_NUGGET, 50},
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_NUGGET, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_LAPIS_LAZULI, 20},
			},
		},
	};

}

class cSimpleDungeonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			3, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{}, E_ITEM_SADDLE, 20},
				{{}, E_ITEM_GOLDEN_APPLE, 15},
				{{}, E_ITEM_ENCHANTED_GOLDEN_APPLE, 2},
				{{}, E_ITEM_MUSIC_DISC_13, 15},
				{{}, E_ITEM_MUSIC_DISC_CAT, 15},
				{{}, E_ITEM_NAME_TAG, 20},
				{{}, E_ITEM_GOLDEN_HORSE_ARMOR, 10},
				{{}, E_ITEM_IRON_HORSE_ARMOR, 15},
				{{}, E_ITEM_DIAMOND_HORSE_ARMOR, 5},
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_BOOK, 10},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_INGOT, 5},
				{{}, E_ITEM_BREAD, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_WHEAT, 20},
				{{}, E_ITEM_BUCKET, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_REDSTONE, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_COAL, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_MELON_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_PUMPKIN_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_BEETROOT_SEEDS, 10},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BONE, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GUNPOWDER, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_ROTTEN_FLESH, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_STRING, 10},
			},
		},
	};

}

class cSpawnBonusChestLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			4, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{}, E_ITEM_WOODEN_AXE, 3},
			},
		},
		{
			{
				{{}, E_ITEM_WOODEN_PICKAXE, 3},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_APPLE, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BREAD, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_SALMON, 3},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "12"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_STICK, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "12"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_OAK_PLANKS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_OAK_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_SPRUCE_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BIRCH_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_JUNGLE_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_ACACIA_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_DARK_OAK_LOG, 3},
			},
		},
	};

}

class cStrongholdCorridorLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {2, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{}, E_ITEM_ENDER_PEARL, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_DIAMOND, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_INGOT, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "9"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_REDSTONE, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BREAD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_APPLE, 15},
				{{}, E_ITEM_IRON_PICKAXE, 5},
				{{}, E_ITEM_IRON_SWORD, 5},
				{{}, E_ITEM_IRON_CHESTPLATE, 5},
				{{}, E_ITEM_IRON_HELMET, 5},
				{{}, E_ITEM_IRON_LEGGINGS, 5},
				{{}, E_ITEM_IRON_BOOTS, 5},
			},
		},
	};

}

class cStrongholdCrossingLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {1, 4},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_INGOT, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "9"}{"type", "minecraft:uniform"}{"min", "4"}}}, E_ITEM_REDSTONE, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "3"}}}, E_ITEM_COAL, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BREAD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_APPLE, 15},
			},
		},
	};

}

class cStrongholdLibraryLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {2, 10},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BOOK, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_PAPER, 20},
				{{{LootTable::eFunctionType::EnchantWithLevels}, E_ITEM_BOOK, 10},
			},
		},
	};

}

class cUnderwaterRuinBigLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_COAL, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_NUGGET, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_WHEAT, 10},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_BOOK, 5},
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_FISHING_ROD, 5},
				{{{LootTable::eFunctionType::ExplorationMapLootTable::eFunctionType::RedXLootTable::eFunctionType::BuriedTreasure}, E_ITEM_MAP, 10},
			},
		},
	};

}

class cUnderwaterRuinSmallLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_COAL, 10},
				{{}, E_ITEM_STONE_AXE, 2},
				{{}, E_ITEM_ROTTEN_FLESH, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_WHEAT, 10},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_FISHING_ROD, 5},
				{{{LootTable::eFunctionType::ExplorationMapLootTable::eFunctionType::RedXLootTable::eFunctionType::BuriedTreasure}, E_ITEM_MAP, 5},
			},
		},
	};

}

class cWoodlandMansionLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
			3, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{}, E_ITEM_LEAD, 20},
				{{}, E_ITEM_GOLDEN_APPLE, 15},
				{{}, E_ITEM_ENCHANTED_GOLDEN_APPLE, 2},
				{{}, E_ITEM_MUSIC_DISC_13, 15},
				{{}, E_ITEM_MUSIC_DISC_CAT, 15},
				{{}, E_ITEM_NAME_TAG, 20},
				{{}, E_ITEM_CHAINMAIL_CHESTPLATE, 10},
				{{}, E_ITEM_DIAMOND_HOE, 15},
				{{}, E_ITEM_DIAMOND_CHESTPLATE, 5},
				{{{LootTable::eFunctionType::EnchantRandomly}, E_ITEM_BOOK, 10},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_IRON_INGOT, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GOLD_INGOT, 5},
				{{}, E_ITEM_BREAD, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_WHEAT, 20},
				{{}, E_ITEM_BUCKET, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_REDSTONE, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_COAL, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_MELON_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_PUMPKIN_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}{"type", "minecraft:uniform"}{"min", "2"}}}, E_ITEM_BEETROOT_SEEDS, 10},
			},
		},
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_BONE, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_GUNPOWDER, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_ROTTEN_FLESH, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}{"type", "minecraft:uniform"}{"min", "1"}}}, E_ITEM_STRING, 10},
			},
		},
	};

}

class cArmorStandLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cBatLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cBlazeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cCatLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cCaveSpiderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cChickenLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cCodLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cCowLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cCreeperLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cDolphinLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cDonkeyLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cDrownedLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cElderGuardianLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
				{{{LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}LootTable::eFunctionType::FurnaceSmelt, {}}, E_ITEM_COD, 3},
				{{{LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}}, E_ITEM_PRISMARINE_CRYSTALS, 2},
			},
		},
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cEndermanLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cEndermiteLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cEnderDragonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cEvokerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cFoxLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cGhastLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cGiantLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cGuardianLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
				{{{LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}LootTable::eFunctionType::FurnaceSmelt, {}}, E_ITEM_COD, 2},
				{{{LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}}, E_ITEM_PRISMARINE_CRYSTALS, 2},
			},
		},
		{
			{
			},
		},
	};

}

class cHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cHuskLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cIllusionerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cIronGolemLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cLlamaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cMagmaCubeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cMooshroomLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cMuleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cOcelotLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cPandaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cParrotLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cPhantomLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cPigLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cPillagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cPlayerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cPolarBearLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}}, E_ITEM_COD, 3},
			},
		},
	};

}

class cPufferfishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cRabbitLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cRavagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cSalmonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cSheepLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cShulkerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cSilverfishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cSkeletonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cSkeletonHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cSlimeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cSnowGolemLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cSpiderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cSquidLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cStrayLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cTraderLlamaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cTropicalFishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cTurtleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}}, E_ITEM_SEAGRASS, 3},
			},
		},
		{
			{
			},
		},
	};

}

class cVexLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cVillagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cVindicatorLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cWanderingTraderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cWitchLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			-1, {1, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}{LootTable::eFunctionType::SetCount, {{"max", "2"}{"type", "minecraft:uniform"}{"min", "0"}}LootTable::eFunctionType::LootingEnchant, {{"min", "0"}{"max", "1"}}}, E_ITEM_STICK, 2},
			},
		},
	};

}

class cWitherLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cWitherSkeletonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cWolfLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::None;

class cZombieLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cZombieHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
	};

}

class cZombiePigmanLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
		{
			{
			},
		},
	};

}

class cZombieVillagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
	{
			1, {0, 0},  // Rolls
			0, {0, 0},  // Bonus Rolls
		{
			{
			},
		},
		{
			{
			},
		},
	};

}
