
#pragma once

#include "BlockHandler.h"




class cBlockBookShelfHandler final :
	public cBlockHandler
{
public:

	using Super = cBlockHandler

	using Super::Super;

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
