
#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"




class cBlockEntityHandler : public cBlockHandler
{
public:
	cBlockEntityHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual bool OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		return a_ChunkInterface.UseBlockEntity(&a_Player, a_BlockX, a_BlockY, a_BlockZ);
	}

	virtual bool IsUseable() override
	{
		return true;
	}
};




