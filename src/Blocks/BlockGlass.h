
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

	/** Returns true if the specified block type is one of the glass handled by this handler */
	static bool IsAnyGlassType(BLOCKTYPE a_BlockType)
	{
		return (
			(a_BlockType == E_BLOCK_GLASS) ||
			(a_BlockType == E_BLOCK_GLASS_PANE) ||
			(a_BlockType == E_BLOCK_STAINED_GLASS) ||
			(a_BlockType == E_BLOCK_STAINED_GLASS_PANE)
		);
	}
} ;




