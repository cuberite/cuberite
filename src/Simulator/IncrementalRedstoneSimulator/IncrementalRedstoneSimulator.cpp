

#include "Globals.h"

#include "IncrementalRedstoneSimulator.h"
#include "Chunk.h"

#include "CommandBlockHandler.h"
#include "DoorHandler.h"
#include "RedstoneHandler.h"
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
#include "TNTHandler.h"
#include "PoweredRailHandler.h"
#include "PressurePlateHandler.h"
#include "TripwireHookHandler.h"
#include "DropSpenserHandler.h"
#include "RedstoneComparatorHandler.h"
#include "TrappedChestHandler.h"




const cRedstoneHandler * cIncrementalRedstoneSimulator::GetComponentHandler(BLOCKTYPE a_BlockType)
{
	struct sComponents:
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
		case E_BLOCK_POWERED_RAIL: return cpp14::make_unique<cPoweredRailHandler>();

		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_INACTIVE_COMPARATOR: return cpp14::make_unique<cRedstoneComparatorHandler>();

		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER: return cpp14::make_unique<cDropSpenserHandler>();

		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_WOODEN_PRESSURE_PLATE: return cpp14::make_unique<cPressurePlateHandler>();

		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_FENCE_GATE:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_TRAPDOOR: return cpp14::make_unique<cSmallGateHandler>();

		case E_BLOCK_REDSTONE_LAMP_OFF:
		case E_BLOCK_REDSTONE_LAMP_ON: return cpp14::make_unique<cRedstoneLampHandler>();

		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON: return cpp14::make_unique<cRedstoneRepeaterHandler>();

		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON: return cpp14::make_unique<cRedstoneTorchHandler>();

		case E_BLOCK_PISTON:
		case E_BLOCK_STICKY_PISTON: return cpp14::make_unique<cPistonHandler>();

		case E_BLOCK_LEVER:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_WOODEN_BUTTON: return cpp14::make_unique<cRedstoneToggleHandler>();

		case E_BLOCK_BLOCK_OF_REDSTONE: return cpp14::make_unique<cRedstoneBlockHandler>();
		case E_BLOCK_COMMAND_BLOCK: return cpp14::make_unique<cCommandBlockHandler>();
		case E_BLOCK_NOTE_BLOCK: return cpp14::make_unique<cNoteBlockHandler>();
		case E_BLOCK_REDSTONE_WIRE: return cpp14::make_unique<cRedstoneWireHandler>();
		case E_BLOCK_TNT: return cpp14::make_unique<cTNTHandler>();
		case E_BLOCK_TRAPPED_CHEST: return cpp14::make_unique<cTrappedChestHandler>();
		case E_BLOCK_TRIPWIRE_HOOK: return cpp14::make_unique<cTripwireHookHandler>();
		default:
		{
			if (cBlockDoorHandler::IsDoorBlockType(a_BlockType))
			{
				return cpp14::make_unique<cDoorHandler>();
			}

			if (cBlockInfo::FullyOccupiesVoxel(a_BlockType))
			{
				return cpp14::make_unique<cSolidBlockHandler>();
			}
			return nullptr;
		}
	}
}





void cIncrementalRedstoneSimulator::Simulate(float a_dt)
{
	for (auto & DelayInfo : m_Data.m_MechanismDelays)
	{
		if ((--DelayInfo.second.first) == 0)
		{
			m_Data.GetActiveBlocks().emplace_back(DelayInfo.first);
		}
	}

	// Build our work queue
	cVector3iArray WorkQueue;
	std::swap(WorkQueue, m_Data.GetActiveBlocks());

	// Process the work queue
	while (!WorkQueue.empty())
	{
		// Grab the first element and remove it from the list
		Vector3i CurrentLocation = WorkQueue.back();
		WorkQueue.pop_back();

		BLOCKTYPE CurrentBlock;
		NIBBLETYPE CurrentMeta;
		if (!m_World.GetBlockTypeMeta(CurrentLocation.x, CurrentLocation.y, CurrentLocation.z, CurrentBlock, CurrentMeta))
		{
			continue;
		}

		auto CurrentHandler = GetComponentHandler(CurrentBlock);
		if (CurrentHandler == nullptr)  // Block at CurrentPosition doesn't have a corresponding redstone handler
		{
			// Clean up cached PowerData for CurrentPosition
			static_cast<cIncrementalRedstoneSimulator *>(m_World.GetRedstoneSimulator())->GetChunkData()->ErasePowerData(CurrentLocation);

			continue;
		}

		cRedstoneHandler::PoweringData Power;
		for (const auto & Location : CurrentHandler->GetValidSourcePositions(m_World, CurrentLocation, CurrentBlock, CurrentMeta))
		{
			if (!cChunk::IsValidHeight(Location.y))
			{
				continue;
			}
			BLOCKTYPE PotentialBlock;
			NIBBLETYPE PotentialMeta;
			m_World.GetBlockTypeMeta(Location.x, Location.y, Location.z, PotentialBlock, PotentialMeta);

			auto PotentialSourceHandler = GetComponentHandler(PotentialBlock);
			if (PotentialSourceHandler == nullptr)
			{
				continue;
			}

			decltype(Power) PotentialPower(PotentialBlock, PotentialSourceHandler->GetPowerDeliveredToPosition(m_World, Location, PotentialBlock, PotentialMeta, CurrentLocation, CurrentBlock));
			Power = std::max(Power, PotentialPower);
		}

		// Inform the handler to update
		cVector3iArray Updates = CurrentHandler->Update(m_World, CurrentLocation, CurrentBlock, CurrentMeta, Power);
		WorkQueue.insert(WorkQueue.end(), Updates.begin(), Updates.end());

		if (IsAlwaysTicked(CurrentBlock))
		{
			m_Data.GetActiveBlocks().emplace_back(CurrentLocation);
		}
	}
}
