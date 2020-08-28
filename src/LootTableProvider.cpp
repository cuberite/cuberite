
// cLootTableProvider.cpp

#include "LootTableProvider.h"
// #include "Registries/LootTables.cpp"

cLootTableProvider::cLootTableProvider()
{

}





std::shared_ptr<cLootTable> cLootTableProvider::GetLootTableFromString(AString a_Name)
{
	return nullptr;
}





std::shared_ptr<cLootTable> cLootTableProvider::GetLootTableFromBlock()
{
	return nullptr;
}





std::shared_ptr<cLootTable> cLootTableProvider::GetLooTableFromMob(eMonsterType a_Type)
{
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
		default: return std::make_shared<cLootTable>();
	}
}

cLootTable::cLootTable() {}
cLootTable::cLootTable(AString a_Description) {}
bool cLootTable::ReadFromString(AString a_Description) { return false; }
bool cLootTable::FillWithLoot(Vector3i a_Pos) { return false; }
