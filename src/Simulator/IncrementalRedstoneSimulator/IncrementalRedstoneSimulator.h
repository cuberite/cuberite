
#pragma once

#include "../RedstoneSimulator.h"
#include "RedstoneSimulatorChunkData.h"





class cIncrementalRedstoneSimulator final :
	public cRedstoneSimulator
{
	using Super = cRedstoneSimulator;

public:

	using Super::cRedstoneSimulator;

private:

	/** Returns if a redstone device is always ticked due to influence by its environment */
	inline static bool IsAlwaysTicked(BLOCKTYPE a_Block)
	{
		switch (a_Block)  // Call the appropriate simulator for the entry's block type
		{
			case E_BLOCK_DAYLIGHT_SENSOR:
			case E_BLOCK_TRIPWIRE_HOOK:
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
			case E_BLOCK_STONE_PRESSURE_PLATE:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE: return true;
			default: return false;
		}
	}

	/** Returns if a block is any sort of redstone device */
	inline static bool IsRedstone(BLOCKTYPE a_Block)
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

	virtual void Simulate(float Dt) override {}
	virtual void SimulateChunk(std::chrono::milliseconds Dt, int ChunkX, int ChunkZ, cChunk * Chunk) override;

	void ProcessWorkItem(cChunk & Chunk, cChunk & TickingSource, const Vector3i Position);

	virtual cIncrementalRedstoneSimulatorChunkData * CreateChunkData() override
	{
		return new cIncrementalRedstoneSimulatorChunkData;
	}

	virtual void AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block) override;
	virtual void WakeUp(cChunk & a_Chunk, Vector3i a_Position, Vector3i a_Offset, BLOCKTYPE a_Block) override;
} ;
