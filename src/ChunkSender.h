
// ChunkSender.h

// Interfaces to the cChunkSender class representing the thread that waits for chunks becoming ready (loaded / generated) and sends them to clients

/*
The whole thing is a thread that runs in a loop, waiting for either:
	"finished chunks" (ChunkReady()), or
	"chunks to send" (QueueSendChunkTo())
to come to a queue.
And once they do, it requests the chunk data and sends it all away, either
	broadcasting (ChunkReady), or
	sends to a specific client (QueueSendChunkTo)
Chunk data is queried using the cChunkDataCallback interface.
It is cached inside the ChunkSender object during the query and then processed after the query ends.
Note that the data needs to be compressed only after the query finishes,
because the query callbacks run with ChunkMap's CS locked.

A client may remove itself from all direct requests(QueueSendChunkTo()) by calling RemoveClient();
this ensures that the client's Send() won't be called anymore by ChunkSender.
Note that it may be called by world's BroadcastToChunk() if the client is still in the chunk.
*/



#pragma once

#include "OSSupport/IsThread.h"
#include "ChunkDef.h"
#include "ChunkDataCallback.h"
#include "ChunkStay.h"





class cWorld;
class cClientHandle;





// fwd:
class cChunkSender;





class cChunkSender:
	public cIsThread
{
	typedef cIsThread super;
public:
	cChunkSender(cWorld & a_World);
	~cChunkSender();

	enum eChunkPriority
	{
		E_CHUNK_PRIORITY_HIGH = 0,
		E_CHUNK_PRIORITY_MIDHIGH,
		E_CHUNK_PRIORITY_MEDIUM,
		E_CHUNK_PRIORITY_LOW,

	};

	bool Start();

	void Stop(void);

	/** Queues a chunk to be sent to a specific client
	A chunkstay ensures that the chunk is valid before sending */
	void QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, const std::weak_ptr<cClientHandle> & a_Client);
	void QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, const std::vector<std::weak_ptr<cClientHandle>> & a_Clients);

protected:

	class cChunkQueue :
		public cChunkStay,
		public cChunkDataSeparateCollector
	{
	public:

		cChunkQueue(eChunkPriority a_Priority, cChunkCoords a_ChunkCoordinate, const std::weak_ptr<cClientHandle> & a_Client, cChunkSender & a_ChunkSender) :
			m_Priority(a_Priority),
			m_Chunk(a_ChunkCoordinate),
			m_Clients{ a_Client },
			m_ChunkSender(a_ChunkSender)
		{
			Add(a_ChunkCoordinate.m_ChunkX, a_ChunkCoordinate.m_ChunkZ);
		}

		eChunkPriority m_Priority;
		cChunkCoords m_Chunk;
		std::vector<std::weak_ptr<cClientHandle>> m_Clients;

		// Data about the chunk that is being sent:
		// NOTE that m_BlockData[] is inherited from the cChunkDataCollector
		unsigned char m_BiomeMap[cChunkDef::Width * cChunkDef::Width];

	private:
		cChunkSender & m_ChunkSender;

		// cChunkDataCollector overrides:
		// (Note that they are called while the ChunkMap's CS is locked - don't do heavy calculations here!)
		virtual void BiomeData(const cChunkDef::BiomeMap * a_BiomeMap) override;

		// cChunkStay overrides
		virtual void OnChunkAvailable(int a_ChunkX, int a_ChunkZ) override
		{
			UNUSED(a_ChunkX);
			UNUSED(a_ChunkZ);
		}
		virtual bool OnAllChunksAvailable(void) override;
		virtual void OnDisabled(void) override
		{
			delete this;
		}
	};

	cWorld & m_World;

	cCriticalSection  m_CS;
	std::vector<cChunkQueue *> m_LoadQueue;
	std::vector<cChunkQueue *> m_SendChunks;
	cEvent m_evtQueue;  // Set when anything is added to m_ChunksReady

	// cIsThread override:
	virtual void Execute(void) override;

	/** Sends the specified chunk to all the specified clients */
	void SendChunk(const cChunkQueue & a_Item);
} ;



