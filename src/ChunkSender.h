
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
#include "Protocol/ChunkDataSerializer.h"





class cWorld;
class cClientHandle;





// fwd:
class cChunkSender;





class cChunkSender:
	public cIsThread,
	public cChunkDataCopyCollector
{
	using Super = cIsThread;

public:

	cChunkSender(cWorld & a_World);
	virtual ~cChunkSender() override;

	/** Tag indicating urgency of chunk to be sent.
	Order MUST be from least to most urgent. */
	enum class Priority
	{
		Low,
		Medium,
		High,
		Critical
	};

	void Stop(void);

	/** Queues a chunk to be sent to a specific client */
	void QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, Priority a_Priority, cClientHandle * a_Client);
	void QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, Priority a_Priority, cChunkClientHandles a_Client);

	/** Removes the a_Client from all waiting chunk send operations */
	void RemoveClient(cClientHandle * a_Client);

protected:

	struct sChunkQueue
	{
		Priority m_Priority;
		cChunkCoords m_Chunk;

		bool operator <(const sChunkQueue & a_Other) const
		{
			// The operator will return true to affirm we're less urgent than Other
			// This comparison depends on the Priority enum ordering lower priority as smaller:
			return m_Priority < a_Other.m_Priority;
		}
	};

	/** Used for sending chunks to specific clients */
	struct sSendChunk
	{
		cChunkCoords m_Chunk;
		std::unordered_set<cClientHandle *> m_Clients;
		Priority m_Priority;
		sSendChunk(cChunkCoords a_Chunk, Priority a_Priority) :
			m_Chunk(a_Chunk),
			m_Priority(a_Priority)
		{
		}
	};

	cWorld & m_World;

	/** An instance of a chunk serializer, held to maintain its internal cache. */
	cChunkDataSerializer m_Serializer;

	cCriticalSection  m_CS;
	std::priority_queue<sChunkQueue> m_SendChunks;
	std::unordered_map<cChunkCoords, sSendChunk, cChunkCoordsHash> m_ChunkInfo;
	cEvent m_evtQueue;  // Set when anything is added to m_ChunksReady
	cEvent m_evtRemoved;  // Set when removed clients are safe to be deleted

	// Data about the chunk that is being sent:
	// NOTE that m_BlockData[] is inherited from the cChunkDataCollector
	unsigned char m_BiomeMap[cChunkDef::Width * cChunkDef::Width];
	std::vector<Vector3i> m_BlockEntities;  // Coords of the block entities to send
	std::vector<UInt32> m_EntityIDs;        // Entity-IDs of the entities to send

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



