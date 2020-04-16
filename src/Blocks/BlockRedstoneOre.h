
#pragma once

#include "BlockHandler.h"
#include "BlockOre.h"





class cBlockRedstoneOreHandler :
	public cBlockOreHandler
{
	using Super = cBlockOreHandler;
public:

	using Super::Super;

	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ,
		eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ
	) override
	{
		Vector3i BlockPos{a_BlockX, a_BlockY, a_BlockZ};
		a_ChunkInterface.SetBlock(BlockPos, E_BLOCK_REDSTONE_ORE_GLOWING, 0);
		return false;
	}

	virtual void OnDigging(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ
	) override
	{
		Vector3i BlockPos{a_BlockX, a_BlockY, a_BlockZ};
		a_ChunkInterface.SetBlock(BlockPos, E_BLOCK_REDSTONE_ORE_GLOWING, 0);
	}

	virtual bool IsUseable() override
	{
		return true;
	}
};





class cBlockGlowingRedstoneOreHandler:
	public cBlockOreHandler
{
	using Super = cBlockOreHandler;
public:

	using Super::Super;

	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_BlockPluginInterface,
		cChunk & a_Chunk,
		int a_RelX, int a_RelY, int a_RelZ
	) override
	{
		const Vector3i a_RelPos{a_RelX, a_RelY, a_RelZ};
		auto BlockPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		a_ChunkInterface.SetBlock(BlockPos, E_BLOCK_REDSTONE_ORE, 0);
	}
};





