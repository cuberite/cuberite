
// TestRailsPrefabs.cpp

// Defines the prefabs in the group TestRails

// NOTE: This file has been generated automatically by GalExport!
// Any manual changes will be overwritten by the next automatic export!

#include "Globals.h"
#include "TestRailsPrefabs.h"





const cPrefab::sDef g_TestRailsPrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// ActivatorRail:
	// The data has been exported from the gallery Plains, area index 251, ID 746, created by Aloe_vera
	{
		// Size:
		7, 3, 7,  // SizeX = 7, SizeY = 3, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 2, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b:  5: 0\n"  /* wood */
		"c:157: 0\n"  /* activatorrail */
		"d:157: 2\n"  /* activatorrail */
		"e:157: 3\n"  /* activatorrail */
		"f:157: 5\n"  /* activatorrail */
		"g: 50: 5\n"  /* torch */
		"h:157: 4\n"  /* activatorrail */
		"i:157: 1\n"  /* activatorrail */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "aaab..."
		/*  1 */ "abbbbb."
		/*  2 */ "abbb.b."
		/*  3 */ "bbbb.bb"
		/*  4 */ ".b...b."
		/*  5 */ ".bbbbb."
		/*  6 */ "...b..."

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".cdbec."
		/*  2 */ ".fg..f."
		/*  3 */ ".b.g.b."
		/*  4 */ ".h...h."
		/*  5 */ ".cdbec."
		/*  6 */ "......."

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "...i..."
		/*  2 */ "......."
		/*  3 */ ".c...c."
		/*  4 */ "......."
		/*  5 */ "...i..."
		/*  6 */ ".......",

		// Connectors:
		"1: 6, 1, 3: 5\n"  /* Type 1, direction X+ */
		"-1: 6, 1, 3: 5\n"  /* Type -1, direction X+ */
		"1: 3, 1, 6: 3\n"  /* Type 1, direction Z+ */
		"-1: 3, 1, 6: 3\n"  /* Type -1, direction Z+ */
		"1: 0, 1, 3: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 3: 4\n"  /* Type -1, direction X- */
		"1: 3, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 3, 1, 0: 2\n"  /* Type -1, direction Z- */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		false,

		// DefaultWeight:
		100,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // ActivatorRail



	////////////////////////////////////////////////////////////////////////////////
	// DetectorRail:
	// The data has been exported from the gallery Plains, area index 250, ID 745, created by Aloe_vera
	{
		// Size:
		7, 3, 7,  // SizeX = 7, SizeY = 3, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 2, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b:  5: 0\n"  /* wood */
		"c: 28: 0\n"  /* detectorrail */
		"d: 28: 2\n"  /* detectorrail */
		"e: 28: 3\n"  /* detectorrail */
		"f: 28: 5\n"  /* detectorrail */
		"g: 50: 5\n"  /* torch */
		"h: 28: 4\n"  /* detectorrail */
		"i: 28: 1\n"  /* detectorrail */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "aaab..."
		/*  1 */ "abbbbb."
		/*  2 */ "abbb.b."
		/*  3 */ "bbbb.bb"
		/*  4 */ ".b...b."
		/*  5 */ ".bbbbb."
		/*  6 */ "...b..."

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".cdbec."
		/*  2 */ ".fg..f."
		/*  3 */ ".b.g.b."
		/*  4 */ ".h...h."
		/*  5 */ ".cdbec."
		/*  6 */ "......."

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "...i..."
		/*  2 */ "......."
		/*  3 */ ".c...c."
		/*  4 */ "......."
		/*  5 */ "...i..."
		/*  6 */ ".......",

		// Connectors:
		"1: 6, 1, 3: 5\n"  /* Type 1, direction X+ */
		"-1: 6, 1, 3: 5\n"  /* Type -1, direction X+ */
		"1: 3, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 3, 1, 0: 2\n"  /* Type -1, direction Z- */
		"1: 0, 1, 3: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 3: 4\n"  /* Type -1, direction X- */
		"1: 3, 1, 6: 3\n"  /* Type 1, direction Z+ */
		"-1: 3, 1, 6: 3\n"  /* Type -1, direction Z+ */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		false,

		// DefaultWeight:
		100,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // DetectorRail



	////////////////////////////////////////////////////////////////////////////////
	// PowerRail:
	// The data has been exported from the gallery Plains, area index 248, ID 743, created by Aloe_vera
	{
		// Size:
		7, 3, 7,  // SizeX = 7, SizeY = 3, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 2, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b:  5: 0\n"  /* wood */
		"c: 27: 0\n"  /* poweredrail */
		"d: 27: 2\n"  /* poweredrail */
		"e: 27: 3\n"  /* poweredrail */
		"f: 27: 5\n"  /* poweredrail */
		"g: 50: 5\n"  /* torch */
		"h: 27: 4\n"  /* poweredrail */
		"i: 27: 1\n"  /* poweredrail */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "aaab..."
		/*  1 */ "abbbbb."
		/*  2 */ "abbb.b."
		/*  3 */ "bbbb.bb"
		/*  4 */ ".b...b."
		/*  5 */ ".bbbbb."
		/*  6 */ "...b..."

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".cdbec."
		/*  2 */ ".fg..f."
		/*  3 */ ".b.g.b."
		/*  4 */ ".h...h."
		/*  5 */ ".cdbec."
		/*  6 */ "......."

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "...i..."
		/*  2 */ "......."
		/*  3 */ ".c...c."
		/*  4 */ "......."
		/*  5 */ "...i..."
		/*  6 */ ".......",

		// Connectors:
		"1: 6, 1, 3: 5\n"  /* Type 1, direction X+ */
		"-1: 6, 1, 3: 5\n"  /* Type -1, direction X+ */
		"1: 3, 1, 6: 3\n"  /* Type 1, direction Z+ */
		"-1: 3, 1, 6: 3\n"  /* Type -1, direction Z+ */
		"1: 0, 1, 3: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 3: 4\n"  /* Type -1, direction X- */
		"1: 3, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 3, 1, 0: 2\n"  /* Type -1, direction Z- */
		"1: 6, 1, 3: 5\n"  /* Type 1, direction X+ */
		"-1: 6, 1, 3: 5\n"  /* Type -1, direction X+ */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		false,

		// DefaultWeight:
		100,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // PowerRail



	////////////////////////////////////////////////////////////////////////////////
	// RegularRail:
	// The data has been exported from the gallery Plains, area index 247, ID 742, created by Aloe_vera
	{
		// Size:
		7, 3, 7,  // SizeX = 7, SizeY = 3, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 2, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b:  5: 0\n"  /* wood */
		"c: 66: 6\n"  /* tracks */
		"d: 66: 2\n"  /* tracks */
		"e: 66: 3\n"  /* tracks */
		"f: 66: 7\n"  /* tracks */
		"g: 66: 5\n"  /* tracks */
		"h: 50: 5\n"  /* torch */
		"i: 66: 4\n"  /* tracks */
		"j: 66: 9\n"  /* tracks */
		"k: 66: 8\n"  /* tracks */
		"l: 66: 1\n"  /* tracks */
		"m: 19: 0\n"  /* sponge */
		"n: 66: 0\n"  /* tracks */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "aaab..."
		/*  1 */ "abbbbb."
		/*  2 */ "abbb.b."
		/*  3 */ "bbbb.bb"
		/*  4 */ ".b...b."
		/*  5 */ ".bbbbb."
		/*  6 */ "...b..."

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".cdbef."
		/*  2 */ ".gh..g."
		/*  3 */ ".b.h.b."
		/*  4 */ ".i...i."
		/*  5 */ ".jdbek."
		/*  6 */ "......."

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "...l..."
		/*  2 */ "......."
		/*  3 */ ".n...n."
		/*  4 */ "......."
		/*  5 */ "...l..."
		/*  6 */ ".......",

		// Connectors:
		"1: 6, 1, 3: 5\n"  /* Type 1, direction X+ */
		"-1: 6, 1, 3: 5\n"  /* Type -1, direction X+ */
		"1: 3, 1, 6: 3\n"  /* Type 1, direction Z+ */
		"-1: 3, 1, 6: 3\n"  /* Type -1, direction Z+ */
		"1: 0, 1, 3: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 3: 4\n"  /* Type -1, direction X- */
		"1: 3, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 3, 1, 0: 2\n"  /* Type -1, direction Z- */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		false,

		// DefaultWeight:
		100,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // RegularRail
};  // g_TestRailsPrefabs






const cPrefab::sDef g_TestRailsStartingPrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// CentralPiece:
	// The data has been exported from the gallery Plains, area index 249, ID 744, created by Aloe_vera
	{
		// Size:
		6, 3, 6,  // SizeX = 6, SizeY = 3, SizeZ = 6

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		5, 2, 5,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b:  5: 0\n"  /* wood */
		"c: 66: 6\n"  /* tracks */
		"d: 66: 2\n"  /* tracks */
		"e: 66: 3\n"  /* tracks */
		"f: 66: 7\n"  /* tracks */
		"g: 66: 5\n"  /* tracks */
		"h: 50: 5\n"  /* torch */
		"i: 66: 4\n"  /* tracks */
		"j: 66: 9\n"  /* tracks */
		"k: 66: 8\n"  /* tracks */
		"l: 66: 1\n"  /* tracks */
		"m: 19: 0\n"  /* sponge */
		"n: 66: 0\n"  /* tracks */,

		// Block data:
		// Level 0
		/* z\x*   012345 */
		/*  0 */ "aaab.."
		/*  1 */ "abbbbb"
		/*  2 */ "abbb.b"
		/*  3 */ "bbbb.b"
		/*  4 */ ".b...b"
		/*  5 */ ".bbbbb"

		// Level 1
		/* z\x*   012345 */
		/*  0 */ "......"
		/*  1 */ ".cdbef"
		/*  2 */ ".gh..g"
		/*  3 */ ".b.h.b"
		/*  4 */ ".i...i"
		/*  5 */ ".jdbek"

		// Level 2
		/* z\x*   012345 */
		/*  0 */ "......"
		/*  1 */ "...l.."
		/*  2 */ "......"
		/*  3 */ ".n...n"
		/*  4 */ "......"
		/*  5 */ "...l..",

		// Connectors:
		"1: 3, 1, 6: 3\n"  /* Type 1, direction Z+ */
		"1: 0, 1, 3: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 3: 4\n"  /* Type -1, direction X- */
		"-1: 3, 1, 6: 3\n"  /* Type -1, direction Z+ */
		"1: 6, 1, 3: 5\n"  /* Type 1, direction X+ */
		"-1: 6, 1, 3: 5\n"  /* Type -1, direction X+ */
		"1: 3, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 3, 1, 0: 2\n"  /* Type -1, direction Z- */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		true,

		// DefaultWeight:
		100,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // CentralPiece
};





// The prefab counts:

const size_t g_TestRailsPrefabsCount = ARRAYCOUNT(g_TestRailsPrefabs);

const size_t g_TestRailsStartingPrefabsCount = ARRAYCOUNT(g_TestRailsStartingPrefabs);

