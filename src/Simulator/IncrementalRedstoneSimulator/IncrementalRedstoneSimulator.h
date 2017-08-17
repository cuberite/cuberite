
#pragma once

#include "../RedstoneSimulator.h"
#include "RedstoneSimulatorChunkData.h"





class cIncrementalRedstoneSimulator :
	public cRedstoneSimulator
{
	typedef cRedstoneSimulator super;
public:
	cIncrementalRedstoneSimulator(cWorld & a_World) :
		super(a_World)
	{
	}

	virtual void Simulate(float a_dt) override;
	virtual void SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override {}

	virtual cIncrementalRedstoneSimulatorChunkData * CreateChunkData() override
	{
		return new cIncrementalRedstoneSimulatorChunkData;
	}

	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) override
	{
		return IsRedstone(a_BlockType);
	}

	virtual void AddBlock(Vector3i a_Block, cChunk * a_Chunk) override
	{
		m_Data.WakeUp(a_Block);
	}

	/** Returns if a block is a mechanism (something that accepts power and does something)
	Used by torches to determine if they will power a block
	*/
	inline static bool IsMechanism(BLOCKTYPE Block)
	{
		switch (Block)
		{
			case E_BLOCK_ACACIA_DOOR:
			case E_BLOCK_ACACIA_FENCE_GATE:
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_ACTIVE_COMPARATOR:
			case E_BLOCK_BIRCH_DOOR:
			case E_BLOCK_BIRCH_FENCE_GATE:
			case E_BLOCK_COMMAND_BLOCK:
			case E_BLOCK_DARK_OAK_DOOR:
			case E_BLOCK_DARK_OAK_FENCE_GATE:
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			case E_BLOCK_FENCE_GATE:
			case E_BLOCK_HOPPER:
			case E_BLOCK_INACTIVE_COMPARATOR:
			case E_BLOCK_IRON_DOOR:
			case E_BLOCK_IRON_TRAPDOOR:
			case E_BLOCK_JUNGLE_DOOR:
			case E_BLOCK_JUNGLE_FENCE_GATE:
			case E_BLOCK_NOTE_BLOCK:
			case E_BLOCK_PISTON:
			case E_BLOCK_POWERED_RAIL:
			case E_BLOCK_REDSTONE_LAMP_OFF:
			case E_BLOCK_REDSTONE_LAMP_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_WIRE:
			case E_BLOCK_SPRUCE_DOOR:
			case E_BLOCK_SPRUCE_FENCE_GATE:
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_TNT:
			case E_BLOCK_TRAPDOOR:
			case E_BLOCK_WOODEN_DOOR:
			{
				return true;
			}
			default: return false;
		}
	}

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
			case E_BLOCK_TRIPWIRE:
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

	cIncrementalRedstoneSimulatorChunkData * GetChunkData() { return &m_Data; }

	static const cRedstoneHandler * GetComponentHandler(BLOCKTYPE a_BlockType);

private:

	static std::unique_ptr<cRedstoneHandler> CreateComponent(BLOCKTYPE a_BlockType);

	// oh yea its crazy time
	cIncrementalRedstoneSimulatorChunkData m_Data;
} ;
