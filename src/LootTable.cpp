

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTable.h"
#include "Mobs/MonsterTypes.h"
#include "json/json.h"
#include "BlockEntities/BlockEntityWithItems.h"
#include "Noise/Noise.h"
#include "Root.h"
#include "FurnaceRecipe.h"

namespace LootTable
{
	/** Gets the eType from String. Defaults to Generic */
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
			LOGWARNING("Unknown Loot table type provided: %s. defaulting to Generic", a_Type);
			return eType::Generic;
		}
	}





	/** Gets the eConditionType from String. Defaults to None */
	enum eConditionType eConditionType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "Alternative") == 0)
		{
			return eConditionType::Alternative;
		}
		else if (NoCaseCompare(a_Type, "BlockStateProperty") == 0)
		{
			return eConditionType::BlockStateProperty;
		}
		else if (NoCaseCompare(a_Type, "DamageSourceProperties") == 0)
		{
			return eConditionType::DamageSourceProperties;
		}
		else if (NoCaseCompare(a_Type, "EntityProperties") == 0)
		{
			return eConditionType::EntityProperties;
		}
		else if (NoCaseCompare(a_Type, "EntityScores") == 0)
		{
			return eConditionType::EntityScores;
		}
		else if (NoCaseCompare(a_Type, "Inverted") == 0)
		{
			return eConditionType::Inverted;
		}
		else if (NoCaseCompare(a_Type, "KilledByPlayer") == 0)
		{
			return eConditionType::KilledByPlayer;
		}
		else if (NoCaseCompare(a_Type, "LocationCheck") == 0)
		{
			return eConditionType::LocationCheck;
		}
		else if (NoCaseCompare(a_Type, "MatchTool") == 0)
		{
			return eConditionType::MatchTool;
		}
		else if (NoCaseCompare(a_Type, "RandomChance") == 0)
		{
			return eConditionType::RandomChance;
		}
		else if (NoCaseCompare(a_Type, "RandomChanceWithLooting") == 0)
		{
			return eConditionType::RandomChanceWithLooting;
		}
		else if (NoCaseCompare(a_Type, "Reference") == 0)
		{
			return eConditionType::Reference;
		}
		else if (NoCaseCompare(a_Type, "SurvivesExplosion") == 0)
		{
			return eConditionType::SurvivesExplosion;
		}
		else if (NoCaseCompare(a_Type, "TableBonus") == 0)
		{
			return eConditionType::TableBonus;
		}
		else if (NoCaseCompare(a_Type, "TimeCheck") == 0)
		{
			return eConditionType::TimeCheck;
		}
		else if (NoCaseCompare(a_Type, "WeatherCheck") == 0)
		{
			return eConditionType::WeatherCheck;
		}
		else
		{
			LOGWARNING("Unknown loot table condition provided: %s. Using no condition", a_Type);
			return eConditionType::None;
		}
	}




	enum eFunctionType eFunctionType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "ApplyBonus") == 0)
		{
			return eFunctionType::ApplyBonus;
		}
		else if (NoCaseCompare(a_Type, "CopyName") == 0)
		{
			return eFunctionType::CopyName;
		}
		else if (NoCaseCompare(a_Type, "CopyNbt") == 0)
		{
			return eFunctionType::CopyNbt;
		}
		else if (NoCaseCompare(a_Type, "CopyState") == 0)
		{
			return eFunctionType::CopyState;
		}
		else if (NoCaseCompare(a_Type, "EnchantRandomly") == 0)
		{
			return eFunctionType::EnchantRandomly;
		}
		else if (NoCaseCompare(a_Type, "EnchantWithLevels") == 0)
		{
			return eFunctionType::EnchantWithLevels;
		}
		else if (NoCaseCompare(a_Type, "ExplorationMap") == 0)
		{
			return eFunctionType::ExplorationMap;
		}
		else if (NoCaseCompare(a_Type, "ExplosionDecay") == 0)
		{
			return eFunctionType::ExplosionDecay;
		}
		else if (NoCaseCompare(a_Type, "FurnaceSmelt") == 0)
		{
			return eFunctionType::FurnaceSmelt;
		}
		else if (NoCaseCompare(a_Type, "FillPlayerHead") == 0)
		{
			return eFunctionType::FillPlayerHead;
		}
		else if (NoCaseCompare(a_Type, "LimitCount") == 0)
		{
			return eFunctionType::LimitCount;
		}
		else if (NoCaseCompare(a_Type, "LootingEnchant") == 0)
		{
			return eFunctionType::LootingEnchant;
		}
		else if (NoCaseCompare(a_Type, "SetAttributes") == 0)
		{
			return eFunctionType::SetAttributes;
		}
		else if (NoCaseCompare(a_Type, "SetContents") == 0)
		{
			return eFunctionType::SetContents;
		}
		else if (NoCaseCompare(a_Type, "SetCount") == 0)
		{
			return eFunctionType::SetCount;
		}
		else if (NoCaseCompare(a_Type, "SetDamage") == 0)
		{
			return eFunctionType::SetDamage;
		}
		else if (NoCaseCompare(a_Type, "SetLootTable") == 0)
		{
			return eFunctionType::SetLootTable;
		}
		else if (NoCaseCompare(a_Type, "SetLore") == 0)
		{
			return eFunctionType::SetLore;
		}
		else if (NoCaseCompare(a_Type, "SetName") == 0)
		{
			return eFunctionType::SetName;
		}
		else if (NoCaseCompare(a_Type, "SetNbt") == 0)
		{
			return eFunctionType::SetNbt;
		}
		else if (NoCaseCompare(a_Type, "SetStewEffect") == 0)
		{
			return eFunctionType::SetStewEffect;
		}
		else if (NoCaseCompare(a_Type, "None") == 0)
		{
			return eFunctionType::None;
		}
		else
		{
			LOGWARNING("Unknown loot table function provided: %s. Using empty function", a_Type);
			return eFunctionType::None;
		}
	}




	/** Gets the ePoolEntryType from String. Defaults to Empty */
	enum ePoolEntryType ePoolEntryType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "Item") == 0)
		{
			return ePoolEntryType::Item;
		}
		else if (NoCaseCompare(a_Type, "Tag") == 0)
		{
			return ePoolEntryType::Tag;
		}
		else if (NoCaseCompare(a_Type, "LootTable") == 0)
		{
			return ePoolEntryType::LootTable;
		}
		else if (NoCaseCompare(a_Type, "Group") == 0)
		{
			return ePoolEntryType::Group;
		}
		else if (NoCaseCompare(a_Type, "Alternatives") == 0)
		{
			return ePoolEntryType::Alternatives;
		}
		else if (NoCaseCompare(a_Type, "Sequence") == 0)
		{
			return ePoolEntryType::Sequence;
		}
		else if (NoCaseCompare(a_Type, "Dynamic") == 0)
		{
			return ePoolEntryType::Dynamic;
		}
		else if (NoCaseCompare(a_Type, "Empty") == 0)
		{
			return ePoolEntryType::Empty;
		}
		else
		{
			LOGWARNING("Unknown loot table pool entry type provided: %s. Defaulting to Empty", a_Type);
			return ePoolEntryType::Empty;
		}
	}




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




	/** Replaces a_From with a_To in a_String */
	void Replace(AString & a_String, const AString & a_From, const AString & a_To)
	{
		size_t start_pos;
		while (true)
		{
			start_pos = a_String.find(a_From);
			if (start_pos == AString ::npos)
			{
				return;
			}
			a_String.replace(start_pos, a_From.length(), a_To);
		}
	}




	/** Capitalizes any letter following a space */
	void Capitalize (AString & a_String)
	{
		bool Cap = true;

		for (unsigned int i = 0; i <= a_String.length(); i++)
		{
			if (isalpha(a_String[i]) && Cap)
			{
				a_String[i] = toupper(a_String[i]);
				Cap = false;
			}
			else if (isspace(a_String[i]))
			{
				Cap = true;
			}
		}
	}




	/** Moves a_String from the "minecraft:" namespace to cuberite namespace */
	AString NamespaceConverter(AString a_String)
	{
		Replace(a_String, "minecraft:", "");
		Replace(a_String, "_", " ");
		Capitalize(a_String);
		Replace(a_String, " ", "");
		return a_String;
	}
}

