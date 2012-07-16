#pragma once
#include "Block.h"


class cBlockFluidHandler : public cBlockHandler
{
public:
	cBlockFluidHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{

	}
	
	virtual bool IgnoreBuildCollision() override
	{
		return true;
	}
	
	
};