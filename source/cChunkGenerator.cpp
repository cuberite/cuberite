
#include "Globals.h"

#include "cChunkGenerator.h"
#include "cWorld.h"
#include "cWorldGenerator.h"
#include "cWorldGenerator_Test.h"





typedef std::pair<int, int> ChunkCoord;
typedef std::list< ChunkCoord > ChunkCoordList;





/// If the generation queue size exceeds this number, a warning will be output
const int QUEUE_WARNING_LIMIT = 1000;

/// If the generation queue size exceeds this number, chunks with no clients will be skipped
const int QUEUE_SKIP_LIMIT = 50;





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
	
	if (a_WorldGeneratorName.compare("Test") == 0 )
	{
		m_pWorldGenerator = new cWorldGenerator_Test();
	}
	else	// Default
	{
		m_pWorldGenerator = new cWorldGenerator();
	}
	
	return super::Start();
}





void cChunkGenerator::Stop(void)
{
	mShouldTerminate = true;
	m_Event.Set();
	Wait();
	
	delete m_pWorldGenerator;
	m_pWorldGenerator = NULL;
}





void cChunkGenerator::GenerateChunk(int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CS);
	
	// Check if it is already in the queue:
	for (cChunkCoordsList::iterator itr = m_Queue.begin(); itr != m_Queue.end(); ++itr)
	{
		if ((itr->m_ChunkX == a_ChunkX) && (itr->m_ChunkZ == a_ChunkZ))
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
	m_Queue.push_back(cChunkCoords(a_ChunkX, a_ChunkZ));
	
	m_Event.Set();
}





void cChunkGenerator::Execute(void)
{
	while (!mShouldTerminate)
	{
		cCSLock Lock(m_CS);
		while (m_Queue.size() == 0)
		{
			cCSUnlock Unlock(Lock);
			m_Event.Wait();
			if (mShouldTerminate)
			{
				return;
			}
		}
		
		cChunkCoords coords = m_Queue.front();		// Get next coord from queue
		m_Queue.erase( m_Queue.begin() );	// Remove coordinate from queue
		bool SkipEnabled = (m_Queue.size() > QUEUE_SKIP_LIMIT);
		Lock.Unlock();			// Unlock ASAP

		cChunkPtr Chunk = m_World->GetChunk(coords.m_ChunkX, 0, coords.m_ChunkZ);
		if ((Chunk != NULL) && (Chunk->IsValid() || (SkipEnabled && !Chunk->HasAnyClient())))
		{
			// Already generated / overload-skip, ignore request
			continue;
		}

		LOG("Generating chunk [%d, %d]", coords.m_ChunkX, coords.m_ChunkZ);
		m_pWorldGenerator->GenerateChunk(Chunk);
		
		Chunk->SetValid();
	}  // while (!bStop)
}




