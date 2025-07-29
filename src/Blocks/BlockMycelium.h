
#pragma once

#include "BlockHandler.h"





class cBlockMyceliumHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	// TODO: Add Mycel Spread

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::Dirt);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 24;
	}
} ;




