
#pragma once

#include "BlockHandler.h"





class cBlockDirtHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return 10;
	}
} ;




