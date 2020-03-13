#include "Globals.h"
#include "../TestHelpers.h"
#include "BlockTypePalette.h"
#include "Stopwatch.h"





/** Tests the BlockTypePalette's basic APIs - creation, addition, querying. */
static void testBasic()
{
	LOGD("Testing the basic BlockTypePalette's APIs...");

	// Check inserting different block type names:
	BlockTypePalette pal;
	TEST_EQUAL(pal.index("testblock", BlockState()), 0);  // Insert the first entry
	TEST_EQUAL(pal.index("testblock", BlockState()), 0);  // Check that it's not inserted again
	TEST_EQUAL(pal.maybeIndex("testblock", BlockState()), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(pal.maybeIndex("nonexistent", BlockState()).second, false);

	TEST_EQUAL(pal.index("another", BlockState()), 1);  // Insert the second entry
	TEST_EQUAL(pal.index("another", BlockState()), 1);  // Check that it's not inserted twice
	TEST_EQUAL(pal.maybeIndex("another", BlockState()), (std::make_pair<UInt32, bool>(1, true)));
	TEST_EQUAL(pal.maybeIndex("testblock", BlockState()), (std::make_pair<UInt32, bool>(0, true)));  // The first one stayed

	// Check same block type name, different BlockState:
	BlockState bs1;
	BlockState bs2("key1", "value1");
	BlockState bs3({{"key1", "value1"}, {"key2", "value2"}});
	BlockState bs2Copy(bs2);
	TEST_EQUAL(pal.index("multistate", bs1), 2);
	TEST_EQUAL(pal.index("multistate", bs2), 3);
	TEST_EQUAL(pal.index("multistate", bs3), 4);
	TEST_EQUAL(pal.index("multistate", bs2Copy), 3);  // Different BlockState instance, but same content
	TEST_EQUAL(pal.count(), 5);

	// Check the entry() API:
	TEST_EQUAL(pal.entry(0), (std::make_pair<AString, BlockState>("testblock",  BlockState())));
	TEST_EQUAL(pal.entry(1), (std::make_pair<AString, BlockState>("another",    BlockState())));
	TEST_EQUAL(pal.entry(2), (std::make_pair<AString, BlockState>("multistate", BlockState(bs1))));  // make_pair requires a copy of the state
	TEST_EQUAL(pal.entry(3), (std::make_pair<AString, BlockState>("multistate", BlockState(bs2))));
	TEST_EQUAL(pal.entry(4), (std::make_pair<AString, BlockState>("multistate", BlockState(bs3))));
	TEST_THROWS(pal.entry(5), BlockTypePalette::NoSuchIndexException);
}





/** Tests creating the transform map between two palettes. */
static void testTransformAddMissing()
{
	LOGD("Testing the createTransformMapAddMissing API...");

	// Create two palettes with some overlap:
	BlockTypePalette pal1, pal2;
	/* 0 */ pal1.index("block1", BlockState());
	/* 1 */ pal1.index("block2", BlockState());
	/* 2 */ pal1.index("block3", BlockState());
	/* 3 */ pal1.index("block4", BlockState());
	/* 4 */ pal1.index("block5", BlockState("key1", "value1"));
	/* 0 */ pal2.index("block0", BlockState());
	/* 1 */ pal2.index("block2", BlockState());  // overlap
	/* 2 */ pal2.index("block4", BlockState());  // overlap
	/* 3 */ pal2.index("block4", BlockState("key1", "value1"));
	/* 4 */ pal2.index("block5", BlockState("key1", "value1"));  // overlap
	/* 5 */ pal2.index("block6", BlockState("key1", "value1"));

	// Check the transform map:
	auto trans = pal1.createTransformMapAddMissing(pal2);
	TEST_EQUAL(pal1.maybeIndex("block1", BlockState()),                 (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(pal1.maybeIndex("block2", BlockState()),                 (std::make_pair<UInt32, bool>(1, true)));
	TEST_EQUAL(pal1.maybeIndex("block3", BlockState()),                 (std::make_pair<UInt32, bool>(2, true)));
	TEST_EQUAL(pal1.maybeIndex("block4", BlockState()),                 (std::make_pair<UInt32, bool>(3, true)));
	TEST_EQUAL(pal1.maybeIndex("block5", BlockState("key1", "value1")), (std::make_pair<UInt32, bool>(4, true)));
	TEST_EQUAL(pal1.maybeIndex("block0", BlockState()),                 (std::make_pair<UInt32, bool>(5, true)));
	TEST_EQUAL(pal1.maybeIndex("block4", BlockState("key1", "value1")), (std::make_pair<UInt32, bool>(6, true)));
	TEST_EQUAL(pal1.maybeIndex("block6", BlockState("key1", "value1")), (std::make_pair<UInt32, bool>(7, true)));
	TEST_EQUAL(trans.size(), 6);
	TEST_EQUAL(trans[0], 5);  // Added
	TEST_EQUAL(trans[1], 1);  // Mapped
	TEST_EQUAL(trans[2], 3);  // Mapped
	TEST_EQUAL(trans[3], 6);  // Added
	TEST_EQUAL(trans[4], 4);  // Mapped
	TEST_EQUAL(trans[5], 7);  // Added
}





/** Tests creating the transform map between two palettes, with fallback. */
static void testTransformWithFallback()
{
	LOGD("Testing the createTransformMapWithFallback API...");

	// Create two palettes with some overlap:
	BlockTypePalette pal1, pal2;
	/* 0 */ pal1.index("block1", BlockState());
	/* 1 */ pal1.index("block2", BlockState());
	/* 2 */ pal1.index("block3", BlockState());
	/* 3 */ pal1.index("block4", BlockState());
	/* 4 */ pal1.index("block5", BlockState("key1", "value1"));
	/* 0 */ pal2.index("block0", BlockState());
	/* 1 */ pal2.index("block2", BlockState());  // overlap
	/* 2 */ pal2.index("block4", BlockState());  // overlap
	/* 3 */ pal2.index("block4", BlockState("key1", "value1"));
	/* 4 */ pal2.index("block5", BlockState("key1", "value1"));  // overlap
	/* 5 */ pal2.index("block6", BlockState("key1", "value1"));

	// Check the transform map:
	auto trans = pal1.createTransformMapWithFallback(pal2, 0);
	TEST_EQUAL(trans.size(), 6);
	TEST_EQUAL(trans[0], 0);  // Fallback
	TEST_EQUAL(trans[1], 1);  // Mapped
	TEST_EQUAL(trans[2], 3);  // Mapped
	TEST_EQUAL(trans[3], 0);  // Fallback
	TEST_EQUAL(trans[4], 4);  // Mapped
	TEST_EQUAL(trans[5], 0);  // Fallback
}





/** Tests that loading fails for nonsense input */
static void testLoadErrors(void)
{
	LOG("Testing palette load error reporting.");

	BlockTypePalette palette;
	TEST_THROWS(palette.loadFromString(""),        BlockTypePalette::LoadFailedException);
	TEST_THROWS(palette.loadFromString("[]"),      BlockTypePalette::LoadFailedException);
	TEST_THROWS(palette.loadFromString("a = {}"),  BlockTypePalette::LoadFailedException);
	TEST_THROWS(palette.loadFromString("{x = 1}"), BlockTypePalette::LoadFailedException);  // Lua style
	TEST_THROWS(palette.loadFromString("$#^%&"),   BlockTypePalette::LoadFailedException);
}





/** Tests that loading a simple JSON palette succeeds. */
static void testLoadJsonSimple(void)
{
	LOG("Testing loading a simple JSON palette");

	BlockTypePalette palette;

	auto example = " \
	{ \
		\"minecraft:air\": { \
			\"states\": [ \
				{ \
					\"id\": 0, \
					\"default\": true \
				} \
			] \
		} \
	}";

	palette.loadFromString(example);
	TEST_EQUAL(palette.maybeIndex("minecraft:air", BlockState()), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:air", BlockState({{"foo", "baz"}})).second, false);
	TEST_EQUAL(palette.maybeIndex("minecraft:a",   BlockState()).second, false);
}





/** Tests loading a complex block with multiple states and duplicates. */
static void testLoadJsonComplex(void)
{
	LOG("Testing loading a complex JSON palette");
	BlockTypePalette palette;
	auto str = " \
	{ \
		\"minecraft:oak_sapling\": { \
		\"properties\": { \
			\"stage\": [ \
				\"0\", \
				\"1\" \
			] \
		}, \
			\"states\": [ \
				{ \
					\"properties\": { \
						\"stage\": \"0\" \
					}, \
					\"id\" : 21, \
					\"default\" : true \
				}, \
				{ \
					\"properties\": { \
						\"stage\": \"1\" \
					}, \
					\"id\" : 22 \
				}, \
				{ \
					\"properties\": { \
						\"stage\": \"1\" \
					}, \
					\"id\" : 23 \
				}\
			] \
		} \
	}";

	// Note: The palette has a duplicate entry with differrent IDs, the latter ID wins
	palette.loadFromString(str);
	TEST_EQUAL(palette.maybeIndex("minecraft:oak_sapling", {{"stage", "10"}}).second, false);
	TEST_EQUAL(palette.maybeIndex("minecraft:oak_sapling", {{"stage", "0"}}), (std::make_pair<UInt32, bool>(21, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:oak_sapling", {{"stage", "1"}}), (std::make_pair<UInt32, bool>(23, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:oak_sapling", {{"foo", "baz"}}).second, false);
	TEST_EQUAL(palette.maybeIndex("minecraft:oak_sap",     {{"stage", "0"}}).second, false);
}





/** Tests loading a palette from simple regular TSV text data. */
static void testLoadTsvRegular(void)
{
	LOG("Testing loading a simple regular TSV palette");
	BlockTypePalette palette;
	auto str = "\
BlockTypePalette\r\n\
FileVersion\t1\n\
CommonPrefix\tminecraft:\n\
\r\n\
0\tair\r\n\
1\tstone\n\
2\tgrass\tsnow_covered\t0\n\
3\tgrass\tsnow_covered\t1\n\
";
	palette.loadFromString(str);
	TEST_EQUAL(palette.maybeIndex("minecraft:air",   BlockState()), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:stone", BlockState()), (std::make_pair<UInt32, bool>(1, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:grass", BlockState({{"snow_covered", "0"}})), (std::make_pair<UInt32, bool>(2, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:grass", BlockState({{"snow_covered", "1"}})), (std::make_pair<UInt32, bool>(3, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:air",   BlockState({{"snow_covered", "0"}})).second, false);
	TEST_EQUAL(palette.maybeIndex("minecraft:grass", BlockState({{"snow_covered", "2"}})).second, false);
	TEST_EQUAL(palette.maybeIndex("minecraft:grass", BlockState()).second, false);
}





/** Tests loading a palette from simple upgrade TSV text data. */
static void testLoadTsvUpgrade(void)
{
	LOG("Testing loading a simple upgrade TSV palette");
	BlockTypePalette palette;
	auto str = "\
UpgradeBlockTypePalette\r\n\
FileVersion\t1\n\
CommonPrefix\tminecraft:\r\n\
\n\
0\t0\tair\r\n\
1\t0\tstone\n\
2\t0\tgrass\tsnow_covered\t0\n\
2\t1\tgrass\tsnow_covered\t1\n\
";
	palette.loadFromString(str);
	TEST_EQUAL(palette.maybeIndex("minecraft:air",   BlockState()), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:stone", BlockState()), (std::make_pair<UInt32, bool>(16, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:grass", BlockState({{"snow_covered", "0"}})), (std::make_pair<UInt32, bool>(32, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:grass", BlockState({{"snow_covered", "1"}})), (std::make_pair<UInt32, bool>(33, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:air",   BlockState({{"snow_covered", "0"}})).second, false);
	TEST_EQUAL(palette.maybeIndex("minecraft:grass", BlockState({{"snow_covered", "2"}})).second, false);
	TEST_EQUAL(palette.maybeIndex("minecraft:grass", BlockState()).second, false);
}





/** Tests loading a palette from a real-life protocol base file (1.13). */
static void testLoadFromBaseFile(void)
{
	LOG("Testing loading a palette from file \"base.btp.txt\" (1.13)");
	BlockTypePalette palette;
	{
		auto fileContents = cFile::ReadWholeFile("base.btp.txt");
		cStopwatch sw("Loading palette");
		palette.loadFromString(fileContents);
	}

	TEST_EQUAL(palette.maybeIndex("minecraft:air",   BlockState()), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:stone", BlockState()), (std::make_pair<UInt32, bool>(1, true)));
	TEST_EQUAL(
		palette.maybeIndex(
			"minecraft:dark_oak_leaves",
			BlockState({{"persistent", "false"}, {"distance", "6"}})
		),
		(std::make_pair<UInt32, bool>(225, true))
	);
	TEST_EQUAL(
		palette.maybeIndex(
			"minecraft:dark_oak_leaves",
			BlockState({{"persistent", "false"}})
		).second,
		false
	);
}





/** Tests loading an upgrade-palette from a real-life upgrade file. */
static void testLoadFromUpgradeFile(void)
{
	LOG("Testing loading an upgrade palette from file \"UpgradeBlockTypePalette.txt\".");
	BlockTypePalette palette;
	{
		auto fileContents = cFile::ReadWholeFile("UpgradeBlockTypePalette.txt");
		cStopwatch sw("Loading upgrade palette");
		palette.loadFromString(fileContents);
	}

	TEST_EQUAL(palette.entry(0), (std::make_pair<AString, BlockState>("minecraft:air", {})));
	TEST_EQUAL(palette.entry(44 * 16 + 8), (std::make_pair<AString, BlockState>("minecraft:stone_slab", {{"type", "top"}})));
}





IMPLEMENT_TEST_MAIN("BlockTypePalette",
	testBasic();
	testTransformAddMissing();
	testTransformWithFallback();
	testLoadErrors();
	testLoadJsonSimple();
	testLoadJsonComplex();
	testLoadTsvRegular();
	testLoadTsvUpgrade();
	testLoadFromBaseFile();
	testLoadFromUpgradeFile();
)




