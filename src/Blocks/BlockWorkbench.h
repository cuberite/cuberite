
#pragma once

#include "BlockHandler.h"
#include "../UI/Window.h"
#include "../Entities/Player.h"





class cBlockWorkbenchHandler:
	public cBlockHandler
{
public:
	cBlockWorkbenchHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		cWindow * Window = new cCraftingWindow(a_BlockX, a_BlockY, a_BlockZ);
		a_Player->OpenWindow(Window);
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




