
#pragma once

#include <stack>

#include "../RedstoneSimulator.h"
#include "../../Chunk.h"





struct PoweringData
{
public:
	PoweringData(BLOCKTYPE a_PoweringBlock, unsigned char a_PowerLevel) :
		PoweringBlock(a_PoweringBlock),
		PowerLevel(a_PowerLevel)
	{
	}

	PoweringData(void) :
		PoweringBlock(E_BLOCK_AIR),
		PowerLevel(0)
	{
	}

	BLOCKTYPE PoweringBlock;
	unsigned char PowerLevel;

	inline friend bool operator < (const PoweringData & Lhs, const PoweringData & Rhs)
	{
		return (
			(Lhs.PowerLevel < Rhs.PowerLevel) ||
			(
				(Lhs.PowerLevel == Rhs.PowerLevel) &&
				((Lhs.PoweringBlock == E_BLOCK_REDSTONE_WIRE) && (Rhs.PoweringBlock != E_BLOCK_REDSTONE_WIRE))
			)
		);
	}

	inline friend bool operator == (const PoweringData & Lhs, const PoweringData & Rhs)
	{
		return (Lhs.PowerLevel == Rhs.PowerLevel);
	}

	inline friend bool operator != (const PoweringData & Lhs, const PoweringData & Rhs)
	{
		return !operator ==(Lhs, Rhs);
	}
};





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

	const PoweringData GetCachedPowerData(const Vector3i Position) const
	{
		auto Result = m_CachedPowerLevels.find(Position);
		return (Result == m_CachedPowerLevels.end()) ? PoweringData() : Result->second;
	}

	void SetCachedPowerData(const Vector3i Position, PoweringData PoweringData)
	{
		m_CachedPowerLevels[Position] = PoweringData;
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
	}

	PoweringData ExchangeUpdateOncePowerData(const Vector3i & a_Position, PoweringData a_PoweringData)
	{
		auto Result = m_CachedPowerLevels.find(a_Position);
		if (Result == m_CachedPowerLevels.end())
		{
			m_CachedPowerLevels[a_Position] = a_PoweringData;
			return PoweringData();
		}
		std::swap(Result->second, a_PoweringData);
		return a_PoweringData;
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

	/** Structure storing position of mechanism + it's delay ticks (countdown) & if to power on. */
	std::unordered_map<Vector3i, std::pair<int, bool>, VectorHasher<int>> m_MechanismDelays;

private:

	std::stack<Vector3i, std::vector<Vector3i>> m_ActiveBlocks;

	// TODO: map<Vector3i, int> -> Position of torch + it's heat level

	std::unordered_map<Vector3i, PoweringData, VectorHasher<int>> m_CachedPowerLevels;

	friend class cRedstoneHandlerFactory;

};
