
#pragma once

#include "BlockEntity.h"
#include "../BlockEntities/MobHeadEntity.h"





class cBlockMobHeadHandler :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		if ((a_BlockEntity == nullptr) || (a_BlockEntity->GetBlockType() != E_BLOCK_HEAD))
		{
			return {};
		}
		auto mobHeadEntity = static_cast<cMobHeadEntity *>(a_BlockEntity);
		return cItem(E_ITEM_HEAD, 1, static_cast<short>(mobHeadEntity->GetType()));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




