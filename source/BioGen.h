
// BioGen.h

/*
Interfaces to the various biome generators:
	- cBioGenConstant
	- cBioGenCheckerboard
	- cBioGenDistortedVoronoi
*/





#pragma once

#include "cChunkGenerator.h"
#include "cNoise.h"





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





class cBioGenDistortedVoronoi :
	public cBiomeGen
{
public:
	cBioGenDistortedVoronoi(int a_Seed) : m_Seed(a_Seed) {}

protected:

	int m_Seed;

	// cBiomeGen override:
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




