#pragma once

#include "BlockState.h"
#include "Registries/Items.h"
#include "Registries/CustomStatistics.h"

namespace Palette_1_21_4
{
	UInt32 From(BlockState Block);
	UInt32 From(BlockType a_Block);
	UInt32 From(Item ID);
	UInt32 From(CustomStatistic ID);
	UInt32 From(FluidType a_Fluid);
	Item ToItem(UInt32 ID);
}
