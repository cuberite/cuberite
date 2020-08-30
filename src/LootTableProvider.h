
// cLootTableProvider.h

#pragma once

#include <utility>

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules
#include "BlockType.h"
#include "Mobs/MonsterTypes.h"
#include "BlockEntities/BlockEntityWithItems.h"

namespace LootTable
{
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




	/** Gets the eType from String. Defaults to generic */
	enum eType eType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "Empty") == 0)
		{
			return eType::Empty;
		}
		else if (NoCaseCompare(a_Type, "Entity") == 0)
		{
			return eType::Entity;
		}
		else if (NoCaseCompare(a_Type, "Block") == 0)
		{
			return eType::Block;
		}
		else if (NoCaseCompare(a_Type, "Chest") == 0)
		{
			return eType::Chest;
		}
		else if (NoCaseCompare(a_Type, "Fishing") == 0)
		{
			return eType::Fishing;
		}
		else if (NoCaseCompare(a_Type, "Gift") == 0)
		{
			return eType::Gift;
		}
		else if (NoCaseCompare(a_Type, "AdvancementReward") == 0)
		{
			return eType::AdvancementReward;
		}
		else if (NoCaseCompare(a_Type, "Barter") == 0)
		{
			return eType::Barter;
		}
		else if (NoCaseCompare(a_Type, "Command") == 0)
		{
			return eType::Command;
		}
		else if (NoCaseCompare(a_Type, "Selector") == 0)
		{
			return eType::Selector;
		}
		else if (NoCaseCompare(a_Type, "AdvancementEntity") == 0)
		{
			return eType::AdvancementEntity;
		}
		else
		{
			return eType::Generic;
		}
	}



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




	/** Gets the eChestType from String as of 1.16.2 */
	enum eChestType eChestType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "AbandonedMineshaft") == 0)
		{
			return eChestType::AbandonedMineshaft;
		}
		else if (NoCaseCompare(a_Type, "BuriedTreasure") == 0)
		{
			return eChestType::BuriedTreasure;
		}
		else if (NoCaseCompare(a_Type, "DesertPyramid") == 0)
		{
			return eChestType::DesertPyramid;
		}
		else if (NoCaseCompare(a_Type, "EndCityTreasure") == 0)
		{
			return eChestType::EndCityTreasure;
		}
		else if (NoCaseCompare(a_Type, "IglooChest") == 0)
		{
			return eChestType::IglooChest;
		}
		else if (NoCaseCompare(a_Type, "JungleTemple") == 0)
		{
			return eChestType::JungleTemple;
		}
		else if (NoCaseCompare(a_Type, "JungleTempleDispenser") == 0)
		{
			return eChestType::JungleTempleDispenser;
		}
		else if (NoCaseCompare(a_Type, "NetherBridge") == 0)
		{
			return eChestType::NetherBridge;
		}
		else if (NoCaseCompare(a_Type, "PillagerOutpost") == 0)
		{
			return eChestType::PillagerOutpost;
		}
		else if (NoCaseCompare(a_Type, "ShipwreckMap") == 0)
		{
			return eChestType::ShipwreckMap;
		}
		else if (NoCaseCompare(a_Type, "ShipwreckSupply") == 0)
		{
			return eChestType::ShipwreckSupply;
		}
		else if (NoCaseCompare(a_Type, "ShipwreckTreasure") == 0)
		{
			return eChestType::ShipwreckTreasure;
		}
		else if (NoCaseCompare(a_Type, "SimpleDungeon") == 0)
		{
			return eChestType::SimpleDungeon;
		}
		else if (NoCaseCompare(a_Type, "SpawnBonusChest") == 0)
		{
			return eChestType::SpawnBonusChest;
		}
		else if (NoCaseCompare(a_Type, "StrongholdCorridor") == 0)
		{
			return eChestType::StrongholdCorridor;
		}
		else if (NoCaseCompare(a_Type, "StrongholdCrossing") == 0)
		{
			return eChestType::StrongholdCrossing;
		}
		else if (NoCaseCompare(a_Type, "StrongholdLibrary") == 0)
		{
			return eChestType::StrongholdLibrary;
		}
		else if (NoCaseCompare(a_Type, "UnderwaterRuinBig") == 0)
		{
			return eChestType::UnderwaterRuinBig;
		}
		else if (NoCaseCompare(a_Type, "UnderwaterRuinSmall") == 0)
		{
			return eChestType::UnderwaterRuinSmall;
		}

		/* Village chest types */

		else if (NoCaseCompare(a_Type, "VillageArmorer") == 0)
		{
			return eChestType::VillageArmorer;
		}
		else if (NoCaseCompare(a_Type, "VillageButcher") == 0)
		{
			return eChestType::VillageButcher;
		}
		else if (NoCaseCompare(a_Type, "VillageCartographer") == 0)
		{
			return eChestType::VillageCartographer;
		}
		else if (NoCaseCompare(a_Type, "VillageDesertHouse") == 0)
		{
			return eChestType::VillageDesertHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageFisher") == 0)
		{
			return eChestType::VillageFisher;
		}
		else if (NoCaseCompare(a_Type, "VillageFletcher") == 0)
		{
			return eChestType::VillageFletcher;
		}
		else if (NoCaseCompare(a_Type, "VillageMason") == 0)
		{
			return eChestType::VillageMason;
		}
		else if (NoCaseCompare(a_Type, "VillagePlainsHouse") == 0)
		{
			return eChestType::VillagePlainsHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageSavannaHouse") == 0)
		{
			return eChestType::VillageSavannaHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageShepherd") == 0)
		{
			return eChestType::VillageShepherd;
		}
		else if (NoCaseCompare(a_Type, "VillageSnowyHouse") == 0)
		{
			return eChestType::VillageSnowyHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageTaigaHouse") == 0)
		{
			return eChestType::VillageTaigaHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageTannery") == 0)
		{
			return eChestType::VillageTannery;
		}
		else if (NoCaseCompare(a_Type, "VillageTemple") == 0)
		{
			return eChestType::VillageTemple;
		}
		else if (NoCaseCompare(a_Type, "VillageToolsmith") == 0)
		{
			return eChestType::VillageToolsmith;
		}
		else if (NoCaseCompare(a_Type, "VillageWeaponsmith") == 0)
		{
			return eChestType::VillageWeaponsmith;
		}

		/* Village chest types end */

		else if (NoCaseCompare(a_Type, "WoodlandMansion") == 0)
		{
			return eChestType::WoodlandMansion;
		}
		else
		{
			return eChestType::None;
		}
	}




	/** Gets the eMonsterType from String. Defaults to Giant -> no loot */
	eMonsterType eMonsterType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "Bat") == 0)
		{
			return mtBat;
		}
		else if (NoCaseCompare(a_Type, "Blaze") == 0)
		{
			return mtBlaze;
		}
		else if (NoCaseCompare(a_Type, "CaveSpider") == 0)
		{
			return mtCaveSpider;
		}
		else if (NoCaseCompare(a_Type, "mtChicken") == 0)
		{
			return mtChicken;
		}
		else if (NoCaseCompare(a_Type, "Cow") == 0)
		{
			return mtCow;
		}
		else if (NoCaseCompare(a_Type, "Creeper") == 0)
		{
			return mtCreeper;
		}
		else if (NoCaseCompare(a_Type, "EnderDragon") == 0)
		{
			return mtEnderDragon;
		}
		else if (NoCaseCompare(a_Type, "Enderman") == 0)
		{
			return mtEnderman;
		}
		else if (NoCaseCompare(a_Type, "Ghast") == 0)
		{
			return mtGhast;
		}
		else if (NoCaseCompare(a_Type, "Giant") == 0)
		{
			return mtGiant;
		}
		else if (NoCaseCompare(a_Type, "Guardian") == 0)
		{
			return mtGuardian;
		}
			else if (NoCaseCompare(a_Type, "Horse") == 0)
		{
			return mtHorse;
		}
		else if (NoCaseCompare(a_Type, "IronGolem") == 0)
		{
			return mtIronGolem;
		}
		else if (NoCaseCompare(a_Type, "MagmaCube") == 0)
		{
			return mtMagmaCube;
		}
		else if (NoCaseCompare(a_Type, "Mooshroom") == 0)
		{
			return mtMooshroom;
		}
		else if (NoCaseCompare(a_Type, "Ocelot") == 0)
		{
			return mtOcelot;
		}
		else if (NoCaseCompare(a_Type, "Pig") == 0)
		{
		return mtPig;
		}
		else if (NoCaseCompare(a_Type, "Rabbit") == 0)
		{
			return mtRabbit;
		}
		else if (NoCaseCompare(a_Type, "Sheep") == 0)
		{
			return mtSheep;
		}
		else if (NoCaseCompare(a_Type, "Silverfish") == 0)
		{
			return mtSilverfish;
		}
		else if (NoCaseCompare(a_Type, "Skeleton") == 0)
		{
			return mtSkeleton;
		}
		else if (NoCaseCompare(a_Type, "mtSlime") == 0)
		{
			return mtSlime;
		}
		else if (NoCaseCompare(a_Type, "SnowGolem") == 0)
		{
			return mtSnowGolem;
		}
		else if (NoCaseCompare(a_Type, "Spider") == 0)
		{
			return mtSpider;
		}
		else if (NoCaseCompare(a_Type, "Squid") == 0)
		{
			return mtSquid;
		}
		else if (NoCaseCompare(a_Type, "Villager") == 0)
		{
			return mtVillager;
		}
		else if (NoCaseCompare(a_Type, "mtWitch") == 0)
		{
			return mtWitch;
		}
		else if (NoCaseCompare(a_Type, "Wither") == 0)
		{
			return mtWither;
		}
		else if (NoCaseCompare(a_Type, "WitherSkeleton") == 0)
		{
			return mtWitherSkeleton;
		}
		else if (NoCaseCompare(a_Type, "Wolf") == 0)
		{
			return mtWolf;
		}
		else if (NoCaseCompare(a_Type, "Zombie") == 0)
		{
			return mtZombie;
		}
		else if (NoCaseCompare(a_Type, "ZombiePigman") == 0)
		{
			return mtZombiePigman;
		}
		else if (NoCaseCompare(a_Type, "ZombieVillager") == 0)
		{
			return mtZombieVillager;
		}
		else
		{
			return mtGiant;
		}
	}
}




