
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobCensus.h"





void cMobCensus::CollectMob(cMonster & a_Monster, cChunk & a_Chunk, double a_Distance)
{
	m_ProximityCounter.CollectMob(a_Monster, a_Chunk, a_Distance);
	m_MobFamilyCollecter.CollectMob(a_Monster);
}





bool cMobCensus::IsCapped(cMonster::eFamily a_MobFamily)
{
	const int ratio = 319;  // This should be 256 as we are only supposed to take account from chunks that are in 17 x 17 from a player
	// but for now, we use all chunks loaded by players. that means 19 x 19 chunks. That's why we use 256 * (19 * 19) / (17 * 17) = 319
	// MG TODO : code the correct count
	const auto MobCap = ((GetCapMultiplier(a_MobFamily) * GetNumChunks()) / ratio);
	return (MobCap < m_MobFamilyCollecter.GetNumberOfCollectedMobs(a_MobFamily));
}





int cMobCensus::GetCapMultiplier(cMonster::eFamily a_MobFamily)
{
	switch (a_MobFamily)
	{
		case cMonster::mfHostile: return 79;
		case cMonster::mfPassive: return 11;
		case cMonster::mfAmbient: return 16;
		case cMonster::mfWater:   return 5;
		case cMonster::mfNoSpawn:
		case cMonster::mfUnhandled:
		{
			ASSERT(!"Unhandled mob family");
			return -1;
		}
	}
	UNREACHABLE("Unsupported mob family");
}





void cMobCensus::CollectSpawnableChunk(cChunk & a_Chunk)
{
	m_EligibleForSpawnChunks.insert(&a_Chunk);
}





int cMobCensus::GetNumChunks(void)
{
	return static_cast<int>(m_EligibleForSpawnChunks.size());
}





cMobProximityCounter & cMobCensus::GetProximityCounter(void)
{
	return m_ProximityCounter;
}





void cMobCensus::Logd()
{
	LOGD("Hostile mobs : %d %s", m_MobFamilyCollecter.GetNumberOfCollectedMobs(cMonster::mfHostile), IsCapped(cMonster::mfHostile) ? "(capped)" : "");
	LOGD("Ambient mobs : %d %s", m_MobFamilyCollecter.GetNumberOfCollectedMobs(cMonster::mfAmbient), IsCapped(cMonster::mfAmbient) ? "(capped)" : "");
	LOGD("Water mobs   : %d %s", m_MobFamilyCollecter.GetNumberOfCollectedMobs(cMonster::mfWater),   IsCapped(cMonster::mfWater)   ? "(capped)" : "");
	LOGD("Passive mobs : %d %s", m_MobFamilyCollecter.GetNumberOfCollectedMobs(cMonster::mfPassive), IsCapped(cMonster::mfPassive) ? "(capped)" : "");
}





