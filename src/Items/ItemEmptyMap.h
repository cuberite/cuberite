
// ItemEmptyMap.h





#pragma once

#include "../Entities/Entity.h"
#include "../Item.h"





class cItemEmptyMapHandler :
	public cItemHandler
{
	typedef cItemHandler super;
	
public:
	cItemEmptyMapHandler() :
		super(E_ITEM_EMPTY_MAP)
	{
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		UNUSED(a_Item);
		UNUSED(a_BlockX);
		UNUSED(a_BlockZ);
		UNUSED(a_Dir);

		// The map center is fixed at the central point of the 8x8 block of chunks you are standing in when you right-click it.

		const int RegionWidth = cChunkDef::Width * 8;

		int CenterX = round(a_Player->GetPosX() / (float) RegionWidth) * RegionWidth;
		int CenterZ = round(a_Player->GetPosZ() / (float) RegionWidth) * RegionWidth;

		a_World->CreateMap(CenterX, CenterZ, 0);

		return true;
	}
} ;