/** Represents a function for a pool item */
typedef struct cLootTableFunction
{
	cLootTableFunction(LootTable::eFunctionType a_Type, AStringMap a_Parameter, AStringMap a_Conditions):
		m_Parameter(std::move(a_Parameter)),
		m_Conditions(std::move(a_Conditions))
	{
		m_Type = a_Type;
	}
	LootTable::eFunctionType m_Type;
	AStringMap m_Parameter;
	AStringMap m_Conditions;
} cLootTableFunction;

typedef std::vector<cLootTableFunction> cLootTableFunctionVector;




/** Stores the rolls of a pool */
typedef struct cLootTablePoolRolls
{
	/** Pool rolls with optional bonus roll parameter - steady roll needs to be -1 to activate roll range */
	cLootTablePoolRolls(int a_Roll, int a_RollsMin, int a_RollsMax, int a_BonusRoll = 0, int a_BonusRollsMin = 0, int a_BonusRollsMax = 0)
	{
		m_Roll = a_Roll;
		m_RollsMin = a_RollsMin;
		m_RollsMax = a_RollsMax;
		m_BonusRoll = a_BonusRoll;
		m_BonusRollsMin = a_BonusRollsMin;
		m_BonusRollsMax = a_BonusRollsMax;
	}

	int m_Roll = 0;
	int m_RollsMin = 0;
	int m_RollsMax = 0;
	int m_BonusRoll = 0;
	int m_BonusRollsMin = 0;
	int m_BonusRollsMax = 0;
} cLootTablePoolRolls;



