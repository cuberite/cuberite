/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// ChunkStay.cpp

// Implements the cChunkStay class representing a base for classes that keep chunks loaded

#include "Globals.h"
#include "ChunkStay.h"
#include "ChunkMap.h"





cChunkStay::cChunkStay(void) :
	m_ChunkMap(NULL)
{
}





cChunkStay::~cChunkStay()
{
	Clear();
}





void cChunkStay::Clear(void)
{
	if (m_ChunkMap != NULL)
	{
		Disable();
	}
	m_Chunks.clear();
}





void cChunkStay::Add(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_ChunkMap == NULL);

	for (cChunkCoordsVector::const_iterator itr = m_Chunks.begin(); itr != m_Chunks.end(); ++itr)
	{
		if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkZ == a_ChunkZ))
		{
			// Already present
			return;
		}
	}  // for itr - Chunks[]
	m_Chunks.push_back(cChunkCoords(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ));
}





void cChunkStay::Remove(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(m_ChunkMap == NULL);

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
	ASSERT(m_ChunkMap == NULL);
	
	m_OutstandingChunks = m_Chunks;
	m_ChunkMap = &a_ChunkMap;
	a_ChunkMap.AddChunkStay(*this);
}





void cChunkStay::Disable(void)
{
	ASSERT(m_ChunkMap != NULL);
	
	m_ChunkMap->DelChunkStay(*this);
	m_ChunkMap = NULL;
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




