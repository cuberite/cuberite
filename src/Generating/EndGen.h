
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
	cPerlinNoise m_VoidOffsetNoise;

	NOISE_DATATYPE m_AirThresholdMainIsland;
	NOISE_DATATYPE m_AirThresholdOtherIslands;
	int m_MainIslandSize;
	int m_BaseHeight;
	int m_TerrainTopMultiplier;
	int m_TerrainBottomMultiplier;
	int m_VoidOffsetNoiseMultiplier;

	// XYZ Frequencies of the noise functions:
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;

	// cTerrainShapeGen overrides:
	virtual void GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape) override;


	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override;

	// cTerrainShapeGen overrides:
	virtual void InitializeShapeGen(cIniFile & a_IniFile) override;
} ;
