
#pragma once

#include "BlockHandler.h"





class cBlockTNTHandler :
	public cBlockHandler
{
public:
	cBlockTNTHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual cStepSound GetSound(void) override
	{
		return cStepSound("grass");
	}

	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	}
};




