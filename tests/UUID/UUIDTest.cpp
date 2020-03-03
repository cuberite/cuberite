
// UUIDTest.cpp

#include "Globals.h"
#include "../TestHelpers.h"
#include "UUID.h"

#include <numeric>  // for std::iota





/** Test that FromString -> ToShortString preserves the original value for short UUIDs. */
static void UUIDFromStringToShortString()
{
	const char TestStrings[][33]
	{
		"0123456789abcdef0123456789ABCDEF",
		"d188b2648cc311e7bb31be2e44b06b34",
		"e760d270d8b34288b895d9f78a31e083",
		"7052f2f2594246abb8e3fed602158870",
		"7f14d4b60cd84ba7885c8301b67ce891",
		"57be7039250548b590af272291fabcfa"
	};

	for (auto TestString : TestStrings)
	{
		cUUID UUID;
		TEST_TRUE(UUID.FromString(TestString));
		auto ResultString = UUID.ToShortString();
		// Result should be equivalent to original
		TEST_EQUAL(NoCaseCompare(ResultString, TestString), 0);
		// And should be all lower case
		TEST_EQUAL(ResultString, StrToLower(ResultString));
	}
}





/** Test that FromString -> ToLongString preserves the original value for long UUIDs. */
static void UUIDFromStringToLongString()
{
	const char TestStrings[][37]
	{
		"01234567-89ab-cdef-0123-456789ABCDEF",
		"d188b264-8cc3-11e7-bb31-be2e44b06b34",
		"e760d270-d8b3-4288-b895-d9f78a31e083",
		"7052f2f2-5942-46ab-b8e3-fed602158870",
		"7f14d4b6-0cd8-4ba7-885c-8301b67ce891",
		"57be7039-2505-48b5-90af-272291fabcfa"
	};

	for (auto TestString : TestStrings)
	{
		cUUID UUID;
		TEST_TRUE(UUID.FromString(TestString));
		auto ResultString = UUID.ToLongString();
		// Result should be equivalent to original
		TEST_EQUAL(NoCaseCompare(ResultString, TestString), 0);
		// And should be all lower case
		TEST_EQUAL(ResultString, StrToLower(ResultString));
	}
}





/** Test that FromRaw -> ToRaw preserves the original value. */
static void UUIDFromRawToRaw()
{
	std::array<Byte, 16> TestData[16]{};
	// Fill test data with all values 0 - 255
	for (int i = 0; i != 16; ++i)
	{
		std::iota(begin(TestData[i]), end(TestData[i]), i * 16);
	}

	for (const auto & TestRaw : TestData)
	{
		cUUID UUID;
		UUID.FromRaw(TestRaw);
		auto ResultRaw = UUID.ToRaw();
		TEST_EQUAL(ResultRaw, TestRaw);
	}
}





/** Test that IsNil correctly identifies nil UUIDs. */
static void UUIDNil()
{
	const auto NilString    = "00000000-0000-0000-0000-000000000000";
	const auto NonNilString = "e760d270-d8b3-4288-b895-d9f78a31e083";

	{
		cUUID UUID;
		TEST_TRUE(UUID.FromString(NilString));
		TEST_TRUE(UUID.IsNil());
	}
	{
		cUUID UUID;
		TEST_TRUE(UUID.FromString(NonNilString));
		TEST_TRUE(!UUID.IsNil());
	}
}





/** Test that variant and version numbers are read correctly. */
static void UUIDType()
{
	// From issue #4209
	const char TestStrings[][33]
	{
		"31cfcbeea8f2324a86dbccb1d5aaa6f8",
		"3f2e1dd234a03b6b824c5a0e74083b1e"
	};

	for (const auto & String : TestStrings)
	{
		cUUID UUID;
		TEST_TRUE(UUID.FromString(String));
		TEST_EQUAL(UUID.Variant(), 1);
		TEST_EQUAL(UUID.Version(), 3);
	}

}





IMPLEMENT_TEST_MAIN("UUID",
	UUIDFromStringToShortString();
	UUIDFromStringToLongString();
	UUIDFromRawToRaw();
	UUIDNil();
	UUIDType();
)
