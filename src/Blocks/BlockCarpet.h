
// BlockCarpet.h

// Declares the cBlockCarpetHandler class representing the handler for the carpet block




#pragma once

#include "BlockHandler.h"





class cBlockCarpetHandler final :
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
		a_BlockMeta = a_Player.GetEquippedItem().m_ItemDamage & 0x0f;
		return true;
	}





	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		return (a_RelPos.y > 0) && (a_Chunk.GetBlock(a_RelPos.addedY(-1)) != E_BLOCK_AIR);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		switch (a_Meta)
		{
			case E_META_CARPET_WHITE: return 14;
			case E_META_CARPET_ORANGE: return 15;
			case E_META_CARPET_MAGENTA: return 16;
			case E_META_CARPET_LIGHTBLUE: return 17;
			case E_META_CARPET_YELLOW: return 18;
			case E_META_CARPET_LIGHTGREEN: return 19;
			case E_META_CARPET_PINK: return 20;
			case E_META_CARPET_GRAY: return 21;
			case E_META_CARPET_LIGHTGRAY: return 22;
			case E_META_CARPET_CYAN: return 23;
			case E_META_CARPET_PURPLE: return 24;
			case E_META_CARPET_BLUE: return 25;
			case E_META_CARPET_BROWN: return 26;
			case E_META_CARPET_GREEN: return 27;
			case E_META_CARPET_RED: return 28;
			case E_META_CARPET_BLACK: return 29;
			default:
			{
				ASSERT(!"Unhandled meta in carpet handler!");
				return 0;
			}
		}
	}
} ;




