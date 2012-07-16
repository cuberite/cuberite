#pragma once
#include "Block.h"
#include "../cCraftingWindow.h"
#include "../cPlayer.h"

class cBlockWorkbenchHandler : public cBlockHandler
{
public:
	cBlockWorkbenchHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	virtual void OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z) override
	{
		cWindow* Window = new cCraftingWindow(0, true);
		a_Player->OpenWindow(Window);
	}

	virtual bool IsUseable() override
	{
		return true;
	}
	
	
};