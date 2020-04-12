
#pragma once

#include "ItemHandler.h"



class cItemCactusHandler :
	public cItemHandler
{
public:
	cItemCactusHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}


	virtual bool OnPlayerPlace(
        cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
        int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
        int a_CursorX, int a_CursorY, int a_CursorZ
    ) 
	{
		/** We will use the block face and the block coordinates to determine if we can place the cactus
		There must be sand or cactus under the cactus and no blocks on the sides inorder for it to be placed
		*/
		Vector3i PlacementPos = GetBlockNextTo(Vector3i(a_BlockX, a_BlockY, a_BlockZ), a_BlockFace);

		BLOCKTYPE Surface = a_World.GetBlock(PlacementPos - Vector3i(0, 1, 0));
		if ((Surface != E_BLOCK_SAND) && (Surface != E_BLOCK_CACTUS))
		{
			// Cactus can only be placed on sand and itself
			return false;
		}

		const Vector3i Offsets[4] = 
		{
			{1, 0, 0},
			{-1, 0, 0},
			{0, 0, 1},
			{0, 0, -1}
		};

		for(int i = 0; i < ARRAYCOUNT(Offsets); i++) 
		{
			BLOCKTYPE Surrounding = a_World.GetBlock(PlacementPos + Offsets[i]);
			if(cBlockInfo::IsSolid(Surrounding) || IsBlockLava(Surrounding))
			{
				return false;
			}
		}

		return cItemHandler::OnPlayerPlace(a_World, a_Player, a_EquippedItem, 
			a_BlockX, a_BlockY, a_BlockZ, a_BlockFace,
			a_CursorX, a_CursorY, a_CursorZ
		);
	}
} ;




