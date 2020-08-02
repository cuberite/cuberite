
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
		m_BiomeGen(std::move(a_BiomeGen)),
		m_ShapeGen(std::move(a_ShapeGen)),
		m_CompositionGen(std::move(a_CompositionGen))
	{
	}



	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(cChunkCoords a_ChunkCoords, cChunkDef::HeightMap & a_HeightMap) override
	{
		// Wrap the cChunkDesc::Shape into a struct so that we can make a unique_ptr out of it:
		struct ShapeHelper
		{
			cChunkDesc::Shape mShape;
		};
		auto shape = std::make_unique<ShapeHelper>();
		m_ShapeGen->GenShape(a_ChunkCoords, shape->mShape);
		auto desc = std::make_unique<cChunkDesc>(a_ChunkCoords);
		m_BiomeGen->GenBiomes(a_ChunkCoords, desc->GetBiomeMap());  // Need to initialize biomes for the composition gen
		desc->SetHeightFromShape(shape->mShape);
		m_CompositionGen->ComposeTerrain(*desc, shape->mShape);
		memcpy(a_HeightMap, desc->GetHeightMap(), sizeof(a_HeightMap));
	}

protected:
	cBiomeGenPtr m_BiomeGen;
	cTerrainShapeGenPtr m_ShapeGen;
	cTerrainCompositionGenPtr m_CompositionGen;
};




