
#pragma once

#include "BlockHandler.h"





class cBlockNetherrack final : public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual bool CanSustainPlant(BLOCKTYPE a_Plant) const override
	{
		return a_Plant == E_BLOCK_NETHER_WART;
	}
};
