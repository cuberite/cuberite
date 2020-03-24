
#pragma once

#include "BlockEntity.h"
#include "../BlockEntities/MobHeadEntity.h"





class cBlockMobHeadHandler :
	public cBlockEntityHandler
{
	using super = cBlockEntityHandler;

public:
	cBlockMobHeadHandler(BLOCKTYPE a_BlockType):
		super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
	{
		if ((a_BlockEntity == nullptr) || (a_BlockEntity->GetBlockType() != E_BLOCK_HEAD))
		{
			return {};
		}
		auto mobHeadEntity = static_cast<cMobHeadEntity *>(a_BlockEntity);
		return cItem(E_ITEM_HEAD, 1, static_cast<short>(mobHeadEntity->GetType()));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




