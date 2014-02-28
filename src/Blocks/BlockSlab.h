
// BlockSlab.h

// Declares cBlockSlabHandler and cBlockDoubleSlabHandler classes





#pragma once

#include "BlockHandler.h"
#include "../Items/ItemHandler.h"





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
		BLOCKTYPE  Type = (BLOCKTYPE) (a_Player->GetEquippedItem().m_ItemType);
		NIBBLETYPE Meta = (NIBBLETYPE) a_Player->GetEquippedItem().m_ItemDamage;
		
		// HandlePlaceBlock wants a cItemHandler pointer thing, so let's give it one
		cItemHandler * ItemHandler = cItemHandler::GetItemHandler(GetDoubleSlabType(Type));

		// Check if the block at the coordinates is a slab. Eligibility for combining has already been processed in ClientHandle
		if (IsAnySlabType(a_ChunkInterface.GetBlock(a_BlockX, a_BlockY, a_BlockZ)))
		{
			// Call the function in ClientHandle that places a block when the client sends the packet,
			// so that plugins may interfere with the placement.
			
			if ((a_BlockFace == BLOCK_FACE_TOP) || (a_BlockFace == BLOCK_FACE_BOTTOM))
			{
				// Top and bottom faces need no parameter modification
				a_Player->GetClientHandle()->HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
			}
			else
			{
				// The other faces need to distinguish between top and bottom cursor positions
				if (a_CursorY > 7)
				{
					// Edit the call to use BLOCK_FACE_BOTTOM, otherwise it places incorrectly
					a_Player->GetClientHandle()->HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_TOP, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
				}
				else
				{
					// Edit the call to use BLOCK_FACE_TOP, otherwise it places incorrectly
					a_Player->GetClientHandle()->HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, BLOCK_FACE_BOTTOM, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
				}
			}
			return false;  // Cancel the event, because dblslabs were already placed, nothing else needed
		}
		
		// Place the single-slab with correct metas:
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
		}  // switch (a_BlockFace)
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
		BLOCKTYPE Block = GetSingleSlabType(m_BlockType);
		if (Block != m_BlockType)
		{
			return cBlockHandler::GetBlockHandler(Block)->GetStepSound();
		}
		else
		{
			return "step.stone";
		}
	}
} ;




