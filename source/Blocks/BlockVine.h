
#pragma once

#include "BlockHandler.h"
#include "../Vine.h"





class cBlockVineHandler :
	public cBlockHandler
{
public:
	cBlockVineHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}


	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	
	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		a_World->SetBlock(a_X, a_Y, a_Z, m_BlockID, cVine::DirectionToMetaData(a_Dir));
		OnPlacedByPlayer(a_World, a_Player, a_X, a_Y, a_Z, a_Dir);
	}
	
	
	virtual bool DoesAllowBlockOnTop(void) override
	{
		return false;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.grass";
	}
} ;




