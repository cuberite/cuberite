
// DistortedHeightmap.h

// Declares the cDistortedHeightmap class representing the height and composition generator capable of overhangs





#pragma once

#include "ComposableGenerator.h"
#include "HeiGen.h"
#include "../Noise.h"





#define NOISE_SIZE_Y (257 + 32)





class cDistortedHeightmap :
	public cTerrainHeightGen,
	public cTerrainCompositionGen
{
public:
	cDistortedHeightmap(int a_Seed, cBiomeGen & a_BiomeGen);
	
	void Initialize(cIniFile & a_IniFile);
	
protected:
	typedef cChunkDef::BiomeMap BiomeNeighbors[3][3];

	cNoise m_Noise1;
	cNoise m_Noise2;
	cNoise m_Noise3;
	cNoise m_Noise4;
	cNoise m_Noise5;
	
	int m_SeaLevel;
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;

	int m_CurChunkX;
	int m_CurChunkZ;
	NOISE_DATATYPE m_DistortedHeightmap[17 * 17 * 257];

	cBiomeGen &   m_BiomeGen;
	cHeiGenBiomal m_UnderlyingHeiGen;  // This generator provides us with base heightmap (before distortion)
	cHeiGenCache  m_HeightGen;         // Cache above m_UnderlyingHeiGen
	
	/// Heightmap for the current chunk, before distortion (from m_HeightGen). Used for optimization.
	cChunkDef::HeightMap m_CurChunkHeights;
	
	// Per-biome terrain generator parameters:
	struct sGenParam
	{
		NOISE_DATATYPE m_DistortAmpX;
		NOISE_DATATYPE m_DistortAmpZ;
	} ;
	static const sGenParam m_GenParam[biNumBiomes];
	
	NOISE_DATATYPE m_DistortAmpX[17 * 17];
	NOISE_DATATYPE m_DistortAmpZ[17 * 17];
	

	/// Unless the LastChunk coords are equal to coords given, prepares the internal state (noise arrays, heightmap)
	void PrepareState(int a_ChunkX, int a_ChunkZ);
	
	/// Generates the m_DistortedHeightmap array for the current chunk
	void GenerateHeightArray(void);
	
	/// Calculates the heightmap value (before distortion) at the specified (floating-point) coords
	int GetHeightmapAt(NOISE_DATATYPE a_X, NOISE_DATATYPE a_Z);
	
	/// Updates m_DistortAmpX/Z[] based on m_CurChunkX and m_CurChunkZ
	void UpdateDistortAmps(void);
	
	/// Calculates the X and Z distortion amplitudes based on the neighbors' biomes
	void GetDistortAmpsAt(BiomeNeighbors & a_Neighbors, int a_RelX, int a_RelZ, NOISE_DATATYPE & a_DistortAmpX, NOISE_DATATYPE & a_DistortAmpZ);
	
	
	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override;

	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) override;
} ;
