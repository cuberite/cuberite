
// Statistics.cpp

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Statistics.h"





void cStatManager::SetValue(const Statistic a_Stat, const StatValue a_Value)
{
	m_CustomStatistics[a_Stat] = a_Value;
}





cStatManager::StatValue cStatManager::AddValue(const Statistic a_Stat, const StatValue a_Delta)
{
	return m_CustomStatistics[a_Stat] += a_Delta;
}





bool cStatManager::SatisfiesPrerequisite(const Statistic a_Stat)
{
	switch (a_Stat)
	{
		case Statistic::AchMineWood:           return IsStatisticPresent(Statistic::AchOpenInventory);
		case Statistic::AchBuildWorkBench:     return IsStatisticPresent(Statistic::AchMineWood);
		case Statistic::AchBuildHoe:           return IsStatisticPresent(Statistic::AchBuildWorkBench);
		case Statistic::AchBakeCake:           return IsStatisticPresent(Statistic::AchBuildHoe);
		case Statistic::AchMakeBread:          return IsStatisticPresent(Statistic::AchBuildHoe);
		case Statistic::AchBuildSword:         return IsStatisticPresent(Statistic::AchBuildWorkBench);
		case Statistic::AchKillCow:            return IsStatisticPresent(Statistic::AchBuildSword);
		case Statistic::AchFlyPig:             return IsStatisticPresent(Statistic::AchKillCow);
		case Statistic::AchBreedCow:           return IsStatisticPresent(Statistic::AchKillCow);
		case Statistic::AchKillEnemy:          return IsStatisticPresent(Statistic::AchBuildSword);
		case Statistic::AchSnipeSkeleton:      return IsStatisticPresent(Statistic::AchKillEnemy);
		case Statistic::AchBuildPickaxe:       return IsStatisticPresent(Statistic::AchBuildWorkBench);
		case Statistic::AchBuildBetterPickaxe: return IsStatisticPresent(Statistic::AchBuildPickaxe);
		case Statistic::AchBuildFurnace:       return IsStatisticPresent(Statistic::AchBuildWorkBench);
		case Statistic::AchCookFish:           return IsStatisticPresent(Statistic::AchBuildFurnace);
		case Statistic::AchAcquireIron:        return IsStatisticPresent(Statistic::AchBuildFurnace);
		case Statistic::AchOnARail:            return IsStatisticPresent(Statistic::AchAcquireIron);
		case Statistic::AchDiamonds:           return IsStatisticPresent(Statistic::AchAcquireIron);
		case Statistic::AchPortal:             return IsStatisticPresent(Statistic::AchDiamonds);
		case Statistic::AchGhast:              return IsStatisticPresent(Statistic::AchPortal);
		case Statistic::AchBlazeRod:           return IsStatisticPresent(Statistic::AchPortal);
		case Statistic::AchPotion:             return IsStatisticPresent(Statistic::AchBlazeRod);
		case Statistic::AchTheEnd:             return IsStatisticPresent(Statistic::AchBlazeRod);
		case Statistic::AchTheEnd2:            return IsStatisticPresent(Statistic::AchTheEnd);
		case Statistic::AchEnchantments:       return IsStatisticPresent(Statistic::AchDiamonds);
		case Statistic::AchOverkill:           return IsStatisticPresent(Statistic::AchEnchantments);
		case Statistic::AchBookcase:           return IsStatisticPresent(Statistic::AchEnchantments);
		case Statistic::AchExploreAllBiomes:   return IsStatisticPresent(Statistic::AchTheEnd);
		case Statistic::AchSpawnWither:        return IsStatisticPresent(Statistic::AchTheEnd2);
		case Statistic::AchKillWither:         return IsStatisticPresent(Statistic::AchSpawnWither);
		case Statistic::AchFullBeacon:         return IsStatisticPresent(Statistic::AchKillWither);
		case Statistic::AchDiamondsToYou:      return IsStatisticPresent(Statistic::AchDiamonds);
		default: return true;
	}
}





bool cStatManager::IsStatisticPresent(const Statistic a_Stat) const
{
	const auto Result = m_CustomStatistics.find(a_Stat);
	if (Result != m_CustomStatistics.end())
	{
		return Result->second > 0;
	}
	return false;
}
