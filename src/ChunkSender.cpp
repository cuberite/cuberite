
// ChunkSender.cpp

// Interfaces to the cChunkSender class representing the thread that waits for chunks becoming ready (loaded / generated) and sends them to clients





#include "Globals.h"
#include "ChunkSender.h"
#include "World.h"
#include "BlockEntities/BlockEntity.h"
#include "Protocol/ChunkDataSerializer.h"
#include "ClientHandle.h"
#include "Chunk.h"




////////////////////////////////////////////////////////////////////////////////
// cNotifyChunkSender:


/// Callback that can be used to notify chunk sender upon another chunkcoord notification
class cNotifyChunkSender :
	public cChunkCoordCallback
{
	virtual void Call(int a_ChunkX, int a_ChunkZ) override
	{
		cChunkSender & ChunkSender = m_ChunkSender;
		m_World.DoWithChunk(
			a_ChunkX, a_ChunkZ, 
			[&ChunkSender] (cChunk & a_Chunk) -> bool
			{
				ChunkSender.QueueSendChunkTo(a_Chunk.GetPosX(), a_Chunk.GetPosZ(), cChunkSender::PRIORITY_BROADCAST, a_Chunk.GetAllClients());
				return true;
			}
		);
	}

	cChunkSender & m_ChunkSender;

	cWorld & m_World;
public:
	cNotifyChunkSender(cChunkSender & a_ChunkSender, cWorld & a_World) : m_ChunkSender(a_ChunkSender), m_World(a_World) {}


};

cChunkSender::cChunkSender(cWorld & a_World) :
	super("ChunkSender"),
	m_World(a_World),
	m_RemoveCount(0)
{
}





cChunkSender::~cChunkSender()
{
	Stop();
}





bool cChunkSender::Start()
{
	m_ShouldTerminate = false;
	return super::Start();
}





void cChunkSender::Stop(void)
{
	m_ShouldTerminate = true;
	m_evtQueue.Set();
	Wait();
}





void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, cClientHandle * a_Client)
{
	ASSERT(a_Client != nullptr);
	{
		sSendChunk Chunk(a_ChunkX, a_ChunkZ, a_Client);

		cCSLock Lock(m_CS);
		if (
			std::find(m_SendChunksLowPriority.begin(), m_SendChunksLowPriority.end(), Chunk) != m_SendChunksLowPriority.end() ||
			std::find(m_SendChunksMediumPriority.begin(), m_SendChunksMediumPriority.end(), Chunk) != m_SendChunksMediumPriority.end() ||
			std::find(m_SendChunksHighPriority.begin(), m_SendChunksHighPriority.end(), Chunk) != m_SendChunksHighPriority.end()
		)
		{
			// Already queued, bail out
			return;
		}

		switch (a_Priority)
		{
			case E_CHUNK_PRIORITY_LOW:
			{
				m_SendChunksLowPriority.push_back(Chunk);
				break;
			}
			case E_CHUNK_PRIORITY_MEDIUM:
			{
				m_SendChunksMediumPriority.push_back(Chunk);
				break;
			}
			case E_CHUNK_PRIORITY_HIGH:
			{
				m_SendChunksHighPriority.push_back(Chunk);
				break;
			}
			case PRIORITY_BROADCAST:
			{
				m_SendChunksBroadcastPriority.push_back(Chunk);
				break;
			}
		}
	}
	m_evtQueue.Set();
}






void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, std::list<cClientHandle *> a_Clients)
{
	{
		cCSLock Lock(m_CS);
		for (auto client : a_Clients)
		{
			sSendChunk Chunk(a_ChunkX, a_ChunkZ, client);
			if (
				std::find(m_SendChunksLowPriority.begin(), m_SendChunksLowPriority.end(), Chunk) != m_SendChunksLowPriority.end() ||
				std::find(m_SendChunksMediumPriority.begin(), m_SendChunksMediumPriority.end(), Chunk) != m_SendChunksMediumPriority.end() ||
				std::find(m_SendChunksHighPriority.begin(), m_SendChunksHighPriority.end(), Chunk) != m_SendChunksHighPriority.end()
			)
			{
				// Already queued, bail out
				continue;
			}

			switch (a_Priority)
			{
				case E_CHUNK_PRIORITY_LOW:
				{
					m_SendChunksLowPriority.push_back(Chunk);
					break;
				}
				case E_CHUNK_PRIORITY_MEDIUM:
				{
					m_SendChunksMediumPriority.push_back(Chunk);
					break;
				}
				case E_CHUNK_PRIORITY_HIGH:
				{
					m_SendChunksHighPriority.push_back(Chunk);
					break;
				}
				case PRIORITY_BROADCAST:
				{
					m_SendChunksBroadcastPriority.push_back(Chunk);
					break;
				}
			}
		}
	}
	m_evtQueue.Set();
}





