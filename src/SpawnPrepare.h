
#pragma once

#include <atomic>

class cWorld;



/** Generates and lights the spawn area of the world. Runs as a separate thread. */
class cSpawnPrepare
{

public:
	cSpawnPrepare(cWorld & a_World, int a_PrepareDistance, std::function<void()> a_PreparationCompletedCallback);

	static void PrepareChunks(cWorld & a_World, int a_PrepareDistance, std::function<void()> a_PreparationCompletedCallback = {});

	/** Generates a random spawnpoint on solid land by walking chunks and finding their biomes */
	static Vector3d GenerateRandomSpawn(cWorld & a_World, int a_PrepareDistance);

protected:
	cWorld & m_World;
	const int m_TotalChunks;

	/** Total number of chunks already finished preparing. Preparation finishes when this number reaches m_MaxIdx. */
	int m_NumPrepared;

	/** The timestamp of the last progress report emitted. */
	std::chrono::steady_clock::time_point m_LastReportTime;

	/** Number of chunks prepared when the last progress report was emitted. */
	int m_LastReportChunkCount;

	std::function<void()> m_PreparationCompletedCallback;

	void PreparedChunkCallback(int a_ChunkX, int a_ChunkZ);

	/** Returns if the biome of the given chunk coordinates is a valid spawn candidate. */
	static bool IsValidSpawnBiome(cWorld & a_World, int a_ChunkX, int a_ChunkZ);

	/** Can the specified coordinates be used as a spawn point?
	Returns true if spawn position is valid and sets a_Y to the valid spawn height */
	static bool CanSpawnAt(cWorld & a_World, double a_X, double & a_Y, double a_Z);

	/** Check if player starting point is acceptable */
	static bool CheckPlayerSpawnPoint(cWorld & a_World, int a_PosX, int a_PosY, int a_PosZ);

	friend class cSpawnPrepareCallback;

};

