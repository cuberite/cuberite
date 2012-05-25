
// ChunkSender.cpp

// Interfaces to the cChunkSender class representing the thread that waits for chunks becoming ready (loaded / generated) and sends them to clients





#include "Globals.h"
#include "ChunkSender.h"
#include "cWorld.h"
#include "packets/cPacket_MapChunk.h"
#include "packets/cPacket_PreChunk.h"
#include "cBlockEntity.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNotifyChunkSender:

void cNotifyChunkSender::Call(int a_ChunkX, int a_ChunkZ)
{
	m_ChunkSender->ChunkReady(a_ChunkX, ZERO_CHUNK_Y, a_ChunkZ);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cChunkSender:

cChunkSender::cChunkSender(void) :
	super("ChunkSender"),
	m_World(NULL),
	m_Notify(NULL)
{
	m_Notify.SetChunkSender(this);
}





cChunkSender::~cChunkSender()
{
	Stop();
}





bool cChunkSender::Start(cWorld * a_World)
{
	m_World = a_World;
	return super::Start();
}





void cChunkSender::Stop(void)
{
	m_ShouldTerminate = true;
	m_evtQueue.Set();
	Wait();
}





void cChunkSender::ChunkReady(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	// This is probably never gonna be called twice for the same chunk, and if it is, we don't mind, so we don't check
	{
		cCSLock Lock(m_CS);
		m_ChunksReady.push_back(cChunkCoords(a_ChunkX, a_ChunkY, a_ChunkZ));
	}
	m_evtQueue.Set();
}





void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client)
{
	ASSERT(a_Client != NULL);
	{
		cCSLock Lock(m_CS);
		if (std::find(m_SendChunks.begin(), m_SendChunks.end(), sSendChunk(a_ChunkX, a_ChunkY, a_ChunkZ, a_Client)) != m_SendChunks.end())
		{
			// Already queued, bail out
			return;
		}
		m_SendChunks.push_back(sSendChunk(a_ChunkX, a_ChunkY, a_ChunkZ, a_Client));
	}
	m_evtQueue.Set();
}





void cChunkSender::RemoveClient(cClientHandle * a_Client)
{
	{
		cCSLock Lock(m_CS);
		for (sSendChunkList::iterator itr = m_SendChunks.begin(); itr != m_SendChunks.end();)
		{
			if (itr->m_Client == a_Client)
			{
				itr = m_SendChunks.erase(itr);
				continue;
			}
			++itr;
		}  // for itr - m_SendChunks[]
		m_RemoveCount++;
	}
	m_evtQueue.Set();
	m_evtRemoved.Wait();  // Wait for removal confirmation
}





void cChunkSender::Execute(void)
{
	while (!m_ShouldTerminate)
	{
		cCSLock Lock(m_CS);
		while (m_ChunksReady.empty() && m_SendChunks.empty())
		{
			int RemoveCount = m_RemoveCount;
			m_RemoveCount = 0;
			cCSUnlock Unlock(Lock);
			for (int i = 0; i < RemoveCount; i++)
			{
				m_evtRemoved.Set();  // Notify that the removed clients are safe to be deleted
			}
			m_evtQueue.Wait();
			if (m_ShouldTerminate)
			{
				return;
			}
		}  // while (empty)
		
		if (!m_ChunksReady.empty())
		{
			// Take one from the queue:
			cChunkCoords Coords(m_ChunksReady.front());
			m_ChunksReady.pop_front();
			Lock.Unlock();
			
			SendChunk(Coords.m_ChunkX, Coords.m_ChunkY, Coords.m_ChunkZ, NULL);
		}
		else
		{
			// Take one from the queue:
			sSendChunk Chunk(m_SendChunks.front());
			m_SendChunks.pop_front();
			Lock.Unlock();
			
			SendChunk(Chunk.m_ChunkX, Chunk.m_ChunkY, Chunk.m_ChunkZ, Chunk.m_Client);
		}
		Lock.Lock();
		int RemoveCount = m_RemoveCount;
		m_RemoveCount = 0;
		Lock.Unlock();
		for (int i = 0; i < RemoveCount; i++)
		{
			m_evtRemoved.Set();  // Notify that the removed clients are safe to be deleted
		}
	}  // while (!mShouldTerminate)
}





void cChunkSender::SendChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, cClientHandle * a_Client)
{
	ASSERT(m_World != NULL);
	
	// Ask the client if it still wants the chunk:
	if (a_Client != NULL)
	{
		if (!a_Client->WantsSendChunk(a_ChunkX, a_ChunkY, a_ChunkZ))
		{
			return;
		}
	}
	
	// If the chunk has no clients, no need to packetize it:
	if (!m_World->HasChunkAnyClients(a_ChunkX, a_ChunkY, a_ChunkZ))
	{
		return;
	}
	
	// If the chunk is not valid, do nothing - whoever needs it has queued it for loading / generating
	if (!m_World->IsChunkValid(a_ChunkX, a_ChunkY, a_ChunkZ))
	{
		return;
	}
	
	// If the chunk is not lighted, queue it for relighting and get notified when it's ready:
	if (!m_World->IsChunkLighted(a_ChunkX, a_ChunkZ))
	{
		m_World->QueueLightChunk(a_ChunkX, a_ChunkZ, &m_Notify);
		return;
	}
	
	// Prepare MapChunk packets:
	if( !m_World->GetChunkData(a_ChunkX, a_ChunkY, a_ChunkZ, *this) )
	{
		return;
	}
	cPacket_PreChunk PreChunk(a_ChunkX, a_ChunkZ, true);
	cPacket_MapChunk MapChunk(a_ChunkX, a_ChunkY, a_ChunkZ, m_BlockData, m_BiomeMap);
	
	// Send:
	if (a_Client == NULL)
	{
		m_World->BroadcastToChunk(a_ChunkX, a_ChunkY, a_ChunkZ, PreChunk);
		m_World->BroadcastToChunk(a_ChunkX, a_ChunkY, a_ChunkZ, MapChunk);
	}
	else
	{
		a_Client->Send(PreChunk);
		a_Client->Send(MapChunk);
	}
	
	// Send entity creation packets:
	for (PacketList::iterator itr = m_Packets.begin(); itr != m_Packets.end(); ++itr)
	{
		if (a_Client == NULL)
		{
			m_World->BroadcastToChunk(a_ChunkX, a_ChunkY, a_ChunkZ, **itr);
		}
		else
		{
			a_Client->Send(**itr);
		}
		delete *itr;
	}  // for itr - m_Packets[]
	m_Packets.clear();
}





void cChunkSender::BlockEntity(cBlockEntity * a_Entity)
{
	cPacket * Packet = a_Entity->GetPacket();
	if (Packet != NULL)
	{
		m_Packets.push_back(Packet);
	}
}




void cChunkSender::Entity(cEntity * a_Entity)
{
	// Nothing needed yet, perhaps in the future when we save entities into chunks we'd like to send them upon load, too ;)
}





void cChunkSender::BiomeData(const cChunkDef::BiomeMap * a_BiomeMap)
{
	for (int i = 0; i < ARRAYCOUNT(m_BiomeMap); i++)
	{
		if ((*a_BiomeMap)[i] < 255)
		{
			// Normal MC biome, copy as-is:
			m_BiomeMap[i] = (unsigned char)((*a_BiomeMap)[i]);
		}
		else
		{
			// TODO: MCS-specific biome, need to map to some basic MC biome:
			ASSERT(!"Unimplemented MCS-specific biome");
		}
	}  // for i - m_BiomeMap[]
}




