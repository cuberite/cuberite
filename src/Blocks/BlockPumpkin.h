#pragma once

#include "BlockHandler.h"
#include "Mixins.h"



class cBlockPumpkinHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID() const override
	{
		UNUSED(a_Meta);
		return 15;
	}
} ;




