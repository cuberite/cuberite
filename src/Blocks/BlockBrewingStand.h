
#pragma once

#include "BlockEntity.h"
#include "Mixins.h"





class cBlockBrewingStandHandler :
	public cClearMetaOnDrop<cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>>
{
	using super = cClearMetaOnDrop<cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>>;

public:

	cBlockBrewingStandHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 6;
	}
} ;




