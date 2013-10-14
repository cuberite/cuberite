
#pragma once

#include "Simulator.h"





class cRedstoneSimulator :
	public cSimulator
{
	typedef cSimulator super;
public:

	cRedstoneSimulator(cWorld & a_World);
	~cRedstoneSimulator();

	virtual void Simulate( float a_Dt ) override;
	virtual bool IsAllowedBlock( BLOCKTYPE a_BlockType ) override { return true; }
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
	eRedstoneDirection GetWireDirection(const Vector3i & a_Pos) { return GetWireDirection(a_Pos.x, a_Pos.y, a_Pos.z); }

	static NIBBLETYPE RepeaterRotationToMetaData(double a_Rotation);
	static NIBBLETYPE LeverDirectionToMetaData(char a_Dir);

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
	
	typedef std::deque <Vector3i> BlockList;
	typedef std::deque <sPoweredBlocks> PoweredBlocksList;
	typedef std::deque <sLinkedPoweredBlocks> LinkedBlocksList;

	BlockList m_Blocks;
	PoweredBlocksList m_PoweredBlocks;
	LinkedBlocksList m_LinkedPoweredBlocks;

	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override {}

	// Sources
	void HandleRedstoneTorch(const Vector3i & a_BlockPos, BLOCKTYPE a_MyState);
	void HandleRedstoneBlock(const Vector3i & a_BlockPos);
	void HandleRedstoneLever(const Vector3i & a_BlockPos);
	void HandleRedstoneButton(const Vector3i & a_BlockPos, BLOCKTYPE a_BlockType);

	// The all important wire and repeater
	void HandleRedstoneWire(const Vector3i & a_BlockPos);
	void HandleRedstoneRepeater(const Vector3i & a_BlockPos);

	// Devices
	void HandlePiston(const Vector3i & a_BlockPos);
	void HandleDropSpenser(const Vector3i & a_BlockPos);
	void HandleTNT(const Vector3i & a_BlockPos);
	void HandleRedstoneLamp(const Vector3i & a_BlockPos, BLOCKTYPE a_MyState);
	void HandleDoor(const Vector3i & a_BlockPos);

	// Helper functions
	void SetBlockPowered(const Vector3i & a_BlockPos, const Vector3i & a_SourcePos, BLOCKTYPE a_SourceBlock);
	void SetBlockLinkedPowered(const Vector3i & a_BlockPos, const Vector3i & a_MiddlePos, const Vector3i & a_SourcePos, BLOCKTYPE a_SourceBlock, BLOCKTYPE a_MiddeBlock);
	bool AreCoordsPowered(const Vector3i & a_BlockPos);
	bool IsRepeaterPowered(const Vector3i & a_BlockPos, NIBBLETYPE a_Meta);
	bool IsLeverOn(NIBBLETYPE a_BlockMeta);
	bool IsButtonOn(NIBBLETYPE a_BlockMeta);

	inline static bool IsMechanism(BLOCKTYPE a_Block)
	{
		switch (a_Block)
		{
			case E_BLOCK_PISTON:
			case E_BLOCK_STICKY_PISTON:
			case E_BLOCK_DISPENSER:
			case E_BLOCK_DROPPER:
			case E_BLOCK_TNT:
			case E_BLOCK_REDSTONE_LAMP_OFF:
			case E_BLOCK_REDSTONE_LAMP_ON:
			case E_BLOCK_WOODEN_DOOR:
			case E_BLOCK_IRON_DOOR:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			{
				return true;
			}
		}
		return false;
	}

	inline static bool IsPotentialSource(BLOCKTYPE a_Block)
	{
		switch (a_Block)
		{
			case E_BLOCK_WOODEN_BUTTON:
			case E_BLOCK_STONE_BUTTON:
			case E_BLOCK_REDSTONE_WIRE:
			case E_BLOCK_REDSTONE_TORCH_OFF:
			case E_BLOCK_REDSTONE_TORCH_ON:
			case E_BLOCK_LEVER:
			case E_BLOCK_REDSTONE_REPEATER_ON:
			case E_BLOCK_REDSTONE_REPEATER_OFF:
			case E_BLOCK_BLOCK_OF_REDSTONE:
			case E_BLOCK_ACTIVE_COMPARATOR:
			case E_BLOCK_INACTIVE_COMPARATOR:
			{
				return true;
			}
		}
		return false;
	}

};