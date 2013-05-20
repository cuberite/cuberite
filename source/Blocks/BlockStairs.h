
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
				if (a_CursorY > 8)
				{
					a_BlockMeta |= 0x4;
				}
				break;
			}
		}
		return true;
	}
	
	// TODO: step sound
	
	
	virtual NIBBLETYPE MetaRotateCCW(NIBBLETYPE a_Meta) override
	{
		// Bits 3 and 4 stay, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x0c);
		switch (a_Meta & 0x03)
		{
			case 0x00: return TopBits | 0x03;  // East -> North
			case 0x01: return TopBits | 0x02;  // West -> South
			case 0x02: return TopBits | 0x00;  // South -> East
			case 0x03: return TopBits | 0x01;  // North -> West
		}
		// Not reachable, but to avoid a compiler warning:
		return 0;
	}


	virtual NIBBLETYPE MetaRotateCW(NIBBLETYPE a_Meta) override
	{
		// Bits 3 and 4 stay, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x0c);
		switch (a_Meta & 0x03)
		{
			case 0x00: return TopBits | 0x02;  // East -> South
			case 0x01: return TopBits | 0x03;  // West -> North
			case 0x02: return TopBits | 0x01;  // South -> West
			case 0x03: return TopBits | 0x00;  // North -> East
		}
		// Not reachable, but to avoid a compiler warning:
		return 0;
	}


	virtual NIBBLETYPE MetaMirrorXY(NIBBLETYPE a_Meta) override
	{
		// Bits 3 and 4 stay, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x0c);
		switch (a_Meta & 0x03)
		{
			case 0x00: return TopBits | 0x00;  // East -> East
			case 0x01: return TopBits | 0x01;  // West -> West
			case 0x02: return TopBits | 0x03;  // South -> North
			case 0x03: return TopBits | 0x02;  // North -> South
		}
		// Not reachable, but to avoid a compiler warning:
		return 0;
	}


	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		// Toggle bit 3:
		return (a_Meta & 0x0b) | ((~a_Meta) & 0x04);
	}


	virtual NIBBLETYPE MetaMirrorYZ(NIBBLETYPE a_Meta) override
	{
		// Bits 3 and 4 stay, the rest is swapped around according to a table:
		NIBBLETYPE TopBits = (a_Meta & 0x0c);
		switch (a_Meta & 0x03)
		{
			case 0x00: return TopBits | 0x01;  // East -> West
			case 0x01: return TopBits | 0x00;  // West -> East
			case 0x02: return TopBits | 0x02;  // South -> South
			case 0x03: return TopBits | 0x03;  // North -> North
		}
		// Not reachable, but to avoid a compiler warning:
		return 0;
	}
} ;




