
#pragma once

#include "Item.h"
#include <variant>
#include "json/json.h"
#include "Noise/Noise.h"

/*
This is a representation of the loot tables as described in the wiki:
https://minecraft.gamepedia.com/Loot_table
The json files are parsed every time on startup. Mostly the interpretation is done on loading.
The functions and conditions still contain some Json objects. It was the most effective way to store the parameters.
Notes:
	01.09.2020:
		Vanilla Minecraft uses a luck value per player that influences the outcome.
		This is not added to cuberite at the moment.
		I noted where you need to add some code (search for: "TODO: Luck")
	06.09.2020:
		When updating the loot tables you have to check the Tag interpretation. There might be new items added.
		There is a link to the list in the minecraft wiki in the LootTable class.
*/

// fwd:
class cWorld;
class cNoise;
class cBlockHandler;
class cBlockEntity;

/** This namespace contains all enum, structs, typedefs used in the loot table classes */
namespace LootTable
{
	/** Expected filenames of loot tables.
	This list is used for reference when looking for custom loot tables.
	The path to the folder is added later on. (global and per world). */
	const AString FileNames[] =
		{
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "AbandonedMineshaft.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "BuriedTreasure.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "DesertPyramid.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "EndCityTreasure.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "IglooChest.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "JungleTemple.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "JungleTempleDispenser.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "NetherBridge.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "PillagerOutpost.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "ShipwreckMap.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "ShipwreckSupply.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "ShipwreckTreasure.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "SimpleDungeon.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "SpawnBonusChest.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "StrongholdCorridor.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "StrongholdCrossing.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "StrongholdLibrary.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "UnderwaterRuinBig.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "UnderwaterRuinSmall.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageArmorer.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageButcher.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageCartographer.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageDesertHouse.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageFisher.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageFletcher.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageMason.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillagePlainsHouse.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageSavannaHouse.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageShepherd.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageSnowyHouse.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageTaigaHouse.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageTannery.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageTemple.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageToolsmith.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "VillageWeaponsmith.json",
			AString("LootTables") + cFile::PathSeparator() + "Chests" + cFile::PathSeparator() + "WoodlandMansion.json"
		};

	const AString PathToUserLootTables = AString("LootTables") + cFile::PathSeparator() + "User";

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




	/** Any available type of loot table Type as of 1.16.2 */
	enum class eType
	{
		Empty,                         // doesnt generate any Loot
		Entity,                        // Loot from any entity
		Block,                         // Loot from mined blocks
		Chest,                         // Loot from chests
		Fishing,                       // Loot from fishing
		Gift,                          // Loot gifted to the player from NPCs
		AdvancementReward,             // Rewards from advancements
		Barter,                        // Loot from "trading" with Piglins
		Command,                       // Customizable loot tables for the loot command
		Selector,                      // ?
		AdvancementEntity,             // ?
		Generic,                       // ?
		User                           // Non Vanilla type used for custom loot in generated structures
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
	/** Gets the eType from String. Defaults to Generic */
	extern enum eType eType(const AString & a_Type);
	/** Gets the eChestType from String as of 1.16.2 */
	extern enum eChestType eChestType(const AString & a_Type);
	/** Moves a_String from the "minecraft:" namespace to cuberite namespace */
	extern AString NamespaceConverter(AString a_String);
	/** Reads keys min and max and writes them into a_Min and a_Max
	if there are no appropriately named keys there is no change */
	extern void MinMaxRange(const Json::Value & a_Value, int & a_Min, int & a_Max);
	/** Reads keys min and max and returns a random integer value in between.
	a_Noise is used as the random generator and a_Pos is used as the seed */
	extern int MinMaxRand(const Json::Value & a_Value, const cNoise & a_Noise, const Vector3i & a_Pos, const int & a_Modifier);
}





struct cLootTableCondition;
typedef std::vector<cLootTableCondition> cLootTableConditionVector;
/** Represents a condition for a pool item */
struct cLootTableCondition
{
	cLootTableCondition()
	{
		m_Type = LootTable::eConditionType::None;
	}





	cLootTableCondition(
		enum LootTable::eConditionType a_Type,
		cLootTableConditionVector a_Parameter
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
	std::variant<cLootTableConditionVector, Json::Value> m_Parameter;
};




/** Represents a function for a pool item */
struct cLootTableFunction;
typedef std::vector<cLootTableFunction> cLootTableFunctionVector;
struct cLootTableFunction
{
	cLootTableFunction(
		enum LootTable::eFunctionType a_Type,
		Json::Value a_Parameter,
		cLootTableConditionVector a_Conditions
	):
		m_Type(a_Type),
		m_Parameter(std::move(a_Parameter)),
		m_Conditions(std::move(a_Conditions))
	{
	}

