
// VillageGen.h

// Declares the cVillageGen class representing the village generator





#pragma once

#include "GridStructGen.h"
#include "PrefabPiecePool.h"





// fwd:
class cVillagePiecePool;





class cVillageGen :
	public cGridStructGen
{
	typedef cGridStructGen super;
public:
	/** Creates a new instance of the generator with the specified parameters. */
	cVillageGen(
		int a_Seed,
		int a_GridSize,
		int a_MaxOffset,
		int a_MaxDepth,
		int a_MaxSize,
		int a_MinDensity, int a_MaxDensity,
		cBiomeGenPtr a_BiomeGen,
		cTerrainHeightGenPtr a_HeightGen,
		int a_SeaLevel,
		const AStringVector & a_PrefabsToLoad
	);

protected:
	class cVillage;  // fwd: VillageGen.cpp
	typedef std::vector<SharedPtr<cVillagePiecePool> > cVillagePiecePools;

	/** The noise used for generating random numbers */
	cNoise m_Noise;
	
	/** Maximum depth of the generator tree */
	int m_MaxDepth;
	
	/** Maximum size, in X / Z blocks, of the village (radius from the origin) */
	int m_MaxSize;
	
	/** Minimum density - percentage of allowed house connections. Range [0, 100] */
	int m_MinDensity;
	
	/** Maximum density - percentage of allowed house connections. Range [0, 100] */
	int m_MaxDensity;

	/** The underlying biome generator that defines whether the village is created or not */
	cBiomeGenPtr m_BiomeGen;
	
	/** The underlying height generator, used to position the prefabs crossing chunk borders */
	cTerrainHeightGenPtr m_HeightGen;

	/** All available prefab sets. Each village gets one of these chosen randomly. */
	cVillagePiecePools m_Pools;


	// cGridStructGen overrides:
	virtual cStructurePtr CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ) override;
} ;




