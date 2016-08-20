
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
	}
protected:

	std::shared_ptr<cSpawnPrepare> m_SpawnPrepare;

	virtual void Call(int a_ChunkX, int a_ChunkZ, bool a_IsSuccess) override
	{
		m_SpawnPrepare->PreparedChunkCallback(a_ChunkX, a_ChunkZ);
	}
};





cSpawnPrepare::cSpawnPrepare(cWorld & a_World, int a_PrepareDistance, std::function<void()> a_PreparationCompletedCallback):
	m_World(a_World),
	m_TotalChunks(a_PrepareDistance * a_PrepareDistance),
	m_NumPrepared(0),
	m_LastReportTime(std::chrono::steady_clock::now()),
	m_LastReportChunkCount(0),
	m_PreparationCompletedCallback(std::move(a_PreparationCompletedCallback))
{
}







void cSpawnPrepare::PrepareChunks(cWorld & a_World, int a_PrepareDistance, std::function<void()> a_PreparationCompletedCallback)
{
	auto PerformanceAnalysisObject = std::make_shared<cSpawnPrepare>(a_World, a_PrepareDistance, a_PreparationCompletedCallback);
	auto HalfPrepareDistance = (a_PrepareDistance - 1) / 2.f;
	auto NegativePrepareDistance = FloorC(-HalfPrepareDistance), PositivePrepareDistance = FloorC(HalfPrepareDistance);

	for (int ChunkX = NegativePrepareDistance; ChunkX <= PositivePrepareDistance; ++ChunkX)
	{
		for (int ChunkZ = NegativePrepareDistance; ChunkZ <= PositivePrepareDistance; ++ChunkZ)
		{
			a_World.PrepareChunk(ChunkX, ChunkZ, cpp14::make_unique<cSpawnPrepareCallback>(PerformanceAnalysisObject));
		}
	}
}





void cSpawnPrepare::PreparedChunkCallback(int a_ChunkX, int a_ChunkZ)
{
	m_NumPrepared += 1;
	if (m_NumPrepared == m_TotalChunks)
	{
		if (m_PreparationCompletedCallback)
		{
			m_PreparationCompletedCallback();
		}

		LOG("Preparing spawn (%s): completed!", m_World.GetName().c_str());
		return;
	}

	// Report progress every 1 second:
	auto Now = std::chrono::steady_clock::now();
	if (Now - m_LastReportTime > std::chrono::seconds(1))
	{
		float PercentDone = static_cast<float>(m_NumPrepared * 100) / m_TotalChunks;
		float ChunkSpeed = static_cast<float>((m_NumPrepared - m_LastReportChunkCount) * 1000) / std::chrono::duration_cast<std::chrono::milliseconds>(Now - m_LastReportTime).count();
		LOG("Preparing spawn (%s): %.02f%% (%d/%d; %.02f chunks / sec)",
			m_World.GetName().c_str(), PercentDone, m_NumPrepared, m_TotalChunks, ChunkSpeed
		);
		m_LastReportTime = Now;
		m_LastReportChunkCount = m_NumPrepared;
	}
}





bool cSpawnPrepare::IsValidSpawnBiome(cWorld & a_World, int a_ChunkX, int a_ChunkZ)
{
	auto Biome = a_World.GetBiomeAt(a_ChunkX, a_ChunkZ);
	if ((Biome != EMCSBiome::biOcean) && (Biome != EMCSBiome::biFrozenOcean) && (Biome != EMCSBiome::biDeepOcean))
	{
		return true;
	}

	return false;
}





Vector3d cSpawnPrepare::GenerateRandomSpawn(cWorld & a_World, int a_PrepareDistance)
{
	LOGD("Generating random spawnpoint...");
	int ChunkX = 0, ChunkZ = 0;

	for (int CurrentRadius = 0; CurrentRadius <= a_PrepareDistance; ++CurrentRadius)
	{
		// Iterate through right and left sides
		for (int PerpendicularRadius = -CurrentRadius; PerpendicularRadius <= CurrentRadius; ++PerpendicularRadius)
		{
			if (cSpawnPrepare::IsValidSpawnBiome(a_World, CurrentRadius, PerpendicularRadius))
			{
				ChunkX = CurrentRadius;
				ChunkZ = PerpendicularRadius;
				goto IUsedAGotoEatThat;
			}

			if (cSpawnPrepare::IsValidSpawnBiome(a_World, -CurrentRadius, PerpendicularRadius))
			{
				ChunkX = -CurrentRadius;
				ChunkZ = PerpendicularRadius;
				goto IUsedAGotoEatThat;
			}
		}

		// Iterate through top and bottom sides, omitting the corners
		for (int PerpendicularRadius = -CurrentRadius + 1; PerpendicularRadius < CurrentRadius; ++PerpendicularRadius)
		{
			if (cSpawnPrepare::IsValidSpawnBiome(a_World, PerpendicularRadius, CurrentRadius))
			{
				ChunkX = PerpendicularRadius;
				ChunkZ = CurrentRadius;
				goto IUsedAGotoEatThat;
			}

			if (cSpawnPrepare::IsValidSpawnBiome(a_World, PerpendicularRadius, -CurrentRadius))
			{
				ChunkX = PerpendicularRadius;
				ChunkZ = -CurrentRadius;
				goto IUsedAGotoEatThat;
			}
		}
	}

IUsedAGotoEatThat:

	for (auto BlockX = ChunkX * cChunkDef::Width; BlockX != (ChunkX + 1) * cChunkDef::Width; ++BlockX)
	{
		for (auto BlockZ = ChunkZ * cChunkDef::Width; BlockZ != (ChunkZ + 1)  * cChunkDef::Width; ++BlockZ)
		{
			Vector3d Position;
			Position.x = static_cast<double>(BlockX);
			Position.z = static_cast<double>(BlockZ);

			if (CanSpawnAt(a_World, Position.x, Position.y, Position.z))
			{
				Position += {0.5, 0.0, 0.5};

				LOGINFO("Generated spawn position at {%.2f, %.2f, %.2f}", Position.x, Position.y, Position.z);
				return Position;
			}
		}
	}

	int Height;
	VERIFY(a_World.TryGetHeight(0, 0, Height));

	LOGWARNING("Did not find an acceptable spawnpoint. Defaulted to spawn at the origin, elevation %i blocks", Height);
	return { 0.0, static_cast<double>(Height), 0.0 };
}





