
#pragma once

#include "BlockHandler.h"





class cBlockTNTHandler :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	cBlockTNTHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual void OnCancelRightClick(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace
	) override
	{
		a_WorldInterface.SendBlockTo(a_BlockPos, a_Player);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 4;
	}
};




