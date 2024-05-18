#pragma once

#include "BlockState.h"
#include "Registries/Items.h"
#include "Registries/CustomStatistics.h"

#include "BlockType.h"

namespace Palette_1_15
{
	UInt32 From(BlockState Block);
	UInt32 From(Item ID);
	UInt32 From(CustomStatistic ID);
	Item ToItem(UInt32 ID);

	UInt32 ToProtocolIdBlock(NEWBLOCKTYPE block);

	NEWBLOCKTYPE ToInternalIdBlock(UInt32 block);

	//std::unordered_map<AString, UINT32> StringNameToId;
	//static int aaa;
	//void SetupMap();
	//#pragma comment(linker, "/STACK:1500000")
	static std::unordered_map<AString, ENUM_BLOCKS> StringNameToId
	{
		{"dirt", DIRT},
		{"grass_block snowy: true", GRASS_BLOCK__SNOWY_TRUE},
		{"grass_block snowy: false", GRASS_BLOCK__SNOWY_FALSE},
		{"oak_trapdoor facing: north, half: top, open: true, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_NORTH__HALF_TOP__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: north, half: top, open: true, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_NORTH__HALF_TOP__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: north, half: top, open: true, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_NORTH__HALF_TOP__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: north, half: top, open: true, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_NORTH__HALF_TOP__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: north, half: top, open: false, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_NORTH__HALF_TOP__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: north, half: top, open: false, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_NORTH__HALF_TOP__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: north, half: top, open: false, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_NORTH__HALF_TOP__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: north, half: top, open: false, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_NORTH__HALF_TOP__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: north, half: bottom, open: true, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_NORTH__HALF_BOTTOM__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: north, half: bottom, open: true, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_NORTH__HALF_BOTTOM__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: north, half: bottom, open: true, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_NORTH__HALF_BOTTOM__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: north, half: bottom, open: true, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_NORTH__HALF_BOTTOM__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: north, half: bottom, open: false, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_NORTH__HALF_BOTTOM__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: north, half: bottom, open: false, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_NORTH__HALF_BOTTOM__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: north, half: bottom, open: false, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_NORTH__HALF_BOTTOM__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: north, half: bottom, open: false, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_NORTH__HALF_BOTTOM__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: south, half: top, open: true, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_SOUTH__HALF_TOP__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: south, half: top, open: true, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_SOUTH__HALF_TOP__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: south, half: top, open: true, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_SOUTH__HALF_TOP__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: south, half: top, open: true, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_SOUTH__HALF_TOP__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: south, half: top, open: false, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_SOUTH__HALF_TOP__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: south, half: top, open: false, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_SOUTH__HALF_TOP__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: south, half: top, open: false, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_SOUTH__HALF_TOP__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: south, half: top, open: false, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_SOUTH__HALF_TOP__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: south, half: bottom, open: true, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_SOUTH__HALF_BOTTOM__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: south, half: bottom, open: true, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_SOUTH__HALF_BOTTOM__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: south, half: bottom, open: true, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_SOUTH__HALF_BOTTOM__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: south, half: bottom, open: true, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_SOUTH__HALF_BOTTOM__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: south, half: bottom, open: false, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_SOUTH__HALF_BOTTOM__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: south, half: bottom, open: false, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_SOUTH__HALF_BOTTOM__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: south, half: bottom, open: false, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_SOUTH__HALF_BOTTOM__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: south, half: bottom, open: false, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_SOUTH__HALF_BOTTOM__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: west, half: top, open: true, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_WEST__HALF_TOP__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: west, half: top, open: true, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_WEST__HALF_TOP__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: west, half: top, open: true, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_WEST__HALF_TOP__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: west, half: top, open: true, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_WEST__HALF_TOP__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: west, half: top, open: false, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_WEST__HALF_TOP__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: west, half: top, open: false, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_WEST__HALF_TOP__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: west, half: top, open: false, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_WEST__HALF_TOP__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: west, half: top, open: false, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_WEST__HALF_TOP__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: west, half: bottom, open: true, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_WEST__HALF_BOTTOM__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: west, half: bottom, open: true, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_WEST__HALF_BOTTOM__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: west, half: bottom, open: true, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_WEST__HALF_BOTTOM__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: west, half: bottom, open: true, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_WEST__HALF_BOTTOM__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: west, half: bottom, open: false, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_WEST__HALF_BOTTOM__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: west, half: bottom, open: false, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_WEST__HALF_BOTTOM__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: west, half: bottom, open: false, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_WEST__HALF_BOTTOM__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: west, half: bottom, open: false, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_WEST__HALF_BOTTOM__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: east, half: top, open: true, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_EAST__HALF_TOP__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: east, half: top, open: true, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_EAST__HALF_TOP__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: east, half: top, open: true, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_EAST__HALF_TOP__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: east, half: top, open: true, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_EAST__HALF_TOP__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: east, half: top, open: false, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_EAST__HALF_TOP__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: east, half: top, open: false, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_EAST__HALF_TOP__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: east, half: top, open: false, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_EAST__HALF_TOP__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: east, half: top, open: false, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_EAST__HALF_TOP__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: east, half: bottom, open: true, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_EAST__HALF_BOTTOM__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: east, half: bottom, open: true, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_EAST__HALF_BOTTOM__OPEN_TRUE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: east, half: bottom, open: true, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_EAST__HALF_BOTTOM__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: east, half: bottom, open: true, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_EAST__HALF_BOTTOM__OPEN_TRUE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: east, half: bottom, open: false, powered: true, waterlogged: true", OAK_TRAPDOOR__FACING_EAST__HALF_BOTTOM__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: east, half: bottom, open: false, powered: true, waterlogged: false", OAK_TRAPDOOR__FACING_EAST__HALF_BOTTOM__OPEN_FALSE__POWERED_TRUE__WATERLOGGED_FALSE},
		{"oak_trapdoor facing: east, half: bottom, open: false, powered: false, waterlogged: true", OAK_TRAPDOOR__FACING_EAST__HALF_BOTTOM__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_TRUE},
		{"oak_trapdoor facing: east, half: bottom, open: false, powered: false, waterlogged: false", OAK_TRAPDOOR__FACING_EAST__HALF_BOTTOM__OPEN_FALSE__POWERED_FALSE__WATERLOGGED_FALSE},
		{"air", AIR},
		{"bedrock", BEDROCK},
	};

}
