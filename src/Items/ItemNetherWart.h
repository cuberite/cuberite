
#pragma once

#include "ItemHandler.h"
#include "../World.h"





class cItemNetherWartHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	cItemNetherWartHandler(Item a_ItemType):
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
		BlockState & a_Block
	) override
	{
		// Only allow planting nether wart onto the top side of the block:
		if (a_ClickedBlockFace != BLOCK_FACE_TOP)
		{
			return false;
		}

		// Only allow placement on soulsand
		if ((a_PlacedBlockPos.y < 1) || (a_World->GetBlock(a_PlacedBlockPos.addedY(-1)).Type() != BlockType::SoulSand))
		{
			return false;
		}

		a_Block = Block::NetherWart::NetherWart();
		return true;
	}
} ;
