
// PieceStructuresGen.h

// Declares the cPieceStructuresGen class representing the PieceStructures finisher generator

/*
This generator loads various pieces from "piecepool" files, and each such piecepool is then used in a separate
cPieceGenerator instance.
*/





#pragma once

#include "ComposableGenerator.h"
#include "PrefabPiecePool.h"





class cPieceStructuresGen:
	public cFinishGen
{
	using Super = cFinishGen;

public:

	cPieceStructuresGen(int a_Seed);

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




