
// cLootTableProvider.h

#pragma once

#include <utility>

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "Mobs/MonsterTypes.h"
#include "Item.h"

// fwd:
class cBlockEntityWithItems;
class cItem;
class cWorld;

/*
This file contains all classes, types, ... used in the loot table functions.
The default loot tables are from LootTables/ in the root folder
The custom loot tables are read per world and must be contained in %worldname%/LootTables
They follow the vanilla file structure so any possible entry should be respected

Notes:
	30.08.2020:
		At the moment only loot tables for item container are supported.
		There are some functions commented fo the lookup uncomment them and add the code
*/

/** This namespace contains all enum, structs, typedefs used in the loot table classes */
namespace LootTable
{
	/** Expected filenames of loot tables.
	This list is used for reference when looking for custom loot tables.
	The path to the folder is added later on. (global and per world). */
	const AString FileNames[] =
	{
		"Chests%cAbandonedMineshaft.json",
		"Chests%cBuriedTreasure.json",
		"Chests%cDesertPyramid.json",
		"Chests%cEndCityTreasure.json",
		"Chests%cIglooChest.json",
		"Chests%cJungleTemple.json",
		"Chests%cJungleTempleDispenser.json",
		"Chests%cNetherBridge.json",
		"Chests%cPillagerOutpost.json",
		"Chests%cShipwreckMap.json",
		"Chests%cShipwreckSupply.json",
		"Chests%cShipwreckTreasure.json",
		"Chests%cSimpleDungeon.json",
		"Chests%cSpawnBonusChest.json",
		"Chests%cStrongholdCorridor.json",
		"Chests%cStrongholdCrossing.json",
		"Chests%cStrongholdLibrary.json",
		"Chests%cUnderwaterRuinBig.json",
		"Chests%cUnderwaterRuinSmall.json",
		"Chests%cVillageArmorer.json",
		"Chests%cVillageButcher.json",
		"Chests%cVillageCartographer.json",
		"Chests%cVillageDesertHouse.json",
		"Chests%cVillageFisher.json",
		"Chests%cVillageFletcher.json",
		"Chests%cVillageMason.json",
		"Chests%cVillagePlainsHouse.json",
		"Chests%cVillageSavannaHouse.json",
		"Chests%cVillageShepherd.json",
		"Chests%cVillageSnowyHouse.json",
		"Chests%cVillageTaigaHouse.json",
		"Chests%cVillageTannery.json",
		"Chests%cVillageTemple.json",
		"Chests%cVillageToolsmith.json",
		"Chests%cVillageWeaponsmith.json",
		"Chests%cWoodlandMansion.json"
	};




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
		Generic                        // ?
	};




	/** Any available type of loot table conditions as of 1.16.2 */
	enum class eConditionType
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




	/**  */
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
}





/** Represents a condition for a pool item */
typedef struct cLootTableCondition
{
	LootTable::eConditionType m_Type;
	AStringMap m_Parameter;
} cLootTableCondition;

typedef std::vector<cLootTableCondition> cLootTableConditionVector;




/** Represents a function for a pool item */
typedef struct cLootTableFunction
{
	cLootTableFunction(
		LootTable::eFunctionType a_Type,
		AStringMap a_Parameter,
		cLootTableConditionVector a_Conditions
	):
		m_Parameter(std::move(a_Parameter)),
		m_Conditions(std::move(a_Conditions)),
		m_Type(a_Type)
	{
	}
	LootTable::eFunctionType m_Type;
	AStringMap m_Parameter;
	cLootTableConditionVector m_Conditions;
} cLootTableFunction;

typedef std::vector<cLootTableFunction> cLootTableFunctionVector;




/** Stores the rolls of a pool */
typedef struct cLootTablePoolRolls
{
	cLootTablePoolRolls()
	{
		m_Roll = 0;
		m_RollsMin = 0;
		m_RollsMax = 0;
		m_BonusRoll = 0;
		m_BonusRollsMin = 0;
		m_BonusRollsMax = 0;
	}




	/** Pool rolls with optional bonus roll parameter - steady roll needs to be -1 to activate roll range */
	cLootTablePoolRolls(
		int a_Roll,
		int a_RollsMin,
		int a_RollsMax,
		int a_BonusRoll = 0,
		int a_BonusRollsMin = 0,
		int a_BonusRollsMax = 0
	):
		m_Roll(a_Roll),
		m_RollsMin(a_RollsMin),
		m_RollsMax(a_RollsMax),
		m_BonusRoll(a_BonusRoll),
		m_BonusRollsMin(a_BonusRollsMin),
		m_BonusRollsMax(a_BonusRollsMax)
	{
	}

	int m_Roll;
	int m_RollsMin;
	int m_RollsMax;
	int m_BonusRoll;
	int m_BonusRollsMin;
	int m_BonusRollsMax;
	// Note: The loot tables specify another value (type) this is usually "Uniform".
	// I think this is the probability distribution for the random value.
	// The wiki doesn't have any information in it. So this is left out for now
} cLootTablePoolRolls;



