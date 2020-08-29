
// LootTables.h

#pragma once

#include "../LootTableProvider.h"

/*
This files contains all default loot tables
*/

class cAbandonedMineshaftLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_GOLDEN_APPLE, 20},
				{{}, E_ITEM_ENCHANTED_GOLDEN_APPLE, 0},
				{{}, E_ITEM_NAME_TAG, 30},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_BOOK, 10},
				{{}, E_ITEM_IRON_PICKAXE, 5},
				{{}, E_ITEM_IRON_PICKAXE, 0},
			},

		},
		{
			-1, {2, 4},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "9"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_REDSTONE, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "9"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_LAPIS_LAZULI, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_DIAMOND, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "3"}}, {}}}, E_ITEM_COAL, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BREAD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_MELON_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_PUMPKIN_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_BEETROOT_SEEDS, 10},
			},

		},
		{
		3, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_RAIL, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_POWERED_RAIL, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_DETECTOR_RAIL, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_ACTIVATOR_RAIL, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "16"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_TORCH, 15},
			},

		},
	};
};

class cBuriedTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_HEART_OF_THE_SEA, 0},
			},

		},
		{
			-1, {5, 8},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_TNT, 5},
			},

		},
		{
			-1, {1, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_EMERALD, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_DIAMOND, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_PRISMARINE_CRYSTALS, 5},
			},

		},
		{
			-1, {0, 1},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_LEATHER_CHESTPLATE, 0},
				{{}, E_ITEM_IRON_SWORD, 0},
			},

		},
		{
		2, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_COOKED_COD, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_COOKED_SALMON, 0},
			},

		},
	};
};

class cDesertPyramidLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 4},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_DIAMOND, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_GOLD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_EMERALD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_BONE, 25},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_SPIDER_EYE, 25},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "3"}}, {}}}, E_ITEM_ROTTEN_FLESH, 25},
				{{}, E_ITEM_SADDLE, 20},
				{{}, E_ITEM_IRON_HORSE_ARMOR, 15},
				{{}, E_ITEM_GOLDEN_HORSE_ARMOR, 10},
				{{}, E_ITEM_DIAMOND_HORSE_ARMOR, 5},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_BOOK, 20},
				{{}, E_ITEM_GOLDEN_APPLE, 20},
				{{}, E_ITEM_ENCHANTED_GOLDEN_APPLE, 2},
				{{}, E_ITEM_ENCHANTED_GOLDEN_APPLE, 0},
			},

		},
		{
		4, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BONE, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GUNPOWDER, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_STRING, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_SAND, 10},
			},

		},
	};
};

class cEndCityTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 6},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_DIAMOND, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_IRON, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_GOLD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_EMERALD, 2},
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BEETROOT_SEEDS, 5},
				{{}, E_ITEM_SADDLE, 3},
				{{}, E_ITEM_IRON_HORSE_ARMOR, 0},
				{{}, E_ITEM_GOLDEN_HORSE_ARMOR, 0},
				{{}, E_ITEM_DIAMOND_HORSE_ARMOR, 0},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_DIAMOND_SWORD, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_DIAMOND_BOOTS, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_DIAMOND_CHESTPLATE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_DIAMOND_LEGGINGS, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_DIAMOND_HELMET, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_DIAMOND_PICKAXE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_DIAMOND_SHOVEL, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_IRON_SWORD, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_IRON_BOOTS, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_IRON_CHESTPLATE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_IRON_LEGGINGS, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_IRON_HELMET, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_IRON_PICKAXE, 3},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {}}}, E_ITEM_IRON_SHOVEL, 3},
			},

		},
	};
};

class cIglooChestLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 8},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_APPLE, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_COAL, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD_NUGGET, 10},
				{{}, E_ITEM_STONE_AXE, 2},
				{{}, E_ITEM_ROTTEN_FLESH, 10},
				{{}, E_ITEM_EMERALD, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_WHEAT, 10},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_GOLDEN_APPLE, 0},
			},

		},
	};
};

class cJungleTempleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 6},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_DIAMOND, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_GOLD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BAMBOO, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_EMERALD, 2},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_BONE, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "3"}}, {}}}, E_ITEM_ROTTEN_FLESH, 16},
				{{}, E_ITEM_SADDLE, 3},
				{{}, E_ITEM_IRON_HORSE_ARMOR, 0},
				{{}, E_ITEM_GOLDEN_HORSE_ARMOR, 0},
				{{}, E_ITEM_DIAMOND_HORSE_ARMOR, 0},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"levels", "30"}, {"treasure", "True"}}, {}}}, E_ITEM_BOOK, 0},
			},

		},
	};
};

class cJungleTempleDispenserLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {1, 2},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_ARROW, 30},
			},

		},
	};
};

class cNetherBridgeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 4},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_DIAMOND, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD, 15},
				{{}, E_ITEM_GOLDEN_SWORD, 5},
				{{}, E_ITEM_GOLDEN_CHESTPLATE, 5},
				{{}, E_ITEM_FLINT_AND_STEEL, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "3"}}, {}}}, E_ITEM_NETHER_WART, 5},
				{{}, E_ITEM_SADDLE, 10},
				{{}, E_ITEM_GOLDEN_HORSE_ARMOR, 8},
				{{}, E_ITEM_IRON_HORSE_ARMOR, 5},
				{{}, E_ITEM_DIAMOND_HORSE_ARMOR, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_OBSIDIAN, 2},
			},

		},
	};
};

class cPillagerOutpostLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {0, 1},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_CROSSBOW, 0},
			},

		},
		{
			-1, {2, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "3"}}, {}}}, E_ITEM_WHEAT, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_POTATO, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "3"}}, {}}}, E_ITEM_CARROT, 5},
			},

		},
		{
			-1, {1, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_DARK_OAK_LOG, 0},
			},

		},
		{
			-1, {2, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_EXPERIENCE_BOTTLE, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_STRING, 4},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_ARROW, 4},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_TRIPWIRE_HOOK, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 3},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_BOOK, 0},
			},

		},
	};
};

class cShipwreckMapLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::ExplorationMap, {{"decoration", "RedX"}, {"skip_existing_chunks", "False"}, {"destination", "BuriedTreasure"}, {"zoom", "1"}}, {}}}, E_ITEM_MAP, 0},
			},

		},
		{
		3, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_COMPASS, 0},
				{{}, E_ITEM_MAP, 0},
				{{}, E_ITEM_CLOCK, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_PAPER, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_FEATHER, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BOOK, 5},
			},

		},
	};
};

class cShipwreckSupplyLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {3, 10},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "12"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_PAPER, 8},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_POTATO, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_POISONOUS_POTATO, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_CARROT, 7},
				{{{LootTable::eFunctionType::SetCount, {{"max", "21"}, {"type", "Uniform"}, {"min", "8"}}, {}}}, E_ITEM_WHEAT, 7},
				{{{LootTable::eFunctionType::SetStewEffect, {}, {}}}, E_ITEM_SUSPICIOUS_STEW, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_COAL, 6},
				{{{LootTable::eFunctionType::SetCount, {{"max", "24"}, {"type", "Uniform"}, {"min", "5"}}, {}}}, E_ITEM_ROTTEN_FLESH, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_PUMPKIN, 2},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BAMBOO, 2},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GUNPOWDER, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_TNT, 0},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_LEATHER_HELMET, 3},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_LEATHER_CHESTPLATE, 3},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_LEATHER_LEGGINGS, 3},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_LEATHER_BOOTS, 3},
			},

		},
	};
};

class cShipwreckTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {3, 6},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 90},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_EMERALD, 40},
				{{}, E_ITEM_DIAMOND, 5},
				{{}, E_ITEM_EXPERIENCE_BOTTLE, 5},
			},

		},
		{
			-1, {2, 5},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON_NUGGET, 50},
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD_NUGGET, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_LAPIS_LAZULI, 20},
			},

		},
	};
};

class cSimpleDungeonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {1, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
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
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_BOOK, 10},
			},

		},
		{
			-1, {1, 4},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD, 5},
				{{}, E_ITEM_BREAD, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_WHEAT, 20},
				{{}, E_ITEM_BUCKET, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_REDSTONE, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_COAL, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_MELON_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_PUMPKIN_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_BEETROOT_SEEDS, 10},
			},

		},
		{
		3, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BONE, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GUNPOWDER, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_STRING, 10},
			},

		},
	};
};

class cSpawnBonusChestLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_STONE_AXE, 0},
				{{}, E_ITEM_WOODEN_AXE, 3},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_STONE_PICKAXE, 0},
				{{}, E_ITEM_WOODEN_PICKAXE, 3},
			},

		},
		{
		3, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_APPLE, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BREAD, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_SALMON, 3},
			},

		},
		{
		4, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "12"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_STICK, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "12"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_OAK_PLANKS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_OAK_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_SPRUCE_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BIRCH_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_JUNGLE_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_ACACIA_LOG, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_DARK_OAK_LOG, 3},
			},

		},
	};
};

class cStrongholdCorridorLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_ENDER_PEARL, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_DIAMOND, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "9"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_REDSTONE, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BREAD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_APPLE, 15},
				{{}, E_ITEM_IRON_PICKAXE, 5},
				{{}, E_ITEM_IRON_SWORD, 5},
				{{}, E_ITEM_IRON_CHESTPLATE, 5},
				{{}, E_ITEM_IRON_HELMET, 5},
				{{}, E_ITEM_IRON_LEGGINGS, 5},
				{{}, E_ITEM_IRON_BOOTS, 5},
				{{}, E_ITEM_GOLDEN_APPLE, 0},
				{{}, E_ITEM_SADDLE, 0},
				{{}, E_ITEM_IRON_HORSE_ARMOR, 0},
				{{}, E_ITEM_GOLDEN_HORSE_ARMOR, 0},
				{{}, E_ITEM_DIAMOND_HORSE_ARMOR, 0},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"levels", "30"}, {"treasure", "True"}}, {}}}, E_ITEM_BOOK, 0},
			},

		},
	};
};

class cStrongholdCrossingLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {1, 4},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "9"}, {"type", "Uniform"}, {"min", "4"}}, {}}}, E_ITEM_REDSTONE, 5},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "3"}}, {}}}, E_ITEM_COAL, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BREAD, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_APPLE, 15},
				{{}, E_ITEM_IRON_PICKAXE, 0},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"levels", "30"}, {"treasure", "True"}}, {}}}, E_ITEM_BOOK, 0},
			},

		},
	};
};

class cStrongholdLibraryLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 10},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BOOK, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_PAPER, 20},
				{{}, E_ITEM_MAP, 0},
				{{}, E_ITEM_COMPASS, 0},
				{{{LootTable::eFunctionType::EnchantWithLevels, {{"levels", "30"}, {"treasure", "True"}}, {}}}, E_ITEM_BOOK, 10},
			},

		},
	};
};

class cUnderwaterRuinBigLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 8},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_COAL, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD_NUGGET, 10},
				{{}, E_ITEM_EMERALD, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_WHEAT, 10},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_GOLDEN_APPLE, 0},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_BOOK, 5},
				{{}, E_ITEM_LEATHER_CHESTPLATE, 0},
				{{}, E_ITEM_GOLDEN_HELMET, 0},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_FISHING_ROD, 5},
				{{{LootTable::eFunctionType::ExplorationMap, {{"decoration", "RedX"}, {"skip_existing_chunks", "False"}, {"destination", "BuriedTreasure"}, {"zoom", "1"}}, {}}}, E_ITEM_MAP, 10},
			},

		},
	};
};

class cUnderwaterRuinSmallLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {2, 8},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_COAL, 10},
				{{}, E_ITEM_STONE_AXE, 2},
				{{}, E_ITEM_ROTTEN_FLESH, 5},
				{{}, E_ITEM_EMERALD, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_WHEAT, 10},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_LEATHER_CHESTPLATE, 0},
				{{}, E_ITEM_GOLDEN_HELMET, 0},
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_FISHING_ROD, 5},
				{{{LootTable::eFunctionType::ExplorationMap, {{"decoration", "RedX"}, {"skip_existing_chunks", "False"}, {"destination", "BuriedTreasure"}, {"zoom", "1"}}, {}}}, E_ITEM_MAP, 5},
			},

		},
	};
};

class cWoodlandMansionLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {1, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
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
				{{{LootTable::eFunctionType::EnchantRandomly, {}, {}}}, E_ITEM_BOOK, 10},
			},

		},
		{
			-1, {1, 4},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_IRON, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GOLD, 5},
				{{}, E_ITEM_BREAD, 20},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_WHEAT, 20},
				{{}, E_ITEM_BUCKET, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_REDSTONE, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_COAL, 15},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_MELON_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_PUMPKIN_SEEDS, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {}}}, E_ITEM_BEETROOT_SEEDS, 10},
			},

		},
		{
		3, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_BONE, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_GUNPOWDER, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 10},
				{{{LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {}}}, E_ITEM_STRING, 10},
			},

		},
	};
};

class cArmorStandLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cBatLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cBlazeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_BLAZE_ROD, 0},
			},

		},
	};
};

class cCatLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}}, E_ITEM_STRING, 0},
			},

		},
	};
};

class cCaveSpiderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_STRING, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "-1"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_SPIDER_EYE, 0},
			},

		},
	};
};

class cChickenLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_FEATHER, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_CHICKEN, 0},
			},

		},
	};
};

class cCodLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}}, E_ITEM_COD, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_BONE_MEAL, 0},
			},

		},
	};
};

class cCowLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_LEATHER, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}, {LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_BEEF, 0},
			},

		},
	};
};

class cCreeperLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_GUNPOWDER, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_MUSIC_DISCS, 0},
			},

		},
	};
};

class cDolphinLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}, {LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}}, E_ITEM_COD, 0},
			},

		},
	};
};

class cDonkeyLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_LEATHER, 0},
			},

		},
	};
};

class cDrownedLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_GOLD, 0},
			},

		},
	};
};

class cElderGuardianLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_PRISMARINE_SHARD, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}, {LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}}, E_ITEM_COD, 3},
				{{{LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_PRISMARINE_CRYSTALS, 2},
				{{}, E_ITEM_PRISMARINE_CRYSTALS, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_WET_SPONGE, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_GAMEPLAY/FISHING/FISH, 0},
			},

		},
	};
};

class cEndermanLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ENDER_PEARL, 0},
			},

		},
	};
};

class cEndermiteLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cEnderDragonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cEvokerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_TOTEM_OF_UNDYING, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_EMERALD, 0},
			},

		},
	};
};

class cFoxLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cGhastLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_GHAST_TEAR, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_GUNPOWDER, 0},
			},

		},
	};
};

class cGiantLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cGuardianLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_PRISMARINE_SHARD, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}, {LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}}, E_ITEM_COD, 2},
				{{{LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_PRISMARINE_CRYSTALS, 2},
				{{}, E_ITEM_PRISMARINE_CRYSTALS, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_GAMEPLAY/FISHING/FISH, 0},
			},

		},
	};
};

class cHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_LEATHER, 0},
			},

		},
	};
};

class cHuskLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_IRON, 0},
				{{}, E_ITEM_CARROT, 0},
				{{}, E_ITEM_POTATO, 0},
			},

		},
	};
};

class cIllusionerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cIronGolemLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}}, E_ITEM_POPPY, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "3"}}, {}}}, E_ITEM_IRON, 0},
			},

		},
	};
};

class cLlamaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_LEATHER, 0},
			},

		},
	};
};

class cMagmaCubeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "-2"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_MAGMA_CREAM, 0},
			},

		},
	};
};

class cMooshroomLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_LEATHER, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}, {LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_BEEF, 0},
			},

		},
	};
};

class cMuleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_LEATHER, 0},
			},

		},
	};
};

class cOcelotLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cPandaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"count", "1"}}, {}}}, E_ITEM_BAMBOO, 0},
			},

		},
	};
};

class cParrotLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_FEATHER, 0},
			},

		},
	};
};

class cPhantomLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_PHANTOM_MEMBRANE, 0},
			},

		},
	};
};

class cPigLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}, {LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_PORKCHOP, 0},
			},

		},
	};
};

class cPillagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cPlayerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cPolarBearLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_COD, 3},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_SALMON, 0},
			},

		},
	};
};

class cPufferfishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"count", "1"}}, {}}}, E_ITEM_PUFFERFISH, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_BONE_MEAL, 0},
			},

		},
	};
};

class cRabbitLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_RABBIT_HIDE, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_RABBIT, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_RABBIT_FOOT, 0},
			},

		},
	};
};

class cRavagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"count", "1"}}, {}}}, E_ITEM_SADDLE, 0},
			},

		},
	};
};

class cSalmonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}}, E_ITEM_SALMON, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_BONE_MEAL, 0},
			},

		},
	};
};

class cSheepLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}}, {LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_MUTTON, 0},
			},

		},
	};
};

class cShulkerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_SHULKER_SHELL, 0},
			},

		},
	};
};

class cSilverfishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cSkeletonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ARROW, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_BONE, 0},
			},

		},
	};
};

class cSkeletonHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_BONE, 0},
			},

		},
	};
};

class cSlimeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_SLIME_BALL, 0},
			},

		},
	};
};

class cSnowGolemLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "15"}, {"type", "Uniform"}, {"min", "0"}}, {}}}, E_ITEM_SNOWBALL, 0},
			},

		},
	};
};

class cSpiderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_STRING, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "-1"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_SPIDER_EYE, 0},
			},

		},
	};
};

class cSquidLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_INK_SAC, 0},
			},

		},
	};
};

class cStrayLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ARROW, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_BONE, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}, {"limit", "1"}}, {}}, {LootTable::eFunctionType::SetNbt, {{"tag", "{Potion:"Slowness"}"}}, {}}}, E_ITEM_TIPPED_ARROW, 0},
			},

		},
	};
};

class cTraderLlamaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_LEATHER, 0},
			},

		},
	};
};

class cTropicalFishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"count", "1"}}, {}}}, E_ITEM_TROPICAL_FISH, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_BONE_MEAL, 0},
			},

		},
	};
};

class cTurtleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_SEAGRASS, 3},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_BOWL, 0},
			},

		},
	};
};

class cVexLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cVillagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cVindicatorLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_EMERALD, 0},
			},

		},
	};
};

class cWanderingTraderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cWitchLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
			-1, {1, 3},  // Rolls
			0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_GLOWSTONE_DUST, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_SUGAR, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_REDSTONE, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_SPIDER_EYE, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_GLASS_BOTTLE, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_GUNPOWDER, 0},
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_STICK, 2},
			},

		},
	};
};

class cWitherLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cWitherSkeletonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "-1"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_COAL, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_BONE, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_WITHER_SKELETON_SKULL, 0},
			},

		},
	};
};

class cWolfLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cZombieLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_IRON, 0},
				{{}, E_ITEM_CARROT, 0},
				{{}, E_ITEM_POTATO, 0},
			},

		},
	};
};

class cZombieHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 0},
			},

		},
	};
};

class cZombiePigmanLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_GOLD_NUGGET, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_GOLD, 0},
			},

		},
	};
};

class cZombieVillagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolList m_LootTablePools =
	{
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{{LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}}, {LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}}}, E_ITEM_ROTTEN_FLESH, 0},
			},

		},
		{
		1, {0, 0},  // Rolls
		0, {0, 0},  // Bonus Rolls
			{
				{{}, E_ITEM_IRON, 0},
				{{}, E_ITEM_CARROT, 0},
				{{}, E_ITEM_POTATO, 0},
			},

		},
	};
};
