#include "Globals.h"
#include "Bindings/BlockState.h"
#include "../TestHelpers.h"




/** Tests the class constructors with static (hard-coded) data. */
static void testStaticCreation()
{
	LOGD("Testing CustomBlockState creation from static data...");

	// Create a few CustomBlockStates using the static-data constructors:
	CustomBlockState bs1v1;
	CustomBlockState bs2v1("property", "value");
	CustomBlockState bs3v1({{"property1", "value1"}, {"property2", "value2"}});
	CustomBlockState bs1v2(bs1v1);
	CustomBlockState bs2v2(bs2v1);
	CustomBlockState bs3v2(bs3v1);
	CustomBlockState bs1v3(bs1v2, {{"added property", "value1"}});
	CustomBlockState bs2v3(bs2v2, {{"added property", "value2"}});
	CustomBlockState bs3v3(bs3v2, {{"added property", "value3"}});

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
	LOGD("Testing CustomBlockState creation from dynamic data...");

	using Map = std::map<AString, AString>;

	// Construct from scratch:
	{
		CustomBlockState bs1a({{"property", "value"}});
		Map map1({{"property", "value"}});
		CustomBlockState bs1b(map1);
		TEST_EQUAL(bs1a, bs1b);  // Creation works
		map1.clear();
		TEST_EQUAL(bs1a, bs1b);  // Created a copy independent of map1
	}

	// Construct by moving:
	{
		CustomBlockState bs2a({{"property", "value"}});
		Map map2({{"property", "value"}});
		CustomBlockState bs2b(std::move(map2));
		TEST_EQUAL(bs2a, bs2b);  // Creation works
	}

	// Construct by modifying:
	{
		CustomBlockState bsSrc("property1", "value1");
		CustomBlockState bs3a(bsSrc, {{"property2", "value2"}});
		Map map3({{"property2", "value2"}});
		CustomBlockState bs3b(bsSrc, map3);
		TEST_EQUAL(bs3a, bs3b);
		map3.clear();
		TEST_EQUAL(bs3a, bs3b);
	}
}





/** Tests replacing the properties in the copy-and-modify constructors. */
static void testReplacing()
{
	LOGD("Testing replacing / removing properties in CustomBlockState copies...");

	// Test replacing:
	CustomBlockState bs1("property1", "value1v1");
	CustomBlockState bs2(bs1, {{"property1", "value1v2"}});
	TEST_EQUAL(bs2.value("property1"), "value1v2");  // Stored the new one
	TEST_EQUAL(bs1.value("property1"), "value1v1");  // Didn't replace in the original

	// Test removing:
	CustomBlockState bs3(bs1, {{"property1", ""}});
	CustomBlockState bsEmpty;
	TEST_EQUAL(bs3, bsEmpty);
}





/** Tests the comparison operator. */
static void testComparison()
{
	LOGD("Testing comparison of CustomBlockStates...");

	// Simple property value tests
	TEST_FALSE((CustomBlockState({{"a", "a"}}) < CustomBlockState({{"a", "a"}})));
	TEST_FALSE((CustomBlockState() < CustomBlockState()));
	TEST_TRUE((CustomBlockState() < CustomBlockState({{"foo", "bar"}})));
	TEST_FALSE((CustomBlockState({{"foo", "bar"}}) < CustomBlockState()));
}





/** Tests the comparison operator using crafted data to defeat the checksum. */
static void testComparison2()
{
	/* The following test ensures that items inserted in different order result
	in the same map. I.e. that the < operator is stable. */
	std::vector<CustomBlockState> v;
	std::map<CustomBlockState, bool> map1;
	std::map<CustomBlockState, bool> map2;

	for (int i = 0; i < 128; ++i)
	{
		v.push_back(CustomBlockState({{std::string(1, static_cast<char>(0x1F)), std::string(1, static_cast<char>(i))}}));
		v.push_back(CustomBlockState({{std::string(1, static_cast<char>(0x10)), std::string(1, static_cast<char>(i | 0x80))},
			{std::string(1, static_cast<char>(0x0F)), std::string(1, static_cast<char>(0x80))}}));
	}

	for (size_t i = 0; i < v.size(); ++i)
	{
		map1[v[i]] = true;
	}

	for (auto i = v.size(); i > 0; --i)
	{
		map2[v[i - 1]] = true;
	}

	// Check result
	TEST_EQUAL(v.size(), 2 * 128);
	TEST_EQUAL(map1.size(), v.size());
	TEST_EQUAL(map1.size(), map2.size());
	for (const auto & item: map1)
	{
		TEST_EQUAL(map1[item.first], map2[item.first]);
	}
}





IMPLEMENT_TEST_MAIN("CustomBlockStateTest",
	testStaticCreation();
	testDynamicCreation();
	testReplacing();
	testComparison();
	testComparison2();
)
