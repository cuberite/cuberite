#pragma once

#include "../../Registries/Items.h"
#include "../../Registries/Statistics.h"

namespace Palette_1_13_1
{
	UInt32 FromBlock(short ID);
	UInt32 FromItem(Item ID);
	UInt32 From(Statistic ID);
	Item ToItem(UInt32 ID);
}
