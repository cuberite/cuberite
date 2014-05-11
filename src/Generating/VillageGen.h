
// VillageGen.h

// Declares the cVillageGen class representing the village generator





#pragma once

#include "GridStructGen.h"
#include "PrefabPiecePool.h"





class cVillageGen :
	public cGridStructGen
{
	typedef cGridStructGen super;
public:
	cVillageGen(int a_Seed, int a_GridSize, cBiomeGen & a_BiomeGen, cTerrainHeightGen & a_HeightGen);

protected:
	class cVillage;  // fwd: VillageGen.cpp

	/** The prefabs for the sand village. We're not exactly using the cPiecePool functionality, only the containment. */
	static cPrefabPiecePool m_SandVillage;

	/** The prefabs for the plains village. We're not exactly using the cPiecePool functionality, only the containment. */
	static cPrefabPiecePool m_PlainsVillage;

	/** The underlying biome generator that defines whether the village is created or not */
	cBiomeGen & m_BiomeGen;
	
	/** The underlying height generator, used to position the prefabs crossing chunk borders */
	cTerrainHeightGen & m_HeightGen;


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_OriginX, int a_OriginZ) override;
} ;




