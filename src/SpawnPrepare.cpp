
#include "Globals.h"

#include "SpawnPrepare.h"
#include "World.h"





class cSpawnPrepareCallback :
	public cChunkCoordCallback
{
public:
	cSpawnPrepareCallback(std::shared_ptr<cSpawnPrepare> a_SpawnPrepare) :
		m_SpawnPrepare(std::move(a_SpawnPrepare))
	{
		ASSERT(m_SpawnPrepare != nullptr);
	}
protected:

	std::shared_ptr<cSpawnPrepare> m_SpawnPrepare;

	virtual void Call(cChunkCoords a_Coords, bool a_IsSuccess) override
	{
		m_SpawnPrepare->PreparedChunkCallback(a_Coords.m_ChunkX, a_Coords.m_ChunkZ);
	}
};





cSpawnPrepare::cSpawnPrepare(cWorld & a_World, int a_SpawnChunkX, int a_SpawnChunkZ, int a_PrepareDistance, int a_FirstIdx, sMakeSharedTag):
	m_World(a_World),
	m_SpawnChunkX(a_SpawnChunkX),
	m_SpawnChunkZ(a_SpawnChunkZ),
	m_PrepareDistance(a_PrepareDistance),
	m_NextIdx(a_FirstIdx),
	m_MaxIdx(a_PrepareDistance * a_PrepareDistance),
	m_NumPrepared(0),
	m_LastReportTime(std::chrono::steady_clock::now()),
	m_LastReportChunkCount(0)
{
}





void cSpawnPrepare::PrepareChunks(cWorld & a_World, int a_SpawnChunkX, int a_SpawnChunkZ, int a_PrepareDistance)
{

	// Queue the initial chunks:
	int MaxIdx = a_PrepareDistance * a_PrepareDistance;
	int maxQueue = std::min(MaxIdx - 1, 100);  // Number of chunks to queue at once
	auto prep = std::make_shared<cSpawnPrepare>(a_World, a_SpawnChunkX, a_SpawnChunkZ, a_PrepareDistance, maxQueue, sMakeSharedTag{});
	for (int i = 0; i < maxQueue; i++)
	{
		int chunkX, chunkZ;
		prep->DecodeChunkCoords(i, chunkX, chunkZ);
		a_World.PrepareChunk(chunkX, chunkZ, std::make_unique<cSpawnPrepareCallback>(prep));
	}  // for i

	// Wait for the lighting thread to prepare everything. Event is set in the Call() callback:
	if (MaxIdx > 0)
	{
		prep->m_EvtFinished.Wait();
	}
}





void cSpawnPrepare::DecodeChunkCoords(int a_Idx, int & a_ChunkX, int & a_ChunkZ)
{
	// A zigzag pattern from the top to bottom, each row alternating between forward-x and backward-x:
	int z = a_Idx / m_PrepareDistance;
	int x = a_Idx % m_PrepareDistance;
	if ((z & 1) == 0)
	{
		// Reverse every second row:
		x = m_PrepareDistance - 1 - x;
	}
	a_ChunkZ = m_SpawnChunkZ + z - m_PrepareDistance / 2;
	a_ChunkX = m_SpawnChunkX + x - m_PrepareDistance / 2;
}





void cSpawnPrepare::PreparedChunkCallback(int a_ChunkX, int a_ChunkZ)
{
	// Check if this was the last chunk:
	m_NumPrepared += 1;
	if (m_NumPrepared >= m_MaxIdx)
	{
		m_EvtFinished.Set();
		// Must return here, because "this" may have gotten deleted by the previous line
		return;
	}

	// Queue another chunk, if appropriate:
	if (m_NextIdx < m_MaxIdx)
	{
		int chunkX, chunkZ;
		DecodeChunkCoords(m_NextIdx, chunkX, chunkZ);
		m_World.GetLightingThread().QueueChunk(chunkX, chunkZ, std::make_unique<cSpawnPrepareCallback>(shared_from_this()));
		m_NextIdx += 1;
	}

	// Report progress every 1 second:
	auto Now = std::chrono::steady_clock::now();
	if (Now - m_LastReportTime > std::chrono::seconds(1))
	{
		float PercentDone = static_cast<float>(m_NumPrepared * 100) / m_MaxIdx;
		float ChunkSpeed = static_cast<float>((m_NumPrepared - m_LastReportChunkCount) * 1000) / std::chrono::duration_cast<std::chrono::milliseconds>(Now - m_LastReportTime).count();
		LOG("Preparing spawn (%s): %.02f%% (%d/%d; %.02f chunks / sec)",
			m_World.GetName().c_str(), PercentDone, m_NumPrepared.load(std::memory_order_seq_cst), m_MaxIdx, ChunkSpeed
		);
		m_LastReportTime = Now;
		m_LastReportChunkCount = m_NumPrepared;
	}
}

