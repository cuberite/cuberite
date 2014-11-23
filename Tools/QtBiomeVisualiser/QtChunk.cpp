#include "Globals.h"
#include "QtChunk.h"





Chunk::Chunk() :
	m_IsValid(false)
{
}





void Chunk::setBiomes(const cChunkDef::BiomeMap & a_Biomes)
{
	for (size_t idx = 0; idx < ARRAYCOUNT(a_Biomes); ++idx)
	{
		m_Biomes[idx] = static_cast<short>(a_Biomes[idx]);
	}
	m_IsValid = true;
}





EMCSBiome Chunk::getBiome(int a_RelX, int a_RelZ)
{
	if (!m_IsValid)
	{
		return biInvalidBiome;
	}
	return static_cast<EMCSBiome>(m_Biomes[a_RelX + 16 * a_RelZ]);
}




