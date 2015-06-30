
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
		if (find(m_ActiveBlocks.begin(), m_ActiveBlocks.end(), location) == m_ActiveBlocks.end())
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
	if (!m_ChunkData.erase(location))
	{
		return;
	}

	//// not sure if this is the best place, but update surounding blocks when one changes
	cVector3iList adjacents = {
		{ location.x + 1, location.y, location.z },
		{ location.x - 1, location.y, location.z },
		{ location.x, location.y + 1, location.z },
		{ location.x, location.y - 1, location.z },
		{ location.x, location.y, location.z + 1 },
		{ location.x, location.y, location.z - 1 },
	};

	for (Vector3i item : adjacents)
	{
		WakeUp(item);
	}
}

void cRedstoneSimulatorChunkData::SetComponent(ComponentPtr component)
{
	m_ChunkData[component->Location] = component;
}