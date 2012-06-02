
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




