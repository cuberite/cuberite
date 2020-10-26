
// BioGen.h

/*
Interfaces to the various biome generators:
	- cBioGenConstant
	- cBioGenCheckerboard
	- cBioGenDistortedVoronoi
*/





#pragma once

#include "ComposableGenerator.h"
#include "../Noise/Noise.h"
#include "../VoronoiMap.h"





class cBioGenConstant :
	public cBiomeGen
{
public:
	cBioGenConstant(void) : m_Biome(biPlains) {}

protected:

	EMCSBiome m_Biome;

	// cBiomeGen overrides:
	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) override;
} ;





/** A simple cache that stores N most recently generated chunks' biomes; N being settable upon creation */
class cBioGenCache:
	public cBiomeGen
{
	using Super = cBiomeGen;

public:

	cBioGenCache(cBiomeGenPtr a_BioGenToCache, size_t a_CacheSize);
	virtual ~cBioGenCache() override = default;


protected:

	cBiomeGenPtr m_BioGenToCache;

	struct sCacheData
	{
		cChunkCoords m_Coords;
		cChunkDef::BiomeMap m_BiomeMap;

		/** Default constructor: Fill in bogus coords so that the item is not used in the cache until properly calculated. */
		sCacheData():
			m_Coords(0x7fffffff, 0x7fffffff)
		{
		}
	} ;

	// To avoid moving large amounts of data for the MRU behavior, we MRU-ize indices to an array of the actual data
	size_t                  m_CacheSize;
	std::vector<size_t>     m_CacheOrder;  // MRU-ized order, indices into m_CacheData array
	std::vector<sCacheData> m_CacheData;   // m_CacheData[m_CacheOrder[0]] is the most recently used

	// Cache statistics
	size_t m_NumHits;
	size_t m_NumMisses;
	size_t m_TotalChain;  // Number of cache items walked to get to a hit (only added for hits)

	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) override;
} ;





class cBioGenMulticache:
	public cBiomeGen
{
	using Super = cBiomeGen;

public:
	/* Creates a new multicache - a cache that divides the caching into several sub-caches based on the chunk coords.
	This allows us to use shorter cache depths with faster lookups for more covered area. (#381)
	a_SubCacheSize defines the size of each sub-cache
	a_NumSubCaches defines how many sub-caches are used for the multicache. */
	cBioGenMulticache(const cBiomeGenPtr & a_BioGenToCache, size_t a_SubCacheSize, size_t a_NumSubCaches);

protected:
	typedef std::vector<cBiomeGenPtr> cBiomeGenPtrs;


	/** Number of sub-caches. Pulled out of m_Caches.size() for faster access. */
	size_t m_NumSubCaches;

	/** Individual sub-caches. */
	cBiomeGenPtrs m_Caches;


	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) override;
};





/** Base class for generators that use a list of available biomes. This class takes care of the list. */
class cBiomeGenList:
	public cBiomeGen
{
	using Super = cBiomeGen;

protected:

	// List of biomes that the generator is allowed to generate:
	typedef std::vector<EMCSBiome> EMCSBiomes;
	EMCSBiomes m_Biomes;
	int        m_BiomesCount;  // Pulled out of m_Biomes for faster access

	/** Parses the INI file setting string into m_Biomes. */
	void InitializeBiomes(const AString & a_Biomes);
} ;





class cBioGenCheckerboard:
	public cBiomeGenList
{
	using Super = cBiomeGenList;

protected:

	int m_BiomeSize;

	// cBiomeGen overrides:
	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) override;
} ;





class cBioGenVoronoi:
	public cBiomeGenList
{
	using Super = cBiomeGenList;

public:

	cBioGenVoronoi(int a_Seed) :
		m_Voronoi(a_Seed)
	{
	}

protected:

	cVoronoiMap m_Voronoi;

	// cBiomeGen overrides:
	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) override;

	EMCSBiome VoronoiBiome(int a_BlockX, int a_BlockZ);
} ;





