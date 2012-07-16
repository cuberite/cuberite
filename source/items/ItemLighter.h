#pragma once

#include "Item.h"
#include "../cWorld.h"
#include "../cPlayer.h"

class cItemLighterHandler : public cItemHandler
{
public:
	cItemLighterHandler(int a_ItemID)
		: cItemHandler(a_ItemID)
	{

	}

	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, cItem *a_Item, int a_X, int a_Y, int a_Z, char a_Dir) override
	{
		a_Player->UseEquippedItem();

		AddDirection(a_X, a_Y, a_Z, a_Dir);

		a_World->SetBlock(a_X, a_Y, a_Z, E_BLOCK_FIRE, 0);	//0 -> new fire TODO: Make Firesimulator use this

		return false;
	}

};