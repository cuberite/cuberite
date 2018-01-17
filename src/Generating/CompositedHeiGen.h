
// CompositedHeiGen.h

// Declares the cCompositedHeiGen class representing a cTerrainHeightGen descendant that calculates heightmap of the composited terrain
// This is used to further cache heightmaps for chunks already generated for finishers that require only heightmap information





#pragma once

#include "ComposableGenerator.h"





class cCompositedHeiGen:
	public cTerrainHeightGen
{
public:
	cCompositedHeiGen(cBiomeGenPtr a_BiomeGen, cTerrainShapeGenPtr a_ShapeGen, cTerrainCompositionGenPtr a_CompositionGen):
		m_BiomeGen(a_BiomeGen),
		m_ShapeGen(a_ShapeGen),
		m_CompositionGen(a_CompositionGen)
	{
	}



	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override
	{
		cChunkDesc::Shape shape;
		m_ShapeGen->GenShape(a_ChunkX, a_ChunkZ, shape);
		cChunkDesc desc(a_ChunkX, a_ChunkZ);
		m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, desc.GetBiomeMap());	  // Need to initialize biomes for the composition gen
		desc.SetHeightFromShape(shape);
		m_CompositionGen->ComposeTerrain(desc, shape);
		memcpy(a_HeightMap, desc.GetHeightMap(), sizeof(a_HeightMap));
	}

protected:
	cBiomeGenPtr m_BiomeGen;
	cTerrainShapeGenPtr m_ShapeGen;
	cTerrainCompositionGenPtr m_CompositionGen;
};




