#pragma once

#include "BlockHandler.h"





#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cBlockSlimeHandler:
	public cClearMetaOnDrop<cBlockHandler>
{
public:

	using cClearMetaOnDrop<cBlockHandler>::cClearMetaOnDrop;

private:

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 1;
	}
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
