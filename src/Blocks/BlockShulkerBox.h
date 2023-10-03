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

	virtual bool IsUseable() const override
	{
		return true;
	}

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		UNUSED(a_BlockMeta);
		UNUSED(a_Tool);
		return cItem();
	}
};
