
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

	static inline bool IsBlockCarpet(BlockState a_Block)
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

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		return (a_RelPos.y > 0) && (a_Chunk.GetBlock(a_RelPos.addedY(-1)) != E_BLOCK_AIR);
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::WhiteCarpet: return 14;
			case BlockType::OrangeCarpet: return 15;
			case BlockType::MagentaCarpet: return 16;
			case BlockType::LightBlueCarpet: return 17;
			case BlockType::YellowCarpet: return 18;
			case BlockType::LimeCarpet: return 19;
			case BlockType::PinkCarpet: return 20;
			case BlockType::GrayCarpet: return 21;
			case BlockType::LightGrayCarpet: return 22;
			case BlockType::CyanCarpet: return 23;
			case BlockType::PurpleCarpet: return 24;
			case BlockType::BlueCarpet: return 25;
			case BlockType::BrownCarpet: return 26;
			case BlockType::GreenCarpet: return 27;
			case BlockType::RedCarpet: return 28;
			case BlockType::BlackCarpet: return 29;
			default:
			{
				ASSERT(!"Unhandled blocktyp in carpet handler!");
				return 0;
			}
		}
	}
} ;




