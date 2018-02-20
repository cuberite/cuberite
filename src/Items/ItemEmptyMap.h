
#pragma once

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



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		UNUSED(a_Item);
		UNUSED(a_BlockX);
		UNUSED(a_BlockZ);
		UNUSED(a_BlockFace);

		// The map center is fixed at the central point of the 8x8 block of chunks you are standing in when you right-click it.

		const int RegionWidth = cChunkDef::Width * 8;

		int CenterX = FloorC(a_Player->GetPosX() / RegionWidth) * RegionWidth + (RegionWidth / 2);
		int CenterZ = FloorC(a_Player->GetPosZ() / RegionWidth) * RegionWidth + (RegionWidth / 2);

		cMap * NewMap = a_World->GetMapManager().CreateMap(CenterX, CenterZ, DEFAULT_SCALE);

		// Remove empty map from inventory
		if (!a_Player->GetInventory().RemoveOneEquippedItem())
		{
			ASSERT(!"Inventory mismatch");
			return true;
		}

		if (NewMap == nullptr)
		{
			return true;
		}

		a_Player->GetInventory().AddItem(cItem(E_ITEM_MAP, 1, static_cast<short>(NewMap->GetID() & 0x7fff)));

		return true;
	}
} ;
