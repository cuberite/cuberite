#pragma once
#include "Block.h"


class cBlockEntityHandler : public cBlockHandler
{
public:
	cBlockEntityHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	virtual void OnClick(cWorld *a_World, cPlayer *a_Player, int a_X, int a_Y, int a_Z) override
	{
		a_World->UseBlockEntity(a_Player, a_X, a_Y, a_Z);
	}
	virtual bool IsUseable() override
	{
		return true;
	}
	
	
};