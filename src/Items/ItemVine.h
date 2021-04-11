#pragma once

#include "ItemHandler.h"





class cItemVineHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		BLOCKTYPE Block;
		NIBBLETYPE Meta;
		a_Player.GetWorld()->GetBlockTypeMeta(a_PlacePosition, Block, Meta);

		NIBBLETYPE PlaceMeta;
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_NORTH: PlaceMeta = 0x1; break;
			case BLOCK_FACE_SOUTH: PlaceMeta = 0x4; break;
			case BLOCK_FACE_WEST:  PlaceMeta = 0x8; break;
			case BLOCK_FACE_EAST:  PlaceMeta = 0x2; break;
			default: return false;
		}

		if (Block == E_BLOCK_VINES)
		{
			PlaceMeta |= Meta;
		}

		return a_Player.PlaceBlock(a_PlacePosition, E_BLOCK_VINES, PlaceMeta);
	}
};
