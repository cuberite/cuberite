
#pragma once

#include "BlockEntity.h"
#include "Mixins/Mixins.h"





class cBlockJukeboxHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;
public:

	using Super::Super;

private:

	virtual ColourID GetMapBaseColourID() const override
	{
		return 10;
	}
} ;
