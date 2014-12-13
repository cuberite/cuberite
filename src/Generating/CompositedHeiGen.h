
// CompositedHeiGen.h

// Declares the cCompositedHeiGen class representing a cTerrainHeightGen descendant that calculates heightmap of the composited terrain
// This is used to further cache heightmaps for chunks already generated for finishers that require only heightmap information





#pragma once

#include "ComposableGenerator.h"





class cCompositedHeiGen:
	public cTerrainHeightGen
{
public:
	cCompositedHeiGen(cTerrainShapeGenPtr a_ShapeGen, cTerrainCompositionGenPtr a_CompositionGen):
		m_ShapeGen(a_ShapeGen),
		m_CompositionGen(a_CompositionGen)
	{
	}



	// cTerrainheightGen overrides:
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override
	{
		cChunkDesc::Shape shape;
		m_ShapeGen->GenShape(a_ChunkX, a_ChunkZ, shape);
		cChunkDesc desc(a_ChunkX, a_ChunkZ);
		desc.SetHeightFromShape(shape);
		m_CompositionGen->ComposeTerrain(desc, shape);
		memcpy(a_HeightMap, desc.GetHeightMap(), sizeof(a_HeightMap));
	}

protected:
	cTerrainShapeGenPtr m_ShapeGen;
	cTerrainCompositionGenPtr m_CompositionGen;
};




