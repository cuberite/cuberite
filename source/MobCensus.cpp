
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobCensus.h"



cMobCensus::tCapMultipliersMap cMobCensus::CapMultiplierInitializerBeforeCx11()
{
	std::map<cMonster::eFamily,int> toReturn;
	toReturn[cMonster::mfHostile] = 79;
	toReturn[cMonster::mfPassive] = 11;
	toReturn[cMonster::mfAmbient] = 16;
	toReturn[cMonster::mfWater] = 5;
	return toReturn;
}

cMobCensus::tMobSpawnRate cMobCensus::MobSpawnRateInitializerBeforeCx11()
{
	std::map<cMonster::eFamily,int> toReturn;
	toReturn[cMonster::mfHostile] = 1;
	toReturn[cMonster::mfPassive] = 400;
	toReturn[cMonster::mfAmbient] = 400;
	toReturn[cMonster::mfWater] = 400;
	return toReturn;
}

cMobCensus::tCapMultipliersMap& cMobCensus::m_CapMultipliers()
{
	static tCapMultipliersMap* value = new tCapMultipliersMap(CapMultiplierInitializerBeforeCx11());
	return *value;
}

cMobCensus::tMobSpawnRate& cMobCensus::m_SpawnRate()
{
	static tMobSpawnRate* value = new tMobSpawnRate(MobSpawnRateInitializerBeforeCx11());
	return *value;
}

cMobCensus::cMobCensus()
{
}	

void cMobCensus::CollectMob(cMonster& a_Monster, cChunk& a_Chunk, double a_Distance)
{
	m_ProximityCounter.CollectMob(a_Monster,a_Chunk,a_Distance);
	m_MobFamilyCollecter.CollectMob(a_Monster);
}

bool cMobCensus::isCaped(cMonster::eFamily a_MobFamily)
{
	bool toReturn = true;
	const int ratio = 319; // this should be 256 as we are only supposed to take account from chunks that are in 17x17 from a player
	// but for now, we use all chunks loaded by players. that means 19 x 19 chucks. That's why we use 256 * (19*19) / (17*17) = 319
	// MG TODO : code the correct count	
	tCapMultipliersMap::const_iterator capMultiplier = m_CapMultipliers().find(a_MobFamily);
	if	(
		(capMultiplier != m_CapMultipliers().end()) &&
		(capMultiplier->second * getChunkNb()) / ratio >= m_MobFamilyCollecter.getNumberOfCollectedMobs(a_MobFamily)
		)
	{
		toReturn = false;
	}
	return toReturn;
}

void cMobCensus::CollectSpawnableChunk(cChunk& a_Chunk)
{
	m_EligibleForSpawnChunks.insert(&a_Chunk);
}

int cMobCensus::getChunkNb()
{
	return m_EligibleForSpawnChunks.size();
}

cMobProximityCounter& cMobCensus::getProximityCounter()
{
	return m_ProximityCounter;
}


void cMobCensus::logd()
{
	LOGD((std::string("Hostile mobs : %d") + (isCaped(cMonster::mfHostile)?"(capped)":"")).c_str(),m_MobFamilyCollecter.getNumberOfCollectedMobs(cMonster::mfHostile));
	LOGD((std::string("Ambiant mobs : %d") + (isCaped(cMonster::mfAmbient)?"(capped)":"")).c_str(),m_MobFamilyCollecter.getNumberOfCollectedMobs(cMonster::mfAmbient));
	LOGD((std::string("Water mobs   : %d") + (isCaped(cMonster::mfWater)?  "(capped)":"")).c_str(),m_MobFamilyCollecter.getNumberOfCollectedMobs(cMonster::mfWater));
	LOGD((std::string("Passive mobs : %d") + (isCaped(cMonster::mfPassive)?"(capped)":"")).c_str(),m_MobFamilyCollecter.getNumberOfCollectedMobs(cMonster::mfPassive));
}
