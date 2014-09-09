
#pragma once

#include "BlockHandler.h"
#include "MetaRotator.h"




class cBlockStairsHandler :
	public cMetaRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x02, 0x01, true>
{
public:
	cBlockStairsHandler(BLOCKTYPE a_BlockType) :
		cMetaRotator<cBlockHandler, 0x03, 0x03, 0x00, 0x02, 0x01, true>(a_BlockType)
	{

	}


	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		UNUSED(a_ChunkInterface);
		UNUSED(a_BlockX);
		UNUSED(a_BlockY);
		UNUSED(a_BlockZ);
		UNUSED(a_CursorX);
		UNUSED(a_CursorZ);
		a_BlockType = m_BlockType;
		a_BlockMeta = RotationToMetaData(a_Player->GetYaw());
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
			case BLOCK_FACE_NONE: return false;
		}
		return true;
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to zero
		a_Pickups.push_back(cItem(m_BlockType, 1, 0));
	}


	virtual bool CanDirtGrowGrass(NIBBLETYPE a_Meta) override
	{
		return true;
	}


	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45;  // So its not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}
		if ((a_Rotation >= 0) && (a_Rotation < 90))
		{
			return 0x0;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return 0x1;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return 0x2;
		}
		else
		{
			return 0x3;
		}
	}

	virtual NIBBLETYPE MetaMirrorXZ(NIBBLETYPE a_Meta) override
	{
		// Toggle bit 3:
		return (a_Meta & 0x0b) | ((~a_Meta) & 0x04);
	}
} ;




