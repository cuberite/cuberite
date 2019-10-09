
#pragma once

#include "BlockEntity.h"





class cBlockCommandBlockHandler :
	public cBlockEntityHandler
{
	using super = cBlockEntityHandler;

public:

	cBlockCommandBlockHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Don't allow as a pickup:
		return {};
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 10;
	}
} ;




