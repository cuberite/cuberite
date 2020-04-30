
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemNetherWartHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemNetherWartHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		// Only allow planting nether wart onto the top side of the block:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		// Only allow placement on soulsand
		if ((a_PlacedBlockPos.y < 1) || (a_World->GetBlock(a_PlacedBlockPos.addedY(-1)) != E_BLOCK_SOULSAND))
		{
			return false;
		}

		a_BlockMeta = 0;
		a_BlockType = E_BLOCK_NETHER_WART;
		return true;
	}
} ;
