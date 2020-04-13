
// Noise3DGenerator.h

// Declares cNoise3DGenerator and cNoise3DComposable classes, representing 3D-noise-based generators.
// They generate terrain shape by combining a lerp of two 3D noises with a vertical linear gradient
// cNoise3DGenerator is obsolete and unmaintained.
// cNoise3DComposable is used to test parameter combinations for single-biome worlds.





#pragma once

#include "ComposableGenerator.h"
#include "../Noise/Noise.h"
#include "../Noise/InterpolNoise.h"





class cNoise3DGenerator:
	public cChunkGenerator
{
	using Super = cChunkGenerator;

public:

	cNoise3DGenerator();
	virtual ~cNoise3DGenerator() override;

	virtual void Initialize(cIniFile & a_IniFile) override;
	virtual void GenerateBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void Generate(cChunkDesc & a_ChunkDesc) override;

protected:
	// Linear interpolation step sizes, must be divisors of cChunkDef::Width and cChunkDef::Height, respectively:
	static const int UPSCALE_X = 4;
	static const int UPSCALE_Y = 8;
	static const int UPSCALE_Z = 4;

	// Linear interpolation buffer dimensions, calculated from the step sizes:
	static const int DIM_X = 1 + cChunkDef::Width  / UPSCALE_X;
	static const int DIM_Y = 1 + cChunkDef::Height / UPSCALE_Y;
	static const int DIM_Z = 1 + cChunkDef::Width  / UPSCALE_Z;

	/** The base 3D noise source for the actual composition */
	cOctavedNoise<cInterp5DegNoise> m_Perlin;

	/** The noise used for heightmap directing. */
	cOctavedNoise<cInterp5DegNoise> m_Cubic;

	int            m_SeaLevel;
	NOISE_DATATYPE m_HeightAmplification;
	NOISE_DATATYPE m_MidPoint;  // Where the vertical "center" of the noise should be
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;
	NOISE_DATATYPE m_AirThreshold;

	/** Generates the 3D noise array used for terrain generation into a_Noise; a_Noise is of ChunkData-size */
	void GenerateNoiseArray(cChunkCoords a_ChunkCoords, NOISE_DATATYPE * a_Noise);

	/** Composes terrain - adds dirt, grass and sand */
	void ComposeTerrain(cChunkDesc & a_ChunkDesc);
} ;





class cNoise3DComposable :
	public cTerrainShapeGen
{
public:
	cNoise3DComposable(int a_Seed);

	void Initialize(cIniFile & a_IniFile);

protected:
	/** The 3D noise that is used to choose between density noise A and B. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_ChoiceNoise;

	/** Density 3D noise, variant A. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_DensityNoiseA;

	/** Density 3D noise, variant B. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_DensityNoiseB;

	/** Heightmap-like noise used to provide variance for low-amplitude biomes. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_BaseNoise;

	/** The main parameter of the generator, specifies the slope of the vertical linear gradient.
	A higher value means a steeper slope and a smaller total amplitude of the generated terrain. */
	NOISE_DATATYPE m_HeightAmplification;

	/** Where the vertical "center" of the noise should be, as block height. */
	NOISE_DATATYPE m_MidPoint;

	// Frequency of the 3D noise's first octave:
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;

	// Frequency of the base terrain noise:
	NOISE_DATATYPE m_BaseFrequencyX;
	NOISE_DATATYPE m_BaseFrequencyZ;

	// Frequency of the choice noise:
	NOISE_DATATYPE m_ChoiceFrequencyX;
	NOISE_DATATYPE m_ChoiceFrequencyY;
	NOISE_DATATYPE m_ChoiceFrequencyZ;

	// Threshold for when the values are considered air:
	NOISE_DATATYPE m_AirThreshold;

	// Cache for the last calculated chunk (reused between heightmap and composition queries):
	cChunkCoords m_LastChunkCoords;
	NOISE_DATATYPE m_NoiseArray[17 * 17 * 257];  // x + 17 * z + 17 * 17 * y


	/** Generates the 3D noise array used for terrain generation (m_NoiseArray), unless the LastChunk coords are equal to coords given */
	void GenerateNoiseArrayIfNeeded(cChunkCoords a_ChunkCoords);

	// cTerrainHeightGen overrides:
	virtual void GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeShapeGen(cIniFile & a_IniFile) override { Initialize(a_IniFile); }
} ;





class cBiomalNoise3DComposable :
	public cTerrainShapeGen
{
public:
	cBiomalNoise3DComposable(int a_Seed, cBiomeGenPtr a_BiomeGen);

	void Initialize(cIniFile & a_IniFile);

protected:
	/** Number of columns around the pixel to query for biomes for averaging. Must be less than or equal to 16. */
	static const int AVERAGING_SIZE = 9;

	/** Type used for a single parameter across the entire (downscaled) chunk. */
	typedef NOISE_DATATYPE ChunkParam[5 * 5];


	/** The noise that is used to choose between density noise A and B. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_ChoiceNoise;

	/** Density 3D noise, variant A. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_DensityNoiseA;

	/** Density 3D noise, variant B. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_DensityNoiseB;

	/** Heightmap-like noise used to provide variance for low-amplitude biomes. */
	cOctavedNoise<cInterpolNoise<Interp5Deg>> m_BaseNoise;

	/** The underlying biome generator. */
	cBiomeGenPtr m_BiomeGen;

	/** Block height of the sealevel, used for composing the terrain. */
	int m_SeaLevel;

	// Frequency of the 3D noise's first octave:
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;

	// Frequency of the base terrain noise:
	NOISE_DATATYPE m_BaseFrequencyX;
	NOISE_DATATYPE m_BaseFrequencyZ;

	// Frequency of the choice noise:
	NOISE_DATATYPE m_ChoiceFrequencyX;
	NOISE_DATATYPE m_ChoiceFrequencyY;
	NOISE_DATATYPE m_ChoiceFrequencyZ;

	// Threshold for when the values are considered air:
	NOISE_DATATYPE m_AirThreshold;

	// Cache for the last calculated chunk (reused between heightmap and composition queries):
	cChunkCoords m_LastChunkCoords;
	NOISE_DATATYPE m_NoiseArray[17 * 17 * 257];  // 257 * x + y + 257 * 17 * z

	/** Weights for summing up neighboring biomes. */
	NOISE_DATATYPE m_Weight[AVERAGING_SIZE * 2 + 1][AVERAGING_SIZE * 2 + 1];

	/** The sum of m_Weight[]. */
	NOISE_DATATYPE m_WeightSum;


	/** Generates the 3D noise array used for terrain generation (m_NoiseArray), unless the LastChunk coords are equal to coords given */
	void GenerateNoiseArrayIfNeeded(cChunkCoords a_ChunkCoords);

	/** Calculates the biome-related parameters for the chunk. */
	void CalcBiomeParamArrays(cChunkCoords a_ChunkCoords, ChunkParam & a_HeightAmp, ChunkParam & a_MidPoint);

	/** Returns the parameters for the specified biome. */
	void GetBiomeParams(EMCSBiome a_Biome, NOISE_DATATYPE & a_HeightAmp, NOISE_DATATYPE & a_MidPoint);

	// cTerrainShapeGen overrides:
	virtual void GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape) override;
	virtual void InitializeShapeGen(cIniFile & a_IniFile) override { Initialize(a_IniFile); }
} ;




