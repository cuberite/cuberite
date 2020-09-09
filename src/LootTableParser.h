
// LootTableParser.h

#pragma once

#include <variant>
#include "json/json.h"
#include "Noise/Noise.h"
#include "Item.h"

/** This namespace contains all enum, structs, typedefs used in the loot table classes */
namespace LootTable
	{
	/** A list of the names of the vanilla chest loot tables. */  // TODO: do we need this?
	const AString VanillaChestLootTableNames[] =
	{
		"AbandonedMineshaft",
		"BuriedTreasure",
		"DesertPyramid",
		"EndCityTreasure",
		"IglooChest",
		"JungleTemple",
		"JungleTempleDispenser",
		"NetherBridge",
		"PillagerOutpost",
		"ShipwreckMap",
		"ShipwreckSupply",
		"ShipwreckTreasure",
		"SimpleDungeon",
		"SpawnBonusChest",
		"StrongholdCorridor",
		"StrongholdCrossing",
		"StrongholdLibrary",
		"UnderwaterRuinBig",
		"UnderwaterRuinSmall",
		"VillageArmorer",
		"VillageButcher",
		"VillageCartographer",
		"VillageDesertHouse",
		"VillageFisher",
		"VillageFletcher",
		"VillageMason",
		"VillagePlainsHouse",
		"VillageSavannaHouse",
		"VillageShepherd",
		"VillageSnowyHouse",
		"VillageTaigaHouse",
		"VillageTannery",
		"VillageTemple",
		"VillageToolsmith",
		"VillageWeaponsmith",
		"WoodlandMansion"
	};

	const AString LootTablePath = "LootTables";

	/** Any available function type as of 1.16.2 */
	enum class eFunctionType
	{
		ApplyBonus,
		CopyName,
		CopyNbt,
		CopyState,
		EnchantRandomly,
		EnchantWithLevels,
		ExplorationMap,
		ExplosionDecay,
		FurnaceSmelt,
		FillPlayerHead,
		LimitCount,
		LootingEnchant,
		SetAttributes,
		SetContents,
		SetCount,
		SetDamage,
		SetLootTable,
		SetLore,
		SetName,
		SetNbt,
		SetStewEffect,
		None
	};




	/** Any available type of loot table conditions as of 1.16.2 */
	enum eConditionType
	{
		Alternative,  // Joins conditions from parameter terms with "or".
		/* terms: A list of conditions to join using 'or'. */

		BlockStateProperty,  // Check properties of a block state.
		/*  block: A block ID. The test fails if the block doesn't match.
			properties: (Optional) A map of block property names to values. All values are strings. The test fails if the block doesn't match. */

		DamageSourceProperties,  // Check properties of damage source.
		/* predicate: Predicate applied to the damage source. */

		EntityProperties,  // Test properties of an entity.
		/*  entity: Specifies the entity to check for the condition. Set to this to use the entity that died or the player that gained the advancement, opened the container or broke the block, killer for the killer, or killer_player for a killer that is a player.
			predicate: Predicate applied to entity, uses same structure as advancements. */

		EntityScores,  // Test the scoreboard scores of an entity.
		/* 	entity: Specifies the entity to check for the condition. Set to this to use the entity that died or the player that gained the advancement, opened the container or broke the block, killer for the killer, or killer_player for a killer that is a player.
			scores: Scores to check. All specified scores must pass for the condition to pass.
			A score: Key name is the objective while the value is the exact score value required for the condition to pass.
			A score: Key name is the objective while the value specifies a range of score values required for the condition to pass.
			min: Minimum score.
			max: Maximum score. */

		Inverted,  // Inverts condition from parameter term.
		/* term: The condition to be negated. */

		KilledByPlayer,  // Test if a killer_player entity is available.
		/* inverse: If true, the condition passes if killer_player is not available. */

		LocationCheck,  // Checks if the current location matches.
		/*  offsetX - optional offsets to location
			offsetY - optional offsets to location
			offsetZ - optional offsets to location
			predicate: Predicate applied to location, uses same structure as advancements. */

		MatchTool,  // Checks tool.
		/* predicate: Predicate applied to item, uses same structure as advancements. */

		RandomChance,  // Test if a random number 0.0 - 1.0 is less than a specified value.
		/* chance: Success rate as a number 0.0 - 1.0. */

		RandomChanceWithLooting,  // Test if a random number 0.0 - 1.0 is less than a specified value, affected by the level of Looting on the killer entity.
		/*  chance: Base success rate.
			looting_multiplier: Looting adjustment to the base success rate. Formula is chance + (looting_level * looting_multiplier). */

		Reference,  // Test if another referred condition (predicate) passes.
		/* name: The namespaced ID of the condition (predicate) referred to. A cyclic reference causes a parsing failure. */

		SurvivesExplosion,  // Returns true with 1 divided by explosion radius probability.

		TableBonus,  // Passes with probability picked from table, indexed by enchantment level.
		/*  enchantment: Id of enchantment.
			chances: List of probabilities for enchantment level, indexed from 0. */

		TimeCheck,  // Checks the current time
		/*  value: The time value in ticks.
			min: The minimum value.
			max: The maximum value.
			period: If present, time gets modulo-divided by this value (for example, if set to 24000, value operates on a time period of days). */

		WeatherCheck,  // Checks for a current weather state
		/*  raining: If true, the condition evaluates to true only if it's raining.
			thundering: If true, the condition evaluates to true only if it's thundering. */
		None  // Default rule. NOT VANILLA Used for the string comparison if string is unknown
	};





	/** Any available pool entry type as of 1.16.2 */
	enum class ePoolEntryType
	{
		Item,          // Plain item
		Tag,           // Descriptor for multiple items. E.g Music Discs, newer arrows...
		LootTable,     // Another loot table
		Group,         // Child entries
		Alternatives,  // Select one entry from the list
		Sequence,      // Select entries until one cannot be granted
		Dynamic,       // "generate block specific drops": drops the block itself as item
		Empty
	};




	/** Any available types of chest loots as of 1.16.2 */
	enum class eChestType
	{
		AbandonedMineshaft,
		BuriedTreasure,
		DesertPyramid,
		EndCityTreasure,
		IglooChest,
		JungleTemple,
		JungleTempleDispenser,
		NetherBridge,
		PillagerOutpost,
		ShipwreckMap,
		ShipwreckSupply,
		ShipwreckTreasure,
		SimpleDungeon,
		SpawnBonusChest,
		StrongholdCorridor,
		StrongholdCrossing,
		StrongholdLibrary,
		UnderwaterRuinBig,
		UnderwaterRuinSmall,

		/* Village chest types */
		VillageArmorer,
		VillageButcher,
		VillageCartographer,
		VillageDesertHouse,
		VillageFisher,
		VillageFletcher,
		VillageMason,
		VillagePlainsHouse,
		VillageSavannaHouse,
		VillageShepherd,
		VillageSnowyHouse,
		VillageTaigaHouse,
		VillageTannery,
		VillageTemple,
		VillageToolsmith,
		VillageWeaponsmith,

		WoodlandMansion,
		None
	};

	// Declaration of methods in the cpp
	/** Gets the eChestType from String as of 1.16.2 */
	enum eChestType eChestType(const AString & a_Type);
	/** Gets the eFunctionType from String. Defaults to None */
	enum eFunctionType eFunctionType(const AString & a_Type);
	/** Gets the eConditionType from String. Defaults to None */
	enum eConditionType eConditionType(const AString & a_Type);
	/** Gets the ePoolEntryType from String. Defaults to Empty */
	enum ePoolEntryType ePoolEntryType(const AString & a_Type);
	/** Moves a_String from the "minecraft:" namespace to cuberite namespace */
	AString NamespaceConverter(AString a_String);
	/** Reads keys min and max and writes them into a_Min and a_Max
	if there are no appropriately named keys there is no change */
	void MinMaxRange(const Json::Value & a_Value, int & a_Min, int & a_Max);
	/** Reads keys min and max and returns a random integer value in between.
	a_Noise is used as the random generator and a_Pos is used as the seed */
	int MinMaxRand(const Json::Value & a_Value, const cNoise & a_Noise, const Vector3i & a_Pos, const int & a_Modifier);
}





