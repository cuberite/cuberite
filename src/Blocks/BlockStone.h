
#pragma once

#include "BlockHandler.h"




class cBlockStoneHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Convert stone to cobblestone, unless using silk-touch:
		if (
			(a_BlockMeta == E_META_STONE_STONE) &&
			!ToolHasSilkTouch(a_Tool)
		)
		{
			return cItem(E_BLOCK_COBBLESTONE, 1, 0);
		}
		return cItem(m_BlockType, 1, a_BlockMeta);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 11;
	}
};




