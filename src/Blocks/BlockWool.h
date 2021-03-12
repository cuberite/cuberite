
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

	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		switch (a_Meta)
		{
			case E_META_WOOL_WHITE: return 14;
			case E_META_WOOL_ORANGE: return 15;
			case E_META_WOOL_MAGENTA: return 16;
			case E_META_WOOL_LIGHTBLUE: return 17;
			case E_META_WOOL_YELLOW: return 18;
			case E_META_WOOL_LIGHTGREEN: return 19;
			case E_META_WOOL_PINK: return 20;
			case E_META_WOOL_GRAY: return 21;
			case E_META_WOOL_LIGHTGRAY: return 22;
			case E_META_WOOL_CYAN: return 23;
			case E_META_WOOL_PURPLE: return 24;
			case E_META_WOOL_BLUE: return 25;
			case E_META_WOOL_BROWN: return 26;
			case E_META_WOOL_GREEN: return 27;
			case E_META_WOOL_RED: return 28;
			case E_META_WOOL_BLACK: return 29;
			default:
			{
				ASSERT(!"Unhandled meta in wool handler!");
				return 0;
			}
		}
	}
} ;
