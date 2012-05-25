
// HeiGen.cpp

// Implements the various terrain height generators

#include "Globals.h"
#include "HeiGen.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHeiGenFlat:

void cHeiGenFlat::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	for (int i = 0; i < ARRAYCOUNT(a_HeightMap); i++)
	{
		a_HeightMap[i] = m_Height;
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cHeiGenClassic:

cHeiGenClassic::cHeiGenClassic(int a_Seed, float a_HeightFreq1, float a_HeightAmp1, float a_HeightFreq2, float a_HeightAmp2, float a_HeightFreq3, float a_HeightAmp3) :
	m_Seed(a_Seed),
	m_Noise(a_Seed),
	m_HeightFreq1(a_HeightFreq1),
	m_HeightAmp1 (a_HeightAmp1),
	m_HeightFreq2(a_HeightFreq2),
	m_HeightAmp2 (a_HeightAmp2),
	m_HeightFreq3(a_HeightFreq3),
	m_HeightAmp3 (a_HeightAmp3)
{
	// Nothing needed yet
}





float cHeiGenClassic::GetNoise(float x, float y)
{
	float oct1 = m_Noise.CubicNoise2D(x * m_HeightFreq1, y * m_HeightFreq1) * m_HeightAmp1;
	float oct2 = m_Noise.CubicNoise2D(x * m_HeightFreq2, y * m_HeightFreq2) * m_HeightAmp2;
	float oct3 = m_Noise.CubicNoise2D(x * m_HeightFreq3, y * m_HeightFreq3) * m_HeightAmp3;

	float height = m_Noise.CubicNoise2D(x * 0.1f, y * 0.1f ) * 2;

	float flatness = ((m_Noise.CubicNoise2D(x * 0.5f, y * 0.5f) + 1.f) * 0.5f) * 1.1f; // 0 ... 1.5
	flatness *= flatness * flatness;

	return (oct1 + oct2 + oct3) * flatness + height;
}





void cHeiGenClassic::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	for (int z = 0; z < cChunkDef::Width; z++) 
	{
		const float zz = (float)(a_ChunkZ * cChunkDef::Width + z);
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			const float xx = (float)(a_ChunkX * cChunkDef::Width + x);
			
			int hei = 64 + (int)(GetNoise(xx * 0.05f, zz * 0.05f) * 16);
			if (hei < 10)
			{
				hei = 10;
			}
			if (hei > 250)
			{
				hei = 250;
			}
			cChunkDef::SetHeight(a_HeightMap, x , z, hei);
		}  // for x
	}  // for z
}