/** Represents a pool entry */
typedef struct cLootTablePoolEntry
{
	cLootTablePoolEntry(cLootTableFunctionVector a_Functions, cItem a_Item, int a_Weight)
	{
		m_Functions = std::move(a_Functions);
		m_Item = std::move(a_Item);
		m_Weight = a_Weight;
	}

	cLootTablePoolEntry(cLootTableFunctionVector a_Functions, AString a_Children, int a_Weight)
	{
		m_Functions = std::move(a_Functions);
		m_Children = std::move(a_Children);
		m_Weight = a_Weight;
	}

	// Todo: Conditions
	cLootTableFunctionVector m_Functions;
	// Todo: Add type and think about what is necessary
	cItem m_Item;
	AString m_Children;
	// Todo: Children
	// Todo: Add expand
	int m_Weight;
	// Todo: Add quality
} cLootTablePoolEntry;

typedef std::vector<cLootTablePoolEntry> cLootTablePoolEntryVector;


/** Represents a pool in a loot table */
typedef struct cLootTablePool
{
	/** create a pool with steady roll count */
	cLootTablePool(cLootTablePoolRolls a_Rolls, cLootTablePoolEntryVector a_Entries):
		m_Rolls(a_Rolls),
		m_Entries(std::move(a_Entries))
	{
	}

	cLootTablePoolRolls m_Rolls;
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
	cLootTable(const AString & a_Description);

	cLootTable(const cLootTable & a_Other);

	~cLootTable();

	// Todo: add json interpreter

	/** Reads the string and generates a filled loot table from that and returns the success */
	bool ReadFromString(const AString & a_Description);

	/** Fills the specified block entity at the position with loot and returns the success */
	bool FillWithLoot(cBlockEntityWithItems & a_BlockEntity);

protected:
	/** Type of loot table */
	enum LootTable::eType m_Type;

	/** Vector of loot pools */
	cLootTablePoolVector m_LootTablePools;

	/** Vector of functions applied to all pools */
	cLootTableFunctionVector m_LootTableFunctions;
};

