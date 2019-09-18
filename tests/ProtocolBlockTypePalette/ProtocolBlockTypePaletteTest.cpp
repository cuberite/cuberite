// ProtocolBlockTypePaletteTest.cpp

#include <string>
#include <fstream>
#include <streambuf>
#include "Globals.h"

#include "Protocol/ProtocolBlockTypePalette.h"

#include "../TestHelpers.h"





static void TestSuccess(void)
{
	LOG("Test TestSuccess");
	ProtocolBlockTypePalette palette;

	auto example = "{\"Metadata\":{\"ProtocolBlockTypePaletteVersion\":1}, \"Palette\":[{\
		\"props\": {\
			\"foo\": \"bar\"\
		}, \
		\"name\": \"b\", \
		\"id\": \"0\"\
	}]}";

	palette.clear();
	TEST_TRUE(palette.loadFromString(example));
	TEST_EQUAL(palette.index("b", BlockState({{"foo", "bar"}})), 0);
	TEST_EQUAL(palette.index("b", BlockState({{"foo", "baz"}})), ProtocolBlockTypePalette::NOT_FOUND);
	TEST_EQUAL(palette.index("a", BlockState({{"foo", "bar"}})), ProtocolBlockTypePalette::NOT_FOUND);
}





static void TestErrors(void)
{
	LOG("Test TestErrors");
	ProtocolBlockTypePalette palette;
	TEST_FALSE(palette.loadFromString(""));

	palette.clear();
	TEST_FALSE(palette.loadFromString("[]"));

	palette.clear();
	TEST_FALSE(palette.loadFromString("a = {}"));

	palette.clear();
	TEST_FALSE(palette.loadFromString("{x = 1}"));  // Lua style

	palette.clear();
	TEST_FALSE(palette.loadFromString("$#^%&"));
}





static void TestComplex1(void)
{
	LOG("Test TestComplex1");
	ProtocolBlockTypePalette palette;
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
	}]}";
	TEST_TRUE(palette.loadFromString(str));
	TEST_EQUAL(palette.index("b", BlockState({{"foo","bar"}})), ProtocolBlockTypePalette::NOT_FOUND);
	TEST_EQUAL(palette.index("b", BlockState({{"foo","bar"}, {"moo","baz"}})), 0);
	TEST_EQUAL(palette.index("b", BlockState({{"foo","baz"}, {"moo","bar"}})), 1);
	TEST_EQUAL(palette.index("c", BlockState({{"foo","baz"}, {"moo","bar"}})), ProtocolBlockTypePalette::NOT_FOUND);
}





static void TestComplex2(void)
{
	LOG("Test TestComplex2");
	ProtocolBlockTypePalette palette;
	auto str = "{\"Metadata\":{\"ProtocolBlockTypePaletteVersion\":1}, \"Palette\":[{\
		\"id\": \"0\", \
		\"name\": \"a\"\
	}, {\
		\"id\": \"1\", \
		\"name\": \"b\"\
	}]}";
	TEST_TRUE(palette.loadFromString(str));
	TEST_EQUAL(palette.index("a", BlockState()), 0);
	TEST_EQUAL(palette.index("b", BlockState({})), 1);
}





static void TestFile(void)
{
	LOG("Test TestFile");
	std::ifstream f("base.btp.json");
	ProtocolBlockTypePalette palette;
	TEST_TRUE(palette.loadFromStream(f));

	// This is a bit problematic - the only permanently fixed block Id is air...
	TEST_EQUAL(palette.index("minecraft:air", BlockState()), 0);
	TEST_NOTEQUAL(palette.index("minecraft:stone", BlockState()), ProtocolBlockTypePalette::NOT_FOUND);
	TEST_NOTEQUAL(palette.index("minecraft:dirt", BlockState()), ProtocolBlockTypePalette::NOT_FOUND);
}





static void TestFile2(void)
{
	LOG("Test TestFile2");
	std::ifstream f("test.btp.json");
	ProtocolBlockTypePalette palette;
	TEST_TRUE(palette.loadFromStream(f));

	TEST_EQUAL(palette.index("minecraft:air", BlockState({})), 0);
	TEST_EQUAL(palette.index("minecraft:stone", BlockState()), 1);
	TEST_EQUAL(palette.index("minecraft:stone", BlockState()), 1);
	TEST_EQUAL(palette.index(
		"minecraft:dark_oak_leaves",
		BlockState({{"persistent", "false"}, {"distance", "6"}})
	), 225);
	TEST_EQUAL(palette.index(
		"minecraft:dark_oak_leaves",
		BlockState({{"persistent", "false"}})
	), ProtocolBlockTypePalette::NOT_FOUND);
}





IMPLEMENT_TEST_MAIN("ProtocolBlockTypePaletteTest",
	TestSuccess();
	TestErrors();
	TestComplex1();
	TestComplex2();
	TestFile();
	TestFile2();
)
