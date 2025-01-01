
// CompositedHeiGen.h

// Declares the cCompositedHeiGen class representing a cTerrainHeightGen descendant that calculates heightmap of the composited terrain
// This is used to further cache heightmaps for chunks already generated for finishers that require only heightmap information





#pragma once

#include "ComposableGenerator.h"





class cCompositedHeiGen:
	public cTerrainHeightGen
{
public:
	cCompositedHeiGen(cBiomeGen & a_BiomeGen, cTerrainShapeGen & a_ShapeGen, cTerrainCompositionGen & a_CompositionGen):
		m_BiomeGen(a_BiomeGen),
		m_ShapeGen(a_ShapeGen),
		m_CompositionGen(a_CompositionGen)
	{
	}



	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(cChunkCoords a_ChunkCoords, cChunkDef::HeightMap & a_HeightMap) override
	{
		cChunkDesc::Shape shape;
		m_ShapeGen.GenShape(a_ChunkCoords, shape);
		cChunkDesc desc(a_ChunkCoords);
		m_BiomeGen.GenBiomes(a_ChunkCoords, desc.GetBiomeMap());  // Need to initialize biomes for the composition gen
		desc.SetHeightFromShape(shape);
		m_CompositionGen.ComposeTerrain(desc, shape);
		memcpy(a_HeightMap.data(), desc.GetHeightMap().data(), sizeof(a_HeightMap));
	}

protected:
	cBiomeGen & m_BiomeGen;
	cTerrainShapeGen & m_ShapeGen;
	cTerrainCompositionGen & m_CompositionGen;
};