typedef std::map<enum eMonsterType,          std::shared_ptr<cLootTable>> cCustomMonsterLootTableMap;
typedef std::map<enum LootTable::eChestType, std::shared_ptr<cLootTable>> cCustomChestLootTableMap;

/** The LootTableProvider is used per world to ask for loot tables */
class cLootTableProvider
{
public:
	cLootTableProvider(cWorld * a_World);

	/** Function to load a custom loot tables from specified path */
	void LoadCustomLootTable(AString a_Path);

	/** Functions to load loot tables. Custom loot tables are also checked */
	std::shared_ptr<cLootTable> GetLootTable(const AString & a_Name) const;
	std::shared_ptr<cLootTable> GetLootTable(const BLOCKTYPE a_Block) const;
	std::shared_ptr<cLootTable> GetLootTable(const BLOCKTYPE a_Block, const ENUM_BLOCK_META) const;
	std::shared_ptr<cLootTable> GetLootTable(const enum LootTable::eChestType a_Type) const;
	std::shared_ptr<cLootTable> GetLootTable(const eMonsterType a_Type) const;

	/** Drops the loot of a_Monster specified by the loot tables */
	void DropLoot(const cMonster & a_Monster) const;  // Todo: move to unique id of monster

private:

	/** Maps containing custom loot tables */
	cCustomChestLootTableMap   m_CustomChestLootTables   = cCustomChestLootTableMap();
	cCustomMonsterLootTableMap m_CustomMonsterLootTables = cCustomMonsterLootTableMap();

	cWorld * m_World;
};





class cEmptyLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Empty;
};




