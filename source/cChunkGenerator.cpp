
#include "Globals.h"

#include "cChunkGenerator.h"
#include "cWorld.h"
#include "cWorldGenerator.h"
#include "cWorldGenerator_Test.h"
#include "WGFlat.h"





/// If the generation queue size exceeds this number, a warning will be output
const int QUEUE_WARNING_LIMIT = 1000;

/// If the generation queue size exceeds this number, chunks with no clients will be skipped
const int QUEUE_SKIP_LIMIT = 500;





cChunkGenerator::cChunkGenerator(void)
	: super("cChunkGenerator")
	, m_World(NULL)
	, m_pWorldGenerator(NULL)
{
}





cChunkGenerator::~cChunkGenerator()
{
	Stop();
}





bool cChunkGenerator::Start(cWorld * a_World, const AString & a_WorldGeneratorName)
{
	m_World = a_World;
	
	if (NoCaseCompare(a_WorldGeneratorName, "Test") == 0 )
	{
		m_pWorldGenerator = new cWorldGenerator_Test(a_World);
	}
	else if (NoCaseCompare(a_WorldGeneratorName, "flat") == 0)
	{
		m_pWorldGenerator = new cWGFlat(a_World);
	}
	else	// Default
	{
		m_pWorldGenerator = new cWorldGenerator(a_World);
	}
	
	return super::Start();
}





void cChunkGenerator::Stop(void)
{
	m_ShouldTerminate = true;
	m_Event.Set();
	m_evtRemoved.Set();  // Wake up anybody waiting for empty queue
	Wait();
	
	delete m_pWorldGenerator;
	m_pWorldGenerator = NULL;
}





void cChunkGenerator::GenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	{
		cCSLock Lock(m_CS);
		
		// Check if it is already in the queue:
		for (cChunkCoordsList::iterator itr = m_Queue.begin(); itr != m_Queue.end(); ++itr)
		{
			if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkY == a_ChunkY) && (itr->m_ChunkZ == a_ChunkZ))
			{
				// Already in the queue, bail out
				return;
			}
		}  // for itr - m_Queue[]
		
		// Add to queue, issue a warning if too many:
		if (m_Queue.size() >= QUEUE_WARNING_LIMIT)
		{
			LOGWARN("WARNING: Adding chunk [%i, %i] to generation queue; Queue is too big! (%i)", a_ChunkX, a_ChunkZ, m_Queue.size());
		}
		m_Queue.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
	}
	
	m_Event.Set();
}





void cChunkGenerator::WaitForQueueEmpty(void)
{
	cCSLock Lock(m_CS);
	while (!m_ShouldTerminate && !m_Queue.empty())
	{
		cCSUnlock Unlock(Lock);
		m_evtRemoved.Wait();
	}
}





int cChunkGenerator::GetQueueLength(void)
{
	cCSLock Lock(m_CS);
	return (int)m_Queue.size();
}





void cChunkGenerator::Execute(void)
{
	while (!m_ShouldTerminate)
	{
		cCSLock Lock(m_CS);
		while (m_Queue.size() == 0)
		{
			cCSUnlock Unlock(Lock);
			m_Event.Wait();
			if (m_ShouldTerminate)
			{
				return;
			}
		}
		
		cChunkCoords coords = m_Queue.front();		// Get next coord from queue
		m_Queue.erase( m_Queue.begin() );	// Remove coordinate from queue
		bool SkipEnabled = (m_Queue.size() > QUEUE_SKIP_LIMIT);
		Lock.Unlock();			// Unlock ASAP
		m_evtRemoved.Set();

		if (m_World->IsChunkValid(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ))
		{
			// Already generated, ignore request
			continue;
		}
		
		if (SkipEnabled && !m_World->HasChunkAnyClients(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ))
		{
			LOGWARNING("Chunk generator overloaded, skipping chunk [%d, %d, %d] (HasClients: %d)", coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
			continue;
		}
		
		LOGD("Generating chunk [%d, %d, %d]", coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
		DoGenerate(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
		
		// Save the chunk right after generating, so that we don't have to generate it again on next run
		m_World->GetStorage().QueueSaveChunk(coords.m_ChunkX, coords.m_ChunkY, coords.m_ChunkZ);
	}  // while (!bStop)
}




void cChunkGenerator::DoGenerate(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	BLOCKTYPE BlockData[cChunkDef::BlockDataSize];
	cEntityList Entities;
	cBlockEntityList BlockEntities;
	m_pWorldGenerator->GenerateChunk(a_ChunkX, a_ChunkY, a_ChunkZ, BlockData, Entities, BlockEntities);
	
	m_World->ChunkDataGenerated(
		a_ChunkX, a_ChunkY, a_ChunkZ, 
		BlockData, 
		BlockData + cChunkDef::MetaOffset,
		BlockData + cChunkDef::LightOffset,
		BlockData + cChunkDef::SkyLightOffset,
		NULL,
		Entities, BlockEntities
	);
	
	m_pWorldGenerator->PostGenerateChunk(a_ChunkX, a_ChunkY, a_ChunkZ);
}




