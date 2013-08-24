
#pragma once

#include "BlockHandler.h"




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
		a_Pickups.push_back(cItem(m_BlockType, 1, a_BlockMeta));
	}


	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		BLOCKTYPE Type = (BLOCKTYPE)(a_Player->GetEquippedItem().m_ItemType);
		NIBBLETYPE Meta = (NIBBLETYPE)(a_Player->GetEquippedItem().m_ItemDamage & 0x07);
		
		int DoubleType;
		if (Type == E_BLOCK_STONE_SLAB)
		{
			DoubleType = 43; // Make it a double slab (with old type wood)
		}
		else
		{
			 DoubleType = 125; // Make it a wooden double slab (new type)
		}
		// HandlePlaceBlock wants a cItemHandler pointer thing, so let's give it one
		cItemHandler * ItemHandler = cItemHandler::GetItemHandler(DoubleType);

		// Check if the block at the coordinates is a slab. Eligibility for combining etc. were processed in ClientHandle
		BLOCKTYPE IsSlab;
		IsSlab = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
		if ((IsSlab == E_BLOCK_STONE_SLAB) || (IsSlab == E_BLOCK_WOODEN_SLAB))
		{
			// Special handling for non top/bottom clicks
			if ((a_BlockFace == BLOCK_FACE_TOP) || (a_BlockFace == BLOCK_FACE_BOTTOM))
			{
				// As with previous, call the function in ClientHandle that places a block when the client sends the packet
				// This effectively simulates a client placing a double slab, so it goes through plugins etc. so the slabbing can be cancelled
				a_Player->GetClientHandle()->HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
			}
			else
			{
				// If player cursor is at top half of block
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
			return false; // Cancel the event because dblslabs were already placed, nothing else needed
		}
		
		switch (a_BlockFace)
		{
			// Previous IF condition didn't cancel the event (not a slab at coords), so place slab with correct metas
			case BLOCK_FACE_TOP:
			{
				a_BlockMeta = Meta & 0x7; break;  // Bottom half slab block
			}
			case BLOCK_FACE_BOTTOM:
			{
				a_BlockMeta = Meta | 0x8; break;  // Top half slab block
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
		return ((m_BlockType == E_BLOCK_WOODEN_SLAB) || (m_BlockType == E_BLOCK_STONE_SLAB)) ?  "step.wood" : "step.stone";
	}
} ;




