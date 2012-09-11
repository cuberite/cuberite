#pragma once
#include "Block.h"


class cBlockFireHandler : public cBlockHandler
{
public:
	cBlockFireHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual void OnDigging(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z) override
	{
		a_World->DigBlock(a_X, a_Y, a_Z);
	}

	virtual char GetDropCount() override
	{
		return -1;
	}

	virtual bool IsClickedThrough() override
	{
		return true;
	}
	
	virtual AString GetStepSound(void) override
	{
		return "step.wood";
	}

};