struct cLootTableCondition;
using cLootTableConditions = std::vector<cLootTableCondition>;
/** Represents a condition for a pool item */
struct cLootTableCondition
{
	cLootTableCondition()
	{
		m_Type = LootTable::eConditionType::None;
	}





	cLootTableCondition(
		enum LootTable::eConditionType a_Type,
		cLootTableConditions a_Parameter
	):
		m_Type(a_Type),
		m_Parameter(std::move(a_Parameter))
	{
	}





	cLootTableCondition(
		enum LootTable::eConditionType a_Type,
		Json::Value a_Parameter
	):
		m_Type(a_Type),
		m_Parameter(std::move(a_Parameter))
	{
	}

	enum LootTable::eConditionType m_Type;
	std::variant<cLootTableConditions, Json::Value> m_Parameter;
};




/** Represents a function for a pool item */
struct cLootTableFunction;
using cLootTableFunctions = std::vector<cLootTableFunction>;
struct cLootTableFunction
{
	cLootTableFunction(
		enum LootTable::eFunctionType a_Type,
		Json::Value a_Parameter,
		cLootTableConditions a_Conditions
	):
		m_Type(a_Type),
		m_Parameter(std::move(a_Parameter)),
		m_Conditions(std::move(a_Conditions))
	{
	}

	enum LootTable::eFunctionType m_Type;
	Json::Value m_Parameter;
	cLootTableConditions m_Conditions;
};




