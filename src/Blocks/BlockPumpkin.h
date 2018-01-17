#pragma once

#include "BlockHandler.h"
#include "MetaRotator.h"



class cBlockPumpkinHandler :
	public cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x03, 0x00, 0x01, false> >
{
	typedef cClearMetaOnDrop<cMetaRotator<cBlockHandler, 0x07, 0x02, 0x03, 0x00, 0x01, false> > super;
public:

	cBlockPumpkinHandler(BLOCKTYPE a_BlockType) :
		super(a_BlockType)
	{
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer & a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = PlayerYawToMetaData(a_Player.GetYaw());
		return true;
	}

	inline static NIBBLETYPE PlayerYawToMetaData(double a_Yaw)
	{
		ASSERT((a_Yaw >= -180) && (a_Yaw < 180));

		a_Yaw += 180 + 45;
		if (a_Yaw > 360)
		{
			a_Yaw -= 360;
		}
		if ((a_Yaw >= 0) && (a_Yaw < 90))
		{
			return 0x0;
		}
		else if ((a_Yaw >= 180) && (a_Yaw < 270))
		{
			return 0x2;
		}
		else if ((a_Yaw >= 90) && (a_Yaw < 180))
		{
			return 0x1;
		}
		else
		{
			return 0x3;
		}
	}

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 15;
	}
} ;




