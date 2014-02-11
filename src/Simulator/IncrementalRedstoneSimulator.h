
#pragma once

#include "RedstoneSimulator.h"

/// Per-chunk data for the simulator, specified individual chunks to simulate; 'Data' is not used
typedef cCoordWithBlockAndBoolVector cRedstoneSimulatorChunkData;





class cIncrementalRedstoneSimulator :
	public cRedstoneSimulator
{
	typedef cRedstoneSimulator super;
public:

	cIncrementalRedstoneSimulator(cWorld & a_World);
	~cIncrementalRedstoneSimulator();

	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}  // not used
	virtual void SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk) override;
	virtual bool IsAllowedBlock( BLOCKTYPE a_BlockType ) override { return IsRedstone(a_BlockType); }

	enum eRedstoneDirection
	{
		REDSTONE_NONE =  0,
		REDSTONE_X_POS = 0x1,
		REDSTONE_X_NEG = 0x2,
		REDSTONE_Z_POS = 0x4,
		REDSTONE_Z_NEG = 0x8,
	};
	eRedstoneDirection GetWireDirection(int a_BlockX, int a_BlockY, int a_BlockZ);

private:

	struct sPoweredBlocks // Define structure of the directly powered blocks list
	{
		Vector3i a_BlockPos; // Position of powered block
		Vector3i a_SourcePos; // Position of source powering the block at a_BlockPos
	};

	struct sLinkedPoweredBlocks // Define structure of the indirectly powered blocks list (i.e. repeaters powering through a block to the block at the other side)
	{
		Vector3i a_BlockPos;
		Vector3i a_MiddlePos;
		Vector3i a_SourcePos;
	};

	struct sSimulatedPlayerToggleableList
	{
		Vector3i a_BlockPos;
		bool WasLastStatePowered;
	};

	struct sRepeatersDelayList
	{
		Vector3i a_BlockPos;
		short a_DelayTicks;
		short a_ElapsedTicks;
		bool ShouldPowerOn;
	};
	
	typedef std::vector <sPoweredBlocks> PoweredBlocksList;
	typedef std::vector <sLinkedPoweredBlocks> LinkedBlocksList;
	typedef std::vector <sSimulatedPlayerToggleableList> SimulatedPlayerToggleableList;
	typedef std::vector <sRepeatersDelayList> RepeatersDelayList;

	PoweredBlocksList m_PoweredBlocks;
	LinkedBlocksList m_LinkedPoweredBlocks;
	SimulatedPlayerToggleableList m_SimulatedPlayerToggleableBlocks;
	RepeatersDelayList m_RepeatersDelayList;

	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override;

	// We want a_MyState for devices needing a full FastSetBlock (as opposed to meta) because with our simulation model, we cannot keep setting the block if it is already set correctly
	// In addition to being non-performant, it would stop the player from actually breaking said device

	/* ====== SOURCES ====== */
	/** Handles the redstone torch */
	void HandleRedstoneTorch(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState);
	/** Handles the redstone block */
	void HandleRedstoneBlock(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles levers */
	void HandleRedstoneLever(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles Fence Gates */
	void HandleFenceGate(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles buttons */
	void HandleRedstoneButton(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType);
	/** Handles daylight sensors */
	void HandleDaylightSensor(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles pressure plates */
	void HandlePressurePlate(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyType);
	/* ==================== */

	/* ====== CARRIERS ====== */
	/** Handles redstone wire */
	void HandleRedstoneWire(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles repeaters */
	void HandleRedstoneRepeater(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState);
	/* ====================== */

	/* ====== DEVICES ====== */
	/** Handles pistons */
	void HandlePiston(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles dispensers and droppers */
	void HandleDropSpenser(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles TNT (exploding) */
	void HandleTNT(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles redstone lamps */
	void HandleRedstoneLamp(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState);
	/** Handles doords */
	void HandleDoor(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles command blocks */
	void HandleCommandBlock(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles activator, detector, and powered rails */
	void HandleRail(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyType);
	/** Handles trapdoors */
	void HandleTrapdoor(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Handles noteblocks */
	void HandleNoteBlock(int a_BlockX, int a_BlockY, int a_BlockZ);
	/* ===================== */

	/* ====== Helper functions ====== */
	/** Marks a block as powered */
	void SetBlockPowered(int a_BlockX, int a_BlockY, int a_BlockZ, int a_SourceX, int a_SourceY, int a_SourceZ, BLOCKTYPE a_SourceBlock);
	/** Marks a block as being powered through another block */
	void SetBlockLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ, int a_MiddleX, int a_MiddleY, int a_MiddleZ, int a_SourceX, int a_SourceY, int a_SourceZ, BLOCKTYPE a_SourceBlock, BLOCKTYPE a_MiddeBlock);
	/** Marks a block as simulated, who should not be simulated further unless their power state changes, to accomodate a player manually toggling the block without triggering the simulator toggling it back */
	void SetPlayerToggleableBlockAsSimulated(int a_BlockX, int a_BlockY, int a_BlockZ, bool WasLastStatePowered);
	/** Marks the second block in a direction as linked powered */
	void SetDirectionLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Direction, BLOCKTYPE a_SourceBlock);
	/** Marks all blocks immediately surrounding a coordinate as powered */
	void SetAllDirsAsPowered(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_SourceBlock);
	/** Queues a repeater to be powered or unpowered */
	void QueueRepeaterPowerChange(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta, short a_ElapsedTicks, bool ShouldPowerOn);

	/** Returns if a coordinate is powered or linked powered */
	bool AreCoordsPowered(int a_BlockX, int a_BlockY, int a_BlockZ) { return AreCoordsDirectlyPowered(a_BlockX, a_BlockY, a_BlockZ) || AreCoordsLinkedPowered(a_BlockX, a_BlockY, a_BlockZ); }
	/** Returns if a coordinate is in the directly powered blocks list */
	bool AreCoordsDirectlyPowered(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Returns if a coordinate is in the indirectly powered blocks list */
	bool AreCoordsLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ);
	/** Returns if a coordinate was marked as simulated (for blocks toggleable by players) */
	bool AreCoordsSimulated(int a_BlockX, int a_BlockY, int a_BlockZ, bool IsCurrentStatePowered);
	/** Returns if a repeater is powered */
	bool IsRepeaterPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta);
	/** Returns if a piston is powered */
	bool IsPistonPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta);
	/** Returns if a wire is powered
		The only diffence between this and a normal AreCoordsPowered is that this function checks for a wire powering another wire
	*/
	bool IsWirePowered(int a_BlockX, int a_BlockY, int a_BlockZ);


	/** Returns if lever metadata marks it as emitting power */
	bool IsLeverOn(NIBBLETYPE a_BlockMeta);
	/** Returns if button metadata marks it as emitting power */
	bool IsButtonOn(NIBBLETYPE a_BlockMeta);
	/* ============================== */

	/* ====== Misc Functions ====== */
	/** Returns if a block is viable to be the MiddleBlock of a SetLinkedPowered operation */
	inline static bool IsViableMiddleBlock(BLOCKTYPE Block) { return g_BlockFullyOccupiesVoxel[Block]; }

	/** Returns if a block is a mechanism (something that accepts power and does something) */
	inline static bool IsMechanism(BLOCKTYPE Block)
	{
		switch (Block)
		{
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_COMMAND_BLOCK:
			case E_BLOCK_PISTON:
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			case E_BLOCK_FENCE_GATE:
			case E_BLOCK_HOPPER:
			case E_BLOCK_NOTE_BLOCK:
			case E_BLOCK_TNT:
			case E_BLOCK_TRAPDOOR:
			case E_BLOCK_REDSTONE_LAMP_OFF:
			case E_BLOCK_REDSTONE_LAMP_ON:
			case E_BLOCK_WOODEN_DOOR:
			case E_BLOCK_IRON_DOOR:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_POWERED_RAIL:
			{
				return true;
			}
			default: return false;
		}
	}

	/** Returns if a block has the potential to output power */
	inline static bool IsPotentialSource(BLOCKTYPE Block)
	{
		switch (Block)
		{
			case E_BLOCK_DETECTOR_RAIL:
			case E_BLOCK_DAYLIGHT_SENSOR:
			case E_BLOCK_WOODEN_BUTTON:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_REDSTONE_WIRE:
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_LEVER:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_BLOCK_OF_REDSTONE:
			case E_BLOCK_ACTIVE_COMPARATOR:
			{
				return true;
			}
			default: return false;
		}
	}

	/** Returns if a block is any sort of redstone device */
	inline static bool IsRedstone(BLOCKTYPE Block)
	{
		switch (Block)
		{
			// All redstone devices, please alpha sort
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_ACTIVE_COMPARATOR:
			case E_BLOCK_BLOCK_OF_REDSTONE:
			case E_BLOCK_COMMAND_BLOCK:
			case E_BLOCK_DETECTOR_RAIL:
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DAYLIGHT_SENSOR:
			case E_BLOCK_DROPPER:
			case E_BLOCK_FENCE_GATE:
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_HOPPER:
			case E_BLOCK_INACTIVE_COMPARATOR:
			case E_BLOCK_IRON_DOOR:
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
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_STONE_PRESSURE_PLATE:
			case E_BLOCK_TNT:
			case E_BLOCK_TRAPDOOR:
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
};
