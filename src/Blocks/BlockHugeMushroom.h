
#pragma once

#include "BlockHandler.h"





/** Handler for huge mushroom blocks. */
class cBlockHugeMushroomHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(const NIBBLETYPE a_BlockMeta, const cItem * const a_Tool) const override
	{
		if (ToolHasSilkTouch(a_Tool))
		{
			return cItem(m_BlockType);
		}
		else if ((a_BlockMeta == E_META_MUSHROOM_FULL_STEM) || (a_BlockMeta == E_META_MUSHROOM_STEM))
		{
			// Stems don't drop anything
			return cItem();
		}

		const auto MushroomType = (m_BlockType == E_BLOCK_HUGE_BROWN_MUSHROOM) ? E_BLOCK_BROWN_MUSHROOM : E_BLOCK_RED_MUSHROOM;
		const auto DropNum = GetRandomProvider().RandInt<char>(2);

		return cItem(MushroomType, DropNum);
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return (m_BlockType == E_BLOCK_HUGE_BROWN_MUSHROOM) ? 10 : 28;
	}
} ;




