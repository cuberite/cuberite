
#pragma once

#include "BlockHandler.h"





class cBlockDirtHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		if (a_BlockMeta == E_META_DIRT_COARSE)
		{
			// Drop the coarse block (dirt, meta 1)
			return cItem(E_BLOCK_DIRT, 1, E_META_DIRT_COARSE);
		}
		else
		{
			return cItem(E_BLOCK_DIRT, 1, E_META_DIRT_NORMAL);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 10;
	}
} ;




