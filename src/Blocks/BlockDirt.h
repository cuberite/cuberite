
#pragma once

#include "BlockHandler.h"





class cBlockDirtHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID() const override
	{
		return 10;
	}
} ;




