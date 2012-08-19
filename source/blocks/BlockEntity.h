
#pragma once

#include "Block.h"





class cBlockEntityHandler : public cBlockHandler
{
public:
	cBlockEntityHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	virtual void OnUse(cWorld * a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override
	{
		a_World->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ);
	}
	
	virtual bool IsUseable() override
	{
		return true;
	}
};




