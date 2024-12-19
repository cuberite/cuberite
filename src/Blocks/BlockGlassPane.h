
#pragma once

#include "BlockHandler.h"

class cBlockGlassPaneHandler :
	cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

	static inline bool IsBlockGlassPane(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::GlassPane:
			case BlockType::BlackStainedGlassPane:
			case BlockType::BlueStainedGlassPane:
			case BlockType::BrownStainedGlassPane:
			case BlockType::CyanStainedGlassPane:
			case BlockType::GrayStainedGlassPane:
			case BlockType::GreenStainedGlassPane:
			case BlockType::LightBlueStainedGlassPane:
			case BlockType::LightGrayStainedGlassPane:
			case BlockType::LimeStainedGlassPane:
			case BlockType::MagentaStainedGlassPane:
			case BlockType::OrangeStainedGlassPane:
			case BlockType::PinkStainedGlassPane:
			case BlockType::PurpleStainedGlassPane:
			case BlockType::RedStainedGlassPane:
			case BlockType::YellowStainedGlassPane:
			case BlockType::WhiteStainedGlassPane:
				return true;
			default: return false;
		}
	}
};
