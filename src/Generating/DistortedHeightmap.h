
// DistortedHeightmap.h

// Declares the cDistortedHeightmap class representing the height and composition generator capable of overhangs





#pragma once

#include "ComposableGenerator.h"
#include "HeiGen.h"





#define NOISE_SIZE_Y (257 + 32)





class cDistortedHeightmap :
	public cTerrainShapeGen
{
public:
	cDistortedHeightmap(int a_Seed, const cBiomeGenPtr & a_BiomeGen);

protected:
	typedef cChunkDef::BiomeMap BiomeNeighbors[3][3];

	// Linear upscaling step sizes, must be divisors of cChunkDef::Width and cChunkDef::Height, respectively:
	static const int INTERPOL_X = 8;
	static const int INTERPOL_Y = 4;
	static const int INTERPOL_Z = 8;

	// Linear upscaling buffer dimensions, calculated from the step sizes:
	static const int DIM_X = 1 +  (17 / INTERPOL_X);
	static const int DIM_Y = 1 + (257 / INTERPOL_Y);
	static const int DIM_Z = 1 +  (17 / INTERPOL_Z);

	cPerlinNoise m_NoiseDistortX;
	cPerlinNoise m_NoiseDistortZ;

	int m_SeaLevel;
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;

	cChunkCoords m_CurChunkCoords;
	NOISE_DATATYPE m_DistortedHeightmap[17 * 257 * 17];

	/** The bime generator to query for biomes. */
	cBiomeGenPtr m_BiomeGen;

	/** The generator that provides the base heightmap (before distortion). */
	cTerrainHeightGenPtr m_UnderlyingHeiGen;

	/** Cache for m_UnderlyingHeiGen. */
	cHeiGenCache m_HeightGen;

	/** Heightmap for the current chunk, before distortion (from m_HeightGen). Used for optimization. */
	cChunkDef::HeightMap m_CurChunkHeights;

	// Per-biome terrain generator parameters:
	struct sGenParam
	{
		NOISE_DATATYPE m_DistortAmpX;
		NOISE_DATATYPE m_DistortAmpZ;
	} ;
	static const sGenParam m_GenParam[256];

	// Distortion amplitudes for each direction, before linear upscaling
	NOISE_DATATYPE m_DistortAmpX[DIM_X * DIM_Z];
	NOISE_DATATYPE m_DistortAmpZ[DIM_X * DIM_Z];

	/** True if Initialize() has been called. Used to initialize-once even with multiple init entrypoints (HeiGen / CompoGen). */
	bool m_IsInitialized;


	/** Unless the LastChunk coords are equal to coords given, prepares the internal state (noise arrays, heightmap). */
	void PrepareState(cChunkCoords a_ChunkCoords);

	/** Generates the m_DistortedHeightmap array for the current chunk. */
	void GenerateHeightArray(void);

	/** Calculates the heightmap value (before distortion) at the specified (floating-point) coords. */
	int GetHeightmapAt(NOISE_DATATYPE a_X, NOISE_DATATYPE a_Z);

	/** Updates m_DistortAmpX/Z[] based on m_CurChunkX and m_CurChunkZ. */
	void UpdateDistortAmps(void);

	/** Calculates the X and Z distortion amplitudes based on the neighbors' biomes. */
	void GetDistortAmpsAt(BiomeNeighbors & a_Neighbors, int a_RelX, int a_RelZ, NOISE_DATATYPE & a_DistortAmpX, NOISE_DATATYPE & a_DistortAmpZ);

	/** Reads the settings from the ini file. Skips reading if already initialized. */
	void Initialize(cIniFile & a_IniFile);


	// cTerrainShapeGen overrides:
	virtual void GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeShapeGen(cIniFile & a_IniFile) override;
} ;
