
// BlockSlab.h

// Declares cBlockSlabHandler and cBlockDoubleSlabHandler classes





#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../BlockInfo.h"
#include "../Entities/Player.h"
#include "../BlockInfo.h"



class cBlockSlabHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	/** Returns true if the specified blocktype is one of the slabs handled by this handler */
	static bool IsAnySlabType(BLOCKTYPE a_BlockType)
	{
		return (
			(a_BlockType == E_BLOCK_WOODEN_SLAB) ||
			(a_BlockType == E_BLOCK_STONE_SLAB) ||
			(a_BlockType == E_BLOCK_RED_SANDSTONE_SLAB) ||
			(a_BlockType == E_BLOCK_PURPUR_SLAB)
		);
	}

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Reset the "top half" flag:
		return cItem(m_BlockType, 1, a_BlockMeta & 0x07);
	}


	virtual bool DoesIgnoreBuildCollision(const cWorld & a_World, const cItem & a_HeldItem, const Vector3i a_Position, const NIBBLETYPE a_Meta, const eBlockFace a_ClickedBlockFace, const bool a_ClickedDirectly) const override
	{
		/* Double slab combining uses build collision checks to replace single slabs with double slabs in the right conditions.
		For us to be replaced, the player must be:
		1. Placing the same slab material.
		2. Placing the same slab sub-kind (and existing slab is single). */
		if ((m_BlockType != a_HeldItem.m_ItemType) || ((a_Meta & 0x07) != a_HeldItem.m_ItemDamage))
		{
			return false;
		}

		const bool IsTopSlab = (a_Meta & 0x08) == 0x08;
		const auto CanClickCombine = ((a_ClickedBlockFace == BLOCK_FACE_TOP) && !IsTopSlab) || ((a_ClickedBlockFace == BLOCK_FACE_BOTTOM) && IsTopSlab);

		/* When the player clicks on us directly, we'll combine if we're
		a bottom slab and he clicked the top, or vice versa. Clicking on the sides will not combine.
		However, when indirectly clicked (on the side of another block, that caused placement to go to us)
		the conditions are exactly the opposite. */
		return a_ClickedDirectly ? CanClickCombine : !CanClickCombine;
	}





	virtual void OnCancelRightClick(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace
	) const override
	{
		if ((a_BlockFace == BLOCK_FACE_NONE) || (a_Player.GetEquippedItem().m_ItemType != static_cast<short>(m_BlockType)))
		{
			return;
		}

		// Sends the slab back to the client. It's to refuse a doubleslab placement. */
		a_Player.GetWorld()->SendBlockTo(a_BlockPos, a_Player);
	}





	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) const override
	{
		// Toggle the 4th bit - up / down:
		return (a_Meta ^ 0x08);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		a_Meta &= 0x7;

		switch (m_BlockType)
		{
			case E_BLOCK_STONE_SLAB:
			{
				switch (a_Meta)
				{
					case E_META_STONE_SLAB_SANDSTONE: return 2;
					case E_META_STONE_SLAB_PLANKS: return 13;
					case E_META_STONE_SLAB_STONE_BRICK:
					case E_META_STONE_SLAB_STONE:
					case E_META_STONE_SLAB_COBBLESTONE: return 11;
					case E_META_STONE_SLAB_BRICK: return 28;
					case E_META_STONE_SLAB_NETHER_BRICK: return 35;
					case E_META_STONE_SLAB_QUARTZ: return 8;
					default:
					{
						ASSERT(!"Unhandled meta in slab handler!");
						return 0;
					}
				}
			}
			case E_BLOCK_WOODEN_SLAB:
			{
				switch (a_Meta)
				{
					case E_META_WOODEN_SLAB_BIRCH: return 2;
					case E_META_WOODEN_SLAB_JUNGLE: return 10;
					case E_META_WOODEN_SLAB_OAK: return 13;
					case E_META_WOODEN_SLAB_ACACIA: return 15;
					case E_META_WOODEN_SLAB_DARK_OAK: return 26;
					case E_META_WOODEN_SLAB_SPRUCE: return 34;
					default:
					{
						ASSERT(!"Unhandled meta in slab handler!");
						return 0;
					}
				}
			}
			case E_BLOCK_RED_SANDSTONE_SLAB:
			{
				return 10;
			}
			case E_BLOCK_PURPUR_SLAB:
			{
				return 16;
			}
			default:
			{
				ASSERT(!"Unhandled blocktype in slab handler!");
				return 0;
			}
		}
	}





	virtual bool IsInsideBlock(Vector3d a_Position, const NIBBLETYPE a_BlockMeta) const override
	{
		if (a_BlockMeta & 0x08)  // top half
		{
			return true;
		}
		return cBlockHandler::IsInsideBlock(a_Position, a_BlockMeta);
	}
} ;





class cBlockDoubleSlabHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		BLOCKTYPE Block = GetSingleSlabType(m_BlockType);
		return cItem(Block, 2, a_BlockMeta & 0x7);
	}





	inline static BLOCKTYPE GetSingleSlabType(BLOCKTYPE a_BlockType)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_DOUBLE_STONE_SLAB:         return E_BLOCK_STONE_SLAB;
			case E_BLOCK_DOUBLE_WOODEN_SLAB:        return E_BLOCK_WOODEN_SLAB;
			case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB: return E_BLOCK_RED_SANDSTONE_SLAB;
			case E_BLOCK_PURPUR_DOUBLE_SLAB:        return E_BLOCK_PURPUR_SLAB;
		}
		ASSERT(!"Unhandled double slab type!");
		return a_BlockType;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		// For doule slabs, the meta values are the same. Only the meaning of the 4th bit changes, but that's ignored in the below handler
		return cBlockHandler::For(GetSingleSlabType(m_BlockType)).GetMapBaseColourID(a_Meta);
	}
} ;




