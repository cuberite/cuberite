
#pragma once

#include "BlockHandler.h"




class cBlockBookShelfHandler final :
	public cBlockHandler
{
public:
	using Super = cBlockHandler;

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(Item::Bookshelf, 1);
		}
		return cItem(Item::Book, 3);
	}
};
