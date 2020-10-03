
#pragma once

#include "BlockHandler.h"




#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

class cBlockBookShelfHandler :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType, 1);
		}
		return cItem(E_ITEM_BOOK, 3);
	}
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif
