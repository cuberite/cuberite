
// SandVillagePrefabs.cpp

// Defines the prefabs in the group SandVillage

// NOTE: This file has been generated automatically by GalExport!
// Any manual changes will be overwritten by the next automatic export!

#include "Globals.h"
#include "SandVillagePrefabs.h"





const cPrefab::sDef g_SandVillagePrefabs[] =
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DoubleField:
	// The data has been exported from the gallery Desert, area index 5, ID 75, created by tonibm1999
	{
		// Size:
		13, 2, 9,  // SizeX = 13, SizeY = 2, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		12, 1, 8,  // MaxX, MaxY, MaxZ

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
		/*  1 */ "abbcbbabbcbba"
		/*  2 */ "abbcbbabbcbba"
		/*  3 */ "abbcbbabbcbba"
		/*  4 */ "abbcbbabbcbba"
		/*  5 */ "abbcbbabbcbba"
		/*  6 */ "abbcbbabbcbba"
		/*  7 */ "abbcbbabbcbba"
		/*  8 */ "aaaaaaaaaaaaa"

		// Level 1
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "d.....d.....d"
		/*  1 */ ".......ee.ee."
		/*  2 */ ".......ee.ee."
		/*  3 */ ".......ee.ee."
		/*  4 */ ".......ee.ee."
		/*  5 */ ".......ee.ee."
		/*  6 */ ".......ee.ee."
		/*  7 */ ".......ee.ee."
		/*  8 */ "d.....d.....d",

		// Connectors:
		"-1: 6, 0, 8: 3\n"  /* Type -1, direction Z+ */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House11x7:
	// The data has been exported from the gallery Desert, area index 6, ID 81, created by Aloe_vera
	{
		// Size:
		11, 6, 7,  // SizeX = 11, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 5, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ "....abc...."
		/*  1 */ ".ddddddddd."
		/*  2 */ ".ddddddddd."
		/*  3 */ ".ddddddddd."
		/*  4 */ ".ddddddddd."
		/*  5 */ ".ddddddddd."
		/*  6 */ "..........."

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".ddddedddd."
		/*  2 */ ".d.......d."
		/*  3 */ ".d.......d."
		/*  4 */ ".d.......d."
		/*  5 */ ".ddddddddd."
		/*  6 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".dffdgdffd."
		/*  2 */ ".f.......f."
		/*  3 */ ".f.......f."
		/*  4 */ ".f.......f."
		/*  5 */ ".dffdfdffd."
		/*  6 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "bbbbbbbbbbb"
		/*  1 */ "hdddddddddh"
		/*  2 */ ".d..i.i..d."
		/*  3 */ ".d.......d."
		/*  4 */ ".d..j.j..d."
		/*  5 */ "kdddddddddk"
		/*  6 */ "lllllllllll"

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "bbbbbbbbbbb"
		/*  2 */ "hdddddddddh"
		/*  3 */ ".dn.....od."
		/*  4 */ "kdddddddddk"
		/*  5 */ "lllllllllll"
		/*  6 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "bbbbbbbbbbb"
		/*  3 */ "ddddddddddd"
		/*  4 */ "lllllllllll"
		/*  5 */ "..........."
		/*  6 */ "...........",

		// Connectors:
		"-1: 5, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House11x9:
	// The data has been exported from the gallery Desert, area index 11, ID 115, created by xoft
	{
		// Size:
		11, 7, 9,  // SizeX = 11, SizeY = 7, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 6, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ "....abc...."
		/*  1 */ ".ddddddddd."
		/*  2 */ ".ddddddddd."
		/*  3 */ ".ddddddddd."
		/*  4 */ ".ddddddddd."
		/*  5 */ ".ddddddddd."
		/*  6 */ ".ddddddddd."
		/*  7 */ ".ddddddddd."
		/*  8 */ "..........."

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".ddddedddd."
		/*  2 */ ".d.......d."
		/*  3 */ ".d.......d."
		/*  4 */ ".d.......d."
		/*  5 */ ".d.......d."
		/*  6 */ ".d.......d."
		/*  7 */ ".ddddddddd."
		/*  8 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".dffdgdffd."
		/*  2 */ ".f.......f."
		/*  3 */ ".f.......f."
		/*  4 */ ".d.......d."
		/*  5 */ ".f.......f."
		/*  6 */ ".f.......f."
		/*  7 */ ".dfffdfffd."
		/*  8 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "bbbbbbbbbbb"
		/*  1 */ "hdddddddddh"
		/*  2 */ ".d..i.i..d."
		/*  3 */ ".d.......d."
		/*  4 */ ".d.......d."
		/*  5 */ ".d.......d."
		/*  6 */ ".d...j...d."
		/*  7 */ "kdddddddddk"
		/*  8 */ "lllllllllll"

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "bbbbbbbbbbb"
		/*  2 */ "hdddddddddh"
		/*  3 */ ".d.......d."
		/*  4 */ ".d.......d."
		/*  5 */ ".d.......d."
		/*  6 */ "kdddddddddk"
		/*  7 */ "lllllllllll"
		/*  8 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "bbbbbbbbbbb"
		/*  3 */ "hdddddddddh"
		/*  4 */ ".d.......d."
		/*  5 */ "kdddddddddk"
		/*  6 */ "lllllllllll"
		/*  7 */ "..........."
		/*  8 */ "..........."

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "bbbbbbbbbbb"
		/*  4 */ "ddddddddddd"
		/*  5 */ "lllllllllll"
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "...........",

		// Connectors:
		"-1: 5, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House13x7:
	// The data has been exported from the gallery Desert, area index 15, ID 125, created by Aloe_vera
	{
		// Size:
		13, 6, 7,  // SizeX = 13, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		12, 5, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ ".....abc....."
		/*  1 */ ".ddddddddddd."
		/*  2 */ ".ddddddddddd."
		/*  3 */ ".ddddddddddd."
		/*  4 */ ".ddddddddddd."
		/*  5 */ ".ddddddddddd."
		/*  6 */ "............."

		// Level 1
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".dddddeddddd."
		/*  2 */ ".d.........d."
		/*  3 */ ".d.........d."
		/*  4 */ ".d.........d."
		/*  5 */ ".ddddddddddd."
		/*  6 */ "............."

		// Level 2
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".dfffdgdfffd."
		/*  2 */ ".f.........f."
		/*  3 */ ".f.........f."
		/*  4 */ ".f.........f."
		/*  5 */ ".dffdfffdffd."
		/*  6 */ "............."

		// Level 3
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "bbbbbbbbbbbbb"
		/*  1 */ "hdddddddddddh"
		/*  2 */ ".d...i.i...d."
		/*  3 */ ".d.........d."
		/*  4 */ ".d..j...j..d."
		/*  5 */ "kdddddddddddk"
		/*  6 */ "lllllllllllll"

		// Level 4
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "bbbbbbbbbbbbb"
		/*  2 */ "hdddddddddddh"
		/*  3 */ ".d.........d."
		/*  4 */ "kdddddddddddk"
		/*  5 */ "lllllllllllll"
		/*  6 */ "............."

		// Level 5
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "............."
		/*  2 */ "bbbbbbbbbbbbb"
		/*  3 */ "ddddddddddddd"
		/*  4 */ "lllllllllllll"
		/*  5 */ "............."
		/*  6 */ ".............",

		// Connectors:
		"-1: 6, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House13x9:
	// The data has been exported from the gallery Desert, area index 12, ID 116, created by xoft
	{
		// Size:
		13, 7, 9,  // SizeX = 13, SizeY = 7, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		12, 6, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ ".....abc....."
		/*  1 */ ".ddddddddddd."
		/*  2 */ ".ddddddddddd."
		/*  3 */ ".ddddddddddd."
		/*  4 */ ".ddddddddddd."
		/*  5 */ ".ddddddddddd."
		/*  6 */ ".ddddddddddd."
		/*  7 */ ".ddddddddddd."
		/*  8 */ "............."

		// Level 1
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".dddddeddddd."
		/*  2 */ ".d.........d."
		/*  3 */ ".d.........d."
		/*  4 */ ".d.........d."
		/*  5 */ ".d.........d."
		/*  6 */ ".d.........d."
		/*  7 */ ".ddddddddddd."
		/*  8 */ "............."

		// Level 2
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".dfffdgdfffd."
		/*  2 */ ".f.........f."
		/*  3 */ ".f.........f."
		/*  4 */ ".d.........d."
		/*  5 */ ".f.........f."
		/*  6 */ ".f.........f."
		/*  7 */ ".dffdffdfffd."
		/*  8 */ "............."

		// Level 3
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "bbbbbbbbbbbbb"
		/*  1 */ "hdddddddddddh"
		/*  2 */ ".d...i.i...d."
		/*  3 */ ".d.........d."
		/*  4 */ ".d.........d."
		/*  5 */ ".d.........d."
		/*  6 */ ".d..j..j...d."
		/*  7 */ "kdddddddddddk"
		/*  8 */ "lllllllllllll"

		// Level 4
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "bbbbbbbbbbbbb"
		/*  2 */ "hdddddddddddh"
		/*  3 */ ".d.........d."
		/*  4 */ ".d.........d."
		/*  5 */ ".d.........d."
		/*  6 */ "kdddddddddddk"
		/*  7 */ "lllllllllllll"
		/*  8 */ "............."

		// Level 5
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "............."
		/*  2 */ "bbbbbbbbbbbbb"
		/*  3 */ "hdddddddddddh"
		/*  4 */ ".d.........d."
		/*  5 */ "kdddddddddddk"
		/*  6 */ "lllllllllllll"
		/*  7 */ "............."
		/*  8 */ "............."

		// Level 6
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "............."
		/*  2 */ "............."
		/*  3 */ "bbbbbbbbbbbbb"
		/*  4 */ "ddddddddddddd"
		/*  5 */ "lllllllllllll"
		/*  6 */ "............."
		/*  7 */ "............."
		/*  8 */ ".............",

		// Connectors:
		"-1: 6, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House15x9:
	// The data has been exported from the gallery Desert, area index 13, ID 118, created by xoft
	{
		// Size:
		15, 7, 9,  // SizeX = 15, SizeY = 7, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		14, 6, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ ".....abc......."
		/*  1 */ ".ddddddddddddd."
		/*  2 */ ".ddddddddddddd."
		/*  3 */ ".ddddddddddddd."
		/*  4 */ ".ddddddddddddd."
		/*  5 */ ".ddddddddddddd."
		/*  6 */ ".ddddddddddddd."
		/*  7 */ ".ddddddddddddd."
		/*  8 */ "..............."

		// Level 1
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".dddddeddddddd."
		/*  2 */ ".d...........d."
		/*  3 */ ".d...........d."
		/*  4 */ ".d...........d."
		/*  5 */ ".d...........d."
		/*  6 */ ".d...........d."
		/*  7 */ ".ddddddddddddd."
		/*  8 */ "..............."

		// Level 2
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".dfffdgdffdffd."
		/*  2 */ ".f...........f."
		/*  3 */ ".f...........f."
		/*  4 */ ".d...........d."
		/*  5 */ ".f...........f."
		/*  6 */ ".f...........f."
		/*  7 */ ".dffdffdffdffd."
		/*  8 */ "..............."

		// Level 3
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "bbbbbbbbbbbbbbb"
		/*  1 */ "hdddddddddddddh"
		/*  2 */ ".d...i.i..i..d."
		/*  3 */ ".d...........d."
		/*  4 */ ".d...........d."
		/*  5 */ ".d...........d."
		/*  6 */ ".d..j..j..j..d."
		/*  7 */ "kdddddddddddddk"
		/*  8 */ "lllllllllllllll"

		// Level 4
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "bbbbbbbbbbbbbbb"
		/*  2 */ "hdddddddddddddh"
		/*  3 */ ".d...........d."
		/*  4 */ ".d...........d."
		/*  5 */ ".d...........d."
		/*  6 */ "kdddddddddddddk"
		/*  7 */ "lllllllllllllll"
		/*  8 */ "..............."

		// Level 5
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "..............."
		/*  2 */ "bbbbbbbbbbbbbbb"
		/*  3 */ "hdddddddddddddh"
		/*  4 */ ".d...........d."
		/*  5 */ "kdddddddddddddk"
		/*  6 */ "lllllllllllllll"
		/*  7 */ "..............."
		/*  8 */ "..............."

		// Level 6
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "..............."
		/*  2 */ "..............."
		/*  3 */ "bbbbbbbbbbbbbbb"
		/*  4 */ "ddddddddddddddd"
		/*  5 */ "lllllllllllllll"
		/*  6 */ "..............."
		/*  7 */ "..............."
		/*  8 */ "...............",

		// Connectors:
		"-1: 6, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House16x9:
	// The data has been exported from the gallery Desert, area index 16, ID 126, created by Aloe_vera
	{
		// Size:
		16, 7, 9,  // SizeX = 16, SizeY = 7, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 6, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ "........abc....."
		/*  1 */ ".dddddddddddddd."
		/*  2 */ ".dddddddddddddd."
		/*  3 */ ".dddddddddddddd."
		/*  4 */ ".dddddddddddddd."
		/*  5 */ ".dddddddddddddd."
		/*  6 */ ".dddddddddddddd."
		/*  7 */ ".dddddddddddddd."
		/*  8 */ "................"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".ddddddddeddddd."
		/*  2 */ ".d............d."
		/*  3 */ ".d............d."
		/*  4 */ ".d............d."
		/*  5 */ ".d............d."
		/*  6 */ ".d............d."
		/*  7 */ ".dddddddddddddd."
		/*  8 */ "................"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".dffdfffdgdfffd."
		/*  2 */ ".f............f."
		/*  3 */ ".f............f."
		/*  4 */ ".d............d."
		/*  5 */ ".f............f."
		/*  6 */ ".f............f."
		/*  7 */ ".dffdffdfffdffd."
		/*  8 */ "................"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "bbbbbbbbbbbbbbbb"
		/*  1 */ "hddddddddddddddh"
		/*  2 */ ".d..i...i.i...d."
		/*  3 */ ".d............d."
		/*  4 */ ".d............d."
		/*  5 */ ".d............d."
		/*  6 */ ".d..j..j...j..d."
		/*  7 */ "kddddddddddddddk"
		/*  8 */ "llllllllllllllll"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "bbbbbbbbbbbbbbbb"
		/*  2 */ "hddddddddddddddh"
		/*  3 */ ".d............d."
		/*  4 */ ".d............d."
		/*  5 */ ".d............d."
		/*  6 */ "kddddddddddddddk"
		/*  7 */ "llllllllllllllll"
		/*  8 */ "................"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "bbbbbbbbbbbbbbbb"
		/*  3 */ "hddddddddddddddh"
		/*  4 */ ".d............d."
		/*  5 */ "kddddddddddddddk"
		/*  6 */ "llllllllllllllll"
		/*  7 */ "................"
		/*  8 */ "................"

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "bbbbbbbbbbbbbbbb"
		/*  4 */ "dddddddddddddddd"
		/*  5 */ "llllllllllllllll"
		/*  6 */ "................"
		/*  7 */ "................"
		/*  8 */ "................",

		// Connectors:
		"-1: 9, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House7x7:
	// The data has been exported from the gallery Desert, area index 8, ID 112, created by Aloe_vera
	{
		// Size:
		7, 6, 7,  // SizeX = 7, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 5, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ "...abc."
		/*  1 */ ".ddddd."
		/*  2 */ ".ddddd."
		/*  3 */ ".ddddd."
		/*  4 */ ".ddddd."
		/*  5 */ ".ddddd."
		/*  6 */ "......."

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ddded."
		/*  2 */ ".d...d."
		/*  3 */ ".d...d."
		/*  4 */ ".d...d."
		/*  5 */ ".ddddd."
		/*  6 */ "......."

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".dfdgd."
		/*  2 */ ".f...f."
		/*  3 */ ".f...f."
		/*  4 */ ".f...f."
		/*  5 */ ".dfffd."
		/*  6 */ "......."

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ "bbbbbbb"
		/*  1 */ "hdddddh"
		/*  2 */ ".d.i.d."
		/*  3 */ ".d...d."
		/*  4 */ ".d...d."
		/*  5 */ "jdddddj"
		/*  6 */ "kkkkkkk"

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "bbbbbbb"
		/*  2 */ "hdddddh"
		/*  3 */ ".d...d."
		/*  4 */ "jdddddj"
		/*  5 */ "kkkkkkk"
		/*  6 */ "......."

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "bbbbbbb"
		/*  3 */ "ddddddd"
		/*  4 */ "kkkkkkk"
		/*  5 */ "......."
		/*  6 */ ".......",

		// Connectors:
		"-1: 4, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House9x7:
	// The data has been exported from the gallery Desert, area index 9, ID 113, created by xoft
	{
		// Size:
		9, 6, 7,  // SizeX = 9, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		8, 5, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ "...abc..."
		/*  1 */ ".ddddddd."
		/*  2 */ ".ddddddd."
		/*  3 */ ".ddddddd."
		/*  4 */ ".ddddddd."
		/*  5 */ ".ddddddd."
		/*  6 */ "........."

		// Level 1
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".dddeddd."
		/*  2 */ ".d.....d."
		/*  3 */ ".d.....d."
		/*  4 */ ".d.....d."
		/*  5 */ ".ddddddd."
		/*  6 */ "........."

		// Level 2
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".dfdgdfd."
		/*  2 */ ".f.....f."
		/*  3 */ ".f.....f."
		/*  4 */ ".f.....f."
		/*  5 */ ".dffdffd."
		/*  6 */ "........."

		// Level 3
		/* z\x*   012345678 */
		/*  0 */ "bbbbbbbbb"
		/*  1 */ "hdddddddh"
		/*  2 */ ".d.i.i.d."
		/*  3 */ ".d.....d."
		/*  4 */ ".d..j..d."
		/*  5 */ "kdddddddk"
		/*  6 */ "lllllllll"

		// Level 4
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "bbbbbbbbb"
		/*  2 */ "hdddddddh"
		/*  3 */ ".d.....d."
		/*  4 */ "kdddddddk"
		/*  5 */ "lllllllll"
		/*  6 */ "........."

		// Level 5
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "bbbbbbbbb"
		/*  3 */ "ddddddddd"
		/*  4 */ "lllllllll"
		/*  5 */ "........."
		/*  6 */ ".........",

		// Connectors:
		"-1: 4, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// House9x9:
	// The data has been exported from the gallery Desert, area index 10, ID 114, created by xoft
	{
		// Size:
		9, 7, 9,  // SizeX = 9, SizeY = 7, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		8, 6, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ "...abc..."
		/*  1 */ ".ddddddd."
		/*  2 */ ".ddddddd."
		/*  3 */ ".ddddddd."
		/*  4 */ ".ddddddd."
		/*  5 */ ".ddddddd."
		/*  6 */ ".ddddddd."
		/*  7 */ ".ddddddd."
		/*  8 */ "........."

		// Level 1
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".dddeddd."
		/*  2 */ ".d.....d."
		/*  3 */ ".d.....d."
		/*  4 */ ".d.....d."
		/*  5 */ ".d.....d."
		/*  6 */ ".d.....d."
		/*  7 */ ".ddddddd."
		/*  8 */ "........."

		// Level 2
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".dfdgdfd."
		/*  2 */ ".f.....f."
		/*  3 */ ".f.....f."
		/*  4 */ ".d.....d."
		/*  5 */ ".f.....f."
		/*  6 */ ".f.....f."
		/*  7 */ ".dffdffd."
		/*  8 */ "........."

		// Level 3
		/* z\x*   012345678 */
		/*  0 */ "bbbbbbbbb"
		/*  1 */ "hdddddddh"
		/*  2 */ ".d.i.i.d."
		/*  3 */ ".d.....d."
		/*  4 */ ".d.....d."
		/*  5 */ ".d.....d."
		/*  6 */ ".d..j..d."
		/*  7 */ "kdddddddk"
		/*  8 */ "lllllllll"

		// Level 4
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "bbbbbbbbb"
		/*  2 */ "hdddddddh"
		/*  3 */ ".d.....d."
		/*  4 */ ".d.....d."
		/*  5 */ ".d.....d."
		/*  6 */ "kdddddddk"
		/*  7 */ "lllllllll"
		/*  8 */ "........."

		// Level 5
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "bbbbbbbbb"
		/*  3 */ "hdddddddh"
		/*  4 */ ".d.....d."
		/*  5 */ "kdddddddk"
		/*  6 */ "lllllllll"
		/*  7 */ "........."
		/*  8 */ "........."

		// Level 6
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "........."
		/*  3 */ "bbbbbbbbb"
		/*  4 */ "ddddddddd"
		/*  5 */ "lllllllll"
		/*  6 */ "........."
		/*  7 */ "........."
		/*  8 */ ".........",

		// Connectors:
		"-1: 4, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// HouseL14x12:
	// The data has been exported from the gallery Desert, area index 14, ID 124, created by Aloe_vera
	{
		// Size:
		14, 7, 12,  // SizeX = 14, SizeY = 7, SizeZ = 12

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		13, 6, 11,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
		"e:128: 3\n"  /* sandstonestairs */
		"f: 64: 7\n"  /* wooddoorblock */
		"g:102: 0\n"  /* glasspane */
		"h: 64:12\n"  /* wooddoorblock */
		"i:128: 7\n"  /* sandstonestairs */
		"j: 50: 3\n"  /* torch */
		"k: 50: 2\n"  /* torch */
		"l: 50: 4\n"  /* torch */
		"m: 19: 0\n"  /* sponge */
		"n:128: 6\n"  /* sandstonestairs */
		"o: 50: 1\n"  /* torch */
		"p:128: 5\n"  /* sandstonestairs */
		"q:128: 4\n"  /* sandstonestairs */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "....abc......."
		/*  1 */ ".dddddddddddd."
		/*  2 */ ".dddddddddddd."
		/*  3 */ ".dddddddddddd."
		/*  4 */ ".dddddddddddd."
		/*  5 */ ".dddddddddddd."
		/*  6 */ ".dddddddddddd."
		/*  7 */ ".dddddddddddd."
		/*  8 */ "....aeddddddd."
		/*  9 */ "mmmmm.ddddddd."
		/* 10 */ "mmmmm.ddddddd."
		/* 11 */ "mmmmm........."

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".ddddfddddddd."
		/*  2 */ ".d..........d."
		/*  3 */ ".d..........d."
		/*  4 */ ".d..........d."
		/*  5 */ ".d..........d."
		/*  6 */ ".d..........d."
		/*  7 */ ".ddddfd.....d."
		/*  8 */ "......d.....d."
		/*  9 */ "mmmmm.d.....d."
		/* 10 */ "mmmmm.ddddddd."
		/* 11 */ "mmmmm........."

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".dggdhdggdggd."
		/*  2 */ ".g..........g."
		/*  3 */ ".g..........g."
		/*  4 */ ".d..........d."
		/*  5 */ ".g..........g."
		/*  6 */ ".g..........g."
		/*  7 */ ".dggdhd.....d."
		/*  8 */ "......g.....g."
		/*  9 */ "mmmmm.g.....g."
		/* 10 */ "mmmmm.dggdggd."
		/* 11 */ "mmmmm........."

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "bbbbbbbbbbbbbb"
		/*  1 */ "iddddddddddddc"
		/*  2 */ ".d..j.j.....dc"
		/*  3 */ ".d..........dc"
		/*  4 */ ".d.........kdc"
		/*  5 */ ".d..........dc"
		/*  6 */ ".d..l.l.....dc"
		/*  7 */ "nddddddo...kdc"
		/*  8 */ "eeeeead.....dc"
		/*  9 */ "mmmmmad.....dc"
		/* 10 */ "mmmmmadddddddc"
		/* 11 */ "mmmmmap.....qc"

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ "bbbbbbbbbbbbc."
		/*  2 */ "idddddddddddc."
		/*  3 */ ".d.........dc."
		/*  4 */ ".d.........dc."
		/*  5 */ ".d.........dc."
		/*  6 */ "nddddddd...dc."
		/*  7 */ "eeeeeead...dc."
		/*  8 */ "......ad...dc."
		/*  9 */ "mmmmm.ad...dc."
		/* 10 */ "mmmmm.adddddc."
		/* 11 */ "mmmmm.ap...qc."

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ "bbbbbbbbbbbb.."
		/*  3 */ "iddddddddddc.."
		/*  4 */ ".d........dc.."
		/*  5 */ "ndddddddd.dc.."
		/*  6 */ "eeeeeeeed.dc.."
		/*  7 */ ".......ad.dc.."
		/*  8 */ ".......ad.dc.."
		/*  9 */ "mmmmm..ad.dc.."
		/* 10 */ "mmmmm..adddc.."
		/* 11 */ "mmmmm..ap.qc.."

		// Level 6
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ ".............."
		/*  3 */ "bbbbbbbbbbb..."
		/*  4 */ "ddddddddddc..."
		/*  5 */ "eeeeeeeeadc..."
		/*  6 */ "........adc..."
		/*  7 */ "........adc..."
		/*  8 */ "........adc..."
		/*  9 */ "mmmmm...adc..."
		/* 10 */ "mmmmm...adc..."
		/* 11 */ "mmmmm...adc...",

		// Connectors:
		"-1: 5, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// HouseL14x12:
	// The data has been exported from the gallery Desert, area index 7, ID 82, created by Aloe_vera
	{
		// Size:
		14, 6, 12,  // SizeX = 14, SizeY = 6, SizeZ = 12

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		13, 5, 11,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:128: 0\n"  /* sandstonestairs */
		"b:128: 2\n"  /* sandstonestairs */
		"c:128: 1\n"  /* sandstonestairs */
		"d: 24: 0\n"  /* sandstone */
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
		/*  0 */ ".......abc...."
		/*  1 */ ".dddddddddddd."
		/*  2 */ ".dddddddddddd."
		/*  3 */ ".dddddddddddd."
		/*  4 */ ".dddddddddddd."
		/*  5 */ ".dddddddddddd."
		/*  6 */ "....aec.ddddd."
		/*  7 */ "mmmmmmm.ddddd."
		/*  8 */ "mmmmmmm.ddddd."
		/*  9 */ "mmmmmmm.ddddd."
		/* 10 */ "mmmmmmm.ddddd."
		/* 11 */ "mmmmmmm......."

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".dddddddfdddd."
		/*  2 */ ".d..........d."
		/*  3 */ ".d..........d."
		/*  4 */ ".d..........d."
		/*  5 */ ".ddddgddd...d."
		/*  6 */ "........d...d."
		/*  7 */ "mmmmmmm.d...d."
		/*  8 */ "mmmmmmm.d...d."
		/*  9 */ "mmmmmmm.d...d."
		/* 10 */ "mmmmmmm.ddddd."
		/* 11 */ "mmmmmmm......."

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".dhhdhhdidhhd."
		/*  2 */ ".h..........h."
		/*  3 */ ".h..........h."
		/*  4 */ ".h..........d."
		/*  5 */ ".dhhdidhh...h."
		/*  6 */ "........h...h."
		/*  7 */ "mmmmmmm.d...d."
		/*  8 */ "mmmmmmm.h...h."
		/*  9 */ "mmmmmmm.h...h."
		/* 10 */ "mmmmmmm.dhhhd."
		/* 11 */ "mmmmmmm......."

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "bbbbbbbbbbbbbb"
		/*  1 */ "jddddddddddddc"
		/*  2 */ ".d.....k.k..dc"
		/*  3 */ ".d..........dc"
		/*  4 */ ".d..l.l.....dc"
		/*  5 */ "ndddddddd...dc"
		/*  6 */ "eeeeeeead...dc"
		/*  7 */ "mmmmmmmad...dc"
		/*  8 */ "mmmmmmmad...dc"
		/*  9 */ "mmmmmmmad...dc"
		/* 10 */ "mmmmmmmadddddc"
		/* 11 */ "mmmmmmmao...pc"

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ "bbbbbbbbbbbbb."
		/*  2 */ "jdddddddddddc."
		/*  3 */ ".dq........dc."
		/*  4 */ "nddddddddd.dc."
		/*  5 */ "eeeeeeeead.dc."
		/*  6 */ "........ad.dc."
		/*  7 */ "mmmmmmm.ad.dc."
		/*  8 */ "mmmmmmm.ad.dc."
		/*  9 */ "mmmmmmm.adldc."
		/* 10 */ "mmmmmmm.adddc."
		/* 11 */ "mmmmmmm.ao.pc."

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ "bbbbbbbbbbbb.."
		/*  3 */ "dddddddddddc.."
		/*  4 */ "eeeeeeeeeadc.."
		/*  5 */ ".........adc.."
		/*  6 */ ".........adc.."
		/*  7 */ "mmmmmmm..adc.."
		/*  8 */ "mmmmmmm..adc.."
		/*  9 */ "mmmmmmm..adc.."
		/* 10 */ "mmmmmmm..adc.."
		/* 11 */ "mmmmmmm..adc..",

		// Connectors:
		"-1: 8, 0, 0: 2\n"  /* Type -1, direction Z- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SingleField:
	// The data has been exported from the gallery Desert, area index 17, ID 127, created by Aloe_vera
	{
		// Size:
		10, 2, 7,  // SizeX = 10, SizeY = 2, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		9, 1, 6,  // MaxX, MaxY, MaxZ

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
		/*  1 */ "abbbbbbbba"
		/*  2 */ "abbbbbbbba"
		/*  3 */ "acccccccca"
		/*  4 */ "abbbbbbbba"
		/*  5 */ "abbbbbbbba"
		/*  6 */ "aaaaaaaaaa"

		// Level 1
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
		"-1: 0, 0, 3: 4\n"  /* Type -1, direction X- */,

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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SmallHut:
	// The data has been exported from the gallery Desert, area index 4, ID 68, created by tonibm1999
	{
		// Size:
		5, 5, 6,  // SizeX = 5, SizeY = 5, SizeZ = 6

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		4, 4, 5,  // MaxX, MaxY, MaxZ

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
		/*  5 */ "..b.."

		// Level 1
		/* z\x*   01234 */
		/*  0 */ "accca"
		/*  1 */ "cdedc"
		/*  2 */ "c.f.c"
		/*  3 */ "c...c"
		/*  4 */ "acgca"
		/*  5 */ "....."

		// Level 2
		/* z\x*   01234 */
		/*  0 */ "ac.ca"
		/*  1 */ "c...c"
		/*  2 */ "....."
		/*  3 */ "c...c"
		/*  4 */ "achca"
		/*  5 */ "....."

		// Level 3
		/* z\x*   01234 */
		/*  0 */ "accca"
		/*  1 */ "c...c"
		/*  2 */ "c...c"
		/*  3 */ "c...c"
		/*  4 */ "accca"
		/*  5 */ "....."

		// Level 4
		/* z\x*   01234 */
		/*  0 */ ".aaa."
		/*  1 */ "aaaaa"
		/*  2 */ "aaaaa"
		/*  3 */ "aaaaa"
		/*  4 */ ".aaa."
		/*  5 */ ".....",

		// Connectors:
		"-1: 2, 0, 5: 3\n"  /* Type -1, direction Z+ */,

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
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		"e:118: 3\n"  /* cauldronblock */
		"f: 85: 0\n"  /* fence */
		"g:128: 2\n"  /* sandstonestairs */
		"h:128: 7\n"  /* sandstonestairs */
		"i:128: 4\n"  /* sandstonestairs */
		"j:128: 5\n"  /* sandstonestairs */
		"k:128: 6\n"  /* sandstonestairs */
		"l:128: 3\n"  /* sandstonestairs */
		"m: 19: 0\n"  /* sponge */,

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
		/*  0 */ "ddddddd"
		/*  1 */ "dbbbbbd"
		/*  2 */ "dbcccbd"
		/*  3 */ "dbcccbd"
		/*  4 */ "dbcccbd"
		/*  5 */ "dbbbbbd"
		/*  6 */ "ddddddd"

		// Level 7
		/* z\x*   0123456 */
		/*  0 */ "ddbbbdd"
		/*  1 */ "dbbbbbd"
		/*  2 */ "bbcccbb"
		/*  3 */ "bbcccbb"
		/*  4 */ "bbcccbb"
		/*  5 */ "dbbbbbd"
		/*  6 */ "ddbbbdd"

		// Level 8
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".bbbbb."
		/*  2 */ ".b...b."
		/*  3 */ ".b.e.b."
		/*  4 */ ".b...b."
		/*  5 */ ".bbbbb."
		/*  6 */ "......."

		// Level 9
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".f...f."
		/*  2 */ "......."
		/*  3 */ "...f..."
		/*  4 */ "......."
		/*  5 */ ".f...f."
		/*  6 */ "......."

		// Level 10
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".f...f."
		/*  2 */ "......."
		/*  3 */ "...f..."
		/*  4 */ "......."
		/*  5 */ ".f...f."
		/*  6 */ "......."

		// Level 11
		/* z\x*   0123456 */
		/*  0 */ "ggggggg"
		/*  1 */ "hbhhhbh"
		/*  2 */ ".i...j."
		/*  3 */ ".i.f.j."
		/*  4 */ ".i...j."
		/*  5 */ "kbkkkbk"
		/*  6 */ "lllllll"

		// Level 12
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "ggggggg"
		/*  2 */ "hb...bh"
		/*  3 */ ".b.f.b."
		/*  4 */ "kb...bk"
		/*  5 */ "lllllll"
		/*  6 */ "......."

		// Level 13
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "ggggggg"
		/*  3 */ "bbbbbbb"
		/*  4 */ "lllllll"
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



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

