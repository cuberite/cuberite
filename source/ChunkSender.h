
// ChunkSender.h

// Interfaces to the cChunkSender class representing the thread that waits for chunks becoming ready (loaded / generated) and sends them to clients

/*
The whole thing is a thread that runs in a loop, waiting for either:
	"finished chunks" (ChunkReady()), or
	"chunks to send" (QueueSendChunkTo() ) 
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

#include "cIsThread.h"
#include "ChunkDef.h"
#include "packets/cPacket.h"





class cWorld;
class cClientHandle;





class cChunkSender:
	public cIsThread,
	public cChunkDataCollector
{
	typedef cIsThread super;
public:
	cChunkSender(void);
	~cChunkSender();
	
	bool Start(cWorld * a_World);
	
	void Stop(void);
	
	/// Notifies that a chunk has become ready and it should be sent to all its clients
	void ChunkReady(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
	/// Queues a chunk to be sent to a specific client
	void QueueSendChunkTo(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client);
	
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
	};
	typedef std::list<sSendChunk> sSendChunkList;

	cWorld * m_World;
	
	cCriticalSection  m_CS;
	cChunkCoordsList  m_ChunksReady;
	sSendChunkList    m_SendChunks;
	cEvent            m_evtQueue;  // Set when anything is added to m_ChunksReady
	cEvent            m_evtRemoved;  // Set when removed clients are safe to be deleted
	
	// Data about the chunk that is being sent:
	// NOTE that m_BlockData[] is inherited from the cChunkDataCollector
	BLOCKTYPE  m_BiomeData[cChunkDef::Width * cChunkDef::Width];
	PacketList m_Packets;  // Accumulator for the entity-packets to send
	
	// cIsThread override:
	virtual void Execute(void) override;
	
	// cChunkDataCollector overrides:
	// (Note that they are called while the ChunkMap's CS is locked - don't do heavy calculations here!)
	virtual void Entity       (cEntity *      a_Entity) override;
	virtual void BlockEntity  (cBlockEntity * a_Entity) override;

	/// Sends the specified chunk to a_Client, or to all chunk clients if a_Client == NULL
	void SendChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client);
} ;




