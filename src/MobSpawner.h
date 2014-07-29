
#pragma once

#include <set>
#include "BlockID.h"
#include "ChunkDef.h"
#include "Chunk.h"
#include "FastRandom.h"
#include "Mobs/Monster.h"  // This is a side-effect of keeping Mobfamily inside Monster class. I'd prefer to keep both (Mobfamily and Monster) inside a "Monster" namespace MG TODO : do it




// fwd:
class cChunk;





/** This class is used to determine which monster can be spawned in which place
it is essentially static (eg. Squids spawn in water, Zombies spawn in dark places)
but it also has dynamic part depending on the world.ini settings.
*/
class cMobSpawner
{
public :
	// constructor
	// a_MobFamily is the Family of mobs that this spawner will spawn
	// a_AllowedTypes is the set of types allowed for mobs it will spawn. Empty set
	// would result in no spawn at all
	// Allowed mobs thah are not of the right Family will not be include (no warning)
	cMobSpawner(cMonster::eFamily MobFamily, const std::set<cMonster::eType> & a_AllowedTypes);

	/// Check if specified block can be a Pack center for this spawner
	bool CheckPackCenter(BLOCKTYPE a_BlockType);

	// Try to create a monster here
	// if this is the first of a Pack : determine the type of monster
	// BlockType & BlockMeta are used to decide what kind of Mob can Spawn here
	// MaxPackSize is set to the maximal size for a pack this type of mob
	cMonster * TryToSpawnHere(cChunk * a_Chunk, int A_RelX, int a_RelY, int a_RelZ, EMCSBiome a_Biome, int& a_MaxPackSize);

	// mark the beginning of a new Pack
	// all mobs of the same Pack are the same type
	void NewPack(void);

	// return true if there is at least one allowed type
	bool CanSpawnAnything(void);

	typedef const std::set<cMonster *> tSpawnedContainer;
	tSpawnedContainer & getSpawned(void);

protected :
	// return true if specified type of mob can spawn on specified block
	bool CanSpawnHere(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, cMonster::eType a_MobType, EMCSBiome a_Biome);

	// return a random type that can spawn on specified biome.
	// returns E_ENTITY_TYPE_DONOTUSE if none is possible
	cMonster::eType ChooseMobType(EMCSBiome a_Biome);

	// add toAdd inside toAddIn, if toAdd is in m_AllowedTypes
	void addIfAllowed(cMonster::eType toAdd, std::set<cMonster::eType> & toAddIn);

protected :
	cMonster::eFamily m_MonsterFamily;
	std::set<cMonster::eType> m_AllowedTypes;
	bool m_NewPack;
	cMonster::eType m_MobType;
	std::set<cMonster*> m_Spawned;
	cFastRandom m_Random;
} ;




