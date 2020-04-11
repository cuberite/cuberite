
#pragma once

#include "RedstoneHandler.h"
#include "../RedstoneSimulator.h"
#include <unordered_map>





class cIncrementalRedstoneSimulatorChunkData : public cRedstoneSimulatorChunkData
{

public:
	void WakeUp(const Vector3i & a_Position)
	{
		m_ActiveBlocks.push_back(a_Position);
	}

	cVector3iArray & GetActiveBlocks()
	{
		return m_ActiveBlocks;
	}

	cRedstoneHandler::ENUM_PRESSURE_PLATE_STATE GetCachedPressurePlateState(const Vector3i & a_Position)
	{
		auto Result = m_CachedPressurePlateStates.find(a_Position);
		return (Result == m_CachedPressurePlateStates.end()) ? cRedstoneHandler::E_PRESSURE_PLATE_RAISED : Result->second;
	}

	void SetCachedPressurePlateState(const Vector3i & a_Position, cRedstoneHandler::ENUM_PRESSURE_PLATE_STATE a_PressurePlateState)
	{
		m_CachedPressurePlateStates[a_Position] = a_PressurePlateState;
	}

	const cRedstoneHandler::PoweringData GetCachedPowerData(const Vector3i & a_Position) const
	{
		auto Result = m_CachedPowerLevels.find(a_Position);
		return (Result == m_CachedPowerLevels.end()) ? cRedstoneHandler::PoweringData() : Result->second;
	}

	void SetCachedPowerData(const Vector3i & a_Position, cRedstoneHandler::PoweringData a_PoweringData)
	{
		m_CachedPowerLevels[a_Position] = a_PoweringData;
	}

	std::pair<int, bool> * GetMechanismDelayInfo(const Vector3i & a_Position)
	{
		auto Result = m_MechanismDelays.find(a_Position);
		return (Result == m_MechanismDelays.end()) ? nullptr : &Result->second;
	}

	/** Erase cached PowerData for position */
	void ErasePowerData(const Vector3i & a_Position)
	{
		m_CachedPowerLevels.erase(a_Position);
	}

	cRedstoneHandler::PoweringData ExchangeUpdateOncePowerData(const Vector3i & a_Position, cRedstoneHandler::PoweringData a_PoweringData)
	{
		auto Result = m_CachedPowerLevels.find(a_Position);
		if (Result == m_CachedPowerLevels.end())
		{
			m_CachedPowerLevels[a_Position] = a_PoweringData;
			return cRedstoneHandler::PoweringData();
		}
		std::swap(Result->second, a_PoweringData);
		return a_PoweringData;
	}

	/** Structure storing position of mechanism + it's delay ticks (countdown) & if to power on */
	std::unordered_map<Vector3i, std::pair<int, bool>, VectorHasher<int>> m_MechanismDelays;

private:

	cVector3iArray m_ActiveBlocks;

	// TODO: map<Vector3i, int> -> Position of torch + it's heat level

	std::unordered_map<Vector3i, cRedstoneHandler::ENUM_PRESSURE_PLATE_STATE, VectorHasher<int>> m_CachedPressurePlateStates;
	std::unordered_map<Vector3i, cRedstoneHandler::PoweringData, VectorHasher<int>> m_CachedPowerLevels;

	friend class cRedstoneHandlerFactory;

};