void cChunkSender::RemoveClient(cClientHandle * a_Client)
{
	{
		cCSLock Lock(m_CS);
		for (sSendChunkList::iterator itr = m_SendChunksLowPriority.begin(); itr != m_SendChunksLowPriority.end();)
		{
			if (itr->m_Client == a_Client)
			{
				itr = m_SendChunksLowPriority.erase(itr);
				continue;
			}
			++itr;
		}  // for itr - m_SendChunksLowPriority[]
		for (sSendChunkList::iterator itr = m_SendChunksMediumPriority.begin(); itr != m_SendChunksMediumPriority.end();)
		{
			if (itr->m_Client == a_Client)
			{
				itr = m_SendChunksMediumPriority.erase(itr);
				continue;
			}
			++itr;
		}  // for itr - m_SendChunksMediumPriority[]
		for (sSendChunkList::iterator itr = m_SendChunksHighPriority.begin(); itr != m_SendChunksHighPriority.end();)
		{
			if (itr->m_Client == a_Client)
			{
				itr = m_SendChunksHighPriority.erase(itr);
				continue;
			}
			++itr;
		}  // for itr - m_SendChunksHighPriority[]
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
		while (m_SendChunksBroadcastPriority.empty() && m_SendChunksLowPriority.empty() && m_SendChunksMediumPriority.empty() && m_SendChunksHighPriority.empty())
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

		if (!m_SendChunksHighPriority.empty())
		{
			// Take one from the queue:
			sSendChunk Chunk(m_SendChunksHighPriority.front());
			m_SendChunksHighPriority.pop_front();
			Lock.Unlock();

			SendChunk(Chunk.m_ChunkX, Chunk.m_ChunkZ, Chunk.m_Client);
		}
		else if (!m_SendChunksBroadcastPriority.empty())
		{
			// Take one from the queue:
			sSendChunk Chunk(m_SendChunksBroadcastPriority.front());
			m_SendChunksBroadcastPriority.pop_front();
			Lock.Unlock();

			SendChunk(Chunk.m_ChunkX, Chunk.m_ChunkZ, Chunk.m_Client);
		}
		else if (!m_SendChunksMediumPriority.empty())
		{
			// Take one from the queue:
			sSendChunk Chunk(m_SendChunksMediumPriority.front());
			m_SendChunksMediumPriority.pop_front();
			Lock.Unlock();

			SendChunk(Chunk.m_ChunkX, Chunk.m_ChunkZ, Chunk.m_Client);
		}
		else
		{
			// Take one from the queue:
			sSendChunk Chunk(m_SendChunksLowPriority.front());
			m_SendChunksLowPriority.pop_front();
			Lock.Unlock();

			SendChunk(Chunk.m_ChunkX, Chunk.m_ChunkZ, Chunk.m_Client);
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





void cChunkSender::SendChunk(int a_ChunkX, int a_ChunkZ, cClientHandle * a_Client)
{
	
	// Ask the client if it still wants the chunk:
	if (!a_Client->WantsSendChunk(a_ChunkX, a_ChunkZ))
	{
		return;
	}

	// If the chunk has no clients, no need to packetize it:
	if (!m_World.HasChunkAnyClients(a_ChunkX, a_ChunkZ))
	{
		return;
	}

	// If the chunk is not valid, do nothing - whoever needs it has queued it for loading / generating
	if (!m_World.IsChunkValid(a_ChunkX, a_ChunkZ))
	{
		return;
	}

	// If the chunk is not lighted, queue it for relighting and get notified when it's ready:
	if (!m_World.IsChunkLighted(a_ChunkX, a_ChunkZ))
	{
		m_World.QueueLightChunk(a_ChunkX, a_ChunkZ, cpp14::make_unique<cNotifyChunkSender>(*this, m_World));
		return;
	}

	// Query and prepare chunk data:
	if (!m_World.GetChunkData(a_ChunkX, a_ChunkZ, *this))
	{
		return;
	}
	cChunkDataSerializer Data(m_BlockTypes, m_BlockMetas, m_BlockLight, m_BlockSkyLight, m_BiomeMap);

	// Send:
	a_Client->SendChunkData(a_ChunkX, a_ChunkZ, Data);

	// Send block-entity packets:
	for (sBlockCoords::iterator itr = m_BlockEntities.begin(); itr != m_BlockEntities.end(); ++itr)
	{
		m_World.SendBlockEntity(itr->m_BlockX, itr->m_BlockY, itr->m_BlockZ, *a_Client);
	}  // for itr - m_Packets[]
	m_BlockEntities.clear();

	// TODO: Send entity spawn packets
}





void cChunkSender::BlockEntity(cBlockEntity * a_Entity)
{
	m_BlockEntities.push_back(sBlockCoord(a_Entity->GetPosX(), a_Entity->GetPosY(), a_Entity->GetPosZ()));
}




void cChunkSender::Entity(cEntity *)
{
	// Nothing needed yet, perhaps in the future when we save entities into chunks we'd like to send them upon load, too ;)
}





void cChunkSender::BiomeData(const cChunkDef::BiomeMap * a_BiomeMap)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_BiomeMap); i++)
	{
		if ((*a_BiomeMap)[i] < 255)
		{
			// Normal MC biome, copy as-is:
			m_BiomeMap[i] = static_cast<unsigned char>((*a_BiomeMap)[i]);
		}
		else
		{
			// TODO: MCS-specific biome, need to map to some basic MC biome:
			ASSERT(!"Unimplemented MCS-specific biome");
		}
	}  // for i - m_BiomeMap[]
}




