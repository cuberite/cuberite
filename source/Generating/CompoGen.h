
// CompoGen.h

/* Interfaces to the various terrain composition generators:
	- cCompoGenSameBlock
	- cCompoGenDebugBiomes
	- cCompoGenClassic
	- cCompoGenBiomal
	- cCompoGenNether
	- cCompoGenCache
*/





#pragma once

#include "ComposableGenerator.h"
#include "../Noise.h"





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
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) override;
} ;





class cCompoGenDebugBiomes :
	public cTerrainCompositionGen
{
public:
	cCompoGenDebugBiomes(void) {}
	
protected:
	
	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) override;
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
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) override;
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
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) override;
	
	void FillColumnGrass    (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	void FillColumnSand     (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	void FillColumnMycelium (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	void FillColumnWaterSand(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	void FillColumnWaterDirt(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes);
	
	void FillColumnPattern  (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes, const BLOCKTYPE * a_Pattern, int a_PatternSize);
} ;





class cCompoGenNether :
	public cTerrainCompositionGen
{
public:
	cCompoGenNether(int a_Seed);
	
protected:
	cNoise m_Noise1;
	cNoise m_Noise2;
	
	int m_Threshold;
	
	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) override;
} ;





/// Caches most-recently-used chunk composition of another composition generator. Caches only the types and metas
class cCompoGenCache :
	public cTerrainCompositionGen
{
public:
	cCompoGenCache(cTerrainCompositionGen * a_Underlying, int a_CacheSize);  // Doesn't take ownership of a_Underlying
	~cCompoGenCache();
	
	// cTerrainCompositionGen override:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) override;
	
protected:

	cTerrainCompositionGen * m_Underlying;
	
	struct sCacheData
	{
		int m_ChunkX;
		int m_ChunkZ;
		cChunkDef::BlockTypes        m_BlockTypes;
		cChunkDesc::BlockNibbleBytes m_BlockMetas;  // The metas are uncompressed, 1 meta per byte
	} ;
	
	// To avoid moving large amounts of data for the MRU behavior, we MRU-ize indices to an array of the actual data
	int          m_CacheSize;
	int *        m_CacheOrder;  // MRU-ized order, indices into m_CacheData array
	sCacheData * m_CacheData;   // m_CacheData[m_CacheOrder[0]] is the most recently used
	
	// Cache statistics
	int m_NumHits;
	int m_NumMisses;
	int m_TotalChain;  // Number of cache items walked to get to a hit (only added for hits)
} ;




