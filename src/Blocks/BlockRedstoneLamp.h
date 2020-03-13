
#pragma once

#include "BlockHandler.h"





class cBlockRedstoneLampHandler:
	public cBlockHandler
{
public:
	cBlockRedstoneLampHandler(BLOCKTYPE a_BlockType):
		cBlockHandler(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Always drop the Off variant:
		return(cItem(E_BLOCK_REDSTONE_LAMP_OFF, 1, 0));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 15;
	}
};




