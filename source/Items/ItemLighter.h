
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Player.h"





class cItemLighterHandler :
	public cItemHandler
{
public:
	cItemLighterHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}

	virtual bool OnItemUse(cWorld * a_World, cPlayer * a_Player, cItem * a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace) override
	{
		if (a_BlockFace < 0)
		{
			return false;
		}
		
		a_Player->UseEquippedItem();

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);

		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FIRE, 0);	//0 -> new fire TODO: Make Firesimulator use this

		return false;
	}
} ;




