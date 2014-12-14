
#include "Mobs/Monster.h"


// CreatureSpawner.h

// This class handle the mob spawning in a world.





#pragma once

class cWorld;
class cMonster;

// fwd:
class cCreatureSpawner;





class cCreatureSpawner
{
public:

	cCreatureSpawner(cWorld * a_World);

	void LoadAllowedTypes(const std::set<eMonsterType> & a_AllowedTypes);

	int SpawnRandomCreatures(bool a_SpawnMonsters, bool a_SpawnAnimals, bool a_SpawnWater);

	int SpawnRandomCreatures(cMonster::eFamily a_Family);

	int GetSpawnLimit(cMonster::eFamily a_Family) const;

	bool CanSpawnHere(int a_BlockX, int a_BlockY, int a_BlockZ, eMonsterType a_MobType);

	bool CanSpawnFamily(int a_BlockX, int a_BlockY, int a_BlockZ, cMonster::eFamily a_Family);

	/** Return a random type that can spawn on specified biome.
	Returns mtInvalidType if none is possible */
	eMonsterType ChooseMobType(EMCSBiome a_Biome, cMonster::eFamily a_Family);

protected:
	
	void UpdateChunks(void);

	bool IsPlayerInNear(int a_BlockX, int a_BlockY, int a_BlockZ);

	// Add ToAdd inside ToAddIn, if ToAdd is in m_AllowedTypes
	void AddIfAllowed(eMonsterType ToAdd, std::vector<eMonsterType> & ToAddIn, cMonster::eFamily a_Family);

	std::set<eMonsterType> m_AllowedTypes;

	cChunkCoordsList m_Chunks;

	cWorld * m_World;
} ;




