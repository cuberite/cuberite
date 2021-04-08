
#pragma once

#include "BlockHandler.h"





class cBlockMelonHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		const auto DropNum = FortuneDiscreteRandom(3, 7, ToolFortuneLevel(a_Tool), 9);
		return cItem(Item::MelonSlice, DropNum);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 19;
	}
} ;




