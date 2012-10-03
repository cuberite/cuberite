
#pragma once

#include "BlockHandler.h"
#include "../Vine.h"





class cBlockVineHandler :
	public cBlockHandler
{
public:
	cBlockVineHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual bool DoesIgnoreBuildCollision(void) override
	{
		return true;
	}
	
	
	virtual void PlaceBlock(cWorld * a_World, cPlayer * a_Player, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Dir) override
	{
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_BlockType, cVine::DirectionToMetaData(a_Dir));
		OnPlacedByPlayer(a_World, a_Player, a_BlockX, a_BlockY, a_BlockZ, a_Dir);
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




