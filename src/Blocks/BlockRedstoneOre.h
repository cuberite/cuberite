
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
		a_ChunkInterface.FastSetBlock(a_BlockPos, Block::RedstoneOre::RedstoneOre(!Block::RedstoneOre::Lit(a_ChunkInterface.GetBlock(a_BlockPos))));
		return false;
	}





	virtual void OnDigging(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos
	) const override
	{
		a_ChunkInterface.FastSetBlock(a_BlockPos, Block::RedstoneOre::RedstoneOre(!Block::RedstoneOre::Lit(a_ChunkInterface.GetBlock(a_BlockPos))));
	}





	virtual bool IsUseable() const override
	{
		return true;
	}
};
