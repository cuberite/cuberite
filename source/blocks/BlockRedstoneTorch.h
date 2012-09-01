
#pragma once
#include "BlockRedstone.h"
#include "BlockTorch.h"
#include "../cTorch.h"





class cBlockRedstoneTorchHandler : public cBlockTorchHandler
{
public:
	cBlockRedstoneTorchHandler(BLOCKTYPE a_BlockID)
		: cBlockTorchHandler(a_BlockID)
	{
	}

	virtual int GetDropID(void) override
	{
		return E_ITEM_REDSTONE_TORCH_ON;
	}
};