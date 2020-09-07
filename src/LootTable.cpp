

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LootTable.h"

#include "Entities/Player.h"
#include "FurnaceRecipe.h"
#include "ItemGrid.h"
#include "Mobs/MonsterTypes.h"
#include "Registries/ItemTags.h"
#include "Root.h"

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



	/** Gets the eFunctionType from String. Defaults to None */
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




	/** Moves a_String from the "minecraft:" namespace to cuberite namespace */
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
}

////////////////////////////////////////////////////////////////////////////////
// cLootTable


cLootTable::cLootTable()
{
	m_Type = LootTable::eType::Empty;
}





cLootTable::cLootTable(const Json::Value & a_Description, cWorld * a_World)
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
			for (unsigned int PoolId = 0; PoolId < Pools.size(); PoolId++)
			{
				ReadLootTablePool(Pools[PoolId]);
			}
		}
		else if (NoCaseCompare(RootId, "functions") == 0)
		{
			auto FunctionsObject = a_Description[RootId];
			for (unsigned int FunctionIndex = 0; FunctionIndex < FunctionsObject.size(); FunctionIndex++)
			{
				m_Functions.push_back(ReadLootTableFunction(FunctionsObject[FunctionIndex]));
			}
		}
	}
}





bool cLootTable::FillWithLoot(cItemGrid & a_ItemGrid, cWorld * a_World, const Vector3i & a_Pos, const UInt32 & a_Player) const
{
	auto Noise = cNoise(a_World->GetGenerator().GetSeed());

	// The player is killed and killer here because he influences both in the functions or conditions
	auto Items = GetItems(Noise, a_Pos, a_World, a_Player, a_Player);

	// Places items in a_ItemGrid
	int i = 0;  // This value is used for some more randomness
	for (auto & Item : Items)
	{
		cItem Copy;
		while (Item.m_ItemCount > 0)
		{
			Copy = Item.CopyOne();
			a_ItemGrid.AddItem(Copy, true, abs((Noise.IntNoise3DInt(a_Pos) * Noise.IntNoise1DInt(i)) % a_ItemGrid.GetNumSlots()));
			Item.m_ItemCount--;
			i++;
		}
	}
	return true;
}





