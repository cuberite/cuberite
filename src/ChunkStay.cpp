
// ChunkStay.cpp

// Implements the cChunkStay class representing a base for classes that keep chunks loaded

#include "Globals.h"
#include "ChunkStay.h"
#include "ChunkMap.h"





cChunkStay::cChunkStay(void) :
	m_ChunkMap(nullptr)
{
}





cChunkStay::~cChunkStay()
{
	Clear();
}





void cChunkStay::Clear(void)
{
	ASSERT(m_ChunkMap == nullptr);
	m_Chunks.clear();
}





void cChunkStay::Add(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_ChunkMap == nullptr);

	for (cChunkCoordsVector::const_iterator itr = m_Chunks.begin(); itr != m_Chunks.end(); ++itr)
	{
		if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkZ == a_ChunkZ))
		{
			// Already present
			return;
		}
	}  // for itr - Chunks[]
	m_Chunks.push_back(cChunkCoords(a_ChunkX, a_ChunkZ));
}





void cChunkStay::Remove(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_ChunkMap == nullptr);

	for (cChunkCoordsVector::iterator itr = m_Chunks.begin(); itr != m_Chunks.end(); ++itr)
	{
		if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkZ == a_ChunkZ))
		{
			// Found, un-"stay"
			m_Chunks.erase(itr);
			return;
		}
	}  // for itr - m_Chunks[]
}





void cChunkStay::Enable(cChunkMap & a_ChunkMap)
{
	ASSERT(m_ChunkMap == nullptr);

	m_OutstandingChunks = m_Chunks;
	m_ChunkMap = &a_ChunkMap;
	a_ChunkMap.AddChunkStay(*this);
}





void cChunkStay::Disable(void)
{
	ASSERT(m_ChunkMap != nullptr);

	cChunkMap * ChunkMap = m_ChunkMap;
	m_ChunkMap = nullptr;
	ChunkMap->DelChunkStay(*this);
}





bool cChunkStay::ChunkAvailable(int a_ChunkX, int a_ChunkZ)
{
	// Check if this is a chunk that we want:
	bool IsMine = false;
	for (cChunkCoordsVector::iterator itr = m_OutstandingChunks.begin(), end = m_OutstandingChunks.end(); itr != end; ++itr)
	{
		if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkZ == a_ChunkZ))
		{
			m_OutstandingChunks.erase(itr);
			IsMine = true;
			break;
		}
	}  // for itr - m_OutstandingChunks[]
	if (!IsMine)
	{
		return false;
	}

	// Call the appropriate callbacks:
	OnChunkAvailable(a_ChunkX, a_ChunkZ);
	if (m_OutstandingChunks.empty())
	{
		return OnAllChunksAvailable();
	}
	return false;
}




