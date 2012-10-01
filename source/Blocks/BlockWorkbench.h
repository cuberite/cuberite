
#pragma once

#include "BlockHandler.h"
#include "../UI/Window.h"
#include "../Player.h"





class cBlockWorkbenchHandler:
	public cBlockHandler
{
public:
	cBlockWorkbenchHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	
	virtual void OnUse(cWorld * a_World, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
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




