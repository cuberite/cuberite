#include "Globals.h"
#include "PalettedBlockArea.h"
#include "../TestHelpers.h"





/** Tests creating a PBA. */
static void testCreation()
{
	LOG("Testing PBA creation...");

	// Check failures:
	TEST_ASSERTS(PalettedBlockArea::createFilled({-2, 3, 4}, "block", BlockState()));  // Negative coords
	TEST_THROWS(PalettedBlockArea::createFilled({4096, 4096, 4096}, "block", BlockState()), std::runtime_error);  // Size too large for UInt32

	// Check that a created area really is filled:
	auto pba = PalettedBlockArea::createFilled({2, 3, 4}, "block", BlockState());
	TEST_EQUAL(pba.size(), Vector3i(2, 3, 4));
	TEST_EQUAL(pba.whole(), cCuboid({0, 0, 0}, {2, 3, 4}));
	TEST_EQUAL(pba.palette().count(), 1);
	TEST_EQUAL(pba.maybePaletteIndex("block", BlockState()), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(pba.maybePaletteIndex("nonexistentBlock", BlockState()).second, false);
	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 3; ++y)
		{
			for (int z = 0; z < 4; ++z)
			{
				TEST_EQUAL(pba.blockPaletteIndex({x, y, z}), 0);
			}
		}
	}
}





/** Tests setting and getting blocks. */
static void testSetting()
{
	LOG("Testing PBA's set and get APIs...");
	auto pba = PalettedBlockArea::createFilled({2, 3, 4}, "block1", BlockState());
	pba.setBlock({0, 0, 0}, "block2", BlockState());
	pba.setBlock({1, 0, 0}, "block2", BlockState("key1", "value1"));
	TEST_ASSERTS(pba.setBlock({2, 0, 0}, "block2", BlockState()));  // Invalid coords
	pba.setBlock({0, 1, 0}, 1);
	TEST_ASSERTS(pba.setBlock({1, 1, 0}, 100));  // Invalid palette index

	// Check that the blocks have been set:
	TEST_EQUAL(pba.palette().count(), 3);
	TEST_EQUAL(pba.block({0, 0, 0}), (std::make_pair<AString, BlockState>("block2", BlockState())));
	TEST_EQUAL(pba.block({1, 0, 0}), (std::make_pair<AString, BlockState>("block2", BlockState("key1", "value1"))));
	TEST_ASSERTS(pba.block({2, 0, 0}));  // Invalid coords
	TEST_EQUAL(pba.block({0, 1, 0}), (std::make_pair<AString, BlockState>("block2", BlockState())));
	TEST_EQUAL(pba.block({1, 1, 0}), (std::make_pair<AString, BlockState>("block1", BlockState())));  // Didn't overwrite with invalid palette index
	TEST_EQUAL(pba.blockPaletteIndex({0, 0, 0}), 1);
	TEST_EQUAL(pba.blockPaletteIndex({1, 0, 0}), 2);
	TEST_ASSERTS(pba.blockPaletteIndex({2, 0, 0}));  // Invalid coords
	TEST_EQUAL(pba.blockPaletteIndex({0, 1, 0}), 1);
	TEST_EQUAL(pba.blockPaletteIndex({1, 1, 0}), 0);  // Didn't overwrite with invalid palette index

	// Test filling:
	LOG("Testing PBA's fill API...");
	pba.fill("block3", BlockState("key1", "value1"));
	TEST_EQUAL(pba.palette().count(), 1);
	TEST_EQUAL(pba.paletteEntry(0), (std::make_pair<AString, BlockState>("block3", BlockState("key1", "value1"))));
	for (int x = 0; x < 2; ++x)
	{
		for (int y = 0; y < 2; ++y)
		{
			for (int z = 0; z < 2; ++z)
			{
				TEST_EQUAL(pba.blockPaletteIndex({x, y, z}), 0);
			}
		}
	}
}





