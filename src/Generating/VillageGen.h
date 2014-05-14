
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
	cVillageGen(int a_Seed, int a_GridSize, int a_MaxDepth, int a_MaxSize, cBiomeGen & a_BiomeGen, cTerrainHeightGen & a_HeightGen);

protected:
	class cVillage;  // fwd: VillageGen.cpp

	/** Maximum depth of the generator tree*/
	int m_MaxDepth;
	
	/** Maximum size, in X/Z blocks, of the village (radius from the origin) */
	int m_MaxSize;

	/** The underlying biome generator that defines whether the village is created or not */
	cBiomeGen & m_BiomeGen;
	
	/** The underlying height generator, used to position the prefabs crossing chunk borders */
	cTerrainHeightGen & m_HeightGen;


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_OriginX, int a_OriginZ) override;
} ;




