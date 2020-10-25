
#include "Globals.h"

#include "RedstoneHandler.h"
#include "RedstoneDataHelper.h"
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





#define INVOKE_FOR_HANDLERS(Callback)                                                       \
	do                                                                                      \
	{                                                                                       \
		switch (BlockType)                                                                  \
		{                                                                                   \
			case E_BLOCK_ACTIVATOR_RAIL:                                                    \
			case E_BLOCK_DETECTOR_RAIL:                                                     \
			case E_BLOCK_POWERED_RAIL:          return PoweredRailHandler::Callback;        \
			case E_BLOCK_ACTIVE_COMPARATOR:                                                 \
			case E_BLOCK_INACTIVE_COMPARATOR:   return RedstoneComparatorHandler::Callback; \
			case E_BLOCK_DISPENSER:                                                         \
			case E_BLOCK_DROPPER:               return DropSpenserHandler::Callback;        \
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:                                     \
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:                                     \
			case E_BLOCK_STONE_PRESSURE_PLATE:                                              \
			case E_BLOCK_WOODEN_PRESSURE_PLATE: return PressurePlateHandler::Callback;      \
			case E_BLOCK_ACACIA_FENCE_GATE:                                                 \
			case E_BLOCK_BIRCH_FENCE_GATE:                                                  \
			case E_BLOCK_DARK_OAK_FENCE_GATE:                                               \
			case E_BLOCK_FENCE_GATE:                                                        \
			case E_BLOCK_IRON_TRAPDOOR:                                                     \
			case E_BLOCK_JUNGLE_FENCE_GATE:                                                 \
			case E_BLOCK_SPRUCE_FENCE_GATE:                                                 \
			case E_BLOCK_TRAPDOOR:              return SmallGateHandler::Callback;          \
			case E_BLOCK_REDSTONE_LAMP_OFF:                                                 \
			case E_BLOCK_REDSTONE_LAMP_ON:      return RedstoneLampHandler::Callback;       \
			case E_BLOCK_REDSTONE_REPEATER_OFF:                                             \
			case E_BLOCK_REDSTONE_REPEATER_ON:  return RedstoneRepeaterHandler::Callback;   \
			case E_BLOCK_REDSTONE_TORCH_OFF:                                                \
			case E_BLOCK_REDSTONE_TORCH_ON:     return RedstoneTorchHandler::Callback;      \
			case E_BLOCK_OBSERVER:              return ObserverHandler::Callback;           \
			case E_BLOCK_PISTON:                                                            \
			case E_BLOCK_STICKY_PISTON:         return PistonHandler::Callback;             \
			case E_BLOCK_LEVER:                                                             \
			case E_BLOCK_STONE_BUTTON:                                                      \
			case E_BLOCK_WOODEN_BUTTON:         return RedstoneToggleHandler::Callback;     \
			case E_BLOCK_BLOCK_OF_REDSTONE:     return RedstoneBlockHandler::Callback;      \
			case E_BLOCK_COMMAND_BLOCK:         return CommandBlockHandler::Callback;       \
			case E_BLOCK_HOPPER:                return HopperHandler::Callback;             \
			case E_BLOCK_NOTE_BLOCK:            return NoteBlockHandler::Callback;          \
			case E_BLOCK_REDSTONE_WIRE:         return RedstoneWireHandler::Callback;       \
			case E_BLOCK_TNT:                   return TNTHandler::Callback;                \
			case E_BLOCK_TRAPPED_CHEST:         return TrappedChestHandler::Callback;       \
			case E_BLOCK_TRIPWIRE_HOOK:         return TripwireHookHandler::Callback;       \
			default:                                                                        \
			{                                                                               \
				if (cBlockDoorHandler::IsDoorBlockType(BlockType))                          \
				{                                                                           \
					return DoorHandler::Callback;                                           \
				}                                                                           \
			}                                                                               \
		}                                                                                   \
	} while (false)





namespace RedstoneHandler
{
	PowerLevel GetPowerDeliveredToPosition(const cChunk & Chunk, const Vector3i Position, const BLOCKTYPE BlockType, const Vector3i QueryPosition, const BLOCKTYPE QueryBlockType, const bool IsLinked)
	{
		INVOKE_FOR_HANDLERS(GetPowerDeliveredToPosition(Chunk, Position, BlockType, QueryPosition, QueryBlockType, IsLinked));

		// Fell through the switch statement
		// Block at Position doesn't have a corresponding redstone handler
		// ErasePowerData will have been called in AddBlock

		// Default:
		return 0;
	}

	void Update(cChunk & Chunk, cChunk & CurrentlyTicking, const Vector3i Position, const BLOCKTYPE BlockType, const NIBBLETYPE Meta, const PowerLevel PowerLevel)
	{
		INVOKE_FOR_HANDLERS(Update(Chunk, CurrentlyTicking, Position, BlockType, Meta, PowerLevel));
	}

	void ForValidSourcePositions(const cChunk & Chunk, const Vector3i Position, const BLOCKTYPE BlockType, const NIBBLETYPE Meta, ForEachSourceCallback & Callback)
	{
		INVOKE_FOR_HANDLERS(ForValidSourcePositions(Chunk, Position, BlockType, Meta, Callback));
	}

	void SetWireState(const cChunk & Chunk, const Vector3i Position)
	{
		RedstoneWireHandler::SetWireState(Chunk, Position);
	}
}
