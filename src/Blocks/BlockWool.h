
#pragma once

#include "BlockHandler.h"





class cBlockWoolHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

	static inline bool IsBlockWool(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::BlackWool:
			case BlockType::BlueWool:
			case BlockType::BrownWool:
			case BlockType::CyanWool:
			case BlockType::GrayWool:
			case BlockType::GreenWool:
			case BlockType::LightBlueWool:
			case BlockType::LightGrayWool:
			case BlockType::LimeWool:
			case BlockType::MagentaWool:
			case BlockType::OrangeWool:
			case BlockType::PinkWool:
			case BlockType::PurpleWool:
			case BlockType::RedWool:
			case BlockType::WhiteWool:
			case BlockType::YellowWool:
				return true;
			default: return false;
		}
	}

private:

	virtual ColourID GetMapBaseColourID() const override
	{
		switch (m_BlockType)
		{
			case BlockType::BlackWool:     return 29;
			case BlockType::BlueWool:      return 25;
			case BlockType::BrownWool:     return 26;
			case BlockType::CyanWool:      return 23;
			case BlockType::GrayWool:      return 21;
			case BlockType::GreenWool:     return 27;
			case BlockType::LightBlueWool: return 17;
			case BlockType::LightGrayWool: return 22;
			case BlockType::LimeWool:      return 19;
			case BlockType::MagentaWool:   return 16;
			case BlockType::OrangeWool:    return 15;
			case BlockType::PinkWool:      return 20;
			case BlockType::PurpleWool:    return 24;
			case BlockType::RedWool:       return 28;
			case BlockType::WhiteWool:     return 14;
			case BlockType::YellowWool:    return 18;
			default:
			{
				ASSERT(!"Unhandled block in wool handler!");
				return 0;
			}
		}
	}
} ;
