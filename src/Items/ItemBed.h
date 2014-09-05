
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

	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		if (a_BlockFace != BLOCK_FACE_TOP)
		{
			// Can only be placed on the floor
			return false;
		}

		a_BlockMeta = cBlockBedHandler::RotationToMetaData(a_Player->GetYaw());
		
		// Check if there is empty space for the foot section:
		Vector3i Direction = cBlockBedHandler::MetaDataToDirection(a_BlockMeta);
		if (a_World->GetBlock(a_BlockX + Direction.x, a_BlockY, a_BlockZ + Direction.z) != E_BLOCK_AIR)
		{
			return false;
		}

		a_BlockType = E_BLOCK_BED;
		return true;
	}
} ;




