
#pragma once

#include "BlockHandler.h"
#include "../Stairs.h"





class cBlockStairsHandler :
	public cBlockHandler
{
public:
	cBlockStairsHandler(BLOCKTYPE a_BlockType) :
		cBlockHandler(a_BlockType)
	{

	}
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cWorld * a_World, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = cStairs::RotationToMetaData(a_Player->GetRotation());
		switch (a_BlockFace)
		{
			case BLOCK_FACE_TOP:    break;
			case BLOCK_FACE_BOTTOM: a_BlockMeta = a_BlockMeta | 0x4; break;  // When placing onto a bottom face, always place an upside-down stairs block
			case BLOCK_FACE_EAST:
			case BLOCK_FACE_NORTH:
			case BLOCK_FACE_SOUTH:
			case BLOCK_FACE_WEST:
			{
				// When placing onto a sideways face, check cursor, if in top half, make it an upside-down stairs block
				if (a_CursorY < 8)
				{
					a_BlockMeta |= 0x4;
				}
				break;
			}
		}
		return true;
	}
	
	// TODO: step sound
} ;




