
#pragma once

#include "ItemHandler.h"





class cItemSlabHandler:
	public cItemHandler
{
	typedef cItemHandler super;

public:

	/** Creates a new handler for the specified slab item type.
	Sets the handler to use the specified doubleslab block type for combining self into doubleslabs. */
	cItemSlabHandler(int a_ItemType, BLOCKTYPE a_DoubleSlabBlockType):
		super(a_ItemType),
		m_DoubleSlabBlockType(a_DoubleSlabBlockType)
	{
	}


	// cItemHandler overrides:
	virtual bool OnPlayerPlace(
		cWorld & a_World, cPlayer & a_Player, const cItem & a_EquippedItem,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ
	) override
	{
		// Special slab handling - placing a slab onto another slab produces a dblslab instead:
		BLOCKTYPE ClickedBlockType;
		NIBBLETYPE ClickedBlockMeta;
		a_World.GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, ClickedBlockType, ClickedBlockMeta);
		// If clicked on a half slab directly
		if (
			(ClickedBlockType == m_ItemType) &&                         // Placing the same slab material
			((ClickedBlockMeta & 0x07) == a_EquippedItem.m_ItemDamage)  // Placing the same slab sub-kind (and existing slab is single)
		)
		{
			// If clicking the top side of a bottom-half slab, combine into a doubleslab:
			if (
				(a_BlockFace == BLOCK_FACE_TOP) &&
				((ClickedBlockMeta & 0x08) == 0)
			)
			{
				if (!a_Player.PlaceBlock(a_BlockX, a_BlockY, a_BlockZ, m_DoubleSlabBlockType, ClickedBlockMeta & 0x07))
				{
					return false;
				}
				if (a_Player.IsGameModeSurvival())
				{
					a_Player.GetInventory().RemoveOneEquippedItem();
				}
				return true;
			}

			// If clicking the bottom side of a top-half slab, combine into a doubleslab:
			if (
				(a_BlockFace == BLOCK_FACE_BOTTOM) &&
				((ClickedBlockMeta & 0x08) != 0)
			)
			{
				if (!a_Player.PlaceBlock(a_BlockX, a_BlockY, a_BlockZ, m_DoubleSlabBlockType, ClickedBlockMeta & 0x07))
				{
					return false;
				}
				if (a_Player.IsGameModeSurvival())
				{
					a_Player.GetInventory().RemoveOneEquippedItem();
				}
				return true;
			}
		}

		// Checking the type of block that should be placed
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
		BLOCKTYPE PlaceBlockType;
		NIBBLETYPE PlaceBlockMeta;
		a_World.GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, PlaceBlockType, PlaceBlockMeta);
		// If it's a slab combine into a doubleslab (means that clicked on side, top or bottom of a block adjacent to a half slab)
		if (
			(PlaceBlockType == m_ItemType) &&                         // Placing the same slab material
			((PlaceBlockMeta & 0x07) == a_EquippedItem.m_ItemDamage)  // Placing the same slab sub-kind (and existing slab is single)
		)
		{
			if (!a_Player.PlaceBlock(a_BlockX, a_BlockY, a_BlockZ, m_DoubleSlabBlockType, PlaceBlockMeta & 0x07))
			{
				return false;
			}
			if (a_Player.IsGameModeSurvival())
			{
				a_Player.GetInventory().RemoveOneEquippedItem();
			}
			return true;
		}

		// The slabs didn't combine, use the default handler to place the slab:
		AddFaceDirection(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, true);
		bool res = super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);

		/*
		The client has a bug when a slab replaces snow and there's a slab above it.
		The client then combines the slab above, rather than replacing the snow.
		We send the block above the currently placed block back to the client to fix the bug.
		Ref.: https://forum.cuberite.org/thread-434-post-17388.html#pid17388
		*/
		if ((a_BlockFace == BLOCK_FACE_TOP) && (a_BlockY < cChunkDef::Height - 1))
		{
			a_Player.SendBlocksAround(a_BlockX, a_BlockY + 1, a_BlockZ, 1);
		}
		return res;
	}

protected:
	/** The block type to use when the slab combines into a doubleslab block. */
	BLOCKTYPE m_DoubleSlabBlockType;
};
