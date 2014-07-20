
// SandVillagePrefabs.cpp

// Defines the prefabs in the group SandVillage

// NOTE: This file has been generated automatically by GalExport!
// Any manual changes will be overwritten by the next automatic export!

#include "Globals.h"
#include "SandVillagePrefabs.h"





const cPrefab::sDef g_SandVillagePrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// DoubleField:
	// The data has been exported from the gallery Desert, area index 5, ID 75, created by tonibm1999
	{
		// Size:
		13, 3, 9,  // SizeX = 13, SizeY = 3, SizeZ = 9

		// Hitbox (relative to bounding box):
		-1, 0, -1,  // MinX, MinY, MinZ
		13, 2, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b: 60: 7\n"  /* tilleddirt */
		"c:  8: 0\n"  /* water */
		"d: 50: 5\n"  /* torch */
		"e: 59: 7\n"  /* crops */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "aaaaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaaaa"

		// Level 1
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "aaaaaaaaaaaaa"
		/*  1 */ "abbcbbabbcbba"
		/*  2 */ "abbcbbabbcbba"
		/*  3 */ "abbcbbabbcbba"
		/*  4 */ "abbcbbabbcbba"
		/*  5 */ "abbcbbabbcbba"
		/*  6 */ "abbcbbabbcbba"
		/*  7 */ "abbcbbabbcbba"
		/*  8 */ "aaaaaaaaaaaaa"

		// Level 2
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "d.....d.....d"
		/*  1 */ ".ee.ee.ee.ee."
		/*  2 */ ".ee.ee.ee.ee."
		/*  3 */ ".ee.ee.ee.ee."
		/*  4 */ ".ee.ee.ee.ee."
		/*  5 */ ".ee.ee.ee.ee."
		/*  6 */ ".ee.ee.ee.ee."
		/*  7 */ ".ee.ee.ee.ee."
		/*  8 */ "d.....d.....d",

		// Connectors:
		"-1: 6, 1, 8: 3\n"  /* Type -1, direction Z+ */,

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
		true,
	},  // DoubleField



	////////////////////////////////////////////////////////////////////////////////
	// House11x7:
	// The data has been exported from the gallery Desert, area index 6, ID 81, created by Aloe_vera
	{
		// Size:
		11, 7, 7,  // SizeX = 11, SizeY = 7, SizeZ = 7

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		11, 6, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j: 50: 4\n"  /* torch */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */
		"n: 50: 1\n"  /* torch */
		"o: 50: 2\n"  /* torch */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "mmmmaaammmm"
		/*  1 */ "maaaaaaaaam"
		/*  2 */ "maaaaaaaaam"
		/*  3 */ "maaaaaaaaam"
		/*  4 */ "maaaaaaaaam"
		/*  5 */ "maaaaaaaaam"
		/*  6 */ "mmmmmmmmmmm"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "....bcd...."
		/*  1 */ ".aaaaaaaaa."
		/*  2 */ ".aaaaaaaaa."
		/*  3 */ ".aaaaaaaaa."
		/*  4 */ ".aaaaaaaaa."
		/*  5 */ ".aaaaaaaaa."
		/*  6 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".aaaaeaaaa."
		/*  2 */ ".a.......a."
		/*  3 */ ".a.......a."
		/*  4 */ ".a.......a."
		/*  5 */ ".aaaaaaaaa."
		/*  6 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".affagaffa."
		/*  2 */ ".f.......f."
		/*  3 */ ".f.......f."
		/*  4 */ ".f.......f."
		/*  5 */ ".affafaffa."
		/*  6 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "ccccccccccc"
		/*  1 */ "haaaaaaaaah"
		/*  2 */ ".a..i.i..a."
		/*  3 */ ".a.......a."
		/*  4 */ ".a..j.j..a."
		/*  5 */ "kaaaaaaaaak"
		/*  6 */ "lllllllllll"

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "ccccccccccc"
		/*  2 */ "haaaaaaaaah"
		/*  3 */ ".an.....oa."
		/*  4 */ "kaaaaaaaaak"
		/*  5 */ "lllllllllll"
		/*  6 */ "..........."

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "ccccccccccc"
		/*  3 */ "aaaaaaaaaaa"
		/*  4 */ "lllllllllll"
		/*  5 */ "..........."
		/*  6 */ "...........",

		// Connectors:
		"-1: 5, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House11x7



	////////////////////////////////////////////////////////////////////////////////
	// House11x9:
	// The data has been exported from the gallery Desert, area index 11, ID 115, created by xoft
	{
		// Size:
		11, 8, 9,  // SizeX = 11, SizeY = 8, SizeZ = 9

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		11, 7, 9,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j: 50: 4\n"  /* torch */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "mmmmaaammmm"
		/*  1 */ "maaaaaaaaam"
		/*  2 */ "maaaaaaaaam"
		/*  3 */ "maaaaaaaaam"
		/*  4 */ "maaaaaaaaam"
		/*  5 */ "maaaaaaaaam"
		/*  6 */ "maaaaaaaaam"
		/*  7 */ "maaaaaaaaam"
		/*  8 */ "mmmmmmmmmmm"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "....bcd...."
		/*  1 */ ".aaaaaaaaa."
		/*  2 */ ".aaaaaaaaa."
		/*  3 */ ".aaaaaaaaa."
		/*  4 */ ".aaaaaaaaa."
		/*  5 */ ".aaaaaaaaa."
		/*  6 */ ".aaaaaaaaa."
		/*  7 */ ".aaaaaaaaa."
		/*  8 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".aaaaeaaaa."
		/*  2 */ ".a.......a."
		/*  3 */ ".a.......a."
		/*  4 */ ".a.......a."
		/*  5 */ ".a.......a."
		/*  6 */ ".a.......a."
		/*  7 */ ".aaaaaaaaa."
		/*  8 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".affagaffa."
		/*  2 */ ".f.......f."
		/*  3 */ ".f.......f."
		/*  4 */ ".a.......a."
		/*  5 */ ".f.......f."
		/*  6 */ ".f.......f."
		/*  7 */ ".afffafffa."
		/*  8 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "ccccccccccc"
		/*  1 */ "haaaaaaaaah"
		/*  2 */ ".a..i.i..a."
		/*  3 */ ".a.......a."
		/*  4 */ ".a.......a."
		/*  5 */ ".a.......a."
		/*  6 */ ".a...j...a."
		/*  7 */ "kaaaaaaaaak"
		/*  8 */ "lllllllllll"

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "ccccccccccc"
		/*  2 */ "haaaaaaaaah"
		/*  3 */ ".a.......a."
		/*  4 */ ".a.......a."
		/*  5 */ ".a.......a."
		/*  6 */ "kaaaaaaaaak"
		/*  7 */ "lllllllllll"
		/*  8 */ "..........."

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "ccccccccccc"
		/*  3 */ "haaaaaaaaah"
		/*  4 */ ".a.......a."
		/*  5 */ "kaaaaaaaaak"
		/*  6 */ "lllllllllll"
		/*  7 */ "..........."
		/*  8 */ "..........."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "ccccccccccc"
		/*  4 */ "aaaaaaaaaaa"
		/*  5 */ "lllllllllll"
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "...........",

		// Connectors:
		"-1: 5, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House11x9



	////////////////////////////////////////////////////////////////////////////////
	// House13x7:
	// The data has been exported from the gallery Desert, area index 15, ID 125, created by Aloe_vera
	{
		// Size:
		13, 7, 7,  // SizeX = 13, SizeY = 7, SizeZ = 7

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		13, 6, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j: 50: 4\n"  /* torch */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "mmmmmaaammmmm"
		/*  1 */ "maaaaaaaaaaam"
		/*  2 */ "maaaaaaaaaaam"
		/*  3 */ "maaaaaaaaaaam"
		/*  4 */ "maaaaaaaaaaam"
		/*  5 */ "maaaaaaaaaaam"
		/*  6 */ "mmmmmmmmmmmmm"

		// Level 1
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ ".....bcd....."
		/*  1 */ ".aaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaa."
		/*  4 */ ".aaaaaaaaaaa."
		/*  5 */ ".aaaaaaaaaaa."
		/*  6 */ "............."

		// Level 2
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".aaaaaeaaaaa."
		/*  2 */ ".a.........a."
		/*  3 */ ".a.........a."
		/*  4 */ ".a.........a."
		/*  5 */ ".aaaaaaaaaaa."
		/*  6 */ "............."

		// Level 3
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".afffagafffa."
		/*  2 */ ".f.........f."
		/*  3 */ ".f.........f."
		/*  4 */ ".f.........f."
		/*  5 */ ".affafffaffa."
		/*  6 */ "............."

		// Level 4
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "ccccccccccccc"
		/*  1 */ "haaaaaaaaaaah"
		/*  2 */ ".a...i.i...a."
		/*  3 */ ".a.........a."
		/*  4 */ ".a..j...j..a."
		/*  5 */ "kaaaaaaaaaaak"
		/*  6 */ "lllllllllllll"

		// Level 5
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "ccccccccccccc"
		/*  2 */ "haaaaaaaaaaah"
		/*  3 */ ".a.........a."
		/*  4 */ "kaaaaaaaaaaak"
		/*  5 */ "lllllllllllll"
		/*  6 */ "............."

		// Level 6
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "............."
		/*  2 */ "ccccccccccccc"
		/*  3 */ "aaaaaaaaaaaaa"
		/*  4 */ "lllllllllllll"
		/*  5 */ "............."
		/*  6 */ ".............",

		// Connectors:
		"-1: 6, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House13x7



	////////////////////////////////////////////////////////////////////////////////
	// House13x9:
	// The data has been exported from the gallery Desert, area index 12, ID 116, created by xoft
	{
		// Size:
		13, 8, 9,  // SizeX = 13, SizeY = 8, SizeZ = 9

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		13, 7, 9,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j: 50: 4\n"  /* torch */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "mmmmmaaammmmm"
		/*  1 */ "maaaaaaaaaaam"
		/*  2 */ "maaaaaaaaaaam"
		/*  3 */ "maaaaaaaaaaam"
		/*  4 */ "maaaaaaaaaaam"
		/*  5 */ "maaaaaaaaaaam"
		/*  6 */ "maaaaaaaaaaam"
		/*  7 */ "maaaaaaaaaaam"
		/*  8 */ "mmmmmmmmmmmmm"

		// Level 1
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ ".....bcd....."
		/*  1 */ ".aaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaa."
		/*  4 */ ".aaaaaaaaaaa."
		/*  5 */ ".aaaaaaaaaaa."
		/*  6 */ ".aaaaaaaaaaa."
		/*  7 */ ".aaaaaaaaaaa."
		/*  8 */ "............."

		// Level 2
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".aaaaaeaaaaa."
		/*  2 */ ".a.........a."
		/*  3 */ ".a.........a."
		/*  4 */ ".a.........a."
		/*  5 */ ".a.........a."
		/*  6 */ ".a.........a."
		/*  7 */ ".aaaaaaaaaaa."
		/*  8 */ "............."

		// Level 3
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".afffagafffa."
		/*  2 */ ".f.........f."
		/*  3 */ ".f.........f."
		/*  4 */ ".a.........a."
		/*  5 */ ".f.........f."
		/*  6 */ ".f.........f."
		/*  7 */ ".affaffafffa."
		/*  8 */ "............."

		// Level 4
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "ccccccccccccc"
		/*  1 */ "haaaaaaaaaaah"
		/*  2 */ ".a...i.i...a."
		/*  3 */ ".a.........a."
		/*  4 */ ".a.........a."
		/*  5 */ ".a.........a."
		/*  6 */ ".a..j..j...a."
		/*  7 */ "kaaaaaaaaaaak"
		/*  8 */ "lllllllllllll"

		// Level 5
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "ccccccccccccc"
		/*  2 */ "haaaaaaaaaaah"
		/*  3 */ ".a.........a."
		/*  4 */ ".a.........a."
		/*  5 */ ".a.........a."
		/*  6 */ "kaaaaaaaaaaak"
		/*  7 */ "lllllllllllll"
		/*  8 */ "............."

		// Level 6
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "............."
		/*  2 */ "ccccccccccccc"
		/*  3 */ "haaaaaaaaaaah"
		/*  4 */ ".a.........a."
		/*  5 */ "kaaaaaaaaaaak"
		/*  6 */ "lllllllllllll"
		/*  7 */ "............."
		/*  8 */ "............."

		// Level 7
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "............."
		/*  2 */ "............."
		/*  3 */ "ccccccccccccc"
		/*  4 */ "aaaaaaaaaaaaa"
		/*  5 */ "lllllllllllll"
		/*  6 */ "............."
		/*  7 */ "............."
		/*  8 */ ".............",

		// Connectors:
		"-1: 6, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House13x9



	////////////////////////////////////////////////////////////////////////////////
	// House15x9:
	// The data has been exported from the gallery Desert, area index 13, ID 118, created by xoft
	{
		// Size:
		15, 8, 9,  // SizeX = 15, SizeY = 8, SizeZ = 9

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		15, 7, 9,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j: 50: 4\n"  /* torch */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "mmmmmaaammmmmmm"
		/*  1 */ "maaaaaaaaaaaaam"
		/*  2 */ "maaaaaaaaaaaaam"
		/*  3 */ "maaaaaaaaaaaaam"
		/*  4 */ "maaaaaaaaaaaaam"
		/*  5 */ "maaaaaaaaaaaaam"
		/*  6 */ "maaaaaaaaaaaaam"
		/*  7 */ "maaaaaaaaaaaaam"
		/*  8 */ "mmmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ ".....bcd......."
		/*  1 */ ".aaaaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaaaa."
		/*  4 */ ".aaaaaaaaaaaaa."
		/*  5 */ ".aaaaaaaaaaaaa."
		/*  6 */ ".aaaaaaaaaaaaa."
		/*  7 */ ".aaaaaaaaaaaaa."
		/*  8 */ "..............."

		// Level 2
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".aaaaaeaaaaaaa."
		/*  2 */ ".a...........a."
		/*  3 */ ".a...........a."
		/*  4 */ ".a...........a."
		/*  5 */ ".a...........a."
		/*  6 */ ".a...........a."
		/*  7 */ ".aaaaaaaaaaaaa."
		/*  8 */ "..............."

		// Level 3
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".afffagaffaffa."
		/*  2 */ ".f...........f."
		/*  3 */ ".f...........f."
		/*  4 */ ".a...........a."
		/*  5 */ ".f...........f."
		/*  6 */ ".f...........f."
		/*  7 */ ".affaffaffaffa."
		/*  8 */ "..............."

		// Level 4
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "ccccccccccccccc"
		/*  1 */ "haaaaaaaaaaaaah"
		/*  2 */ ".a...i.i..i..a."
		/*  3 */ ".a...........a."
		/*  4 */ ".a...........a."
		/*  5 */ ".a...........a."
		/*  6 */ ".a..j..j..j..a."
		/*  7 */ "kaaaaaaaaaaaaak"
		/*  8 */ "lllllllllllllll"

		// Level 5
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "ccccccccccccccc"
		/*  2 */ "haaaaaaaaaaaaah"
		/*  3 */ ".a...........a."
		/*  4 */ ".a...........a."
		/*  5 */ ".a...........a."
		/*  6 */ "kaaaaaaaaaaaaak"
		/*  7 */ "lllllllllllllll"
		/*  8 */ "..............."

		// Level 6
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "..............."
		/*  2 */ "ccccccccccccccc"
		/*  3 */ "haaaaaaaaaaaaah"
		/*  4 */ ".a...........a."
		/*  5 */ "kaaaaaaaaaaaaak"
		/*  6 */ "lllllllllllllll"
		/*  7 */ "..............."
		/*  8 */ "..............."

		// Level 7
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "..............."
		/*  2 */ "..............."
		/*  3 */ "ccccccccccccccc"
		/*  4 */ "aaaaaaaaaaaaaaa"
		/*  5 */ "lllllllllllllll"
		/*  6 */ "..............."
		/*  7 */ "..............."
		/*  8 */ "...............",

		// Connectors:
		"-1: 6, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House15x9



	////////////////////////////////////////////////////////////////////////////////
	// House16x9:
	// The data has been exported from the gallery Desert, area index 16, ID 126, created by Aloe_vera
	{
		// Size:
		16, 8, 9,  // SizeX = 16, SizeY = 8, SizeZ = 9

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		16, 7, 9,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j: 50: 4\n"  /* torch */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmaaammmmm"
		/*  1 */ "maaaaaaaaaaaaaam"
		/*  2 */ "maaaaaaaaaaaaaam"
		/*  3 */ "maaaaaaaaaaaaaam"
		/*  4 */ "maaaaaaaaaaaaaam"
		/*  5 */ "maaaaaaaaaaaaaam"
		/*  6 */ "maaaaaaaaaaaaaam"
		/*  7 */ "maaaaaaaaaaaaaam"
		/*  8 */ "mmmmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "........bcd....."
		/*  1 */ ".aaaaaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaaaaa."
		/*  4 */ ".aaaaaaaaaaaaaa."
		/*  5 */ ".aaaaaaaaaaaaaa."
		/*  6 */ ".aaaaaaaaaaaaaa."
		/*  7 */ ".aaaaaaaaaaaaaa."
		/*  8 */ "................"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".aaaaaaaaeaaaaa."
		/*  2 */ ".a............a."
		/*  3 */ ".a............a."
		/*  4 */ ".a............a."
		/*  5 */ ".a............a."
		/*  6 */ ".a............a."
		/*  7 */ ".aaaaaaaaaaaaaa."
		/*  8 */ "................"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".affafffagafffa."
		/*  2 */ ".f............f."
		/*  3 */ ".f............f."
		/*  4 */ ".a............a."
		/*  5 */ ".f............f."
		/*  6 */ ".f............f."
		/*  7 */ ".affaffafffaffa."
		/*  8 */ "................"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "cccccccccccccccc"
		/*  1 */ "haaaaaaaaaaaaaah"
		/*  2 */ ".a..i...i.i...a."
		/*  3 */ ".a............a."
		/*  4 */ ".a............a."
		/*  5 */ ".a............a."
		/*  6 */ ".a..j..j...j..a."
		/*  7 */ "kaaaaaaaaaaaaaak"
		/*  8 */ "llllllllllllllll"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "cccccccccccccccc"
		/*  2 */ "haaaaaaaaaaaaaah"
		/*  3 */ ".a............a."
		/*  4 */ ".a............a."
		/*  5 */ ".a............a."
		/*  6 */ "kaaaaaaaaaaaaaak"
		/*  7 */ "llllllllllllllll"
		/*  8 */ "................"

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "cccccccccccccccc"
		/*  3 */ "haaaaaaaaaaaaaah"
		/*  4 */ ".a............a."
		/*  5 */ "kaaaaaaaaaaaaaak"
		/*  6 */ "llllllllllllllll"
		/*  7 */ "................"
		/*  8 */ "................"

		// Level 7
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "cccccccccccccccc"
		/*  4 */ "aaaaaaaaaaaaaaaa"
		/*  5 */ "llllllllllllllll"
		/*  6 */ "................"
		/*  7 */ "................"
		/*  8 */ "................",

		// Connectors:
		"-1: 9, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House16x9



	////////////////////////////////////////////////////////////////////////////////
	// House7x7:
	// The data has been exported from the gallery Desert, area index 8, ID 112, created by Aloe_vera
	{
		// Size:
		7, 7, 7,  // SizeX = 7, SizeY = 7, SizeZ = 7

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		7, 6, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j:128: 6\n"  /* sandstonestairs */
		"k:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "mmmaaam"
		/*  1 */ "maaaaam"
		/*  2 */ "maaaaam"
		/*  3 */ "maaaaam"
		/*  4 */ "maaaaam"
		/*  5 */ "maaaaam"
		/*  6 */ "mmmmmmm"

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "...bcd."
		/*  1 */ ".aaaaa."
		/*  2 */ ".aaaaa."
		/*  3 */ ".aaaaa."
		/*  4 */ ".aaaaa."
		/*  5 */ ".aaaaa."
		/*  6 */ "......."

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".aaaea."
		/*  2 */ ".a...a."
		/*  3 */ ".a...a."
		/*  4 */ ".a...a."
		/*  5 */ ".aaaaa."
		/*  6 */ "......."

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".afaga."
		/*  2 */ ".f...f."
		/*  3 */ ".f...f."
		/*  4 */ ".f...f."
		/*  5 */ ".afffa."
		/*  6 */ "......."

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "ccccccc"
		/*  1 */ "haaaaah"
		/*  2 */ ".a.i.a."
		/*  3 */ ".a...a."
		/*  4 */ ".a...a."
		/*  5 */ "jaaaaaj"
		/*  6 */ "kkkkkkk"

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "ccccccc"
		/*  2 */ "haaaaah"
		/*  3 */ ".a...a."
		/*  4 */ "jaaaaaj"
		/*  5 */ "kkkkkkk"
		/*  6 */ "......."

		// Level 6
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "ccccccc"
		/*  3 */ "aaaaaaa"
		/*  4 */ "kkkkkkk"
		/*  5 */ "......."
		/*  6 */ ".......",

		// Connectors:
		"-1: 4, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House7x7



	////////////////////////////////////////////////////////////////////////////////
	// House9x7:
	// The data has been exported from the gallery Desert, area index 9, ID 113, created by xoft
	{
		// Size:
		9, 7, 7,  // SizeX = 9, SizeY = 7, SizeZ = 7

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		9, 6, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j: 50: 4\n"  /* torch */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   012345678 */
		/*  0 */ "mmmaaammm"
		/*  1 */ "maaaaaaam"
		/*  2 */ "maaaaaaam"
		/*  3 */ "maaaaaaam"
		/*  4 */ "maaaaaaam"
		/*  5 */ "maaaaaaam"
		/*  6 */ "mmmmmmmmm"

		// Level 1
		/* z\x*   012345678 */
		/*  0 */ "...bcd..."
		/*  1 */ ".aaaaaaa."
		/*  2 */ ".aaaaaaa."
		/*  3 */ ".aaaaaaa."
		/*  4 */ ".aaaaaaa."
		/*  5 */ ".aaaaaaa."
		/*  6 */ "........."

		// Level 2
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".aaaeaaa."
		/*  2 */ ".a.....a."
		/*  3 */ ".a.....a."
		/*  4 */ ".a.....a."
		/*  5 */ ".aaaaaaa."
		/*  6 */ "........."

		// Level 3
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".afagafa."
		/*  2 */ ".f.....f."
		/*  3 */ ".f.....f."
		/*  4 */ ".f.....f."
		/*  5 */ ".affaffa."
		/*  6 */ "........."

		// Level 4
		/* z\x*   012345678 */
		/*  0 */ "ccccccccc"
		/*  1 */ "haaaaaaah"
		/*  2 */ ".a.i.i.a."
		/*  3 */ ".a.....a."
		/*  4 */ ".a..j..a."
		/*  5 */ "kaaaaaaak"
		/*  6 */ "lllllllll"

		// Level 5
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "ccccccccc"
		/*  2 */ "haaaaaaah"
		/*  3 */ ".a.....a."
		/*  4 */ "kaaaaaaak"
		/*  5 */ "lllllllll"
		/*  6 */ "........."

		// Level 6
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "ccccccccc"
		/*  3 */ "aaaaaaaaa"
		/*  4 */ "lllllllll"
		/*  5 */ "........."
		/*  6 */ ".........",

		// Connectors:
		"-1: 4, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House9x7



	////////////////////////////////////////////////////////////////////////////////
	// House9x9:
	// The data has been exported from the gallery Desert, area index 10, ID 114, created by xoft
	{
		// Size:
		9, 8, 9,  // SizeX = 9, SizeY = 8, SizeZ = 9

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		9, 7, 9,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:102: 0\n"  /* glasspane */
		"g: 64:12\n"  /* wooddoorblock */
		"h:128: 7\n"  /* sandstonestairs */
		"i: 50: 3\n"  /* torch */
		"j: 50: 4\n"  /* torch */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   012345678 */
		/*  0 */ "mmmaaammm"
		/*  1 */ "maaaaaaam"
		/*  2 */ "maaaaaaam"
		/*  3 */ "maaaaaaam"
		/*  4 */ "maaaaaaam"
		/*  5 */ "maaaaaaam"
		/*  6 */ "maaaaaaam"
		/*  7 */ "maaaaaaam"
		/*  8 */ "mmmmmmmmm"

		// Level 1
		/* z\x*   012345678 */
		/*  0 */ "...bcd..."
		/*  1 */ ".aaaaaaa."
		/*  2 */ ".aaaaaaa."
		/*  3 */ ".aaaaaaa."
		/*  4 */ ".aaaaaaa."
		/*  5 */ ".aaaaaaa."
		/*  6 */ ".aaaaaaa."
		/*  7 */ ".aaaaaaa."
		/*  8 */ "........."

		// Level 2
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".aaaeaaa."
		/*  2 */ ".a.....a."
		/*  3 */ ".a.....a."
		/*  4 */ ".a.....a."
		/*  5 */ ".a.....a."
		/*  6 */ ".a.....a."
		/*  7 */ ".aaaaaaa."
		/*  8 */ "........."

		// Level 3
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".afagafa."
		/*  2 */ ".f.....f."
		/*  3 */ ".f.....f."
		/*  4 */ ".a.....a."
		/*  5 */ ".f.....f."
		/*  6 */ ".f.....f."
		/*  7 */ ".affaffa."
		/*  8 */ "........."

		// Level 4
		/* z\x*   012345678 */
		/*  0 */ "ccccccccc"
		/*  1 */ "haaaaaaah"
		/*  2 */ ".a.i.i.a."
		/*  3 */ ".a.....a."
		/*  4 */ ".a.....a."
		/*  5 */ ".a.....a."
		/*  6 */ ".a..j..a."
		/*  7 */ "kaaaaaaak"
		/*  8 */ "lllllllll"

		// Level 5
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "ccccccccc"
		/*  2 */ "haaaaaaah"
		/*  3 */ ".a.....a."
		/*  4 */ ".a.....a."
		/*  5 */ ".a.....a."
		/*  6 */ "kaaaaaaak"
		/*  7 */ "lllllllll"
		/*  8 */ "........."

		// Level 6
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "ccccccccc"
		/*  3 */ "haaaaaaah"
		/*  4 */ ".a.....a."
		/*  5 */ "kaaaaaaak"
		/*  6 */ "lllllllll"
		/*  7 */ "........."
		/*  8 */ "........."

		// Level 7
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "........."
		/*  3 */ "ccccccccc"
		/*  4 */ "aaaaaaaaa"
		/*  5 */ "lllllllll"
		/*  6 */ "........."
		/*  7 */ "........."
		/*  8 */ ".........",

		// Connectors:
		"-1: 4, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // House9x9



	////////////////////////////////////////////////////////////////////////////////
	// HouseL14x12:
	// The data has been exported from the gallery Desert, area index 14, ID 124, created by Aloe_vera
	{
		// Size:
		14, 8, 12,  // SizeX = 14, SizeY = 8, SizeZ = 12

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		14, 7, 12,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e:128: 3\n"  /* sandstonestairs */
		"f: 64: 3\n"  /* wooddoorblock */
		"g: 64: 1\n"  /* wooddoorblock */
		"h:102: 0\n"  /* glasspane */
		"i: 64: 8\n"  /* wooddoorblock */
		"j:128: 7\n"  /* sandstonestairs */
		"k: 50: 3\n"  /* torch */
		"l: 50: 2\n"  /* torch */
		"m: 19: 0\n"  /* sponge */
		"n: 50: 4\n"  /* torch */
		"o:128: 6\n"  /* sandstonestairs */
		"p: 50: 1\n"  /* torch */
		"q:128: 5\n"  /* sandstonestairs */
		"r:128: 4\n"  /* sandstonestairs */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmaaammmmmmm"
		/*  1 */ "maaaaaaaaaaaam"
		/*  2 */ "maaaaaaaaaaaam"
		/*  3 */ "maaaaaaaaaaaam"
		/*  4 */ "maaaaaaaaaaaam"
		/*  5 */ "maaaaaaaaaaaam"
		/*  6 */ "maaaaaaaaaaaam"
		/*  7 */ "maaaaaaaaaaaam"
		/*  8 */ "mmmmaaaaaaaaam"
		/*  9 */ "mmmmmmaaaaaaam"
		/* 10 */ "mmmmmmaaaaaaam"
		/* 11 */ "mmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "....bcd......."
		/*  1 */ ".aaaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaaa."
		/*  4 */ ".aaaaaaaaaaaa."
		/*  5 */ ".aaaaaaaaaaaa."
		/*  6 */ ".aaaaaaaaaaaa."
		/*  7 */ ".aaaaaaaaaaaa."
		/*  8 */ "....beaaaaaaa."
		/*  9 */ "mmmmm.aaaaaaa."
		/* 10 */ "mmmmm.aaaaaaa."
		/* 11 */ "mmmmm........."

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".aaaafaaaaaaa."
		/*  2 */ ".a..........a."
		/*  3 */ ".a..........a."
		/*  4 */ ".a..........a."
		/*  5 */ ".a..........a."
		/*  6 */ ".a..........a."
		/*  7 */ ".aaaaga.....a."
		/*  8 */ "......a.....a."
		/*  9 */ "mmmmm.a.....a."
		/* 10 */ "mmmmm.aaaaaaa."
		/* 11 */ "mmmmm........."

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".ahhaiahhahha."
		/*  2 */ ".h..........h."
		/*  3 */ ".h..........h."
		/*  4 */ ".a..........a."
		/*  5 */ ".h..........h."
		/*  6 */ ".h..........h."
		/*  7 */ ".ahhaia.....a."
		/*  8 */ "......h.....h."
		/*  9 */ "mmmmm.h.....h."
		/* 10 */ "mmmmm.ahhahha."
		/* 11 */ "mmmmm........."

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "cccccccccccccc"
		/*  1 */ "jaaaaaaaaaaaad"
		/*  2 */ ".a..k.k.....ad"
		/*  3 */ ".a..........ad"
		/*  4 */ ".a.........lad"
		/*  5 */ ".a..........ad"
		/*  6 */ ".a..n.n.....ad"
		/*  7 */ "oaaaaaap...lad"
		/*  8 */ "eeeeeba.....ad"
		/*  9 */ "mmmmmba.....ad"
		/* 10 */ "mmmmmbaaaaaaad"
		/* 11 */ "mmmmmbq.....rd"

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ "ccccccccccccd."
		/*  2 */ "jaaaaaaaaaaad."
		/*  3 */ ".a.........ad."
		/*  4 */ ".a.........ad."
		/*  5 */ ".a.........ad."
		/*  6 */ "oaaaaaaa...ad."
		/*  7 */ "eeeeeeba...ad."
		/*  8 */ "......ba...ad."
		/*  9 */ "mmmmm.ba...ad."
		/* 10 */ "mmmmm.baaaaad."
		/* 11 */ "mmmmm.bq...rd."

		// Level 6
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ "cccccccccccc.."
		/*  3 */ "jaaaaaaaaaad.."
		/*  4 */ ".a........ad.."
		/*  5 */ "oaaaaaaaa.ad.."
		/*  6 */ "eeeeeeeea.ad.."
		/*  7 */ ".......ba.ad.."
		/*  8 */ ".......ba.ad.."
		/*  9 */ "mmmmm..ba.ad.."
		/* 10 */ "mmmmm..baaad.."
		/* 11 */ "mmmmm..bq.rd.."

		// Level 7
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ ".............."
		/*  3 */ "ccccccccccc..."
		/*  4 */ "aaaaaaaaaad..."
		/*  5 */ "eeeeeeeebad..."
		/*  6 */ "........bad..."
		/*  7 */ "........bad..."
		/*  8 */ "........bad..."
		/*  9 */ "mmmmm...bad..."
		/* 10 */ "mmmmm...bad..."
		/* 11 */ "mmmmm...bad...",

		// Connectors:
		"-1: 5, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // HouseL14x12



	////////////////////////////////////////////////////////////////////////////////
	// HouseL14x12:
	// The data has been exported from the gallery Desert, area index 7, ID 82, created by Aloe_vera
	{
		// Size:
		14, 7, 12,  // SizeX = 14, SizeY = 7, SizeZ = 12

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		14, 6, 12,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 0\n"  /* sandstonestairs */
		"c:128: 2\n"  /* sandstonestairs */
		"d:128: 1\n"  /* sandstonestairs */
		"e:128: 3\n"  /* sandstonestairs */
		"f: 64: 7\n"  /* wooddoorblock */
		"g: 64: 5\n"  /* wooddoorblock */
		"h:102: 0\n"  /* glasspane */
		"i: 64:12\n"  /* wooddoorblock */
		"j:128: 7\n"  /* sandstonestairs */
		"k: 50: 3\n"  /* torch */
		"l: 50: 4\n"  /* torch */
		"m: 19: 0\n"  /* sponge */
		"n:128: 6\n"  /* sandstonestairs */
		"o:128: 5\n"  /* sandstonestairs */
		"p:128: 4\n"  /* sandstonestairs */
		"q: 50: 1\n"  /* torch */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmaaammmm"
		/*  1 */ "maaaaaaaaaaaam"
		/*  2 */ "maaaaaaaaaaaam"
		/*  3 */ "maaaaaaaaaaaam"
		/*  4 */ "maaaaaaaaaaaam"
		/*  5 */ "maaaaaaaaaaaam"
		/*  6 */ "mmmmaaamaaaaam"
		/*  7 */ "mmmmmmmmaaaaam"
		/*  8 */ "mmmmmmmmaaaaam"
		/*  9 */ "mmmmmmmmaaaaam"
		/* 10 */ "mmmmmmmmaaaaam"
		/* 11 */ "mmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".......bcd...."
		/*  1 */ ".aaaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaaa."
		/*  4 */ ".aaaaaaaaaaaa."
		/*  5 */ ".aaaaaaaaaaaa."
		/*  6 */ "....bed.aaaaa."
		/*  7 */ "........aaaaa."
		/*  8 */ "........aaaaa."
		/*  9 */ "........aaaaa."
		/* 10 */ "........aaaaa."
		/* 11 */ ".............."

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".aaaaaaafaaaa."
		/*  2 */ ".a..........a."
		/*  3 */ ".a..........a."
		/*  4 */ ".a..........a."
		/*  5 */ ".aaaagaaa...a."
		/*  6 */ "........a...a."
		/*  7 */ "........a...a."
		/*  8 */ "........a...a."
		/*  9 */ "........a...a."
		/* 10 */ "........aaaaa."
		/* 11 */ ".............."

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".ahhahhaiahha."
		/*  2 */ ".h..........h."
		/*  3 */ ".h..........h."
		/*  4 */ ".h..........a."
		/*  5 */ ".ahhaiahh...h."
		/*  6 */ "........h...h."
		/*  7 */ "........a...a."
		/*  8 */ "........h...h."
		/*  9 */ "........h...h."
		/* 10 */ "........ahhha."
		/* 11 */ ".............."

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "cccccccccccccc"
		/*  1 */ "jaaaaaaaaaaaad"
		/*  2 */ ".a.....k.k..ad"
		/*  3 */ ".a..........ad"
		/*  4 */ ".a..l.l.....ad"
		/*  5 */ "naaaaaaaa...ad"
		/*  6 */ "eeeeeeeba...ad"
		/*  7 */ ".......ba...ad"
		/*  8 */ ".......ba...ad"
		/*  9 */ ".......ba...ad"
		/* 10 */ ".......baaaaad"
		/* 11 */ ".......bo...pd"

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ "ccccccccccccc."
		/*  2 */ "jaaaaaaaaaaad."
		/*  3 */ ".aq........ad."
		/*  4 */ "naaaaaaaaa.ad."
		/*  5 */ "eeeeeeeeba.ad."
		/*  6 */ "........ba.ad."
		/*  7 */ "........ba.ad."
		/*  8 */ "........ba.ad."
		/*  9 */ "........balad."
		/* 10 */ "........baaad."
		/* 11 */ "........bo.pd."

		// Level 6
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ "cccccccccccc.."
		/*  3 */ "aaaaaaaaaaad.."
		/*  4 */ "eeeeeeeeebad.."
		/*  5 */ ".........bad.."
		/*  6 */ ".........bad.."
		/*  7 */ ".........bad.."
		/*  8 */ ".........bad.."
		/*  9 */ ".........bad.."
		/* 10 */ ".........bad.."
		/* 11 */ ".........bad..",

		// Connectors:
		"-1: 8, 1, 0: 2\n"  /* Type -1, direction Z- */,

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
		true,
	},  // HouseL14x12



	////////////////////////////////////////////////////////////////////////////////
	// SingleField:
	// The data has been exported from the gallery Desert, area index 17, ID 127, created by Aloe_vera
	{
		// Size:
		10, 3, 7,  // SizeX = 10, SizeY = 3, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, -1,  // MinX, MinY, MinZ
		10, 2, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b: 60: 7\n"  /* tilleddirt */
		"c:  8: 0\n"  /* water */
		"d: 50: 5\n"  /* torch */
		"e: 59: 7\n"  /* crops */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "aaaaaaaaaa"
		/*  1 */ "aaaaaaaaaa"
		/*  2 */ "aaaaaaaaaa"
		/*  3 */ "aaaaaaaaaa"
		/*  4 */ "aaaaaaaaaa"
		/*  5 */ "aaaaaaaaaa"
		/*  6 */ "aaaaaaaaaa"

		// Level 1
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "aaaaaaaaaa"
		/*  1 */ "abbbbbbbba"
		/*  2 */ "abbbbbbbba"
		/*  3 */ "acccccccca"
		/*  4 */ "abbbbbbbba"
		/*  5 */ "abbbbbbbba"
		/*  6 */ "aaaaaaaaaa"

		// Level 2
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "d........d"
		/*  1 */ ".eeeeeeee."
		/*  2 */ ".eeeeeeee."
		/*  3 */ ".........."
		/*  4 */ ".eeeeeeee."
		/*  5 */ ".eeeeeeee."
		/*  6 */ "d........d",

		// Connectors:
		"-1: 0, 1, 3: 4\n"  /* Type -1, direction X- */,

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
		true,
	},  // SingleField



	////////////////////////////////////////////////////////////////////////////////
	// SmallHut:
	// The data has been exported from the gallery Desert, area index 4, ID 68, created by tonibm1999
	{
		// Size:
		5, 6, 6,  // SizeX = 5, SizeY = 6, SizeZ = 6

		// Hitbox (relative to bounding box):
		-1, 0, -1,  // MinX, MinY, MinZ
		5, 5, 5,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 24: 0\n"  /* sandstone */
		"b:128: 3\n"  /* sandstonestairs */
		"c: 24: 2\n"  /* sandstone */
		"d: 50: 5\n"  /* torch */
		"e: 26:10\n"  /* bedblock */
		"f: 26: 2\n"  /* bedblock */
		"g: 64: 5\n"  /* wooddoorblock */
		"h: 64:12\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   01234 */
		/*  0 */ "aaaaa"
		/*  1 */ "aaaaa"
		/*  2 */ "aaaaa"
		/*  3 */ "aaaaa"
		/*  4 */ "aaaaa"
		/*  5 */ "mmamm"

		// Level 1
		/* z\x*   01234 */
		/*  0 */ "aaaaa"
		/*  1 */ "aaaaa"
		/*  2 */ "aaaaa"
		/*  3 */ "aaaaa"
		/*  4 */ "aaaaa"
		/*  5 */ "..b.."

		// Level 2
		/* z\x*   01234 */
		/*  0 */ "accca"
		/*  1 */ "cdedc"
		/*  2 */ "c.f.c"
		/*  3 */ "c...c"
		/*  4 */ "acgca"
		/*  5 */ "....."

		// Level 3
		/* z\x*   01234 */
		/*  0 */ "ac.ca"
		/*  1 */ "c...c"
		/*  2 */ "....."
		/*  3 */ "c...c"
		/*  4 */ "achca"
		/*  5 */ "....."

		// Level 4
		/* z\x*   01234 */
		/*  0 */ "accca"
		/*  1 */ "c...c"
		/*  2 */ "c...c"
		/*  3 */ "c...c"
		/*  4 */ "accca"
		/*  5 */ "....."

		// Level 5
		/* z\x*   01234 */
		/*  0 */ ".aaa."
		/*  1 */ "aaaaa"
		/*  2 */ "aaaaa"
		/*  3 */ "aaaaa"
		/*  4 */ ".aaa."
		/*  5 */ ".....",

		// Connectors:
		"-1: 2, 1, 5: 3\n"  /* Type -1, direction Z+ */,

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
		true,
	},  // SmallHut
};  // g_SandVillagePrefabs






const cPrefab::sDef g_SandVillageStartingPrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// RoofedWell:
	// The data has been exported from the gallery Desert, area index 43, ID 274, created by Aloe_vera
	{
		// Size:
		7, 14, 7,  // SizeX = 7, SizeY = 14, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 13, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b: 24: 0\n"  /* sandstone */
		"c:  8: 0\n"  /* water */
		"d: 12: 0\n"  /* sand */
		"e:  4: 0\n"  /* cobblestone */
		"f: 13: 0\n"  /* gravel */
		"g:118: 3\n"  /* cauldronblock */
		"h: 85: 0\n"  /* fence */
		"i:128: 2\n"  /* sandstonestairs */
		"j:128: 7\n"  /* sandstonestairs */
		"k:128: 4\n"  /* sandstonestairs */
		"l:128: 5\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */
		"n:128: 6\n"  /* sandstonestairs */
		"o:128: 3\n"  /* sandstonestairs */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "aaaaaaa"
		/*  1 */ "aaaaaaa"
		/*  2 */ "aaaaaaa"
		/*  3 */ "aaaaaaa"
		/*  4 */ "aaaaaaa"
		/*  5 */ "aaaaaaa"
		/*  6 */ "aaaaaaa"

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "aaaaaaa"
		/*  1 */ "abbbbba"
		/*  2 */ "abcccba"
		/*  3 */ "abcccba"
		/*  4 */ "abcccba"
		/*  5 */ "abbbbba"
		/*  6 */ "aaaaaaa"

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "aaaaaaa"
		/*  1 */ "abbbbba"
		/*  2 */ "abcccba"
		/*  3 */ "abcccba"
		/*  4 */ "abcccba"
		/*  5 */ "abbbbba"
		/*  6 */ "aaaaaaa"

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ "aaaaaaa"
		/*  1 */ "abbbbba"
		/*  2 */ "abcccba"
		/*  3 */ "abcccba"
		/*  4 */ "abcccba"
		/*  5 */ "abbbbba"
		/*  6 */ "aaaaaaa"

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "ddddddd"
		/*  1 */ "dbbbbbd"
		/*  2 */ "dbcccbd"
		/*  3 */ "dbcccbd"
		/*  4 */ "dbcccbd"
		/*  5 */ "dbbbbbd"
		/*  6 */ "ddddddd"

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "ddddddd"
		/*  1 */ "dbbbbbd"
		/*  2 */ "dbcccbd"
		/*  3 */ "dbcccbd"
		/*  4 */ "dbcccbd"
		/*  5 */ "dbbbbbd"
		/*  6 */ "ddddddd"

		// Level 6
		/* z\x*   0123456 */
		/*  0 */ "ddeeedd"
		/*  1 */ "dbbbbbd"
		/*  2 */ "ebcccbe"
		/*  3 */ "ebcccbe"
		/*  4 */ "ebcccbe"
		/*  5 */ "dbbbbbd"
		/*  6 */ "ddeeedd"

		// Level 7
		/* z\x*   0123456 */
		/*  0 */ "ddfffdd"
		/*  1 */ "dbbbbbd"
		/*  2 */ "fbcccbf"
		/*  3 */ "fbcccbf"
		/*  4 */ "fbcccbf"
		/*  5 */ "dbbbbbd"
		/*  6 */ "ddfffdd"

		// Level 8
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".bbbbb."
		/*  2 */ ".b...b."
		/*  3 */ ".b.g.b."
		/*  4 */ ".b...b."
		/*  5 */ ".bbbbb."
		/*  6 */ "......."

		// Level 9
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".h...h."
		/*  2 */ "......."
		/*  3 */ "...h..."
		/*  4 */ "......."
		/*  5 */ ".h...h."
		/*  6 */ "......."

		// Level 10
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".h...h."
		/*  2 */ "......."
		/*  3 */ "...h..."
		/*  4 */ "......."
		/*  5 */ ".h...h."
		/*  6 */ "......."

		// Level 11
		/* z\x*   0123456 */
		/*  0 */ "iiiiiii"
		/*  1 */ "jbjjjbj"
		/*  2 */ ".k...l."
		/*  3 */ ".k.h.l."
		/*  4 */ ".k...l."
		/*  5 */ "nbnnnbn"
		/*  6 */ "ooooooo"

		// Level 12
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "iiiiiii"
		/*  2 */ "jb...bj"
		/*  3 */ ".b.h.b."
		/*  4 */ "nb...bn"
		/*  5 */ "ooooooo"
		/*  6 */ "......."

		// Level 13
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "iiiiiii"
		/*  3 */ "bbbbbbb"
		/*  4 */ "ooooooo"
		/*  5 */ "......."
		/*  6 */ ".......",

		// Connectors:
		"2: 6, 8, 3: 5\n"  /* Type 2, direction X+ */
		"2: 3, 8, 6: 3\n"  /* Type 2, direction Z+ */
		"2: 0, 8, 3: 4\n"  /* Type 2, direction X- */
		"2: 3, 8, 0: 2\n"  /* Type 2, direction Z- */,

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
		true,
	},  // RoofedWell



	////////////////////////////////////////////////////////////////////////////////
	// Well:
	// The data has been exported from the gallery Desert, area index 0, ID 1, created by Aloe_vera
	{
		// Size:
		4, 13, 4,  // SizeX = 4, SizeY = 13, SizeZ = 4

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		3, 12, 3,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b: 24: 0\n"  /* sandstone */
		"c:  8: 0\n"  /* water */
		"d: 85: 0\n"  /* fence */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   0123 */
		/*  0 */ "aaaa"
		/*  1 */ "aaaa"
		/*  2 */ "aaaa"
		/*  3 */ "aaaa"

		// Level 1
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "bccb"
		/*  2 */ "bccb"
		/*  3 */ "bbbb"

		// Level 2
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "bccb"
		/*  2 */ "bccb"
		/*  3 */ "bbbb"

		// Level 3
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "bccb"
		/*  2 */ "bccb"
		/*  3 */ "bbbb"

		// Level 4
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "bccb"
		/*  2 */ "bccb"
		/*  3 */ "bbbb"

		// Level 5
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "bccb"
		/*  2 */ "bccb"
		/*  3 */ "bbbb"

		// Level 6
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "bccb"
		/*  2 */ "bccb"
		/*  3 */ "bbbb"

		// Level 7
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "bccb"
		/*  2 */ "bccb"
		/*  3 */ "bbbb"

		// Level 8
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "b..b"
		/*  2 */ "b..b"
		/*  3 */ "bbbb"

		// Level 9
		/* z\x*   0123 */
		/*  0 */ "d..d"
		/*  1 */ "...."
		/*  2 */ "...."
		/*  3 */ "d..d"

		// Level 10
		/* z\x*   0123 */
		/*  0 */ "d..d"
		/*  1 */ "...."
		/*  2 */ "...."
		/*  3 */ "d..d"

		// Level 11
		/* z\x*   0123 */
		/*  0 */ "d..d"
		/*  1 */ "...."
		/*  2 */ "...."
		/*  3 */ "d..d"

		// Level 12
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "bbbb"
		/*  2 */ "bbbb"
		/*  3 */ "bbbb",

		// Connectors:
		"2: 2, 8, 0: 2\n"  /* Type 2, direction Z- */
		"2: 0, 8, 1: 4\n"  /* Type 2, direction X- */
		"2: 1, 8, 3: 3\n"  /* Type 2, direction Z+ */
		"2: 3, 8, 2: 5\n"  /* Type 2, direction X+ */,

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
		true,
	},  // Well
};





// The prefab counts:

const size_t g_SandVillagePrefabsCount = ARRAYCOUNT(g_SandVillagePrefabs);

const size_t g_SandVillageStartingPrefabsCount = ARRAYCOUNT(g_SandVillageStartingPrefabs);

