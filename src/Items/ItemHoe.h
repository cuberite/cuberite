
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"





class cItemHoeHandler :
	public cItemHandler
{
public:
	cItemHoeHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{
	}

	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		if ((a_Dir == BLOCK_FACE_NONE) || (a_BlockY >= cChunkDef::Height))
		{
			return false;
		}
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		BLOCKTYPE UpperBlock = a_World->GetBlock(a_BlockX, a_BlockY + 1, a_BlockZ);

		if (((Block == E_BLOCK_DIRT) || (Block == E_BLOCK_GRASS)) && (UpperBlock == E_BLOCK_AIR))
		{
			a_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_FARMLAND, 0);
			a_World->BroadcastSoundEffect("dig.gravel", a_BlockX + 0.5, a_BlockY + 0.5, a_BlockZ + 0.5, 1.0f, 0.8f);
			a_Player->UseEquippedItem();
			return true;
		}

		return false;
	}


	virtual short GetDurabilityLossByAction(eDurabilityLostAction a_Action) override
	{
		return 0;
	}
} ;




