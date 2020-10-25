#pragma once

#include "../../BlockType.h"
#include "../../Registries/Items.h"

namespace PaletteUpgrade
{
	short FromBlock(BLOCKTYPE Block, NIBBLETYPE Meta);
	Item FromItem(short Item, short Damage);
	std::pair<short, short> ToItem(Item ID);
}
