
#pragma once

#include "BlockHandler.h"





/** Handler for the small (singleblock) mushrooms. */
class cBlockMushroomHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	// TODO: Add Mushroom Spread

	virtual bool CanBeAt(const cChunk & a_Chunk, Vector3i a_Position, BlockState a_Self) const override
	{
		const auto BasePos = a_Position.addedY(-1);
		if (!cChunkDef::IsValidHeight(BasePos))
		{
			return false;
		}

		// TODO: Cannot be at too much daylight

		switch (a_Chunk.GetBlock(BasePos).Type())
		{
			case BlockType::AcaciaLeaves:
			case BlockType::Air:
			case BlockType::BirchLeaves:
			case BlockType::BlackStainedGlass:
			case BlockType::BlueStainedGlass:
			case BlockType::BrownStainedGlass:
			case BlockType::Cactus:
			case BlockType::CyanStainedGlass:
			case BlockType::DarkOakLeaves:
			case BlockType::Glass:
			case BlockType::GrayStainedGlass:
			case BlockType::GreenStainedGlass:
			case BlockType::Ice:
			case BlockType::JungleLeaves:
			case BlockType::LightBlueStainedGlass:
			case BlockType::LightGrayStainedGlass:
			case BlockType::LimeStainedGlass:
			case BlockType::MagentaStainedGlass:
			case BlockType::OakLeaves:
			case BlockType::OrangeStainedGlass:
			case BlockType::PinkStainedGlass:
			case BlockType::PurpleStainedGlass:
			case BlockType::RedStainedGlass:
			case BlockType::SpruceLeaves:
			case BlockType::WhiteStainedGlass:
			case BlockType::YellowStainedGlass:
				return false;
			default: return true;
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 0;
	}
} ;




