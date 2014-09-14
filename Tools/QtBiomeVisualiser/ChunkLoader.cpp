#include "Globals.h"
#include "ChunkLoader.h"
#include "ChunkSource.h"





ChunkLoader::ChunkLoader(int a_ChunkX, int a_ChunkZ, ChunkPtr a_Chunk, ChunkSourcePtr a_ChunkSource) :
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
	m_Chunk(a_Chunk),
	m_ChunkSource(a_ChunkSource)
{
}





void ChunkLoader::run()
{
	m_ChunkSource->getChunkBiomes(m_ChunkX, m_ChunkZ, m_Chunk);
	emit loaded(m_ChunkX, m_ChunkZ);
}




