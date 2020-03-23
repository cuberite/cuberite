
#pragma once

#include "BlockEntity.h"
#include "Mixins.h"





class cBlockJukeboxHandler :
	public cClearMetaOnDrop<cBlockEntityHandler>
{
	using super = cClearMetaOnDrop<cBlockEntityHandler>;

public:

	cBlockJukeboxHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 6;
	}
} ;