/** Creates pbaA and pbaB that are pre-filled with known content.
The PBAs are then used for paste()-testing.
Used to be a function, but clang-3.5 didn't like it ("error: debug information for auto is not yet supported"). */
#define PREPARE_PASTING_PBAS \
	auto pbaA = PalettedBlockArea::createFilled({5, 5, 5}, "blockA", BlockState()); \
	for (int x = 0; x < 5; ++x) \
	{ \
		for (int y = 0; y < 5; ++y) \
		{ \
			for (int z = 0; z < 5; ++z) \
			{ \
				pbaA.setBlock({x, y, z}, Printf("A-%d-%d-%d", x, y, z), BlockState()); \
			} \
		} \
	} \
	auto pbaB = PalettedBlockArea::createFilled({6, 6, 6}, "blockB", BlockState()); \
	for (int x = 0; x < 6; ++x) \
	{ \
		for (int y = 0; y < 6; ++y) \
		{ \
			for (int z = 0; z < 6; ++z) \
			{ \
				pbaB.setBlock({x, y, z}, Printf("B-%d-%d-%d", x, y, z), BlockState()); \
			} \
		} \
	} \
	do { } while (false)





// This is the data for the original PBA, before the paste() operations.
// It is included here so that when adding new paste() tests we can simply copy it
// into the test function and modify for the test.
/*
static const AString expected[5][5][5] =
{
	{
		{"A-0-0-0", "A-1-0-0", "A-2-0-0", "A-3-0-0", "A-4-0-0"},
		{"A-0-1-0", "A-1-1-0", "A-2-1-0", "A-3-1-0", "A-4-1-0"},
		{"A-0-2-0", "A-1-2-0", "A-2-2-0", "A-3-2-0", "A-4-2-0"},
		{"A-0-3-0", "A-1-3-0", "A-2-3-0", "A-3-3-0", "A-4-3-0"},
		{"A-0-4-0", "A-1-4-0", "A-2-4-0", "A-3-4-0", "A-4-4-0"},
	},
	{
		{"A-0-0-1", "A-1-0-1", "A-2-0-1", "A-3-0-1", "A-4-0-1"},
		{"A-0-1-1", "A-1-1-1", "A-2-1-1", "A-3-1-1", "A-4-1-1"},
		{"A-0-2-1", "A-1-2-1", "A-2-2-1", "A-3-2-1", "A-4-2-1"},
		{"A-0-3-1", "A-1-3-1", "A-2-3-1", "A-3-3-1", "A-4-3-1"},
		{"A-0-4-1", "A-1-4-1", "A-2-4-1", "A-3-4-1", "A-4-4-1"},
	},
	{
		{"A-0-0-2", "A-1-0-2", "A-2-0-2", "A-3-0-2", "A-4-0-2"},
		{"A-0-1-2", "A-1-1-2", "A-2-1-2", "A-3-1-2", "A-4-1-2"},
		{"A-0-2-2", "A-1-2-2", "A-2-2-2", "A-3-2-2", "A-4-2-2"},
		{"A-0-3-2", "A-1-3-2", "A-2-3-2", "A-3-3-2", "A-4-3-2"},
		{"A-0-4-2", "A-1-4-2", "A-2-4-2", "A-3-4-2", "A-4-4-2"},
	},
	{
		{"A-0-0-3", "A-1-0-3", "A-2-0-3", "A-3-0-3", "A-4-0-3"},
		{"A-0-1-3", "A-1-1-3", "A-2-1-3", "A-3-1-3", "A-4-1-3"},
		{"A-0-2-3", "A-1-2-3", "A-2-2-3", "A-3-2-3", "A-4-2-3"},
		{"A-0-3-3", "A-1-3-3", "A-2-3-3", "A-3-3-3", "A-4-3-3"},
		{"A-0-4-3", "A-1-4-3", "A-2-4-3", "A-3-4-3", "A-4-4-3"},
	},
	{
		{"A-0-0-4", "A-1-0-4", "A-2-0-4", "A-3-0-4", "A-4-0-4"},
		{"A-0-1-4", "A-1-1-4", "A-2-1-4", "A-3-1-4", "A-4-1-4"},
		{"A-0-2-4", "A-1-2-4", "A-2-2-4", "A-3-2-4", "A-4-2-4"},
		{"A-0-3-4", "A-1-3-4", "A-2-3-4", "A-3-3-4", "A-4-3-4"},
		{"A-0-4-4", "A-1-4-4", "A-2-4-4", "A-3-4-4", "A-4-4-4"},
	},
};
*/





