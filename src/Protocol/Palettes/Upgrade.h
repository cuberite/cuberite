#pragma once

#include "ChunkDef.h"
#include "BlockState.h"
#include "Registries/Items.h"

namespace PaletteUpgrade
{
	BlockState FromBlock(BLOCKTYPE Block, NIBBLETYPE Meta);
	Item FromItem(short Item, short Damage);
	std::pair<short, short> ToItem(Item ID);
}
