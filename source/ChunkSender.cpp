
// ChunkSender.cpp

// Interfaces to the cChunkSender class representing the thread that waits for chunks becoming ready (loaded / generated) and sends them to clients





#include "Globals.h"
#include "ChunkSender.h"
#include "cWorld.h"
#include "packets/cPacket_MapChunk.h"
#include "packets/cPacket_PreChunk.h"
#include "cBlockEntity.h"





cChunkSender::cChunkSender(void) :
	super("ChunkSender"),
	m_World(NULL)
{
}





cChunkSender::~cChunkSender()
{
	mShouldTerminate = true;
	m_Event.Set();
}





bool cChunkSender::Start(cWorld * a_World)
{
	m_World = a_World;
	return super::Start();
}





void cChunkSender::ChunkReady(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	// This is probably never gonna be called twice for the same chunk, and if it is, we don't mind, so we don't check
	{
		cCSLock Lock(m_CS);
		m_ChunksReady.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
	}
	m_Event.Set();
}





void cChunkSender::Execute(void)
{
	while (!mShouldTerminate)
	{
		cCSLock Lock(m_CS);
		while (m_ChunksReady.empty())
		{
			cCSUnlock Unlock(Lock);
			m_Event.Wait();
			if (mShouldTerminate)
			{
				return;
			}
		}  // while (empty)
		
		// Take one from the queue:
		cChunkCoords Coords(m_ChunksReady.front());
		m_ChunksReady.pop_front();
		Lock.Unlock();
		
		ASSERT(m_World != NULL);
		
		// Send it to anyone waiting:
		m_World->GetChunkData(Coords.m_ChunkX, Coords.m_ChunkY, Coords.m_ChunkZ, this);
		cPacket_PreChunk PreChunk(Coords.m_ChunkX, Coords.m_ChunkZ, true);
		cPacket_MapChunk MapChunk(Coords.m_ChunkX, Coords.m_ChunkY, Coords.m_ChunkZ, m_BlockData);
		m_World->BroadcastToChunk(Coords.m_ChunkX, Coords.m_ChunkY, Coords.m_ChunkZ, PreChunk);
		m_World->BroadcastToChunk(Coords.m_ChunkX, Coords.m_ChunkY, Coords.m_ChunkZ, MapChunk);
		
		// Send entity creation packets:
		for (PacketList::iterator itr = m_Packets.begin(); itr != m_Packets.end(); ++itr)
		{
			m_World->BroadcastToChunk(Coords.m_ChunkX, Coords.m_ChunkY, Coords.m_ChunkZ, **itr);
			delete *itr;
		}  // for itr - m_Packets
		m_Packets.clear();
		
	}  // while (!mShouldTerminate)
}





void cChunkSender::BlockData(const char * a_Data)
{
	memcpy(m_BlockData, a_Data, cChunk::c_BlockDataSize);
}





void cChunkSender::BlockEntity(cBlockEntity * a_Entity)
{
	m_Packets.push_back(a_Entity->GetPacket());
}




void cChunkSender::Entity(cEntity * a_Entity)
{
	// Nothing needed yet, perhaps in the future when we save entities into chunks we'd like to send them upon load, too ;)
}