class cBioGenDistortedVoronoi:
	public cBiomeGenList
{
	using Super = cBiomeGenList;

public:

	cBioGenDistortedVoronoi(int a_Seed):
		m_Noise(a_Seed),
		m_Voronoi(a_Seed),
		m_CellSize(0)
	{
	}

protected:
	/** Noise used for the distortion */
	cNoise m_Noise;

	/** The underlying Voronoi map of the biomes */
	cVoronoiMap m_Voronoi;

	/** Size of the Voronoi cells, also used for distortion amplitude */
	int m_CellSize;

	// cBiomeGen overrides:
	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) override;

	/** Distorts the coords using a Perlin-like noise */
	void Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ);
} ;





class cBioGenMultiStepMap:
	public cBiomeGen
{
	using Super = cBiomeGen;

public:

	cBioGenMultiStepMap(int a_Seed);

protected:
	// Noises used for composing the perlin-noise:
	cNoise m_Noise1;
	cNoise m_Noise2;
	cNoise m_Noise3;
	cNoise m_Noise4;
	cNoise m_Noise5;
	cNoise m_Noise6;

	int    m_Seed;
	int    m_OceanCellSize;
	int    m_MushroomIslandSize;
	int    m_RiverCellSize;
	double m_RiverWidthThreshold;
	float  m_LandBiomesSize;

	typedef int    IntMap[17 * 17];  // x + 17 * z, expected trimmed into [0..255] range
	typedef double DblMap[17 * 17];  // x + 17 * z, expected trimmed into [0..1] range

	// cBiomeGen overrides:
	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) override;

	/** Step 1: Decides between ocean, land and mushroom, using a DistVoronoi with special conditions and post-processing for mushroom islands
	Sets biomes to biOcean, -1 (i.e. land), biMushroomIsland or biMushroomShore. */
	void DecideOceanLandMushroom(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap);

	/** Step 2: Add rivers to the land
	Flips some "-1" biomes into biRiver. */
	void AddRivers(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap);

	/** Step 3: Decide land biomes using a temperature / humidity map; freeze ocean / river in low temperatures.
	Flips all remaining "-1" biomes into land biomes. Also flips some biOcean and biRiver into biFrozenOcean, biFrozenRiver, based on temp map. */
	void ApplyTemperatureHumidity(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap);

	/** Distorts the coords using a Perlin-like noise, with a specified cell-size */
	void Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ, int a_CellSize);

	/** Builds two Perlin-noise maps, one for temperature, the other for humidity. Trims both into [0..255] range */
	void BuildTemperatureHumidityMaps(cChunkCoords a_ChunkCoords, IntMap & a_TemperatureMap, IntMap & a_HumidityMap);

	/** Flips all remaining "-1" biomes into land biomes using the two maps */
	void DecideLandBiomes(cChunkDef::BiomeMap & a_BiomeMap, const IntMap & a_TemperatureMap, const IntMap & a_HumidityMap);

	/** Flips biOcean and biRiver into biFrozenOcean and biFrozenRiver if the temperature is too low */
	void FreezeWaterBiomes(cChunkDef::BiomeMap & a_BiomeMap, const IntMap & a_TemperatureMap);
} ;





class cBioGenTwoLevel:
	public cBiomeGen
{
	using Super = cBiomeGen;

public:

	cBioGenTwoLevel(int a_Seed);

protected:
	/** The Voronoi map that decides the groups of biomes */
	cVoronoiMap m_VoronoiLarge;

	/** The Voronoi map that decides biomes inside individual biome groups */
	cVoronoiMap m_VoronoiSmall;

	// The noises used for the distortion:
	cNoise m_Noise1;
	cNoise m_Noise2;
	cNoise m_Noise3;
	cNoise m_Noise4;
	cNoise m_Noise5;
	cNoise m_Noise6;

	// Frequencies and amplitudes for the distortion noises:
	float m_FreqX1, m_AmpX1;
	float m_FreqX2, m_AmpX2;
	float m_FreqX3, m_AmpX3;
	float m_FreqZ1, m_AmpZ1;
	float m_FreqZ2, m_AmpZ2;
	float m_FreqZ3, m_AmpZ3;


	// cBiomeGen overrides:
	virtual void GenBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) override;

	/** Selects biome from the specified biome group, based on the specified index.
	Note that both params may overflow
	a_DistLevel is either 0 or 1; zero when it is at the edge of the small Voronoi cell, 1 near the center */
	EMCSBiome SelectBiome(int a_BiomeGroup, size_t a_BiomeIdx, int a_DistLevel);
} ;




