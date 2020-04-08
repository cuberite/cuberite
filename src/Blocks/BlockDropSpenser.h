
// BlockDropSpenser.h

// Declares the cBlockDropSpenserHandler class representing the BlockHandler for Dropper and Dispenser blocks

#pragma once

#include "../Blocks/BlockPiston.h"
#include "../BlockEntities/DropSpenserEntity.h"
#include "Mixins.h"





class cBlockDropSpenserHandler :
	public cPitchYawRotator<cBlockEntityHandler>
{
	using super = cPitchYawRotator<cBlockEntityHandler>;

public:

	cBlockDropSpenserHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		cItems res(cItem(m_BlockType, 1));
		if (a_BlockEntity != nullptr)
		{
			auto be = static_cast<cDropSpenserEntity *>(a_BlockEntity);
			res.AddItemGrid(be->GetContents());
		}
		return res;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 11;
	}
} ;