/** Tests the "paste()" operation with the pasted region being completely inside the destination PBA. */
static void testPastingCompletelyInside()
{
	LOG("Testing the paste() API with destination completely inside (with cropping)...");
	PREPARE_PASTING_PBAS;
	pbaA.paste(pbaB, cCuboid({1, 1, 1}, {4, 4, 4}), {1, 0, 0});  // Paste the 3x3x3 inside area from pbaB to pbaA, starting at {1, 0, 0}
	static const AString expected[5][5][5] =
	{
		{
			{"A-0-0-0", "B-1-1-1", "B-2-1-1", "B-3-1-1", "A-4-0-0"},
			{"A-0-1-0", "B-1-2-1", "B-2-2-1", "B-3-2-1", "A-4-1-0"},
			{"A-0-2-0", "B-1-3-1", "B-2-3-1", "B-3-3-1", "A-4-2-0"},
			{"A-0-3-0", "A-1-3-0", "A-2-3-0", "A-3-3-0", "A-4-3-0"},
			{"A-0-4-0", "A-1-4-0", "A-2-4-0", "A-3-4-0", "A-4-4-0"},
		},
		{
			{"A-0-0-1", "B-1-1-2", "B-2-1-2", "B-3-1-2", "A-4-0-1"},
			{"A-0-1-1", "B-1-2-2", "B-2-2-2", "B-3-2-2", "A-4-1-1"},
			{"A-0-2-1", "B-1-3-2", "B-2-3-2", "B-3-3-2", "A-4-2-1"},
			{"A-0-3-1", "A-1-3-1", "A-2-3-1", "A-3-3-1", "A-4-3-1"},
			{"A-0-4-1", "A-1-4-1", "A-2-4-1", "A-3-4-1", "A-4-4-1"},
		},
		{
			{"A-0-0-2", "B-1-1-3", "B-2-1-3", "B-3-1-3", "A-4-0-2"},
			{"A-0-1-2", "B-1-2-3", "B-2-2-3", "B-3-2-3", "A-4-1-2"},
			{"A-0-2-2", "B-1-3-3", "B-2-3-3", "B-3-3-3", "A-4-2-2"},
			{"A-0-3-2", "A-1-3-2", "A-2-3-2", "A-3-3-2", "A-4-3-2"},
			{"A-0-4-2", "A-1-4-2", "A-2-4-2", "A-3-4-2", "A-4-4-2"},
		},
		{
			{"A-0-0-3", "A-1-0-3", "A-2-0-3", "A-3-0-3", "A-4-0-3"},
			{"A-0-1-3", "A-1-1-3", "A-2-1-3", "A-3-1-3", "A-4-1-3"},
			{"A-0-2-3", "A-1-2-3", "A-2-2-3", "A-3-2-3", "A-4-2-3"},
			{"A-0-3-3", "A-1-3-3", "A-2-3-3", "A-3-3-3", "A-4-3-3"},
			{"A-0-4-3", "A-1-4-3", "A-2-4-3", "A-3-4-3", "A-4-4-3"},
		},
		{
			{"A-0-0-4", "A-1-0-4", "A-2-0-4", "A-3-0-4", "A-4-0-4"},
			{"A-0-1-4", "A-1-1-4", "A-2-1-4", "A-3-1-4", "A-4-1-4"},
			{"A-0-2-4", "A-1-2-4", "A-2-2-4", "A-3-2-4", "A-4-2-4"},
			{"A-0-3-4", "A-1-3-4", "A-2-3-4", "A-3-3-4", "A-4-3-4"},
			{"A-0-4-4", "A-1-4-4", "A-2-4-4", "A-3-4-4", "A-4-4-4"},
		},
	};
	for (int x = 0; x < 5; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			for (int z = 0; z < 5; ++z)
			{
				auto got = pbaA.block({x, y, z}).first;
				TEST_EQUAL_MSG(
					pbaA.block({x, y, z}).first,
					expected[z][y][x],
					Printf("{%d, %d, %d}, exp %s, got %s", x, y, z, expected[z][y][x].c_str(), pbaA.block({x, y, z}).first.c_str()).c_str()
				);
			}
		}
	}
}





