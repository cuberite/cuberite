
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
	int m_MainIslandSize;
	int m_IslandThickness;
	int m_IslandYOffset;

	// XYZ Frequencies of the noise functions:
	NOISE_DATATYPE m_MainIslandFrequencyX;
	NOISE_DATATYPE m_MainIslandFrequencyY;
	NOISE_DATATYPE m_MainIslandFrequencyZ;
	NOISE_DATATYPE m_MainIslandMinThreshold;

	// XYZ Frequencies of the noise functions on the smaller islands:
	NOISE_DATATYPE m_SmallIslandFrequencyX;
	NOISE_DATATYPE m_SmallIslandFrequencyY;
	NOISE_DATATYPE m_SmallIslandFrequencyZ;
	NOISE_DATATYPE m_SmallIslandMinThreshold;


	// Noise array for the last chunk (in the noise range)
	cChunkCoords m_LastChunkCoords;
	NOISE_DATATYPE m_NoiseArray[17 * 17 * 257];  // x + 17 * z + 17 * 17 * y


	/** Unless the LastChunk coords are equal to coords given, prepares the internal state (noise array) */
	void PrepareState(cChunkCoords a_ChunkCoords);

	/** Generates the m_NoiseArray array for the current chunk */
	void GenerateNoiseArray(void);

	// cTerrainShapeGen overrides:
	virtual void GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeShapeGen(cIniFile & a_IniFile) override;


	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override;
} ;
