
#pragma once

#include "BlockHandler.h"





class cBlockMyceliumHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	// TODO: Add Mycel Spread

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		return cItem(E_BLOCK_DIRT, 1, 0);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 24;
	}
} ;




