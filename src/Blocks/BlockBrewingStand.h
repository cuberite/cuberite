
#pragma once

#include "../BlockEntities/BrewingstandEntity.h"
#include "Mixins.h"





class cBlockBrewingStandHandler :
	public cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>
{
	using super = cMetaRotator<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>;

public:

	cBlockBrewingStandHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		cItems res(cItem(E_ITEM_BREWING_STAND, 1)); // We have to drop the item form of a brewing stand
		if (a_BlockEntity != nullptr)
		{
			auto be = static_cast<cBrewingstandEntity *>(a_BlockEntity);
			res.AddItemGrid(be->GetContents());
		}
		return res;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 6;
	}
} ;




