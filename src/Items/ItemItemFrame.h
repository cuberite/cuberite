
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



	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		if ((a_BlockFace == BLOCK_FACE_NONE) || (a_BlockFace == BLOCK_FACE_YP) || (a_BlockFace == BLOCK_FACE_YM))
		{
			// Client sends this if clicked on top or bottom face
			return false;
		}

		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);  // Make sure block that will be occupied is free
		BLOCKTYPE Block = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);  // We want the clicked block, so go back again

		if (Block == E_BLOCK_AIR)
		{
			auto ItemFrame = cpp14::make_unique<cItemFrame>(a_BlockFace, a_BlockX, a_BlockY, a_BlockZ);
			auto ItemFramePtr = ItemFrame.get();
			if (!ItemFramePtr->Initialize(std::move(ItemFrame), *a_World))
			{
				return false;
			}

			if (!a_Player->IsGameModeCreative())
			{
				a_Player->GetInventory().RemoveOneEquippedItem();
			}

			return true;

		}
		return false;
	}
};




