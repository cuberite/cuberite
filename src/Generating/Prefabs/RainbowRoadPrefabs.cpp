
// RainbowRoadPrefabs.cpp

// Defines the prefabs in the group RainbowRoad

// NOTE: This file has been generated automatically by GalExport!
// Any manual changes will be overwritten by the next automatic export!

#include "Globals.h"
#include "RainbowRoadPrefabs.h"





const cPrefab::sDef g_RainbowRoadPrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// CurveDouble:
	// The data has been exported from the gallery Cube, area index 89, ID 467, created by Aloe_vera
	{
		// Size:
		14, 1, 14,  // SizeX = 14, SizeY = 1, SizeZ = 14

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		13, 2, 13,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:11\n"  /* wool */
		"b: 35: 3\n"  /* wool */
		"c: 35: 5\n"  /* wool */
		"d: 35: 4\n"  /* wool */
		"e: 35: 1\n"  /* wool */
		"f: 35:14\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "aaaaaa........"
		/*  1 */ "bbbbbba......."
		/*  2 */ "cccccbbaaa...."
		/*  3 */ "dddddccbbaa..."
		/*  4 */ "eeeeeddccbaa.."
		/*  5 */ "fffffeddccba.."
		/*  6 */ "ffffffeedcbaa."
		/*  7 */ "eeeefffeddcba."
		/*  8 */ "dddeefffedcbba"
		/*  9 */ "cccddefffedcba"
		/* 10 */ "bbccdeeffedcba"
		/* 11 */ "abbccdeffedcba"
		/* 12 */ ".abbcdeffedcba"
		/* 13 */ "..abcdeffedcba",

		// Connectors:
		"2: 2, 1, 13: 3\n"  /* Type 2, direction Z+ */
		"2: 0, 1, 0: 4\n"  /* Type 2, direction X- */
		"-2: 0, 1, 11: 4\n"  /* Type -2, direction X- */
		"-2: 13, 1, 13: 3\n"  /* Type -2, direction Z+ */,

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
	},  // CurveDouble



	////////////////////////////////////////////////////////////////////////////////
	// CurveDownFromTopSingle:
	// The data has been exported from the gallery Cube, area index 100, ID 478, created by Aloe_vera
	{
		// Size:
		11, 8, 11,  // SizeX = 11, SizeY = 8, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		10, 9, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:14\n"  /* wool */
		"b: 35: 5\n"  /* wool */
		"c: 35: 4\n"  /* wool */
		"d: 35: 1\n"  /* wool */
		"e: 35:11\n"  /* wool */
		"f: 35: 3\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........a"
		/*  9 */ ".......bcda"
		/* 10 */ ".....efbcda"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "........cda"
		/*  7 */ ".......bcda"
		/*  8 */ ".......bcd."
		/*  9 */ ".....ef...."
		/* 10 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ ".........a."
		/*  5 */ ".......cdda"
		/*  6 */ "......bc..."
		/*  7 */ "......b...."
		/*  8 */ ".....ff...."
		/*  9 */ "....ee....."
		/* 10 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "........aa."
		/*  4 */ "......ccd.."
		/*  5 */ ".....bb...."
		/*  6 */ ".....f....."
		/*  7 */ "....ef....."
		/*  8 */ "....e......"
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "......daa.."
		/*  3 */ ".....ccd..."
		/*  4 */ "....bb....."
		/*  5 */ "....f......"
		/*  6 */ "...ef......"
		/*  7 */ "...ee......"
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".....daa..."
		/*  2 */ "...ccd....."
		/*  3 */ "...bc......"
		/*  4 */ "...b......."
		/*  5 */ "..ff......."
		/*  6 */ "..ee......."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "...aaa....."
		/*  1 */ "..ddd......"
		/*  2 */ ".cc........"
		/*  3 */ ".bb........"
		/*  4 */ ".ff........"
		/*  5 */ ".e........."
		/*  6 */ ".ee........"
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaa........"
		/*  1 */ "dd........."
		/*  2 */ "cc........."
		/*  3 */ "bb........."
		/*  4 */ "ff........."
		/*  5 */ "e.........."
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "...........",

		// Connectors:
		"-1: 0, 8, 5: 4\n"  /* Type -1, direction X- */
		"1: 5, 1, 10: 3\n"  /* Type 1, direction Z+ */,

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
	},  // CurveDownFromTopSingle



	////////////////////////////////////////////////////////////////////////////////
	// CurveSingle:
	// The data has been exported from the gallery Cube, area index 84, ID 462, created by Aloe_vera
	{
		// Size:
		11, 1, 11,  // SizeX = 11, SizeY = 1, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		10, 2, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:14\n"  /* wool */
		"b: 35: 1\n"  /* wool */
		"c: 35: 4\n"  /* wool */
		"d: 35: 5\n"  /* wool */
		"e: 35: 3\n"  /* wool */
		"f: 35:11\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaaaaa....."
		/*  1 */ "bbbbbbaa..."
		/*  2 */ "cccccbbaa.."
		/*  3 */ "ddddcccbaa."
		/*  4 */ "eeedddccba."
		/*  5 */ "ffeeeddcbba"
		/*  6 */ ".fffeedccba"
		/*  7 */ "...ffeddcba"
		/*  8 */ "....feedcba"
		/*  9 */ "....ffedcba"
		/* 10 */ ".....fedcba",

		// Connectors:
		"-1: 0, 1, 5: 4\n"  /* Type -1, direction X- */
		"1: 5, 1, 10: 3\n"  /* Type 1, direction Z+ */,

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
	},  // CurveSingle



	////////////////////////////////////////////////////////////////////////////////
	// CurveSingleLeft:
	// The data has been exported from the gallery Cube, area index 97, ID 475, created by Aloe_vera
	{
		// Size:
		11, 1, 11,  // SizeX = 11, SizeY = 1, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		10, 2, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:14\n"  /* wool */
		"b: 35: 1\n"  /* wool */
		"c: 35: 4\n"  /* wool */
		"d: 35: 5\n"  /* wool */
		"e: 35: 3\n"  /* wool */
		"f: 35:11\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ ".....abcdef"
		/*  1 */ "....aabcdef"
		/*  2 */ "....abbcdef"
		/*  3 */ "...aabccdef"
		/*  4 */ ".aaabbcddef"
		/*  5 */ "aabbbccdeef"
		/*  6 */ "bbbcccddef."
		/*  7 */ "ccccdddeff."
		/*  8 */ "dddddeeff.."
		/*  9 */ "eeeeeeff..."
		/* 10 */ "ffffff.....",

		// Connectors:
		"-1: 0, 1, 10: 4\n"  /* Type -1, direction X- */
		"1: 10, 1, 0: 2\n"  /* Type 1, direction Z- */,

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
	},  // CurveSingleLeft



	////////////////////////////////////////////////////////////////////////////////
	// CurveUpDouble:
	// The data has been exported from the gallery Cube, area index 92, ID 470, created by Aloe_vera
	{
		// Size:
		14, 8, 14,  // SizeX = 14, SizeY = 8, SizeZ = 14

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		13, 9, 13,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:11\n"  /* wool */
		"b: 35: 3\n"  /* wool */
		"c: 35: 5\n"  /* wool */
		"d: 35: 4\n"  /* wool */
		"e: 35: 1\n"  /* wool */
		"f: 35:14\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "a............."
		/*  1 */ "b............."
		/*  2 */ "c............."
		/*  3 */ "d............."
		/*  4 */ "e............."
		/*  5 */ "f............."
		/*  6 */ "f............."
		/*  7 */ "e............."
		/*  8 */ "d............."
		/*  9 */ "c............."
		/* 10 */ "b............."
		/* 11 */ "a............."
		/* 12 */ ".............."
		/* 13 */ ".............."

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".aa..........."
		/*  1 */ ".bb..........."
		/*  2 */ ".cc..........."
		/*  3 */ ".dd..........."
		/*  4 */ ".ee..........."
		/*  5 */ ".f............"
		/*  6 */ ".f............"
		/*  7 */ ".e............"
		/*  8 */ ".d............"
		/*  9 */ ".c............"
		/* 10 */ ".b............"
		/* 11 */ ".b............"
		/* 12 */ ".............."
		/* 13 */ ".............."

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "...aaa........"
		/*  1 */ "...bb........."
		/*  2 */ "...cc........."
		/*  3 */ "...dd........."
		/*  4 */ "...ee........."
		/*  5 */ "..ff.........."
		/*  6 */ "..ff.........."
		/*  7 */ "..ee.........."
		/*  8 */ "..de.........."
		/*  9 */ "..c..........."
		/* 10 */ ".b............"
		/* 11 */ ".b............"
		/* 12 */ ".............."
		/* 13 */ ".............."

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".....baa......"
		/*  2 */ ".....bbaaa...."
		/*  3 */ "....dccbba...."
		/*  4 */ "....eddcc....."
		/*  5 */ "....fedd......"
		/*  6 */ "....ffee......"
		/*  7 */ "....ff........"
		/*  8 */ "....e........."
		/*  9 */ "...dd........."
		/* 10 */ "..cc.........."
		/* 11 */ "..b..........."
		/* 12 */ ".a............"
		/* 13 */ ".............."

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ ".............."
		/*  3 */ "..........a..."
		/*  4 */ ".........ba..."
		/*  5 */ "........cc...."
		/*  6 */ ".......edc...."
		/*  7 */ "......fedd...."
		/*  8 */ ".....ff......."
		/*  9 */ "....de........"
		/* 10 */ "...cde........"
		/* 11 */ "..b..........."
		/* 12 */ ".a............"
		/* 13 */ ".............."

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ ".............."
		/*  3 */ ".............."
		/*  4 */ "...........a.."
		/*  5 */ "..........ba.."
		/*  6 */ "..........baa."
		/*  7 */ "..........cba."
		/*  8 */ ".......fedcb.."
		/*  9 */ "......fffed..."
		/* 10 */ ".....eef......"
		/* 11 */ "...ccd........"
		/* 12 */ "..b..........."
		/* 13 */ ".............."

		// Level 6
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ ".............."
		/*  3 */ ".............."
		/*  4 */ ".............."
		/*  5 */ ".............."
		/*  6 */ ".............."
		/*  7 */ ".............."
		/*  8 */ "............ba"
		/*  9 */ "...........cba"
		/* 10 */ "........fedcba"
		/* 11 */ "......effedc.."
		/* 12 */ "..bbcdef......"
		/* 13 */ "..a..........."

		// Level 7
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ ".............."
		/*  1 */ ".............."
		/*  2 */ ".............."
		/*  3 */ ".............."
		/*  4 */ ".............."
		/*  5 */ ".............."
		/*  6 */ ".............."
		/*  7 */ ".............."
		/*  8 */ ".............."
		/*  9 */ ".............."
		/* 10 */ ".............."
		/* 11 */ "............ba"
		/* 12 */ "........fedcba"
		/* 13 */ "..abcdeffedcba",

		// Connectors:
		"-2: 0, 1, 11: 4\n"  /* Type -2, direction X- */
		"2: 0, 1, 0: 4\n"  /* Type 2, direction X- */
		"2: 2, 8, 13: 3\n"  /* Type 2, direction Z+ */
		"-2: 13, 8, 13: 3\n"  /* Type -2, direction Z+ */,

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
	},  // CurveUpDouble



	////////////////////////////////////////////////////////////////////////////////
	// CurveUpSingle:
	// The data has been exported from the gallery Cube, area index 87, ID 465, created by Aloe_vera
	{
		// Size:
		11, 8, 11,  // SizeX = 11, SizeY = 8, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		10, 9, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:14\n"  /* wool */
		"b: 35: 1\n"  /* wool */
		"c: 35: 4\n"  /* wool */
		"d: 35: 5\n"  /* wool */
		"e: 35: 3\n"  /* wool */
		"f: 35:11\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaa........"
		/*  1 */ "bb........."
		/*  2 */ "cc........."
		/*  3 */ "dd........."
		/*  4 */ "ee........."
		/*  5 */ "f.........."
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "...aaa....."
		/*  1 */ "..bbb......"
		/*  2 */ ".cc........"
		/*  3 */ ".dd........"
		/*  4 */ ".ee........"
		/*  5 */ ".f........."
		/*  6 */ ".ff........"
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".....baa..."
		/*  2 */ "...ccb....."
		/*  3 */ "...dc......"
		/*  4 */ "...d......."
		/*  5 */ "..ee......."
		/*  6 */ "..ff......."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "......baa.."
		/*  3 */ ".....ccb..."
		/*  4 */ "....dd....."
		/*  5 */ "....e......"
		/*  6 */ "...fe......"
		/*  7 */ "...ff......"
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "........aa."
		/*  4 */ "......ccb.."
		/*  5 */ ".....dd...."
		/*  6 */ ".....e....."
		/*  7 */ "....fe....."
		/*  8 */ "....f......"
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ ".........a."
		/*  5 */ ".......cbba"
		/*  6 */ "......dc..."
		/*  7 */ "......d...."
		/*  8 */ ".....ee...."
		/*  9 */ "....ff....."
		/* 10 */ "..........."

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "........cba"
		/*  7 */ ".......dcba"
		/*  8 */ ".......dcb."
		/*  9 */ ".....fe...."
		/* 10 */ ".....f....."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........a"
		/*  9 */ ".......dcba"
		/* 10 */ ".....fedcba",

		// Connectors:
		"-1: 0, 1, 5: 4\n"  /* Type -1, direction X- */
		"1: 5, 8, 10: 3\n"  /* Type 1, direction Z+ */,

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
	},  // CurveUpSingle



	////////////////////////////////////////////////////////////////////////////////
	// SlopeDownFromTopSingle:
	// The data has been exported from the gallery Cube, area index 98, ID 476, created by Aloe_vera
	{
		// Size:
		16, 8, 6,  // SizeX = 16, SizeY = 8, SizeZ = 6

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		15, 9, 5,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:14\n"  /* wool */
		"b: 35: 1\n"  /* wool */
		"c: 35: 4\n"  /* wool */
		"d: 35: 5\n"  /* wool */
		"e: 35: 3\n"  /* wool */
		"f: 35:11\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..............aa"
		/*  1 */ "..............bb"
		/*  2 */ "..............cc"
		/*  3 */ "..............dd"
		/*  4 */ "..............ee"
		/*  5 */ "..............ff"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "............aa.."
		/*  1 */ "............bb.."
		/*  2 */ "............cc.."
		/*  3 */ "............dd.."
		/*  4 */ "............ee.."
		/*  5 */ "............ff.."

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..........aa...."
		/*  1 */ "..........bb...."
		/*  2 */ "..........cc...."
		/*  3 */ "..........dd...."
		/*  4 */ "..........ee...."
		/*  5 */ "..........ff...."

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "........aa......"
		/*  1 */ "........bb......"
		/*  2 */ "........cc......"
		/*  3 */ "........dd......"
		/*  4 */ "........ee......"
		/*  5 */ "........ff......"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "......aa........"
		/*  1 */ "......bb........"
		/*  2 */ "......cc........"
		/*  3 */ "......dd........"
		/*  4 */ "......ee........"
		/*  5 */ "......ff........"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "....aa.........."
		/*  1 */ "....bb.........."
		/*  2 */ "....cc.........."
		/*  3 */ "....dd.........."
		/*  4 */ "....ee.........."
		/*  5 */ "....ff.........."

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..aa............"
		/*  1 */ "..bb............"
		/*  2 */ "..cc............"
		/*  3 */ "..dd............"
		/*  4 */ "..ee............"
		/*  5 */ "..ff............"

		// Level 7
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "aa.............."
		/*  1 */ "bb.............."
		/*  2 */ "cc.............."
		/*  3 */ "dd.............."
		/*  4 */ "ee.............."
		/*  5 */ "ff..............",

		// Connectors:
		"-1: 0, 8, 5: 4\n"  /* Type -1, direction X- */
		"1: 15, 1, 5: 5\n"  /* Type 1, direction X+ */,

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
	},  // SlopeDownFromTopSingle



	////////////////////////////////////////////////////////////////////////////////
	// SlopeUpDouble:
	// The data has been exported from the gallery Cube, area index 90, ID 468, created by Aloe_vera
	{
		// Size:
		16, 8, 12,  // SizeX = 16, SizeY = 8, SizeZ = 12

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		15, 9, 11,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:11\n"  /* wool */
		"b: 35: 3\n"  /* wool */
		"c: 35: 5\n"  /* wool */
		"d: 35: 4\n"  /* wool */
		"e: 35: 1\n"  /* wool */
		"f: 35:14\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "aa.............."
		/*  1 */ "bb.............."
		/*  2 */ "cc.............."
		/*  3 */ "dd.............."
		/*  4 */ "ee.............."
		/*  5 */ "ff.............."
		/*  6 */ "ff.............."
		/*  7 */ "ee.............."
		/*  8 */ "dd.............."
		/*  9 */ "cc.............."
		/* 10 */ "bb.............."
		/* 11 */ "aa.............."

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..aa............"
		/*  1 */ "..bb............"
		/*  2 */ "..cc............"
		/*  3 */ "..dd............"
		/*  4 */ "..ee............"
		/*  5 */ "..ff............"
		/*  6 */ "..ff............"
		/*  7 */ "..ee............"
		/*  8 */ "..dd............"
		/*  9 */ "..cc............"
		/* 10 */ "..bb............"
		/* 11 */ "..aa............"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "....aa.........."
		/*  1 */ "....bb.........."
		/*  2 */ "....cc.........."
		/*  3 */ "....dd.........."
		/*  4 */ "....ee.........."
		/*  5 */ "....ff.........."
		/*  6 */ "....ff.........."
		/*  7 */ "....ee.........."
		/*  8 */ "....dd.........."
		/*  9 */ "....cc.........."
		/* 10 */ "....bb.........."
		/* 11 */ "....aa.........."

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "......aa........"
		/*  1 */ "......bb........"
		/*  2 */ "......cc........"
		/*  3 */ "......dd........"
		/*  4 */ "......ee........"
		/*  5 */ "......ff........"
		/*  6 */ "......ff........"
		/*  7 */ "......ee........"
		/*  8 */ "......dd........"
		/*  9 */ "......cc........"
		/* 10 */ "......bb........"
		/* 11 */ "......aa........"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "........aa......"
		/*  1 */ "........bb......"
		/*  2 */ "........cc......"
		/*  3 */ "........dd......"
		/*  4 */ "........ee......"
		/*  5 */ "........ff......"
		/*  6 */ "........ff......"
		/*  7 */ "........ee......"
		/*  8 */ "........dd......"
		/*  9 */ "........cc......"
		/* 10 */ "........bb......"
		/* 11 */ "........aa......"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..........aa...."
		/*  1 */ "..........bb...."
		/*  2 */ "..........cc...."
		/*  3 */ "..........dd...."
		/*  4 */ "..........ee...."
		/*  5 */ "..........ff...."
		/*  6 */ "..........ff...."
		/*  7 */ "..........ee...."
		/*  8 */ "..........dd...."
		/*  9 */ "..........cc...."
		/* 10 */ "..........bb...."
		/* 11 */ "..........aa...."

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "............aa.."
		/*  1 */ "............bb.."
		/*  2 */ "............cc.."
		/*  3 */ "............dd.."
		/*  4 */ "............ee.."
		/*  5 */ "............ff.."
		/*  6 */ "............ff.."
		/*  7 */ "............ee.."
		/*  8 */ "............dd.."
		/*  9 */ "............cc.."
		/* 10 */ "............bb.."
		/* 11 */ "............aa.."

		// Level 7
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..............aa"
		/*  1 */ "..............bb"
		/*  2 */ "..............cc"
		/*  3 */ "..............dd"
		/*  4 */ "..............ee"
		/*  5 */ "..............ff"
		/*  6 */ "..............ff"
		/*  7 */ "..............ee"
		/*  8 */ "..............dd"
		/*  9 */ "..............cc"
		/* 10 */ "..............bb"
		/* 11 */ "..............aa",

		// Connectors:
		"-2: 0, 1, 11: 4\n"  /* Type -2, direction X- */
		"2: 0, 1, 0: 4\n"  /* Type 2, direction X- */
		"-2: 15, 8, 0: 5\n"  /* Type -2, direction X+ */
		"2: 15, 8, 11: 5\n"  /* Type 2, direction X+ */,

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
	},  // SlopeUpDouble



	////////////////////////////////////////////////////////////////////////////////
	// SlopeUpSingle:
	// The data has been exported from the gallery Cube, area index 85, ID 463, created by Aloe_vera
	{
		// Size:
		16, 8, 6,  // SizeX = 16, SizeY = 8, SizeZ = 6

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		15, 9, 5,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:14\n"  /* wool */
		"b: 35: 1\n"  /* wool */
		"c: 35: 4\n"  /* wool */
		"d: 35: 5\n"  /* wool */
		"e: 35: 3\n"  /* wool */
		"f: 35:11\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "aa.............."
		/*  1 */ "bb.............."
		/*  2 */ "cc.............."
		/*  3 */ "dd.............."
		/*  4 */ "ee.............."
		/*  5 */ "ff.............."

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..aa............"
		/*  1 */ "..bb............"
		/*  2 */ "..cc............"
		/*  3 */ "..dd............"
		/*  4 */ "..ee............"
		/*  5 */ "..ff............"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "....aa.........."
		/*  1 */ "....bb.........."
		/*  2 */ "....cc.........."
		/*  3 */ "....dd.........."
		/*  4 */ "....ee.........."
		/*  5 */ "....ff.........."

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "......aa........"
		/*  1 */ "......bb........"
		/*  2 */ "......cc........"
		/*  3 */ "......dd........"
		/*  4 */ "......ee........"
		/*  5 */ "......ff........"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "........aa......"
		/*  1 */ "........bb......"
		/*  2 */ "........cc......"
		/*  3 */ "........dd......"
		/*  4 */ "........ee......"
		/*  5 */ "........ff......"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..........aa...."
		/*  1 */ "..........bb...."
		/*  2 */ "..........cc...."
		/*  3 */ "..........dd...."
		/*  4 */ "..........ee...."
		/*  5 */ "..........ff...."

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "............aa.."
		/*  1 */ "............bb.."
		/*  2 */ "............cc.."
		/*  3 */ "............dd.."
		/*  4 */ "............ee.."
		/*  5 */ "............ff.."

		// Level 7
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..............aa"
		/*  1 */ "..............bb"
		/*  2 */ "..............cc"
		/*  3 */ "..............dd"
		/*  4 */ "..............ee"
		/*  5 */ "..............ff",

		// Connectors:
		"-1: 0, 1, 5: 4\n"  /* Type -1, direction X- */
		"1: 15, 8, 5: 5\n"  /* Type 1, direction X+ */,

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
		-1000,

		// MoveToGround:
		false,
	},  // SlopeUpSingle



	////////////////////////////////////////////////////////////////////////////////
	// SplitTee:
	// The data has been exported from the gallery Cube, area index 93, ID 471, created by Aloe_vera
	{
		// Size:
		16, 1, 14,  // SizeX = 16, SizeY = 1, SizeZ = 14

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		15, 2, 13,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:11\n"  /* wool */
		"b: 35: 3\n"  /* wool */
		"c: 35: 5\n"  /* wool */
		"d: 35: 4\n"  /* wool */
		"e: 35: 1\n"  /* wool */
		"f: 35:14\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "aaaaaa.........."
		/*  1 */ "bbbbbbaaa......."
		/*  2 */ "ccccccbbbaaa...."
		/*  3 */ "ddddddcccbbbaaaa"
		/*  4 */ "eeeeeedddcccbbbb"
		/*  5 */ "ffffffeeedddcccc"
		/*  6 */ "fffffffffeeedddd"
		/*  7 */ "eeeeff...fffeeee"
		/*  8 */ "dddeeff.....ffff"
		/*  9 */ "cccddeff........"
		/* 10 */ "bbbccdeef......."
		/* 11 */ "aaabbcddef......"
		/* 12 */ "...aabcddef....."
		/* 13 */ ".....abcdef.....",

		// Connectors:
		"-2: 0, 1, 11: 4\n"  /* Type -2, direction X- */
		"2: 0, 1, 0: 4\n"  /* Type 2, direction X- */
		"-1: 15, 1, 3: 5\n"  /* Type -1, direction X+ */
		"1: 5, 1, 13: 3\n"  /* Type 1, direction Z+ */,

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
	},  // SplitTee



	////////////////////////////////////////////////////////////////////////////////
	// StraightDouble:
	// The data has been exported from the gallery Cube, area index 88, ID 466, created by Aloe_vera
	{
		// Size:
		16, 1, 12,  // SizeX = 16, SizeY = 1, SizeZ = 12

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		15, 2, 11,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:11\n"  /* wool */
		"b: 35: 3\n"  /* wool */
		"c: 35: 5\n"  /* wool */
		"d: 35: 4\n"  /* wool */
		"e: 35: 1\n"  /* wool */
		"f: 35:14\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "aaaaaaaaaaaaaaaa"
		/*  1 */ "bbbbbbbbbbbbbbbb"
		/*  2 */ "cccccccccccccccc"
		/*  3 */ "dddddddddddddddd"
		/*  4 */ "eeeeeeeeeeeeeeee"
		/*  5 */ "ffffffffffffffff"
		/*  6 */ "ffffffffffffffff"
		/*  7 */ "eeeeeeeeeeeeeeee"
		/*  8 */ "dddddddddddddddd"
		/*  9 */ "cccccccccccccccc"
		/* 10 */ "bbbbbbbbbbbbbbbb"
		/* 11 */ "aaaaaaaaaaaaaaaa",

		// Connectors:
		"-2: 0, 1, 11: 4\n"  /* Type -2, direction X- */
		"2: 0, 1, 0: 4\n"  /* Type 2, direction X- */
		"-2: 15, 1, 0: 5\n"  /* Type -2, direction X+ */
		"2: 15, 1, 11: 5\n"  /* Type 2, direction X+ */,

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
	},  // StraightDouble
};  // g_RainbowRoadPrefabs






