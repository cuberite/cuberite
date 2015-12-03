
// PieceStructuresGen.h

// Declares the cPieceStructuresGen class representing the PieceStructures finisher generator

/*
This generator loads various pieces from "piecepool" files, and each such piecepool is then used in a separate
cPieceGenerator instance.
*/





#pragma once

#include "ComposableGenerator.h"
#include "PrefabPiecePool.h"
#include "GridStructGen.h"





class cPieceStructuresGen :
	public cFinishGen
{
	typedef cFinishGen Super;

public:
	cPieceStructuresGen(int a_Seed);

	/** Initializes the generator based on the specified prefab sets.
	a_Prefabs contains the list of prefab sets that should be activated, "|"-separated.
	All problems are logged to the console and the generator skips over them.
	Returns true if at least one prefab set is valid (the generator should be kept). */
	bool Initialize(const AString & a_Prefabs, int a_SeaLevel, cBiomeGenPtr a_BiomeGen, cTerrainHeightGenPtr a_HeightGen);

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

protected:
	class cGen:
		public cGridStructGen
	{
		typedef cGridStructGen Super;
	public:
		cGen(int a_Seed, cBiomeGenPtr a_BiomeGen, cTerrainHeightGenPtr a_HeightGen, int a_SeaLevel, const AString & a_Name);

		/** Loads the piecepool from a file.
		Returns true on success, logs warning and returns false on failure. */
		bool LoadFromFile(const AString & a_FileName);

		// cGridStructGen overrides:
		virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;

	protected:

		/** The underlying biome generator that defines whether the structure is created or not */
		cBiomeGenPtr m_BiomeGen;
	
		/** The underlying height generator, used to position the prefabs crossing chunk borders if they are set to FitGround. */
		cTerrainHeightGenPtr m_HeightGen;

		/** The world's sea level, if available. Used for some cVerticalStrategy descendants. */
		int m_SeaLevel;

		/** The name that is used for reporting. */
		AString m_Name;

		/** All available prefabs. */
		cPrefabPiecePool m_Pool;

		/** Maximum depth of the generated piece tree. */
		int m_MaxDepth;
	};

	typedef SharedPtr<cGen> cGenPtr;
	typedef std::vector<cGenPtr> cGenPtrs;


	/** The individual structure generators, one per piecepool. */
	cGenPtrs m_Gens;

	/** The seed for the random number generator */
	int m_Seed;
};




