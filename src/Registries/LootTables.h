
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
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE), 20),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE, 0x01), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_NAME_TAG), 30),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_BOOK), 10
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_PICKAXE), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_PICKAXE), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 4),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "9"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_REDSTONE_DUST), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "9"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_DYE, 1, 4), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_DIAMOND), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "3"}}, {})
								},
							cItem(E_ITEM_COAL), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BREAD), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_MELON_SEEDS), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_PUMPKIN_SEEDS), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_BEETROOT_SEEDS), 10
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(3, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_BLOCK_RAIL), 20
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_POWERED_RAIL), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_DETECTOR_RAIL), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_ACTIVATOR_RAIL), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "16"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_TORCH), 15
						),
					}
			),
		};
};

class cBuriedTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_HEART_OF_THE_SEA), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 5, 8),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 20
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_TNT), 5
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 3),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_EMERALD), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_DIAMOND), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_PRISMARINE_CRYSTALS), 5
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 0, 1),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_LEATHER_TUNIC), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_SWORD), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(2, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_COOKED_FISH), 0
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_COOKED_FISH, 1, 1), 0
						),
					}
			),
		};
};

class cDesertPyramidLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 4),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_DIAMOND), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_GOLD), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_EMERALD), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_BONE), 25
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_SPIDER_EYE), 25
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "3"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 25
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_SADDLE), 20),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_HORSE_ARMOR), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_HORSE_ARMOR), 10),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND_HORSE_ARMOR), 5),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_BOOK), 20
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE), 20),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE, 0x01), 2),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE, 0x01), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(4, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BONE), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GUNPOWDER), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_STRING), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_SAND), 10
						),
					}
			),
		};
};

class cEndCityTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 6),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_DIAMOND), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_IRON), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_GOLD), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_EMERALD), 2
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BEETROOT_SEEDS), 5
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_SADDLE), 3),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_HORSE_ARMOR), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_HORSE_ARMOR), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND_HORSE_ARMOR), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_DIAMOND_SWORD), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_DIAMOND_BOOTS), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_DIAMOND_CHESTPLATE), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_DIAMOND_LEGGINGS), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_DIAMOND_HELMET), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_DIAMOND_PICKAXE), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_DIAMOND_SHOVEL), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_IRON_SWORD), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_IRON_BOOTS), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_IRON_CHESTPLATE), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_IRON_LEGGINGS), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_IRON_HELMET), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_IRON_PICKAXE), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"max", "39"}, {"type", "Uniform"}, {"min", "20"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_IRON_SHOVEL), 3
						),
					}
			),
		};
};

class cIglooChestLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 8),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_RED_APPLE), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_COAL), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD_NUGGET), 10
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_STONE_AXE), 2),
						cLootTablePoolEntry({}, cItem(E_ITEM_ROTTEN_FLESH), 10),
						cLootTablePoolEntry({}, cItem(E_ITEM_EMERALD), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_WHEAT), 10
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE), 0),
					}
			),
		};
};

class cJungleTempleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 6),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_DIAMOND), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_GOLD), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BAMBOO), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_EMERALD), 2
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_BONE), 20
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "3"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 16
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_SADDLE), 3),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_HORSE_ARMOR), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_HORSE_ARMOR), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND_HORSE_ARMOR), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"levels", "30"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_BOOK), 0
						),
					}
			),
		};
};

class cJungleTempleDispenserLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 2),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_ARROW), 30
						),
					}
			),
		};
};

class cNetherBridgeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 4),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_DIAMOND), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD), 15
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_SWORD), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_CHESTPLATE), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_FLINT_AND_STEEL), 5),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "3"}}, {})
								},
							cItem(E_ITEM_NETHER_WART), 5
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_SADDLE), 10),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_HORSE_ARMOR), 8),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_HORSE_ARMOR), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND_HORSE_ARMOR), 3),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_BLOCK_OBSIDIAN), 2
						),
					}
			),
		};
};

class cPillagerOutpostLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 0, 1),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_CROSSBOW), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 3),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "3"}}, {})
								},
							cItem(E_ITEM_WHEAT), 7
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_POTATO), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "3"}}, {})
								},
							cItem(E_ITEM_CARROT), 5
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 3),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_BLOCK_NEW_LOG, 1, 1), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 3),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_BOTTLE_O_ENCHANTING), 7),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_STRING), 4
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_ARROW), 4
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_TRIPWIRE_HOOK), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_BOOK), 0
						),
					}
			),
		};
};

class cShipwreckMapLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::ExplorationMap, {{"decoration", "RedX"}, {"skip_existing_chunks", "False"}, {"destination", "BuriedTreasure"}, {"zoom", "1"}}, {})
								},
							cItem(E_ITEM_MAP), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(3, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_COMPASS), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_MAP), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_CLOCK), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_PAPER), 20
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_FEATHER), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BOOK), 5
						),
					}
			),
		};
};

class cShipwreckSupplyLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 3, 10),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "12"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_PAPER), 8
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_POTATO), 7
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "6"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_POISONOUS_POTATO), 7
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_CARROT), 7
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "21"}, {"type", "Uniform"}, {"min", "8"}}, {})
								},
							cItem(E_ITEM_WHEAT), 7
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetStewEffect, {}, {})
								},
							cItem(E_ITEM_SUSPICIOUS_STEW), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_COAL), 6
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "24"}, {"type", "Uniform"}, {"min", "5"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_PUMPKIN), 2
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BAMBOO), 2
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GUNPOWDER), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_TNT), 0
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_LEATHER_CAP), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_LEATHER_TUNIC), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_LEATHER_PANTS), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_LEATHER_BOOTS), 3
						),
					}
			),
		};
};

class cShipwreckTreasureLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 3, 6),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 90
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_EMERALD), 40
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_BOTTLE_O_ENCHANTING), 5),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 5),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON_NUGGET), 50
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD_NUGGET), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "10"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_DYE, 1, 4), 20
						),
					}
			),
		};
};

class cSimpleDungeonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 3),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_SADDLE), 20),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE, 0x01), 2),
						cLootTablePoolEntry({}, cItem(E_ITEM_13_DISC), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_CAT_DISC), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_NAME_TAG), 20),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_HORSE_ARMOR), 10),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_HORSE_ARMOR), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND_HORSE_ARMOR), 5),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_BOOK), 10
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 4),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD), 5
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_BREAD), 20),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_WHEAT), 20
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_BUCKET), 10),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_REDSTONE_DUST), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_COAL), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_MELON_SEEDS), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_PUMPKIN_SEEDS), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_BEETROOT_SEEDS), 10
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(3, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BONE), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GUNPOWDER), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_STRING), 10
						),
					}
			),
		};
};

class cSpawnBonusChestLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_STONE_AXE), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_WOODEN_AXE), 3),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_STONE_PICKAXE), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_WOODEN_PICKAXE), 3),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(3, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_RED_APPLE), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BREAD), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_RAW_FISH, 1, 1), 3
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(4, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "12"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_STICK), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "12"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_PLANKS), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_LOG), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_LOG, 1, 1), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_LOG, 1, 2), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_LOG, 1, 3), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_NEW_LOG), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_BLOCK_NEW_LOG, 1, 1), 3
						),
					}
			),
		};
};

class cStrongholdCorridorLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 3),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_ENDER_PEARL), 10),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_DIAMOND), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "9"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_REDSTONE_DUST), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BREAD), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_RED_APPLE), 15
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_PICKAXE), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_SWORD), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_CHESTPLATE), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_HELMET), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_LEGGINGS), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_BOOTS), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_SADDLE), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_HORSE_ARMOR), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_HORSE_ARMOR), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND_HORSE_ARMOR), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"levels", "30"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_BOOK), 0
						),
					}
			),
		};
};

class cStrongholdCrossingLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 4),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "9"}, {"type", "Uniform"}, {"min", "4"}}, {})
								},
							cItem(E_ITEM_REDSTONE_DUST), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "3"}}, {})
								},
							cItem(E_ITEM_COAL), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BREAD), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_RED_APPLE), 15
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON_PICKAXE), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"levels", "30"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_BOOK), 0
						),
					}
			),
		};
};

class cStrongholdLibraryLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 10),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BOOK), 20
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "7"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_PAPER), 20
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_MAP), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_COMPASS), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantWithLevels, {{"levels", "30"}, {"treasure", "True"}}, {})
								},
							cItem(E_ITEM_BOOK), 10
						),
					}
			),
		};
};

class cUnderwaterRuinBigLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 8),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_COAL), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD_NUGGET), 10
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_EMERALD), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_WHEAT), 10
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_BOOK), 5
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_LEATHER_TUNIC), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_HELMET), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_FISHING_ROD), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::ExplorationMap, {{"decoration", "RedX"}, {"skip_existing_chunks", "False"}, {"destination", "BuriedTreasure"}, {"zoom", "1"}}, {})
								},
							cItem(E_ITEM_MAP), 10
						),
					}
			),
		};
};

class cUnderwaterRuinSmallLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 2, 8),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_COAL), 10
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_STONE_AXE), 2),
						cLootTablePoolEntry({}, cItem(E_ITEM_ROTTEN_FLESH), 5),
						cLootTablePoolEntry({}, cItem(E_ITEM_EMERALD), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_WHEAT), 10
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_LEATHER_TUNIC), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD_HELMET), 0),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_FISHING_ROD), 5
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::ExplorationMap, {{"decoration", "RedX"}, {"skip_existing_chunks", "False"}, {"destination", "BuriedTreasure"}, {"zoom", "1"}}, {})
								},
							cItem(E_ITEM_MAP), 5
						),
					}
			),
		};
};

class cWoodlandMansionLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 3),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_LEAD), 20),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLDEN_APPLE, 0x01), 2),
						cLootTablePoolEntry({}, cItem(E_ITEM_13_DISC), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_CAT_DISC), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_NAME_TAG), 20),
						cLootTablePoolEntry({}, cItem(E_ITEM_CHAIN_CHESTPLATE), 10),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND_HOE), 15),
						cLootTablePoolEntry({}, cItem(E_ITEM_DIAMOND_CHESTPLATE), 5),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::EnchantRandomly, {}, {})
								},
							cItem(E_ITEM_BOOK), 10
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 4),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_IRON), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GOLD), 5
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_BREAD), 20),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_WHEAT), 20
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_BUCKET), 10),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_REDSTONE_DUST), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_COAL), 15
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_MELON_SEEDS), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_PUMPKIN_SEEDS), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "4"}, {"type", "Uniform"}, {"min", "2"}}, {})
								},
							cItem(E_ITEM_BEETROOT_SEEDS), 10
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(3, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_BONE), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_GUNPOWDER), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 10
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "8"}, {"type", "Uniform"}, {"min", "1"}}, {})
								},
							cItem(E_ITEM_STRING), 10
						),
					}
			),
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
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_BLAZE_ROD), 0
						),
					}
			),
		};
};

class cCatLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {})
								},
							cItem(E_ITEM_STRING), 0
						),
					}
			),
		};
};

class cCaveSpiderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_STRING), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "-1"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_SPIDER_EYE), 0
						),
					}
			),
		};
};

class cChickenLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_FEATHER), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RAW_CHICKEN), 0
						),
					}
			),
		};
};

class cCodLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}})
								},
							cItem(E_ITEM_RAW_FISH), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_DYE, 1, 15), 0),
					}
			),
		};
};

class cCowLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_LEATHER), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RAW_BEEF), 0
						),
					}
			),
		};
};

class cCreeperLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_GUNPOWDER), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_MUSIC_DISCS), 0),
					}
			),
		};
};

class cDolphinLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}})
								},
							cItem(E_ITEM_RAW_FISH), 0
						),
					}
			),
		};
};

class cDonkeyLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_LEATHER), 0
						),
					}
			),
		};
};

class cDrownedLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD), 0),
					}
			),
		};
};

class cElderGuardianLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_PRISMARINE_SHARD), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}})
								},
							cItem(E_ITEM_RAW_FISH), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_PRISMARINE_CRYSTALS), 2
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_PRISMARINE_CRYSTALS), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_BLOCK_SPONGE, 1, 1), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_GAMEPLAY/FISHING/FISH), 0),
					}
			),
		};
};

class cEndermanLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ENDER_PEARL), 0
						),
					}
			),
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
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_TOTEM_OF_UNDYING), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_EMERALD), 0
						),
					}
			),
		};
};

class cFoxLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cGhastLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_GHAST_TEAR), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_GUNPOWDER), 0
						),
					}
			),
		};
};

class cGiantLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cGuardianLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_PRISMARINE_SHARD), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}})
								},
							cItem(E_ITEM_RAW_FISH), 2
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_PRISMARINE_CRYSTALS), 2
						),
						cLootTablePoolEntry({}, cItem(E_ITEM_PRISMARINE_CRYSTALS), 0),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_GAMEPLAY/FISHING/FISH), 0),
					}
			),
		};
};

class cHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_LEATHER), 0
						),
					}
			),
		};
};

class cHuskLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_CARROT), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_POTATO), 0),
					}
			),
		};
};

class cIllusionerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cIronGolemLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {})
								},
							cItem(E_BLOCK_FLOWER, 1, 0), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "5"}, {"type", "Uniform"}, {"min", "3"}}, {})
								},
							cItem(E_ITEM_IRON), 0
						),
					}
			),
		};
};

class cLlamaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_LEATHER), 0
						),
					}
			),
		};
};

class cMagmaCubeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "-2"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_MAGMA_CREAM), 0
						),
					}
			),
		};
};

class cMooshroomLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_LEATHER), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RAW_BEEF), 0
						),
					}
			),
		};
};

class cMuleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_LEATHER), 0
						),
					}
			),
		};
};

class cOcelotLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cPandaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"count", "1"}}, {})
								},
							cItem(E_ITEM_BAMBOO), 0
						),
					}
			),
		};
};

class cParrotLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_FEATHER), 0
						),
					}
			),
		};
};

class cPhantomLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_PHANTOM_MEMBRANE), 0
						),
					}
			),
		};
};

class cPigLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RAW_PORKCHOP), 0
						),
					}
			),
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
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RAW_FISH), 3
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RAW_FISH, 1, 1), 0
						),
					}
			),
		};
};

class cPufferfishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"count", "1"}}, {})
								},
							cItem(E_ITEM_RAW_FISH, 1, 3), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_DYE, 1, 15), 0),
					}
			),
		};
};

class cRabbitLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RABBIT_HIDE), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RAW_RABBIT), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_RAW_RABBIT_FOOT), 0),
					}
			),
		};
};

class cRavagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"count", "1"}}, {})
								},
							cItem(E_ITEM_SADDLE), 0
						),
					}
			),
		};
};

class cSalmonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}})
								},
							cItem(E_ITEM_RAW_FISH, 1, 1), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_DYE, 1, 15), 0),
					}
			),
		};
};

class cSheepLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::FurnaceSmelt, {}, {{"entity", "This"}, {"is_on_fire", "true"}, {"condition", "EntityProperties"}}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_RAW_MUTTON), 0
						),
					}
			),
		};
};

class cShulkerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_SHULKER_SHELL), 0),
					}
			),
		};
};

class cSilverfishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cSkeletonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ARROW), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_BONE), 0
						),
					}
			),
		};
};

class cSkeletonHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_BONE), 0
						),
					}
			),
		};
};

class cSlimeLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_SLIMEBALL), 0
						),
					}
			),
		};
};

class cSnowGolemLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "15"}, {"type", "Uniform"}, {"min", "0"}}, {})
								},
							cItem(E_ITEM_SNOWBALL), 0
						),
					}
			),
		};
};

class cSpiderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_STRING), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "-1"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_SPIDER_EYE), 0
						),
					}
			),
		};
};

class cSquidLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "3"}, {"type", "Uniform"}, {"min", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_DYE), 0
						),
					}
			),
		};
};

class cStrayLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ARROW), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_BONE), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}, {"limit", "1"}}, {}), cLootTableFunction(LootTable::eFunctionType::SetNbt, {{"tag", "{Potion:\"Slowness\"}"}}, {})
								},
							cItem(E_ITEM_TIPPED_ARROW), 0
						),
					}
			),
		};
};

class cTraderLlamaLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_LEATHER), 0
						),
					}
			),
		};
};

class cTropicalFishLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"count", "1"}}, {})
								},
							cItem(E_ITEM_RAW_FISH), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_DYE, 1, 15), 0),
					}
			),
		};
};

class cTurtleLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_SEAGRASS), 3
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_BOWL), 0),
					}
			),
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
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_EMERALD), 0
						),
					}
			),
		};
};

class cWanderingTraderLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cWitchLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(-1, 1, 3),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_GLOWSTONE_DUST), 0
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_SUGAR), 0
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_REDSTONE_DUST), 0
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_SPIDER_EYE), 0
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_GLASS_BOTTLE), 0
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_GUNPOWDER), 0
						),
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_STICK), 2
						),
					}
			),
		};
};

class cWitherLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cWitherSkeletonLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "-1"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_COAL), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_BONE), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_HEAD, 1, 1), 0),
					}
			),
		};
};

class cWolfLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};

class cZombieLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_CARROT), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_POTATO), 0),
					}
			),
		};
};

class cZombieHorseLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 0
						),
					}
			),
		};
};

class cZombiePigmanLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "1"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_GOLD_NUGGET), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_GOLD), 0),
					}
			),
		};
};

class cZombieVillagerLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Entity;
	cLootTablePoolVector m_LootTablePools =
		{
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry(
							cLootTableFunctionVector
								{
									cLootTableFunction(LootTable::eFunctionType::SetCount, {{"max", "2"}, {"type", "Uniform"}, {"min", "0"}}, {}), cLootTableFunction(LootTable::eFunctionType::LootingEnchant, {{"min", "0"}, {"max", "1"}}, {})
								},
							cItem(E_ITEM_ROTTEN_FLESH), 0
						),
					}
			),
			cLootTablePool(
				cLootTablePoolRolls(1, 0, 0),
				cLootTablePoolEntryVector
					{
						cLootTablePoolEntry({}, cItem(E_ITEM_IRON), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_CARROT), 0),
						cLootTablePoolEntry({}, cItem(E_ITEM_POTATO), 0),
					}
			),
		};
};
