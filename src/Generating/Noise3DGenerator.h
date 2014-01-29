
// Noise3DGenerator.h

// Generates terrain using 3D noise, rather than composing. Is a test.




#pragma once

#include "ComposableGenerator.h"
#include "../Noise.h"





class cNoise3DGenerator :
	public cChunkGenerator::cGenerator
{
	typedef cChunkGenerator::cGenerator super;
	
public:
	cNoise3DGenerator(cChunkGenerator & a_ChunkGenerator);
	virtual ~cNoise3DGenerator();
	
	virtual void Initialize(cIniFile & a_IniFile) override;
	virtual void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc) override;
	
protected:
	// Linear interpolation step sizes, must be divisors of cChunkDef::Width and cChunkDef::Height, respectively:
	static const int UPSCALE_X = 8;
	static const int UPSCALE_Y = 4;
	static const int UPSCALE_Z = 8;
	
	// Linear interpolation buffer dimensions, calculated from the step sizes:
	static const int DIM_X = 1 + cChunkDef::Width  / UPSCALE_X;
	static const int DIM_Y = 1 + cChunkDef::Height / UPSCALE_Y;
	static const int DIM_Z = 1 + cChunkDef::Width  / UPSCALE_Z;

	cPerlinNoise m_Perlin;   // The base 3D noise source for the actual composition
	cCubicNoise  m_Cubic;    // The noise used for heightmap directing
	
	int            m_SeaLevel;
	NOISE_DATATYPE m_HeightAmplification;
	NOISE_DATATYPE m_MidPoint;  // Where the vertical "center" of the noise should be
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;
	NOISE_DATATYPE m_AirThreshold;
	
	/// Generates the 3D noise array used for terrain generation; a_Noise is of ChunkData-size
	void GenerateNoiseArray(int a_ChunkX, int a_ChunkZ, NOISE_DATATYPE * a_Noise);
	
	/// Updates heightmap based on the chunk's contents
	void UpdateHeightmap(cChunkDesc & a_ChunkDesc);
	
	/// Composes terrain - adds dirt, grass and sand
	void ComposeTerrain(cChunkDesc & a_ChunkDesc);
} ;





class cNoise3DComposable :
	public cTerrainHeightGen,
	public cTerrainCompositionGen
{
public:
	cNoise3DComposable(int a_Seed);

	void Initialize(cIniFile & a_IniFile);

protected:
	cNoise m_Noise1;
	cNoise m_Noise2;
	cNoise m_Noise3;
	
	int            m_SeaLevel;
	NOISE_DATATYPE m_HeightAmplification;
	NOISE_DATATYPE m_MidPoint;  // Where the vertical "center" of the noise should be
	NOISE_DATATYPE m_FrequencyX;
	NOISE_DATATYPE m_FrequencyY;
	NOISE_DATATYPE m_FrequencyZ;
	NOISE_DATATYPE m_AirThreshold;
	
	int m_LastChunkX;
	int m_LastChunkZ;
	NOISE_DATATYPE m_NoiseArray[17 * 17 * 257];  // x + 17 * z + 17 * 17 * y
	
	
	/// Generates the 3D noise array used for terrain generation, unless the LastChunk coords are equal to coords given
	void GenerateNoiseArrayIfNeeded(int a_ChunkX, int a_ChunkZ);
	
	// cTerrainHeightGen overrides:
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) override;

	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) override;
} ;




