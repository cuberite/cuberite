
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
#include "ChunkDataCallback.h"

#include <unordered_set>
#include <unordered_map>





class cWorld;
class cClientHandle;





// fwd:
class cChunkSender;





class cChunkSender:
	public cIsThread,
	public cChunkDataCopyCollector
{
	typedef cIsThread super;
public:
	cChunkSender(cWorld & a_World);
	virtual ~cChunkSender() override;

	enum eChunkPriority
	{
		E_CHUNK_PRIORITY_HIGH = 0,
		E_CHUNK_PRIORITY_MIDHIGH,
		E_CHUNK_PRIORITY_MEDIUM,
		E_CHUNK_PRIORITY_LOW,

	};

	bool Start();

	void Stop(void);

	/** Queues a chunk to be sent to a specific client */
	void QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, cClientHandle * a_Client);
	void QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, std::list<cClientHandle *> a_Client);

	/** Removes the a_Client from all waiting chunk send operations */
	void RemoveClient(cClientHandle * a_Client);

protected:

	struct sChunkQueue
	{
		eChunkPriority m_Priority;
		cChunkCoords m_Chunk;

		bool operator <(const sChunkQueue & a_Other) const
		{
			/* The Standard Priority Queue sorts from biggest to smallest
			return true here means you are smaller than the other object, and you get pushed down.

			The priorities go from HIGH (0) to LOW (2), so a smaller priority should mean further up the list
			therefore, return true (affirm we're "smaller", and get pushed down) only if our priority is bigger than theirs (they're more urgent)
			*/
			return this->m_Priority > a_Other.m_Priority;
		}
	};

	/** Used for sending chunks to specific clients */
	struct sSendChunk
	{
		cChunkCoords m_Chunk;
		std::unordered_set<cClientHandle *> m_Clients;
		eChunkPriority m_Priority;
		sSendChunk(cChunkCoords a_Chunk, eChunkPriority a_Priority) :
			m_Chunk(a_Chunk),
			m_Priority(a_Priority)
		{
		}
	};

	cWorld & m_World;

	cCriticalSection  m_CS;
	std::priority_queue<sChunkQueue> m_SendChunks;
	std::unordered_map<cChunkCoords, sSendChunk, cChunkCoordsHash> m_ChunkInfo;
	cEvent m_evtQueue;  // Set when anything is added to m_ChunksReady
	cEvent m_evtRemoved;  // Set when removed clients are safe to be deleted

	// Data about the chunk that is being sent:
	// NOTE that m_BlockData[] is inherited from the cChunkDataCollector
	unsigned char m_BiomeMap[cChunkDef::Width * cChunkDef::Width];
	std::vector<Vector3i> m_BlockEntities;  // Coords of the block entities to send
	// TODO: sEntityIDs    m_Entities;       // Entity-IDs of the entities to send

	// cIsThread override:
	virtual void Execute(void) override;

	// cChunkDataCollector overrides:
	// (Note that they are called while the ChunkMap's CS is locked - don't do heavy calculations here!)
	virtual void BiomeData    (const cChunkDef::BiomeMap * a_BiomeMap) override;
	virtual void Entity       (cEntity *      a_Entity) override;
	virtual void BlockEntity  (cBlockEntity * a_Entity) override;

	/** Sends the specified chunk to all the specified clients */
	void SendChunk(int a_ChunkX, int a_ChunkZ, std::unordered_set<cClientHandle *> a_Clients);
} ;



