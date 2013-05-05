
// BioGen.h

/*
Interfaces to the various biome generators:
	- cBioGenConstant
	- cBioGenCheckerboard
	- cBioGenDistortedVoronoi
*/





#pragma once

#include "ComposableGenerator.h"
#include "../Noise.h"





class cBioGenConstant :
	public cBiomeGen
{
public:
	cBioGenConstant(void) : m_Biome(biPlains) {}
	
protected:

	EMCSBiome m_Biome;

	// cBiomeGen overrides:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void Initialize(cIniFile & a_IniFile) override;
} ;





/// A simple cache that stores N most recently generated chunks' biomes; N being settable upon creation
class cBioGenCache :
	public cBiomeGen
{
	typedef cBiomeGen super;
	
public:
	cBioGenCache(cBiomeGen * a_BioGenToCache, int a_CacheSize);  // Doesn't take ownership of a_BioGenToCache
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
	virtual void Initialize(cIniFile & a_IniFile) override;
} ;





/// Base class for generators that use a list of available biomes. This class takes care of the list.
class cBiomeGenList :
	public cBiomeGen
{
	typedef cBiomeGen super;
	
protected:
	// List of biomes that the generator is allowed to generate:
	typedef std::vector<EMCSBiome> EMCSBiomes;
	EMCSBiomes m_Biomes;
	int        m_BiomesCount;  // Pulled out of m_Biomes for faster access
	
	/// Parses the INI file setting string into m_Biomes.
	void InitializeBiomes(const AString & a_Biomes);
} ;





class cBioGenCheckerboard :
	public cBiomeGenList
{
	typedef cBiomeGenList super;
	
protected:
	int m_BiomeSize;
	
	// cBiomeGen overrides:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void Initialize(cIniFile & a_IniFile) override;
} ;





class cBioGenVoronoi :
	public cBiomeGenList
{
	typedef cBiomeGenList super;
	
public:
	cBioGenVoronoi(int a_Seed) :
		m_Noise(a_Seed)
	{
	}
	
protected:
	int m_CellSize;
	
	cNoise m_Noise;

	// cBiomeGen overrides:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void Initialize(cIniFile & a_IniFile) override;
	
	EMCSBiome VoronoiBiome(int a_BlockX, int a_BlockZ);
} ;





class cBioGenDistortedVoronoi :
	public cBioGenVoronoi
{
	typedef cBioGenVoronoi super;
public:
	cBioGenDistortedVoronoi(int a_Seed) :
		cBioGenVoronoi(a_Seed)
	{
	}

protected:
	// cBiomeGen overrides:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void Initialize(cIniFile & a_IniFile) override;
	
	/// Distorts the coords using a Perlin-like noise
	void Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ);
} ;





class cBioGenMultiStepMap :
	public cBiomeGen
{
	typedef cBiomeGen super;
	
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
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void Initialize(cIniFile & a_IniFile) override;
	
	/** Step 1: Decides between ocean, land and mushroom, using a DistVoronoi with special conditions and post-processing for mushroom islands
	Sets biomes to biOcean, -1 (i.e. land), biMushroomIsland or biMushroomShore
	*/
	void DecideOceanLandMushroom(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap);
	
	/** Step 2: Add rivers to the land
	Flips some "-1" biomes into biRiver
	*/
	void AddRivers(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap);
	
	/** Step 3: Decide land biomes using a temperature / humidity map; freeze ocean / river in low temperatures.
	Flips all remaining "-1" biomes into land biomes. Also flips some biOcean and biRiver into biFrozenOcean, biFrozenRiver, based on temp map.
	*/
	void ApplyTemperatureHumidity(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap);
	
	/// Distorts the coords using a Perlin-like noise, with a specified cell-size
	void Distort(int a_BlockX, int a_BlockZ, int & a_DistortedX, int & a_DistortedZ, int a_CellSize);
	
	/// Builds two Perlin-noise maps, one for temperature, the other for humidity. Trims both into [0..255] range
	void BuildTemperatureHumidityMaps(int a_ChunkX, int a_ChunkZ, IntMap & a_TemperatureMap, IntMap & a_HumidityMap);
	
	/// Flips all remaining "-1" biomes into land biomes using the two maps
	void DecideLandBiomes(cChunkDef::BiomeMap & a_BiomeMap, const IntMap & a_TemperatureMap, const IntMap & a_HumidityMap);
	
	/// Flips biOcean and biRiver into biFrozenOcean and biFrozenRiver if the temperature is too low
	void FreezeWaterBiomes(cChunkDef::BiomeMap & a_BiomeMap, const IntMap & a_TemperatureMap);
} ;