////////////////////////////////////////////////////////////////////////////////
// cLootTable


cLootTable::cLootTable()
{
	m_Type = LootTable::eType::Empty;
}





cLootTable::cLootTable(const Json::Value & a_Description)
{
	for (const auto & RootId : a_Description.getMemberNames())
	{
		if (NoCaseCompare(RootId, "type") == 0)
		{
			m_Type = LootTable::eType(LootTable::NamespaceConverter(a_Description[RootId].asString()));
		}
		else if (NoCaseCompare(RootId, "pools") == 0)
		{
			Json::Value Pools = a_Description[RootId];
			for (auto PoolId = 0; PoolId < Pools.size(); PoolId++)
			{
				ReadLootTablePool(Pools[PoolId]);
			}
		}
		else if (NoCaseCompare(RootId, "functions") == 0)
		{
			auto FunctionsObject = a_Description[RootId];
			for (auto FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
			{
				m_Functions.push_back(ReadLootTableFunction(FunctionsObject[FunctionIndex]));
			}
		}
	}
}





bool cLootTable::FillWithLoot(cBlockEntityWithItems * a_BlockEntity, cUUID * a_Player) const
{
	auto & ItemGrid = a_BlockEntity->GetContents();
	auto Seed = a_BlockEntity->GetWorld()->GetGenerator().GetSeed();
	auto Noise = cNoise(Seed);
	auto Items = GetItems(Noise, a_BlockEntity->GetPos(), a_Player);
	return false;
}





std::vector<cItem> cLootTable::GetItems(cNoise & a_Noise, const Vector3i & a_Pos, cUUID * a_Player, cEntity * a_Entity) const
{
	auto Items = std::vector<cItem>();
	for (const auto & Pool : m_LootTablePools)
	{
		auto NewItems = GetItems(Pool, m_World, a_Noise, a_Pos, a_Player, a_Entity);
		Items.insert(Items.end(), NewItems.begin(), NewItems.end());
	}
	for (auto & Item : Items)
	{
		for (const auto & Function : m_Functions)
		{
			ApplyFunction(Function, Item, m_World, a_Noise, a_Player);
		}
	}
	return Items;
}





void cLootTable::ReadLootTablePool(const Json::Value & a_Value)
{
	cLootTablePoolRolls PoolRolls;
	cLootTablePoolRolls PoolBonusRolls;
	cLootTablePoolEntryVector PoolEntries;
	cLootTableFunctionVector Functions;
	cLootTableConditionVector Conditions;
	for (auto & PoolElement : a_Value.getMemberNames())
	{
		if (NoCaseCompare(PoolElement, "rolls") == 0)
		{
			auto Rolls = a_Value[PoolElement];
			if (Rolls.isInt())
			{
				PoolRolls = cLootTablePoolRolls(Rolls.asInt(), Rolls.asInt());
			}
			else
			{
				int Min = -1, Max = -1;
				for (auto & RollEntry: Rolls.getMemberNames())
				{
					if (NoCaseCompare(RollEntry, "min") == 0)
					{
						Min = Rolls[RollEntry].asInt();
					}
					else if (NoCaseCompare(RollEntry, "max") == 0)
					{
						Max = Rolls[RollEntry].asInt();
					}
					// Todo - add type
				}
				// Todo: add info on which file is wrong
				if (Min == -1)
				{
					LOGWARNING("Missing Minimum value in ... Dropping loot table pool.");
					return;
				}
				if (Max == -1)
				{
					LOGWARNING("Missing maximum value in ... Dropping loot table pool.");
					return;
				}
				if (Min > Max)
				{
					PoolRolls = cLootTablePoolRolls(Min, Min);
				}
				PoolRolls = cLootTablePoolRolls(Min, Max);
			}
		}
		else if (NoCaseCompare(PoolElement, "bonus_rolls") == 0)
		{
			auto Rolls = a_Value[PoolElement];
			if (Rolls.isInt())
			{
				PoolRolls = cLootTablePoolRolls(Rolls.asInt(), Rolls.asInt());
			}
			else
			{
				int Min = -1, Max = -1;
				for (auto & RollEntry: Rolls.getMemberNames())
				{
					if (NoCaseCompare(RollEntry, "min") == 0)
					{
						Min = Rolls[RollEntry].asInt();
					}
					else if (NoCaseCompare(RollEntry, "max") == 0)
					{
						Max = Rolls[RollEntry].asInt();
					}
					// Todo - add type
				}
				// Todo: add info on which file is wrong
				if (Min == -1)
				{
					LOGWARNING("Missing Minimum value in ... Dropping loot table pool.");
					return;
				}
				if (Max == -1)
				{
					LOGWARNING("Missing maximum value in ... Dropping loot table pool.");
					return;
				}
				PoolBonusRolls = cLootTablePoolRolls(Min, Max);
			}
		}
		else if (NoCaseCompare(PoolElement, "entries") == 0)
		{
			auto Entries = a_Value[PoolElement];
			for (unsigned int EntryIndex = 0; EntryIndex < Entries.size(); EntryIndex++)
			{
				auto Entry = Entries[EntryIndex];
				PoolEntries.push_back(ReadLootTablePoolEntry(Entry));
			}
		}
		else if (NoCaseCompare(PoolElement, "functions") == 0)
		{
			auto FunctionsObject = a_Value[PoolElement];
			for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
			{
				Functions.push_back(ReadLootTableFunction(FunctionsObject[FunctionIndex]));
			}
		}
		else if (NoCaseCompare(PoolElement, "conditions") == 0)
		{
			auto ConditionsObject = a_Value[PoolElement];
			for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
			{
				Conditions.push_back(ReadLootTableCondition(ConditionsObject[ConditionId]));
			}
		}
	}
	m_LootTablePools.push_back(cLootTablePool(PoolRolls, PoolEntries, Conditions));
}





cLootTableFunction cLootTable::ReadLootTableFunction(const Json::Value & a_Value)
{
	enum LootTable::eFunctionType Type;
	AStringMap Parameter;
	cLootTableConditionVector Conditions;
	for (auto & FunctionInfo : a_Value.getMemberNames())
	{
		if (NoCaseCompare(FunctionInfo, "function") == 0)
		{
			Type = LootTable::eFunctionType(LootTable::NamespaceConverter(a_Value[FunctionInfo].asString()));
		}
		else if (NoCaseCompare(FunctionInfo, "conditions") == 0)
		{
			auto ConditionsObject = a_Value[FunctionInfo];
			for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
			{
				Conditions.push_back(ReadLootTableCondition(ConditionsObject[ConditionId]));
			}
		}
		else  // Parameters
		{
			Parameter.merge(ReadParameter(a_Value[FunctionInfo]));
		}
	}
	return cLootTableFunction(Type, Parameter, Conditions);
}





cLootTableCondition cLootTable::ReadLootTableCondition(const Json::Value & a_Value)
{
	enum LootTable::eConditionType Type;
	AStringMap Parameter;
	cLootTableConditionVector SubConditions;

	// Type has to be known beforehand
	if (a_Value.isMember("condition"))
	{
		Type = LootTable::eConditionType(LootTable::NamespaceConverter(a_Value["type"].asString()));
	}
	else if (a_Value.isMember("Condition"))
	{
		Type = LootTable::eConditionType(LootTable::NamespaceConverter(a_Value["Type"].asString()));
	}
	else
	{
		LOGWARNING("Loot table is missing condition type. Dropping condition!");
		return cLootTableCondition();
	}

	switch (Type)
	{
		case LootTable::eConditionType::Alternative:
		{
			Json::Value Terms;
			if (a_Value.isMember("terms"))
			{
				Terms = a_Value["terms"];
			}
			else if (a_Value.isMember("Terms"))
			{
				Terms = a_Value["Terms"];
			}
			else
			{
				LOGWARNING("Loot table condition \"Alternative\" is missing sub - conditions. Dropping condition!");
				return cLootTableCondition();
			}
			for (unsigned int i = 0; i < Terms.size(); i++)
			{
				SubConditions.push_back(ReadLootTableCondition(Terms[i]));
			}
			break;
		}
		case LootTable::eConditionType::Inverted:
		{
			Json::Value Term;
			if (a_Value.isMember("term"))
			{
				Term = a_Value["term"];
				SubConditions.push_back(ReadLootTableCondition(Term));
			}
			else if (a_Value.isMember("Term"))
			{
				Term = a_Value["Term"];
				SubConditions.push_back(ReadLootTableCondition(Term));
			}
			else
			{
				LOGWARNING("Loot table condition \"Inverted\" is missing sub-condition. Dropping condition!");
				return cLootTableCondition();
			}
		}
		case LootTable::eConditionType::Reference:
		{
			LOGWARNING("Type \"Reference\" for loot table conditions is not yet supported. Dropping condition");
			return cLootTableCondition();
		}
		case LootTable::eConditionType::SurvivesExplosion:
		{
			return cLootTableCondition(Type, Parameter, SubConditions);
		}
		case LootTable::eConditionType::None:
		{
			return cLootTableCondition();
		}
		case LootTable::eConditionType::BlockStateProperty:
		case LootTable::eConditionType::DamageSourceProperties:
		case LootTable::eConditionType::EntityProperties:
		case LootTable::eConditionType::EntityScores:
		case LootTable::eConditionType::KilledByPlayer:
		case LootTable::eConditionType::LocationCheck:
		case LootTable::eConditionType::MatchTool:
		case LootTable::eConditionType::RandomChance:
		case LootTable::eConditionType::RandomChanceWithLooting:
		case LootTable::eConditionType::TableBonus:
		case LootTable::eConditionType::TimeCheck:
		case LootTable::eConditionType::WeatherCheck:
		{
			for (const auto & Name : a_Value.getMemberNames())
			{
				if (NoCaseCompare(Name, "condition"))
				{
					auto ParameterObject = a_Value[Name];
					Parameter.merge(ReadParameter(ParameterObject));
				}
			}
			return cLootTableCondition(Type, Parameter, SubConditions);
		}
	}

	for (const auto & ConditionParameterName: a_Value.getMemberNames())
	{
		if (NoCaseCompare(ConditionParameterName, "condition") == 0)
		{
			Type = LootTable::eConditionType(LootTable::NamespaceConverter(a_Value[ConditionParameterName].asString()));
		}
		else
		{
			Parameter.merge(ReadParameter(a_Value[ConditionParameterName]));
		}
	}
	return cLootTableCondition(Type, Parameter, SubConditions);
}





cLootTablePoolEntry cLootTable::ReadLootTablePoolEntry(const Json::Value & a_Value)
{
	cLootTableConditionVector Conditions;
	cLootTableFunctionVector Functions;
	enum LootTable::ePoolEntryType Type;

	cItem Item;
	AString Name;
	cLootTablePoolEntryVector Children;

	bool Expand;
	int Weight = 0;
	int Quality = 0;

	// The type has to be known beforehand
	if (a_Value.isMember("type"))
	{
		Type = LootTable::ePoolEntryType(LootTable::NamespaceConverter(a_Value["type"].asString()));
	}
	else if (a_Value.isMember("Type"))
	{
		Type = LootTable::ePoolEntryType(LootTable::NamespaceConverter(a_Value["Type"].asString()));
	}
	else
	{
		Type = LootTable::ePoolEntryType::Empty;
	}

	for (auto & EntryParameter : a_Value.getMemberNames())
	{
		if (NoCaseCompare(EntryParameter, "weight") == 0)
		{
			Weight = a_Value[EntryParameter].asInt();
		}
		else if (NoCaseCompare(EntryParameter, "name") == 0)
		{
			switch (Type)
			{
				case LootTable::ePoolEntryType::Item:
				{
					StringToItem(LootTable::NamespaceConverter(a_Value[EntryParameter].asString()), Item);
					break;
				}
				case LootTable::ePoolEntryType::Tag:
				case LootTable::ePoolEntryType::LootTable:
				case LootTable::ePoolEntryType::Dynamic:
				{
					Name = LootTable::NamespaceConverter(a_Value[EntryParameter].asString());
					break;
				}
				default: break;
			}
		}
		else if (NoCaseCompare(EntryParameter, "functions") == 0)
		{
			auto FunctionsObject = a_Value[EntryParameter];
			for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
			{
				Functions.push_back(ReadLootTableFunction(FunctionsObject[FunctionIndex]));
			}
		}
		else if (NoCaseCompare(EntryParameter, "conditions") == 0)
		{
			auto ConditionsObject = a_Value[EntryParameter];
			for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
			{
				Conditions.push_back(ReadLootTableCondition(ConditionsObject[ConditionId]));
			}
		}
		else if (NoCaseCompare(EntryParameter, "children") == 0)
		{
			switch (Type)
			{
				case LootTable::ePoolEntryType::Group:
				case LootTable::ePoolEntryType::Alternatives:
				case LootTable::ePoolEntryType::Sequence:
				{
					auto ChildrenObject = a_Value[EntryParameter];
					for (unsigned int ChildrenObjectId = 0; ChildrenObjectId < ChildrenObject.size(); ++ChildrenObjectId)
					{
						Children.push_back(ReadLootTablePoolEntry(ChildrenObject[ChildrenObjectId]));
					}
					break;
				}
				default: break;
			}
		}
		else if (NoCaseCompare(EntryParameter, "expand") == 0)
		{
			Expand = a_Value[EntryParameter].asBool();
		}
		else if (NoCaseCompare(EntryParameter, "quality") == 0)
		{
			Quality = a_Value[EntryParameter].asInt();
		}
	}
	switch (Type)
	{
		case LootTable::ePoolEntryType::Item:
		{
			return cLootTablePoolEntry(Conditions, Functions, Type, Item, Weight, Quality);
		}

		case LootTable::ePoolEntryType::Tag:
		case LootTable::ePoolEntryType::LootTable:
		case LootTable::ePoolEntryType::Dynamic:
		{
			return cLootTablePoolEntry(Conditions, Functions, Type, Name, Expand, Weight, Quality);
		}

		case LootTable::ePoolEntryType::Group:
		case LootTable::ePoolEntryType::Alternatives:
		case LootTable::ePoolEntryType::Sequence:
		{
			return cLootTablePoolEntry(Conditions, Functions, Type, Children, Weight, Quality);
		}

		case LootTable::ePoolEntryType::Empty: return cLootTablePoolEntry();
		default:                               return cLootTablePoolEntry();
	}
}





AStringMap cLootTable::ReadParameter(const Json::Value & a_Value)
{
	AStringMap Result;
	if (a_Value.isObject())
	{
		for (const auto & Name : a_Value.getMemberNames())
		{
			if (a_Value.isNumeric())
			{
				Result[Name] = std::to_string(a_Value[Name].asInt());
			}
			else if (a_Value.isBool())
			{
				if (a_Value[Name].asBool())
				{
					Result[LootTable::NamespaceConverter(Name)] = "True";
				}
				else
				{
					Result[LootTable::NamespaceConverter(Name)] = "False";
				}
			}
			else if (a_Value.isString())
			{
				Result[LootTable::NamespaceConverter(Name)] = LootTable::NamespaceConverter(a_Value[Name].asString());
			}
			else if (a_Value.isObject() ||a_Value.isArray())
			{
				Result.merge(ReadParameter(a_Value[Name]));
			}
		}
	}
	else if (a_Value.isArray())
	{
		for (unsigned int i = 0; i < a_Value.size(); i++)
		{
			Result.merge(ReadParameter(a_Value[i]));
		}
	}
	return Result;
}





std::vector<cItem> cLootTable::GetItems(const cLootTablePool & a_Pool, const cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const cUUID * a_Player, const cEntity * a_Entity)
{
	auto Items = std::vector<cItem>();
	if (!ConditionsApply(a_Pool.m_Conditions, a_World, a_Noise, a_Player))
	{
		return Items;
	}

	// Steady Roll
	if (a_Pool.m_Rolls.m_RollsMax == a_Pool.m_Rolls.m_RollsMin)
	{
		for (int i = 0; i < a_Pool.m_Rolls.m_RollsMin; i++)
		{
			for (const auto & Entry : a_Pool.m_Entries)
			{
				auto NewItems = GetItems(Entry, a_World, a_Noise, a_Pos, a_Player, a_Entity);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			}
		}
	}
	else  // Roll range
	{
		auto RollCount = a_Noise.IntNoise3DInt(a_Pos) % (a_Pool.m_Rolls.m_RollsMax - a_Pool.m_Rolls.m_RollsMin) + a_Pool.m_Rolls.m_RollsMin;
		for (int i = 0; i < RollCount; i++)
		{
			for (const auto & Entry : a_Pool.m_Entries)
			{
				auto NewItems = GetItems(Entry, a_World, a_Noise, a_Pos, a_Player, a_Entity);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			}
		}
	}
	return Items;
}





std::vector<cItem> cLootTable::GetItems(const cLootTablePoolEntry & a_Entry, const cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const cUUID * a_Player, const cEntity * a_Entity)
{
	// Todo: add luck here, when it's implemented
	auto Items = std::vector<cItem>();

	if (ConditionsApply(a_Entry.m_Conditions, a_World, a_Noise, a_Player, a_Entity))

		switch (a_Entry.m_Type)
		{
			case LootTable::ePoolEntryType::Item:
			{
				// Usually this only contains item - but just case
				try
				{
					Items.push_back(std::get<cItem>(a_Entry.m_Content));
				}
				catch (const std::bad_variant_access &)
				{
					LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
				}
				break;
			}
			case LootTable::ePoolEntryType::Tag:
			{
				// Todo: check what names are used
				// Todo: add loot tables for that
				// Todo: return item(s)
				break;
			}
			case LootTable::ePoolEntryType::LootTable:
			{
				// Todo - add loot table lookup
				break;
			}
			case LootTable::ePoolEntryType::Group:
			{
				try
				{
					for (const auto & Child : std::get<cLootTablePoolEntryVector>(a_Entry.m_Content))
					{
						auto NewItems = GetItems(Child, a_World, a_Noise, a_Pos, a_Player, a_Entity);
						Items.insert(Items.end(), NewItems.begin(), NewItems.end());
					}
				}
				catch (const std::bad_variant_access &)
				{
					LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
				}
				break;
			}
			case LootTable::ePoolEntryType::Alternatives:
			{
				try
				{
					auto Children = std::get<cLootTablePoolEntryVector>(a_Entry.m_Content);
					auto ChildPos = a_Noise.IntNoise3DInt(a_Pos) % Children.size();
					auto NewItems = GetItems(Children[ChildPos], a_World, a_Noise, a_Pos, a_Player, a_Entity);
					Items.insert(Items.end(), NewItems.begin(), NewItems.end());
				}
				catch (const std::bad_variant_access &)
				{
					LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
				}
				break;
			}
			case LootTable::ePoolEntryType::Sequence:
				break;
				// TODO
			case LootTable::ePoolEntryType::Dynamic:
				break;
				// TODO
			case LootTable::ePoolEntryType::Empty:
			{
				Items.push_back(cItem(E_BLOCK_AIR));
				break;
			}
		}
	for (auto & Item : Items)
	{
		for (const auto & Function : a_Entry.m_Functions)
		{
			ApplyFunction(Function, Item, a_World, a_Noise, a_Player);
		}
	}
	return Items;
}





bool cLootTable::ConditionsApply(const cLootTableConditionVector & a_Conditions, const cWorld * a_World, const cNoise & a_Noise, const cUUID * a_Player, const cEntity * a_Entity)
{
	bool Success = true;
	for (const auto & Condition : a_Conditions)
	{
		Success &= ConditionApplies(Condition, a_World, a_Noise, a_Player, a_Entity);
	}
	return Success;
}





bool cLootTable::ConditionApplies(const cLootTableCondition & a_Condition, const cWorld * a_World, const cNoise & a_Noise, const cUUID * a_Player, const cEntity * a_Entity)
{
	switch (a_Condition.m_Type)
	{
		case LootTable::eConditionType::None:
		{
			return true;
		}
		case LootTable::eConditionType::Alternative:
		{
			bool Success = false;
			for (const auto & SubCondition: a_Condition.m_SubConditions)
			{
				Success |= ConditionApplies(SubCondition, a_World, a_Noise, a_Player, a_Entity);
			}
			return Success;
		}
		case LootTable::eConditionType::BlockStateProperty:
		{
			// TODO
			LOGWARNING("Loot table condition \"BlockStateProperty\" is not yet supported. Assuming it's true.");
			return true;
		}
		case LootTable::eConditionType::DamageSourceProperties:
		case LootTable::eConditionType::EntityProperties:
		case LootTable::eConditionType::EntityScores:
		case LootTable::eConditionType::Inverted:
		case LootTable::eConditionType::KilledByPlayer:
		case LootTable::eConditionType::LocationCheck:
		case LootTable::eConditionType::MatchTool:
		case LootTable::eConditionType::RandomChance:
		case LootTable::eConditionType::RandomChanceWithLooting:
		case LootTable::eConditionType::Reference:
		{
			// TODO
			return true;
		}
		case LootTable::eConditionType::SurvivesExplosion:
		case LootTable::eConditionType::TableBonus:
		case LootTable::eConditionType::TimeCheck:
		case LootTable::eConditionType::WeatherCheck:
		default: return true;
	}
}




// Item in container
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, const cWorld * a_World, const cNoise & a_Noise, const cUUID * a_Player)
{
	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::EnchantRandomly:  // Item
		{
			break;
		}
		case LootTable::eFunctionType::EnchantWithLevels:  // Item
		{
			break;
		}
		case LootTable::eFunctionType::ExplorationMap:  // Item
		{
			LOGWARNING("Exploration maps are not implemented, dropping function!");
			break;
		}
		case LootTable::eFunctionType::FurnaceSmelt:  // Item, Block break, Entity Kill
		{
			auto Recipes = cRoot::Get()->GetFurnaceRecipe();
			auto NewItem = Recipes->GetRecipeFrom(a_Item)->Out;
			a_Item.m_ItemType = NewItem->m_ItemType;
			a_Item.m_ItemDamage = NewItem->m_ItemDamage;
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:  // All
		{
			break;
		}
		case LootTable::eFunctionType::LimitCount:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetAttributes:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetCount:  // All
		{
			auto Count = a_Function.m_Parameter.at("Count");
			// Has no specified range
			if (Count.empty())
			{
				auto Type = a_Function.m_Parameter.at("Type");
				if (Type == "Uniform")
				{
					auto MinString = a_Function.m_Parameter.at("Min");
					auto MaxString = a_Function.m_Parameter.at("Max");
					if (MinString.empty() || MaxString.empty())
					{
						LOGWARNING("Missing Range parameters on SetCount Function. dropping function!");
						break;
					}
					int Min = std::stoi(MinString);
					int Max = std::stoi(MaxString);
					a_Item.m_ItemCount = a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Max - Min) + Min;
				}
				else if (Type == "Binomial")
				{
					auto NString = a_Function.m_Parameter.at("N");
					auto PString = a_Function.m_Parameter.at("P");
					if (NString.empty() || PString.empty())
					{
						LOGWARNING("Missing Range parameters on SetCount Function. dropping function!");
						break;
					}
					int N = std::stoi(NString);
					float P = std::stof(PString);
					std::default_random_engine Generator(a_Noise.GetSeed());
					std::binomial_distribution<int> Dist(N, P);
					std::vector<int> Values;
					for (int i = 0; i < N; i++)
					{
						Values[i] = Dist(Generator);
					}
					a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (N);
				}
			}
			else
			{
				a_Item.m_ItemCount = std::stoi(Count);
			}
			break;
		}
		case LootTable::eFunctionType::SetDamage:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetLootTable:  // All
		{
			// TODO: add when implemented
			LOGWARNING("NBT for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetLore:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetName:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetNbt:  // Item
		{
			// TODO: add when implemented
			LOGWARNING("NBT for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetStewEffect:  // Item
		{
			// TODO: add when implemented
			LOGWARNING("Stews are not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::None:
		{
			break;
		}
		default:
		{
			LOGWARNING("Function type if not supported for this entity, dropping function!");
			// TODO add warning for unsupported function type
			break;
		}
	}
}




// Block
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, const cWorld * a_World, const cBlockHandler & a_Block, const cNoise & a_Noise, const cUUID * a_Player)
{
	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::ApplyBonus:  // Block Break, killed Entity
		{
			auto Enchantment = cEnchantments::StringToEnchantmentID(a_Function.m_Parameter.at("Enchantment"));
			auto Formula = a_Function.m_Parameter.at("Formula");

			// Todo: get enchantment level of tool in player hand
			int Level = 1;

			if (Formula == "BinomialWithBonusCount")
			{
				// Binomial Probability Distribution with n=level + extra, p=probability
				auto ExtraString = a_Function.m_Parameter.at("Extra");
				int Extra = std::stoi(ExtraString);
				auto ProbString = a_Function.m_Parameter.at("Probability");
				float Probability = std::stof(ProbString);

				std::default_random_engine Generator(a_Noise.GetSeed());
				std::binomial_distribution<int> Dist(Level + Extra, Probability);
				std::vector<int> Values;
				for (int i = 0; i < Level + Extra; i++)
				{
					Values[i] = Dist(Generator);
				}
				a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Level + Extra);
			}
			else if (Formula == "UniformBonusCount")
			{
				// prob = (0, level * BonusMultiplier)
				auto BonusMultiplierString = a_Function.m_Parameter.at("BonusMultiplier");
				int BonusMultiplier = std::stoi(BonusMultiplierString);
				a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Level * BonusMultiplier);
			}
			else if (Formula == "OreDrops")
			{
				// Count *= (max(0; random(0..Level + 2) - 1) + 1)
				a_Item.m_ItemCount *= std::max(0, a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Level + 2) - 1) + 1;
			}
			break;
		}
		case LootTable::eFunctionType::CopyNbt:  // Block Entity, Killed Entity
		{
			LOGWARNING("NBT for items is not yet supported, Dropping function!");
			break;
		}
		case LootTable::eFunctionType::CopyState:  // Block break
		{
			// TODO: add when implemented
			LOGWARNING("NBT for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::ExplosionDecay:  // Block break
		{
			break;
		}
		case LootTable::eFunctionType::FurnaceSmelt:  // Item, Block break
		{
			auto Recipes = cRoot::Get()->GetFurnaceRecipe();
			auto NewItem = Recipes->GetRecipeFrom(a_Item)->Out;
			a_Item.m_ItemType = NewItem->m_ItemType;
			a_Item.m_ItemDamage = NewItem->m_ItemDamage;
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:  // All
		{
			break;
		}
		case LootTable::eFunctionType::LimitCount:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetAttributes:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetContents:  // Block break
		{
			break;
		}
		case LootTable::eFunctionType::SetCount:  // All
		{
			auto Count = a_Function.m_Parameter.at("Count");
			// Has no specified range
			if (Count.empty())
			{
				auto Type = a_Function.m_Parameter.at("Type");
				if (Type == "Uniform")
				{
					auto MinString = a_Function.m_Parameter.at("Min");
					auto MaxString = a_Function.m_Parameter.at("Max");
					if (MinString.empty() || MaxString.empty())
					{
						LOGWARNING("Missing Range parameters on SetCount Function. dropping function!");
						break;
					}
					int Min = std::stoi(MinString);
					int Max = std::stoi(MaxString);
					a_Item.m_ItemCount = a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Max - Min) + Min;
				}
				else if (Type == "Binomial")
				{
					auto NString = a_Function.m_Parameter.at("N");
					auto PString = a_Function.m_Parameter.at("P");
					if (NString.empty() || PString.empty())
					{
						LOGWARNING("Missing Range parameters on SetCount Function. dropping function!");
						break;
					}
					int N = std::stoi(NString);
					float P = std::stof(PString);
					std::default_random_engine Generator(a_Noise.GetSeed());
					std::binomial_distribution<int> Dist(N, P);
					std::vector<int> Values;
					for (int i = 0; i < N; i++)
					{
						Values[i] = Dist(Generator);
					}
					a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (N);
				}
			}
			else
			{
				a_Item.m_ItemCount = std::stoi(Count);
			}
			break;
		}
		case LootTable::eFunctionType::SetLore:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetDamage:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetName:  // All
		{
			break;
		}
		default:
		{
			LOGWARNING("Function type if not supported for this entity, dropping function!");
			break;
		}
	}
}




// Block Entity
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, const cWorld * a_World, const cBlockEntity & a_BlockEntity, const cNoise & a_Noise, const cUUID * a_Player)
{
	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::SetAttributes:  // All
		{
			break;
		}
		case LootTable::eFunctionType::CopyName:  // Block Entity
		{
			// TODO: add when implemented
			LOGWARNING("Custom Names for Block entities are not yet supported. Dropping function");
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:  // All
		{
			break;
		}
		case LootTable::eFunctionType::LimitCount:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetCount:  // All
		{
			auto Count = a_Function.m_Parameter.at("Count");
			// Has no specified range
			if (Count.empty())
			{
				auto Type = a_Function.m_Parameter.at("Type");
				if (Type == "Uniform")
				{
					auto MinString = a_Function.m_Parameter.at("Min");
					auto MaxString = a_Function.m_Parameter.at("Max");
					if (MinString.empty() || MaxString.empty())
					{
						LOGWARNING("Missing Range parameters on SetCount Function. dropping function!");
						break;
					}
					int Min = std::stoi(MinString);
					int Max = std::stoi(MaxString);
					a_Item.m_ItemCount = a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Max - Min) + Min;
				}
				else if (Type == "Binomial")
				{
					auto NString = a_Function.m_Parameter.at("N");
					auto PString = a_Function.m_Parameter.at("P");
					if (NString.empty() || PString.empty())
					{
						LOGWARNING("Missing Range parameters on SetCount Function. dropping function!");
						break;
					}
					int N = std::stoi(NString);
					float P = std::stof(PString);
					std::default_random_engine Generator(a_Noise.GetSeed());
					std::binomial_distribution<int> Dist(N, P);
					std::vector<int> Values;
					for (int i = 0; i < N; i++)
					{
						Values[i] = Dist(Generator);
					}
					a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (N);
				}
			}
			else
			{
				a_Item.m_ItemCount = std::stoi(Count);
			}
			break;
		}
		case LootTable::eFunctionType::SetDamage:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetLore:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetName:  // All
		{
			break;
		}
		default:
		{
			LOGWARNING("Function type if not supported for this entity, dropping function!");
			break;
		}
	}
}



