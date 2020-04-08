
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"


class cBlockGlazedTerracottaHandler:
	public cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x3, 0x1, 0x3, 0x0, 0x2>>
{
	using super = cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x3, 0x1, 0x3, 0x0, 0x2>>;

public:

	cBlockGlazedTerracottaHandler(BLOCKTYPE a_BlockType) : super(a_BlockType)
	{
	}

};
