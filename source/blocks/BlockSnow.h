#pragma once
#include "Block.h"


class cBlockSnowHandler : public cBlockHandler
{
public:
	cBlockSnowHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	virtual bool IgnoreBuildCollision() override
	{
		return true;
	}

	virtual int GetDropID() override
	{
		return E_ITEM_SNOWBALL;
	}

	virtual char GetDropCount() override
	{
		return 4;
	}

	
	virtual bool CanBeAt(cWorld *a_World, int a_X, int a_Y, int a_Z) override
	{
		return a_World->GetBlock(a_X, a_Y - 1, a_Z) != E_BLOCK_AIR;
	}
	
	virtual bool DropOnUnsuitable() override
	{
		return false;
	}
	
};