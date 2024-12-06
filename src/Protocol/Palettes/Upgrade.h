//#pragma once

#include "ChunkDef.h"
#include "BlockState.h"
#include "Registries/Items.h"

namespace PaletteUpgrade
{
	using BLOCKTYPE = unsigned char;
	using NIBBLETYPE = unsigned char;

	BlockState FromBlock(BLOCKTYPE Block, NIBBLETYPE Meta);
	Item FromItem(short Item, short Damage);

	// Any unimplemented Block before 1.13 is returned as air.
	std::pair<unsigned char, unsigned char> ToBlock(BlockState a_Block);
	std::pair<short, short> ToItem(Item ID);
	BlockType ToBlockType(NEWBLOCKTYPE block);
	BlockType BlockToItemType(Item blocktype);
	std::array<std::pair<std::string, std::string>, 8> GetSaveStrings(NEWBLOCKTYPE a_block);
};
