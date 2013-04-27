
// Noise3DGenerator.h

// Generates terrain using 3D noise, rather than composing. Is a test.




#pragma once

#include "ChunkGenerator.h"
#include "ChunkDesc.h"
#include "../Noise.h"





class cNoise3DGenerator :
	public cChunkGenerator::cGenerator
{
	typedef cChunkGenerator::cGenerator super;
	
public:
	cNoise3DGenerator(cChunkGenerator & a_ChunkGenerator);
	virtual ~cNoise3DGenerator();
	
	virtual void Initialize(cWorld * a_World, cIniFile & a_IniFile) override;
	virtual void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc) override;
	
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
	
	/// Generates the 3D noise array used for terrain generation
	void GenerateNoiseArray(int a_ChunkX, int a_ChunkZ, NOISE_DATATYPE * a_Noise);
	
	/// Updates heightmap based on the chunk's contents
	void UpdateHeightmap(cChunkDesc & a_ChunkDesc);
	
	/// Composes terrain - adds dirt, grass and sand
	void ComposeTerrain(cChunkDesc & a_ChunkDesc);
} ;