bool cSpawnPrepare::CanSpawnAt(cWorld & a_World, double a_X, double & a_Y, double a_Z)
{
	// All this blocks can only be found above ground.
	// Apart from netherrack (as the Nether is technically a massive cave)
	static const BLOCKTYPE ValidSpawnBlocks[] =
	{
		E_BLOCK_GRASS,
		E_BLOCK_SAND,
		E_BLOCK_SNOW,
		E_BLOCK_SNOW_BLOCK,
		E_BLOCK_NETHERRACK,
		E_BLOCK_END_STONE
	};

	static const int ValidSpawnBlocksCount = ARRAYCOUNT(ValidSpawnBlocks);

	// Increase this by two, because we need two more blocks for body and head
	int HighestSpawnPoint;
	VERIFY(a_World.TryGetHeight(static_cast<int>(a_X), static_cast<int>(a_Z), HighestSpawnPoint));
	HighestSpawnPoint += 2;

	const int LowestSpawnPoint = static_cast<int>(HighestSpawnPoint / 2.0f);

	for (int PotentialY = HighestSpawnPoint; PotentialY > LowestSpawnPoint; --PotentialY)
	{
		BLOCKTYPE HeadBlock = a_World.GetBlock(static_cast<int>(a_X), PotentialY, static_cast<int>(a_Z));

		// Is this block safe for spawning
		if (HeadBlock != E_BLOCK_AIR)
		{
			continue;
		}

		BLOCKTYPE BodyBlock = a_World.GetBlock(static_cast<int>(a_X), PotentialY - 1, static_cast<int>(a_Z));

		// Is this block safe for spawning
		if (BodyBlock != E_BLOCK_AIR)
		{
			continue;
		}

		BLOCKTYPE FloorBlock = a_World.GetBlock(static_cast<int>(a_X), PotentialY - 2, static_cast<int>(a_Z));

		// Early out - Is the floor block air
		if (FloorBlock == E_BLOCK_AIR)
		{
			continue;
		}

		// Is the floor block ok
		bool ValidSpawnBlock = false;
		for (int BlockIndex = 0; BlockIndex < ValidSpawnBlocksCount; ++BlockIndex)
		{
			ValidSpawnBlock |= (ValidSpawnBlocks[BlockIndex] == FloorBlock);
		}

		if (!ValidSpawnBlock)
		{
			continue;
		}

		if (!CheckPlayerSpawnPoint(a_World, static_cast<int>(a_X), PotentialY - 1, static_cast<int>(a_Z)))
		{
			continue;
		}

		a_Y = PotentialY - 1.0;
		return true;
	}

	return false;
}





bool cSpawnPrepare::CheckPlayerSpawnPoint(cWorld & a_World, int a_PosX, int a_PosY, int a_PosZ)
{
	// Check height bounds
	if (!cChunkDef::IsValidHeight(a_PosY))
	{
		return false;
	}

	// Check that surrounding blocks are neither solid or liquid
	static const Vector3i SurroundingCoords[] =
	{
		Vector3i(0,  0,  1),
		Vector3i(1,  0,  1),
		Vector3i(1,  0,  0),
		Vector3i(1,  0, -1),
		Vector3i(0,  0, -1),
		Vector3i(-1, 0, -1),
		Vector3i(-1, 0,  0),
		Vector3i(-1, 0,  1),
	};

	static const int SurroundingCoordsCount = ARRAYCOUNT(SurroundingCoords);

	for (int CoordIndex = 0; CoordIndex < SurroundingCoordsCount; ++CoordIndex)
	{
		const int XPos = a_PosX + SurroundingCoords[CoordIndex].x;
		const int ZPos = a_PosZ + SurroundingCoords[CoordIndex].z;

		const BLOCKTYPE BlockType = a_World.GetBlock(XPos, a_PosY, ZPos);
		if (cBlockInfo::IsSolid(BlockType) || IsBlockLiquid(BlockType))
		{
			return false;
		}
	}

	return true;
}
