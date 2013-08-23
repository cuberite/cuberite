
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
		char Count = ((m_BlockType == E_BLOCK_STONE_SLAB) || (m_BlockType == E_BLOCK_WOODEN_SLAB)) ? 1 : 1;
		a_Pickups.push_back(cItem(m_BlockType, Count, a_BlockMeta));
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
			DoubleType = 43; //Make it a double slab (with old type wood)
		}
		else
		{
			 DoubleType = 125; //Make it a wooden double slab (new type)
		}
		cItemHandler * ItemHandler = cItemHandler::GetItemHandler(DoubleType);

		switch (a_BlockFace)
		{
			case BLOCK_FACE_TOP:
			{
				if ((a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_STONE_SLAB) || (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_WOODEN_SLAB))
				{
					a_Player->GetClientHandle()->HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
					return false;
				}
				else
				{
					a_BlockMeta = Meta & 0x7; break;  //Bottom half if on top of non slab block
				}
			}
			case BLOCK_FACE_BOTTOM:
			{
				if ((a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_STONE_SLAB) || (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_WOODEN_SLAB))
				{
					a_Player->GetClientHandle()->HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
					return false;
				}
				else
				{
					a_BlockMeta = Meta | 0x8; break;  //Bottom half if on top of non slab block
				}
			}
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			case BLOCK_FACE_WEST:
			{
				if (a_CursorY > 7)
				{
					// Cursor at the top half of the face, place a top half of slab
					if ((a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_STONE_SLAB) || (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_WOODEN_SLAB))
					{
						a_Player->GetClientHandle()->HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
						return false;
					}
					else
					{
						a_BlockMeta = Meta | 0x8; break;
					}
				}
				else
				{
					// Cursor at the bottom half of the face, place a bottom half of slab:
					if ((a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_STONE_SLAB) || (a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ) == E_BLOCK_WOODEN_SLAB))
					{
						a_Player->GetClientHandle()->HandlePlaceBlock(a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, *ItemHandler);
						return false;
					}
					else
					{
						a_BlockMeta = Meta & 0x7; break;
					}
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




