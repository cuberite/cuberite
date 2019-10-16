
#pragma once

#include "BlockHandler.h"





class cBlockGlassHandler :
	public cBlockHandler
{
public:
	cBlockGlassHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		// Only drop self when mined with silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType, 1, a_BlockMeta);
		}
		return {};
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




