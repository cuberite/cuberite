
// cLootTableProvider.cpp

#include "LootTableProvider.h"

#include "OSSupport/File.h"
#include "JsonUtils.h"
#include "json/json.h"
#include "BlockEntities/BlockEntityWithItems.h"

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
			LOGWARNING("Unknown Loot table type provided: %s. defaulting to Generic", a_Type.c_str());
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
			LOGWARNING("Unknown loot table condition provided: %s. Using no condition", a_Type.c_str());
			return eConditionType::None;
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
			LOGWARNING("Unknown loot table pool entry type provided: %s. Defaulting to Empty", a_Type.c_str());
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
	bool Replace(AString & a_String, const AString & a_From, const AString & a_To)
	{
		size_t start_pos = a_String.find(a_From);
		if (start_pos == AString ::npos)
		{
			return false;
		}
		a_String.replace(start_pos, a_From.length(), a_To);
		return true;
	}




	/** Capitalizes any letter following a space */
	void Capitalize (AString & a_String)
	{
		bool cap = true;

		for (unsigned int i = 0; i <= a_String.length(); i++)
		{
			if (isalpha(a_String[i]) && cap)
			{
				a_String[i] = toupper(a_String[i]);
				cap = false;
			}
			else if (isspace(a_String[i]))
			{
				cap = true;
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





cLootTableProvider::cLootTableProvider(AString & a_Path):
	m_Path(a_Path)
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

	// Load if available
}





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
			case LootTable::eType::Chest:
			{
				LootTable::Replace(a_Type, "Chests%c", "");
				auto ChestType = LootTable::eChestType(LootTable::NamespaceConverter(a_Type));
				m_ChestLootTables.insert(std::pair<enum LootTable::eChestType, std::shared_ptr<cLootTable>>(ChestType, std::make_shared<cLootTable>(JsonObject)));
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





std::shared_ptr<cLootTable> cLootTableProvider::GetLootTable(const AString & a_Name) const
{
	auto Data = StringSplitAndTrim(a_Name,"|");

	if (Data.size() != 2)
	{
		LOGWARNING("Got ill formatted string: \"%s\" to look up a loot table.\n"
			"Please use Type|Subtype. Returning empty loot table!", a_Name.c_str());
		return std::make_shared<cLootTable>(cLootTable());
	}

	auto Type = LootTable::eType(Data[0]);

	switch (Type)
	{
		case LootTable::eType::Chest: return GetLootTable(LootTable::eChestType(Data[1]));
		default:
		{
			LOGWARNING("Trying to use unsupported or unknown loot table type: %s", Data[1].c_str());
			return std::make_shared<cLootTable>(cLootTable());
		}
	}
}





std::shared_ptr<cLootTable> cLootTableProvider::GetLootTable(const enum LootTable::eChestType a_Type) const
{
	return std::make_shared<cLootTable>();
}





cLootTable::cLootTable()
{
	m_Type = LootTable::eType::Empty;
	m_LootTableFunctions = cLootTableFunctionVector();
	m_LootTablePools = cLootTablePoolVector();
}





cLootTable::cLootTable(const Json::Value & a_Description)
{
	m_Type = LootTable::eType(LootTable::NamespaceConverter(a_Description["type"].asString()));
	if (m_Type == LootTable::eType::Empty)
	{
		return;
	}
}





cLootTable::cLootTable(const cLootTable & a_Other)
{
	m_Type = a_Other.m_Type;
	m_LootTablePools = a_Other.m_LootTablePools;
	m_LootTableFunctions = a_Other.m_LootTableFunctions;
}





cLootTable::~cLootTable()
{

}





bool cLootTable::FillWithLoot(cBlockEntityWithItems & a_BlockEntity)
{
	return false;
}




