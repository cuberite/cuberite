#pragma once
#include "Block.h"


class cBlockVineHandler : public cBlockHandler
{
public:
	cBlockVineHandler(BLOCKTYPE a_BlockID);
	
	virtual bool IgnoreBuildCollision()
	{
		return true;
	}
	
	virtual void PlaceBlock(cWorld *a_World, cPlayer *a_Player, char a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir);
	
	
	virtual bool AllowBlockOnTop()
	{
		return false;
	}
	
};