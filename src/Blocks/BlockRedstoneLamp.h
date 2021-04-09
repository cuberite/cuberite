
#pragma once

#include "BlockHandler.h"





class cBlockRedstoneLampHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return 15;
	}
};




