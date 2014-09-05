
#pragma once

#include "RedstoneSimulator.h"

/// Per-chunk data for the simulator, specified individual chunks to simulate
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
	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) override { return IsRedstone(a_BlockType); }
	virtual void WakeUp(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override;

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

	#define MAX_POWER_LEVEL 15

	struct sPoweredBlocks  // Define structure of the directly powered blocks list
	{
		Vector3i a_BlockPos;  // Position of powered block
		Vector3i a_SourcePos;  // Position of source powering the block at a_BlockPos
		unsigned char a_PowerLevel;
	};

	struct sLinkedPoweredBlocks  // Define structure of the indirectly powered blocks list (i.e. repeaters powering through a block to the block at the other side)
	{
		Vector3i a_BlockPos;
		Vector3i a_MiddlePos;  // Position of block that is betwixt a source and the destination
		Vector3i a_SourcePos;
		unsigned char a_PowerLevel;
	};

	struct sSimulatedPlayerToggleableList  // Define structure of the list containing simulate-on-update blocks (such as trapdoors that respond once to a block update, and can be toggled by a player)
	{
		Vector3i a_RelBlockPos;
		bool WasLastStatePowered;  // Was the last state powered or not? Determines whether a source update has happened and if I should resimulate
	};

	struct sRepeatersDelayList  // Define structure of list containing repeaters' delay states
	{
		Vector3i a_RelBlockPos;
		unsigned char a_DelayTicks;  // For how many ticks should the repeater delay
		unsigned char a_ElapsedTicks;  // How much of the previous has been elapsed?
		bool ShouldPowerOn;  // What happens when the delay time is fulfilled?
	};

public:

	typedef std::vector <sPoweredBlocks> PoweredBlocksList;
	typedef std::vector <sLinkedPoweredBlocks> LinkedBlocksList;
	typedef std::vector <sSimulatedPlayerToggleableList> SimulatedPlayerToggleableList;
	typedef std::vector <sRepeatersDelayList> RepeatersDelayList;

private:

	cRedstoneSimulatorChunkData * m_RedstoneSimulatorChunkData;
	PoweredBlocksList * m_PoweredBlocks;
	LinkedBlocksList * m_LinkedPoweredBlocks;
	SimulatedPlayerToggleableList * m_SimulatedPlayerToggleableBlocks;
	RepeatersDelayList * m_RepeatersDelayList;

	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override { RedstoneAddBlock(a_BlockX, a_BlockY, a_BlockZ, a_Chunk); }
	void RedstoneAddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk, cChunk * a_OtherChunk = NULL);
	cChunk * m_Chunk;

	// We want a_MyState for devices needing a full FastSetBlock (as opposed to meta) because with our simulation model, we cannot keep setting the block if it is already set correctly
	// In addition to being non-performant, it would stop the player from actually breaking said device

	/* ====== SOURCES ====== */
	/** Handles the redstone torch */
	void HandleRedstoneTorch(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyState);
	/** Handles the redstone block */
	void HandleRedstoneBlock(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles levers */
	void HandleRedstoneLever(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles buttons */
	void HandleRedstoneButton(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles daylight sensors */
	void HandleDaylightSensor(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles pressure plates */
	void HandlePressurePlate(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyType);
	/** Handles tripwire hooks
	Performs correct meta and power setting for self by going in the direction it faces and looking for a continous line of tripwire bounded by another oppositely facing hook
	If this line is complete, it verifies that at least on wire reports an entity is on top (via its meta), and performs its task
	*/
	void HandleTripwireHook(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles trapped chests */
	void HandleTrappedChest(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/* ==================== */

	/* ====== CARRIERS ====== */
	/** Handles redstone wire */
	void HandleRedstoneWire(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles repeaters */
	void HandleRedstoneRepeater(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyState);
	/* ====================== */

	/* ====== DEVICES ====== */
	/** Handles pistons */
	void HandlePiston(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles dispensers and droppers */
	void HandleDropSpenser(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles TNT (exploding) */
	void HandleTNT(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles redstone lamps */
	void HandleRedstoneLamp(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyState);
	/** Handles doords */
	void HandleDoor(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles command blocks */
	void HandleCommandBlock(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles activator, detector, and powered rails */
	void HandleRail(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, BLOCKTYPE a_MyType);
	/** Handles trapdoors */
	void HandleTrapdoor(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles fence gates */
	void HandleFenceGate(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles noteblocks */
	void HandleNoteBlock(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Handles tripwires */
	void HandleTripwire(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/* ===================== */

	/* ====== Helper functions ====== */
	/** Marks a block as powered */
	void SetBlockPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, int a_RelSourceX, int a_RelSourceY, int a_RelSourceZ, unsigned char a_PowerLevel = MAX_POWER_LEVEL);
	/** Marks a block as being powered through another block */
	void SetBlockLinkedPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, int a_RelMiddleX, int a_RelMiddleY, int a_RelMiddleZ, int a_RelSourceX, int a_RelSourceY, int a_RelSourceZ, BLOCKTYPE a_MiddeBlock, unsigned char a_PowerLevel = MAX_POWER_LEVEL);
	/** Marks a block as simulated, who should not be simulated further unless their power state changes, to accomodate a player manually toggling the block without triggering the simulator toggling it back */
	void SetPlayerToggleableBlockAsSimulated(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, bool WasLastStatePowered);
	/** Marks the second block in a direction as linked powered */
	void SetDirectionLinkedPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, char a_Direction, unsigned char a_PowerLevel = MAX_POWER_LEVEL);
	/** Marks all blocks immediately surrounding a coordinate as powered */
	void SetAllDirsAsPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, unsigned char a_PowerLevel = MAX_POWER_LEVEL);
	/** Queues a repeater to be powered or unpowered and returns if the m_RepeatersDelayList iterators were invalidated */
	bool QueueRepeaterPowerChange(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, NIBBLETYPE a_Meta, bool ShouldPowerOn);
	/** Removes a block from the Powered and LinkedPowered lists
	Used for variable sources such as tripwire hooks, daylight sensors, and trapped chests
	*/
	void SetSourceUnpowered(int a_RelSourceX, int a_RelSourceY, int a_RelSourceZ, cChunk * a_Chunk, bool a_IsFirstCall = true);

	/** Returns if a coordinate is powered or linked powered */
	bool AreCoordsPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ) { return AreCoordsDirectlyPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ, m_Chunk) || AreCoordsLinkedPowered(a_RelBlockX, a_RelBlockY, a_RelBlockZ); }
	/** Returns if a coordinate is in the directly powered blocks list */
	bool AreCoordsDirectlyPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, cChunk * a_Chunk);
	/** Returns if a coordinate is in the indirectly powered blocks list */
	bool AreCoordsLinkedPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ);
	/** Returns if a coordinate was marked as simulated (for blocks toggleable by players) */
	bool AreCoordsSimulated(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, bool IsCurrentStatePowered);
	/** Returns if a repeater is powered by testing for power sources behind the repeater */
	bool IsRepeaterPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, NIBBLETYPE a_Meta);
	/** Returns if a repeater is locked */
	bool IsRepeaterLocked(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, NIBBLETYPE a_Meta);
	/** Returns if a piston is powered */
	bool IsPistonPowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, NIBBLETYPE a_Meta);
	/** Returns if a wire is powered
	The only diffence between this and a normal AreCoordsPowered is that this function checks for a wire powering another wire */
	bool IsWirePowered(int a_RelBlockX, int a_RelBlockY, int a_RelBlockZ, unsigned char & a_PowerLevel);
	/** Handles delayed updates to repeaters **/
	void HandleRedstoneRepeaterDelays(void);

	/** Returns if lever metadata marks it as emitting power */
	bool IsLeverOn(NIBBLETYPE a_BlockMeta);
	/** Returns if button metadata marks it as emitting power */
	bool IsButtonOn(NIBBLETYPE a_BlockMeta) { return IsLeverOn(a_BlockMeta); }
	/* ============================== */

	/* ====== Misc Functions ====== */
	/** Returns if a block is viable to be the MiddleBlock of a SetLinkedPowered operation */
	inline static bool IsViableMiddleBlock(BLOCKTYPE Block) { return cBlockInfo::FullyOccupiesVoxel(Block); }

	/** Returns if a block is a mechanism (something that accepts power and does something)
	Used by torches to determine if they power a block whilst not standing on the ground
	*/
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
			case E_BLOCK_REDSTONE_WIRE:
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
			case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
			case E_BLOCK_STONE_PRESSURE_PLATE:
			case E_BLOCK_WOODEN_PRESSURE_PLATE:
			case E_BLOCK_TRAPPED_CHEST:
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

	inline static bool AreCoordsOnChunkBoundary(int a_BlockX, int a_BlockY, int a_BlockZ)
	{
		return (  // Are we on a chunk boundary? +- 2 because of LinkedPowered blocks
			((a_BlockX % cChunkDef::Width) <= 1) ||
			((a_BlockX % cChunkDef::Width) >= 14) ||
			((a_BlockZ % cChunkDef::Width) <= 1) ||
			((a_BlockZ % cChunkDef::Width) >= 14)
		);
	}
};




