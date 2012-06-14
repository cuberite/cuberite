
// HeiGen.h

/*
Interfaces to the various height generators:
	- cHeiGenFlat
	- cHeiGenClassic
	- cHeiGenBiomal
*/





#pragma once

#include "cChunkGenerator.h"
#include "cNoise.h"





class cHeiGenFlat :
	public cTerrainHeightGen
{
public:
	cHeiGenFlat(int a_Height) : m_Height(a_Height) {}
	
protected:

	int m_Height;

	// cTerrainHeightGen override:
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override;
} ;





/// A simple cache that stores N most recently generated chunks' heightmaps; N being settable upon creation
class cHeiGenCache :
	public cTerrainHeightGen
{
public:
	cHeiGenCache(cTerrainHeightGen * a_HeiGenToCache, int a_CacheSize);  // Takes ownership of a_HeiGenToCache
	~cHeiGenCache();
	
protected:

	cTerrainHeightGen * m_HeiGenToCache;
	
	struct sCacheData
	{
		int m_ChunkX;
		int m_ChunkZ;
		cChunkDef::HeightMap m_HeightMap;
	} ;
	
	// To avoid moving large amounts of data for the MRU behavior, we MRU-ize indices to an array of the actual data
	int          m_CacheSize;
	int *        m_CacheOrder;  // MRU-ized order, indices into m_CacheData array
	sCacheData * m_CacheData;   // m_CacheData[m_CacheOrder[0]] is the most recently used
	
	// Cache statistics
	int m_NumHits;
	int m_NumMisses;
	int m_TotalChain;  // Number of cache items walked to get to a hit (only added for hits)
	
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override;
} ;





class cHeiGenClassic :
	public cTerrainHeightGen
{
public:
	cHeiGenClassic(int a_Seed, float a_HeightFreq1, float a_HeightAmp1, float a_HeightFreq2, float a_HeightAmp2, float a_HeightFreq3, float a_HeightAmp3);
	
protected:

	int m_Seed;
	cNoise m_Noise;
	float m_HeightFreq1, m_HeightAmp1;
	float m_HeightFreq2, m_HeightAmp2;
	float m_HeightFreq3, m_HeightAmp3;
	
	float GetNoise(float x, float y);

	// cTerrainHeightGen override:
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override;
} ;





class cHeiGenBiomal :
	public cTerrainHeightGen
{
public:
	cHeiGenBiomal(int a_Seed, cBiomeGen & a_BiomeGen) :
		m_Noise(a_Seed),
		m_BiomeGen(a_BiomeGen)
	{
	}
	
protected:

	typedef cChunkDef::BiomeMap BiomeNeighbors[3][3];

	cNoise      m_Noise;
	cBiomeGen & m_BiomeGen;
	
	// Per-biome terrain generator parameters:
	struct sGenParam
	{
		float m_HeightFreq1, m_HeightAmp1;
		float m_HeightFreq2, m_HeightAmp2;
		float m_HeightFreq3, m_HeightAmp3;
		float m_BaseHeight;
	} ;
	static const sGenParam m_GenParam[biNumBiomes];
	
	// cTerrainHeightGen override:
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override;
	
	HEIGHTTYPE GetHeightAt(int a_RelX, int a_RelZ, int a_ChunkX, int a_ChunkZ, const BiomeNeighbors & a_BiomeNeighbors);
} ;




