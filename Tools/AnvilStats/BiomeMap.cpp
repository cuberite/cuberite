
// BiomeMap.cpp

// Implements the cBiomeMap class representing a cCallback descendant that draws a map of biomes for the world

#include "Globals.h"
#include "BiomeMap.h"
#include "../BiomeVisualiser/BiomeColors.h"





static const unsigned char g_BMPHeader[] =
{
	0x42, 0x4D, 0x36, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
	0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0xfe, 0xff, 0xff, 0x01, 0x00, 0x20, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x13, 0x0B, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00
} ;





cBiomeMap::cBiomeMap(void) :
	m_CurrentRegionX(0),
	m_CurrentRegionZ(0),
	m_IsCurrentRegionValid(false)
{
}





void cBiomeMap::Finish(void)
{
	if (m_IsCurrentRegionValid)
	{
		StartNewRegion(0, 0);
	}
}





bool cBiomeMap::OnNewChunk(int a_ChunkX, int a_ChunkZ)
{
	int RegionX = (a_ChunkX < 0) ? (a_ChunkX - 31) / 32 : a_ChunkX / 32;
	int RegionZ = (a_ChunkZ < 0) ? (a_ChunkZ - 31) / 32 : a_ChunkZ / 32;
	if ((RegionX != m_CurrentRegionX) || (RegionZ != m_CurrentRegionZ))
	{
		if (m_IsCurrentRegionValid)
		{
			StartNewRegion(RegionX, RegionZ);
		}
		m_CurrentRegionX = RegionX;
		m_CurrentRegionZ = RegionZ;
	}
	m_IsCurrentRegionValid = true;
	m_CurrentChunkX = a_ChunkX;
	m_CurrentChunkZ = a_ChunkZ;
	m_CurrentChunkOffX = m_CurrentChunkX - m_CurrentRegionX * 32;
	m_CurrentChunkOffZ = m_CurrentChunkZ - m_CurrentRegionZ * 32;
	return false;
}





bool cBiomeMap::OnBiomes(const unsigned char * a_BiomeData)
{
	ASSERT(m_CurrentChunkOffX >= 0);
	ASSERT(m_CurrentChunkOffX < 32);
	ASSERT(m_CurrentChunkOffZ >= 0);
	ASSERT(m_CurrentChunkOffZ < 32);
	char * BaseBiomes = m_Biomes + m_CurrentChunkOffZ * 16 * 512 + m_CurrentChunkOffX * 16;
	for (int z = 0; z < 16; z++)
	{
		char * Row = BaseBiomes + z * 512;
		memcpy(Row, a_BiomeData + z * 16, 16);
	}  // for z
	return true;
}





void cBiomeMap::StartNewRegion(int a_RegionX, int a_RegionZ)
{
	AString FileName;
	Printf(FileName, "Biomes.%d.%d.bmp", m_CurrentRegionX, m_CurrentRegionZ);
	cFile f;
	if (!f.Open(FileName, cFile::fmWrite))
	{
		LOG("Cannot open file \"%s\" for writing the biome map. Data for this region lost.", FileName.c_str());
	}
	else
	{
		f.Write(g_BMPHeader, sizeof(g_BMPHeader));
		for (int z = 0; z < 512; z++)
		{
			int RowData[512];
			unsigned char * BiomeRow = (unsigned char *)m_Biomes + z * 512;
			for (int x = 0; x < 512; x++)
			{
				RowData[x] = g_BiomeColors[BiomeRow[x]];
			}
			f.Write(RowData, sizeof(RowData));
		}  // for z
	}

	memset(m_Biomes, 0, sizeof(m_Biomes));
	m_CurrentRegionX = a_RegionX;
	m_CurrentRegionZ = a_RegionZ;
}





////////////////////////////////////////////////////////////////////////////////
// cBiomeMapFactory:

cBiomeMapFactory::~cBiomeMapFactory()
{
	// Force all threads to save their last regions:
	for (cCallbacks::iterator itr = m_Callbacks.begin(), end = m_Callbacks.end(); itr != end; ++itr)
	{
		((cBiomeMap *)(*itr))->Finish();
	}
	// TODO: Join all the files into one giant image file
}
