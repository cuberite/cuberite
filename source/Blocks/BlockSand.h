#pragma once
#include "BlockHandler.h"


class cBlockSandHandler : public cBlockHandler
{
public:
	cBlockSandHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}

	virtual AString GetStepSound(void) override
	{
		return "step.sand";
	}
	
};