/** Tests the "paste()" operation with the pasted region overflowing the destination PBA into the positive coords. */
static void testPastingPositiveOverflow()
{
	LOG("Testing the paste() API with positive overflow...");
	PREPARE_PASTING_PBAS;
	pbaA.paste(pbaB, Vector3i{3, 2, 1});  // Paste the entire pbaB to pbaA, starting at {3, 2, 1}
	static const AString expected[5][5][5] =
	{
		{
			{"A-0-0-0", "A-1-0-0", "A-2-0-0", "A-3-0-0", "A-4-0-0"},
			{"A-0-1-0", "A-1-1-0", "A-2-1-0", "A-3-1-0", "A-4-1-0"},
			{"A-0-2-0", "A-1-2-0", "A-2-2-0", "A-3-2-0", "A-4-2-0"},
			{"A-0-3-0", "A-1-3-0", "A-2-3-0", "A-3-3-0", "A-4-3-0"},
			{"A-0-4-0", "A-1-4-0", "A-2-4-0", "A-3-4-0", "A-4-4-0"},
		},
		{
			{"A-0-0-1", "A-1-0-1", "A-2-0-1", "A-3-0-1", "A-4-0-1"},
			{"A-0-1-1", "A-1-1-1", "A-2-1-1", "A-3-1-1", "A-4-1-1"},
			{"A-0-2-1", "A-1-2-1", "A-2-2-1", "B-0-0-0", "B-1-0-0"},
			{"A-0-3-1", "A-1-3-1", "A-2-3-1", "B-0-1-0", "B-1-1-0"},
			{"A-0-4-1", "A-1-4-1", "A-2-4-1", "B-0-2-0", "B-1-2-0"},
		},
		{
			{"A-0-0-2", "A-1-0-2", "A-2-0-2", "A-3-0-2", "A-4-0-2"},
			{"A-0-1-2", "A-1-1-2", "A-2-1-2", "A-3-1-2", "A-4-1-2"},
			{"A-0-2-2", "A-1-2-2", "A-2-2-2", "B-0-0-1", "B-1-0-1"},
			{"A-0-3-2", "A-1-3-2", "A-2-3-2", "B-0-1-1", "B-1-1-1"},
			{"A-0-4-2", "A-1-4-2", "A-2-4-2", "B-0-2-1", "B-1-2-1"},
		},
		{
			{"A-0-0-3", "A-1-0-3", "A-2-0-3", "A-3-0-3", "A-4-0-3"},
			{"A-0-1-3", "A-1-1-3", "A-2-1-3", "A-3-1-3", "A-4-1-3"},
			{"A-0-2-3", "A-1-2-3", "A-2-2-3", "B-0-0-2", "B-1-0-2"},
			{"A-0-3-3", "A-1-3-3", "A-2-3-3", "B-0-1-2", "B-1-1-2"},
			{"A-0-4-3", "A-1-4-3", "A-2-4-3", "B-0-2-2", "B-1-2-2"},
		},
		{
			{"A-0-0-4", "A-1-0-4", "A-2-0-4", "A-3-0-4", "A-4-0-4"},
			{"A-0-1-4", "A-1-1-4", "A-2-1-4", "A-3-1-4", "A-4-1-4"},
			{"A-0-2-4", "A-1-2-4", "A-2-2-4", "B-0-0-3", "B-1-0-3"},
			{"A-0-3-4", "A-1-3-4", "A-2-3-4", "B-0-1-3", "B-1-1-3"},
			{"A-0-4-4", "A-1-4-4", "A-2-4-4", "B-0-2-3", "B-1-2-3"},
		},
	};
	for (int x = 0; x < 5; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			for (int z = 0; z < 5; ++z)
			{
				auto got = pbaA.block({x, y, z}).first;
				TEST_EQUAL_MSG(
					pbaA.block({x, y, z}).first,
					expected[z][y][x],
					Printf("{%d, %d, %d}, exp %s, got %s", x, y, z, expected[z][y][x].c_str(), pbaA.block({x, y, z}).first.c_str()).c_str()
				);
			}
		}
	}
}





