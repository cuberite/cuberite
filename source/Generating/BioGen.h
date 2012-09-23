
// BioGen.h

/*
Interfaces to the various biome generators:
	- cBioGenConstant
	- cBioGenCheckerboard
	- cBioGenDistortedVoronoi
*/





#pragma once

#include "ChunkGenerator.h"
#include "../cNoise.h"





class cBioGenConstant :
	public cBiomeGen
{
public:
	cBioGenConstant(EMCSBiome a_Biome) : m_Biome(a_Biome) {}
	
protected:

	EMCSBiome m_Biome;

	// cBiomeGen override:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
} ;





/// A simple cache that stores N most recently generated chunks' biomes; N being settable upon creation
class cBioGenCache :
	public cBiomeGen
{
public:
	cBioGenCache(cBiomeGen * a_BioGenToCache, int a_CacheSize);  // Takes ownership of a_BioGenToCache
	~cBioGenCache();
	
protected:

	cBiomeGen * m_BioGenToCache;
	
	struct sCacheData
	{
		int m_ChunkX;
		int m_ChunkZ;
		cChunkDef::BiomeMap m_BiomeMap;
	} ;
	
	// To avoid moving large amounts of data for the MRU behavior, we MRU-ize indices to an array of the actual data
	int          m_CacheSize;
	int *        m_CacheOrder;  // MRU-ized order, indices into m_CacheData array
	sCacheData * m_CacheData;   // m_CacheData[m_CacheOrder[0]] is the most recently used
	
	// Cache statistics
	int m_NumHits;
	int m_NumMisses;
	int m_TotalChain;  // Number of cache items walked to get to a hit (only added for hits)
	
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
} ;





/// Base class for generators that use a list of available biomes. This class takes care of the list.
class cBiomeGenList :
	public cBiomeGen
{
protected:
	cBiomeGenList(const AString & a_Biomes)
	{
		InitializeBiomes(a_Biomes);
	}
	
	// List of biomes that the generator is allowed to generate:
	typedef std::vector<EMCSBiome> EMCSBiomes;
	EMCSBiomes m_Biomes;
	int        m_BiomesCount;  // Pulled out of m_Biomes for faster access
	
	void InitializeBiomes(const AString & a_Biomes);
	
} ;





class cBioGenCheckerboard :
	public cBiomeGenList
{
public:
	cBioGenCheckerboard(int a_BiomeSize, const AString & a_Biomes) :
		cBiomeGenList(a_Biomes),
		m_BiomeSize((a_BiomeSize < 8) ? 8 : a_BiomeSize)
	{
	}
	
protected:

	int m_BiomeSize;
	
	// cBiomeGen override:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
} ;





class cBioGenVoronoi :
	public cBiomeGenList
{
public:
	cBioGenVoronoi(int a_Seed, int a_CellSize, const AString & a_Biomes) :
		cBiomeGenList(a_Biomes),
		m_CellSize((a_CellSize > 4) ? a_CellSize : 4),
		m_Noise(a_Seed)
	{
	}
	
protected:

	int m_CellSize;
	
	cNoise m_Noise;

	// cBiomeGen override:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	
	EMCSBiome VoronoiBiome(int a_BlockX, int a_BlockZ);
} ;





class cBioGenDistortedVoronoi :
	public cBioGenVoronoi
{
public:
	cBioGenDistortedVoronoi(int a_Seed, int a_CellSize, const AString & a_Biomes) :
		cBioGenVoronoi(a_Seed, a_CellSize, a_Biomes)
	{
	}

protected:

	// cBiomeGen override:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	
	/// Distorts the coords using a Perlin-like noise
	void Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ);
} ;





