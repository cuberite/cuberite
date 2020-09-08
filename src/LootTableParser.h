
// LootTableParser.h

#pragma once

namespace LootTable
{
	/** Reads Loot Table pool from Json */
	extern cLootTablePool ParsePool(const Json::Value & a_Value);

	/** Reads a loot table function from Json */
	extern cLootTableFunction ParseFunction(const Json::Value & a_Value);

	/** Reads a loot table condition from Json */
	extern cLootTableCondition ParseCondition(const Json::Value & a_Value);

	/** Reads a loot table pool entry from Json */
	extern cLootTablePoolEntry ParsePoolEntry(const Json::Value & a_Value);

	/** Reads a loot table pool roll from Json */
	extern cLootTablePoolRolls ParseRolls(const Json::Value & a_Value);
}
