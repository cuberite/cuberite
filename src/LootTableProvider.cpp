
// cLootTableProvider.cpp

#include "LootTableProvider.h"

#include "OSSupport/File.h"
#include "JsonUtils.h"
#include "json/json.h"
#include "BlockEntities/BlockEntityWithItems.h"
#include "Noise/Noise.h"
#include "Entities/Player.h"

namespace LootTable
{
	/** Gets the Type::eType from String. Defaults to Generic */
	Type::eType eType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "Empty") == 0)
		{
			return Type::eType::Empty;
		}
		else if (NoCaseCompare(a_Type, "Entity") == 0)
		{
			return Type::eType::Entity;
		}
		else if (NoCaseCompare(a_Type, "Block") == 0)
		{
			return Type::eType::Block;
		}
		else if (NoCaseCompare(a_Type, "Chest") == 0)
		{
			return Type::eType::Chest;
		}
		else if (NoCaseCompare(a_Type, "Fishing") == 0)
		{
			return Type::eType::Fishing;
		}
		else if (NoCaseCompare(a_Type, "Gift") == 0)
		{
			return Type::eType::Gift;
		}
		else if (NoCaseCompare(a_Type, "AdvancementReward") == 0)
		{
			return Type::eType::AdvancementReward;
		}
		else if (NoCaseCompare(a_Type, "Barter") == 0)
		{
			return Type::eType::Barter;
		}
		else if (NoCaseCompare(a_Type, "Command") == 0)
		{
			return Type::eType::Command;
		}
		else if (NoCaseCompare(a_Type, "Selector") == 0)
		{
			return Type::eType::Selector;
		}
		else if (NoCaseCompare(a_Type, "AdvancementEntity") == 0)
		{
			return Type::eType::AdvancementEntity;
		}
		else
		{
			LOGWARNING("Unknown Loot table type provided: %s. defaulting to Generic", a_Type.c_str());
			return Type::eType::Generic;
		}
	}





	/** Gets the eConditionType from String. Defaults to None */
	ConditionType::eConditionType eConditionType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "Alternative") == 0)
		{
			return ConditionType::eConditionType::Alternative;
		}
		else if (NoCaseCompare(a_Type, "BlockStateProperty") == 0)
		{
			return ConditionType::eConditionType::BlockStateProperty;
		}
		else if (NoCaseCompare(a_Type, "DamageSourceProperties") == 0)
		{
			return ConditionType::eConditionType::DamageSourceProperties;
		}
		else if (NoCaseCompare(a_Type, "EntityProperties") == 0)
		{
			return ConditionType::eConditionType::EntityProperties;
		}
		else if (NoCaseCompare(a_Type, "EntityScores") == 0)
		{
			return ConditionType::eConditionType::EntityScores;
		}
		else if (NoCaseCompare(a_Type, "Inverted") == 0)
		{
			return ConditionType::eConditionType::Inverted;
		}
		else if (NoCaseCompare(a_Type, "KilledByPlayer") == 0)
		{
			return ConditionType::eConditionType::KilledByPlayer;
		}
		else if (NoCaseCompare(a_Type, "LocationCheck") == 0)
		{
			return ConditionType::eConditionType::LocationCheck;
		}
		else if (NoCaseCompare(a_Type, "MatchTool") == 0)
		{
			return ConditionType::eConditionType::MatchTool;
		}
		else if (NoCaseCompare(a_Type, "RandomChance") == 0)
		{
			return ConditionType::eConditionType::RandomChance;
		}
		else if (NoCaseCompare(a_Type, "RandomChanceWithLooting") == 0)
		{
			return ConditionType::eConditionType::RandomChanceWithLooting;
		}
		else if (NoCaseCompare(a_Type, "Reference") == 0)
		{
			return ConditionType::eConditionType::Reference;
		}
		else if (NoCaseCompare(a_Type, "SurvivesExplosion") == 0)
		{
			return ConditionType::eConditionType::SurvivesExplosion;
		}
		else if (NoCaseCompare(a_Type, "TableBonus") == 0)
		{
			return ConditionType::eConditionType::TableBonus;
		}
		else if (NoCaseCompare(a_Type, "TimeCheck") == 0)
		{
			return ConditionType::eConditionType::TimeCheck;
		}
		else if (NoCaseCompare(a_Type, "WeatherCheck") == 0)
		{
			return ConditionType::eConditionType::WeatherCheck;
		}
		else
		{
			LOGWARNING("Unknown loot table condition provided: %s. Using no condition", a_Type.c_str());
			return ConditionType::eConditionType::None;
		}
	}




	FunctionType::eFunctionType eFunctionType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "ApplyBonus") == 0)
		{
			return FunctionType::eFunctionType::ApplyBonus;
		}
		else if (NoCaseCompare(a_Type, "CopyName") == 0)
		{
			return FunctionType::eFunctionType::CopyName;
		}
		else if (NoCaseCompare(a_Type, "CopyNbt") == 0)
		{
			return FunctionType::eFunctionType::CopyNbt;
		}
		else if (NoCaseCompare(a_Type, "CopyState") == 0)
		{
			return FunctionType::eFunctionType::CopyState;
		}
		else if (NoCaseCompare(a_Type, "EnchantRandomly") == 0)
		{
			return FunctionType::eFunctionType::EnchantRandomly;
		}
		else if (NoCaseCompare(a_Type, "EnchantWithLevels") == 0)
		{
			return FunctionType::eFunctionType::EnchantWithLevels;
		}
		else if (NoCaseCompare(a_Type, "ExplorationMap") == 0)
		{
			return FunctionType::eFunctionType::ExplorationMap;
		}
		else if (NoCaseCompare(a_Type, "ExplosionDecay") == 0)
		{
			return FunctionType::eFunctionType::ExplosionDecay;
		}
		else if (NoCaseCompare(a_Type, "FurnaceSmelt") == 0)
		{
			return FunctionType::eFunctionType::FurnaceSmelt;
		}
		else if (NoCaseCompare(a_Type, "FillPlayerHead") == 0)
		{
			return FunctionType::eFunctionType::FillPlayerHead;
		}
		else if (NoCaseCompare(a_Type, "LimitCount") == 0)
		{
			return FunctionType::eFunctionType::LimitCount;
		}
		else if (NoCaseCompare(a_Type, "LootingEnchant") == 0)
		{
			return FunctionType::eFunctionType::LootingEnchant;
		}
		else if (NoCaseCompare(a_Type, "SetAttributes") == 0)
		{
			return FunctionType::eFunctionType::SetAttributes;
		}
		else if (NoCaseCompare(a_Type, "SetContents") == 0)
		{
			return FunctionType::eFunctionType::SetContents;
		}
		else if (NoCaseCompare(a_Type, "SetCount") == 0)
		{
			return FunctionType::eFunctionType::SetCount;
		}
		else if (NoCaseCompare(a_Type, "SetDamage") == 0)
		{
			return FunctionType::eFunctionType::SetDamage;
		}
		else if (NoCaseCompare(a_Type, "SetLootTable") == 0)
		{
			return FunctionType::eFunctionType::SetLootTable;
		}
		else if (NoCaseCompare(a_Type, "SetLore") == 0)
		{
			return FunctionType::eFunctionType::SetLore;
		}
		else if (NoCaseCompare(a_Type, "SetName") == 0)
		{
			return FunctionType::eFunctionType::SetName;
		}
		else if (NoCaseCompare(a_Type, "SetNbt") == 0)
		{
			return FunctionType::eFunctionType::SetNbt;
		}
		else if (NoCaseCompare(a_Type, "SetStewEffect") == 0)
		{
			return FunctionType::eFunctionType::SetStewEffect;
		}
		else if (NoCaseCompare(a_Type, "None") == 0)
		{
			return FunctionType::eFunctionType::None;
		}
		else
		{
			LOGWARNING("Unknown loot table function provided: %s. Using empty function", a_Type.c_str());
			return FunctionType::eFunctionType::None;
		}
	}




	/** Gets the ePoolEntryType from String. Defaults to Empty */
	PoolEntryType::ePoolEntryType ePoolEntryType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "Item") == 0)
		{
			return PoolEntryType::ePoolEntryType::Item;
		}
		else if (NoCaseCompare(a_Type, "Tag") == 0)
		{
			return PoolEntryType::ePoolEntryType::Tag;
		}
		else if (NoCaseCompare(a_Type, "LootTable") == 0)
		{
			return PoolEntryType::ePoolEntryType::LootTable;
		}
		else if (NoCaseCompare(a_Type, "Group") == 0)
		{
			return PoolEntryType::ePoolEntryType::Group;
		}
		else if (NoCaseCompare(a_Type, "Alternatives") == 0)
		{
			return PoolEntryType::ePoolEntryType::Alternatives;
		}
		else if (NoCaseCompare(a_Type, "Sequence") == 0)
		{
			return PoolEntryType::ePoolEntryType::Sequence;
		}
		else if (NoCaseCompare(a_Type, "Dynamic") == 0)
		{
			return PoolEntryType::ePoolEntryType::Dynamic;
		}
		else if (NoCaseCompare(a_Type, "Empty") == 0)
		{
			return PoolEntryType::ePoolEntryType::Empty;
		}
		else
		{
			LOGWARNING("Unknown loot table pool entry type provided: %s. Defaulting to Empty", a_Type.c_str());
			return PoolEntryType::ePoolEntryType::Empty;
		}
	}




	/** Gets the eChestType from String as of 1.16.2 */
	ChestType::eChestType eChestType(const AString & a_Type)
	{
		if (NoCaseCompare(a_Type, "AbandonedMineshaft") == 0)
		{
			return ChestType::eChestType::AbandonedMineshaft;
		}
		else if (NoCaseCompare(a_Type, "BuriedTreasure") == 0)
		{
			return ChestType::eChestType::BuriedTreasure;
		}
		else if (NoCaseCompare(a_Type, "DesertPyramid") == 0)
		{
			return ChestType::eChestType::DesertPyramid;
		}
		else if (NoCaseCompare(a_Type, "EndCityTreasure") == 0)
		{
			return ChestType::eChestType::EndCityTreasure;
		}
		else if (NoCaseCompare(a_Type, "IglooChest") == 0)
		{
			return ChestType::eChestType::IglooChest;
		}
		else if (NoCaseCompare(a_Type, "JungleTemple") == 0)
		{
			return ChestType::eChestType::JungleTemple;
		}
		else if (NoCaseCompare(a_Type, "JungleTempleDispenser") == 0)
		{
			return ChestType::eChestType::JungleTempleDispenser;
		}
		else if (NoCaseCompare(a_Type, "NetherBridge") == 0)
		{
			return ChestType::eChestType::NetherBridge;
		}
		else if (NoCaseCompare(a_Type, "PillagerOutpost") == 0)
		{
			return ChestType::eChestType::PillagerOutpost;
		}
		else if (NoCaseCompare(a_Type, "ShipwreckMap") == 0)
		{
			return ChestType::eChestType::ShipwreckMap;
		}
		else if (NoCaseCompare(a_Type, "ShipwreckSupply") == 0)
		{
			return ChestType::eChestType::ShipwreckSupply;
		}
		else if (NoCaseCompare(a_Type, "ShipwreckTreasure") == 0)
		{
			return ChestType::eChestType::ShipwreckTreasure;
		}
		else if (NoCaseCompare(a_Type, "SimpleDungeon") == 0)
		{
			return ChestType::eChestType::SimpleDungeon;
		}
		else if (NoCaseCompare(a_Type, "SpawnBonusChest") == 0)
		{
			return ChestType::eChestType::SpawnBonusChest;
		}
		else if (NoCaseCompare(a_Type, "StrongholdCorridor") == 0)
		{
			return ChestType::eChestType::StrongholdCorridor;
		}
		else if (NoCaseCompare(a_Type, "StrongholdCrossing") == 0)
		{
			return ChestType::eChestType::StrongholdCrossing;
		}
		else if (NoCaseCompare(a_Type, "StrongholdLibrary") == 0)
		{
			return ChestType::eChestType::StrongholdLibrary;
		}
		else if (NoCaseCompare(a_Type, "UnderwaterRuinBig") == 0)
		{
			return ChestType::eChestType::UnderwaterRuinBig;
		}
		else if (NoCaseCompare(a_Type, "UnderwaterRuinSmall") == 0)
		{
			return ChestType::eChestType::UnderwaterRuinSmall;
		}

			/* Village chest types */

		else if (NoCaseCompare(a_Type, "VillageArmorer") == 0)
		{
			return ChestType::eChestType::VillageArmorer;
		}
		else if (NoCaseCompare(a_Type, "VillageButcher") == 0)
		{
			return ChestType::eChestType::VillageButcher;
		}
		else if (NoCaseCompare(a_Type, "VillageCartographer") == 0)
		{
			return ChestType::eChestType::VillageCartographer;
		}
		else if (NoCaseCompare(a_Type, "VillageDesertHouse") == 0)
		{
			return ChestType::eChestType::VillageDesertHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageFisher") == 0)
		{
			return ChestType::eChestType::VillageFisher;
		}
		else if (NoCaseCompare(a_Type, "VillageFletcher") == 0)
		{
			return ChestType::eChestType::VillageFletcher;
		}
		else if (NoCaseCompare(a_Type, "VillageMason") == 0)
		{
			return ChestType::eChestType::VillageMason;
		}
		else if (NoCaseCompare(a_Type, "VillagePlainsHouse") == 0)
		{
			return ChestType::eChestType::VillagePlainsHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageSavannaHouse") == 0)
		{
			return ChestType::eChestType::VillageSavannaHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageShepherd") == 0)
		{
			return ChestType::eChestType::VillageShepherd;
		}
		else if (NoCaseCompare(a_Type, "VillageSnowyHouse") == 0)
		{
			return ChestType::eChestType::VillageSnowyHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageTaigaHouse") == 0)
		{
			return ChestType::eChestType::VillageTaigaHouse;
		}
		else if (NoCaseCompare(a_Type, "VillageTannery") == 0)
		{
			return ChestType::eChestType::VillageTannery;
		}
		else if (NoCaseCompare(a_Type, "VillageTemple") == 0)
		{
			return ChestType::eChestType::VillageTemple;
		}
		else if (NoCaseCompare(a_Type, "VillageToolsmith") == 0)
		{
			return ChestType::eChestType::VillageToolsmith;
		}
		else if (NoCaseCompare(a_Type, "VillageWeaponsmith") == 0)
		{
			return ChestType::eChestType::VillageWeaponsmith;
		}

			/* Village chest types end */

		else if (NoCaseCompare(a_Type, "WoodlandMansion") == 0)
		{
			return ChestType::eChestType::WoodlandMansion;
		}
		else
		{
			return ChestType::eChestType::None;
		}
	}





	/** Gets the eMonsterType from String. Defaults to Giant -> no loot */
	enum eMonsterType eMonsterType(const AString & a_Type)
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





cLootTableProvider::cLootTableProvider(AString & a_Path)
{
	LOG("Loading loot tables...");
	// Load default loot tables
	for (auto & FileName: LootTable::FileNames)
	{
		auto FilePath = Printf(FileName.c_str(), cFile::PathSeparator());
		auto FilePathWithPrefix = Printf("LootTables%c%s", cFile::PathSeparator(), FilePath.c_str());
		auto Data = cFile::ReadWholeFile(FilePathWithPrefix);
		if (Data != "")
		{
			LoadLootTable(Data, const_cast<AString &>(FileName));
		}
		else
		{
			// Todo: write better error message
			LOGERROR("Could not find default loot table: %s! "
			"Please make sure the file is readable or download them from cuberite.org", FilePathWithPrefix.c_str());
		}
	}

	// Check for custom tables
	for (auto & FileName: LootTable::FileNames)
	{
		auto FilePath = Printf(FileName.c_str(), cFile::PathSeparator());
		auto FilePathWithPrefix = Printf("LootTables%c%s", cFile::PathSeparator(), FilePath.c_str());
		auto FilePathWithWorld = Printf("%s%c%s", a_Path.c_str(), cFile::PathSeparator(), FilePathWithPrefix.c_str());
		auto Data = cFile::ReadWholeFile(FilePathWithWorld);
		if (Data != "")
		{
			LOG("Found custom loot table: %s", FilePathWithWorld);
			LoadLootTable(Data, const_cast<AString &>(FileName));
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
// cLootTableProvider


const cLootTable cLootTableProvider::m_EmptyLootTable = cLootTable();





void cLootTableProvider::LoadLootTable(const AString & a_String, AString & a_Type)
{
	AString ErrorMessage;
	Json::Value JsonObject;
	JsonUtils::ParseString(a_String, JsonObject, & ErrorMessage);
	if ((ErrorMessage != "") || !JsonObject.isObject())
	{
		LOGERROR(ErrorMessage);
	}
	else
	{
		switch (LootTable::eType(LootTable::NamespaceConverter(JsonObject["type"].as<AString>())))
		{
			case LootTable::Type::Chest:
			{
				auto Name = a_Type;
				LootTable::Replace(Name, "Chests%c", "");
				const auto ChestType = LootTable::eChestType(Name);
				m_ChestLootTables[ChestType] = new cLootTable(JsonObject);

				break;
			}
			default:
			{
				LOGWARNING("This loot table type is not supported: %s", JsonObject["type"].as<AString>().c_str());
				break;
			}
		}
	}
}





const cLootTable * cLootTableProvider::GetLootTable(const AString & a_Name)
{
	auto Data = StringSplitAndTrim(a_Name,"|");

	if (Data.size() != 2)
	{
		LOGWARNING("Got ill formatted string: \"%s\" to look up a loot table.\n"
			"Please use Type|Subtype. Returning empty loot table!", a_Name.c_str());
		return & m_EmptyLootTable;
	}

	auto Type = LootTable::eType(Data[0]);

	switch (Type)
	{
		case LootTable::Type::Chest: return GetLootTable(LootTable::eChestType(Data[1]));
		default:
		{
			LOGWARNING("Trying to use unsupported or unknown loot table type: %s", Data[1].c_str());
			return & m_EmptyLootTable;
		}
	}
}





const cLootTable * cLootTableProvider::GetLootTable(const LootTable::ChestType::eChestType a_Type)
{
	return m_ChestLootTables.at(a_Type);
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
// cLootTable


cLootTable::cLootTable()
{
	m_Type = LootTable::Type::Empty;
	m_LootTableFunctions = cLootTableFunctionVector();
	m_LootTablePools = cLootTablePoolVector();
}





cLootTable::cLootTable(const Json::Value & a_Description)
{
	m_LootTablePools = cLootTablePoolVector();
	m_LootTableFunctions = cLootTableFunctionVector();

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
				m_LootTableFunctions.push_back(ReadLootTableFunction(FunctionsObject[FunctionIndex]));
			}
		}
	}
}





cLootTable::cLootTable(cLootTable & a_Other)
{
	m_Type = a_Other.m_Type;
	m_LootTableFunctions = a_Other.m_LootTableFunctions;
	m_LootTablePools = a_Other.m_LootTablePools;
}





bool cLootTable::FillWithLoot(cBlockEntityWithItems * a_BlockEntity, cPlayer * a_Player) const
{
	auto & ItemGrid = a_BlockEntity->GetContents();
	auto Seed = a_BlockEntity->GetWorld()->GetGenerator().GetSeed();
	auto Noise = cNoise(Seed);
	auto Items = GetItems(Noise, a_BlockEntity->GetPos(), a_Player, nullptr);
	return false;
}





std::vector<cItem> cLootTable::GetItems(cNoise & a_Noise, const Vector3i & a_Pos, cPlayer * a_Player, cEntity * a_Entity) const
{
	auto Items = std::vector<cItem>();
	for (const auto & Pool : m_LootTablePools)
	{

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
				PoolRolls = cLootTablePoolRolls(Rolls.asInt());
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
					PoolRolls = cLootTablePoolRolls(Min);
				}
				PoolRolls = cLootTablePoolRolls(-1, Min, Max);
			}
		}
		else if (NoCaseCompare(PoolElement, "bonus_rolls") == 0)
		{
			auto Rolls = a_Value[PoolElement];
			if (Rolls.isInt())
			{
				PoolRolls = cLootTablePoolRolls(Rolls.asInt());
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
				PoolBonusRolls = cLootTablePoolRolls(-1, Min, Max);
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
	enum LootTable::FunctionType::eFunctionType Type;
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
	LootTable::ConditionType::eConditionType Type;
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
		case LootTable::ConditionType::eConditionType::Alternative:
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
		case LootTable::ConditionType::eConditionType::Inverted:
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
		case LootTable::ConditionType::eConditionType::Reference:
		{
			LOGWARNING("Type \"Reference\" for loot table conditions is not yet supported. Dropping condition");
			return cLootTableCondition();
		}
		case LootTable::ConditionType::eConditionType::SurvivesExplosion:
		{
			return cLootTableCondition(Type, Parameter, SubConditions);
		}
		case LootTable::ConditionType::eConditionType::None:
		{
			return cLootTableCondition();
		}
		case LootTable::ConditionType::eConditionType::BlockStateProperty:
		case LootTable::ConditionType::eConditionType::DamageSourceProperties:
		case LootTable::ConditionType::eConditionType::EntityProperties:
		case LootTable::ConditionType::eConditionType::EntityScores:
		case LootTable::ConditionType::eConditionType::KilledByPlayer:
		case LootTable::ConditionType::eConditionType::LocationCheck:
		case LootTable::ConditionType::eConditionType::MatchTool:
		case LootTable::ConditionType::eConditionType::RandomChance:
		case LootTable::ConditionType::eConditionType::RandomChanceWithLooting:
		case LootTable::ConditionType::eConditionType::TableBonus:
		case LootTable::ConditionType::eConditionType::TimeCheck:
		case LootTable::ConditionType::eConditionType::WeatherCheck:
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
	enum LootTable::PoolEntryType::ePoolEntryType Type;

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
		Type = LootTable::PoolEntryType::ePoolEntryType::Empty;
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
				case LootTable::PoolEntryType::ePoolEntryType::Item:
				{
					StringToItem(LootTable::NamespaceConverter(a_Value[EntryParameter].asString()), Item);
					break;
				}
				case LootTable::PoolEntryType::ePoolEntryType::Tag:
				case LootTable::PoolEntryType::ePoolEntryType::LootTable:
				case LootTable::PoolEntryType::ePoolEntryType::Dynamic:
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
				case LootTable::PoolEntryType::ePoolEntryType::Group:
				case LootTable::PoolEntryType::ePoolEntryType::Alternatives:
				case LootTable::PoolEntryType::ePoolEntryType::Sequence:
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
		case LootTable::PoolEntryType::ePoolEntryType::Item:
		{
			return cLootTablePoolEntry(Conditions, Functions, Type, Item, Weight, Quality);
		}

		case LootTable::PoolEntryType::ePoolEntryType::Tag:
		case LootTable::PoolEntryType::ePoolEntryType::LootTable:
		case LootTable::PoolEntryType::ePoolEntryType::Dynamic:
		{
			return cLootTablePoolEntry(Conditions, Functions, Type, Name, Expand, Weight, Quality);
		}

		case LootTable::PoolEntryType::ePoolEntryType::Group:
		case LootTable::PoolEntryType::ePoolEntryType::Alternatives:
		case LootTable::PoolEntryType::ePoolEntryType::Sequence:
		{
			return cLootTablePoolEntry(Conditions, Functions, Type, Children, Weight, Quality);
		}

		case LootTable::PoolEntryType::ePoolEntryType::Empty: return cLootTablePoolEntry();
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





std::vector<cItem> cLootTable::GetItems(const cLootTablePool & a_Pool, cNoise & a_Noise, Vector3i & a_Pos, cPlayer * a_Player, cEntity * a_Entity)
{
	auto Items = std::vector<cItem>();
	if (!ConditionsApply(a_Pool.m_Conditions, a_Player))
	{
		return Items;
	}

	// Steady Roll
	if (a_Pool.m_Rolls.m_Roll > -1)
	{
		for (int i = 0; i < a_Pool.m_Rolls.m_Roll; i++)
		{
			for (const auto & Entry : a_Pool.m_Entries)
			{
				auto NewItems = GetItems(Entry, a_Noise, a_Pos, a_Player, a_Entity);
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
				auto NewItems = GetItems(Entry, a_Noise, a_Pos, a_Player, a_Entity);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			}
		}
	}
	return Items;
}





std::vector<cItem> cLootTable::GetItems(const cLootTablePoolEntry & a_Entry, cNoise & a_Noise, Vector3i & a_Pos, cPlayer * a_Player, cEntity * a_Entity)
{
	// Todo: add luck here, when it's implemented
	auto Items = std::vector<cItem>();
	switch (a_Entry.m_Type)
	{
		case LootTable::PoolEntryType::ePoolEntryType::Item:
		{
			Items.push_back(a_Entry.m_Item);
			break;
		}
		case LootTable::PoolEntryType::ePoolEntryType::Tag:
		{
			// Todo: check what names are used
			// Todo: add loot tables for that
			// Todo: return item(s)
			break;
		}
		case LootTable::PoolEntryType::ePoolEntryType::LootTable:
		{
			// Todo - add loot table lookup
			break;
		}
		case LootTable::PoolEntryType::ePoolEntryType::Group:
		{
			for (const auto & Child : a_Entry.m_Children)
			{
				auto NewItems = GetItems(Child, a_Noise, a_Pos, a_Player, a_Entity);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			}
			break;
		}
		case LootTable::PoolEntryType::ePoolEntryType::Alternatives:
		{
			// Todo - Choose 1
			auto Child = a_Entry.m_Children[0];
			auto NewItems = GetItems(Child, a_Noise, a_Pos, a_Player, a_Entity);
			Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			break;
		}
		case LootTable::PoolEntryType::ePoolEntryType::Sequence:
			break;
			// TODO
		case LootTable::PoolEntryType::ePoolEntryType::Dynamic:
			break;
			// TODO
		case LootTable::PoolEntryType::ePoolEntryType::Empty:
		{
			Items.push_back(cItem(E_BLOCK_AIR));
			break;
		}
	}
	return Items;
}





bool cLootTable::ConditionsApply(const cLootTableConditionVector & a_Conditions, cPlayer * a_Player, cEntity * a_Entity)
{
	bool Success = true;
	for (const auto & Condition : a_Conditions)
	{
		Success &= ConditionApplies(Condition, a_Player, a_Entity);
	}
	return Success;
}





bool cLootTable::ConditionApplies(const cLootTableCondition & a_Condition, cPlayer * a_Player, cEntity * a_Entity)
{
	switch (a_Condition.m_Type)
	{
		case LootTable::ConditionType::eConditionType::None:
		{
			return true;
		}
		case LootTable::ConditionType::eConditionType::Alternative:
		{
			bool Success = false;
			for (const auto & SubCondition: a_Condition.m_SubConditions)
			{
				Success |= ConditionApplies(SubCondition, a_Player, a_Entity);
			}
			return Success;
		}
		case LootTable::ConditionType::eConditionType::BlockStateProperty:
		{
			// TODO
			LOGWARNING("Loot table condition \"BlockStateProperty\" is not yet supported. Assuming it's true.");
			return true;
		}
		case LootTable::ConditionType::eConditionType::DamageSourceProperties:
		case LootTable::ConditionType::eConditionType::EntityProperties:
		case LootTable::ConditionType::eConditionType::EntityScores:
		case LootTable::ConditionType::eConditionType::Inverted:
		case LootTable::ConditionType::eConditionType::KilledByPlayer:
		case LootTable::ConditionType::eConditionType::LocationCheck:
		case LootTable::ConditionType::eConditionType::MatchTool:
		case LootTable::ConditionType::eConditionType::RandomChance:
		case LootTable::ConditionType::eConditionType::RandomChanceWithLooting:
		case LootTable::ConditionType::eConditionType::Reference:
		{
			// TODO
			return true;
		}
		case LootTable::ConditionType::eConditionType::SurvivesExplosion:
		case LootTable::ConditionType::eConditionType::TableBonus:
		case LootTable::ConditionType::eConditionType::TimeCheck:
		case LootTable::ConditionType::eConditionType::WeatherCheck:
		default: return true;
	}
}