/** Tests the "paste()" operation with the pasted region overflowing the destination PBA into the negative coords. */
static void testPastingNegativeOverflow()
{
	LOG("Testing the paste() API with negative overflow...");
	PREPARE_PASTING_PBAS;
	pbaA.paste(pbaB, Vector3i{-4, -3, -2});  // Paste the entire pbaB to pbaA, starting at {-4, -3, -2}
	static const AString expected[5][5][5] =
	{
		{
			{"B-4-3-2", "B-5-3-2", "A-2-0-0", "A-3-0-0", "A-4-0-0"},
			{"B-4-4-2", "B-5-4-2", "A-2-1-0", "A-3-1-0", "A-4-1-0"},
			{"B-4-5-2", "B-5-5-2", "A-2-2-0", "A-3-2-0", "A-4-2-0"},
			{"A-0-3-0", "A-1-3-0", "A-2-3-0", "A-3-3-0", "A-4-3-0"},
			{"A-0-4-0", "A-1-4-0", "A-2-4-0", "A-3-4-0", "A-4-4-0"},
		},
		{
			{"B-4-3-3", "B-5-3-3", "A-2-0-1", "A-3-0-1", "A-4-0-1"},
			{"B-4-4-3", "B-5-4-3", "A-2-1-1", "A-3-1-1", "A-4-1-1"},
			{"B-4-5-3", "B-5-5-3", "A-2-2-1", "A-3-2-1", "A-4-2-1"},
			{"A-0-3-1", "A-1-3-1", "A-2-3-1", "A-3-3-1", "A-4-3-1"},
			{"A-0-4-1", "A-1-4-1", "A-2-4-1", "A-3-4-1", "A-4-4-1"},
		},
		{
			{"B-4-3-4", "B-5-3-4", "A-2-0-2", "A-3-0-2", "A-4-0-2"},
			{"B-4-4-4", "B-5-4-4", "A-2-1-2", "A-3-1-2", "A-4-1-2"},
			{"B-4-5-4", "B-5-5-4", "A-2-2-2", "A-3-2-2", "A-4-2-2"},
			{"A-0-3-2", "A-1-3-2", "A-2-3-2", "A-3-3-2", "A-4-3-2"},
			{"A-0-4-2", "A-1-4-2", "A-2-4-2", "A-3-4-2", "A-4-4-2"},
		},
		{
			{"B-4-3-5", "B-5-3-5", "A-2-0-3", "A-3-0-3", "A-4-0-3"},
			{"B-4-4-5", "B-5-4-5", "A-2-1-3", "A-3-1-3", "A-4-1-3"},
			{"B-4-5-5", "B-5-5-5", "A-2-2-3", "A-3-2-3", "A-4-2-3"},
			{"A-0-3-3", "A-1-3-3", "A-2-3-3", "A-3-3-3", "A-4-3-3"},
			{"A-0-4-3", "A-1-4-3", "A-2-4-3", "A-3-4-3", "A-4-4-3"},
		},
		{
			{"A-0-0-4", "A-1-0-4", "A-2-0-4", "A-3-0-4", "A-4-0-4"},
			{"A-0-1-4", "A-1-1-4", "A-2-1-4", "A-3-1-4", "A-4-1-4"},
			{"A-0-2-4", "A-1-2-4", "A-2-2-4", "A-3-2-4", "A-4-2-4"},
			{"A-0-3-4", "A-1-3-4", "A-2-3-4", "A-3-3-4", "A-4-3-4"},
			{"A-0-4-4", "A-1-4-4", "A-2-4-4", "A-3-4-4", "A-4-4-4"},
		},
	};
	for (int x = 0; x < 5; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			for (int z = 0; z < 5; ++z)
			{
				auto got = pbaA.block({x, y, z}).first;
				TEST_EQUAL_MSG(
					pbaA.block({x, y, z}).first,
					expected[z][y][x],
					Printf("{%d, %d, %d}, exp %s, got %s", x, y, z, expected[z][y][x].c_str(), pbaA.block({x, y, z}).first.c_str()).c_str()
				);
			}
		}
	}
}





