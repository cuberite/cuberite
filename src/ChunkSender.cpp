
// ChunkSender.cpp

// Interfaces to the cChunkSender class representing the thread that waits for chunks becoming ready (loaded / generated) and sends them to clients





#include "Globals.h"
#include "ChunkSender.h"
#include "World.h"
#include "BlockEntities/BlockEntity.h"
#include "Protocol/ChunkDataSerializer.h"
#include "ClientHandle.h"
#include "Chunk.h"
#include "Entities/Player.h"





////////////////////////////////////////////////////////////////////////////////
// cChunkSender:

cChunkSender::cChunkSender(cWorld & a_World) :
	super("ChunkSender"),
	m_World(a_World)
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





void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, const std::weak_ptr<cClientHandle> & a_Client)
{
	ASSERT(!a_Client.expired());
	{
		cChunkCoords Chunk{ a_ChunkX, a_ChunkZ };
		cCSLock Lock(m_CS);

		auto Iterator = std::find_if(m_LoadQueue.begin(), m_LoadQueue.end(), [Chunk](const decltype(m_LoadQueue)::value_type & a_Entry) { return (a_Entry->m_Chunk == Chunk); });
		if (Iterator == m_LoadQueue.end())
		{
			auto ChunkStay = new cChunkQueue(a_Priority, Chunk, a_Client, *this);
			m_LoadQueue.emplace_back(ChunkStay);
			m_World.QueueTask([ChunkStay, this](cWorld & a_World) { ChunkStay->Enable(*m_World.GetChunkMap()); });
		}
		else
		{
			(*Iterator)->m_Priority = std::min(a_Priority, (*Iterator)->m_Priority);
			(*Iterator)->m_Clients.emplace_back(a_Client);
		}
	}
}





void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, const std::vector<std::weak_ptr<cClientHandle>> & a_Clients)
{
	for (const auto Client : a_Clients)
	{
		QueueSendChunkTo(a_ChunkX, a_ChunkZ, a_Priority, Client);
	}
}





void cChunkSender::Execute(void)
{
	while (!m_ShouldTerminate)
	{
		m_evtQueue.Wait();

		decltype(m_SendChunks) QueuedChunks;
		{
			cCSLock Lock(m_CS);
			std::swap(m_SendChunks, QueuedChunks);
		}

		std::sort(QueuedChunks.begin(), QueuedChunks.end(), [](const decltype(QueuedChunks)::value_type & a_Lhs, const decltype(QueuedChunks)::value_type & a_Rhs)
			{
				/* The Standard Priority Queue sorts from biggest to smallest
				return true here means you are smaller than the other object, and you get pushed down.

				The priorities go from HIGH (0) to LOW (3), so a smaller priority should mean further up the list
				therefore, return true (affirm we're "smaller", and get pushed down) only if our priority is bigger than theirs (they're more urgent)
				*/
				return a_Lhs->m_Priority < a_Rhs->m_Priority;
			}
		);

		for (const auto & Entry : QueuedChunks)
		{
			SendChunk(*Entry);
			m_World.QueueTask([Entry](cWorld & a_World) { Entry->Disable(); });
		}
	}  // while (!m_ShouldTerminate)
}





void cChunkSender::SendChunk(const cChunkQueue & a_Item)
{
	cChunkDataSerializer Data(a_Item.m_BlockTypes, a_Item.m_BlockMetas, a_Item.m_BlockLight, a_Item.m_BlockSkyLight, a_Item.m_BiomeMap, m_World.GetDimension());
	for (const auto Client : a_Item.m_Clients)
	{
		// Atomically acquired shared_ptr; thread safe
		auto ClientPointer = Client.lock();
		if (ClientPointer == nullptr)
		{
			continue;
		}

		// Send:
		ClientPointer->SendChunkData(m_World, a_Item.m_Chunk, Data);
	}
}





void cChunkSender::cChunkQueue::BiomeData(const cChunkDef::BiomeMap * a_BiomeMap)
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





bool cChunkSender::cChunkQueue::OnAllChunksAvailable()
{
	VERIFY(m_ChunkSender.m_World.GetChunkData(m_Chunk.m_ChunkX, m_Chunk.m_ChunkZ, *this));

	{
		cCSLock Lock(m_ChunkSender.m_CS);
		m_ChunkSender.m_LoadQueue.erase(std::remove(m_ChunkSender.m_LoadQueue.begin(), m_ChunkSender.m_LoadQueue.end(), this), m_ChunkSender.m_LoadQueue.end());
		m_ChunkSender.m_SendChunks.push_back(this);
	}

	m_ChunkSender.m_evtQueue.Set();
	return false;
}
