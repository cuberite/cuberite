
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockBed.h"





class cItemBedHandler :
	public cItemHandler
{
public:
	cItemBedHandler(int a_ItemType) :
		cItemHandler(a_ItemType)
	{
	}


	virtual bool IsPlaceable(void) override
	{
		return true;
	}


	virtual bool GetBlocksToPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		sSetBlockVector & a_BlocksToPlace
	) override
	{
		// Can only be placed on the floor:
		if (a_BlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		// The "foot" block:
		NIBBLETYPE BlockMeta = cBlockBedHandler::RotationToMetaData(a_Player.GetYaw());
		a_BlocksToPlace.emplace_back(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_BED, BlockMeta);

		// Check if there is empty space for the "head" block:
		// (Vanilla only allows beds to be placed into air)
		Vector3i Direction = cBlockBedHandler::MetaDataToDirection(BlockMeta);
		if (a_World.GetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z) != E_BLOCK_AIR)
		{
			return false;
		}
		a_BlocksToPlace.emplace_back(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z, E_BLOCK_BED, BlockMeta | 0x08);
		return true;
	}
} ;




