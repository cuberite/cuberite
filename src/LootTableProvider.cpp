
// cLootTableProvider.cpp

#include "LootTableProvider.h"
// #include "Registries/LootTables.h"

cLootTableProvider::cLootTableProvider(cWorld * a_World)
{
	m_World = a_World;
	auto Path = m_World->GetDataPath();
	// Check for custom tables

	// Load if available
}




/*
Format:    Type|FurtherInfo
example:   Chest|AbandonedMineshaft
example:   Monster|Skeleton
example:   Block|Stone
This is not case sensitive, and removes all spaces
*/
std::shared_ptr<cLootTable> cLootTableProvider::GetLootTable(const AString & a_Name) const
{
	auto Data = StringSplitAndTrim(a_Name,"|");

	if ((Data.size() > 0) || (Data.size() > 2))
	{
		LOGWARNING("Got ill formatted string to look up a loot table. Please use Type|Subtype");
		return std::make_shared<cLootTable>(cEmptyLootTable());
	}

	auto Type = LootTable::eType(Data[0]);

	switch (Type)
	{
		case LootTable::eType::Chest:
		{
			auto ChestType = LootTable::eChestType(Data[1]);
			return GetLootTable(ChestType);
		}
		case LootTable::eType::Entity:
		{
			auto MobType = LootTable::eMonsterType(Data[1]);
			return GetLootTable(MobType);
		}
		case LootTable::eType::Block:
		{
			// TODO: add block meta
			const BLOCKTYPE BlockType = static_cast<ENUM_BLOCK_TYPE>(BlockStringToType(Data[1]));
			return GetLootTable(BlockType);
		}
		default: return std::make_shared<cLootTable>(cEmptyLootTable());
	}
}





std::shared_ptr<cLootTable> cLootTableProvider::GetLootTable(const BLOCKTYPE a_Block) const
{
	return nullptr;
}





std::shared_ptr<cLootTable> cLootTableProvider::GetLootTable(const BLOCKTYPE a_Block, const ENUM_BLOCK_META) const
{
	return nullptr;
}





std::shared_ptr<cLootTable> cLootTableProvider::GetLootTable(const enum LootTable::eChestType a_Type) const
{
	// Checks if there's a custom Loot table available for this type else returns the default one
	auto CustomTable = m_CustomChestLootTables.find(a_Type);

	if (CustomTable != m_CustomChestLootTables.end())
	{
		return CustomTable->second;
	}

	switch (a_Type)
	{
		case LootTable::eChestType::AbandonedMineshaft:     return std::make_shared<cLootTable>();
		case LootTable::eChestType::BuriedTreasure:         return std::make_shared<cLootTable>();
		case LootTable::eChestType::DesertPyramid:          return std::make_shared<cLootTable>();
		case LootTable::eChestType::EndCityTreasure:        return std::make_shared<cLootTable>();
		case LootTable::eChestType::IglooChest:             return std::make_shared<cLootTable>();
		case LootTable::eChestType::JungleTemple:           return std::make_shared<cLootTable>();
		case LootTable::eChestType::JungleTempleDispenser:  return std::make_shared<cLootTable>();
		case LootTable::eChestType::NetherBridge:           return std::make_shared<cLootTable>();
		case LootTable::eChestType::PillagerOutpost:        return std::make_shared<cLootTable>();
		case LootTable::eChestType::ShipwreckMap:           return std::make_shared<cLootTable>();
		case LootTable::eChestType::ShipwreckSupply:        return std::make_shared<cLootTable>();
		case LootTable::eChestType::ShipwreckTreasure:      return std::make_shared<cLootTable>();
		case LootTable::eChestType::SimpleDungeon:          return std::make_shared<cLootTable>();
		case LootTable::eChestType::SpawnBonusChest:        return std::make_shared<cLootTable>();
		case LootTable::eChestType::StrongholdCorridor:     return std::make_shared<cLootTable>();
		case LootTable::eChestType::StrongholdCrossing:     return std::make_shared<cLootTable>();
		case LootTable::eChestType::StrongholdLibrary:      return std::make_shared<cLootTable>();
		case LootTable::eChestType::UnderwaterRuinBig:      return std::make_shared<cLootTable>();
		case LootTable::eChestType::UnderwaterRuinSmall:    return std::make_shared<cLootTable>();

			/* Village chest types */
		case LootTable::eChestType::VillageArmorer:         return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageButcher:         return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageCartographer:    return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageDesertHouse:     return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageFisher:          return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageFletcher:        return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageMason:           return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillagePlainsHouse:     return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageSavannaHouse:    return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageShepherd:        return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageSnowyHouse:      return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageTaigaHouse:      return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageTannery:         return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageTemple:          return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageToolsmith:       return std::make_shared<cLootTable>();
		case LootTable::eChestType::VillageWeaponsmith:     return std::make_shared<cLootTable>();

		case LootTable::eChestType::WoodlandMansion:        return std::make_shared<cLootTable>();
		default:                                            return nullptr;
	}
}





