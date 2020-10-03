
#pragma once

#include "BlockHandler.h"





#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cBlockNetherrack : public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual bool CanSustainPlant(BLOCKTYPE a_Plant) const override
	{
		return a_Plant == E_BLOCK_NETHER_WART;
	}
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
