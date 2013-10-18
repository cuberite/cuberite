
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobCensus.h"





cMobCensus::tCapMultipliersMap cMobCensus::CapMultiplierInitializerBeforeCx11(void)
{
	std::map<cMonster::eFamily,int> toReturn;
	toReturn[cMonster::mfHostile] = 79;
	toReturn[cMonster::mfPassive] = 11;
	toReturn[cMonster::mfAmbient] = 16;
	toReturn[cMonster::mfWater] = 5;
	return toReturn;
}





cMobCensus::tMobSpawnRate cMobCensus::MobSpawnRateInitializerBeforeCx11(void)
{
	std::map<cMonster::eFamily,int> toReturn;
	toReturn[cMonster::mfHostile] = 1;
	toReturn[cMonster::mfPassive] = 400;
	toReturn[cMonster::mfAmbient] = 400;
	toReturn[cMonster::mfWater] = 400;
	return toReturn;
}





cMobCensus::tCapMultipliersMap & cMobCensus::m_CapMultipliers(void)
{
	// TODO: This memory leaks:
	static tCapMultipliersMap * value = new tCapMultipliersMap(CapMultiplierInitializerBeforeCx11());
	return *value;
}





cMobCensus::tMobSpawnRate & cMobCensus::m_SpawnRate(void)
{
	// TODO: This memory leaks:
	static tMobSpawnRate* value = new tMobSpawnRate(MobSpawnRateInitializerBeforeCx11());
	return *value;
}





void cMobCensus::CollectMob(cMonster & a_Monster, cChunk & a_Chunk, double a_Distance)
{
	m_ProximityCounter.CollectMob(a_Monster,a_Chunk,a_Distance);
	m_MobFamilyCollecter.CollectMob(a_Monster);
}





bool cMobCensus::IsCapped(cMonster::eFamily a_MobFamily)
{
	bool toReturn = true;
	const int ratio = 319; // this should be 256 as we are only supposed to take account from chunks that are in 17x17 from a player
	// but for now, we use all chunks loaded by players. that means 19 x 19 chunks. That's why we use 256 * (19*19) / (17*17) = 319
	// MG TODO : code the correct count	
	tCapMultipliersMap::const_iterator capMultiplier = m_CapMultipliers().find(a_MobFamily);
	if (
		(capMultiplier != m_CapMultipliers().end()) &&
		((capMultiplier->second * GetNumChunks()) / ratio >= m_MobFamilyCollecter.GetNumberOfCollectedMobs(a_MobFamily))
	)
	{
		return false;
	}
	return true;
}





void cMobCensus::CollectSpawnableChunk(cChunk & a_Chunk)
{
	m_EligibleForSpawnChunks.insert(&a_Chunk);
}





int cMobCensus::GetNumChunks(void)
{
	return m_EligibleForSpawnChunks.size();
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




