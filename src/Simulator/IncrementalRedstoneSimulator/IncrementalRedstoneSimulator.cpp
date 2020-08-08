
#include "Globals.h"

#include "IncrementalRedstoneSimulator.h"
#include "ForEachSourceCallback.h"

#include "CommandBlockHandler.h"
#include "DoorHandler.h"
#include "RedstoneTorchHandler.h"
#include "RedstoneWireHandler.h"
#include "RedstoneRepeaterHandler.h"
#include "RedstoneToggleHandler.h"
#include "RedstoneLampHandler.h"
#include "RedstoneBlockHandler.h"
#include "PistonHandler.h"
#include "SmallGateHandler.h"
#include "NoteBlockHandler.h"
#include "ObserverHandler.h"
#include "TNTHandler.h"
#include "PoweredRailHandler.h"
#include "PressurePlateHandler.h"
#include "TripwireHookHandler.h"
#include "DropSpenserHandler.h"
#include "RedstoneComparatorHandler.h"
#include "TrappedChestHandler.h"
#include "HopperHandler.h"





const cRedstoneHandler * cIncrementalRedstoneSimulator::GetComponentHandler(BLOCKTYPE a_BlockType)
{
	struct sComponents :
		public std::array<std::unique_ptr<cRedstoneHandler>, 256>
	{
		sComponents()
		{
			for (size_t i = 0; i != 256; ++i)
			{
				(*this)[i] = cIncrementalRedstoneSimulator::CreateComponent(static_cast<BLOCKTYPE>(i));
			}
		}
	};


	static sComponents Components;
	return Components[a_BlockType].get();
}





std::unique_ptr<cRedstoneHandler> cIncrementalRedstoneSimulator::CreateComponent(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_POWERED_RAIL: return std::make_unique<cPoweredRailHandler>();

		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_INACTIVE_COMPARATOR: return std::make_unique<cRedstoneComparatorHandler>();

		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER: return std::make_unique<cDropSpenserHandler>();

		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_WOODEN_PRESSURE_PLATE: return std::make_unique<cPressurePlateHandler>();

		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_FENCE_GATE:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_TRAPDOOR: return std::make_unique<cSmallGateHandler>();

		case E_BLOCK_REDSTONE_LAMP_OFF:
		case E_BLOCK_REDSTONE_LAMP_ON: return std::make_unique<cRedstoneLampHandler>();

		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON: return std::make_unique<cRedstoneRepeaterHandler>();

		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON: return std::make_unique<cRedstoneTorchHandler>();

		case E_BLOCK_OBSERVER: return std::make_unique<cObserverHandler>();

		case E_BLOCK_PISTON:
		case E_BLOCK_STICKY_PISTON: return std::make_unique<cPistonHandler>();

		case E_BLOCK_LEVER:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_WOODEN_BUTTON: return std::make_unique<cRedstoneToggleHandler>();

		case E_BLOCK_BLOCK_OF_REDSTONE: return std::make_unique<cRedstoneBlockHandler>();
		case E_BLOCK_COMMAND_BLOCK: return std::make_unique<cCommandBlockHandler>();
		case E_BLOCK_HOPPER: return std::make_unique<cHopperHandler>();
		case E_BLOCK_NOTE_BLOCK: return std::make_unique<cNoteBlockHandler>();
		case E_BLOCK_REDSTONE_WIRE: return std::make_unique<cRedstoneWireHandler>();
		case E_BLOCK_TNT: return std::make_unique<cTNTHandler>();
		case E_BLOCK_TRAPPED_CHEST: return std::make_unique<cTrappedChestHandler>();
		case E_BLOCK_TRIPWIRE_HOOK: return std::make_unique<cTripwireHookHandler>();
		default:
		{
			if (cBlockDoorHandler::IsDoorBlockType(a_BlockType))
			{
				return std::make_unique<cDoorHandler>();
			}

			return nullptr;
		}
	}
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

	for (const auto Position : ChunkData.AlwaysTickedPositions)
	{
		ChunkData.WakeUp(Position);
	}
}





void cIncrementalRedstoneSimulator::ProcessWorkItem(cChunk & Chunk, cChunk & TickingSource, const Vector3i Position)
{
	BLOCKTYPE CurrentBlock;
	NIBBLETYPE CurrentMeta;
	Chunk.GetBlockTypeMeta(Position, CurrentBlock, CurrentMeta);

	auto CurrentHandler = GetComponentHandler(CurrentBlock);
	if (CurrentHandler == nullptr)
	{
		// Block at Position doesn't have a corresponding redstone handler
		// ErasePowerData will have been called in AddBlock
		return;
	}

	ForEachSourceCallback Callback(Chunk, Position, CurrentBlock);
	CurrentHandler->ForValidSourcePositions(Chunk, Position, CurrentBlock, CurrentMeta, Callback);

	// Inform the handler to update
	CurrentHandler->Update(Chunk, TickingSource, Position, CurrentBlock, CurrentMeta, Callback.Power);
}





void cIncrementalRedstoneSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	if (!IsRedstone(a_Block))
	{
		return;
	}

	// Never update blocks without a handler:
	ASSERT(GetComponentHandler(a_Block) != nullptr);

	auto & ChunkData = *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());

	if (IsAlwaysTicked(a_Block))
	{
		ChunkData.AlwaysTickedPositions.emplace(a_Position);
	}

	// Temporary: in the absence of block state support calculate our own:
	if (a_Block == E_BLOCK_REDSTONE_WIRE)
	{
		static_cast<const cRedstoneWireHandler *>(GetComponentHandler(a_Block))->SetWireState(a_Chunk, a_Position);
	}

	// Always update redstone devices:
	ChunkData.WakeUp(a_Position);
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

	for (const auto Offset : cSimulator::GetLinkedOffsets(a_Offset))
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
