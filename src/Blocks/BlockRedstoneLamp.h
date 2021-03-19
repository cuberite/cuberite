
#pragma once

#include "BlockHandler.h"





class cBlockRedstoneLampHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual ColourID GetMapBaseColourID() const override
	{
		return 15;
	}
};




