#pragma once

#include "ItemHandler.h"
#include "Blocks/BlockSideways.h"





/** Handler for blocks that have 3 orientations (hay bale, log), specified by the upper 2 bits in meta.
Handles setting the correct orientation on placement.
Additionally supports the metadata specifying block sub-type in its lower 2 bits. */
class cItemSidewaysHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	static NIBBLETYPE BlockFaceToMetaData(eBlockFace a_BlockFace, NIBBLETYPE a_Meta)
	{
		switch (a_BlockFace)
		{
			case BLOCK_FACE_YM:
			case BLOCK_FACE_YP:
			{
				return a_Meta;  // Top or bottom, just return original.
			}
			case BLOCK_FACE_ZP:
			case BLOCK_FACE_ZM:
			{
				return a_Meta | 0x8;  // North or south.
			}
			case BLOCK_FACE_XP:
			case BLOCK_FACE_XM:
			{
				return a_Meta | 0x4;  // East or west.
			}
			default: UNREACHABLE("Unsupported block face");
		}
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		return a_Player.PlaceBlock(a_PlacePosition, static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType), BlockFaceToMetaData(a_ClickedBlockFace, static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage)));
	}
};
