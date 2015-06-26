
#include "Globals.h"
#include "cRedstoneSimulatorChunkData.h"


void cRedstoneSimulatorChunkData::WakeUp(Vector3i location)
{
	if (m_ActiveBlocks.empty())
	{
		m_ActiveBlocks.push_back(location);
	}
	else
	{
		auto result = find(m_ActiveBlocks.begin(), m_ActiveBlocks.end(), location);
		if (result != m_ActiveBlocks.end())
		{
			m_ActiveBlocks.push_back(location);
		}
	}
}

ComponentPtr cRedstoneSimulatorChunkData::GetComponent(Vector3i location)
{
	if (m_ChunkData.find(location) == m_ChunkData.end())
	{
		return nullptr;
	}
	return m_ChunkData[location];
}

void cRedstoneSimulatorChunkData::RemoveComponent(Vector3i location)
{
	m_ChunkData.erase(location);
}

void cRedstoneSimulatorChunkData::SetComponent(ComponentPtr component)
{
	m_ChunkData[component->Location] = component;
}