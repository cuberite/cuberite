
#include "Globals.h"

#include "cChunkGenerator.h"
#include "cChunkMap.h"
#include "cChunk.h"
#include "cWorld.h"

#include "cMCLogger.h"

typedef std::pair<int, int> ChunkCoord;
typedef std::list< ChunkCoord > ChunkCoordList;





/// If the generation queue size exceeds this number, a warning will be output
#define QUEUE_WARNING_LIMIT 1000





struct cChunkGenerator::sChunkGeneratorState
{
	cCriticalSection m_CriticalSection;  // For protecting the variables in this struct

	ChunkCoordList GenerateQueue;
	ChunkCoord CurrentlyGeneratingCoords;
	cChunk* pCurrentlyGenerating;
	bool bCurrentlyGenerating;

	cSemaphore m_Semaphore;
	cThread * pThread;

	bool bStop;

	sChunkGeneratorState(void)
		: m_Semaphore(1, 0)
		, pThread( 0 )
		, bStop( false )
		, bCurrentlyGenerating( false )
		, pCurrentlyGenerating( false )
	{}
};





cChunkGenerator::cChunkGenerator( cChunkMap* a_pChunkMap )
	: m_pState( new sChunkGeneratorState )
	, m_pChunkMap( a_pChunkMap )
{
	m_pState->pThread = new cThread( GenerateThread, this, "cChunkGenerator::GenerateThread" );
	m_pState->pThread->Start( true );
}





cChunkGenerator::~cChunkGenerator()
{
	m_pState->bStop = true;

	m_pState->m_Semaphore.Signal(); // Signal so thread can continue and exit
	delete m_pState->pThread;

	delete m_pState;
}





void cChunkGenerator::GenerateChunk( int a_X, int a_Z )
{
	cCSLock Lock(&m_pState->m_CriticalSection);

	if (m_pState->bCurrentlyGenerating)
	{
		if ((m_pState->CurrentlyGeneratingCoords.first == a_X) && (m_pState->CurrentlyGeneratingCoords.second == a_Z))
		{
			return;	// Already generating this chunk, so ignore
		}
	}

	m_pState->GenerateQueue.remove( ChunkCoord(a_X, a_Z) );
	if (m_pState->GenerateQueue.size() >= QUEUE_WARNING_LIMIT)
	{
		LOGWARN("WARNING: Adding chunk (%i, %i) to generation queue; Queue is too big! (%i)", a_X, a_Z, m_pState->GenerateQueue.size() );
	}
	m_pState->GenerateQueue.push_back( ChunkCoord(a_X, a_Z) );

	Lock.Unlock();
	
	m_pState->m_Semaphore.Signal();
}





void cChunkGenerator::GenerateThread( void* a_Params )
{
	// Cache some values for easy access (they are all references/pointers)
	cChunkGenerator * self = (cChunkGenerator*)a_Params;
	sChunkGeneratorState * m_pState = self->m_pState;
	ChunkCoordList & GenerateQueue = m_pState->GenerateQueue;
	cChunkMap & ChunkMap = *self->m_pChunkMap;
	cCriticalSection * CriticalSection = &m_pState->m_CriticalSection;
	cSemaphore & Semaphore = m_pState->m_Semaphore;

	while (!m_pState->bStop)
	{
		cCSLock Lock(CriticalSection);
		if (GenerateQueue.size() == 0)
		{
			cCSUnlock Unlock(Lock);
			Semaphore.Wait();
		}
		if (m_pState->bStop) break;
		
		ChunkCoord coord = *GenerateQueue.begin();		// Get next coord from queue
		GenerateQueue.erase( GenerateQueue.begin() );	// Remove coordinate from queue
		m_pState->bCurrentlyGenerating = true;
		m_pState->CurrentlyGeneratingCoords = coord;
		Lock.Unlock();			// Unlock ASAP

		ChunkMap.GetWorld()->LockChunks();
		if( ChunkMap.GetChunk( coord.first, 0, coord.second ) ) // Make sure it has not been loaded in the meantime. Don't want to generate the same chunk twice
		{														// This is possible when forcing the server to generate a chunk in the main thread
			ChunkMap.GetWorld()->UnlockChunks();
			continue;
		}
		ChunkMap.GetWorld()->UnlockChunks();

		LOGINFO("cChunkGenerator generating chunk %i %i", coord.first, coord.second );
		cChunk* Chunk = new cChunk( coord.first, 0, coord.second, ChunkMap.GetWorld() );

		Lock.Lock();
		m_pState->pCurrentlyGenerating = Chunk;
		Lock.Unlock();

		Chunk->Initialize(); // Generate the chunk

		ChunkMap.GetWorld()->LockChunks();
		ChunkMap.AddChunk( Chunk );
		ChunkMap.GetWorld()->UnlockChunks();

		Lock.Lock();
		m_pState->bCurrentlyGenerating = false;
		m_pState->pCurrentlyGenerating = 0;
		Lock.Unlock();
	}  // while (!bStop)
}




