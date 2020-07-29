

#include "Globals.h"

#include "IncrementalRedstoneSimulator.h"
#include "../../Chunk.h"

#include "CommandBlockHandler.h"
#include "DoorHandler.h"
#include "RedstoneTorchHandler.h"
#include "RedstoneWireHandler.h"
#include "RedstoneRepeaterHandler.h"
#include "RedstoneToggleHandler.h"
#include "SolidBlockHandler.h"
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

			if (cBlockInfo::FullyOccupiesVoxel(a_BlockType))
			{
				return std::make_unique<cSolidBlockHandler>();
			}
			return nullptr;
		}
	}
}





void cIncrementalRedstoneSimulator::WakeUp(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	Super::WakeUp(a_Chunk, a_Position, a_Block);

	auto & ChunkData = *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());

	// Never update blocks without a handler:
	if (GetComponentHandler(a_Block) == nullptr)
	{
		ChunkData.ErasePowerData(a_Position);
		return;
	}

	// Only update others if there is a redstone device nearby
	for (int x = -1; x < 2; ++x)
	{
		for (int y = -1; y < 2; ++y)
		{
			if (!cChunkDef::IsValidHeight(a_Position.y + y))
			{
				continue;
			}

			for (int z = -1; z < 2; ++z)
			{
				auto CheckPos = a_Position + Vector3i{ x, y, z };
				BLOCKTYPE Block;
				NIBBLETYPE Meta;

				// If we can't read the block, assume it is a mechanism
				if (
					!a_Chunk.UnboundedRelGetBlock(CheckPos, Block, Meta) ||
					IsRedstone(Block)
				)
				{
					ChunkData.WakeUp(a_Position);
					return;
				}
			}
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

	PoweringData Power;
	CurrentHandler->ForValidSourcePositions(Chunk, Position, CurrentBlock, CurrentMeta, [&Chunk, Position, CurrentBlock, &Power](Vector3i Location)
	{
		if (!cChunk::IsValidHeight(Location.y))
		{
			return;
		}

		const auto NeighbourChunk = Chunk.GetRelNeighborChunkAdjustCoords(Location);
		if ((NeighbourChunk == nullptr) || !NeighbourChunk->IsValid())
		{
			return;
		}

		BLOCKTYPE PotentialBlock;
		NIBBLETYPE PotentialMeta;
		NeighbourChunk->GetBlockTypeMeta(Location, PotentialBlock, PotentialMeta);

		auto PotentialSourceHandler = GetComponentHandler(PotentialBlock);
		if (PotentialSourceHandler == nullptr)
		{
			return;
		}

		const PoweringData PotentialPower(
			PotentialBlock,
			PotentialSourceHandler->GetPowerDeliveredToPosition(
				*NeighbourChunk, Location, PotentialBlock, PotentialMeta,
				cIncrementalRedstoneSimulatorChunkData::RebaseRelativePosition(Chunk, *NeighbourChunk, Position), CurrentBlock
			)
		);
		Power = std::max(Power, PotentialPower);
	});

	// Inform the handler to update
	CurrentHandler->Update(Chunk, TickingSource, Position, CurrentBlock, CurrentMeta, Power);
}





void cIncrementalRedstoneSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	auto & ChunkData = *static_cast<cIncrementalRedstoneSimulatorChunkData *>(a_Chunk.GetRedstoneSimulatorData());

	if (!IsRedstone(a_Block))
	{
		return;
	}

	if (IsAlwaysTicked(a_Block))
	{
		ChunkData.AlwaysTickedPositions.emplace(a_Position);
	}

	// Always update redstone devices:
	ChunkData.WakeUp(a_Position);
}
