
#pragma once

#include "BlockHandler.h"
#include "../World.h"





class cBlockIceHandler :
	public cBlockHandler
{
public:
	cBlockIceHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
	}
	
	
	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		// TODO: Ice destroyed with air below it should turn into air instead of water
		a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_WATER, 0);
		// This is called later than the real destroying of this ice block
	}
} ;




