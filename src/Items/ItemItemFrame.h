
#pragma once

#include "ItemHandler.h"
#include "Entities/ItemFrame.h"
#include "../Entities/Player.h"





class cItemItemFrameHandler :
	public cItemHandler
{
public:
	cItemItemFrameHandler(int a_ItemType)
		: cItemHandler(a_ItemType)
	{

	}

	virtual bool OnItemUse(cWorld *a_World, cPlayer *a_Player, const cItem & a_Item, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Dir) override
	{
		if (a_Dir == BLOCK_FACE_NONE)
		{
			return false;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_Dir);
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_Dir, true);

		if (Block == E_BLOCK_AIR)
		{
			int Dir = 0;
			switch (a_Dir)
			{
				case BLOCK_FACE_SOUTH: break;
				case BLOCK_FACE_NORTH: Dir = 2; break;
				case BLOCK_FACE_WEST: Dir = 1; break;
				case BLOCK_FACE_EAST: Dir = 3; break;
				default: return false;
			}

			cItemFrame * ItemFrame = new cItemFrame(Dir, a_BlockX, a_BlockY, a_BlockZ);
			ItemFrame->Initialize(a_World);

			if (!a_Player->IsGameModeCreative())
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}

			return true;

		}
		return false;
	}
};




