
#pragma once

#include "../RedstoneSimulator.h"





class cIncrementalRedstoneSimulator final :
	public cRedstoneSimulator
{
	using Super = cRedstoneSimulator;

public:

	using Super::Super;

private:

	/** Returns if a redstone device is always ticked due to influence by its environment */
	inline static bool IsAlwaysTicked(BlockType a_Block)
	{
		switch (a_Block)  // Call the appropriate simulator for the entry's block type
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

	/** Returns if a block is any sort of redstone device */
	inline static bool IsRedstone(BlockType a_Block)
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
			case BlockType::TNT:
			case BlockType::TrappedChest:
			case BlockType::TripwireHook:
			case BlockType::Piston:
			{
				return true;
			}
			default: return false;
		}
	}

	void ProcessWorkItem(cChunk & Chunk, cChunk & TickingSource, const Vector3i Position);

	virtual void Simulate(float Dt) override {}
	virtual void SimulateChunk(std::chrono::milliseconds Dt, int ChunkX, int ChunkZ, cChunk * Chunk) override;

	void ProcessWorkItem(cChunk & Chunk, cChunk & TickingSource, const Vector3i Position);
	virtual cIncrementalRedstoneSimulatorChunkData * CreateChunkData() override
	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block) override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block) override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BlockState a_Block) override;
} ;
