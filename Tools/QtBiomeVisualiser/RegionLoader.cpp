#include "Globals.h"
#include "RegionLoader.h"
#include "ChunkSource.h"
#include "Region.h"





volatile bool RegionLoader::m_IsShuttingDown = false;





RegionLoader::RegionLoader(int a_RegionX, int a_RegionZ, RegionPtr a_Region, ChunkSourcePtr a_ChunkSource) :
	m_RegionX(a_RegionX),
	m_RegionZ(a_RegionZ),
	m_Region(a_Region),
	m_ChunkSource(a_ChunkSource)
{
}





void RegionLoader::run()
{
	// Load all the chunks in this region:
	for (int z = 0; z < 32; z++)
	{
		for (int x = 0; x < 32; x++)
		{
			m_ChunkSource->getChunkBiomes(m_RegionX * 32 + x, m_RegionZ * 32 + z, m_Region->getRelChunk(x, z));
			if (m_IsShuttingDown)
			{
				return;
			}
		}
	}
	m_Region->m_IsValid = true;

	emit loaded(m_RegionX, m_RegionZ);
}




