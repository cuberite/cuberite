
// BioGen.h

/*
Interfaces to the various biome generators:
	- cBioGenConstant
	- cBioGenCheckerboard
	- cBioGenDistortedVoronoi
*/





#pragma once

#include "cChunkGenerator.h"





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





class cBioGenCheckerboard :
	public cBiomeGen
{
public:
	cBioGenCheckerboard(int a_BiomeSize, const AString & a_Biomes) :
		m_BiomeSize((a_BiomeSize < 8) ? 8 : a_BiomeSize)
	{
		InitializeBiomes(a_Biomes);
	}
	
protected:

	int m_BiomeSize;
	
	// List of biomes that the generator is allowed to generate:
	typedef std::vector<EMCSBiome> EMCSBiomes;
	EMCSBiomes m_Biomes;
	int        m_BiomesCount;
	
	void InitializeBiomes(const AString & a_Biomes);
	
	// cBiomeGen override:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
} ;




