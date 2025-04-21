
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
#include "../Noise/Noise.h"





class cCompoGenSameBlock :
	public cTerrainCompositionGen
{
public:
	cCompoGenSameBlock(void) :
		m_Block(Block::Stone::Stone()),
		m_IsBedrocked(true)
	{}

protected:

	BlockState m_Block;
	bool       m_IsBedrocked;

	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeCompoGen(cIniFile & a_IniFile) override;
} ;





class cCompoGenDebugBiomes :
	public cTerrainCompositionGen
{
public:
	cCompoGenDebugBiomes(void) {}

protected:

	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override;
} ;





class cCompoGenClassic :
	public cTerrainCompositionGen
{
public:
	cCompoGenClassic(void);

protected:

	int m_SeaLevel;
	int m_BeachHeight;
	int m_BeachDepth;
	BlockState m_BlockTop;
	BlockState m_BlockMiddle;
	BlockState m_BlockBottom;
	BlockState m_BlockBeach;
	BlockState m_BlockBeachBottom;
	BlockState m_BlockSea;

	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeCompoGen(cIniFile & a_IniFile) override;
} ;





class cCompoGenNether :
	public cTerrainCompositionGen
{
public:
	cCompoGenNether(int a_Seed);

protected:
	cNoise m_Noise1;
	cNoise m_Noise2;

	double m_MaxThreshold;

	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeCompoGen(cIniFile & a_IniFile) override;
} ;





/** Caches most-recently-used chunk composition of another composition generator. Caches only the types and metas */
class cCompoGenCache :
	public cTerrainCompositionGen
{
public:
	cCompoGenCache(std::unique_ptr<cTerrainCompositionGen> a_Underlying, int a_CacheSize);
	virtual ~cCompoGenCache() override;

	// cTerrainCompositionGen override:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeCompoGen(cIniFile & a_IniFile) override;

protected:

	std::unique_ptr<cTerrainCompositionGen> m_Underlying;

	struct sCacheData
	{
		int m_ChunkX;
		int m_ChunkZ;
		cChunkDef::BlockStates       m_BlockTypes;
		cChunkDef::HeightMap         m_HeightMap;
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
