
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

	static constexpr bool IsBlockCarpet(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::BlackCarpet:
			case BlockType::BlueCarpet:
			case BlockType::BrownCarpet:
			case BlockType::CyanCarpet:
			case BlockType::GrayCarpet:
			case BlockType::GreenCarpet:
			case BlockType::LightBlueCarpet:
			case BlockType::LightGrayCarpet:
			case BlockType::LimeCarpet:
			case BlockType::MagentaCarpet:
			case BlockType::OrangeCarpet:
			case BlockType::PinkCarpet:
			case BlockType::PurpleCarpet:
			case BlockType::RedCarpet:
			case BlockType::WhiteCarpet:
			case BlockType::YellowCarpet:
				return true;
			default: return false;
		}
	}

private:

	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		return (a_Position.y > 0) && (a_Chunk.GetBlock(a_Position.addedY(-1)) != E_BLOCK_AIR);
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




