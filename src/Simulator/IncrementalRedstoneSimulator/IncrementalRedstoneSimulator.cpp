

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





std::unique_ptr<cRedstoneHandler> cIncrementalRedstoneSimulator::CreateComponent(cWorld & a_World, BLOCKTYPE a_BlockType, cIncrementalRedstoneSimulatorChunkData * a_Data)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ACTIVATOR_RAIL:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_POWERED_RAIL: return cpp14::make_unique<cPoweredRailHandler>(a_World);

		case E_BLOCK_ACTIVE_COMPARATOR:
		case E_BLOCK_INACTIVE_COMPARATOR: return cpp14::make_unique<cRedstoneComparatorHandler>(a_World);

		case E_BLOCK_DISPENSER:
		case E_BLOCK_DROPPER: return cpp14::make_unique<cDropSpenserHandler>(a_World);

		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_WOODEN_PRESSURE_PLATE: return cpp14::make_unique<cPressurePlateHandler>(a_World);

		case E_BLOCK_FENCE_GATE:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_TRAPDOOR: return cpp14::make_unique<cSmallGateHandler>(a_World);

		case E_BLOCK_REDSTONE_LAMP_OFF:
		case E_BLOCK_REDSTONE_LAMP_ON: return cpp14::make_unique<cRedstoneLampHandler>(a_World);

		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON: return cpp14::make_unique<cRedstoneRepeaterHandler>(a_World);

		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON: return cpp14::make_unique<cRedstoneTorchHandler>(a_World);

		case E_BLOCK_PISTON:
		case E_BLOCK_STICKY_PISTON: return cpp14::make_unique<cPistonHandler>(a_World);

		case E_BLOCK_LEVER:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_WOODEN_BUTTON: return cpp14::make_unique<cRedstoneToggleHandler>(a_World);

		case E_BLOCK_BLOCK_OF_REDSTONE: return cpp14::make_unique<cRedstoneBlockHandler>(a_World);
		case E_BLOCK_COMMAND_BLOCK: return cpp14::make_unique<cCommandBlockHandler>(a_World);
		case E_BLOCK_NOTE_BLOCK: return cpp14::make_unique<cNoteBlockHandler>(a_World);
		case E_BLOCK_REDSTONE_WIRE: return cpp14::make_unique<cRedstoneWireHandler>(a_World);
		case E_BLOCK_TNT: return cpp14::make_unique<cTNTHandler>(a_World);
		case E_BLOCK_TRAPPED_CHEST: return cpp14::make_unique<cTrappedChestHandler>(a_World);
		case E_BLOCK_TRIPWIRE_HOOK: return cpp14::make_unique<cTripwireHookHandler>(a_World);
		default:
		{
			if (cBlockDoorHandler::IsDoorBlockType(a_BlockType))
			{
				return cpp14::make_unique<cDoorHandler>(a_World);
			}

			if (cBlockInfo::FullyOccupiesVoxel(a_BlockType))
			{
				return cpp14::make_unique<cSolidBlockHandler>(a_World);
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

		auto CurrentHandler = cIncrementalRedstoneSimulator::CreateComponent(m_World, CurrentBlock, &m_Data);
		if (CurrentHandler == nullptr)
		{
			continue;
		}

		cRedstoneHandler::PoweringData Power;
		for (const auto & Location : CurrentHandler->GetValidSourcePositions(CurrentLocation, CurrentBlock, CurrentMeta))
		{
			BLOCKTYPE PotentialBlock;
			NIBBLETYPE PotentialMeta;
			if ((Location.y < 0) || (Location.y > cChunkDef::Height))
			{
				continue;
			}
			m_World.GetBlockTypeMeta(Location.x, Location.y, Location.z, PotentialBlock, PotentialMeta);

			auto PotentialSourceHandler = cIncrementalRedstoneSimulator::CreateComponent(m_World, PotentialBlock, &m_Data);
			if (PotentialSourceHandler == nullptr)
			{
				continue;
			}

			decltype(Power) PotentialPower(PotentialBlock, PotentialSourceHandler->GetPowerDeliveredToPosition(Location, PotentialBlock, PotentialMeta, CurrentLocation, CurrentBlock));
			Power = std::max(Power, PotentialPower);
		}

		// Inform the handler to update
		cVector3iArray Updates = CurrentHandler->Update(CurrentLocation, CurrentBlock, CurrentMeta, Power);
		WorkQueue.insert(WorkQueue.end(), Updates.begin(), Updates.end());

		if (IsAlwaysTicked(CurrentBlock))
		{
			m_Data.GetActiveBlocks().emplace_back(CurrentLocation);
		}

		#ifdef _DEBUG
			for (const auto & UpdateLocation : Updates)
			{
				LOGD("Queueing block for reupdate (%i %i %i)", UpdateLocation.x, UpdateLocation.y, UpdateLocation.z);
			}
		#endif
	}
}
