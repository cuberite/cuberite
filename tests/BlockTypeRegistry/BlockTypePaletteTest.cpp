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
	TEST_EQUAL(pal.entry(0), (std::make_pair<AString, BlockState>("testblock", BlockState())));
	TEST_EQUAL(pal.entry(1), (std::make_pair<AString, BlockState>("another", BlockState())));
	TEST_EQUAL(pal.entry(2), (std::make_pair<AString, BlockState>("multistate", BlockState(bs1))));  // make_pair requires a copy of the state
	TEST_EQUAL(pal.entry(3), (std::make_pair<AString, BlockState>("multistate", BlockState(bs2))));
	TEST_EQUAL(pal.entry(4), (std::make_pair<AString, BlockState>("multistate", BlockState(bs3))));
}





/** Tests creating the transform map between two palettes. */
static void testTransform()
{
	LOGD("Testing the createTransformMap API...");

	// Create two palettes with some overlap:
	BlockTypePalette pal1, pal2;
	pal1.index("block1", BlockState());
	pal1.index("block2", BlockState());
	pal1.index("block3", BlockState());
	pal1.index("block4", BlockState());
	pal1.index("block5", BlockState("key1", "value1"));
	pal2.index("block0", BlockState());
	pal2.index("block2", BlockState());  // overlap
	pal2.index("block3", BlockState());  // overlap
	pal2.index("block4", BlockState("key1", "value1"));
	pal2.index("block5", BlockState("key1", "value1"));  // overlap
	pal2.index("block6", BlockState("key1", "value1"));

	// Check the transform map:
	auto trans = pal1.createTransformMap(pal2);
	TEST_EQUAL(pal1.maybeIndex("block1", BlockState()),                 (std::make_pair<UInt32, bool>(0, true)));
	TEST_EQUAL(pal1.maybeIndex("block2", BlockState()),                 (std::make_pair<UInt32, bool>(1, true)));
	TEST_EQUAL(pal1.maybeIndex("block3", BlockState()),                 (std::make_pair<UInt32, bool>(2, true)));
	TEST_EQUAL(pal1.maybeIndex("block4", BlockState()),                 (std::make_pair<UInt32, bool>(3, true)));
	TEST_EQUAL(pal1.maybeIndex("block5", BlockState("key1", "value1")), (std::make_pair<UInt32, bool>(4, true)));
	TEST_EQUAL(pal1.maybeIndex("block0", BlockState()),                 (std::make_pair<UInt32, bool>(5, true)));
	TEST_EQUAL(pal1.maybeIndex("block4", BlockState("key1", "value1")), (std::make_pair<UInt32, bool>(6, true)));
	TEST_EQUAL(pal1.maybeIndex("block6", BlockState("key1", "value1")), (std::make_pair<UInt32, bool>(7, true)));
	TEST_EQUAL(trans.size(), 6);
	TEST_EQUAL(trans[0], 5);
	TEST_EQUAL(trans[1], 1);
	TEST_EQUAL(trans[2], 2);
	TEST_EQUAL(trans[3], 6);
	TEST_EQUAL(trans[4], 4);
	TEST_EQUAL(trans[5], 7);
}





int main()
{
	LOGD("BlockTypePaletteTest started");

	try
	{
		testBasic();
		testTransform();
	}
	catch (const TestException & exc)
	{
		LOGERROR("BlockTypePaletteTest has failed, an exception was thrown: %s", exc.mMessage.c_str());
		return 1;
	}
	catch (const std::exception & exc)
	{
		LOGERROR("BlockTypePaletteTest has failed, an exception was thrown: %s", exc.what());
		return 1;
	}
	catch (...)
	{
		LOGERROR("BlockTypePaletteTest has failed, an unhandled exception was thrown.");
		return 1;
	}

	LOGD("BlockTypePaletteTest finished");
	return 0;
}




