
#pragma once

#include "BlockHandler.h"





class cBlockEndPortalFrameHandler:
	public cMetaRotator<cBlockHandler, 0x03,
		E_META_END_PORTAL_FRAME_ZM,
		E_META_END_PORTAL_FRAME_XP,
		E_META_END_PORTAL_FRAME_ZP,
		E_META_END_PORTAL_FRAME_XM
	>
{
	using Super = cMetaRotator<
		cBlockHandler, 0x03,
		E_META_END_PORTAL_FRAME_ZM,
		E_META_END_PORTAL_FRAME_XP,
		E_META_END_PORTAL_FRAME_ZP,
		E_META_END_PORTAL_FRAME_XM
	>;

public:

	cBlockEndPortalFrameHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = YawToMetaData(a_Player.GetYaw());
		return true;
	}





	inline static NIBBLETYPE YawToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45;  // So its not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}

		if ((a_Rotation >= 0) && (a_Rotation < 90))
		{
			return E_META_END_PORTAL_FRAME_XM;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return E_META_END_PORTAL_FRAME_XP;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return E_META_END_PORTAL_FRAME_ZM;
		}
		else
		{
			return E_META_END_PORTAL_FRAME_ZP;
		}
	}





	virtual bool IsClickedThrough(void) override
	{
		// TODO: Colision
		return true;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) override
	{
		UNUSED(a_Meta);
		return 27;
	}
};




