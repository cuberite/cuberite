
#pragma once

#include "MobProximityCounter.h"
#include "MobFamilyCollecter.h"

class cChunk;
class cMonster;

// This class is used to collect, for each Mob, what is the distance of the closest player
// it was first being designed in order to make mobs spawn / despawn / act
// as the behaviour and even life of mobs depends on the distance to closest player
//
// as side effect : it also collect the chuncks that are elligible for spawning
// as side effect 2 : it also know the caps for mobs number and can compare census to this numbers
class cMobCensus
{
public :
	cMobCensus();

protected :
	cMobProximityCounter m_ProximityCounter;
	cMobFamilyCollecter m_MobFamilyCollecter;

	typedef const std::map<cMonster::eFamily,int> tCapMultipliersMap;
	static tCapMultipliersMap& m_CapMultipliers();

	std::set<cChunk*> m_EligibleForSpawnChunks;

	// count the chunks that are elligible to spawn (for now, the loaded valide not null chuncks)
	int getChunkNb();

public:
	typedef const std::map<cMonster::eFamily,int> tMobSpawnRate;
	static tMobSpawnRate& m_SpawnRate();

	// return the nested proximity counter
	cMobProximityCounter& getProximityCounter();

public :
	// collect an elligible Chunk for Mob Spawning
	// MG TODO : code the correct rule (not loaded chunck but short distant from players)
	void CollectSpawnableChunck(cChunk& a_Chunk);
	
	// collect a mob - it's distance to player, it's family ...
	void CollectMob(cMonster& a_Monster, cChunk& a_Chunk, double a_Distance);

	// return true if the family is caped (i.e. there is more mobs of this family than max)
	bool isCaped(cMonster::eFamily a_MobFamily);

	// log the results of census
	void logd();

protected : 
	static tCapMultipliersMap CapMultiplierInitializerBeforeCx11();
	static tCapMultipliersMap MobSpawnRateInitializerBeforeCx11();
};

