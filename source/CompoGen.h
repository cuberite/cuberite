
// CompoGen.h

/* Interfaces to the various terrain composition generators:
	- cCompoGenSameBlock
	- cCompoGenDebugBiomes
	- cCompoGenClassic
	- cCompoGenBiomal
*/





#pragma once

#include "cChunkGenerator.h"
#include "cNoise.h"





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
		const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
		cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
		cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
	) override;
} ;





class cCompoGenDebugBiomes :
	public cTerrainCompositionGen
{
public:
	cCompoGenDebugBiomes(void) {}
	
protected:
	
	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
		cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
		const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
		const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
		cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
		cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
	) override;
} ;





class cCompoGenClassic :
	public cTerrainCompositionGen
{
public:
	cCompoGenClassic(
		int a_SeaLevel, int a_BeachHeight, int a_BeachDepth,
		BLOCKTYPE a_BlockTop, BLOCKTYPE a_BlockMiddle, BLOCKTYPE a_BlockBottom,
		BLOCKTYPE a_BlockBeach, BLOCKTYPE a_BlockBeachBottom, BLOCKTYPE a_BlockSea
	);
	
protected:

	int m_SeaLevel;
	int m_BeachHeight;
	int m_BeachDepth;
	BLOCKTYPE m_BlockTop;
	BLOCKTYPE m_BlockMiddle;
	BLOCKTYPE m_BlockBottom;
	BLOCKTYPE m_BlockBeach;
	BLOCKTYPE m_BlockBeachBottom;
	BLOCKTYPE m_BlockSea;
	
	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
		cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
		const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
		const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
		cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
		cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
	) override;
} ;





class cCompoGenBiomal :
	public cTerrainCompositionGen
{
public:
	cCompoGenBiomal(int a_Seed, int a_SeaLevel) :
		m_Noise(a_Seed + 1000),
		m_SeaLevel(a_SeaLevel - 1)  // we do an adjustment later in filling the terrain with water
	{
	}
	
protected:

	cNoise m_Noise;
	int    m_SeaLevel;
	
	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(
		int a_ChunkX, int a_ChunkZ,
		cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
		cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
		const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
		const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
		cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
		cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
	) override;
	
	void FillColumnGrass    (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	void FillColumnSand     (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	void FillColumnMycelium (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	void FillColumnWaterSand(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	
	void FillColumnWaterMix (int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	
	void FillColumnPattern  (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes, const BLOCKTYPE * a_Pattern, int a_PatternSize);
} ;