// Killed entity
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, const cWorld * a_World, const cEntity * a_Entity, const cNoise & a_Noise, const cUUID * a_Player)
{
	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::ApplyBonus:  // Block Break, killed Entity
		{
			auto Enchantment = cEnchantments::StringToEnchantmentID(a_Function.m_Parameter.at("Enchantment"));
			auto Formula = a_Function.m_Parameter.at("Formula");

			// Todo: get enchantment level of tool in player hand
			int Level = 1;

			if (Formula == "BinomialWithBonusCount")
			{
				// Binomial Probability Distribution with n=level + extra, p=probability
				auto ExtraString = a_Function.m_Parameter.at("Extra");
				int Extra = std::stoi(ExtraString);
				auto ProbString = a_Function.m_Parameter.at("Probability");
				float Probability = std::stof(ProbString);

				std::default_random_engine Generator(a_Noise.GetSeed());
				std::binomial_distribution<int> Dist(Level + Extra, Probability);
				std::vector<int> Values;
				for (int i = 0; i < Level + Extra; i++)
				{
					Values[i] = Dist(Generator);
				}
				a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Level + Extra);
			}
			else if (Formula == "UniformBonusCount")
			{
				// prob = (0, level * BonusMultiplier)
				auto BonusMultiplierString = a_Function.m_Parameter.at("BonusMultiplier");
				int BonusMultiplier = std::stoi(BonusMultiplierString);
				a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Level * BonusMultiplier);
			}
			else if (Formula == "OreDrops")
			{
				// Count *= (max(0; random(0..Level + 2) - 1) + 1)
				a_Item.m_ItemCount *= std::max(0, a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Level + 2) - 1) + 1;
			}
			break;
		}
		case LootTable::eFunctionType::CopyNbt:  // Block Entity, Killed Entity
		{
			LOGWARNING("NBT for items is not yet supported, Dropping function!");
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:  // All
		{
			break;
		}
		case LootTable::eFunctionType::FurnaceSmelt:  // Item, Block break, Entity Kill
		{
			auto Recipes = cRoot::Get()->GetFurnaceRecipe();
			auto NewItem = Recipes->GetRecipeFrom(a_Item)->Out;
			a_Item.m_ItemType = NewItem->m_ItemType;
			a_Item.m_ItemDamage = NewItem->m_ItemDamage;
			break;
		}
		case LootTable::eFunctionType::LimitCount:  // All
		{
			break;
		}
		case LootTable::eFunctionType::LootingEnchant:  // Entity killed
		{
			break;
		}
		case LootTable::eFunctionType::SetAttributes:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetCount:  // All
		{
			auto Count = a_Function.m_Parameter.at("Count");
			// Has no specified range
			if (Count.empty())
			{
				auto Type = a_Function.m_Parameter.at("Type");
				if (Type == "Uniform")
				{
					auto MinString = a_Function.m_Parameter.at("Min");
					auto MaxString = a_Function.m_Parameter.at("Max");
					if (MinString.empty() || MaxString.empty())
					{
						LOGWARNING("Missing Range parameters on SetCount Function. dropping function!");
						break;
					}
					int Min = std::stoi(MinString);
					int Max = std::stoi(MaxString);
					a_Item.m_ItemCount = a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (Max - Min) + Min;
				}
				else if (Type == "Binomial")
				{
					auto NString = a_Function.m_Parameter.at("N");
					auto PString = a_Function.m_Parameter.at("P");
					if (NString.empty() || PString.empty())
					{
						LOGWARNING("Missing Range parameters on SetCount Function. dropping function!");
						break;
					}
					int N = std::stoi(NString);
					float P = std::stof(PString);
					std::default_random_engine Generator(a_Noise.GetSeed());
					std::binomial_distribution<int> Dist(N, P);
					std::vector<int> Values;
					for (int i = 0; i < N; i++)
					{
						Values[i] = Dist(Generator);
					}
					a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) % (N);
				}
			}
			else
			{
				a_Item.m_ItemCount = std::stoi(Count);
			}
			break;
		}
		case LootTable::eFunctionType::SetDamage:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetLore:  // All
		{
			break;
		}
		case LootTable::eFunctionType::SetName:  // All
		{
			break;
		}
		default:
		{
			LOGWARNING("Function type if not supported for this entity, dropping function!");
			break;
		}
	}
}




