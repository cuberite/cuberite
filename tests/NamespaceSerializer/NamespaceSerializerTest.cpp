
// NamespaceSerializerTest.cpp

#include "Globals.h"
#include "../TestHelpers.h"

#include "WorldStorage/NamespaceSerializer.h"

static void TestStatistic()
{
	for (int i = static_cast<int>(Statistic::AchOpenInventory); i <= static_cast<int>(Statistic::TreasureFished); ++i)
	{
		TEST_EQUAL(static_cast<int>(NamespaceSerializer::ToCustomStatistic(NamespaceSerializer::From(static_cast<Statistic>(i)))), i);
	}
}

IMPLEMENT_TEST_MAIN("NamespaceSerializerTest",
{
	TestStatistic();
})
