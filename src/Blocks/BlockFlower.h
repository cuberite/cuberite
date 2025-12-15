
#pragma once

#include "BlockHandler.h"
#include "Mixins/DirtLikeUnderneath.h"





class cBlockFlowerHandler final :
	public cDirtLikeUnderneath<cBlockHandler>
{
	using Super = cDirtLikeUnderneath<cBlockHandler>;

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

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		return ((a_Position.y > 0) && IsBlockMaterialDirt(a_Chunk.GetBlock(a_Position.addedY(-1))));
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 7;
	}
} ;




