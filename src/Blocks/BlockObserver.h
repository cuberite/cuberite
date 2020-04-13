
#pragma once

#include "BlockHandler.h"
#include "Mixins.h"


class cBlockObserverHandler:
	public cClearMetaOnDrop<cPitchYawRotator<cBlockHandler>>
{
	using Super = cClearMetaOnDrop<cPitchYawRotator<cBlockHandler>>;

public:

	cBlockObserverHandler(BLOCKTYPE a_BlockType) : Super(a_BlockType)
	{
	}

};
