
// LoadablePieces.cpp

// Implements the LoadablePieces test main entrypoint

#include "Globals.h"
#ifdef _WIN32
	#include <direct.h>
	#define GetCurrentFolder _getcwd
#else
	#include <unistd.h>
	#define GetCurrentFolder getcwd
#endif
#include "../TestHelpers.h"
#include "Generating/PrefabPiecePool.h"





static int DoLoaderTest(void)
{
	cPrefabPiecePool test;
	auto res = test.LoadFromFile("Test.cubeset", true);
	if (!res)
	{
		LOGWARNING("Loading from file \"Test.cubeset\" failed.");
		return 1;
	}
	LOG("Loaded %u regular pieces and %u starting pieces", static_cast<unsigned>(test.GetAllPiecesCount()), static_cast<unsigned>(test.GetStartingPiecesCount()));

	// Check that we loaded all the pieces:
	TEST_EQUAL(test.GetAllPiecesCount(), 1);
	TEST_EQUAL(test.GetStartingPiecesCount(), 1);

	return 0;
}





static int DoParserTest(void)
{
	// Create one static prefab to test the parser:
	static const cPrefab::sDef testPrefabDef =
	{
		// Size:
		7, 6, 7,  // SizeX = 7, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 5, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* 0 */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */,

		// Block data:
		// Level 1
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"

		// Level 2
		"aa...aa"
		"a.....a"
		"......."
		"......."
		"......."
		"a.....a"
		"aa...aa"

		// Level 3
		"aa...aa"
		"a.....a"
		"......."
		"......."
		"......."
		"a.....a"
		"aa...aa"

		// Level 4
		"aa...aa"
		"a.....a"
		"......."
		"......."
		"......."
		"a.....a"
		"aa...aa"

		// Level 5
		"aabbbaa"
		"a.....a"
		"b.....b"
		"b.....b"
		"b.....b"
		"a.....a"
		"aabbbaa"

		// Level 6
		"aaaaaaa"
		"a.....a"
		"a.....a"
		"a.....a"
		"a.....a"
		"a.....a"
		"aaaaaaa",

		// Connections:
		"0: 0, 3, 2: 4\n"
		"0: 2, 3, 0: 2\n",

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */

		// Merge strategy:
		cBlockArea::msImprint,

		// ExtendFloorStrategy:
		cPrefab::efsNone,

		// DefaultWeight:
		10,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		1000,

		// MoveToGround:
		false,
	};

	cPrefab testPrefab(testPrefabDef);
	cPiece & prefabAsPiece(testPrefab);  // GetConnectors() is private in cPrefab, need to cast to parent cPiece where it is public
	if (prefabAsPiece.GetConnectors().size() != 2)
	{
		LOGWARNING("Piece parsing failed, connectors not parsed properly. Expected 2 connectors, got %u", static_cast<unsigned>(prefabAsPiece.GetConnectors().size()));
	}
	return 0;
}





int main(int argc, char * argv[])
{
	LOGD("Test started");

	// Print the current directory for reference:
	char folder[FILENAME_MAX];
	GetCurrentFolder(folder, sizeof(folder));
	LOG("Running cPrefabPiecePool test from folder \"%s\".", folder);

	// Run the Loader test:
	int res = DoLoaderTest();
	LOG("cPrefabPiecePool loading test done: %s", (res == 0) ? "success" : "failure");
	if (res != 0)
	{
		return res;
	}

	// Run the Parser test:
	res = DoParserTest();
	LOG("cPrefab parser test done: %s", (res == 0) ? "success" : "failure");

	return res;
}




