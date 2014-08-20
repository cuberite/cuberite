
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
Note that the data needs to be compressed only *after* the query finishes,
because the query callbacks run with ChunkMap's CS locked.

A client may remove itself from all direct requests(QueueSendChunkTo()) by calling RemoveClient();
this ensures that the client's Send() won't be called anymore by ChunkSender.
Note that it may be called by world's BroadcastToChunk() if the client is still in the chunk.
*/



#pragma once

#include "OSSupport/IsThread.h"
#include "ChunkDef.h"
#include "ChunkDataCallback.h"





class cWorld;
class cClientHandle;





// fwd:
class cChunkSender;





/// Callback that can be used to notify chunk sender upon another chunkcoord notification
class cNotifyChunkSender :
	public cChunkCoordCallback
{
	virtual void Call(int a_ChunkX, int a_ChunkZ) override;

	cChunkSender * m_ChunkSender;
public:
	cNotifyChunkSender(cChunkSender * a_ChunkSender) : m_ChunkSender(a_ChunkSender) {}
	
	void SetChunkSender(cChunkSender * a_ChunkSender)
	{
		m_ChunkSender = a_ChunkSender;
	}
} ;





class cChunkSender:
	public cIsThread,
	public cChunkDataSeparateCollector
{
	typedef cIsThread super;
public:
	cChunkSender(void);
	~cChunkSender();
	
	bool Start(cWorld * a_World);
	
	void Stop(void);
	
	/// Notifies that a chunk has become ready and it should be sent to all its clients
	void ChunkReady(int a_ChunkX, int a_ChunkZ);
	
	/// Queues a chunk to be sent to a specific client
	void QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client);
	
	/// Removes the a_Client from all waiting chunk send operations
	void RemoveClient(cClientHandle * a_Client);
	
protected:

	/// Used for sending chunks to specific clients
	struct sSendChunk
	{
		int m_ChunkX;
		int m_ChunkY;
		int m_ChunkZ;
		cClientHandle * m_Client;
		
		sSendChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client) :
			m_ChunkX(a_ChunkX),
			m_ChunkY(a_ChunkY),
			m_ChunkZ(a_ChunkZ),
			m_Client(a_Client)
		{
		}
		
		bool operator ==(const sSendChunk & a_Other)
		{
			return (
				(a_Other.m_ChunkX == m_ChunkX) &&
				(a_Other.m_ChunkY == m_ChunkY) &&
				(a_Other.m_ChunkZ == m_ChunkZ) &&
				(a_Other.m_Client == m_Client)
			);
		}
	} ;
	typedef std::list<sSendChunk> sSendChunkList;

	struct sBlockCoord
	{
		int m_BlockX;
		int m_BlockY;
		int m_BlockZ;
		
		sBlockCoord(int a_BlockX, int a_BlockY, int a_BlockZ) :
			m_BlockX(a_BlockX),
			m_BlockY(a_BlockY),
			m_BlockZ(a_BlockZ)
		{
		}
	} ;

	typedef std::vector<sBlockCoord> sBlockCoords;
	
	cWorld * m_World;
	
	cCriticalSection  m_CS;
	cChunkCoordsList  m_ChunksReady;
	sSendChunkList    m_SendChunks;
	cEvent            m_evtQueue;  // Set when anything is added to m_ChunksReady
	cEvent            m_evtRemoved;  // Set when removed clients are safe to be deleted
	int               m_RemoveCount;  // Number of threads waiting for a client removal (m_evtRemoved needs to be set this many times)
	
	cNotifyChunkSender m_Notify;  // Used for chunks that don't have a valid lighting - they will be re-queued after lightcalc
	
	// Data about the chunk that is being sent:
	// NOTE that m_BlockData[] is inherited from the cChunkDataCollector
	unsigned char m_BiomeMap[cChunkDef::Width * cChunkDef::Width];
	sBlockCoords  m_BlockEntities;  // Coords of the block entities to send
	// TODO: sEntityIDs    m_Entities;       // Entity-IDs of the entities to send
	
	// cIsThread override:
	virtual void Execute(void) override;
	
	// cChunkDataCollector overrides:
	// (Note that they are called while the ChunkMap's CS is locked - don't do heavy calculations here!)
	virtual void BiomeData    (const cChunkDef::BiomeMap * a_BiomeMap) override;
	virtual void Entity       (cEntity *      a_Entity) override;
	virtual void BlockEntity  (cBlockEntity * a_Entity) override;

	/// Sends the specified chunk to a_Client, or to all chunk clients if a_Client == NULL
	void SendChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client);
} ;




