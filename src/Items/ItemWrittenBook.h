
// ItemWrittenBook.h

#pragma once

#include "ItemHandler.h"
#include "../Entities/Player.h"
#include "../Inventory.h"



class cItemWrittenBookHandler :
	public cItemHandler
{
public:
	cItemWrittenBookHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		// TODO: Currently only main haind. If second hand is implemented, fix this
		a_Player->GetClientHandle()->SendOpenBook(0);
		return true;
	}
} ;
