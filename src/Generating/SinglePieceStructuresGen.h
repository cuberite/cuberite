
// PrefabSingleStructureGen.h


/*
Classes to support the generation of single piece prefab structures
*/

#pragma once

#include "Globals.h"

#include "ComposableGenerator.h"
#include "PrefabPiecePool.h"

/** The Single Prefab Structure Generator:
This uses the cGridStructGen to generate the structures on the map
This is similar to the Piece Structure Generator but only placing one possible structure
The Exported cubeset MUST have all possible structures as start structures or the server crashes on generation
else it isn't accessible from the m_Piecepool. */
class cSinglePieceStructuresGen :
	public cFinishGen
{
using Super = cFinishGen;

public:
	cSinglePieceStructuresGen(int a_Seed);

	/** Initializes the generator based on the specified prefab sets.
	a_Prefabs contains the list of prefab sets that should be activated, "|"-separated.
	All problems are logged to the console and the generator skips over them.
	Returns true if at least one prefab set is valid (the generator should be kept). */
	bool Initialize(const AString & a_Prefabs, int a_SeaLevel, const cBiomeGenPtr & a_BiomeGen, const cTerrainHeightGenPtr & a_HeightGen);


	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

protected:
	/** The generator doing the work for a single prefab set.
	Forward-declared so that its implementation changes don't affect the header. */
	class cGen;

	typedef std::shared_ptr<cGen> cGenPtr;
	typedef std::vector<cGenPtr> cGenPtrs;

	/** The individual structure generators, one per piecepool. */
	cGenPtrs m_Gens;

	/** The seed for the random number generator */
	int m_Seed;
};
