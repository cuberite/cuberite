
#pragma once

#include "../Blocks/BlockPiston.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "Mixins.h"




class cBlockFurnaceHandler :
	public cYawRotator<cBlockEntityHandler, 0x07, 0x03, 0x04, 0x02, 0x05>
{
	using Super = cYawRotator<cBlockEntityHandler, 0x07, 0x03, 0x04, 0x02, 0x05>;

public:

	cBlockFurnaceHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		cItems res(cItem(E_BLOCK_FURNACE, 1));  // We can't drop a lit furnace
		if (a_BlockEntity != nullptr)
		{
			auto be = static_cast<cFurnaceEntity *>(a_BlockEntity);
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




