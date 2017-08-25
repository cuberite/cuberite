
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


/** Callback that can be used to notify chunk sender upon another chunkcoord notification */
class cNotifyChunkSender :
	public cChunkCoordCallback
{
	virtual void Call(int a_ChunkX, int a_ChunkZ, bool a_IsSuccess) override
	{
		cChunkSender & ChunkSender = m_ChunkSender;
		m_World.DoWithChunk(
			a_ChunkX, a_ChunkZ,
			[&ChunkSender] (cChunk & a_Chunk) -> bool
			{
				ChunkSender.QueueSendChunkTo(a_Chunk.GetPosX(), a_Chunk.GetPosZ(), cChunkSender::E_CHUNK_PRIORITY_MIDHIGH, a_Chunk.GetAllClients());
				return true;
			}
		);
	}

	cChunkSender & m_ChunkSender;

	cWorld & m_World;

public:
	cNotifyChunkSender(cChunkSender & a_ChunkSender, cWorld & a_World):
		m_ChunkSender(a_ChunkSender),
		m_World(a_World)
	{
	}
};






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





void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, cClientHandle * a_Client)
{
	ASSERT(a_Client != nullptr);
	{
		cChunkCoords Chunk{a_ChunkX, a_ChunkZ};
		cCSLock Lock(m_CS);
		auto iter = m_ChunkInfo.find(Chunk);
		if (iter != m_ChunkInfo.end())
		{
			auto & info = iter->second;
			if (info.m_Priority > a_Priority)
			{
				m_SendChunks.push(sChunkQueue{a_Priority, Chunk});
				info.m_Priority = a_Priority;
			}
			info.m_Clients.insert(a_Client);
		}
		else
		{
			m_SendChunks.push(sChunkQueue{a_Priority, Chunk});
			auto info = sSendChunk{Chunk, a_Priority};
			info.m_Clients.insert(a_Client);
			m_ChunkInfo.emplace(Chunk, info);
		}
	}
	m_evtQueue.Set();
}






void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, eChunkPriority a_Priority, std::list<cClientHandle *> a_Clients)
{
	{
		cChunkCoords Chunk{a_ChunkX, a_ChunkZ};
		cCSLock Lock(m_CS);
		auto iter = m_ChunkInfo.find(Chunk);
		if (iter != m_ChunkInfo.end())
		{
			auto & info = iter->second;
			if (info.m_Priority > a_Priority)
			{
				m_SendChunks.push(sChunkQueue{a_Priority, Chunk});
				info.m_Priority = a_Priority;
			}
			info.m_Clients.insert(a_Clients.begin(), a_Clients.end());
		}
		else
		{
			m_SendChunks.push(sChunkQueue{a_Priority, Chunk});
			auto info = sSendChunk{Chunk, a_Priority};
			info.m_Clients.insert(a_Clients.begin(), a_Clients.end());
			m_ChunkInfo.emplace(Chunk, info);
		}
	}
	m_evtQueue.Set();
}





void cChunkSender::RemoveClient(cClientHandle * a_Client)
{
	{
		cCSLock Lock(m_CS);
		for (auto && pair : m_ChunkInfo)
		{
			auto && clients = pair.second.m_Clients;
			clients.erase(a_Client);  // nop for sets that do not contain a_Client
		}
	}
	m_evtQueue.Set();
	m_evtRemoved.Wait();  // Wait for all remaining instances of a_Client to be processed (Execute() makes a copy of m_ChunkInfo)
}





void cChunkSender::Execute(void)
{
	while (!m_ShouldTerminate)
	{
		m_evtQueue.Wait();

		{
			cCSLock Lock(m_CS);
			while (!m_SendChunks.empty())
			{
				// Take one from the queue:
				auto Chunk = m_SendChunks.top().m_Chunk;
				m_SendChunks.pop();
				auto itr = m_ChunkInfo.find(Chunk);
				if (itr == m_ChunkInfo.end())
				{
					continue;
				}

				std::unordered_set<cClientHandle *> clients;
				std::swap(itr->second.m_Clients, clients);
				m_ChunkInfo.erase(itr);

				cCSUnlock Unlock(Lock);
				SendChunk(Chunk.m_ChunkX, Chunk.m_ChunkZ, clients);
			}
		}

		m_evtRemoved.SetAll();  // Notify all waiting threads that all clients are processed and thus safe to destroy
	}  // while (!m_ShouldTerminate)
}





void cChunkSender::SendChunk(int a_ChunkX, int a_ChunkZ, std::unordered_set<cClientHandle *> a_Clients)
{
	// Ask the client if it still wants the chunk:
	for (auto itr = a_Clients.begin(); itr != a_Clients.end();)
	{
		if (!(*itr)->WantsSendChunk(a_ChunkX, a_ChunkZ))
		{
			itr = a_Clients.erase(itr);
		}
		else
		{
			itr++;
		}
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
	cChunkDataSerializer Data(m_Data, m_BiomeMap, m_World.GetDimension());

	for (const auto client : a_Clients)
	{
		// Send:
		client->SendChunkData(a_ChunkX, a_ChunkZ, Data);

		// Send block-entity packets:
		for (const auto & Pos : m_BlockEntities)
		{
			m_World.SendBlockEntity(Pos.x, Pos.y, Pos.z, *client);
		}  // for itr - m_Packets[]

	}
	m_Data.Clear();
	m_BlockEntities.clear();

	// TODO: Send entity spawn packets
}





void cChunkSender::BlockEntity(cBlockEntity * a_Entity)
{
	m_BlockEntities.push_back(a_Entity->GetPos());
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




