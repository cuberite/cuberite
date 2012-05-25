
// CompoGen.h

/* Interfaces to the various terrain composition generators:
	- cCompoGenSameBlock
	- cCompoGenDebugBiomes
	- cCompoGenClassic
*/





#pragma once

#include "cChunkGenerator.h"





class cCompoGenSameBlock :
	public cTerrainCompositionGen
{
public:
	cCompoGenSameBlock(BLOCKTYPE a_BlockType, bool a_IsBedrocked) :
		m_BlockType(a_BlockType),
		m_IsBedrocked(a_IsBedrocked)
	{}
	
protected:

	BLOCKTYPE m_BlockType;
	bool      m_IsBedrocked;
	
	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
		cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
		const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
		cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
		cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
	) override;
} ;





class cCompoGenDebugBiomes :
	public cTerrainCompositionGen
{
public:
	cCompoGenDebugBiomes(cBiomeGen * a_BiomeGen) : m_BiomeGen(a_BiomeGen) {}
	
protected:

	cBiomeGen * m_BiomeGen;
	
	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
		cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
		const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
		cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
		cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
	) override;
} ;





class cCompoGenClassic :
	public cTerrainCompositionGen
{
public:
	cCompoGenClassic(int a_SeaLevel, int a_BeachHeight, int a_BeachDepth);
	
protected:

	int m_SeaLevel;
	int m_BeachHeight;
	int m_BeachDepth;
	
	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
		cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
		const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
		cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
		cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
	) override;
} ;




