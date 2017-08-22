
#pragma once

class cWorld;



/** Generates and lights the spawn area of the world. Runs as a separate thread. */
class cSpawnPrepare:
	public std::enable_shared_from_this<cSpawnPrepare>
{
	/** Private tag allows public constructors that can only be used with private access. */
	struct sMakeSharedTag {};
public:

	cSpawnPrepare(cWorld & a_World, int a_SpawnChunkX, int a_SpawnChunkZ, int a_PrepareDistance, int a_FirstIdx, sMakeSharedTag);

	static void PrepareChunks(cWorld & a_World, int a_SpawnChunkX, int a_SpawnChunkZ, int a_PrepareDistance);

protected:
	cWorld & m_World;
	int m_SpawnChunkX;
	int m_SpawnChunkZ;
	int m_PrepareDistance;

	/** The index of the next chunk to be queued in the lighting thread. */
	int m_NextIdx;

	/** The maximum index of the prepared chunks. Queueing stops when m_NextIdx reaches this number. */
	int m_MaxIdx;

	/** Total number of chunks already finished preparing. Preparation finishes when this number reaches m_MaxIdx. */
	std::atomic<int> m_NumPrepared;

	/** Event used to signal that the preparation is finished. */
	cEvent m_EvtFinished;

	/** The timestamp of the last progress report emitted. */
	std::chrono::steady_clock::time_point m_LastReportTime;

	/** Number of chunks prepared when the last progress report was emitted. */
	int m_LastReportChunkCount;

	void PreparedChunkCallback(int a_ChunkX, int a_ChunkZ);

	/** Decodes the index into chunk coords. Provides the specific chunk ordering. */
	void DecodeChunkCoords(int a_Idx, int & a_ChunkX, int & a_ChunkZ);

	friend class cSpawnPrepareCallback;

};

