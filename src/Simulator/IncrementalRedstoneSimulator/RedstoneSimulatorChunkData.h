
#pragma once

#include <stack>

#include "../RedstoneSimulator.h"
#include "../../Chunk.h"





using PowerLevel = unsigned char;





class cIncrementalRedstoneSimulatorChunkData final : public cRedstoneSimulatorChunkData
{
public:

	void WakeUp(const Vector3i & a_Position)
	{
		m_ActiveBlocks.push(a_Position);
	}

	auto & GetActiveBlocks()
	{
		return m_ActiveBlocks;
	}

	PowerLevel GetCachedPowerData(const Vector3i Position) const
	{
		auto Result = m_CachedPowerLevels.find(Position);
		return (Result == m_CachedPowerLevels.end()) ? 0 : Result->second;
	}

	void SetCachedPowerData(const Vector3i Position, const PowerLevel PowerLevel)
	{
		m_CachedPowerLevels[Position] = PowerLevel;
	}

	std::pair<int, bool> * GetMechanismDelayInfo(const Vector3i Position)
	{
		auto Result = m_MechanismDelays.find(Position);
		return (Result == m_MechanismDelays.end()) ? nullptr : &Result->second;
	}

	/** Erase all cached redstone data for position. */
	void ErasePowerData(const Vector3i Position)
	{
		m_CachedPowerLevels.erase(Position);
		m_MechanismDelays.erase(Position);
		AlwaysTickedPositions.erase(Position);
		WireStates.erase(Position);
		ObserverCache.erase(Position);
	}

	PowerLevel ExchangeUpdateOncePowerData(const Vector3i & a_Position, PowerLevel Power)
	{
		auto Result = m_CachedPowerLevels.find(a_Position);

		if (Result == m_CachedPowerLevels.end())
		{
			m_CachedPowerLevels[a_Position] = Power;
			return 0;
		}

		return std::exchange(Result->second, Power);
	}

	/** Adjust From-relative coordinates into To-relative coordinates. */
	inline static Vector3i RebaseRelativePosition(const cChunk & From, const cChunk & To, const Vector3i Position)
	{
		return
		{
			Position.x + (From.GetPosX() - To.GetPosX()) * cChunkDef::Width,
			Position.y,
			Position.z + (From.GetPosZ() - To.GetPosZ()) * cChunkDef::Width
		};
	}

	/** Temporary, should be chunk data: wire block store, to avoid recomputing states every time. */
	std::unordered_map<Vector3i, short, VectorHasher<int>> WireStates;

	std::unordered_set<Vector3i, VectorHasher<int>> AlwaysTickedPositions;

	/** Structure storing an observer's last seen block. */
	std::unordered_map<Vector3i, std::pair<BLOCKTYPE, NIBBLETYPE>, VectorHasher<int>> ObserverCache;

	/** Structure storing position of mechanism + it's delay ticks (countdown) & if to power on. */
	std::unordered_map<Vector3i, std::pair<int, bool>, VectorHasher<int>> m_MechanismDelays;

private:

	std::stack<Vector3i, std::vector<Vector3i>> m_ActiveBlocks;

	// TODO: map<Vector3i, int> -> Position of torch + it's heat level

	std::unordered_map<Vector3i, PowerLevel, VectorHasher<int>> m_CachedPowerLevels;

	friend class cRedstoneHandlerFactory;
};
