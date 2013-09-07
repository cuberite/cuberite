
#pragma once

#include <set>
#include "BlockID.h"
#include "ChunkDef.h"
#include "FastRandom.h"
#include "Mobs/Monster.h" //this is a side-effect of keeping Mobfamily inside Monster class. I'd prefer to keep both (Mobfamily and Monster) inside a "Monster" namespace MG TODO : do it

class cChunk;


// This class is used to determine wich monster can be spawned on wich place
// it is essentially static (f.i. Squids spawn in water, Zombie spawn in dark places)
// but it also has dynamic part depending on the world.ini
class cMobSpawner
{
public :
	// constructor
	// a_MobFamily is the Family of mobs that this spawner will spawn
	// a_AllowedTypes is the set of types allowed for mobs it will spawn. Empty set
	// would result in no spawn at all
	// Allowed mobs thah are not of the right Family will not be include (no warning)
	cMobSpawner(cMonster::eFamily MobFamily, const std::set<cMonster::eType>& a_AllowedTypes);

	// Check if specified block can be a Pack center for this spawner
	bool CheckPackCenter(BLOCKTYPE a_BlockType);

	// Try to create a monster here
	// if this is the first of a Pack : determine the type of monster
	// BlockType & BlockMeta are use to know what kind of Mob can Spawn here
	// MaxPackSize is set to the maximal size for a pack this type of mob
	cMonster* TryToSpawnHere(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, BLOCKTYPE a_BlockType_below, NIBBLETYPE a_BlockMeta_below, BLOCKTYPE a_BlockType_above, NIBBLETYPE a_BlockMeta_above, EMCSBiome a_Biome, int a_Level, int& a_MaxPackSize);

	// mark the beginning of a new Pack
	// all mobs of the same Pack are the same type
	void NewPack();

	// return true if there is at least one allowed type
	bool CanSpawnSomething();

	typedef const std::set<cMonster*> tSpawnedContainer;
	tSpawnedContainer& getSpawned();

protected : 
	// return true if specified type of mob can spawn on specified block
	bool CanSpawnHere(cMonster::eType a_MobType, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, BLOCKTYPE a_BlockType_below, NIBBLETYPE a_BlockMeta_below, BLOCKTYPE a_BlockType_above, NIBBLETYPE a_BlockMeta_above, EMCSBiome a_Biome, int a_Level);

	// return a random type that can spawn on specified biome.
	// returns E_ENTITY_TYPE_DONOTUSE if none is possible
	cMonster::eType ChooseMobType(EMCSBiome a_Biome);

	// add toAdd inside toAddIn, if toAdd is in m_AllowedTypes
	void addIfAllowed(cMonster::eType toAdd, std::set<cMonster::eType>& toAddIn);

protected : 
	cMonster::eFamily m_MonsterFamily;
	std::set<cMonster::eType> m_AllowedTypes;
	bool m_NewPack;
	cMonster::eType m_MobType;
	std::set<cMonster*> m_Spawned;
	cFastRandom m_Random;

public : 
	typedef const std::set<cMonster::eType> tMobTypes; // MG TODO : maybe relocate all those statics set/maps in the same place ?
	static tMobTypes& m_MobTypes();

protected : 
	static tMobTypes initMobTypesBeforeCx11();


};
