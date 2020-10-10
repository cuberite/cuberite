
#pragma once

#include "BlockHandler.h"
#include "BlockOre.h"





class cBlockRedstoneOreHandler final :
	public cBlockOreHandler
{
	using Super = cBlockOreHandler;

public:

	using Super::Super;  // Inherit constructor from base





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		a_ChunkInterface.SetBlock(a_BlockPos, E_BLOCK_REDSTONE_ORE_GLOWING, 0);
		return false;
	}





	virtual void OnDigging(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos
	) const override
	{
		a_ChunkInterface.SetBlock(a_BlockPos, E_BLOCK_REDSTONE_ORE_GLOWING, 0);
	}





	virtual bool IsUseable() const override
	{
		return true;
	}
};





class cBlockGlowingRedstoneOreHandler final :
	public cBlockOreHandler
{
	using Super = cBlockOreHandler;

public:

	using Super::Super;  // Inherit constructor from base





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_BlockPluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		auto BlockPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		a_ChunkInterface.SetBlock(BlockPos, E_BLOCK_REDSTONE_ORE, 0);
	}
};





