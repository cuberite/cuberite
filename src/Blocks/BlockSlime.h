#pragma once

#include "BlockHandler.h"





class cBlockSlimeHandler final :
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
