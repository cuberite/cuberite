
// cLootTableProvider.h

#pragma once
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BlockType.h"
#include "Mobs/MonsterTypes.h"


/** Any available function type as of 1.16.2 */
enum class cLootTableFunctionType
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




enum class LootTableType
{
	Empty,
	Block,
	Chest,
	Fishing,
	Gift,
	AdvancementReward,
	Barter,
	Command,
	Selector,
	AdvancementEntity,
	Generic
};




/** Represents a function for a pool item */
typedef struct LootTableFunction
{
	cLootTableFunctionType m_Type;
	AStringVector m_Parameter;
} cLootTableFunction;

typedef std::vector<cLootTableFunction> cLootTableFunctionVector;




/** Represents a pool entry */
typedef struct cLootTablePoolEntry
{
	// Todo: Conditions
	cLootTableFunctionVector m_Functions;
	// Todo: Add type and think about what is necessary
	ENUM_ITEM_TYPE m_Item;
	// Todo: Children
	// Todo: Add expand
	int m_Weight;
	// Todo: Add quality
} cLootTablePoolEntry;

typedef std::vector<cLootTablePoolEntry> cLootTablePoolEntryVector;


/** Represents a pool in a loot table */
typedef struct cLootTablePool
{
	int m_Roll;
	int m_Rolls[2];
	int m_BonusRoll;
	int m_BonusRolls[2];
	cLootTablePoolEntryVector m_Entries;
	// Todo: Conditions
} cLootTablePool;

typedef std::vector<cLootTablePool> cLootTablePoolVector;


/** A individual loot table */
class cLootTable
{
  public:

	cLootTable();

	/** Creates new loot table from string describing the loot table */
	cLootTable(AString a_Description);

	/** Reads the string and generates a filled loot table from that and returns the success */
	bool ReadFromString(AString a_Description);

	/** Fills the specified block entity at the position with loot and returns the success */
	bool FillWithLoot(Vector3i a_Pos);

protected:
	LootTableType m_Type;
	cLootTablePoolVector m_LootTablePools;
	cLootTableFunctionVector m_LootTableFunctions;
};




/** The LootTableProvider is used per world to ask for loot tables */
class cLootTableProvider
{
public:
	cLootTableProvider();

	std::shared_ptr<cLootTable> GetLootTableFromString(AString a_Name);

	std::shared_ptr<cLootTable> GetLootTableFromBlock();

	std::shared_ptr<cLootTable> GetLooTableFromMob(eMonsterType a_Type);
private:
	// m_CustomLootTables
};





class cChestLootTable: public cLootTable
{
	LootTableType m_Type = LootTableType::Chest;
	cLootTablePoolVector m_LootTablePools =
	{
		{-1, {1, 3}, 0 , {0, 0},
			{
				{{}, E_ITEM_LEAD, 20},
				{{}, E_ITEM_GOLDEN_APPLE, 15},
				{{}, E_ITEM_GOLDEN_APPLE, 2}, // Todo: interpret apple
				{{}, E_ITEM_13_DISC, 15},
				{{}, E_ITEM_CAT_DISC, 15},
				{{}, E_ITEM_NAME_TAG, 20},
				{{}, E_ITEM_CHAIN_CHESTPLATE, 10}
			}
		}
	};
	cLootTableFunctionVector m_LootTableFunctions =
	{

	};
};




