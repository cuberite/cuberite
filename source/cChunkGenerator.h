
// cChunkGenerator.h

// Interfaces to the cChunkGenerator class representing the thread that generates chunks

// The object takes requests for generating chunks and processes them in a separate thread one by one.
// The requests are not added to the queue if there is already a request with the same coords
// Before generating, the thread checks if the chunk hasn't been already generated.
// It is theoretically possible to have multiple generator threads by having multiple instances of this object (if the cChunkPtr is thread-safe)
// If the generator queue is overloaded, the generator skips chunks with no clients in them





#pragma once

#include "cIsThread.h"
#include "cChunk.h"





class cWorld;
class cWorldGenerator;





class cChunkGenerator :
	cIsThread
{
	typedef cIsThread super;
	
public:

	cChunkGenerator (void);
	~cChunkGenerator();

	bool Start(cWorld * a_World, const AString & a_WorldGeneratorName);
	void Stop(void);

	void GenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ);  // Queues the chunk for generation; removes duplicate requests

private:

	cWorld * m_World;
	cWorldGenerator * m_pWorldGenerator;

	cCriticalSection m_CS;
	cChunkCoordsList m_Queue;
	cEvent           m_Event;  // Set when an item is added to the queue or the thread should terminate

	// cIsThread override:
	virtual void Execute(void) override;

	void DoGenerate(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
};