std::shared_ptr<cLootTable> cLootTableProvider::GetLootTable(const eMonsterType a_Type) const
{
	// Checks if there's a custom Loot table available for this type else returns the default one
	auto CustomTable = m_CustomMonsterLootTables.find(a_Type);

	if (CustomTable != m_CustomMonsterLootTables.end())
	{
		return CustomTable->second;
	}

	switch (a_Type)
	{
		case mtBat:            return std::make_shared<cLootTable>();
		case mtBlaze:          return std::make_shared<cLootTable>();
		case mtCaveSpider:     return std::make_shared<cLootTable>();
		case mtChicken:        return std::make_shared<cLootTable>();
		case mtCow:            return std::make_shared<cLootTable>();
		case mtCreeper:        return std::make_shared<cLootTable>();
		case mtEnderDragon:    return std::make_shared<cLootTable>();
		case mtEnderman:       return std::make_shared<cLootTable>();
		case mtGhast:          return std::make_shared<cLootTable>();
		case mtGiant:          return std::make_shared<cLootTable>();
		case mtGuardian:       return std::make_shared<cLootTable>();
		case mtHorse:          return std::make_shared<cLootTable>();
		case mtIronGolem:      return std::make_shared<cLootTable>();
		case mtMagmaCube:      return std::make_shared<cLootTable>();
		case mtMooshroom:      return std::make_shared<cLootTable>();
		case mtOcelot:         return std::make_shared<cLootTable>();
		case mtPig:            return std::make_shared<cLootTable>();
		case mtRabbit:         return std::make_shared<cLootTable>();
		case mtSheep:          return std::make_shared<cLootTable>();
		case mtSilverfish:     return std::make_shared<cLootTable>();
		case mtSkeleton:       return std::make_shared<cLootTable>();
		case mtSlime:          return std::make_shared<cLootTable>();
		case mtSnowGolem:      return std::make_shared<cLootTable>();
		case mtSpider:         return std::make_shared<cLootTable>();
		case mtSquid:          return std::make_shared<cLootTable>();
		case mtVillager:       return std::make_shared<cLootTable>();
		case mtWitch:          return std::make_shared<cLootTable>();
		case mtWither:         return std::make_shared<cLootTable>();
		case mtWitherSkeleton: return std::make_shared<cLootTable>();
		case mtWolf:           return std::make_shared<cLootTable>();
		case mtZombie:         return std::make_shared<cLootTable>();
		case mtZombiePigman:   return std::make_shared<cLootTable>();
		case mtZombieVillager: return std::make_shared<cLootTable>();
		default: return std::make_shared<cLootTable>(cLootTable());
	}
}





void cLootTableProvider::DropLoot(const cMonster & a_Monster) const
{
	auto LootTable = GetLootTable(a_Monster.GetMobType());


}





cLootTable::cLootTable() {}
cLootTable::cLootTable(const AString & a_Description) {}
cLootTable::cLootTable(const cLootTable & a_Other)
{
	m_Type = a_Other.m_Type;
	m_LootTablePools = a_Other.m_LootTablePools;
	m_LootTableFunctions = a_Other.m_LootTableFunctions;
}
cLootTable::~cLootTable() {}
bool cLootTable::ReadFromString(const AString & a_Description) { return false; }
bool cLootTable::FillWithLoot(cBlockEntityWithItems & a_BlockEntity) { return false; }