/** Stores the rolls of a pool */
struct cLootTablePoolRolls
{
	cLootTablePoolRolls()
	{
		m_RollsMin = 0;
		m_RollsMax = 0;
	}




	/** Pool rolls. If min = max -> steady roll count */
	cLootTablePoolRolls(
		int a_RollsMin,
		int a_RollsMax
	):
		m_RollsMin(a_RollsMin),
		m_RollsMax(a_RollsMax)
	{
	}




	/** Returns the rolls result */
	int operator()(const cNoise & a_Noise, const Vector3i & a_Pos) const
	{
		if (m_RollsMax == m_RollsMin)
		{
			return m_RollsMin;
		}
		return (a_Noise.IntNoise3DInt(a_Pos) / 7) % (m_RollsMax - m_RollsMin) + m_RollsMin;
	}
	// Note: The loot tables specify another value (type) this is usually "Uniform".
	// I think this is the probability distribution for the random value.
	// The wiki doesn't have any information in it. So this is left out for now
	int m_RollsMin;
	int m_RollsMax;
};




/** Represents a pool entry */
struct cLootTablePoolEntry;
using cLootTablePoolEntries = std::vector<cLootTablePoolEntry>;
struct cLootTablePoolEntry
{
	cLootTablePoolEntry()
	{
		m_Type = LootTable::ePoolEntryType::Empty;
	}





	cLootTablePoolEntry(
		cLootTableConditions a_Conditions,
		cLootTableFunctions a_Functions,
		enum LootTable::ePoolEntryType a_Type,
		cItem a_Item,
		int a_Weight,
		int a_Quality
	):
		m_Conditions(std::move(a_Conditions)),
		m_Functions(std::move(a_Functions)),
		m_Type(a_Type),
		m_Content(std::move(a_Item)),
		m_Weight(a_Weight),
		m_Quality(a_Quality)
	{
	}





	cLootTablePoolEntry(
		cLootTableConditions a_Conditions,
		cLootTableFunctions a_Functions,
		enum LootTable::ePoolEntryType a_Type,
		AString a_Name,
		bool a_Expand,
		int a_Weight,
		int a_Quality
	):
		m_Conditions(std::move(a_Conditions)),
		m_Functions(std::move(a_Functions)),
		m_Type(a_Type),
		m_Content(std::move(a_Name)),
		m_Expand(a_Expand),
		m_Weight(a_Weight),
		m_Quality(a_Quality)
	{
	}





	cLootTablePoolEntry(
		cLootTableConditions a_Conditions,
		cLootTableFunctions a_Functions,
		enum LootTable::ePoolEntryType a_Type,
		cLootTablePoolEntries a_Children,
		int a_Weight,
		int a_Quality
	):
		m_Conditions(std::move(a_Conditions)),
		m_Functions(std::move(a_Functions)),
		m_Type(a_Type),
		m_Content(std::move(a_Children)),
		m_Weight(a_Weight),
		m_Quality(a_Quality)
	{
	}

	cLootTableConditions m_Conditions;
	cLootTableFunctions m_Functions;
	enum LootTable::ePoolEntryType m_Type;
	std::variant<cItem, AString, cLootTablePoolEntries> m_Content;
	bool m_Expand;
	int m_Weight;
	int m_Quality;
};




/** Represents a loot table pool */
struct cLootTablePool;
using cLootTablePools = std::vector<cLootTablePool>;
struct cLootTablePool
{
	cLootTablePool(
		cLootTablePoolRolls a_Rolls,
		cLootTablePoolEntries a_Entries,
		cLootTableConditions a_Conditions
	):
		m_Rolls(a_Rolls),
		m_Entries(std::move(a_Entries)),
		m_Conditions(std::move(a_Conditions))
	{
		for (const auto & Entry : m_Entries)
		{
			m_TotalWeight += Entry.m_Weight;
			m_TotalQuality += Entry.m_Quality;
		}
	}





	cLootTablePool(
		cLootTablePoolRolls a_Rolls,
		cLootTablePoolRolls a_BonusRolls,
		cLootTablePoolEntries a_Entries,
		cLootTableConditions a_Conditions
	):
		m_Rolls(a_Rolls),
		m_BonusRolls(a_BonusRolls),
		m_Entries(std::move(a_Entries)),
		m_Conditions(std::move(a_Conditions))
	{
		for (const auto & Entry : m_Entries)
		{
			m_TotalWeight += Entry.m_Weight;
			m_TotalQuality += Entry.m_Weight;
		}
	}

	cLootTablePoolRolls m_Rolls;
	cLootTablePoolRolls m_BonusRolls;
	cLootTablePoolEntries m_Entries;
	cLootTableConditions m_Conditions;
	int m_TotalWeight = 0;
	int m_TotalQuality = 0;
};





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
