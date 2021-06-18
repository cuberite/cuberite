
// Statistics.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "StatisticsManager.h"





bool StatisticsManager::SatisfiesPrerequisite(const CustomStatistic a_Stat) const
{
	switch (a_Stat)
	{
		case CustomStatistic::AchOpenInventory:      return true;
		case CustomStatistic::AchMineWood:           return IsStatisticPresent(CustomStatistic::AchOpenInventory);
		case CustomStatistic::AchBuildWorkBench:     return IsStatisticPresent(CustomStatistic::AchMineWood);
		case CustomStatistic::AchBuildHoe:           return IsStatisticPresent(CustomStatistic::AchBuildWorkBench);
		case CustomStatistic::AchBakeCake:           return IsStatisticPresent(CustomStatistic::AchBuildHoe);
		case CustomStatistic::AchMakeBread:          return IsStatisticPresent(CustomStatistic::AchBuildHoe);
		case CustomStatistic::AchBuildSword:         return IsStatisticPresent(CustomStatistic::AchBuildWorkBench);
		case CustomStatistic::AchKillCow:            return IsStatisticPresent(CustomStatistic::AchBuildSword);
		case CustomStatistic::AchFlyPig:             return IsStatisticPresent(CustomStatistic::AchKillCow);
		case CustomStatistic::AchBreedCow:           return IsStatisticPresent(CustomStatistic::AchKillCow);
		case CustomStatistic::AchKillEnemy:          return IsStatisticPresent(CustomStatistic::AchBuildSword);
		case CustomStatistic::AchSnipeSkeleton:      return IsStatisticPresent(CustomStatistic::AchKillEnemy);
		case CustomStatistic::AchBuildPickaxe:       return IsStatisticPresent(CustomStatistic::AchBuildWorkBench);
		case CustomStatistic::AchBuildBetterPickaxe: return IsStatisticPresent(CustomStatistic::AchBuildPickaxe);
		case CustomStatistic::AchBuildFurnace:       return IsStatisticPresent(CustomStatistic::AchBuildWorkBench);
		case CustomStatistic::AchCookFish:           return IsStatisticPresent(CustomStatistic::AchBuildFurnace);
		case CustomStatistic::AchAcquireIron:        return IsStatisticPresent(CustomStatistic::AchBuildFurnace);
		case CustomStatistic::AchOnARail:            return IsStatisticPresent(CustomStatistic::AchAcquireIron);
		case CustomStatistic::AchDiamonds:           return IsStatisticPresent(CustomStatistic::AchAcquireIron);
		case CustomStatistic::AchPortal:             return IsStatisticPresent(CustomStatistic::AchDiamonds);
		case CustomStatistic::AchGhast:              return IsStatisticPresent(CustomStatistic::AchPortal);
		case CustomStatistic::AchBlazeRod:           return IsStatisticPresent(CustomStatistic::AchPortal);
		case CustomStatistic::AchPotion:             return IsStatisticPresent(CustomStatistic::AchBlazeRod);
		case CustomStatistic::AchTheEnd:             return IsStatisticPresent(CustomStatistic::AchBlazeRod);
		case CustomStatistic::AchTheEnd2:            return IsStatisticPresent(CustomStatistic::AchTheEnd);
		case CustomStatistic::AchEnchantments:       return IsStatisticPresent(CustomStatistic::AchDiamonds);
		case CustomStatistic::AchOverkill:           return IsStatisticPresent(CustomStatistic::AchEnchantments);
		case CustomStatistic::AchBookcase:           return IsStatisticPresent(CustomStatistic::AchEnchantments);
		case CustomStatistic::AchExploreAllBiomes:   return IsStatisticPresent(CustomStatistic::AchTheEnd);
		case CustomStatistic::AchSpawnWither:        return IsStatisticPresent(CustomStatistic::AchTheEnd2);
		case CustomStatistic::AchKillWither:         return IsStatisticPresent(CustomStatistic::AchSpawnWither);
		case CustomStatistic::AchFullBeacon:         return IsStatisticPresent(CustomStatistic::AchKillWither);
		case CustomStatistic::AchDiamondsToYou:      return IsStatisticPresent(CustomStatistic::AchDiamonds);
		default: UNREACHABLE("Unsupported achievement type");
	}
}





bool StatisticsManager::IsStatisticPresent(const CustomStatistic a_Stat) const
{
	const auto Result = Custom.find(a_Stat);
	if (Result != Custom.end())
	{
		return Result->second > 0;
	}
	return false;
}
