
#pragma once

#include "BlockTorch.h"





#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cBlockRedstoneTorchHandler :
	public cBlockTorchHandler
{
	using Super = cBlockTorchHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Always drop the ON torch, meta 0
		return cItem(E_BLOCK_REDSTONE_TORCH_ON, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;

#ifdef __clang__
#pragma clang diagnostic pop
#endif




