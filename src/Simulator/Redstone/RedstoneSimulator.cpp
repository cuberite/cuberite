

#include "Globals.h"

#include "RedstoneSimulator.h"
#include "ComponentFactory.h"
#include "Chunk.h"
#include <algorithm>

void cRedstoneSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	return;
}


void cRedstoneSimulator::Simulate(float a_dt)
{
	// redstone ticks are every other world tick
	m_ticks++;

	if (m_ticks % 2 == 0)
	{
		return;
	}

	// factory used for this simulation tick
	ComponentFactory factory(m_World, &data);

	// build our work queue
	std::deque<Vector3i> work;

	cVector3iArray & blocks = data.GetActiveBlocks();
	for (Vector3i & block : blocks)
	{
		work.push_back(block);
	}

	blocks.clear();

	// process the work queue
	std::vector<ComponentPtr> updated;
	while (!work.empty())
	{
		// grab the first element and remove it from the list
		Vector3i location = work.back();
		work.pop_back();
		ComponentPtr component(factory.GetComponent(location));
		if (component == nullptr)
		{
			continue;
		}

		// the actual component processes
		cVector3iArray updates(component->Update(factory, m_ticks));

		// queue blocks for update
		for (Vector3i item : updates)
		{
			if (item == location)
			{
				// if the update returned itself, queue for update next tick
				if (std::find(updated.begin(), updated.end(), component) == updated.end())
				{
					updated.push_back(component);
				}
			}
			else if (std::find(work.begin(), work.end(), item) == work.end())
			{
				LOGD("Queueing block for update (%d %d %d)", item.x, item.y, item.z);
				work.push_back(item);
			}
		}
	}

	for (ComponentPtr item : updated)
	{
		data.WakeUp(item->Location);
		BLOCKTYPE blockTypeUpdated;
		NIBBLETYPE metaDataUpdated;
		if (item->GetState(blockTypeUpdated, metaDataUpdated))
		{
			m_World.SetBlock(item->Location.x, item->Location.y, item->Location.z, blockTypeUpdated, metaDataUpdated, true);
		}
	}

}

void cRedstoneSimulator::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if (!a_Chunk->IsValid())
	{
		return;
	}

	Vector3i relative = cChunkDef::AbsoluteToRelative({ a_BlockX, a_BlockY, a_BlockZ }, a_Chunk->GetPosX(), a_Chunk->GetPosZ());

	if (relative.y < 0)
	{
		return;
	}

	if (ComponentFactory::IsSolidBlock(a_Chunk->GetBlock(relative)))
	{
		return;
	}

	//auto data = a_Chunk->GetRedstoneSimulatorData();
	//if (data == nullptr)
	//{
	//	data = new cRedstoneSimulatorChunkData();
	//	a_Chunk->SetRedstoneSimulatorData(data);
	//}

	//data->WakeUp(relative);

	data.WakeUp({ a_BlockX, a_BlockY, a_BlockZ });

}