/** Represents a pool entry */
typedef struct cLootTablePoolEntry
{
	cLootTablePoolEntry(
		cLootTableFunctionVector a_Functions,
		cItem a_Item,
		int a_Weight,
		cLootTableConditionVector a_Conditions = cLootTableConditionVector()
	):
		m_Functions(std::move(a_Functions)),
		m_Item(std::move(a_Item)),
		m_Weight(a_Weight),
		m_Conditions(std::move(a_Conditions))
	{
	}





	cLootTablePoolEntry(
		cLootTableFunctionVector a_Functions,
		AString a_Children,
		int a_Weight,
		cLootTableConditionVector a_Conditions
	):
		m_Functions(std::move(a_Functions)),
		m_Children(std::move(a_Children)),
		m_Weight(a_Weight),
		m_Conditions(std::move(a_Conditions))
	{
	}

	cLootTableConditionVector m_Conditions;
	cLootTableFunctionVector m_Functions;
	// Todo: Add type
	cItem m_Item;
	AString m_Children;  // Todo: think about datatype
	// Todo: Add expand - what ever that does
	int m_Weight;
	// Todo: Add quality
} cLootTablePoolEntry;

typedef std::vector<cLootTablePoolEntry> cLootTablePoolEntryVector;


/** Represents a pool in a loot table */
typedef struct cLootTablePool
{
	/** create a pool with steady roll count */
	cLootTablePool(
		cLootTablePoolRolls a_Rolls,
		cLootTablePoolEntryVector a_Entries,
		cLootTableConditionVector a_Conditions = cLootTableConditionVector()
		):
		m_Rolls(a_Rolls),
		m_Entries(std::move(a_Entries)),
		m_Conditions(std::move(a_Conditions))
	{
	}

	cLootTablePoolRolls m_Rolls;
	cLootTablePoolEntryVector m_Entries;
	cLootTableConditionVector m_Conditions;
} cLootTablePool;

typedef std::vector<cLootTablePool> cLootTablePoolVector;





/** A individual loot table */
class cLootTable
{
public:
	/** Creates a empty loot table with type empty */
	cLootTable();

	/** Creates new loot table from string describing the loot table */
	cLootTable(const Json::Value & a_Description);

	cLootTable(cLootTable & a_Other);

	/** Fills the specified block entity at the position with loot and returns the success */
	bool FillWithLoot(cBlockEntityWithItems & a_BlockEntity) const;

protected:
	/** Type of loot table */
	enum LootTable::eType m_Type;

	/** Vector of loot pools */
	cLootTablePoolVector m_LootTablePools;

	/** Vector of functions applied to all pools */
	cLootTableFunctionVector m_LootTableFunctions;

private:
	/** Reads Loot Table pool from Json */
	void ReadLootTablePool(const Json::Value & a_Value);

	/** Reads a loot table function from Json */
	cLootTableFunction ReadLootTableFunction(const Json::Value & a_Value) const;

	/** Reads a loot table condition from Json */
	cLootTableCondition ReadLootTableCondition(const Json::Value & a_Value) const;

	/** Reads a loot table pool entry from Json */
	cLootTablePoolEntry ReadLootTablePoolEntry(const Json::Value & a_Value) const;
};

// typedef std::map<enum eMonsterType,          std::shared_ptr<cLootTable>> cMonsterLootTableMap;
typedef std::map<enum LootTable::eChestType, cLootTable> cChestLootTableMap;




/** The LootTableProvider is used per world to ask for loot tables */
class cLootTableProvider
{
public:
	cLootTableProvider(AString & a_Path);

	/** Function to load a loot table from specified path */
	void LoadLootTable(const AString & a_String, AString & a_Type);

	/** Functions to load loot tables. Custom loot tables are also checked */
	/*
	Further information on the string based function:
	Format:    Type|FurtherInfo
	example:   Chest|AbandonedMineshaft
	example:   Monster|Skeleton
	example:   Block|Stone
	This is not case sensitive, and removes all spaces */
	const cLootTable * GetLootTable(const AString & a_Name) const;
	const cLootTable * GetLootTable(const enum LootTable::eChestType a_Type) const;
	// std::shared_ptr<cLootTable> GetLootTable(const eMonsterType a_Type) const;

	/** Drops the loot of a_Monster specified by the loot tables */
	// void DropLoot(const cMonster & a_Monster) const;  // Todo: move to unique id of monster

private:

	/** Maps containing custom loot tables */
	cChestLootTableMap m_ChestLootTables = cChestLootTableMap();
	// cMonsterLootTableMap m_CustomMonsterLootTables = cMonsterLootTableMap();^

	/** Empty loot table in case there is an error */
	static const cLootTable m_EmptyLootTable;
};




