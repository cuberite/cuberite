
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"


class cBlockObserverHandler:
	public cClearMetaOnDrop<cPitchYawRotator<cBlockHandler>>
{
	using super = cClearMetaOnDrop<cPitchYawRotator<cBlockHandler>>;

public:

	cBlockObserverHandler(BLOCKTYPE a_BlockType) : super(a_BlockType)
	{
	}

};
