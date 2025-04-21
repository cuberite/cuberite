
#include "Globals.h"

#include "IncrementalRedstoneSimulator.h"
#include "BlockType.h"
#include "RedstoneHandler.h"
#include "RedstoneSimulatorChunkData.h"
#include "ForEachSourceCallback.h"





inline bool cIncrementalRedstoneSimulator::IsAlwaysTicked(BlockType a_Block)
{
	switch (a_Block)
	{
		case BlockType::DaylightDetector:
		case BlockType::TripwireHook:
		case BlockType::AcaciaPressurePlate:
		case BlockType::BirchPressurePlate:
		case BlockType::CrimsonPressurePlate:
		case BlockType::DarkOakPressurePlate:
		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::JunglePressurePlate:
		case BlockType::OakPressurePlate:
		case BlockType::PolishedBlackstonePressurePlate:
		case BlockType::SprucePressurePlate:
		case BlockType::StonePressurePlate:
		case BlockType::WarpedPressurePlate:
			return true;
		default: return false;
	}
}





inline bool cIncrementalRedstoneSimulator::IsRedstone(BlockType a_Block)
{
	switch (a_Block)
	{
		// All redstone devices, please alpha sort
		case BlockType::AcaciaButton:
		case BlockType::AcaciaDoor:
		case BlockType::AcaciaFenceGate:
		case BlockType::AcaciaPressurePlate:
		case BlockType::AcaciaTrapdoor:

		case BlockType::ActivatorRail:

		case BlockType::BirchButton:
		case BlockType::BirchDoor:
		case BlockType::BirchFenceGate:
		case BlockType::BirchPressurePlate:
		case BlockType::BirchTrapdoor:

		case BlockType::Comparator:
		case BlockType::RedstoneBlock:
		case BlockType::CommandBlock:
		case BlockType::ChainCommandBlock:
		case BlockType::RepeatingCommandBlock:

		case BlockType::DarkOakButton:
		case BlockType::DarkOakDoor:
		case BlockType::DarkOakFenceGate:
		case BlockType::DarkOakPressurePlate:
		case BlockType::DarkOakTrapdoor:

		case BlockType::DaylightDetector:
		case BlockType::DetectorRail:
		case BlockType::Dispenser:
		case BlockType::Dropper:

		case BlockType::OakButton:
		case BlockType::OakDoor:
		case BlockType::OakFenceGate:
		case BlockType::OakPressurePlate:
		case BlockType::OakTrapdoor:

		case BlockType::HeavyWeightedPressurePlate:
		case BlockType::Hopper:
		case BlockType::IronDoor:
		case BlockType::IronTrapdoor:

		case BlockType::JungleButton:
		case BlockType::JungleDoor:
		case BlockType::JungleFenceGate:
		case BlockType::JunglePressurePlate:
		case BlockType::JungleTrapdoor:

		case BlockType::Lever:
		case BlockType::LightWeightedPressurePlate:
		case BlockType::NoteBlock:
		case BlockType::Observer:
		case BlockType::PoweredRail:
		case BlockType::RedstoneLamp:
		case BlockType::Repeater:
		case BlockType::RedstoneTorch:
		case BlockType::RedstoneWire:

		case BlockType::SpruceButton:
		case BlockType::SpruceDoor:
		case BlockType::SpruceFenceGate:
		case BlockType::SprucePressurePlate:
		case BlockType::SpruceTrapdoor:

		case BlockType::StickyPiston:
		case BlockType::StoneButton:
		case BlockType::StonePressurePlate:
		case BlockType::Tnt:
		case BlockType::TrappedChest:
		case BlockType::TripwireHook:
		case BlockType::Piston:
		{
			return true;
		}
		default: return false;
	}
}





void cIncrementalRedstoneSimulator::ProcessWorkItem(cChunk & Chunk, cChunk & TickingSource, const Vector3i Position)
{
	auto CurrentBlock = Chunk.GetBlock(Position);

	ForEachSourceCallback Callback(Chunk, Position, CurrentBlock);
	RedstoneHandler::ForValidSourcePositions(Chunk, Position, CurrentBlock, Callback);

	// Inform the handler to update
	RedstoneHandler::Update(Chunk, TickingSource, Position, CurrentBlock, Callback.Power);
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





void cIncrementalRedstoneSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
{
	// Never update blocks without a handler:
	if (!IsRedstone(a_Block.Type()))
	{
		return;
	}

	auto & ChunkData = *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());

	if (IsAlwaysTicked(a_Block.Type()))
	{
		ChunkData.AlwaysTickedPositions.emplace(a_Position);
	}

	// Temporary: in the absence of block state support calculate our own:
	if (a_Block == BlockType::RedstoneWire)
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





void cIncrementalRedstoneSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
{
	// Having WakeUp called on us directly means someone called SetBlock (or WakeUp)
	// Since the simulator never does this, something external changed. Clear cached data:
	static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData())->ErasePowerData(a_Position);

	// Queue the block, in case the set block was redstone:
	AddBlock(a_Chunk, a_Position, a_Block);
}





void cIncrementalRedstoneSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BlockState a_Block)
{
	// This is an automatic cross-coords wakeup by cSimulatorManager
	// There is no need to erase power data; if a component was destroyed the 3-arg WakeUp will handle it

	AddBlock(a_Chunk, a_Position, a_Block);

	// The only thing to do go one block farther than this cross-coord, in the direction of Offset
	// in order to notify linked-powered positions that there was a change

	for (const auto & Offset : cSimulator::GetLinkedOffsets(a_Offset))
	{
		auto Relative = a_Position - a_Offset + Offset;

		if (!cChunkDef::IsValidHeight(Relative))
		{
			continue;
		}

		const auto Chunk = a_Chunk.GetRelNeighborChunkAdjustCoords(Relative);

		if ((Chunk == nullptr) || !Chunk->IsValid())
		{
			continue;
		}

		AddBlock(*Chunk, Relative, Chunk->GetBlock(Relative));
	}
}
