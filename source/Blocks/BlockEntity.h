
#pragma once

#include "BlockHandler.h"





class cBlockEntityHandler : public cBlockHandler
{
public:
	cBlockEntityHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void OnUse(cWorld * a_World, cPlayer *a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		a_World->UseBlockEntity(a_Player, a_BlockX, a_BlockY, a_BlockZ);
	}
	
	virtual bool IsUseable() override
	{
		return true;
	}
};




