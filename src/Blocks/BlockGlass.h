
#pragma once

#include "BlockHandler.h"





class cBlockGlassHandler final :
	public cBlockHandler
{
public:

	using cBlockHandler::cBlockHandler;

	static inline bool IsBlockGlass(BlockState a_Block)
	{
		switch (a_Block.Type())
		{
			case BlockType::Glass:
			case BlockType::BlackStainedGlass:
			case BlockType::BlueStainedGlass:
			case BlockType::BrownStainedGlass:
			case BlockType::CyanStainedGlass:
			case BlockType::GrayStainedGlass:
			case BlockType::GreenStainedGlass:
			case BlockType::LightBlueStainedGlass:
			case BlockType::LightGrayStainedGlass:
			case BlockType::LimeStainedGlass:
			case BlockType::MagentaStainedGlass:
			case BlockType::OrangeStainedGlass:
			case BlockType::PinkStainedGlass:
			case BlockType::PurpleStainedGlass:
			case BlockType::RedStainedGlass:
			case BlockType::YellowStainedGlass:
			case BlockType::WhiteStainedGlass:
				return true;
			default: return false;
		}
	}

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		// Only drop self when mined with silk-touch:
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType, 1, a_BlockMeta);
		}
		return {};
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 0;
	}
} ;




