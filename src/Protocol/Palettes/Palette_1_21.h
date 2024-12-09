#pragma once

#include "BlockState.h"
#include "Registries/Items.h"
#include "Registries/CustomStatistics.h"

namespace Palette_1_21
{
	UInt32 From(BlockState Block);
	UInt32 From(Item ID);
	UInt32 From(CustomStatistic ID);
	Item ToItem(UInt32 ID);
	UInt32 FromSound(const AString & a_SoundName);
}
