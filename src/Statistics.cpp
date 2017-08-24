
// Statistics.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Statistics.h"



cStatInfo cStatInfo::ms_Info[statCount] =
{
	// The order must match the order of enum eStatistic

	// https://minecraft.gamepedia.com/Achievements

	/*             Type          |      Name            |  Prerequisite      */
	cStatInfo(achOpenInv,           "achievement.openInventory"),
	cStatInfo(achMineWood,          "achievement.mineWood",           achOpenInv),
	cStatInfo(achCraftWorkbench,    "achievement.buildWorkBench",     achMineWood),
	cStatInfo(achCraftPickaxe,      "achievement.buildPickaxe",       achCraftWorkbench),
	cStatInfo(achCraftFurnace,      "achievement.buildFurnace",       achCraftPickaxe),
	cStatInfo(achAcquireIron,       "achievement.acquireIron",        achCraftFurnace),
	cStatInfo(achCraftHoe,          "achievement.buildHoe",           achCraftWorkbench),
	cStatInfo(achMakeBread,         "achievement.makeBread",          achCraftHoe),
	cStatInfo(achBakeCake,          "achievement.bakeCake",           achCraftHoe),
	cStatInfo(achCraftBetterPick,   "achievement.buildBetterPickaxe", achCraftPickaxe),
	cStatInfo(achCookFish,          "achievement.cookFish",           achAcquireIron),
	cStatInfo(achOnARail,           "achievement.onARail",            achAcquireIron),
	cStatInfo(achCraftSword,        "achievement.buildSword",         achCraftWorkbench),
	cStatInfo(achKillMonster,       "achievement.killEnemy",          achCraftSword),
	cStatInfo(achKillCow,           "achievement.killCow",            achCraftSword),
	cStatInfo(achFlyPig,            "achievement.flyPig",             achKillCow),
	cStatInfo(achSnipeSkeleton,     "achievement.snipeSkeleton",      achKillMonster),
	cStatInfo(achDiamonds,          "achievement.diamonds",           achAcquireIron),
	cStatInfo(achEnterPortal,       "achievement.portal",             achDiamonds),
	cStatInfo(achReturnToSender,    "achievement.ghast",              achEnterPortal),
	cStatInfo(achBlazeRod,          "achievement.blazeRod",           achEnterPortal),
	cStatInfo(achBrewPotion,        "achievement.potion",             achBlazeRod),
	cStatInfo(achEnterTheEnd,       "achievement.theEnd",             achBlazeRod),
	cStatInfo(achDefeatDragon,      "achievement.theEnd2",            achEnterTheEnd),
	cStatInfo(achCraftEnchantTable, "achievement.enchantments",       achDiamonds),
	cStatInfo(achOverkill,          "achievement.overkill",           achCraftEnchantTable),
	cStatInfo(achBookshelf,         "achievement.bookcase",           achCraftEnchantTable),
	cStatInfo(achExploreAllBiomes,  "achievement.exploreAllBiomes",   achEnterTheEnd),
	cStatInfo(achSpawnWither,       "achievement.spawnWither",        achDefeatDragon),
	cStatInfo(achKillWither,        "achievement.killWither",         achSpawnWither),
	cStatInfo(achFullBeacon,        "achievement.fullBeacon",         achKillWither),
	cStatInfo(achBreedCow,          "achievement.breedCow",           achKillCow),
	cStatInfo(achThrowDiamonds,     "achievement.diamondsToYou",      achDiamonds),

	// https://minecraft.gamepedia.com/Statistics

	/*             Type         |     Name          */
	cStatInfo(statGamesQuit,      "stat.leaveGame"),
	cStatInfo(statMinutesPlayed,  "stat.playOneMinute"),
	cStatInfo(statDistWalked,     "stat.walkOneCm"),
	cStatInfo(statDistSwum,       "stat.swimOneCm"),
	cStatInfo(statDistFallen,     "stat.fallOneCm"),
	cStatInfo(statDistClimbed,    "stat.climbOneCm"),
	cStatInfo(statDistFlown,      "stat.flyOneCm"),
	cStatInfo(statDistDove,       "stat.diveOneCm"),
	cStatInfo(statDistMinecart,   "stat.minecartOneCm"),
	cStatInfo(statDistBoat,       "stat.boatOneCm"),
	cStatInfo(statDistPig,        "stat.pigOneCm"),
	cStatInfo(statDistHorse,      "stat.horseOneCm"),
	cStatInfo(statJumps,          "stat.jump"),
	cStatInfo(statItemsDropped,   "stat.drop"),
	cStatInfo(statDamageDealt,    "stat.damageDealt"),
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
		if (NoCaseCompare(ms_Info[i].m_Name, a_Name) == 0)
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





cStatManager::cStatManager()
{
	Reset();
}





StatValue cStatManager::GetValue(const eStatistic a_Stat) const
{
	ASSERT((a_Stat > statInvalid) && (a_Stat < statCount));

	return m_MainStats[a_Stat];
}





void cStatManager::SetValue(const eStatistic a_Stat, const StatValue a_Value)
{
	ASSERT((a_Stat > statInvalid) && (a_Stat < statCount));

	m_MainStats[a_Stat] = a_Value;
}





StatValue cStatManager::AddValue(const eStatistic a_Stat, const StatValue a_Delta)
{
	ASSERT((a_Stat > statInvalid) && (a_Stat < statCount));

	m_MainStats[a_Stat] += a_Delta;

	return m_MainStats[a_Stat];
}





void cStatManager::Reset(void)
{
	for (unsigned int i = 0; i < static_cast<unsigned int>(statCount); ++i)
	{
		m_MainStats[i] = 0;
	}
}





