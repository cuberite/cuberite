#pragma once

#include "BlockHandler.h"
#include "Mixins.h"



class cBlockPumpkinHandler :
	public cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>
{
	using Super = cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>;
public:

	cBlockPumpkinHandler(BLOCKTYPE a_BlockType) :
		Super(a_BlockType)
	{
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 15;
	}
} ;




