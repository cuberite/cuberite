
// BlockSlab.h

// Declares cBlockSlabHandler and cBlockDoubleSlabHandler classes





#pragma once

#include "BlockHandler.h"
#include "../Items/ItemHandler.h"
#include "Root.h"



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
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		NIBBLETYPE Meta = (NIBBLETYPE) a_Player->GetEquippedItem().m_ItemDamage;
		
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
		if (IsAnySlabType(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ)))
		{
			a_BlockType = GetDoubleSlabType(m_BlockType);
			a_BlockMeta = a_BlockMeta & 0x7;
		}

		return true;
	}
	
	
	virtual const char * GetStepSound(void) override
	{
		switch (m_BlockType)
		{
			case E_BLOCK_WOODEN_SLAB: return "step.wood";
			case E_BLOCK_STONE_SLAB:  return "step.stone";
		}
		ASSERT(!"Unhandled slab type!");
		return "";
	}


	virtual bool CanDirtGrowGrass(NIBBLETYPE a_Meta) override
	{
		return ((a_Meta & 0x8) != 0);
	}

	
	/// Returns true if the specified blocktype is one of the slabs handled by this handler
	static bool IsAnySlabType(BLOCKTYPE a_BlockType)
	{
		return ((a_BlockType == E_BLOCK_WOODEN_SLAB) || (a_BlockType == E_BLOCK_STONE_SLAB));
	}
	
	
	/// Converts the single-slab blocktype to its equivalent double-slab blocktype
	static BLOCKTYPE GetDoubleSlabType(BLOCKTYPE a_SingleSlabBlockType)
	{
		switch (a_SingleSlabBlockType)
		{
			case E_BLOCK_STONE_SLAB:  return E_BLOCK_DOUBLE_STONE_SLAB;
			case E_BLOCK_WOODEN_SLAB: return E_BLOCK_DOUBLE_WOODEN_SLAB;
		}
		ASSERT(!"Unhandled slab type!");
		return E_BLOCK_AIR;
	}
	
	
	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		// Toggle the 4th bit - up / down:
		return (a_Meta ^ 0x08);
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
			case E_BLOCK_DOUBLE_STONE_SLAB:  return E_BLOCK_STONE_SLAB;
			case E_BLOCK_DOUBLE_WOODEN_SLAB: return E_BLOCK_WOODEN_SLAB;
		}
		ASSERT(!"Unhandled double slab type!");
		return a_BlockType;
	}
	
	virtual const char * GetStepSound(void) override
	{
		switch (m_BlockType)
		{
			case E_BLOCK_DOUBLE_STONE_SLAB:  return "step.stone";
			case E_BLOCK_DOUBLE_WOODEN_SLAB: return "step.wood";
		}
		ASSERT(!"Unhandled double slab type!");
		return "";
	}
} ;