/** Tests the "paste()" operation with the pasted region overflowing the destination PBA into mixed positive and negative coords. */
static void testPastingMixedOverflow()
{
	LOG("Testing the paste() API with mixed positive and negative overflow...");
	PREPARE_PASTING_PBAS;
	pbaA.paste(pbaB, Vector3i{-4, -3, 2});  // Paste the entire pbaB to pbaA, starting at {-4, -3, 2}
	static const AString expected[5][5][5] =
	{
		{
			{"A-0-0-0", "A-1-0-0", "A-2-0-0", "A-3-0-0", "A-4-0-0"},
			{"A-0-1-0", "A-1-1-0", "A-2-1-0", "A-3-1-0", "A-4-1-0"},
			{"A-0-2-0", "A-1-2-0", "A-2-2-0", "A-3-2-0", "A-4-2-0"},
			{"A-0-3-0", "A-1-3-0", "A-2-3-0", "A-3-3-0", "A-4-3-0"},
			{"A-0-4-0", "A-1-4-0", "A-2-4-0", "A-3-4-0", "A-4-4-0"},
		},
		{
			{"A-0-0-1", "A-1-0-1", "A-2-0-1", "A-3-0-1", "A-4-0-1"},
			{"A-0-1-1", "A-1-1-1", "A-2-1-1", "A-3-1-1", "A-4-1-1"},
			{"A-0-2-1", "A-1-2-1", "A-2-2-1", "A-3-2-1", "A-4-2-1"},
			{"A-0-3-1", "A-1-3-1", "A-2-3-1", "A-3-3-1", "A-4-3-1"},
			{"A-0-4-1", "A-1-4-1", "A-2-4-1", "A-3-4-1", "A-4-4-1"},
		},
		{
			{"B-4-3-0", "B-5-3-0", "A-2-0-2", "A-3-0-2", "A-4-0-2"},
			{"B-4-4-0", "B-5-4-0", "A-2-1-2", "A-3-1-2", "A-4-1-2"},
			{"B-4-5-0", "B-5-5-0", "A-2-2-2", "A-3-2-2", "A-4-2-2"},
			{"A-0-3-2", "A-1-3-2", "A-2-3-2", "A-3-3-2", "A-4-3-2"},
			{"A-0-4-2", "A-1-4-2", "A-2-4-2", "A-3-4-2", "A-4-4-2"},
		},
		{
			{"B-4-3-1", "B-5-3-1", "A-2-0-3", "A-3-0-3", "A-4-0-3"},
			{"B-4-4-1", "B-5-4-1", "A-2-1-3", "A-3-1-3", "A-4-1-3"},
			{"B-4-5-1", "B-5-5-1", "A-2-2-3", "A-3-2-3", "A-4-2-3"},
			{"A-0-3-3", "A-1-3-3", "A-2-3-3", "A-3-3-3", "A-4-3-3"},
			{"A-0-4-3", "A-1-4-3", "A-2-4-3", "A-3-4-3", "A-4-4-3"},
		},
		{
			{"B-4-3-2", "B-5-3-2", "A-2-0-4", "A-3-0-4", "A-4-0-4"},
			{"B-4-4-2", "B-5-4-2", "A-2-1-4", "A-3-1-4", "A-4-1-4"},
			{"B-4-5-2", "B-5-5-2", "A-2-2-4", "A-3-2-4", "A-4-2-4"},
			{"A-0-3-4", "A-1-3-4", "A-2-3-4", "A-3-3-4", "A-4-3-4"},
			{"A-0-4-4", "A-1-4-4", "A-2-4-4", "A-3-4-4", "A-4-4-4"},
		},
	};
	for (int x = 0; x < 5; ++x)
	{
		for (int y = 0; y < 5; ++y)
		{
			for (int z = 0; z < 5; ++z)
			{
				auto got = pbaA.block({x, y, z}).first;
				TEST_EQUAL_MSG(
					pbaA.block({x, y, z}).first,
					expected[z][y][x],
					Printf("{%d, %d, %d}, exp %s, got %s", x, y, z, expected[z][y][x].c_str(), pbaA.block({x, y, z}).first.c_str()).c_str()
				);
			}
		}
	}
}





IMPLEMENT_TEST_MAIN("PalettedBlockArea",
	testCreation();
	testSetting();
	testPastingCompletelyInside();
	testPastingPositiveOverflow();
	testPastingNegativeOverflow();
	testPastingMixedOverflow();
)