cItems cLootTable::GetItems(const cNoise & a_Noise, const Vector3i & a_Pos, cWorld * a_World, const UInt32 & a_Killed, const UInt32 & a_Killer) const
{
	auto Items = cItems();
	int i = 0;
	for (const auto & Pool : m_LootTablePools)
	{
		auto NewItems = GetItems(Pool, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
		Items.insert(Items.end(), NewItems.begin(), NewItems.end());
		i++;
	}
	for (auto & Item : Items)
	{
		for (const auto & Function : m_Functions)
		{
			ApplyFunction(Function, Item, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
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
				int Min = 0, Max = -1;
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
				}
				if (Max == -1)
				{
					LOGWARNING("Missing maximum value in loot table pool - assuming steady roll");
					Max = Min;
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
				int Min = 0, Max = -1;
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
				}
				if (Max == -1)
				{
					LOGWARNING("Missing maximum value in loot table pool - assuming steady roll");
					Max = Min;
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
	}
	return cLootTableFunction(Type, a_Value, Conditions);
}





cLootTableCondition cLootTable::ReadLootTableCondition(const Json::Value & a_Value)
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
				SubConditions.push_back(ReadLootTableCondition(Terms[i]));
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
				SubConditions.push_back(ReadLootTableCondition(Term));
				return cLootTableCondition(Type, SubConditions);
			}
			else if (a_Value.isMember("Term"))
			{
				Term = a_Value["Term"];
				SubConditions.push_back(ReadLootTableCondition(Term));
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





cLootTablePoolEntry cLootTable::ReadLootTablePoolEntry(const Json::Value & a_Value)
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





cItems cLootTable::GetItems(const cLootTablePool & a_Pool, cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer)
{
	auto Items = cItems();
	if (!ConditionsApply(a_Pool.m_Conditions, a_World, a_Noise, a_Killed, a_Killer))
	{
		return Items;
	}

	int Luck = 0;
	/*  TODO: 07.09.2020 - Luck
	auto Callback = [&] (cPlayer & a_Player)
	{
		Luck = a_Player.GetLuck();
	};
	a_World->DoWithPlayerByUUID(a_Player, Callback);
	*/

	// Determines the total weight manipulated by the quality
	int TotalWeight = a_Pool.m_TotalWeight + Luck * a_Pool.m_TotalQuality;
	int TotalRolls = a_Pool.m_Rolls(a_Noise, a_Pos) + round(a_Pool.m_BonusRolls(a_Noise, a_Pos) * Luck);
	int EntryNum;

	for (int i = 0; i < TotalRolls; i++)
	{
		auto Rnd = a_Noise.IntNoise3DInt(a_Pos * 25 * a_Noise.IntNoise1DInt(i)) % TotalWeight;
		EntryNum = 0;
		while (Rnd > 0)
		{
			Rnd -= a_Pool.m_Entries[EntryNum].m_Weight + a_Pool.m_Entries[EntryNum].m_Quality * Luck;
			EntryNum = (EntryNum + 1) % a_Pool.m_Entries.size();
		}
		const auto & Entry = a_Pool.m_Entries[EntryNum];
		auto NewItems = GetItems(Entry, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
		Items.insert(Items.end(), NewItems.begin(), NewItems.end());
	}
	return Items;
}





cItems cLootTable::GetItems(const cLootTablePoolEntry & a_Entry, cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer)
{
	auto Items = cItems();

	if (!ConditionsApply(a_Entry.m_Conditions, a_World, a_Noise, a_Killed, a_Killer))
	{
		return Items;
	}

	switch (a_Entry.m_Type)
	{
		case LootTable::ePoolEntryType::Item:
		{
			try
			{
				Items.push_back(cItem(std::get<cItem>(a_Entry.m_Content)));
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
			}
			break;
		}
		case LootTable::ePoolEntryType::Tag:
		{
			AString Tag;
			try
			{
				Tag = std::get<AString>(a_Entry.m_Content);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
				break;
			}

			cItems TagItems = ItemTag::ItemTags(ItemTag::eItemTags(Tag));

			if (a_Entry.m_Expand)
			{
				Items.Add(TagItems[a_Noise.IntNoise3DInt(a_Pos * TagItems.size()) % TagItems.size()]);
			}
			else
			{
				Items.insert(Items.begin(), TagItems.begin(), TagItems.end());
			}
			break;
		}
		case LootTable::ePoolEntryType::LootTable:  // Grants items based on the specified loot table
		{
			AString Loot;
			// Usually this only contains item - but just case
			try
			{
				Loot = std::get<AString>(a_Entry.m_Content);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
				break;
			}

			auto NewItems = a_World->GetLootTableProvider()->GetLootTable(Loot)->GetItems(a_Noise, a_Pos, a_World, a_Killed, a_Killer);
			Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			break;
		}
		case LootTable::ePoolEntryType::Group:  // Grants all listed entries
		{
			try
			{
				for (const auto & Child : std::get<cLootTablePoolEntryVector>(a_Entry.m_Content))
				{
					auto NewItems = GetItems(Child, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
					Items.insert(Items.end(), NewItems.begin(), NewItems.end());
				}
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
			}
			break;
		}
		case LootTable::ePoolEntryType::Alternatives:  // Grants one of the listed entries
		{
			try
			{
				auto Children = std::get<cLootTablePoolEntryVector>(a_Entry.m_Content);
				auto ChildPos = a_Noise.IntNoise3DInt(a_Pos * Children.size()) % Children.size();
				auto NewItems = GetItems(Children[ChildPos], a_World, a_Noise, a_Pos, a_Killed, a_Killer);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry");
			}
			break;
		}
		case LootTable::ePoolEntryType::Sequence:  // Selects entries from Children until one is not granted.
		{
			cLootTablePoolEntryVector Children;
			try
			{
				Children = std::get<cLootTablePoolEntryVector>(a_Entry.m_Content);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table pool - dropping entry!");
				break;
			}
			cItems NewItems;
			int LockCont = 0;  // This is how many times the whole table is repeated to prevent infinite loops
			unsigned int ChildPos = 0;
			do
			{
				NewItems = GetItems(Children[ChildPos], a_World, a_Noise, a_Pos, a_Killed, a_Killer);
				Items.insert(Items.end(), NewItems.begin(), NewItems.end());
				ChildPos = (ChildPos + 1) % Children.size();
				if (ChildPos == Children.size() - 1)
				{
					LockCont++;
				}
			} while ((NewItems.size() != 0) && (LockCont < 10));

			break;
		}
		case LootTable::ePoolEntryType::Dynamic:  // Generates loot based on the block broken: chest content...
		{
			// TODO: 07.09.2020 - 12xx12
			LOGWARNING("Loot table pool entry type \"Dynamic\" is not supported yet - dropping entry!");
			break;
		}
		case LootTable::ePoolEntryType::Empty:
		{
			break;
		}
	}

	for (auto & Item : Items)
	{
		for (const auto & Function : a_Entry.m_Functions)
		{
			ApplyFunction(Function, Item, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
		}
	}
	return Items;
}





bool cLootTable::ConditionsApply(const cLootTableConditionVector & a_Conditions, cWorld * a_World, const cNoise & a_Noise, const UInt32 & a_Killed, const UInt32 & a_Killer)
{
	bool Success = true;
	for (const auto & Condition : a_Conditions)
	{
		Success &= ConditionApplies(Condition, a_World, a_Noise, a_Killed, a_Killer);
	}
	return Success;
}





bool cLootTable::ConditionApplies(const cLootTableCondition & a_Condition, cWorld * a_World, const cNoise & a_Noise, const UInt32 & a_Killed, const UInt32 & a_Killer)
{
	switch (a_Condition.m_Type)
	{
		case LootTable::eConditionType::None:
		{
			return true;
		}
		case LootTable::eConditionType::Alternative:
		{
			cLootTableConditionVector SubConditions;
			try
			{
				SubConditions = std::get<cLootTableConditionVector>(a_Condition.m_Parameter);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table condition - dropping entry");
				return true;;
			}
			bool Success = false;
			for (const auto & SubCondition: SubConditions)
			{
				Success |= ConditionApplies(SubCondition, a_World, a_Noise, a_Killed, a_Killer);
			}
			return Success;
		}
		case LootTable::eConditionType::BlockStateProperty:
		{
			// TODO: 06.09.2020 - Add when implemented - 12xx12
			LOGWARNING("Loot table condition \"BlockStateProperty\" is not yet supported. Assuming it's true.");
			return true;
		}
		case LootTable::eConditionType::DamageSourceProperties:
		{
			// TODO: 06.09.2020 - 12xx12
			LOGWARNING("Loot table condition \"AlternativeDamageSourceProperties\" is is not implemented. Assuming true!");
			return true;
		}
		case LootTable::eConditionType::EntityProperties:
		{
			// TODO: 07.09.2020 - 12xx12
			LOGWARNING("Loot table condition \"EntityProperties\" is not yet supported - assuming true");
			return true;
		}
		case LootTable::eConditionType::EntityScores:
		{
			// TODO: 07.09.2020 - 12xx12
			LOGWARNING("Loot table condition \"EntityScores\" is not yet supported - assuming true");
			return true;
		}
		case LootTable::eConditionType::Inverted:
		{
			cLootTableConditionVector SubCondition;
			try
			{
				SubCondition = std::get<cLootTableConditionVector>(a_Condition.m_Parameter);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table condition - dropping entry");
				return true;
			}
			return !ConditionApplies(SubCondition[0], a_World, a_Noise, a_Killed, a_Killer);
		}
		case LootTable::eConditionType::KilledByPlayer:
		{
			auto Callback = [&] (cEntity & a_Entity)
			{
				return (a_Entity.GetEntityType() == cEntity::etPlayer);
			};
			return a_World->DoWithEntityByID(a_Killer, Callback);
		}
		case LootTable::eConditionType::LocationCheck:
		{
			// TODO: 07.09.2020 - 12xx12
			LOGWARNING("Loot table condition \"LocationCheck\" is not yet supported - assuming true");
			return true;
		}
		case LootTable::eConditionType::MatchTool:
		{
			Json::Value Parameter;
			try
			{
				Parameter = std::get<Json::Value>(a_Condition.m_Parameter);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table condition - dropping entry");
				return true;
			}

			bool Res = true;
			/*
			count: Amount of the item.
				max: The maximum value.
				min: The minimum value.
			*/
			Json::Value CountObject;
			if (Parameter.isMember("count"))
			{
				CountObject = Parameter["count"];
			}
			else if (Parameter.isMember("Count"))
			{
				CountObject = Parameter["Count"];
			}

			if (CountObject.isInt())
			{
				int Count = CountObject.asInt();
				auto Callback = [&] (cEntity & a_Entity)
				{
					if (a_Entity.GetEntityType() != cEntity::etPlayer)
					{
						return true;
					}
					auto & Player = static_cast<cPlayer &>(a_Entity);
					return (Player.GetEquippedItem().m_ItemCount == Count);
				};
				Res &= a_World->DoWithEntityByID(a_Killer, Callback);
			}
			else if (CountObject.isObject())
			{
				int Min = 0, Max = 0;
				LootTable::MinMaxRange(CountObject, Min, Max);
				auto Callback = [&] (cEntity & a_Entity)
				{
					if (a_Entity.GetEntityType() != cEntity::etPlayer)
					{
					  return true;
					}
					auto & Player = static_cast<cPlayer &>(a_Entity);
					return ((Player.GetEquippedItem().m_ItemCount >= Min) && (Player.GetEquippedItem().m_ItemCount <= Min));
				};
				Res &= a_World->DoWithEntityByID(a_Killer, Callback);
			}
			/*
			durability: The durability of the item.
				max: The maximum value.
				min: The minimum value.
			*/
			Json::Value DurabilityObject;
			if (Parameter.isMember("durability"))
			{
				CountObject = Parameter["durability"];
			}
			else if (Parameter.isMember("´Durability"))
			{
				CountObject = Parameter["Durability"];
			}
			if (DurabilityObject.isInt())
			{
				int Durability = DurabilityObject.asInt();
				auto Callback = [&] (cEntity & a_Entity)
				{
					if (a_Entity.GetEntityType() != cEntity::etPlayer)
					{
						return true;
					}
					auto & Player = static_cast<cPlayer &>(a_Entity);
					return ((Player.GetEquippedItem().GetMaxDamage() - Player.GetEquippedItem().m_ItemDamage) == Durability);
				};
				Res &= a_World->DoWithEntityByID(a_Killer, Callback);
			}
			else if (DurabilityObject.isObject())
			{
				int Min = 0, Max = 0;
				LootTable::MinMaxRange(DurabilityObject, Min, Max);
				auto Callback = [&] (cEntity & a_Entity)
				{
					if (a_Entity.GetEntityType() != cEntity::etPlayer)
					{
					  return true;
					}
					auto & Player = static_cast<cPlayer &>(a_Entity);
					int Durability = Player.GetEquippedItem().GetMaxDamage() - Player.GetEquippedItem().m_ItemDamage;
					return ((Durability >= Min) && (Durability <= Min));
				};
				Res &= a_World->DoWithEntityByID(a_Killer, Callback);
			}
			/*
			enchantments: List of enchantments.
				enchantment: An enchantment ID.
				levels: The level of the enchantment.
					max: The maximum value.
					min: The minimum value.
			*/
			Json::Value EnchantmentsObject;
			if (Parameter.isMember("enchantments"))
			{
				EnchantmentsObject = Parameter["enchantments"];
			}
			else if (Parameter.isMember("Enchantments"))
			{
				EnchantmentsObject = Parameter["Enchantments"];
			}
			/*
			stored_enchantments: List of stored enchantments.
				enchantment: An enchantment ID.
				levels: The level of the enchantment.
					max: The maximum value.
					min: The minimum value.
			*/
			else if (Parameter.isMember("stored_enchantments"))
			{
				EnchantmentsObject = Parameter["stored_enchantments"];
			}
			else if (Parameter.isMember("StoredEnchantments"))
			{
				EnchantmentsObject = Parameter["StoredEnchantments"];
			}

			if (EnchantmentsObject.isArray())
			{
				Json::Value EnchantmentObject;
				for (unsigned int i = 0; i < EnchantmentsObject.size(); i++)
				{
					EnchantmentObject = EnchantmentsObject[i];
					int Enchantment;
					if (EnchantmentObject.isMember("enchantment"))
					{
						Enchantment = cEnchantments::StringToEnchantmentID(LootTable::NamespaceConverter(EnchantmentObject["enchantment"].asString()));
					}
					else if (EnchantmentObject.isMember("Enchantment"))
					{
						Enchantment = cEnchantments::StringToEnchantmentID(LootTable::NamespaceConverter(EnchantmentObject["Enchantment"].asString()));
					}

					Json::Value LevelsObject;
					if (EnchantmentObject.isMember("levels"))
					{
						LevelsObject = EnchantmentObject["levels"];
					}
					else if (EnchantmentObject.isMember("Levels"))
					{
						LevelsObject = EnchantmentObject["Levels"];
					}

					if (LevelsObject.isInt())
					{
						unsigned int Levels = LevelsObject.asInt();
						auto Callback = [&] (cEntity & a_Entity)
						{
							if (a_Entity.GetEntityType() != cEntity::etPlayer)
							{
								return true;
							}
							auto & Player = static_cast<cPlayer &>(a_Entity);
							return Player.GetEquippedItem().m_Enchantments.GetLevel(Enchantment) == Levels;
						};
						Res &= a_World->DoWithEntityByID(a_Killer, Callback);
					}
					else if (LevelsObject.isObject())
					{
						int Min = 0, Max = 100;
						LootTable::MinMaxRange(LevelsObject, Min, Max);

						auto Callback = [&] (cEntity & a_Entity)
						{
							if (a_Entity.GetEntityType() != cEntity::etPlayer)
							{
								return true;
							}
							auto & Player = static_cast<cPlayer &>(a_Entity);
							int Level = Player.GetEquippedItem().m_Enchantments.GetLevel(Enchantment);
							return ((Level >= Min) && (Level <= Max));
						};
						Res &= a_World->DoWithEntityByID(a_Killer, Callback);
					}
				}
			}
			// item: An item ID.
			AString ItemString;
			if (Parameter.isMember("item"))
			{
				ItemString = LootTable::NamespaceConverter(Parameter["item"].asString());
			}
			else if (Parameter.isMember("Item"))
			{
				ItemString = LootTable::NamespaceConverter(Parameter["Item"].asString());
			}
			if (!ItemString.empty())
			{
				cItem Item;
				StringToItem(ItemString, Item);
				auto Callback = [&] (cEntity & a_Entity)
				{
					if (a_Entity.GetEntityType() != cEntity::etPlayer)
					{
						return true;
					}
					auto & Player = static_cast<cPlayer & >(a_Entity);
					return Player.GetEquippedItem().IsSameType(Item);
				};
				Res &= a_World->DoWithEntityByID(a_Killer, Callback);
			}

			// nbt: An NBT string.
			if (Parameter.isMember("nbt") || Parameter.isMember("Nbt"))
			{
				// TODO: 06.09.2020 - Add when implemented - 12xx12
				LOGWARNING("Nbt for items is not yet supported - assuming true!");
			}
			// potion: A brewed potion ID.
			if (Parameter.isMember("potion") || Parameter.isMember("Potion"))
			{
				// TODO: 06.09.2020 - Add when implemented - 12xx12
				LOGWARNING("Nbt for items is not yet supported - assuming true!");
			}
			// tag: An item data pack tag.
			AString Tag;
			if (Parameter.isMember("tag"))
			{
				Tag = Parameter["tag"].asString();
			}
			else if (Parameter.isMember("Tag"))
			{
				Tag = Parameter["Tag"].asString();
			}

			if (!Tag.empty())
			{
				auto Callback = [&] (cEntity & a_Entity)
				{
					if (a_Entity.GetEntityType() != cEntity::etPlayer)
					{
						return true;
					}

					auto & Player = static_cast<cPlayer &>(a_Entity);
					return ItemTag::ItemTags(ItemTag::eItemTags(Tag)).ContainsType(Player.GetEquippedItem());
				};
				Res &= a_World->DoWithEntityByID(a_Killer, Callback);
			}

			return Res;
		}
		case LootTable::eConditionType::RandomChance:
		{
			float Rnd = fmod(a_Noise.IntNoise1D(a_World->GetWorldAge()), 1.0f);

			Json::Value Parameter;
			try
			{
				Parameter = std::get<Json::Value>(a_Condition.m_Parameter);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table condition - dropping entry");
				return true;
			}

			if (Parameter.isMember("chance"))
			{
				return Parameter["chance"].asFloat() > Rnd;
			}
			else if (Parameter.isMember("Chance"))
			{
				return Parameter["Chance"].asFloat() > Rnd;
			}
			LOGWARNING("An error occurred during random chance condition. Defaulting to true");
			return true;
		}
		case LootTable::eConditionType::RandomChanceWithLooting:
		{
			float Rnd = fmod(a_Noise.IntNoise1D(a_World->GetWorldAge()), 1.0f);

			Json::Value Parameter;
			try
			{
				Parameter = std::get<Json::Value>(a_Condition.m_Parameter);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table condition - dropping entry");
				return true;
			}

			float LootingMultiplier = 0;
			if (Parameter.isMember("looting_multiplier"))
			{
				LootingMultiplier = Parameter["looting_multiplier"].asFloat();
			}
			else if (Parameter.isMember("LootingMultiplier"))
			{
				LootingMultiplier = Parameter["LootingMultiplier"].asFloat();
			}

			int Looting = 0;  // = a_Killer->GetOffHandEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchLooting);

			if (Parameter.isMember("chance"))
			{
				return Parameter["chance"].asFloat() + Looting * LootingMultiplier > Rnd;
			}
			else if (Parameter.isMember("Chance"))
			{
				return Parameter["Chance"].asFloat() + Looting * LootingMultiplier > Rnd;
			}
			LOGWARNING("An error occurred during random chance condition. Defaulting to true");
			return true;
		}
		case LootTable::eConditionType::Reference:
		{
			// TODO: 06.09.2020 - 12xx12
			// ??? I have no clue what this does... the wiki says "Test if another referred condition (predicate) passes."
			LOGWARNING("Loot table condition \"Reference\" is not yet supported, assuming true");
			return true;
		}
		case LootTable::eConditionType::SurvivesExplosion:
		{
			// TODO: 06.09.2020 - 12xx12
			// You need to access the explosion radius
			LOGWARNING("Loot table condition \"SurvivesExplosion\" is not yet supported, assuming true");
			return true;
		}
		case LootTable::eConditionType::TableBonus:
		{
			Json::Value Parameter;
			try
			{
				Parameter = std::get<Json::Value>(a_Condition.m_Parameter);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table condition - dropping entry");
				return true;
			}

			int Enchantment;
			if (Parameter.isMember("enchantment"))
			{
				Enchantment = cEnchantments::StringToEnchantmentID(LootTable::NamespaceConverter(Parameter["enchantment"].asString()));
			}
			else if (Parameter.isMember("Enchantment"))
			{
				Enchantment = cEnchantments::StringToEnchantmentID(LootTable::NamespaceConverter(Parameter["Enchantment"].asString()));
			}

			Json::Value Chances;
			if (Parameter.isMember("chances"))
			{
				Chances = Parameter["chances"];
			}
			else if (Parameter.isMember("Chances"))
			{
				Chances = Parameter["Chances"];
			}
			if (!Chances.isArray())
			{
				LOGWARNING("Loot table condition \"TableBonus\" is missing chances - assuming true");
				return true;
			}

			auto Callback = [&] (cEntity & a_Entity)
			{
				if (a_Entity.GetEntityType() != cEntity::etPlayer)
				{
					return true;
				}
				auto & Player = static_cast<cPlayer &>(a_Entity);
				unsigned int Level = Player.GetEquippedItem().m_Enchantments.GetLevel(Enchantment);

				Level = std::min({Level, Chances.size()});

				return GetRandomProvider().RandReal(0.0f, 1.0f) < Chances[Level].asFloat();
			};

			return a_World->DoWithEntityByID(a_Killer, Callback);
		}
		case LootTable::eConditionType::TimeCheck:
		{
			Json::Value Parameter;
			try
			{
				Parameter = std::get<Json::Value>(a_Condition.m_Parameter);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table condition - dropping entry");
				return true;
			}

			Json::Value Value, PeriodObject;
			int Period = 24000;  // Defaults to one day
			if (Parameter.isMember("period"))
			{
				Period = Parameter["period"].asInt();
			}
			if (Parameter.isMember("Period"))
			{
				Period = Parameter["Period"].asInt();
			}

			if (Parameter.isMember("value"))
			{
				Value = Parameter["value"];
			}
			else if (Parameter.isMember("Value"))
			{
				Value = Parameter["Value"];
			}

			if (Value.isInt())
			{
				return (a_World->GetTimeOfDay() % Period) == Value.asInt();
			}
			else if (Value.isObject())
			{
				int Min = 0, Max = 0;
				LootTable::MinMaxRange(Value, Min, Max);
				return ((a_World->GetTimeOfDay() % Period) > Min) && ((a_World->GetTimeOfDay() % Period) < Max);
			}
			LOGWARNING("An error occurred during time check, assuming true!");
			return true;
		}
		case LootTable::eConditionType::WeatherCheck:
		{
			auto Weather = a_World->GetWeather();
			Json::Value Parameter;
			try
			{
				Parameter = std::get<Json::Value>(a_Condition.m_Parameter);
			}
			catch (const std::bad_variant_access &)
			{
				LOGWARNING("Unsupported Data type in loot table condition - dropping entry");
				return true;
			}

			bool RainCheck, ThunderCheck;
			if (Parameter.isMember("raining"))
			{
				RainCheck = Parameter["raining"].asBool();
			}
			else if (Parameter.isMember("Raining"))
			{
				RainCheck = Parameter["raining"].asBool();
			}
			else if (Parameter.isMember("thundering"))
			{
				ThunderCheck = Parameter["thundering"].asBool();
			}
			else if (Parameter.isMember("Thundering"))
			{
				ThunderCheck = Parameter["Thundering"].asBool();
			}

			bool Res = true;
			if (RainCheck)
			{
				Res &= (Weather == eWeather_Rain);
			}
			if (ThunderCheck)
			{
				Res &= (Weather == eWeather_ThunderStorm);
			}
			return Res;

		}
		default: return true;
	}
}





void cLootTable::ApplyCommonFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer)
{
	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::ApplyBonus:
		{
			Json::Value EnchantmentObject;

			if (a_Function.m_Parameter.isMember("enchantment"))
			{
				EnchantmentObject = a_Function.m_Parameter["enchantment"];
			}
			else if (a_Function.m_Parameter.isMember("Enchantment"))
			{
				EnchantmentObject = a_Function.m_Parameter["Enchantment"];
			}

			auto Enchantment = cEnchantments::StringToEnchantmentID(LootTable::NamespaceConverter(EnchantmentObject.asString()));
			int Level;

			auto Callback = [&] (cEntity & a_Entity)
			{
				if (a_Entity.GetEntityType() != cEntity::etPlayer)
				{
					return false;
				}

				auto & Player = static_cast<cPlayer &>(a_Entity);

				Level = Player.GetEquippedItem().m_Enchantments.GetLevel(Enchantment);
				return true;
			};

			a_World->DoWithEntityByID(a_Killed, Callback);

			AString Formula;
			if (a_Function.m_Parameter.isMember("formula"))
			{
				Formula = LootTable::NamespaceConverter(a_Function.m_Parameter["formula"].asString());
			}
			else if (a_Function.m_Parameter.isMember("Formula"))
			{
				Formula = LootTable::NamespaceConverter(a_Function.m_Parameter["Formula"].asString());
			}

			Json::Value Parameters;

			if (a_Function.m_Parameter.isMember("parameters"))
			{
				Parameters = a_Function.m_Parameter["parameters"];
			}
			else if (a_Function.m_Parameter.isMember("Parameters"))
			{
				Parameters = a_Function.m_Parameter["Parameters"];
			}

			if (Formula == "BinomialWithBonusCount")
			{
				// Binomial Probability Distribution with n=level + extra, p=probability
				int Extra;

				if (Parameters.isMember("extra"))
				{
					Extra = Parameters["extra"].asInt();
				}
				else if (Parameters.isMember("Extra"))
				{
					Extra = Parameters["Extra"].asInt();
				}

				float Probability;

				if (Parameters.isMember("probability"))
				{
					Probability = Parameters["probability"].asFloat();
				}
				else if (Parameters.isMember("Probability"))
				{
					Probability = Parameters["Probability"].asFloat();
				}

				std::default_random_engine Generator(a_Noise.GetSeed());
				std::binomial_distribution<int> Dist(Level + Extra, Probability);
				std::vector<int> Values;
				for (int i = 0; i < Level + Extra; i++)
				{
					Values[i] = Dist(Generator);
				}
				a_Item.m_ItemCount += Values[a_Noise.IntNoise3DInt(a_Pos * 15) % Values.size()];
			}
			else if (Formula == "UniformBonusCount")
			{
				// prob = (0, level * BonusMultiplier)
				int BonusMultiplier;
				if (Parameters.isMember("bonusMultiplier"))
				{
					BonusMultiplier = Parameters["bonusMultiplier"].asInt();
				}
				else if (Parameters.isMember("BonusMultiplier"))
				{
					BonusMultiplier = Parameters["BonusMultiplier"].asInt();
				}
				a_Item.m_ItemCount += a_Noise.IntNoise1DInt(a_Noise.GetSeed()) * a_Item.m_ItemType % (Level * BonusMultiplier);
			}
			else if (Formula == "OreDrops")
			{
				// Count *= (max(0; random(0..Level + 2) - 1) + 1)
				a_Item.m_ItemCount *= std::max(0, a_Noise.IntNoise1DInt(a_Noise.GetSeed() * a_Item.m_ItemType) % (Level + 2) - 1) + 1;
			}
			break;
		}
		case LootTable::eFunctionType::EnchantRandomly:
		{
			cWeightedEnchantments EnchantmentLimiter;
			if (a_Function.m_Parameter.isMember("enchantments"))
			{
				auto Enchantments = a_Function.m_Parameter["enchantments"];
				for (unsigned int i = 0; i < Enchantments.size(); i++)
				{
					EnchantmentLimiter.push_back({1, cEnchantments(LootTable::NamespaceConverter(Enchantments[i].asString()))});
				}
			}
			else if (a_Function.m_Parameter.isMember("Enchantments"))
			{
				auto Enchantments = a_Function.m_Parameter["Enchantments"];
				for (unsigned int i = 0; i < Enchantments.size(); i++)
				{
					EnchantmentLimiter.push_back({1, cEnchantments(LootTable::NamespaceConverter(Enchantments[i].asString()))});
				}
			}
			else
			{
				// All are possible
				cWeightedEnchantments Enchantments;
				cEnchantments::AddItemEnchantmentWeights(Enchantments, E_ITEM_BOOK, 24 + a_Noise.IntNoise3DInt(a_Pos * 20) % 7);

				cEnchantments Enchantment = cEnchantments::SelectEnchantmentFromVector(Enchantments, a_Noise.IntNoise3DInt(a_Pos * 10) % Enchantments.size());
				a_Item.m_ItemType = E_ITEM_ENCHANTED_BOOK;
				a_Item.m_Enchantments.Add(Enchantment);
				break;
			}
			a_Item.m_ItemType = E_ITEM_ENCHANTED_BOOK;
			a_Item.m_Enchantments.Add(cEnchantments::GetRandomEnchantmentFromVector(EnchantmentLimiter));
			break;
		}
		case LootTable::eFunctionType::EnchantWithLevels:
		{
			// TODO: 02.09.2020 - Add treasure when implemented - 12xx12
			bool Treasure = false;
			if (a_Function.m_Parameter.isMember("treasure"))
			{
				Treasure = a_Function.m_Parameter["treasure"].asBool();
				LOGWARNING("Treasure enchantments are not yet supported");
			}
			else if (a_Function.m_Parameter.isMember("Treasure"))
			{
				Treasure = a_Function.m_Parameter["Treasure"].asBool();
				LOGWARNING("Treasure enchantments are not yet supported");
			}
			UNUSED_VAR(Treasure);
			Json::Value LevelsObject;
			if (a_Function.m_Parameter.isMember("levels"))
			{
				LevelsObject = a_Function.m_Parameter["levels"];
			}
			else if (a_Function.m_Parameter.isMember("Levels"))
			{
				LevelsObject = a_Function.m_Parameter["Levels"];
			}
			else
			{
				LOGWARNING("No levels provided for enchantments in Loot table, dropping function");
				break;
			}
			int Levels = 0;
			if (LevelsObject.isInt())
			{
				Levels = LevelsObject.asInt();
			}
			else if (LevelsObject.isObject())
			{
				Levels = LootTable::MinMaxRand(Levels, a_Noise, a_Pos, a_Item.m_ItemType);
			}
			if (!Levels)
			{
				break;
			}
			a_Item.EnchantByXPLevels(Levels);
			break;
		}
		case LootTable::eFunctionType::ExplorationMap:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("Exploration maps are not implemented, dropping function!");
			break;
		}
		case LootTable::eFunctionType::FurnaceSmelt:
		{
			auto Recipes = cRoot::Get()->GetFurnaceRecipe();
			auto NewItem = Recipes->GetRecipeFrom(a_Item)->Out;
			a_Item.m_ItemType = NewItem->m_ItemType;
			a_Item.m_ItemDamage = NewItem->m_ItemDamage;
			break;
		}
		case LootTable::eFunctionType::LimitCount:
		{
			Json::Value LimitObject;
			if (a_Function.m_Parameter.isMember("limit"))
			{
				LimitObject = a_Function.m_Parameter["limit"];
			}
			else if (a_Function.m_Parameter.isMember("Limit"))
			{
				LimitObject = a_Function.m_Parameter["Limit"];
			}
			else
			{
				LOGWARNING("Missing limit, dropping function!");
			}

			int Limit = -1;
			if (LimitObject.isInt())
			{
				Limit = LimitObject.isInt();
			}
			else if (LimitObject.isObject())
			{
				Limit = LootTable::MinMaxRand(LimitObject, a_Noise, a_Pos, a_Item.m_ItemType);
			}
			if (Limit < 0)
			{
				break;
			}
			if (a_Item.m_ItemCount > Limit)
			{
				a_Item.m_ItemCount = Limit;
			}
			break;
		}
		case LootTable::eFunctionType::SetAttributes:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("Attributes for items are not implemented, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetCount:
		{
			Json::Value CountObject;
			if (a_Function.m_Parameter.isMember("count"))
			{
				CountObject = a_Function.m_Parameter["count"];
			}
			else if (a_Function.m_Parameter.isMember("Count"))
			{
				CountObject = a_Function.m_Parameter["Count"];
			}

			if (CountObject.isInt())
			{
				a_Item.m_ItemCount = CountObject.asInt();
			}
			else if (CountObject.isObject())
			{
				AString Type;
				if (CountObject.isMember("type"))
				{
					Type = LootTable::NamespaceConverter(CountObject["type"].asString());
				}
				else if (CountObject.isMember("Type"))
				{
					Type = LootTable::NamespaceConverter(CountObject["Type"].asString());
				}

				if ((Type == "Uniform") || (Type == "uniform"))
				{
					a_Item.m_ItemCount = LootTable::MinMaxRand(CountObject, a_Noise, a_Pos, 7);
				}
				else if ((Type == "Binomial") || (Type == "binomial"))
				{
					int N = 0;
					float P = 0;
					if (CountObject.isMember("n"))
					{
						N = CountObject["n"].asInt();
					}
					else if (CountObject.isMember("N"))
					{
						N = CountObject["N"].asInt();
					}

					if (CountObject.isMember("p"))
					{
						P = CountObject["p"].asFloat();
					}
					else if (CountObject.isMember("P"))
					{
						P = CountObject["P"].asFloat();
					}
					std::default_random_engine Generator(a_Noise.GetSeed());
					std::binomial_distribution<int> Dist(N, P);
					std::vector<int> Values;
					for (int i = 0; i < N; i++)
					{
						Values[i] = Dist(Generator);
					}
					a_Item.m_ItemCount += Values[a_Noise.IntNoise1DInt(a_Noise.GetSeed() * a_Item.m_ItemType) % Values.size()];
				}
			}
			break;
		}
		case LootTable::eFunctionType::SetDamage:
		{
			Json::Value DamageObject;

			if (a_Function.m_Parameter.isMember("damage"))
			{
				DamageObject = a_Function.m_Parameter["damage"];
			}
			else if (a_Function.m_Parameter.isMember("Damage"))
			{
				DamageObject = a_Function.m_Parameter["Damage"];
			}
			float Damage = 0;
			if (DamageObject.isNumeric())
			{
				Damage = DamageObject.asFloat();
			}
			else if (DamageObject.isObject())
			{
				float Min = 0.0f, Max = 0.0f;
				if (DamageObject.isMember("min"))
				{
					Min = DamageObject["min"].asFloat();
				}
				else if (DamageObject.isMember("Min"))
				{
					Min = DamageObject["Min"].asFloat();
				}

				if (DamageObject.isMember("max"))
				{
					Max = DamageObject["max"].asFloat();
				}
				else if (DamageObject.isMember("Max"))
				{
					Max = DamageObject["Max"].asFloat();
				}

				if (Min > Max)
				{
					Max = Min;
				}
				Damage = std::fmod(a_Noise.IntNoise1D(a_Noise.GetSeed() * a_Item.m_ItemType), Max - Min) + Min;
			}
			a_Item.m_ItemDamage = floor(a_Item.m_ItemDamage * Damage);
			break;
		}
		case LootTable::eFunctionType::SetLootTable:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("NBT for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetLore:
		{
			// TODO: 06.09.2020 - Add the json string parsing - 12xx12
			// Note: When adding this you need to move the function to the other ApplyFunction functions.
			// This is different for each type because the @s tag which might be used reads the entity's name.
			LOGWARNING("Lore for dropped items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetName:
		{
			// TODO: 06.09.2020 - Add the json string parsing - 12xx12
			// Note: When adding this you need to move the function to the other ApplyFunction functions.
			// This is different for each type because the @s tag which might be used reads the entity's name.
			LOGWARNING("Naming for dropped items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetNbt:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("NBT for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::SetStewEffect:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("Stews are not yet supported, dropping function!");
			break;
		}
		default:
		{
			LOGWARNING("Function type is not supported for this entity, dropping function!");
			break;
		}
	}
}




// Item in container
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Killed, a_Killer))
	{
		return;
	}

	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::FillPlayerHead:
		{
			// TODO: 02.09.2020 - Add actual player head when implemented
			if (a_Item.m_ItemType == E_ITEM_HEAD)
			{
				a_Item.m_ItemDamage = E_META_HEAD_PLAYER;
				LOGWARNING("Setting players head as a item is not supported, setting to default player Design!");
			}
			else
			{
				LOGWARNING("Wrong Item provided to assign head type!");
			}
			break;
		}
		case LootTable::eFunctionType::None:
		{
			break;
		}
		default:
		{
			ApplyCommonFunction(a_Function, a_Item, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
			break;
		}
	}
}




// Block
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const cBlockHandler & a_Block, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Killed, a_Killer))
	{
		return;
	}

	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::CopyNbt:
		{
			// TODO: 06.09.2020 - Add when implemented - 12xx12
			LOGWARNING("NBT for items is not yet supported, Dropping function!");
			break;
		}
		case LootTable::eFunctionType::CopyState:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("States for items is not yet supported, dropping function!");
			break;
		}
		case LootTable::eFunctionType::ExplosionDecay:
		{
			// TODO: 06.09.2020: Add. You need access to the next explosion
			LOG("Explosion decay is not supported - dropping function!");
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:
		{
			// TODO: 02.09.2020 - Add actual player head when implemented
			if (a_Item.m_ItemType == E_ITEM_HEAD)
			{
				a_Item.m_ItemDamage = E_META_HEAD_PLAYER;
				LOGWARNING("Setting players head as a item is not supported, setting to default player Design!");
			}
			else
			{
				LOGWARNING("Wrong Item provided to assign head type!");
			}
			break;
		}
		case LootTable::eFunctionType::SetContents:
		{
			// TODO: 02.09.2020 - Add when implemented - 12xx12
			LOGWARNING("NBT for items is not yet supported, Dropping function!");
			break;
		}
		case LootTable::eFunctionType::None:
		{
			break;
		}
		default:
		{
			ApplyCommonFunction(a_Function, a_Item, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
			break;
		}
	}
}




// Block Entity
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const cBlockEntity & a_BlockEntity, const cNoise & a_Noise, const Vector3i & a_Pos, const UInt32 & a_Killed, const UInt32 & a_Killer)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Killed, a_Killer))
	{
		return;
	}

	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::None:
		{
			break;
		}
		default:
		{
			ApplyCommonFunction(a_Function, a_Item, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
			break;
		}
	}
}



