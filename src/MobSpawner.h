
#pragma once

#include "Chunk.h"
#include "Mobs/Monster.h"  // This is a side-effect of keeping Mobfamily inside Monster class. I'd prefer to keep both (Mobfamily and Monster) inside a "Monster" namespace MG TODO : do it





/** This class is used to determine which monster can be spawned in which place
it is essentially static (eg. Squids spawn in water, Zombies spawn in dark places)
but it also has dynamic part depending on the world.ini settings.
*/
class cMobSpawner
{
public :
	/** Constructor
	a_MobFamily is the Family of mobs that this spawner will spawn
	a_AllowedTypes is the set of types allowed for mobs it will spawn. Empty set would result in no spawn at all
	Allowed mobs thah are not of the right Family will not be include (no warning). */
	cMobSpawner(cMonster::eFamily MobFamily, const std::set<eMonsterType> & a_AllowedTypes);

	/** Check if specified block can be a Pack center for this spawner */
	bool CheckPackCenter(BLOCKTYPE a_BlockType);

	/** Try to create a monster here
	If this is the first of a Pack, determine the type of monster
	a_Biome, BlockType & BlockMeta are used to decide what kind of Mob can Spawn here
	a_MaxPackSize is set to the maximum size for a pack this type of mob */
	cMonster * TryToSpawnHere(cChunk * a_Chunk, Vector3i a_RelPos, EMCSBiome a_Biome, int & a_MaxPackSize);

	/** Mark the beginning of a new Pack.
	All mobs of the same Pack are the same type */
	void NewPack(void);

	// return true if there is at least one allowed type
	bool CanSpawnAnything(void);

	std::vector<std::unique_ptr<cMonster>> & getSpawned(void)
	{
		return m_Spawned;
	}

	/** Returns true if specified type of mob can spawn on specified block */
	static bool CanSpawnHere(cChunk * a_Chunk, Vector3i a_RelPos, eMonsterType a_MobType, EMCSBiome a_Biome, bool a_DisableSolidBelowCheck = false);

	/** Returns all mob types that can spawn that biome */
	static std::set<eMonsterType> GetAllowedMobTypes(EMCSBiome a_Biome);


protected :

	/** Returns a random type that can spawn in the specified biome.
	Returns mtInvalidType if none is possible. */
	eMonsterType ChooseMobType(EMCSBiome a_Biome);

	cMonster::eFamily m_MonsterFamily;
	std::set<eMonsterType> m_AllowedTypes;
	bool m_NewPack;
	eMonsterType m_MobType;
	std::vector<std::unique_ptr<cMonster>> m_Spawned;
} ;




