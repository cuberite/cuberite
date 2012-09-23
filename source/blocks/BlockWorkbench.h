#pragma once
#include "Block.h"
#include "../UI/Window.h"
#include "../cPlayer.h"





class cBlockWorkbenchHandler:
	public cBlockHandler
{
public:
	cBlockWorkbenchHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	virtual void OnUse(cWorld * a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		cWindow * Window = new cCraftingWindow(a_BlockX, a_BlockY, a_BlockZ);
		a_Player->OpenWindow(Window);
	}

	virtual bool IsUseable() override
	{
		return true;
	}

	virtual AString GetStepSound(void) override
	{
		return "step.wood";
	}
	
	
};