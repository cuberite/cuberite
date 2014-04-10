#pragma once

#include "BlockHandler.h"





class cBlockCakeHandler :
	public cBlockHandler
{
public:
	cBlockCakeHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);

		if (!a_Player->Feed(2, 0.1))
		{
			return;
		}
		
		if (Meta >= 5)
		{
			a_ChunkInterface.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
		}
		else
		{
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, Meta + 1);
		}
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Give nothing
	}

	virtual bool IsUseable(void) override
	{
		return true;
	}

	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




