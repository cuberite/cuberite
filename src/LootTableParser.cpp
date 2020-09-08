
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTableParser.h"
#include "Mobs/MonsterTypes.h"

namespace LootTable
{
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
		else if (NoCaseCompare(a_Type, "User") == 0)
		{
			return eType::User;
		}
		else
		{
			LOGWARNING("Unknown Loot table type provided: %s. defaulting to Generic", a_Type);
			return eType::Generic;
		}
	}





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





	AString NamespaceConverter(AString a_String)
	{
		ReplaceString(a_String, "minecraft:", "");
		ReplaceString(a_String, "_", " ");
		Capitalize(a_String);
		ReplaceString(a_String, " ", "");
		ReplaceString(a_String, "/", "|");
		return a_String;
	}





	void MinMaxRange(const Json::Value & a_Value, int & a_Min, int & a_Max)
	{
		if (a_Value.isMember("min"))
		{
			a_Min = a_Value["min"].asInt();
		}
		else if (a_Value.isMember("Min"))
		{
			a_Min = a_Value["Min"].asInt();
		}

		if (a_Value.isMember("max"))
		{
			a_Max = a_Value["max"].asInt();
		}
		else if (a_Value.isMember("Max"))
		{
			a_Max = a_Value["Max"].asInt();
		}
		if (a_Min > a_Max)
		{
			a_Max = a_Min;
		}
	}





	int MinMaxRand(const Json::Value & a_Value, const cNoise & a_Noise, const Vector3i & a_Pos, const int & a_Modifier)
	{
		int Min = 0, Max = 0;
		MinMaxRange(a_Value, Min, Max);
		return a_Noise.IntNoise3DInt(a_Pos * a_Modifier) % (Max - Min) + Min;
	}





	cLootTablePool ParsePool(const Json::Value & a_Value)
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
				PoolRolls = LootTable::ParseRolls(Rolls);
			}
			else if (
				(NoCaseCompare(PoolElement, "bonus_rolls") == 0) ||
				(NoCaseCompare(PoolElement, "BonusRolls") == 0))
			{
				auto Rolls = a_Value[PoolElement];
				PoolBonusRolls = ParseRolls(Rolls);
			}
			else if (NoCaseCompare(PoolElement, "entries") == 0)
			{
				auto Entries = a_Value[PoolElement];
				for (unsigned int EntryIndex = 0; EntryIndex < Entries.size(); EntryIndex++)
				{
					PoolEntries.push_back(ParsePoolEntry(Entries[EntryIndex]));
				}
			}
			else if (NoCaseCompare(PoolElement, "functions") == 0)
			{
				auto FunctionsObject = a_Value[PoolElement];
				for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
				{
					Functions.push_back(ParseFunction(FunctionsObject[FunctionIndex]));
				}
			}
			else if (NoCaseCompare(PoolElement, "conditions") == 0)
			{
				auto ConditionsObject = a_Value[PoolElement];
				for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
				{
					Conditions.push_back(ParseCondition(ConditionsObject[ConditionId]));
				}
			}
		}
		return cLootTablePool(PoolRolls, PoolEntries, Conditions);
	}





	cLootTableFunction ParseFunction(const Json::Value & a_Value)
	{
		enum LootTable::eFunctionType Type;
		cLootTableConditionVector Conditions;
		Json::Value Parameter = a_Value;
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
					Conditions.push_back(ParseCondition(ConditionsObject[ConditionId]));
				}
				Parameter.removeMember(FunctionInfo);  // Removes the conditions so the json is a bit smaller
			}
		}
		return cLootTableFunction(Type, Parameter, Conditions);
	}





	cLootTableCondition ParseCondition(const Json::Value & a_Value)
	{
		enum LootTable::eConditionType Type;

		// Type has to be known beforehand
		if (a_Value.isMember("condition"))
		{
			Type = LootTable::eConditionType(LootTable::NamespaceConverter(a_Value["condition"].asString()));
		}
		else if (a_Value.isMember("Condition"))
		{
			Type = LootTable::eConditionType(LootTable::NamespaceConverter(a_Value["Condition"].asString()));
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
				cLootTableConditionVector SubConditions;
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
					SubConditions.push_back(ParseCondition(Terms[i]));
				}
				return cLootTableCondition(Type, SubConditions);
				break;
			}
			case LootTable::eConditionType::Inverted:
			{
				Json::Value Term;
				cLootTableConditionVector SubConditions;
				if (a_Value.isMember("term"))
				{
					Term = a_Value["term"];
					SubConditions.push_back(ParseCondition(Term));
					return cLootTableCondition(Type, SubConditions);
				}
				else if (a_Value.isMember("Term"))
				{
					Term = a_Value["Term"];
					SubConditions.push_back(ParseCondition(Term));
					return cLootTableCondition(Type, SubConditions);
				}
				else
				{
					LOGWARNING("Loot table condition \"Inverted\" is missing sub-condition. Dropping condition!");
					return cLootTableCondition();
				}
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
			case LootTable::eConditionType::Reference:
			case LootTable::eConditionType::SurvivesExplosion:
			case LootTable::eConditionType::TableBonus:
			case LootTable::eConditionType::TimeCheck:
			case LootTable::eConditionType::WeatherCheck:
			{
				return cLootTableCondition(Type, a_Value);
			}
			default: return cLootTableCondition();
		}
	}





	cLootTablePoolEntry ParsePoolEntry(const Json::Value & a_Value)
	{
		cLootTableConditionVector Conditions;
		cLootTableFunctionVector Functions;
		enum LootTable::ePoolEntryType Type;

		cItem Item;
		AString Name;
		cLootTablePoolEntryVector Children;

		bool Expand = true;
		int Weight = 1;
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
			LOGWARNING("No loot table poll entry type provided - dropping entry");
			return cLootTablePoolEntry();
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
					Functions.push_back(ParseFunction(FunctionsObject[FunctionIndex]));
				}
			}
			else if (NoCaseCompare(EntryParameter, "conditions") == 0)
			{
				auto ConditionsObject = a_Value[EntryParameter];
				for (unsigned int ConditionId = 0; ConditionId < ConditionsObject.size(); ConditionId++)
				{
					Conditions.push_back(ParseCondition(ConditionsObject[ConditionId]));
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
							Children.push_back(ParsePoolEntry(ChildrenObject[ChildrenObjectId]));
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





	cLootTablePoolRolls ParseRolls(const Json::Value & a_Value)
	{
		if (a_Value.isInt())
		{
			return cLootTablePoolRolls(a_Value.asInt(), a_Value.asInt());
		}
		else
		{
			int Min = 0, Max = -1;
			for (auto & RollEntry: a_Value.getMemberNames())
			{
				if (NoCaseCompare(RollEntry, "min") == 0)
				{
					Min = a_Value[RollEntry].asInt();
				}
				else if (NoCaseCompare(RollEntry, "max") == 0)
				{
					Max = a_Value[RollEntry].asInt();
				}
			}
			if (Max == -1)
			{
				LOGWARNING("Missing maximum value in loot table pool - assuming steady roll");
				Max = Min;
			}
			return cLootTablePoolRolls(Min, Max);
		}
	}
}