	enum LootTable::eFunctionType m_Type;
	Json::Value m_Parameter;
	cLootTableConditionVector m_Conditions;
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
		return a_Noise.IntNoise3DInt(a_Pos) % (m_RollsMax - m_RollsMin) + m_RollsMin;
	}
	// Note: The loot tables specify another value (type) this is usually "Uniform".
	// I think this is the probability distribution for the random value.
	// The wiki doesn't have any information in it. So this is left out for now
	int m_RollsMin;
	int m_RollsMax;
};




/** Represents a pool entry */
struct cLootTablePoolEntry;
typedef std::vector<cLootTablePoolEntry> cLootTablePoolEntryVector;
struct cLootTablePoolEntry
{
	cLootTablePoolEntry()
	{
		m_Type = LootTable::ePoolEntryType::Empty;
	}





	cLootTablePoolEntry(
		cLootTableConditionVector a_Conditions,
		cLootTableFunctionVector a_Functions,
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
		cLootTableConditionVector a_Conditions,
		cLootTableFunctionVector a_Functions,
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
		cLootTableConditionVector a_Conditions,
		cLootTableFunctionVector a_Functions,
		enum LootTable::ePoolEntryType a_Type,
		cLootTablePoolEntryVector a_Children,
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

	cLootTableConditionVector m_Conditions;
	cLootTableFunctionVector m_Functions;
	enum LootTable::ePoolEntryType m_Type;
	std::variant<cItem, AString, cLootTablePoolEntryVector> m_Content;
	bool m_Expand;
	int m_Weight;
	int m_Quality;
};




/** Represents a loot table pool */
struct cLootTablePool;
typedef std::vector<cLootTablePool> cLootTablePoolVector;
struct cLootTablePool
{
	cLootTablePool(
		cLootTablePoolRolls a_Rolls,
		cLootTablePoolEntryVector a_Entries,
		cLootTableConditionVector a_Conditions
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
		cLootTablePoolEntryVector a_Entries,
		cLootTableConditionVector a_Conditions
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
	cLootTablePoolEntryVector m_Entries;
	cLootTableConditionVector m_Conditions;
	int m_TotalWeight = 0;
	int m_TotalQuality = 0;
};





struct EnumClassHash
{
	template <typename T>
	std::size_t operator()(T t) const
	{
		return static_cast<std::size_t>(t);
	}
};




/** A individual loot table */
class cLootTable
{
public:
	/** Creates a empty loot table with type empty */
	cLootTable();

	/** Creates new loot table from string describing the loot table */
	cLootTable(const Json::Value & a_Description, cWorld * a_World);

	cLootTable(const cLootTable & a_Other) = default;
	cLootTable(cLootTable && a_Other) = default;
	cLootTable & operator = (const cLootTable & a_Other) = default;
	cLootTable & operator = (cLootTable && a_Other) = default;

	/** Fills the specified item grid. Needs world and pos for randomisation and access to player. */
	bool FillWithLoot(cItemGrid & a_ItemGrid, cWorld * a_World, const Vector3i & a_Pos, const UInt32 & a_Player) const;

	// Note: For any function killed describes the entity which triggered the event. And Killer the entity which killed the entity (if applicable)
	/** Returns cItems with all the items generated by the loot table */
	cItems GetItems(const cNoise & a_Noise, const Vector3i & a_Pos, cWorld * a_World, const UInt32 & a_Killed, const UInt32 & a_Killer) const;

protected:
	/** Type of loot table */
	enum LootTable::eType m_Type;

	/** Vector of loot pools */
	cLootTablePoolVector m_LootTablePools;

	/** Vector of functions applied to all pools */
	cLootTableFunctionVector m_Functions;

private:
	static cItems GetItems(const cLootTablePool & a_Pool, cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer);

	static cItems GetItems(const cLootTablePoolEntry & a_Entry, cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer);

	static bool ConditionsApply(const cLootTableConditionVector & a_Conditions, cWorld * a_World, const cNoise & a_Noise, const UInt32 & a_Killed, const UInt32 & a_Killer);

	static bool ConditionApplies(const cLootTableCondition & a_Condition, cWorld * a_World, const cNoise & a_Noise, const UInt32 & a_Killed, const UInt32 & a_Killer);

	/** Applies give function to item for all types */
	static void ApplyCommonFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer);

	/** Applies given function to an item in a Container */
	static void ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer);

	/** Applies given function to an item dropped from a block */
	static void ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const cBlockHandler & a_Block, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer);

	/** Applies given function to an item dropped from a block entity */
	static void ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const cBlockEntity & a_BlockEntity, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer);

	/** Applies given function to an item dropped from a killed entity */
	static void ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const Vector3i & a_Pos, const UInt32 & a_Killed, const cNoise & a_Noise, const UInt32 & a_Killer);
};

typedef std::unordered_map<enum LootTable::eChestType, cLootTable, EnumClassHash> cChestLootTableMap;
typedef std::unordered_map<AString, cLootTable> cUserLootTableMap;



