
#pragma once

#include "BlockHandler.h"





class cBlockRedstoneLampHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		// Always drop the Off variant:
		return(cItem(E_BLOCK_REDSTONE_LAMP_OFF, 1, 0));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 15;
	}
};




