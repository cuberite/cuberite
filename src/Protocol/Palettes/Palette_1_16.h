#pragma once

#include "BlockState.h"
#include "Registries/Items.h"
#include "Registries/CustomStatistics.h"

namespace Palette_1_16
{
	UInt32 From(BlockState Block);
	UInt32 From(Item ID);
	UInt32 From(CustomStatistic ID);
	Item ToItem(UInt32 ID);
	UInt32 ToProtocolIdBlock(NEWBLOCKTYPE block);
	UInt32 ToProtocolIdBlock16_2(NEWBLOCKTYPE block);
}
