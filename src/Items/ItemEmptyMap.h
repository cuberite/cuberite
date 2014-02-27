
// ItemEmptyMap.h





#pragma once

#include "../Entities/Entity.h"
#include "../Item.h"





class cItemEmptyMapHandler :
	public cItemHandler
{
	typedef cItemHandler super;

	static const unsigned int DEFAULT_SCALE = 0;
	
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

		int CenterX = (int)(floor(a_Player->GetPosX() / (float) RegionWidth) * RegionWidth);
		int CenterZ = (int)(floor(a_Player->GetPosZ() / (float) RegionWidth) * RegionWidth);

		cMap * NewMap = a_World->GetMapManager().CreateMap(CenterX, CenterZ, DEFAULT_SCALE);

		// Remove empty map from inventory
		if (!a_Player->GetInventory().RemoveOneEquippedItem())
		{
			ASSERT(!"Inventory mismatch");
			return true;
		}

		if (NewMap == NULL)
		{
			return true;
		}

		a_Player->GetInventory().AddItem(cItem(E_ITEM_MAP, 1, NewMap->GetID()), true, true);

		return true;
	}
} ;
