
#include "Globals.h"

// #define RedstoneLogger

// Some wrapper for the LOGD function to enable - disable the redstone logging
#ifdef RedstoneLogger
	#define LOGREDSTONE LOGD
#else
	#define LOGREDSTONE(...)
#endif


#include "RedstoneHandler.h"
#include "RedstoneDataHelper.h"
#include "ForEachSourceCallback.h"

#include "BlockType.h"
#include "CommandBlockHandler.h"
#include "DaylightSensorHandler.h"
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





#define INVOKE_FOR_HANDLERS(Callback) \
	do \
	{ \
		switch (a_Block.Type()) \
		{ \
			case BlockType::ActivatorRail: \
			case BlockType::DetectorRail: \
			case BlockType::PoweredRail: \
				return PoweredRailHandler::Callback; \
			case BlockType::Comparator: \
				return RedstoneComparatorHandler::Callback; \
			case BlockType::Dispenser: \
			case BlockType::Dropper: \
				return DropSpenserHandler::Callback; \
			case BlockType::AcaciaPressurePlate: \
			case BlockType::BirchPressurePlate: \
			case BlockType::CrimsonPressurePlate: \
			case BlockType::DarkOakPressurePlate: \
			case BlockType::HeavyWeightedPressurePlate: \
			case BlockType::JunglePressurePlate: \
			case BlockType::LightWeightedPressurePlate: \
			case BlockType::OakPressurePlate: \
			case BlockType::PolishedBlackstonePressurePlate: \
			case BlockType::SprucePressurePlate: \
			case BlockType::StonePressurePlate: \
			case BlockType::WarpedPressurePlate: \
				return PressurePlateHandler::Callback; \
			case BlockType::AcaciaFenceGate: \
			case BlockType::BirchFenceGate: \
			case BlockType::CrimsonFenceGate: \
			case BlockType::DarkOakFenceGate: \
			case BlockType::JungleFenceGate: \
			case BlockType::OakFenceGate: \
			case BlockType::SpruceFenceGate: \
			case BlockType::WarpedFenceGate: \
			case BlockType::IronTrapdoor: \
			case BlockType::AcaciaTrapdoor: \
			case BlockType::BirchTrapdoor: \
			case BlockType::CrimsonTrapdoor: \
			case BlockType::DarkOakTrapdoor: \
			case BlockType::JungleTrapdoor: \
			case BlockType::OakTrapdoor: \
			case BlockType::SpruceTrapdoor: \
			case BlockType::WarpedTrapdoor: \
				return SmallGateHandler::Callback; \
			case BlockType::RedstoneLamp: \
				return RedstoneLampHandler::Callback; \
			case BlockType::Repeater: \
				return RedstoneRepeaterHandler::Callback; \
			case BlockType::RedstoneTorch: \
			case BlockType::RedstoneWallTorch: \
				return RedstoneTorchHandler::Callback; \
			case BlockType::Observer: \
				return ObserverHandler::Callback; \
			case BlockType::Piston: \
			case BlockType::PistonHead: \
			case BlockType::StickyPiston: \
			case BlockType::MovingPiston: \
				return PistonHandler::Callback; \
			case BlockType::DaylightDetector: \
				return DaylightSensorHandler::Callback; \
			case BlockType::Lever: \
			case BlockType::AcaciaButton: \
			case BlockType::BirchButton: \
			case BlockType::CrimsonButton: \
			case BlockType::DarkOakButton: \
			case BlockType::JungleButton: \
			case BlockType::OakButton: \
			case BlockType::PolishedBlackstoneButton: \
			case BlockType::SpruceButton: \
			case BlockType::StoneButton: \
			case BlockType::WarpedButton: \
				return RedstoneToggleHandler::Callback; \
			case BlockType::RedstoneBlock: \
				return RedstoneBlockHandler::Callback; \
			case BlockType::ChainCommandBlock: \
			case BlockType::CommandBlock: \
			case BlockType::RepeatingCommandBlock: \
				return CommandBlockHandler::Callback; \
			case BlockType::Hopper: \
				return HopperHandler::Callback; \
			case BlockType::NoteBlock: \
				return NoteBlockHandler::Callback; \
			case BlockType::RedstoneWire: \
				return RedstoneWireHandler::Callback; \
			case BlockType::Tnt: \
				return TNTHandler::Callback; \
			case BlockType::TrappedChest: \
				return TrappedChestHandler::Callback; \
			case BlockType::TripwireHook: \
				return TripwireHookHandler::Callback; \
			default: \
			{ \
				if (cBlockDoorHandler::IsBlockDoor(a_Block)) \
				{ \
					return DoorHandler::Callback; \
				} \
			} \
		} \
	} \
	while (false)




namespace RedstoneHandler
{
	PowerLevel GetPowerDeliveredToPosition(const cChunk & a_Chunk, const Vector3i a_Position, const BlockState a_Block, const Vector3i a_QueryPosition, const BlockState a_QueryBlock, const bool a_IsLinked)
	{
		INVOKE_FOR_HANDLERS(GetPowerDeliveredToPosition(a_Chunk, a_Position, a_Block, a_QueryPosition, a_QueryBlock, a_IsLinked));

		// Fell through the switch statement
		// Block at Position doesn't have a corresponding redstone handler
		// ErasePowerData will have been called in AddBlock
		// Default:
		return 0;
	}

	void Update(cChunk & Chunk, cChunk & CurrentlyTicking, const Vector3i Position, const BlockState a_Block, const PowerLevel PowerLevel)
	{
		INVOKE_FOR_HANDLERS(Update(Chunk, CurrentlyTicking, Position, a_Block, PowerLevel));
	}

	void ForValidSourcePositions(const cChunk & Chunk, const Vector3i Position, const BlockState a_Block, ForEachSourceCallback & Callback)
	{
		INVOKE_FOR_HANDLERS(ForValidSourcePositions(Chunk, Position, a_Block, Callback));
	}

	void SetWireState(const cChunk & Chunk, const Vector3i Position)
	{
		RedstoneWireHandler::SetWireState(Chunk, Position);
	}
}
