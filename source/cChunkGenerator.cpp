#include "MemoryLeak.h"
#include "cChunkGenerator.h"
#include "cChunkMap.h"
#include "cChunk.h"
#include "cWorld.h"

#include "cThread.h"
#include "cCriticalSection.h"
#include "cSemaphore.h"

#include "cMCLogger.h"

#include <list>

typedef std::pair<int, int> ChunkCoord;
typedef std::list< ChunkCoord > ChunkCoordList;

#define MAX_SEMAPHORES 1000

struct cChunkGenerator::sChunkGeneratorState
{
	sChunkGeneratorState()
		: pCriticalSection( 0 )
		, pSemaphore( 0 )
		, pThread( 0 )
		, bStop( false )
		, bCurrentlyGenerating( false )
		, pCurrentlyGenerating( false )
		, pChunkCriticalSection( 0 )
	{}
	ChunkCoordList GenerateQueue;			// Protected by pCriticalSection
	ChunkCoord CurrentlyGeneratingCoords;	// Protected by pCriticalSection
	cChunk* pCurrentlyGenerating;			// Protected by pCriticalSection
	bool bCurrentlyGenerating;				// Protected by pCriticalSection

	cCriticalSection* pCriticalSection;
	cSemaphore* pSemaphore;
	cThread* pThread;

	cCriticalSection* pChunkCriticalSection;// Specially for protecting the actual chunk that is currently being generated, and not just the variables in this struct

	bool bStop;
};

cChunkGenerator::cChunkGenerator( cChunkMap* a_pChunkMap )
	: m_pState( new sChunkGeneratorState )
	, m_pChunkMap( a_pChunkMap )
{
	m_pState->pCriticalSection = new cCriticalSection();
	m_pState->pSemaphore = new cSemaphore( MAX_SEMAPHORES, 0 );

	m_pState->pChunkCriticalSection = new cCriticalSection();

	m_pState->pThread = new cThread( GenerateThread, this, "cChunkGenerator::GenerateThread" );
	m_pState->pThread->Start( true );
}

cChunkGenerator::~cChunkGenerator()
{
	m_pState->bStop = true;

	m_pState->pSemaphore->Signal(); // Signal so thread can continue and exit
	delete m_pState->pThread;

	delete m_pState->pSemaphore;
	delete m_pState->pCriticalSection;
	delete m_pState->pChunkCriticalSection;
	delete m_pState; m_pState = 0;
}

void cChunkGenerator::GenerateChunk( int a_X, int a_Z )
{
	m_pState->pCriticalSection->Lock();

	if( m_pState->bCurrentlyGenerating )
	{
		if( m_pState->CurrentlyGeneratingCoords.first == a_X && m_pState->CurrentlyGeneratingCoords.second == a_Z )
		{
			m_pState->pCriticalSection->Unlock();
			return;	// Already generating this chunk, so ignore
		}
	}

	int SizeBefore = m_pState->GenerateQueue.size();

	m_pState->GenerateQueue.remove( ChunkCoord(a_X, a_Z) );
	if( m_pState->GenerateQueue.size() >= MAX_SEMAPHORES )
	{
		LOGWARN("WARNING: Can't add chunk (%i, %i) to generation queue: Queue is too big! (%i)", a_X, a_Z, m_pState->GenerateQueue.size() );
		m_pState->pCriticalSection->Unlock();
		return;
	}
	m_pState->GenerateQueue.push_back( ChunkCoord(a_X, a_Z) );
	int SizeAfter = m_pState->GenerateQueue.size();

	m_pState->pCriticalSection->Unlock();
	if( SizeBefore < SizeAfter )
		m_pState->pSemaphore->Signal();
}

void cChunkGenerator::GenerateThread( void* a_Params )
{
	// Cache some values for easy access (they are all references/pointers)
	cChunkGenerator* self = (cChunkGenerator*)a_Params;
	sChunkGeneratorState* m_pState = self->m_pState;
	ChunkCoordList& GenerateQueue = m_pState->GenerateQueue;
	cChunkMap& ChunkMap = *self->m_pChunkMap;
	cCriticalSection& CriticalSection = *m_pState->pCriticalSection;
	cSemaphore& Semaphore = *m_pState->pSemaphore;

	while( !m_pState->bStop )
	{
		Semaphore.Wait();
		if( m_pState->bStop ) break;

		CriticalSection.Lock();
		if( GenerateQueue.size() == 0 )
		{
			if( !m_pState->bStop ) LOGERROR("ERROR: Semaphore was signaled while GenerateQueue.size == 0");
			CriticalSection.Unlock();
			continue;
		}
		ChunkCoord coord = *GenerateQueue.begin();		// Get next coord from queue
		GenerateQueue.erase( GenerateQueue.begin() );	// Remove coordinate from queue
		m_pState->bCurrentlyGenerating = true;
		m_pState->CurrentlyGeneratingCoords = coord;
		CriticalSection.Unlock();			// Unlock ASAP

		ChunkMap.GetWorld()->LockChunks();
		if( ChunkMap.GetChunk( coord.first, 0, coord.second ) ) // Make sure it has not been loaded in the meantime. Don't want to generate the same chunk twice
		{														// This is possible when forcing the server to generate a chunk in the main thread
			ChunkMap.GetWorld()->UnlockChunks();
			continue;
		}
		ChunkMap.GetWorld()->UnlockChunks();

		LOGINFO("cChunkGenerator generating chunk %i %i", coord.first, coord.second );
		cChunk* Chunk = new cChunk( coord.first, 0, coord.second, ChunkMap.GetWorld() );

		CriticalSection.Lock();
		m_pState->pCurrentlyGenerating = Chunk;
		CriticalSection.Unlock();

		self->Lock(); // Protect the actual chunk
		Chunk->Initialize(); // Generate the chunk
		self->Unlock();

		ChunkMap.GetWorld()->LockChunks();
		ChunkMap.AddChunk( Chunk );
		ChunkMap.GetWorld()->UnlockChunks();

		CriticalSection.Lock();
		m_pState->bCurrentlyGenerating = false;
		m_pState->pCurrentlyGenerating = 0;
		CriticalSection.Unlock();
	}
}

cChunk* cChunkGenerator::GetCurrentlyGenerating()
{
	return m_pState->pCurrentlyGenerating;
}

void cChunkGenerator::Lock()
{
	m_pState->pChunkCriticalSection->Lock();
}

void cChunkGenerator::Unlock()
{
	m_pState->pChunkCriticalSection->Unlock();
}