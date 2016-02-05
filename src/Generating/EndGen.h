
// EndGen.h

// Declares the cEndGen class representing the generator for the End, both as a HeightGen and CompositionGen





#pragma once

#include "ComposableGenerator.h"
#include "../Noise/Noise.h"





class cEndGen :
	public cTerrainShapeGen,
	public cTerrainCompositionGen
{
public:
	cEndGen(int a_Seed);

protected:

	/** Seed for the noise */
	int m_Seed;

	/** The Perlin noise used for generating */
	cPerlinNoise m_Perlin;

	// XYZ size of the "island", in blocks:
	int m_IslandSizeX;
	int m_IslandSizeY;
	int m_IslandSizeZ;

	// XYZ Frequencies of the noise functions:
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;

	// Minimum and maximum chunk coords for chunks inside the island area. Chunks outside won't get calculated at all
	int m_MinChunkX, m_MaxChunkX;
	int m_MinChunkZ, m_MaxChunkZ;

	// Noise array for the last chunk (in the noise range)
	int m_LastChunkX;
	int m_LastChunkZ;
	NOISE_DATATYPE m_NoiseArray[17 * 17 * 257];  // x + 17 * z + 17 * 17 * y


	/** Unless the LastChunk coords are equal to coords given, prepares the internal state (noise array) */
	void PrepareState(int a_ChunkX, int a_ChunkZ);

	/** Generates the m_NoiseArray array for the current chunk */
	void GenerateNoiseArray(void);

	/** Returns true if the chunk is outside of the island's dimensions */
	bool IsChunkOutsideRange(int a_ChunkX, int a_ChunkZ);


	// cTerrainShapeGen overrides:
	virtual void GenShape(int a_ChunkX, int a_ChunkZ, cChunkDesc::Shape & a_Shape) override;


	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeCompoGen(cIniFile & a_IniFile) override;
} ;
