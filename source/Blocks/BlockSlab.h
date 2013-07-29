
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
		char Count = ((m_BlockType == E_BLOCK_DOUBLE_STONE_SLAB) || (m_BlockType == E_BLOCK_DOUBLE_WOODEN_SLAB)) ? 2 : 1;
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
		NIBBLETYPE Meta = (NIBBLETYPE)(a_Player->GetEquippedItem().m_ItemDamage & 0x07);
		switch (a_BlockFace)
		{
			case BLOCK_FACE_TOP:    a_BlockMeta = Meta & 0x7; break;  // Always bottom half of the slab when placing on top    of something
			case BLOCK_FACE_BOTTOM: a_BlockMeta = Meta | 0x8; break;  // Always top    half of the slab when placing on bottom of something
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			case BLOCK_FACE_WEST:
			{
				if (a_CursorY > 7)
				{
					// Cursor at the top half of the face, place a top half of slab
					a_BlockMeta = Meta | 0x8;
				}
				else
				{
					// Cursor at the bottom half of the face, place a bottom half of slab:
					a_BlockMeta = Meta & 0x7;
				}
				break;
			}
		}  // switch (a_BlockFace)
		return true;
	}
	
	
	virtual const char * GetStepSound(void) override
	{		
		return ((m_BlockType == E_BLOCK_WOODEN_SLAB) || (m_BlockType == E_BLOCK_DOUBLE_WOODEN_SLAB)) ?  "step.wood" : "step.stone";
	}
} ;




