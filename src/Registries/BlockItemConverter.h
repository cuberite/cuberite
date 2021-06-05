
#pragma once

#include "BlockTypes.h"
#include "Items.h"

namespace BlockItemConverter
{
	// These two functions convert the Item and Block enums into each other - there is are many values that are not convertible
	// These return AIR - so do a sanity check when using this or check in the switch statement
	BlockType FromItem(Item a_Item);
	Item FromBlock(BlockType a_Block);
}
