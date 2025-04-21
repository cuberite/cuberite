
// ChunkSender.cpp

// Interfaces to the cChunkSender class representing the thread that waits for chunks becoming ready (loaded / generated) and sends them to clients





#include "Globals.h"
#include "ChunkSender.h"
#include "World.h"
#include "BlockEntities/BlockEntity.h"
#include "ClientHandle.h"
#include "Chunk.h"





////////////////////////////////////////////////////////////////////////////////
// cNotifyChunkSender:


/** Callback that can be used to notify chunk sender upon another chunkcoord notification */
class cNotifyChunkSender :
	public cChunkCoordCallback
{
	virtual void Call(cChunkCoords a_Coords, bool a_IsSuccess) override
	{
		cChunkSender & ChunkSender = m_ChunkSender;
		m_World.DoWithChunk(
			a_Coords.m_ChunkX, a_Coords.m_ChunkZ,
			[&ChunkSender] (cChunk & a_Chunk) -> bool
			{
				ChunkSender.QueueSendChunkTo(a_Chunk.GetPosX(), a_Chunk.GetPosZ(), cChunkSender::Priority::High, a_Chunk.GetAllClients());
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
	Super("Chunk Sender"),
	m_World(a_World),
	m_Serializer(m_World.GetDimension())
{
}





cChunkSender::~cChunkSender()
{
	Stop();
}





void cChunkSender::Stop(void)
{
	m_ShouldTerminate = true;
	m_evtQueue.Set();
	Super::Stop();
}





void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, Priority a_Priority, cClientHandle * a_Client)
{
	ASSERT(a_Client != nullptr);
	{
		cChunkCoords Chunk{a_ChunkX, a_ChunkZ};
		cCSLock Lock(m_CS);
		auto iter = m_ChunkInfo.find(Chunk);
		if (iter != m_ChunkInfo.end())
		{
			auto & info = iter->second;
			if (info.m_Priority < a_Priority)  // Was the chunk's priority boosted?
			{
				m_SendChunks.push(sChunkQueue{a_Priority, Chunk});
				info.m_Priority = a_Priority;
			}
			info.m_Clients.insert(a_Client->shared_from_this());
		}
		else
		{
			m_SendChunks.push(sChunkQueue{a_Priority, Chunk});
			auto info = sSendChunk{Chunk, a_Priority};
			info.m_Clients.insert(a_Client->shared_from_this());
			m_ChunkInfo.emplace(Chunk, info);
		}
	}
	m_evtQueue.Set();
}





void cChunkSender::QueueSendChunkTo(int a_ChunkX, int a_ChunkZ, Priority a_Priority, const std::vector<cClientHandle *> & a_Clients)
{
	{
		cChunkCoords Chunk{a_ChunkX, a_ChunkZ};
		cCSLock Lock(m_CS);
		auto iter = m_ChunkInfo.find(Chunk);
		if (iter != m_ChunkInfo.end())
		{
			auto & info = iter->second;
			if (info.m_Priority < a_Priority)  // Was the chunk's priority boosted?
			{
				m_SendChunks.push(sChunkQueue{a_Priority, Chunk});
				info.m_Priority = a_Priority;
			}
			for (const auto & Client : a_Clients)
			{
				info.m_Clients.insert(Client->shared_from_this());
			}
		}
		else
		{
			m_SendChunks.push(sChunkQueue{a_Priority, Chunk});
			auto info = sSendChunk{Chunk, a_Priority};
			for (const auto & Client : a_Clients)
			{
				info.m_Clients.insert(Client->shared_from_this());
			}
			m_ChunkInfo.emplace(Chunk, info);
		}
	}
	m_evtQueue.Set();
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

				auto clients = std::move(itr->second.m_Clients);
				m_ChunkInfo.erase(itr);

				cCSUnlock Unlock(Lock);
				SendChunk(Chunk.m_ChunkX, Chunk.m_ChunkZ, clients);
			}
		}
	}  // while (!m_ShouldTerminate)
}





void cChunkSender::SendChunk(int a_ChunkX, int a_ChunkZ, const WeakClients & a_Clients)
{
	// Contains strong pointers to clienthandles.
	std::vector<std::shared_ptr<cClientHandle>> Clients;

	// Ask the client if it still wants the chunk:
	for (const auto & WeakClient : a_Clients)
	{
		auto Client = WeakClient.lock();
		if ((Client != nullptr) && Client->WantsSendChunk(a_ChunkX, a_ChunkZ))
		{
			Clients.push_back(std::move(Client));
		}
	}

	// Bail early if every requester disconnected:
	if (Clients.empty())
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
		m_World.QueueLightChunk(a_ChunkX, a_ChunkZ, std::make_unique<cNotifyChunkSender>(*this, m_World));
		return;
	}

	// Query and prepare chunk data:
	if (!m_World.GetChunkData({a_ChunkX, a_ChunkZ}, *this))
	{
		return;
	}

	// Send:
	m_Serializer.SendToClients(a_ChunkX, a_ChunkZ, m_BlockData, m_LightData, m_BiomeMap, m_BlockEntities, m_HeightMap, Clients);

	for (const auto & Client : Clients)
	{
		/*
		// Send block-entity packets:
		for (const auto & Pos : m_BlockEntities)
		{
			m_World.SendBlockEntity(Pos.x, Pos.y, Pos.z, *Client);
		}  // for itr - m_Packets[]
		*/
		// Send entity packets:
		for (const auto EntityID : m_EntityIDs)
		{
			m_World.DoWithEntityByID(EntityID, [Client](cEntity & a_Entity)
			{
				/*
				// DEBUG:
				LOGD("cChunkSender: Entity #%d (%s) at [%f, %f, %f] spawning for player \"%s\"",
					a_Entity.GetUniqueID(), a_Entity.GetClass(),
					a_Entity.GetPosition().x, a_Entity.GetPosition().y, a_Entity.GetPosition().z,
					Client->GetUsername().c_str()
				);
				*/

				/* This check looks highly suspect.
				Its purpose is to check the client still has a valid player object associated,
				since the player destroys itself when the client is destroyed.
				It's done within the world lock to ensure correctness.
				A better way involves fixing chunk sending (GH #3696) to obviate calling SpawnOn from this thread in the first place. */
				if (!Client->IsDestroyed())
				{
					a_Entity.SpawnOn(*Client);
				}

				return true;
			});
		}
	}

	m_BlockEntities.clear();
	m_EntityIDs.clear();
}





void cChunkSender::BlockEntity(cBlockEntity * a_Entity)
{
	m_BlockEntities.push_back(a_Entity);
}





void cChunkSender::HeightMap(const cChunkDef::HeightMap & a_HeightMap)
{
	m_HeightMap = a_HeightMap;
}





void cChunkSender::Entity(cEntity * a_Entity)
{
	m_EntityIDs.push_back(a_Entity->GetUniqueID());
}





void cChunkSender::BiomeMap(const cChunkDef::BiomeMap & a_BiomeMap)
{
	for (size_t i = 0; i < ARRAYCOUNT(m_BiomeMap); i++)
	{
		if (a_BiomeMap[i] < 255)
		{
			// Normal MC biome, copy as-is:
			m_BiomeMap[i] = static_cast<unsigned char>(a_BiomeMap[i]);
		}
		else
		{
			// TODO: MCS-specific biome, need to map to some basic MC biome:
			ASSERT(!"Unimplemented MCS-specific biome");
		}
	}  // for i - m_BiomeMap[]
}
