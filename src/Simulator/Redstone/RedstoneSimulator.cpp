

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
	while (!work.empty())
	{
		// grab the first element and remove it from the list
		Vector3i location = work.front();
		ComponentPtr component(factory.GetComponent(location));
		if (component == nullptr)
		{
			work.pop_front();
			continue;
		}

		// the actual component processes
		BLOCKTYPE blockTypeUpdated;
		NIBBLETYPE metaDataUpdated;
		cVector3iArray updates(component->Update(factory, blockTypeUpdated, metaDataUpdated));

		// queue blocks for update
		for (Vector3i item : updates)
		{
			if (item == location)
			{
				// if the update returned itself, queue for update next tick
				data.WakeUp(location);
				m_World.FastSetBlock(location, blockTypeUpdated, metaDataUpdated);
			}
			else if (std::find(work.begin(), work.end(), item) == work.end())
			{
				work.push_back(item);
			}
		}

		work.pop_front();
	}

}

void cRedstoneSimulator::WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	//if (!a_Chunk->IsValid())
	//{
	//	return;
	//}

	//Vector3i relative = cChunkDef::AbsoluteToRelative({ a_BlockX, a_BlockY, a_BlockZ }, a_Chunk->GetPosX(), a_Chunk->GetPosZ());

	//if (relative.y < 0)
	//{
	//	return;
	//}

	//auto data = a_Chunk->GetRedstoneSimulatorData();
	//if (data == nullptr)
	//{
	//	data = new cRedstoneSimulatorChunkData();
	//	a_Chunk->SetRedstoneSimulatorData(data);
	//}

	//data->WakeUp(relative);

	data.WakeUp({ a_BlockX, a_BlockY, a_BlockZ });

}