
#pragma once

#include "BlockTorch.h"





class cBlockRedstoneTorchHandler :
	public cBlockTorchHandler
{
	using super = cBlockTorchHandler;

public:

	cBlockRedstoneTorchHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Always drop the ON torch, meta 0
		return cItem(E_BLOCK_REDSTONE_TORCH_ON, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




