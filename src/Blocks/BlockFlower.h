
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

	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, const Vector3i a_RelPos, const cChunk & a_Chunk) const override
	{
		return ((a_RelPos.y > 0) && IsBlockStateOfDirt(a_Chunk.GetBlock(a_RelPos.addedY(-1))));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
} ;




