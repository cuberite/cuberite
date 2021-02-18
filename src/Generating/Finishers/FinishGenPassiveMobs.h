
// FinishGenPassiveMobs.h

#pragma once

#include "../ComposableGenerator.h"
#include "../../Mobs/MonsterTypes.h"

/** This class populates generated chunks with packs of biome-dependant animals
Animals: cows, sheep, pigs, mooshrooms, squid, horses, wolves, ocelots */
class cFinishGenPassiveMobs :
	public cFinishGen
{
public:

	cFinishGenPassiveMobs(int a_Seed, cIniFile & a_IniFile, eDimension a_Dimension);

protected:

	/** The noise used as the source of randomness */
	cNoise m_Noise;

	/** Chance, [0..100], that an animal pack will be generated in a chunk */
	int m_AnimalProbability;


	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	/** Returns false if an animal cannot spawn at given coords, else adds it to the chunk's entity list and returns true */
	bool TrySpawnAnimals(cChunkDesc & a_ChunkDesc, int x, int y, int z, eMonsterType AnimalToSpawn);

	/** Picks a random animal from biome-dependant list for a random position in the chunk.
	Returns the chosen mob type, or mtInvalid if no mob chosen. */
	eMonsterType GetRandomMob(cChunkDesc & a_ChunkDesc);
} ;
