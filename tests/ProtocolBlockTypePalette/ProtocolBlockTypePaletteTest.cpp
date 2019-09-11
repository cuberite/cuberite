
// ByteBufferTest.cpp

// Implements the main app entrypoint for the ProtocolBlockTypePalette class test

#include "Globals.h"
#include "../TestHelpers.h"

#include <string>
#include <fstream>
#include <streambuf>
#include "Protocol/ProtocolBlockTypePalette.h"





static void TestInit(void)
{
	ProtocolBlockTypePalette palette("{}");
}





static void TestSuccess(void)
{
	ProtocolBlockTypePalette palette;
	TEST_TRUE(palette.loadFromString("[]"));

	auto example = "[{\
		\"props\": {\
			\"foo\": \"bar\"\
		}, \
		\"name\": \"b\", \
		\"id\": \"0\"\
	}]";

	palette.clear();
	TEST_TRUE(palette.loadFromString(example));
	TEST_EQUAL(palette.index("b", BlockState({{"foo", "bar"}})), 0);
	TEST_EQUAL(palette.index("b", BlockState({{"foo", "baz"}})), ProtocolBlockTypePalette::NOT_FOUND);
	TEST_EQUAL(palette.index("a", BlockState({{"foo", "bar"}})), ProtocolBlockTypePalette::NOT_FOUND);
}





static void TestErrors(void)
{
	ProtocolBlockTypePalette palette;
	TEST_FALSE(palette.loadFromString(""));

	palette.clear();
	TEST_FALSE(palette.loadFromString("a = {}"));

	palette.clear();
	TEST_FALSE(palette.loadFromString("{x = 1}"));  // Lua style

	palette.clear();
	TEST_FALSE(palette.loadFromString("$#^%&"));
}





static void TestComplex1(void)
{
	ProtocolBlockTypePalette palette;
	auto str = "[{\
		\"props\": {\
			\"foo\": \"bar\", \
			\"moo\": \"baz\"\
		}, \
		\"name\": \"b\"\
	}, {\
		\"props\": {\
			\"foo\": \"baz\", \
			\"moo\": \"bar\"\
		}, \
		\"name\": \"b\"\
	}]";
	TEST_TRUE(palette.loadFromString(str));
	TEST_EQUAL(palette.index("b", BlockState({{"foo","bar"}})), ProtocolBlockTypePalette::NOT_FOUND);
	TEST_EQUAL(palette.index("b", BlockState({{"foo","bar"}, {"moo","baz"}})), 0);
	TEST_EQUAL(palette.index("b", BlockState({{"foo","baz"}, {"moo","bar"}})), 1);
	TEST_EQUAL(palette.index("c", BlockState({{"foo","baz"}, {"moo","bar"}})), ProtocolBlockTypePalette::NOT_FOUND);
}





static void TestComplex2(void)
{
	ProtocolBlockTypePalette palette;
	auto str = "[{\
		\"name\": \"a\"\
	}, {\
		\"name\": \"b\"\
	}]";
	TEST_TRUE(palette.loadFromString(str));
	TEST_EQUAL(palette.index("a", BlockState()), 0);
	TEST_EQUAL(palette.index("b", BlockState({})), 1);
}





static void TestFile(void)
{
	std::ifstream f("ProtocolBlockTypePalette.json");
	std::string str((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
	ProtocolBlockTypePalette palette;
	palette.loadFromString(str);

	// This is a bit problematic - the only permanently fixed block Id is air...
	TEST_EQUAL(palette.index("minecraft:air", BlockState({})), 0);
	TEST_TRUE(palette.index("minecraft:stone", BlockState()) != ProtocolBlockTypePalette::NOT_FOUND);
	TEST_TRUE(palette.index("minecraft:dirt", BlockState()) != ProtocolBlockTypePalette::NOT_FOUND);
}





IMPLEMENT_TEST_MAIN("ProtocolBlockTypePaletteTest",
	TestInit();
	TestSuccess();
	TestErrors();
	TestComplex1();
	TestComplex2();
	TestFile();
)
