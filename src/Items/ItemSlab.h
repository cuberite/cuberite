
#pragma once

#include "ItemHandler.h"





class cItemSlabHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	using Super::Super;

private:

	static NIBBLETYPE FaceToMetaData(const NIBBLETYPE a_BaseMeta, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition)
	{
		switch (a_ClickedBlockFace)
		{
			case BLOCK_FACE_TOP:
			{
				// Bottom half slab block:
				return a_BaseMeta & 0x07;
			}
			case BLOCK_FACE_BOTTOM:
			{
				// Top half slab block:
				return a_BaseMeta | 0x08;
			}
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			case BLOCK_FACE_WEST:
			{
				if (a_CursorPosition.y > 7)
				{
					// Cursor at top half of block, place top slab:
					return a_BaseMeta | 0x08;
				}
				else
				{
					// Cursor at bottom half of block, place bottom slab:
					return a_BaseMeta & 0x07;
				}
			}
			default: UNREACHABLE("Unhandled block face");
		}
	}

	/** Converts the single-slab blocktype to its equivalent double-slab blocktype. */
	static BLOCKTYPE GetDoubleSlabType(BLOCKTYPE a_SingleSlabBlockType)
	{
		switch (a_SingleSlabBlockType)
		{
			case E_BLOCK_STONE_SLAB: return E_BLOCK_DOUBLE_STONE_SLAB;
			case E_BLOCK_WOODEN_SLAB: return E_BLOCK_DOUBLE_WOODEN_SLAB;
			case E_BLOCK_RED_SANDSTONE_SLAB: return E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB;
			case E_BLOCK_PURPUR_SLAB: return E_BLOCK_PURPUR_DOUBLE_SLAB;
		}
		UNREACHABLE("Unhandled slab type");
	}


	virtual bool OnPlacementCommit(cPlayer & a_Player, const cItem & a_HeldItem, const Vector3i a_PlacePosition, const eBlockFace a_ClickedBlockFace, const Vector3i a_CursorPosition) override
	{
		// Confer BlockSlab.h, which we're in cahoots with to make the below logic work.

		// If clicking a slab, combine it into a double-slab:
		if (cBlockSlabHandler::IsAnySlabType(a_Player.GetWorld()->GetBlock(a_PlacePosition)))
		{
			if (!a_Player.PlaceBlock(a_PlacePosition, GetDoubleSlabType(static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType)), static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage)))
			{
				return false;
			}

			a_Player.SendBlocksAround(a_PlacePosition.x, a_PlacePosition.y, a_PlacePosition.z, 2);  // (see below)
			return true;
		}

		// Set the correct metadata based on player equipped item:
		if (!a_Player.PlaceBlock(a_PlacePosition, static_cast<BLOCKTYPE>(a_HeldItem.m_ItemType), FaceToMetaData(static_cast<NIBBLETYPE>(a_HeldItem.m_ItemDamage), a_ClickedBlockFace, a_CursorPosition)))
		{
			return false;
		}

		/* This is a workaround for versions < 1.13, where the client combines a slab in the
		direction of the clicked block face of a block ignoring build collision, rather than replacing said block.
		Resend blocks to the client to fix the bug.
		Ref.: https://forum.cuberite.org/thread-434-post-17388.html#pid17388 */
		a_Player.SendBlocksAround(a_PlacePosition.x, a_PlacePosition.y, a_PlacePosition.z, 2);

		return true;
	}
};
