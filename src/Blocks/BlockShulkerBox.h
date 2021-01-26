
#pragma once

#include "../BlockArea.h"
#include "../Entities/Player.h"
#include "BlockHandler.h"





class cBlockShulkerBoxHandler final :
	public cBlockEntityHandler
{
public:
	using cBlockEntityHandler::cBlockEntityHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem();
	}
};
