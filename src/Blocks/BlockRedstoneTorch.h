
#pragma once

#include "BlockRedstone.h"
#include "BlockTorch.h"





class cBlockRedstoneTorchHandler :
	public cBlockTorchHandler
{
public:
	cBlockRedstoneTorchHandler(BLOCKTYPE a_BlockType)
		: cBlockTorchHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Always drop the ON torch, meta 0
		a_Pickups.push_back(cItem(E_BLOCK_REDSTONE_TORCH_ON, 1, 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




