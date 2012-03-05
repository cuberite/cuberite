
// ChunkSender.h

// Interfaces to the cChunkSender class representing the thread that waits for chunks becoming ready (loaded / generated) and sends them to clients





#pragma once

#include "cIsThread.h"
#include "cChunk.h"
#include "packets/cPacket.h"





class cWorld;





class cChunkSender:
	public cIsThread,
	public cChunkDataCallback
{
	typedef cIsThread super;
public:
	cChunkSender(void);
	~cChunkSender();
	
	bool Start(cWorld * a_World);
	
	void ChunkReady(int a_ChunkX, int a_ChunkY, int a_ChunkZ);
	
protected:

	cWorld * m_World;
	
	cCriticalSection m_CS;
	cChunkCoordsList m_ChunksReady;
	cEvent           m_Event;  // Set when anything is added to m_ChunksReady
	
	// Data about the chunk that is being sent:
	char       m_BlockData[cChunk::c_BlockDataSize];
	PacketList m_Packets;  // Accumulator for the entity-packets to send
	
	// cIsThread override:
	virtual void Execute(void) override;
	
	// cChunkDataCallback overrides:
	virtual void BlockData(const char * a_Data) override;
	virtual void Entity(cEntity * a_Entity) override;
	virtual void BlockEntity(cBlockEntity * a_Entity) override;

} ;




