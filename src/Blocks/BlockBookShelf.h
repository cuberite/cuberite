
#pragma once

#include "BlockHandler.h"




class cBlockBookShelfHandler :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType, 1);
		}
		return cItem(E_ITEM_BOOK, 3);
	}
};