const cPrefab::sDef g_RainbowRoadStartingPrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// StraightSingle:
	// The data has been exported from the gallery Cube, area index 83, ID 461, created by Aloe_vera
	{
		// Size:
		16, 1, 6,  // SizeX = 16, SizeY = 1, SizeZ = 6

		// Hitbox (relative to bounding box):
		0, -2, 0,  // MinX, MinY, MinZ
		15, 2, 5,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 35:14\n"  /* wool */
		"b: 35: 1\n"  /* wool */
		"c: 35: 4\n"  /* wool */
		"d: 35: 5\n"  /* wool */
		"e: 35: 3\n"  /* wool */
		"f: 35:11\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "aaaaaaaaaaaaaaaa"
		/*  1 */ "bbbbbbbbbbbbbbbb"
		/*  2 */ "cccccccccccccccc"
		/*  3 */ "dddddddddddddddd"
		/*  4 */ "eeeeeeeeeeeeeeee"
		/*  5 */ "ffffffffffffffff",

		// Connectors:
		"-1: 0, 1, 5: 4\n"  /* Type -1, direction X- */
		"1: 15, 1, 5: 5\n"  /* Type 1, direction X+ */,

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
		500,

		// MoveToGround:
		false,
	},  // StraightSingle
};





// The prefab counts:

const size_t g_RainbowRoadPrefabsCount = ARRAYCOUNT(g_RainbowRoadPrefabs);

const size_t g_RainbowRoadStartingPrefabsCount = ARRAYCOUNT(g_RainbowRoadStartingPrefabs);

