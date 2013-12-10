
#pragma once

#include "Simulator.h"

/// Per-chunk data for the simulator, specified individual chunks to simulate; 'Data' is not used
typedef cCoordWithIntList cRedstoneSimulatorChunkData;





class cRedstoneSimulator :
	public cSimulator
{
	typedef cSimulator super;
public:

	cRedstoneSimulator(cWorld & a_World);
	~cRedstoneSimulator();

	virtual void Simulate(float a_Dt) override {}; // Not used in this simulator
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
		BLOCKTYPE a_SourceBlock; // The source block type (for pistons pushing away sources and replacing with non source etc.)
	};

	struct sLinkedPoweredBlocks // Define structure of the indirectly powered blocks list (i.e. repeaters powering through a block to the block at the other side)
	{
		Vector3i a_BlockPos;
		Vector3i a_MiddlePos;
		Vector3i a_SourcePos;
		BLOCKTYPE a_SourceBlock;
		BLOCKTYPE a_MiddleBlock;
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
	/// <summary>Handles the redstone torch</summary>
	void HandleRedstoneTorch(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState);
	/// <summary>Handles the redstone block</summary>
	void HandleRedstoneBlock(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Handles levers</summary>
	void HandleRedstoneLever(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Handles buttons</summary>
	void HandleRedstoneButton(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType);
	/* ==================== */

	/* ====== CARRIERS ====== */
	/// <summary>Handles redstone wire</summary>
	void HandleRedstoneWire(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Handles repeaters</summary>
	void HandleRedstoneRepeater(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState);
	/* ====================== */

	/* ====== DEVICES ====== */
	/// <summary>Handles pistons</summary>
	void HandlePiston(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Handles dispensers and droppers</summary>
	void HandleDropSpenser(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Handles TNT (exploding)</summary>
	void HandleTNT(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Handles redstone lamps</summary>
	void HandleRedstoneLamp(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyState);
	/// <summary>Handles doords</summary>
	void HandleDoor(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Handles activator, detector, and powered rails</summary>
	void HandleRail(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_MyType);
	/// <summary>Handles trapdoors</summary>
	void HandleTrapdoor(int a_BlockX, int a_BlockY, int a_BlockZ);
	/* ===================== */

	/* ====== Helper functions ====== */
	/// <summary>Marks a block as powered</summary>
	void SetBlockPowered(int a_BlockX, int a_BlockY, int a_BlockZ, int a_SourceX, int a_SourceY, int a_SourceZ, BLOCKTYPE a_SourceBlock);
	/// <summary>Marks a block as being powered through another block</summary>
	void SetBlockLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ, int a_MiddleX, int a_MiddleY, int a_MiddleZ, int a_SourceX, int a_SourceY, int a_SourceZ, BLOCKTYPE a_SourceBlock, BLOCKTYPE a_MiddeBlock);
	/// <summary>Marks a block as simulated, who should not be simulated further unless their power state changes, to accomodate a player manually toggling the block without triggering the simulator toggling it back</summary>
	void SetPlayerToggleableBlockAsSimulated(int a_BlockX, int a_BlockY, int a_BlockZ, bool WasLastStatePowered);
	/// <summary>Marks the second block in a direction as linked powered</summary>
	void SetDirectionLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ, char a_Direction, BLOCKTYPE a_SourceBlock);
	/// <summary>Marks all blocks immediately surrounding a coordinate as powered</summary>
	void SetAllDirsAsPowered(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_SourceBlock);

	/// <summary>Returns if a coordinate is powered or linked powered</summary>
	bool AreCoordsPowered(int a_BlockX, int a_BlockY, int a_BlockZ) { return AreCoordsDirectlyPowered(a_BlockX, a_BlockY, a_BlockZ) || AreCoordsLinkedPowered(a_BlockX, a_BlockY, a_BlockZ); }
	/// <summary>Returns if a coordinate is in the directly powered blocks list</summary>
	bool AreCoordsDirectlyPowered(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Returns if a coordinate is in the indirectly powered blocks list</summary>
	bool AreCoordsLinkedPowered(int a_BlockX, int a_BlockY, int a_BlockZ);
	/// <summary>Returns if a coordinate was marked as simulated (for blocks toggleable by players)</summary>
	bool AreCoordsSimulated(int a_BlockX, int a_BlockY, int a_BlockZ, bool IsCurrentStatePowered);
	/// <summary>Returns if a repeater is powered</summary>
	bool IsRepeaterPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta);
	/// <summary>Returns if a piston is powered</summary>
	bool IsPistonPowered(int a_BlockX, int a_BlockY, int a_BlockZ, NIBBLETYPE a_Meta);

	/// <summary>Returns if lever metadata marks it as emitting power</summary>
	bool IsLeverOn(NIBBLETYPE a_BlockMeta);
	/// <summary>Returns if button metadata marks it as emitting power</summary>
	bool IsButtonOn(NIBBLETYPE a_BlockMeta);
	/* ============================== */

	/* ====== Misc Functions ====== */
	/// <summary>Returns if a block is viable to be the MiddleBlock of a SetLinkedPowered operation</summary>
	inline static bool IsViableMiddleBlock(BLOCKTYPE Block)
	{
		if (!g_BlockIsSolid[Block]) { return false; }

		switch (Block)
		{
			// Add SOLID but not viable middle blocks here
			case E_BLOCK_PISTON:
			case E_BLOCK_PISTON_EXTENSION:
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			{
				return false;
			}
			default:
			{
				return true;
			}
		}
	}

	/// <summary>Returns if a block is a mechanism (something that accepts power and does something)</summary>
	inline static bool IsMechanism(BLOCKTYPE Block)
	{
		switch (Block)
		{
			case E_BLOCK_ACTIVATOR_RAIL:
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

	/// <summary>Returns if a block has the potential to output power</summary>
	inline static bool IsPotentialSource(BLOCKTYPE Block)
	{
		switch (Block)
		{
			case E_BLOCK_WOODEN_BUTTON:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_REDSTONE_WIRE:
			case E_BLOCK_REDSTONE_TORCH_OFF:
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

	/// <summary>Returns if a block is any sort of redstone device</summary>
	inline static bool IsRedstone(BLOCKTYPE Block)
	{
		switch (Block)
		{
			// All redstone devices, please alpha sort
			case E_BLOCK_ACTIVATOR_RAIL:
			case E_BLOCK_ACTIVE_COMPARATOR:
			case E_BLOCK_BLOCK_OF_REDSTONE:
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