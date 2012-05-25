
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
	cBioGenCheckerboard(int a_BiomeSize) : m_BiomeSize((a_BiomeSize < 8) ? 8 : a_BiomeSize) {}
	
protected:

	int m_BiomeSize;
	
	// cBiomeGen override:
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
} ;




