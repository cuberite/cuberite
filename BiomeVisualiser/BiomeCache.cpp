
// BiomeCache.cpp

// Implements the cBiomeCache class representing a biome source that caches data from the underlying biome source

#include "Globals.h"
#include "BiomeCache.h"
#include "Timer.h"





static int GetNumCores(void)
{
	// Get number of cores by querying the system process affinity mask
	DWORD Affinity, ProcAffinity;
	GetProcessAffinityMask(GetCurrentProcess(), &ProcAffinity, &Affinity);
	int NumCores = 0;
	while (Affinity > 0)
	{
		if ((Affinity & 1) == 1)
		{
			NumCores++;
		}
		Affinity >>= 1;
	}  // while (Affinity > 0)
	return NumCores;
}





cBiomeCache::cBiomeCache(void) :
	m_Source(NULL),
	m_BaseX(-100000),
	m_BaseZ(-100000),
	m_Available(NULL),
	m_IsTerminatingThreads(false)
{
	int NumThreads = GetNumCores();
	NumThreads--;  // One core should be left for the system to run on ;)
	for (int i = NumThreads; i > 0; i--)
	{
		cThread * Thread = new cThread(*this);
		m_Threads.push_back(Thread);
		Thread->Start();
	}
}




cBiomeCache::~cBiomeCache()
{
	m_IsTerminatingThreads = true;
	for (cThreads::iterator itr = m_Threads.begin(), end = m_Threads.end(); itr != end; ++itr)
	{
		m_evtQueued.Set();
	}
	for (cThreads::iterator itr = m_Threads.begin(), end = m_Threads.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_Threads.clear();
	
	SetSource(NULL);
}





cBiomeSource::eAvailability cBiomeCache::GetBiome(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_Biomes)
{
	if (m_Source == NULL)
	{
		return baNever;
	}

	// Look up using the cache:
	int x = a_ChunkX - m_BaseX;
	int z = a_ChunkZ - m_BaseZ;
	if ((x < 0) || (x >= m_Width) || (z < 0) || (z >= m_Height))
	{
		// Outside the cached region
		return baNever;
	}
	
	cCSLock Lock(m_CS);
	cItem * Item = m_Available[x + m_Width * z];
	if (Item == NULL)
	{
		// Item hasn't been processed yet
		return baLater;
	}
	if (Item->m_IsValid)
	{
		memcpy(a_Biomes, Item->m_Biomes, sizeof(a_Biomes));
		return baNow;
	}

	// Item has been processed, but the underlying source refused to give the data to us
	return baNever;
}





void cBiomeCache::HintViewArea(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ)
{
	cTimer Timer("Cache: HintViewArea");
	
	if (
		(a_MinChunkX == m_BaseX) && 
		(a_MaxChunkX == m_BaseX + m_Width - 1) &&
		(a_MinChunkZ == m_BaseZ) &&
		(a_MaxChunkZ == m_BaseZ + m_Height - 1)
	)
	{
		// The same set of parameters, bail out
		return;
	}
	
	if (m_Source != NULL)
	{
		m_Source->HintViewArea(a_MinChunkX, a_MaxChunkX, a_MinChunkZ, a_MaxChunkZ);
	}
	
	int NewWidth  = a_MaxChunkX - a_MinChunkX + 1;
	int NewHeight = a_MaxChunkZ - a_MinChunkZ + 1;
	
	// Make a new empty cache table:
	pItem * NewAvailable = new pItem[NewWidth * NewHeight];
	for (int i = NewWidth * NewHeight - 1; i >= 0; --i)
	{
		NewAvailable[i] = NULL;
	}

	// Move the common contents of the old table into the new table:
	cCSLock Lock(m_CS);
	for (int z = 0; z < NewHeight; z++)
	{
		int OldZ = z + a_MinChunkZ - m_BaseZ;
		if ((OldZ < 0) || (OldZ >= m_Height))
		{
			continue;
		}
		for (int x = 0; x < NewWidth; x++)
		{
			int OldX = x + a_MinChunkX - m_BaseX;
			if ((OldX < 0) || (OldX >= m_Width))
			{
				continue;
			}
			NewAvailable[x + NewWidth * z] = m_Available[OldX + m_Width * OldZ];
			m_Available[OldX + m_Width * OldZ] = NULL;
		}  // for x
	}  // for z
	
	// All items that aren't common go into the pool:
	for (int idx = 0, z = 0; z < m_Height; z++)
	{
		for (int x = 0; x < m_Width; ++x, ++idx)
		{
			if (m_Available[idx] != NULL)
			{
				m_Pool.push_back(m_Available[idx]);
				m_Available[idx] = NULL;
			}
		}
	}
	
	// Replace the cache table:
	delete m_Available;
	m_Available = NewAvailable;
	m_Width = NewWidth;
	m_Height = NewHeight;
	m_BaseX = a_MinChunkX;
	m_BaseZ = a_MinChunkZ;

	// Remove all items outside the coords:
	FilterOutItems(m_Queue, a_MinChunkX, a_MaxChunkX, a_MinChunkZ, a_MaxChunkZ);
	
	// Queue all items from inside the coords into m_Queue:
	for (int z = 0; z < NewHeight; z++)
	{
		for (int x = 0; x < NewWidth; x++)
		{
			if (m_Available[x + m_Width * z] != NULL)
			{
				// Already calculated, skip
				continue;
			}
			
			if (m_Pool.empty())
			{
				m_Pool.push_back(new cItem(x + a_MinChunkX, z + a_MinChunkZ));
			}
			ASSERT(!m_Pool.empty());
			m_Pool.back()->m_ChunkX = x + a_MinChunkX;
			m_Pool.back()->m_ChunkZ = z + a_MinChunkZ;
			m_Queue.push_back(m_Pool.back());
			m_Pool.pop_back();
			m_evtQueued.Set();
		}  // for x
	}  // for z
}





void cBiomeCache::SetSource(cBiomeSource * a_Source)
{
	// TODO: Stop all threads, so that they don't use the source anymore!
	
	delete m_Source;
	m_Source = a_Source;
	
	// Invalidate cache contents:
	cCSLock Lock(m_CS);
	m_BaseX = -10000;
	m_BaseZ = -10000;
	m_Pool.splice(m_Pool.end(), m_Queue);
}





void cBiomeCache::FilterOutItems(cItems & a_Items, int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ)
{
	for (cItems::iterator itr = a_Items.begin(); itr != a_Items.end();)
	{
		if (
			((*itr)->m_ChunkX < a_MinChunkX) ||
			((*itr)->m_ChunkX > a_MaxChunkX) ||
			((*itr)->m_ChunkX < a_MinChunkX) ||
			((*itr)->m_ChunkX > a_MaxChunkX)
		)
		{
			m_Pool.push_back(*itr);
			itr = a_Items.erase(itr);
		}
		else
		{
			++itr;
		}
	}
}





void cBiomeCache::thrProcessQueueItem(void)
{
	cItem * Item = NULL;
	{
		cCSLock Lock(m_CS);
		if (m_Queue.empty())
		{
			cCSUnlock Unlock(Lock);
			m_evtQueued.Wait();
		}
		if (m_IsTerminatingThreads || m_Queue.empty())
		{
			// We've been woken up only to die / spurious wakeup
			return;
		}
		Item = m_Queue.back();
		m_Queue.pop_back();
	}
	
	// Process the item:
	Item->m_IsValid = (m_Source->GetBiome(Item->m_ChunkX, Item->m_ChunkZ, Item->m_Biomes) == baNow);
	
	// Store result:
	cCSLock Lock(m_CS);
	int x = Item->m_ChunkX - m_BaseX;
	int z = Item->m_ChunkZ - m_BaseZ;
	if ((x < 0) || (x >= m_Width) || (z < 0) || (z >= m_Height))
	{
		// The cache rectangle has changed under our fingers, drop this chunk
		return;
	}
	m_Available[x + m_Width * z] = Item;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBiomeCache::cItem:

cBiomeCache::cItem::cItem(int a_ChunkX, int a_ChunkZ) :
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ)
{
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cBiomeCache::cThread:

cBiomeCache::cThread::cThread(cBiomeCache & a_Parent) :
	super("Biome cache thread"),
	m_Parent(a_Parent)
{
}





void cBiomeCache::cThread::Execute(void)
{
	while (!m_ShouldTerminate && !m_Parent.m_IsTerminatingThreads)
	{
		m_Parent.thrProcessQueueItem();
	}
}




