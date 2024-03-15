
#pragma once

#include "BlockHandler.h"





class cBlockFlowerHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockFlower(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Allium:
			case BlockType::AzureBluet:
			case BlockType::BlueOrchid:
			case BlockType::Cornflower:
			case BlockType::LilyOfTheValley:
			case BlockType::OrangeTulip:
			case BlockType::OxeyeDaisy:
			case BlockType::PinkTulip:
			case BlockType::Poppy:
			case BlockType::RedTulip:
			case BlockType::WhiteTulip:
				return true;
			default: return false;
		}
	}

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		NIBBLETYPE meta = a_BlockMeta & 0x7;
		return cItem(m_BlockType, 1, meta);
	}





	virtual bool CanBeAt(const cChunk & a_Chunk, const Vector3i a_Position, const NIBBLETYPE a_Meta) const override
	{
		return (a_Position.y > 0) && IsBlockTypeOfDirt(a_Chunk.GetBlock(a_Position.addedY(-1)));
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 7;
	}
} ;




