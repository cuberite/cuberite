
// HeiGen.h

/*
Interfaces to the various height-based terrain shape generators:
	- cHeiGenFlat
	- cHeiGenClassic
	- cHeiGenBiomal

Also implements the heightmap cache
*/





#pragma once

#include "ComposableGenerator.h"
#include "../Noise/Noise.h"





/** A simple cache that stores N most recently generated chunks' heightmaps; N being settable upon creation */
class cHeiGenCache :
	public cTerrainHeightGen
{
public:
	cHeiGenCache(cTerrainHeightGenPtr a_HeiGenToCache, size_t a_CacheSize);
	virtual ~cHeiGenCache() override = default;

	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(cChunkCoords a_ChunkCoords, cChunkDef::HeightMap & a_HeightMap) override;
	virtual HEIGHTTYPE GetHeightAt(int a_BlockX, int a_BlockZ) override;

	/** Retrieves height at the specified point in the cache, returns true if found, false if not found */
	bool GetHeightAt(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelZ, HEIGHTTYPE & a_Height);

protected:
	struct sCacheData
	{
		cChunkCoords m_Coords;
		cChunkDef::HeightMap m_HeightMap;

		/** Default constructor: Fill in bogus coords, so that the item is not used until properly calculated. */
		sCacheData():
			m_Coords(0x7fffffff, 0x7fffffff)
		{}
	} ;

	/** The terrain height generator that is being cached. */
	cTerrainHeightGenPtr m_HeiGenToCache;

	// To avoid moving large amounts of data for the MRU behavior, we MRU-ize indices to an array of the actual data
	size_t                  m_CacheSize;
	std::vector<size_t>     m_CacheOrder;  // MRU-ized order, indices into m_CacheData array
	std::vector<sCacheData> m_CacheData;   // m_CacheData[m_CacheOrder[0]] is the most recently used

	// Cache statistics
	size_t m_NumHits;
	size_t m_NumMisses;
	size_t m_TotalChain;  // Number of cache items walked to get to a hit (only added for hits)
} ;





/** Caches heightmaps in multiple underlying caches to improve the distribution and lower the chain length. */
class cHeiGenMultiCache:
	public cTerrainHeightGen
{
public:
	cHeiGenMultiCache(const cTerrainHeightGenPtr & a_HeightGenToCache, size_t a_SubCacheSize, size_t a_NumSubCaches);

	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(cChunkCoords a_ChunkCoords, cChunkDef::HeightMap & a_HeightMap) override;
	virtual HEIGHTTYPE GetHeightAt(int a_BlockX, int a_BlockZ) override;

	/** Retrieves height at the specified point in the cache, returns true if found, false if not found */
	bool GetHeightAt(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelZ, HEIGHTTYPE & a_Height);

protected:
	typedef std::shared_ptr<cHeiGenCache> cHeiGenCachePtr;
	typedef std::vector<cHeiGenCachePtr> cHeiGenCachePtrs;


	/** The coefficient used to turn Z coords into index (x + Coeff * z). */
	static const size_t m_CoeffZ = 5;

	/** Number of sub-caches, pulled out of m_SubCaches.size() for performance reasons. */
	size_t m_NumSubCaches;

	/** The individual sub-caches. */
	cHeiGenCachePtrs m_SubCaches;
};





class cHeiGenFlat :
	public cTerrainHeightGen
{
public:
	cHeiGenFlat(void) : m_Height(5) {}

protected:

	HEIGHTTYPE m_Height;

	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(cChunkCoords a_ChunkCoords, cChunkDef::HeightMap & a_HeightMap) override;
	virtual void InitializeHeightGen(cIniFile & a_IniFile) override;
} ;





class cHeiGenClassic :
	public cTerrainHeightGen
{
public:
	cHeiGenClassic(int a_Seed);

protected:

	int m_Seed;
	cNoise m_Noise;
	float m_HeightFreq1, m_HeightAmp1;
	float m_HeightFreq2, m_HeightAmp2;
	float m_HeightFreq3, m_HeightAmp3;

	float GetNoise(float x, float y);

	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(cChunkCoords a_ChunkCoords, cChunkDef::HeightMap & a_HeightMap) override;
	virtual void InitializeHeightGen(cIniFile & a_IniFile) override;
} ;





class cHeiGenMountains :
	public cTerrainHeightGen
{
public:
	cHeiGenMountains(int a_Seed);

protected:

	int m_Seed;
	cRidgedMultiNoise m_MountainNoise;
	cRidgedMultiNoise m_DitchNoise;
	cPerlinNoise m_Perlin;

	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(cChunkCoords a_ChunkCoords, cChunkDef::HeightMap & a_HeightMap) override;
	virtual void InitializeHeightGen(cIniFile & a_IniFile) override;
} ;





class cHeiGenBiomal:
	public cTerrainHeightGen
{
	using Super = cTerrainHeightGen;

public:

	cHeiGenBiomal(int a_Seed, cBiomeGenPtr a_BiomeGen):
		m_Noise(a_Seed),
		m_BiomeGen(std::move(a_BiomeGen))
	{
	}

	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(cChunkCoords a_ChunkCoords, cChunkDef::HeightMap & a_HeightMap) override;
	virtual HEIGHTTYPE GetHeightAt(int a_BlockX, int a_BlockZ) override  // Need to provide this override due to clang's overzealous detection of overloaded virtuals
	{
		return Super::GetHeightAt(a_BlockX, a_BlockZ);
	}
	virtual void InitializeHeightGen(cIniFile & a_IniFile) override;

protected:

	typedef cChunkDef::BiomeMap BiomeNeighbors[3][3];

	cNoise       m_Noise;
	cBiomeGenPtr m_BiomeGen;

	// Per-biome terrain generator parameters:
	struct sGenParam
	{
		float m_HeightFreq1, m_HeightAmp1;
		float m_HeightFreq2, m_HeightAmp2;
		float m_HeightFreq3, m_HeightAmp3;
		float m_BaseHeight;
	} ;
	static const sGenParam m_GenParam[256];


	NOISE_DATATYPE GetHeightAt(int a_RelX, int a_RelZ, int a_ChunkX, int a_ChunkZ, const BiomeNeighbors & a_BiomeNeighbors);
} ;




