
#include "Globals.h"

#include "IncrementalRedstoneSimulator.h"
#include "BlockType.h"
#include "RedstoneHandler.h"
#include "RedstoneSimulatorChunkData.h"
#include "ForEachSourceCallback.h"





bool cIncrementalRedstoneSimulator::IsAlwaysTicked(BLOCKTYPE a_Block)
{
	switch (a_Block)  // Call the appropriate simulator for the entry's block type
	{
		case E_BLOCK_DAYLIGHT_SENSOR:
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE: return true;
		default: return false;
	}
}





bool cIncrementalRedstoneSimulator::IsRedstone(BLOCKTYPE a_Block)

{
	switch (a_Block)
	{
		// All redstone devices, please alpha sort
		case E_BLOCK_ACACIA_DOOR:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_BIRCH_DOOR:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_BLOCK_OF_REDSTONE:
		case E_BLOCK_COMMAND_BLOCK:
		case E_BLOCK_DARK_OAK_DOOR:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_DAYLIGHT_SENSOR:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER:
		case E_BLOCK_FENCE_GATE:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_HOPPER:
		case E_BLOCK_INACTIVE_COMPARATOR:
		case E_BLOCK_INVERTED_DAYLIGHT_SENSOR:
		case E_BLOCK_IRON_DOOR:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_JUNGLE_DOOR:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_LEVER:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_NOTE_BLOCK:
		case E_BLOCK_OBSERVER:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_REDSTONE_LAMP_OFF:
		case E_BLOCK_REDSTONE_LAMP_ON:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_SPRUCE_DOOR:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_STICKY_PISTON:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_TNT:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_TRAPPED_CHEST:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_WOODEN_BUTTON:
		case E_BLOCK_WOODEN_DOOR:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_PISTON:
		{
			return true;
		}
		default: return false;
	}
}





void cIncrementalRedstoneSimulator::ProcessWorkItem(cChunk & Chunk, cChunk & TickingSource, const Vector3i Position)
{
	BLOCKTYPE CurrentBlock;
	NIBBLETYPE CurrentMeta;
	Chunk.GetBlockTypeMeta(Position, CurrentBlock, CurrentMeta);

	ForEachSourceCallback Callback(Chunk, Position, CurrentBlock);
	RedstoneHandler::ForValidSourcePositions(Chunk, Position, CurrentBlock, CurrentMeta, Callback);

	// Inform the handler to update
	RedstoneHandler::Update(Chunk, TickingSource, Position, CurrentBlock, CurrentMeta, Callback.Power);
}





void cIncrementalRedstoneSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	auto & ChunkData = *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk->GetRedstoneSimulatorData());
	for (auto & DelayInfo : ChunkData.m_MechanismDelays)
	{
		if ((--DelayInfo.second.first) == 0)
		{
			ChunkData.WakeUp(DelayInfo.first);
		}
	}

	// Build our work queue
	auto & WorkQueue = ChunkData.GetActiveBlocks();

	// Process the work queue
	while (!WorkQueue.empty())
	{
		// Grab the first element and remove it from the list
		Vector3i CurrentLocation = WorkQueue.top();
		WorkQueue.pop();

		const auto NeighbourChunk = a_Chunk->GetRelNeighborChunkAdjustCoords(CurrentLocation);
		if ((NeighbourChunk == nullptr) || !NeighbourChunk->IsValid())
		{
			continue;
		}

		ProcessWorkItem(*NeighbourChunk, *a_Chunk, CurrentLocation);
	}

	for (const auto & Position : ChunkData.AlwaysTickedPositions)
	{
		ChunkData.WakeUp(Position);
	}
}





void cIncrementalRedstoneSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	// Never update blocks without a handler:
	if (!IsRedstone(a_Block))
	{
		return;
	}

	auto & ChunkData = *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());

	if (IsAlwaysTicked(a_Block))
	{
		ChunkData.AlwaysTickedPositions.emplace(a_Position);
	}

	// Temporary: in the absence of block state support calculate our own:
	if (a_Block == E_BLOCK_REDSTONE_WIRE)
	{
		RedstoneHandler::SetWireState(a_Chunk, a_Position);
	}

	// Always update redstone devices:
	ChunkData.WakeUp(a_Position);
}





cRedstoneSimulatorChunkData * cIncrementalRedstoneSimulator::CreateChunkData()
{
	return new cIncrementalRedstoneSimulatorChunkData;
}





void cIncrementalRedstoneSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	// Having WakeUp called on us directly means someone called SetBlock (or WakeUp)
	// Since the simulator never does this, something external changed. Clear cached data:
	static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData())->ErasePowerData(a_Position);

	// Queue the block, in case the set block was redstone:
	AddBlock(a_Chunk, a_Position, a_Block);
}





void cIncrementalRedstoneSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BLOCKTYPE a_Block)
{
	// This is an automatic cross-coords wakeup by cSimulatorManager
	// There is no need to erase power data; if a component was destroyed the 3-arg WakeUp will handle it

	AddBlock(a_Chunk, a_Position, a_Block);

	// The only thing to do go one block farther than this cross-coord, in the direction of Offset
	// in order to notify linked-powered positions that there was a change

	for (const auto & Offset : cSimulator::GetLinkedOffsets(a_Offset))
	{
		auto Relative = a_Position - a_Offset + Offset;
		if (!cChunkDef::IsValidHeight(Relative.y))
		{
			continue;
		}

		auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(Relative);
		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			continue;
		}

		const auto Block = Chunk->GetBlock(Relative);
		AddBlock(*Chunk, Relative, Block);
	}
}