// Killed entity
void cLootTable::ApplyFunction(const cLootTableFunction & a_Function, cItem & a_Item, cWorld * a_World, const Vector3i & a_Pos, const UInt32 & a_Killed, const cNoise & a_Noise, const UInt32 & a_Killer)
{
	if (!ConditionsApply(a_Function.m_Conditions, a_World, a_Noise, a_Killed, a_Killer))
	{
		return;
	}

	switch (a_Function.m_Type)
	{
		case LootTable::eFunctionType::CopyNbt:
		{
			LOGWARNING("NBT for items is not yet supported, Dropping function!");
			break;
		}
		case LootTable::eFunctionType::FillPlayerHead:
		{
			// TODO: 02.09.2020 - Add actual player head when implemented
			if (a_Item.m_ItemType == E_ITEM_HEAD)
			{
				// Gets entity type and assigns the matching Head type to the item.
				auto CallBack = [&] (cEntity & a_Entity)
				{
					if (a_Entity.GetEntityType() != cEntity::etMob)
					{
						return false;
					}
					auto & Mob = static_cast<cMonster &>(a_Entity);
					if (Mob.GetMobType() == mtCreeper)
					{
						a_Item.m_ItemDamage = E_META_HEAD_CREEPER;
					}
					else if (Mob.GetMobType() == mtZombie)
					{
						a_Item.m_ItemDamage = E_META_HEAD_ZOMBIE;
					}
					else if (Mob.GetMobType() == mtSkeleton)
					{
						a_Item.m_ItemDamage = E_META_HEAD_SKELETON;
					}
					else if (Mob.GetMobType() == mtWitherSkeleton)
					{
						a_Item.m_ItemDamage = E_META_HEAD_WITHER;
					}
					else
					{
						a_Item.m_ItemDamage = E_META_HEAD_PLAYER;
					}
					return true;
				};
				a_World->DoWithEntityByID(a_Killed, CallBack);
			}
			else
			{
				LOGWARNING("Wrong Item provided to assign head type!");
			}
			break;
		}
		case LootTable::eFunctionType::LootingEnchant:
		{
			int Looting = 0;
			auto Callback = [&] (cEntity & a_Entity)
			{
				if (a_Entity.GetEntityType() != cEntity::etPlayer)
				{
					return false;
				}
				auto & Player = static_cast<cPlayer &>(a_Entity);
				Looting = Player.GetEquippedItem().m_Enchantments.GetLevel(cEnchantments::enchLooting);
				return true;
			};

			a_World->DoWithEntityByID(a_Killer, Callback);

			Json::Value CountObject;
			if (a_Function.m_Parameter.isMember("count"))
			{
				CountObject = a_Function.m_Parameter["count"];
			}
			else if (a_Function.m_Parameter.isMember("Count"))
			{
				CountObject = a_Function.m_Parameter["Count"];
			}

			int Count;
			if (CountObject.isInt())
			{
				Count = CountObject.asInt();
			}
			else if (CountObject.isObject())
			{
				Count = LootTable::MinMaxRand(CountObject, a_Noise, a_Pos, 5);
			}
			int Limit = 0;
			if (a_Function.m_Parameter.isMember("limit"))
			{
				Limit = a_Function.m_Parameter["limit"].asInt();
			}
			else if (a_Function.m_Parameter.isMember("Limit"))
			{
				Limit = a_Function.m_Parameter["Limit"].asInt();
			}

			a_Item.m_ItemCount += Looting * Count;

			if (Limit > 0)
			{
				if (a_Item.m_ItemCount > Limit)
				{
					a_Item.m_ItemCount = Limit;
				}
			}

			break;
		}
		case LootTable::eFunctionType::None:
		{
			break;
		}
		default:
		{
			ApplyCommonFunction(a_Function, a_Item, a_World, a_Noise, a_Pos, a_Killed, a_Killer);
			break;
		}
	}
}




