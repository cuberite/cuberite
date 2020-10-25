
#pragma once

#include "BlockHandler.h"





class cBlockPlanksHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface,
		cPlayer & a_Player,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) const override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = static_cast<NIBBLETYPE>(a_Player.GetEquippedItem().m_ItemDamage);
		return true;
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		switch (a_Meta)
		{
			case E_META_PLANKS_BIRCH: return 2;
			case E_META_PLANKS_JUNGLE: return 10;
			case E_META_PLANKS_OAK: return 13;
			case E_META_PLANKS_ACACIA: return 15;
			case E_META_PLANKS_DARK_OAK: return 26;
			case E_META_PLANKS_SPRUCE: return 34;
			default:
			{
				ASSERT(!"Unhandled meta in planks handler!");
				return 0;
			}
		}
	}
} ;




