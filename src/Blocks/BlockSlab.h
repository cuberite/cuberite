
// BlockSlab.h

// Declares cBlockSlabHandler and cBlockDoubleSlabHandler classes





#pragma once

#include "BlockHandler.h"
#include "ChunkInterface.h"
#include "../Entities/Player.h"



class cBlockSlabHandler :
	public cBlockHandler
{
public:
	cBlockSlabHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(m_BlockType, 1, a_BlockMeta & 0x7));
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage);

		// Set the correct metadata based on player equipped item (i.e. a_BlockMeta not initialised yet)
		switch (a_BlockFace)
		{
			case BLOCK_FACE_TOP:
			{
				// Bottom half slab block
				a_BlockMeta = Meta & 0x7;
				break;
			}
			case BLOCK_FACE_BOTTOM:
			{
				// Top half slab block
				a_BlockMeta = Meta | 0x8;
				break;
			}
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			case BLOCK_FACE_WEST:
			{
				if (a_CursorY > 7)
				{
					// Cursor at top half of block, place top slab
					a_BlockMeta = Meta | 0x8; break;
				}
				else
				{
					// Cursor at bottom half of block, place bottom slab
					a_BlockMeta = Meta & 0x7; break;
				}
			}
			case BLOCK_FACE_NONE: return false;
		}  // switch (a_BlockFace)

		// Check if the block at the coordinates is a single slab. Eligibility for combining has already been processed in ClientHandle
		// Changed to-be-placed to a double slab if we are clicking on a single slab, as opposed to placing one for the first time
		if (IsAnySlabType(a_ChunkInterface.GetBlock({a_BlockX, a_BlockY, a_BlockZ})))
		{
			a_BlockType = GetDoubleSlabType(m_BlockType);
			a_BlockMeta = a_BlockMeta & 0x7;
		}

		return true;
	}

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

	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		if ((a_BlockFace == BLOCK_FACE_NONE) || (a_Player.GetEquippedItem().m_ItemType != static_cast<short>(m_BlockType)))
		{
			return;
		}

		// Sends the slab back to the client. It's to refuse a doubleslab placement. */
		a_Player.GetWorld()->SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	}

	/** Converts the single-slab blocktype to its equivalent double-slab blocktype */
	static BLOCKTYPE GetDoubleSlabType(BLOCKTYPE a_SingleSlabBlockType)
	{
		switch (a_SingleSlabBlockType)
		{
			case E_BLOCK_STONE_SLAB: return E_BLOCK_DOUBLE_STONE_SLAB;
			case E_BLOCK_WOODEN_SLAB: return E_BLOCK_DOUBLE_WOODEN_SLAB;
			case E_BLOCK_RED_SANDSTONE_SLAB: return E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB;
			case E_BLOCK_PURPUR_SLAB: return E_BLOCK_PURPUR_DOUBLE_SLAB;
		}
		ASSERT(!"Unhandled slab type!");
		return E_BLOCK_AIR;
	}

	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		// Toggle the 4th bit - up / down:
		return (a_Meta ^ 0x08);
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
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

	virtual bool IsInsideBlock(Vector3d a_Position, const BLOCKTYPE a_BlockType, const NIBBLETYPE a_BlockMeta) override
	{
		if (a_BlockMeta & 0x8)  // top half
		{
			return true;
		}
		return cBlockHandler::IsInsideBlock(a_Position, a_BlockType, a_BlockMeta);
	}
} ;





class cBlockDoubleSlabHandler :
	public cBlockHandler
{
public:
	cBlockDoubleSlabHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		BLOCKTYPE Block = GetSingleSlabType(m_BlockType);
		a_Pickups.push_back(cItem(Block, 2, a_BlockMeta & 0x7));
	}

	inline static BLOCKTYPE GetSingleSlabType(BLOCKTYPE a_BlockType)
	{
		switch (a_BlockType)
		{
			case E_BLOCK_DOUBLE_STONE_SLAB: return E_BLOCK_STONE_SLAB;
			case E_BLOCK_DOUBLE_WOODEN_SLAB: return E_BLOCK_WOODEN_SLAB;
			case E_BLOCK_DOUBLE_RED_SANDSTONE_SLAB: return E_BLOCK_RED_SANDSTONE_SLAB;
			case E_BLOCK_PURPUR_DOUBLE_SLAB: return E_BLOCK_PURPUR_SLAB;
		}
		ASSERT(!"Unhandled double slab type!");
		return a_BlockType;
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		// For doule slabs, the meta values are the same. Only the meaning of the 4th bit changes, but that's ignored in the below handler
		return BlockHandler(GetSingleSlabType(m_BlockType))->GetMapBaseColourID(a_Meta);
	}
} ;




