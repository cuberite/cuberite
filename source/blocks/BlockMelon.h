#pragma once
#include "BlockHandler.h"


class cBlockMelonHandler : public cBlockHandler
{
public:
	cBlockMelonHandler(BLOCKTYPE a_BlockID)
		: cBlockHandler(a_BlockID)
	{
	}
	
	
	virtual int GetDropID() override
	{
		return E_ITEM_MELON_SLICE;
	}

	virtual char GetDropCount() override
	{
		MTRand r1;
		return (char)(3 + r1.randInt(4));
	}

	virtual AString GetStepSound(void) override
	{
		return "step.wood";
	}
};
