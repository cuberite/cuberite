
#pragma once

#include "ItemHandler.h"





class cItemSlabHandler:
	public cItemHandler
{
	using Super = cItemHandler;

public:

	/** Creates a new handler for the specified slab item type.
	Sets the handler to use the specified doubleslab block type for combining self into doubleslabs. */
	cItemSlabHandler(int a_ItemType, BLOCKTYPE a_DoubleSlabBlockType):
		Super(a_ItemType),
		m_DoubleSlabBlockType(a_DoubleSlabBlockType)
	{
	}





	// cItemHandler overrides:
	virtual bool OnPlayerPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos
	) override
	{
		// If clicking a slab, try combining it into a double-slab:
		BLOCKTYPE ClickedBlockType;
		NIBBLETYPE ClickedBlockMeta;
		a_World.GetBlockTypeMeta(a_ClickedBlockPos, ClickedBlockType, ClickedBlockMeta);
		if (
			(ClickedBlockType == m_ItemType) &&                         // Placing the same slab material
			((ClickedBlockMeta & 0x07) == a_EquippedItem.m_ItemDamage)  // Placing the same slab sub-kind (and existing slab is single)
		)
		{
			if (
				((a_ClickedBlockFace == BLOCK_FACE_TOP) && ((ClickedBlockMeta & 0x08) == 0)) ||  // Top side of a bottom-half-slab
				((a_ClickedBlockFace == BLOCK_FACE_BOTTOM) && ((ClickedBlockMeta & 0x08) != 0))  // Bottom side of a top-half-slab
			)
			{
				if (!a_Player.PlaceBlock(a_ClickedBlockPos.x, a_ClickedBlockPos.y, a_ClickedBlockPos.z, m_DoubleSlabBlockType, ClickedBlockMeta & 0x07))
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

		// If there's already a slab in the destination, combine it into a double-slab:
		auto PlacePos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace);
		BLOCKTYPE PlaceBlockType;
		NIBBLETYPE PlaceBlockMeta;
		a_World.GetBlockTypeMeta(PlacePos, PlaceBlockType, PlaceBlockMeta);
		if (
			(PlaceBlockType == m_ItemType) &&                         // Placing the same slab material
			((PlaceBlockMeta & 0x07) == a_EquippedItem.m_ItemDamage)  // Placing the same slab sub-kind (and existing slab is single)
		)
		{
			if (!a_Player.PlaceBlock(PlacePos.x, PlacePos.y, PlacePos.z, m_DoubleSlabBlockType, PlaceBlockMeta & 0x07))
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
		bool res = Super::OnPlayerPlace(a_World, a_Player, a_EquippedItem, a_ClickedBlockPos, a_ClickedBlockFace, a_CursorPos);

		/*
		The client has a bug when a slab replaces snow and there's a slab above it.
		The client then combines the slab above, rather than replacing the snow.
		We send the block above the currently placed block back to the client to fix the bug.
		Ref.: https://forum.cuberite.org/thread-434-post-17388.html#pid17388
		*/
		if ((a_ClickedBlockFace == BLOCK_FACE_TOP) && (a_ClickedBlockPos.y < cChunkDef::Height - 1))
		{
			auto AbovePos = a_ClickedBlockPos.addedY(1);
			a_Player.SendBlocksAround(AbovePos.x, AbovePos.y, AbovePos.z, 1);
		}
		return res;
	}


protected:

	/** The block type to use when the slab combines into a doubleslab block. */
	BLOCKTYPE m_DoubleSlabBlockType;
};
