
// Statistics.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Statistics.h"



cStatInfo cStatInfo::ms_Info[statCount] = {
	// The order must match the order of enum eStatistic

	// http://minecraft.gamepedia.com/Achievements

	/*             Type          |      Name            |  Prerequisite      */
	cStatInfo(achOpenInv,           "openInventory"),
	cStatInfo(achMineWood,          "mineWood",           achOpenInv),
	cStatInfo(achCraftWorkbench,    "buildWorkBench",     achMineWood),
	cStatInfo(achCraftPickaxe,      "buildPickaxe",       achCraftWorkbench),
	cStatInfo(achCraftFurnace,      "buildFurnace",       achCraftPickaxe),
	cStatInfo(achAcquireIron,       "acquireIron",        achCraftFurnace),
	cStatInfo(achCraftHoe,          "buildHoe",           achCraftWorkbench),
	cStatInfo(achMakeBread,         "makeBread",          achCraftHoe),
	cStatInfo(achBakeCake,          "bakeCake",           achCraftHoe),
	cStatInfo(achCraftBetterPick,   "buildBetterPickaxe", achCraftPickaxe),
	cStatInfo(achCookFish,          "cookFish",           achAcquireIron),
	cStatInfo(achOnARail,           "onARail",            achAcquireIron),
	cStatInfo(achCraftSword,        "buildSword",         achCraftWorkbench),
	cStatInfo(achKillMonster,       "killEnemy",          achCraftSword),
	cStatInfo(achKillCow,           "killCow",            achCraftSword),
	cStatInfo(achFlyPig,            "flyPig",             achKillCow),
	cStatInfo(achSnipeSkeleton,     "snipeSkeleton",      achKillMonster),
	cStatInfo(achDiamonds,          "diamonds",           achAcquireIron),
	cStatInfo(achEnterPortal,       "portal",             achDiamonds),
	cStatInfo(achReturnToSender,    "ghast",              achEnterPortal),
	cStatInfo(achBlazeRod,          "blazeRod",           achEnterPortal),
	cStatInfo(achBrewPotion,        "potion",             achBlazeRod),
	cStatInfo(achEnterTheEnd,       "theEnd",             achBlazeRod),
	cStatInfo(achDefeatDragon,      "theEnd2",            achEnterTheEnd),
	cStatInfo(achCraftEnchantTable, "enchantments",       achDiamonds),
	cStatInfo(achOverkill,          "overkill",           achCraftEnchantTable),
	cStatInfo(achBookshelf,         "bookcase",           achCraftEnchantTable),
	cStatInfo(achExploreAllBiomes,  "exploreAllBiomes",   achEnterTheEnd),
	cStatInfo(achSpawnWither,       "spawnWither",        achDefeatDragon),
	cStatInfo(achKillWither,        "killWither",         achSpawnWither),
	cStatInfo(achFullBeacon,        "fullBeacon",         achKillWither),
	cStatInfo(achBreedCow,          "breedCow",           achKillCow),
	cStatInfo(achThrowDiamonds,     "diamondsToYou",      achDiamonds),

	// http://minecraft.gamepedia.com/Statistics

	/*             Type         |     Name          */
	cStatInfo(statGamesQuit,      "stat.leaveGame"),
	cStatInfo(statMinutesPlayed,  "stat.playOneMinute"),
	cStatInfo(statDistWalked,     "stat.walkOnCm"),
	cStatInfo(statDistSwum,       "stat.swimOneCm"),
	cStatInfo(statDistFallen,     "stat.fallOneCm"),
	cStatInfo(statDistClimbed,    "stat.climbOneCm"),
	cStatInfo(statDistFlown,      "stat.flyOneCm"),
	cStatInfo(statDistMinecart,   "stat.minecartOneCm"),
	cStatInfo(statDistBoat,       "stat.boatOneCm"),
	cStatInfo(statDistPig,        "stat.pigOneCm"),
	cStatInfo(statDistHorse,      "stat.horseOneCm"),
	cStatInfo(statJumps,          "stat.jump"),
	cStatInfo(statItemsDropped,   "stat.drop"),
	cStatInfo(statDamageDealt,    "stat.damageDealth"),
	cStatInfo(statDamageTaken,    "stat.damageTaken"),
	cStatInfo(statDeaths,         "stat.deaths"),
	cStatInfo(statMobKills,       "stat.mobKills"),
	cStatInfo(statAnimalsBred,    "stat.animalsBred"),
	cStatInfo(statPlayerKills,    "stat.playerKills"),
	cStatInfo(statFishCaught,     "stat.fishCaught"),
	cStatInfo(statJunkFished,     "stat.junkFished"),
	cStatInfo(statTreasureFished, "stat.treasureFished")
};






cStatInfo::cStatInfo()
	: m_Type(statInvalid)
	, m_Depends(statInvalid)
{}





cStatInfo::cStatInfo(const eStatistic a_Type, const AString & a_Name, const eStatistic a_Depends)
	: m_Type(a_Type)
	, m_Name(a_Name)
	, m_Depends(a_Depends)
{}





const AString & cStatInfo::GetName(const eStatistic a_Type)
{
	ASSERT((a_Type > statInvalid) && (a_Type < statCount));

	return ms_Info[a_Type].m_Name;
}





eStatistic cStatInfo::GetType(const AString & a_Name)
{
	for (unsigned int i = 0; i < ARRAYCOUNT(ms_Info); ++i)
	{
		if (NoCaseCompare(ms_Info[i].m_Name, a_Name))
		{
			return ms_Info[i].m_Type;
		}
	}

	return statInvalid;
}





eStatistic cStatInfo::GetPrerequisite(const eStatistic a_Type)
{
	ASSERT((a_Type > statInvalid) && (a_Type < statCount));

	return ms_Info[a_Type].m_Depends;
}





