#include "Globals.h"
#include "BlockState.h"
#include "../TestHelpers.h"




/** Tests the class constructors with static (hard-coded) data. */
static void testStaticCreation()
{
	LOGD("Testing BlockState creation from static data...");

	// Create a few BlockStates using the static-data constructors:
	BlockState bs1v1;
	BlockState bs2v1("property", "value");
	BlockState bs3v1({{"property1", "value1"}, {"property2", "value2"}});
	BlockState bs1v2(bs1v1);
	BlockState bs2v2(bs2v1);
	BlockState bs3v2(bs3v1);
	BlockState bs1v3(bs1v2, {{"added property", "value1"}});
	BlockState bs2v3(bs2v2, {{"added property", "value2"}});
	BlockState bs3v3(bs3v2, {{"added property", "value3"}});

	// Test (in-)equality (v1 = v2 != v3):
	TEST_EQUAL(bs1v1, bs1v2);
	TEST_EQUAL(bs2v1, bs2v2);
	TEST_EQUAL(bs3v1, bs3v2);
	TEST_NOTEQUAL(bs1v1, bs1v3);
	TEST_NOTEQUAL(bs2v1, bs2v3);
	TEST_NOTEQUAL(bs3v1, bs3v3);
	TEST_NOTEQUAL(bs1v2, bs1v3);
	TEST_NOTEQUAL(bs2v2, bs2v3);
	TEST_NOTEQUAL(bs3v2, bs3v3);

	// Test that the values are actually stored:
	TEST_EQUAL(bs1v1.value("property"), "");
	TEST_EQUAL(bs2v1.value("property"), "value");
	TEST_EQUAL(bs2v1.value("property1"), "");
	TEST_EQUAL(bs3v1.value("property1"), "value1");
	TEST_EQUAL(bs3v1.value("property2"), "value2");
	TEST_EQUAL(bs1v3.value("added property"), "value1");
	TEST_EQUAL(bs2v3.value("added property"), "value2");
	TEST_EQUAL(bs3v3.value("added property"), "value3");
}





/** Tests the dynamic-data constructors (map param, deep-copy). */
static void testDynamicCreation()
{
	LOGD("Testing BlockState creation from dynamic data...");

	using Map = std::map<AString, AString>;

	// Construct from scratch:
	{
		BlockState bs1a({{"property", "value"}});
		Map map1({{"property", "value"}});
		BlockState bs1b(map1);
		TEST_EQUAL(bs1a, bs1b);  // Creation works
		map1.clear();
		TEST_EQUAL(bs1a, bs1b);  // Created a copy independent of map1
	}

	// Construct by moving:
	{
		BlockState bs2a({{"property", "value"}});
		Map map2({{"property", "value"}});
		BlockState bs2b(std::move(map2));
		TEST_EQUAL(bs2a, bs2b);  // Creation works
	}

	// Construct by modifying:
	{
		BlockState bsSrc("property1", "value1");
		BlockState bs3a(bsSrc, {{"property2", "value2"}});
		Map map3({{"property2", "value2"}});
		BlockState bs3b(bsSrc, map3);
		TEST_EQUAL(bs3a, bs3b);
		map3.clear();
		TEST_EQUAL(bs3a, bs3b);
	}
}





/** Tests replacing the properties in the copy-and-modify constructors. */
static void testReplacing()
{
	LOGD("Testing replacing / removing properties in BlockState copies...");

	// Test replacing:
	BlockState bs1("property1", "value1v1");
	BlockState bs2(bs1, {{"property1", "value1v2"}});
	TEST_EQUAL(bs2.value("property1"), "value1v2");  // Stored the new one
	TEST_EQUAL(bs1.value("property1"), "value1v1");  // Didn't replace in the original

	// Test removing:
	BlockState bs3(bs1, {{"property1", ""}});
	BlockState bsEmpty;
	TEST_EQUAL(bs3, bsEmpty);
}





int main()
{
	LOGD("BlockStateTest started");

	try
	{
		testStaticCreation();
		testDynamicCreation();
		testReplacing();
	}
	catch (const TestException & exc)
	{
		LOGERROR("BlockStateTest has failed explicitly: %s", exc.mMessage.c_str());
		return 1;
	}
	catch (const std::runtime_error & exc)
	{
		LOGERROR("BlockStateTest has failed, an unhandled exception was thrown: %s", exc.what());
		return 1;
	}
	catch (...)
	{
		LOGERROR("BlockStateTest has failed, an unknown exception was thrown.");
		return 1;
	}

	LOGD("BlockStateTest finished");

	return 0;
}




