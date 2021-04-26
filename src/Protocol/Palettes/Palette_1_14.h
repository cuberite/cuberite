#pragma once

#include "BlockState.h"
#include "Registries/Items.h"
#include "Registries/Statistics.h"

namespace Palette_1_14
{
	UInt32 From(BlockState Block);
	UInt32 From(Item ID);
	UInt32 From(CustomStatistic ID);
	Item ToItem(UInt32 ID);
}
