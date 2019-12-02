#include "Globals.h"
#include "../TestHelpers.h"
#include "BlockTypePalette.h"





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





/** Tests that loading a simple JSON palette succeeds. */
static void testLoadSimpleSuccess(void)
{
	LOG("Testing loading a simple JSON palette");

	BlockTypePalette palette;

	auto example = "{\"Metadata\":{\"ProtocolBlockTypePaletteVersion\":1}, \"Palette\":[{\
		\"props\": {\
			\"foo\": \"bar\"\
		}, \
		\"name\": \"b\", \
		\"id\": \"0\"\
	}]}";

	palette.loadFromString(example);
	TEST_EQUAL(palette.maybeIndex("b", BlockState({{"foo", "bar"}})), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(palette.maybeIndex("b", BlockState({{"foo", "baz"}})), (std::make_pair<UInt32, bool>(0, false)));
	TEST_EQUAL(palette.maybeIndex("a", BlockState({{"foo", "bar"}})), (std::make_pair<UInt32, bool>(0, false)));
}





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





static void testLoadComplex1(void)
{
	LOG("Testing loading a complex palette (1)");
	BlockTypePalette palette;
	auto str = "{\"Metadata\":{\"ProtocolBlockTypePaletteVersion\":1}, \"Palette\":[{\
		\"props\": {\
			\"foo\": \"bar\", \
			\"moo\": \"baz\"\
		}, \
		\"id\": \"0\", \
		\"name\": \"b\"\
	}, {\
		\"props\": {\
			\"foo\": \"baz\", \
			\"moo\": \"bar\"\
		}, \
		\"id\": \"1\", \
		\"name\": \"b\"\
	}, {\
		\"props\": {\
			\"foo\": \"baz\", \
			\"moo\": \"bar\"\
		}, \
		\"id\": \"1001\", \
		\"name\": \"b\"\
	}]}";
	// Note: The palette has a duplicate entry with differrent IDs, the latter ID wins
	palette.loadFromString(str);
	TEST_EQUAL(palette.maybeIndex("b", {{"foo", "bar"}}).second, false);
	TEST_EQUAL(palette.maybeIndex("b", {{"foo", "bar"}, {"moo", "baz"}}), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(palette.maybeIndex("b", {{"foo", "baz"}, {"moo", "bar"}}), (std::make_pair<UInt32, bool>(1001, true)));
	TEST_EQUAL(palette.maybeIndex("c", {{"foo", "baz"}, {"moo", "bar"}}).second, false);
}





static void testLoadComplex2(void)
{
	LOG("Testing loading a complex palette (2)");
	BlockTypePalette palette;
	auto str = "{\"Metadata\":{\"ProtocolBlockTypePaletteVersion\":1}, \"Palette\":[{\
		\"id\": \"0\", \
		\"name\": \"a\"\
	}, {\
		\"id\": \"1\", \
		\"name\": \"b\"\
	}]}";
	palette.loadFromString(str);
	TEST_EQUAL(palette.maybeIndex("a", BlockState()), (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(palette.maybeIndex("b", BlockState()), (std::make_pair<UInt32, bool>(1, true)));
}





static void testLoadFromFile1(void)
{
	LOG("Testing loading a palette from file \"test.btp.json\"");
	BlockTypePalette palette;
	palette.loadFromString(cFile::ReadWholeFile("test.btp.json"));

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





static void testLoadFromFile2(void)
{
	LOG("Testing loading a palette from file \"base.btp.json\" (version 1.13)");
	BlockTypePalette palette;
	palette.loadFromString(cFile::ReadWholeFile("base.btp.json"));

	TEST_EQUAL(palette.maybeIndex("minecraft:air",   BlockState()), (std::make_pair<UInt32, bool>(0,  true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:stone", BlockState()), (std::make_pair<UInt32, bool>(1,  true)));
	TEST_EQUAL(palette.maybeIndex("minecraft:dirt",  BlockState()), (std::make_pair<UInt32, bool>(10, true)));
}





IMPLEMENT_TEST_MAIN("BlockTypePalette",
	testBasic();
	testTransformAddMissing();
	testTransformWithFallback();
	testLoadSimpleSuccess();
	testLoadErrors();
	testLoadComplex1();
	testLoadComplex2();
	testLoadFromFile1();
	testLoadFromFile2();
)




