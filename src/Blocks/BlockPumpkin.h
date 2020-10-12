#pragma once

#include "BlockHandler.h"
#include "Mixins.h"



class cBlockPumpkinHandler final :
	public cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>
{
	using Super = cClearMetaOnDrop<cYawRotator<cBlockHandler, 0x03, 0x00, 0x01, 0x02, 0x03>>;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 15;
	}
} ;




