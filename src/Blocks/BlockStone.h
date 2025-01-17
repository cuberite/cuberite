
#pragma once

#include "BlockHandler.h"




class cBlockStoneHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Convert stone to cobblestone, unless using silk-touch:
		if (!ToolHasSilkTouch(a_Tool))
		{
			return cItem(Item::Cobblestone);
		}
		return cItem(Item::Stone);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 11;
	}
};




