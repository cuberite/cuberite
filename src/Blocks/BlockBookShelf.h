
#pragma once

#include "BlockHandler.h"




class cBlockBookShelfHandler :
	public cBlockHandler
{
	public:
		cBlockBookShelfHandler(BLOCKTYPE a_BlockType) :
			cBlockHandler(a_BlockType)
		{
		}

		virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
		{
			if ((ItemCategory::IsTool(a_Tool->m_ItemType)) && (ToolHasSilkTouch(a_Tool)))
			{
				return cItem(m_BlockType, 1);
			}
			return cItem(E_ITEM_BOOK, 3);
		}
};
