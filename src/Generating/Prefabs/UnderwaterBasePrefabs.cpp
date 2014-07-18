
// UnderwaterBasePrefabs.cpp

// Defines the prefabs in the group UnderwaterBase

// NOTE: This file has been generated automatically by GalExport!
// Any manual changes will be overwritten by the next automatic export!

#include "Globals.h"
#include "UnderwaterBasePrefabs.h"





const cPrefab::sDef g_UnderwaterBasePrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// BrokenRoom:
	// The data has been exported from the gallery Water, area index 49, ID 680, created by STR_Warrior
	{
		// Size:
		14, 7, 12,  // SizeX = 14, SizeY = 7, SizeZ = 12

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		13, 6, 11,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 0\n"  /* wood */
		"b: 20: 0\n"  /* glass */
		"c:  5: 5\n"  /* wood */
		"d:  8: 0\n"  /* water */
		"e: 64: 4\n"  /* wooddoorblock */
		"f: 64:12\n"  /* wooddoorblock */
		"g: 64:13\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmaaaammmmmm"
		/*  2 */ "mmmaabbaammmmm"
		/*  3 */ "mmaabbbbaammmm"
		/*  4 */ "maabbbbbbaammm"
		/*  5 */ "mabbbbbbbbaaac"
		/*  6 */ "mabbbbbbbbaaac"
		/*  7 */ "maabbbbbbaammm"
		/*  8 */ "mmaabbbbaammmm"
		/*  9 */ "mmmaabbaammmmm"
		/* 10 */ "mmmmaaaammmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmaammmmmmm"
		/*  1 */ "mmmaaddaammmmm"
		/*  2 */ "mmaaddddaammmm"
		/*  3 */ "maaddddddaammm"
		/*  4 */ "maddddddddaaac"
		/*  5 */ "adddddddddddde"
		/*  6 */ "adddddddddddde"
		/*  7 */ "maddddddddaaac"
		/*  8 */ "maaddddddaammm"
		/*  9 */ "mmaaddddaammmm"
		/* 10 */ "mmmaaddaammmmm"
		/* 11 */ "mmmmmaammmmmmm"

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmbbmmmmmmm"
		/*  1 */ "mmmbb..bbmmmmm"
		/*  2 */ "mmbb....bbmmmm"
		/*  3 */ "mbb......bbmmm"
		/*  4 */ "mb........bbac"
		/*  5 */ "b............f"
		/*  6 */ "b............g"
		/*  7 */ "mb........bbac"
		/*  8 */ "mbb......bbmmm"
		/*  9 */ "mmbb....bbmmmm"
		/* 10 */ "mmmbb..bbmmmmm"
		/* 11 */ "mmmmmbbmmmmmmm"

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmbbmmmmmmm"
		/*  1 */ "mmmbb..bbmmmmm"
		/*  2 */ "mmbb....bbmmmm"
		/*  3 */ "mbb......bbmmm"
		/*  4 */ "mb........bbac"
		/*  5 */ "b..........bac"
		/*  6 */ "b..........bac"
		/*  7 */ "mb........bbac"
		/*  8 */ "mbb......bbmmm"
		/*  9 */ "mmbb....bbmmmm"
		/* 10 */ "mmmbb..bbmmmmm"
		/* 11 */ "mmmmmbbmmmmmmm"

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmm.bmmmmmmm"
		/*  2 */ "mmmmb..bmmmmmm"
		/*  3 */ "mmmb....bmmmmm"
		/*  4 */ "mmb......bmmmm"
		/*  5 */ "m.........bmmm"
		/*  6 */ "mb........bmmm"
		/*  7 */ "mmb......bmmmm"
		/*  8 */ "mmm.....bmmmmm"
		/*  9 */ "mmmmb..bmmmmmm"
		/* 10 */ "mmmmmbbmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmbbmmmmmmm"
		/*  3 */ "mmmm....mmmmmm"
		/*  4 */ "mmmb....bmmmmm"
		/*  5 */ "mmb......bmmmm"
		/*  6 */ "mmb......bmmmm"
		/*  7 */ "mmmb.....mmmmm"
		/*  8 */ "mmmmb..bmmmmmm"
		/*  9 */ "mmmmmbbmmmmmmm"
		/* 10 */ "mmmmmmmmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"

		// Level 6
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmm"
		/*  3 */ "mmmmmbbmmmmmmm"
		/*  4 */ "mmmmbbbbmmmmmm"
		/*  5 */ "mmmbbbbbbmmmmm"
		/*  6 */ "mmmbb.bbbmmmmm"
		/*  7 */ "mmmmbbbbmmmmmm"
		/*  8 */ "mmmmmbbmmmmmmm"
		/*  9 */ "mmmmmmmmmmmmmm"
		/* 10 */ "mmmmmmmmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm",

		// Connectors:
		"1: 13, 1, 6: 5\n"  /* Type 1, direction X+ */
		"-1: 13, 1, 5: 5\n"  /* Type -1, direction X+ */,

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
	},  // BrokenRoom



	////////////////////////////////////////////////////////////////////////////////
	// Corridor16:
	// The data has been exported from the gallery Water, area index 25, ID 566, created by xoft
	{
		// Size:
		16, 4, 4,  // SizeX = 16, SizeY = 4, SizeZ = 4

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 3, 3,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 64: 2\n"  /* wooddoorblock */
		"d: 64: 0\n"  /* wooddoorblock */
		"e: 20: 0\n"  /* glass */
		"f: 64: 9\n"  /* wooddoorblock */
		"g: 76: 3\n"  /* redstonetorchon */
		"h: 64: 8\n"  /* wooddoorblock */
		"i: 76: 4\n"  /* redstonetorchon */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "abbbbbbbbbbbbbba"
		/*  2 */ "abbbbbbbbbbbbbba"
		/*  3 */ "mmmmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "abbbbbbbbbbbbbba"
		/*  1 */ "c..............d"
		/*  2 */ "c..............d"
		/*  3 */ "abbbbbbbbbbbbbba"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "abeebbbeebbbeeba"
		/*  1 */ "f...g......g...h"
		/*  2 */ "h...i......i...f"
		/*  3 */ "abeebbbeebbbeeba"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "abbbbbbbbbbbbbba"
		/*  1 */ "abbbbbbbbbbbbbba"
		/*  2 */ "abbbbbbbbbbbbbba"
		/*  3 */ "abbbbbbbbbbbbbba",

		// Connectors:
		"1: 0, 1, 1: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 2: 4\n"  /* Type -1, direction X- */
		"1: 15, 1, 2: 5\n"  /* Type 1, direction X+ */
		"-1: 15, 1, 1: 5\n"  /* Type -1, direction X+ */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		true,

		// DefaultWeight:
		500,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // Corridor16



	////////////////////////////////////////////////////////////////////////////////
	// CorridorCorner:
	// The data has been exported from the gallery Water, area index 26, ID 569, created by xoft
	{
		// Size:
		10, 4, 10,  // SizeX = 10, SizeY = 4, SizeZ = 10

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		9, 3, 9,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 64: 5\n"  /* wooddoorblock */
		"d: 64: 2\n"  /* wooddoorblock */
		"e: 64: 4\n"  /* wooddoorblock */
		"f: 64: 1\n"  /* wooddoorblock */
		"g: 20: 0\n"  /* glass */
		"h: 64:12\n"  /* wooddoorblock */
		"i: 76: 3\n"  /* redstonetorchon */
		"j: 64: 8\n"  /* wooddoorblock */
		"k: 76: 4\n"  /* redstonetorchon */
		"l: 76: 2\n"  /* redstonetorchon */
		"m: 19: 0\n"  /* sponge */
		"n: 76: 1\n"  /* redstonetorchon */,

		// Block data:
		// Level 0
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "mmmmmmmmmm"
		/*  1 */ "abbbbbmmmm"
		/*  2 */ "abbbbbbbmm"
		/*  3 */ "mmmbbbbbmm"
		/*  4 */ "mmmmmbbbbm"
		/*  5 */ "mmmmmmbbbm"
		/*  6 */ "mmmmmmbbbm"
		/*  7 */ "mmmmmmmbbm"
		/*  8 */ "mmmmmmmbbm"
		/*  9 */ "mmmmmmmaam"

		// Level 1
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "abbbbbmmmm"
		/*  1 */ "c.....bbmm"
		/*  2 */ "d.......bm"
		/*  3 */ "abb.....bm"
		/*  4 */ "mmmbb....b"
		/*  5 */ "mmmmmb...b"
		/*  6 */ "mmmmmb...b"
		/*  7 */ "mmmmmmb..b"
		/*  8 */ "mmmmmmb..b"
		/*  9 */ "mmmmmmaefa"

		// Level 2
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "abggbbmmmm"
		/*  1 */ "h...i.bbmm"
		/*  2 */ "j.......bm"
		/*  3 */ "abbk....bm"
		/*  4 */ "mmmbb....b"
		/*  5 */ "mmmmmb..lb"
		/*  6 */ "mmmmmbn..g"
		/*  7 */ "mmmmmmb..g"
		/*  8 */ "mmmmmmb..b"
		/*  9 */ "mmmmmmahja"

		// Level 3
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "abbbbbmmmm"
		/*  1 */ "abbbbbbbmm"
		/*  2 */ "abbbbbbbbm"
		/*  3 */ "abbbbbbbbm"
		/*  4 */ "mmmbbbbbbb"
		/*  5 */ "mmmmmbbbbb"
		/*  6 */ "mmmmmbbbbb"
		/*  7 */ "mmmmmmbbbb"
		/*  8 */ "mmmmmmbbbb"
		/*  9 */ "mmmmmmaaaa",

		// Connectors:
		"1: 7, 1, 9: 3\n"  /* Type 1, direction Z+ */
		"-1: 8, 1, 9: 3\n"  /* Type -1, direction Z+ */
		"1: 0, 1, 1: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 2: 4\n"  /* Type -1, direction X- */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		true,

		// DefaultWeight:
		200,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // CorridorCorner



	////////////////////////////////////////////////////////////////////////////////
	// CorridorCrossing:
	// The data has been exported from the gallery Water, area index 31, ID 581, created by LO1ZB
	{
		// Size:
		16, 4, 16,  // SizeX = 16, SizeY = 4, SizeZ = 16

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 3, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 64: 3\n"  /* wooddoorblock */
		"d: 64: 6\n"  /* wooddoorblock */
		"e: 64: 5\n"  /* wooddoorblock */
		"f: 64: 0\n"  /* wooddoorblock */
		"g: 64: 2\n"  /* wooddoorblock */
		"h: 64: 1\n"  /* wooddoorblock */
		"i: 64: 8\n"  /* wooddoorblock */
		"j: 64:12\n"  /* wooddoorblock */
		"k: 20: 0\n"  /* glass */
		"l: 76: 1\n"  /* redstonetorchon */
		"m: 19: 0\n"  /* sponge */
		"n: 76: 2\n"  /* redstonetorchon */
		"o: 76: 3\n"  /* redstonetorchon */
		"p: 76: 4\n"  /* redstonetorchon */
		"q: 64: 9\n"  /* wooddoorblock */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmaammmmmmm"
		/*  1 */ "mmmmmmmbbmmmmmmm"
		/*  2 */ "mmmmmmmbbmmmmmmm"
		/*  3 */ "mmmmmmmbbmmmmmmm"
		/*  4 */ "mmmmmmmbbmmmmmmm"
		/*  5 */ "mmmmmmmbbmmmmmmm"
		/*  6 */ "mmmmmmmbbmmmmmmm"
		/*  7 */ "abbbbbbbbbbbbbba"
		/*  8 */ "abbbbbbbbbbbbbba"
		/*  9 */ "mmmmmmmbbmmmmmmm"
		/* 10 */ "mmmmmmmbbmmmmmmm"
		/* 11 */ "mmmmmmmbbmmmmmmm"
		/* 12 */ "mmmmmmmbbmmmmmmm"
		/* 13 */ "mmmmmmmbbmmmmmmm"
		/* 14 */ "mmmmmmmbbmmmmmmm"
		/* 15 */ "mmmmmmmaammmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmacdammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmb..bmmmmmm"
		/*  3 */ "mmmmmmb..bmmmmmm"
		/*  4 */ "mmmmmmb..bmmmmmm"
		/*  5 */ "mmmmmmb..bmmmmmm"
		/*  6 */ "abbbbbb..bbbbbba"
		/*  7 */ "e..............f"
		/*  8 */ "g..............f"
		/*  9 */ "abbbbbb..bbbbbba"
		/* 10 */ "mmmmmmb..bmmmmmm"
		/* 11 */ "mmmmmmb..bmmmmmm"
		/* 12 */ "mmmmmmb..bmmmmmm"
		/* 13 */ "mmmmmmb..bmmmmmm"
		/* 14 */ "mmmmmmb..bmmmmmm"
		/* 15 */ "mmmmmmahhammmmmm"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaijammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmk..kmmmmmm"
		/*  3 */ "mmmmmmk..kmmmmmm"
		/*  4 */ "mmmmmmblnbmmmmmm"
		/*  5 */ "mmmmmmb..bmmmmmm"
		/*  6 */ "abkkbbb..bbbkkba"
		/*  7 */ "j...o......o...i"
		/*  8 */ "i...p......p...q"
		/*  9 */ "abkkbbb..bbbkkba"
		/* 10 */ "mmmmmmb..bmmmmmm"
		/* 11 */ "mmmmmmblnbmmmmmm"
		/* 12 */ "mmmmmmk..kmmmmmm"
		/* 13 */ "mmmmmmk..kmmmmmm"
		/* 14 */ "mmmmmmb..bmmmmmm"
		/* 15 */ "mmmmmmaqiammmmmm"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaaaammmmmm"
		/*  1 */ "mmmmmmbbbbmmmmmm"
		/*  2 */ "mmmmmmbbbbmmmmmm"
		/*  3 */ "mmmmmmbbbbmmmmmm"
		/*  4 */ "mmmmmmbbbbmmmmmm"
		/*  5 */ "mmmmmmbbbbmmmmmm"
		/*  6 */ "abbbbbbbbbbbbbba"
		/*  7 */ "abbbbbbbbbbbbbba"
		/*  8 */ "abbbbbbbbbbbbbba"
		/*  9 */ "abbbbbbbbbbbbbba"
		/* 10 */ "mmmmmmbbbbmmmmmm"
		/* 11 */ "mmmmmmbbbbmmmmmm"
		/* 12 */ "mmmmmmbbbbmmmmmm"
		/* 13 */ "mmmmmmbbbbmmmmmm"
		/* 14 */ "mmmmmmbbbbmmmmmm"
		/* 15 */ "mmmmmmaaaammmmmm",

		// Connectors:
		"1: 0, 1, 7: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 8: 4\n"  /* Type -1, direction X- */
		"1: 7, 1, 15: 3\n"  /* Type 1, direction Z+ */
		"-1: 8, 1, 15: 3\n"  /* Type -1, direction Z+ */
		"1: 8, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 7, 1, 0: 2\n"  /* Type -1, direction Z- */
		"1: 15, 1, 8: 5\n"  /* Type 1, direction X+ */
		"-1: 15, 1, 7: 5\n"  /* Type -1, direction X+ */,

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
	},  // CorridorCrossing



	////////////////////////////////////////////////////////////////////////////////
	// CorridorStairs:
	// The data has been exported from the gallery Water, area index 32, ID 582, created by LO1ZB
	{
		// Size:
		16, 9, 4,  // SizeX = 16, SizeY = 9, SizeZ = 4

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 8, 3,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 64: 2\n"  /* wooddoorblock */
		"d: 53: 0\n"  /* woodstairs */
		"e: 20: 0\n"  /* glass */
		"f: 64: 9\n"  /* wooddoorblock */
		"g: 76: 3\n"  /* redstonetorchon */
		"h: 64: 8\n"  /* wooddoorblock */
		"i: 76: 4\n"  /* redstonetorchon */
		"j: 64: 0\n"  /* wooddoorblock */
		"k: 64: 7\n"  /* wooddoorblock */
		"l: 64:12\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "abbbbbbbmmmmmmmm"
		/*  2 */ "abbbbbbbmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "abbbbbbbmmmmmmmm"
		/*  1 */ "c.....dbbmmmmmmm"
		/*  2 */ "c.....dbbmmmmmmm"
		/*  3 */ "abbbbbbbmmmmmmmm"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "abeebbbbbmmmmmmm"
		/*  1 */ "f...g..dbbmmmmmm"
		/*  2 */ "h...i..dbbmmmmmm"
		/*  3 */ "abeebbbbbmmmmmmm"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "abbbbbbbbbmmmmmm"
		/*  1 */ "abbbb...dbbmmmmm"
		/*  2 */ "abbbb...dbbmmmmm"
		/*  3 */ "abbbbbbbbbmmmmmm"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmbbbbbmmmmmm"
		/*  1 */ "mmmmmb...dbbmmmm"
		/*  2 */ "mmmmmb...dbbmmmm"
		/*  3 */ "mmmmmbbbbbmmmmmm"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmbbbbbmmmmm"
		/*  1 */ "mmmmmmb...dbbbba"
		/*  2 */ "mmmmmmb...dbbbba"
		/*  3 */ "mmmmmmbbbbbmmmmm"

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmbbbbbbbba"
		/*  1 */ "mmmmmmmb.......j"
		/*  2 */ "mmmmmmmb.......k"
		/*  3 */ "mmmmmmmbbbbbbbba"

		// Level 7
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmbbbeebba"
		/*  1 */ "mmmmmmmmb.g....h"
		/*  2 */ "mmmmmmmmb.i....l"
		/*  3 */ "mmmmmmmmbbbeebba"

		// Level 8
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmbbbbbba"
		/*  1 */ "mmmmmmmmmbbbbbba"
		/*  2 */ "mmmmmmmmmbbbbbba"
		/*  3 */ "mmmmmmmmmbbbbbba",

		// Connectors:
		"1: 0, 1, 1: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 2: 4\n"  /* Type -1, direction X- */
		"1: 15, 6, 2: 5\n"  /* Type 1, direction X+ */
		"-1: 15, 6, 1: 5\n"  /* Type -1, direction X+ */,

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
	},  // CorridorStairs



	////////////////////////////////////////////////////////////////////////////////
	// CorridorTee:
	// The data has been exported from the gallery Water, area index 29, ID 576, created by LO1ZB
	{
		// Size:
		16, 4, 10,  // SizeX = 16, SizeY = 4, SizeZ = 10

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 3, 9,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 64: 3\n"  /* wooddoorblock */
		"d: 64: 6\n"  /* wooddoorblock */
		"e: 64: 5\n"  /* wooddoorblock */
		"f: 64: 0\n"  /* wooddoorblock */
		"g: 64: 2\n"  /* wooddoorblock */
		"h: 64: 8\n"  /* wooddoorblock */
		"i: 64:12\n"  /* wooddoorblock */
		"j: 20: 0\n"  /* glass */
		"k: 76: 1\n"  /* redstonetorchon */
		"l: 76: 2\n"  /* redstonetorchon */
		"m: 19: 0\n"  /* sponge */
		"n: 76: 3\n"  /* redstonetorchon */
		"o: 76: 4\n"  /* redstonetorchon */
		"p: 64: 9\n"  /* wooddoorblock */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmaammmmmmm"
		/*  1 */ "mmmmmmmbbmmmmmmm"
		/*  2 */ "mmmmmmmbbmmmmmmm"
		/*  3 */ "mmmmmmmbbmmmmmmm"
		/*  4 */ "mmmmmmmbbmmmmmmm"
		/*  5 */ "mmmmmmmbbmmmmmmm"
		/*  6 */ "mmmmmmmbbmmmmmmm"
		/*  7 */ "abbbbbbbbbbbbbba"
		/*  8 */ "abbbbbbbbbbbbbba"
		/*  9 */ "mmmmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmacdammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmb..bmmmmmm"
		/*  3 */ "mmmmmmb..bmmmmmm"
		/*  4 */ "mmmmmmb..bmmmmmm"
		/*  5 */ "mmmmmmb..bmmmmmm"
		/*  6 */ "abbbbbb..bbbbbba"
		/*  7 */ "e..............f"
		/*  8 */ "g..............f"
		/*  9 */ "abbbbbbbbbbbbbba"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmahiammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmj..jmmmmmm"
		/*  3 */ "mmmmmmj..jmmmmmm"
		/*  4 */ "mmmmmmbklbmmmmmm"
		/*  5 */ "mmmmmmb..bmmmmmm"
		/*  6 */ "abjjbbb..bbbjjba"
		/*  7 */ "i...n......n...h"
		/*  8 */ "h...o......o...p"
		/*  9 */ "abjjbbbjjbbbjjba"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaaaammmmmm"
		/*  1 */ "mmmmmmbbbbmmmmmm"
		/*  2 */ "mmmmmmbbbbmmmmmm"
		/*  3 */ "mmmmmmbbbbmmmmmm"
		/*  4 */ "mmmmmmbbbbmmmmmm"
		/*  5 */ "mmmmmmbbbbmmmmmm"
		/*  6 */ "abbbbbbbbbbbbbba"
		/*  7 */ "abbbbbbbbbbbbbba"
		/*  8 */ "abbbbbbbbbbbbbba"
		/*  9 */ "abbbbbbbbbbbbbba",

		// Connectors:
		"1: 0, 1, 7: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 8: 4\n"  /* Type -1, direction X- */
		"1: 8, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 7, 1, 0: 2\n"  /* Type -1, direction Z- */
		"1: 15, 1, 8: 5\n"  /* Type 1, direction X+ */
		"-1: 15, 1, 7: 5\n"  /* Type -1, direction X+ */,

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
	},  // CorridorTee



	////////////////////////////////////////////////////////////////////////////////
	// ViewingCorner:
	// The data has been exported from the gallery Water, area index 40, ID 613, created by LO1ZB
	{
		// Size:
		14, 7, 14,  // SizeX = 14, SizeY = 7, SizeZ = 14

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		13, 6, 13,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 0\n"  /* wood */
		"b: 20: 0\n"  /* glass */
		"c:  5: 5\n"  /* wood */
		"d: 76: 3\n"  /* redstonetorchon */
		"e: 76: 1\n"  /* redstonetorchon */
		"f: 64: 0\n"  /* wooddoorblock */
		"g: 76: 4\n"  /* redstonetorchon */
		"h: 76: 2\n"  /* redstonetorchon */
		"i: 64: 1\n"  /* wooddoorblock */
		"j: 64: 8\n"  /* wooddoorblock */
		"k: 64: 9\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmaaaammmmmm"
		/*  2 */ "mmmaabbaammmmm"
		/*  3 */ "mmaabbbbaammmm"
		/*  4 */ "maabbbbbbaammm"
		/*  5 */ "mabbbbbbbbaaac"
		/*  6 */ "mabbbbbbbbaaac"
		/*  7 */ "maabbbbbbaammm"
		/*  8 */ "mmaabbbbaammmm"
		/*  9 */ "mmmaabbaammmmm"
		/* 10 */ "mmmmaaaammmmmm"
		/* 11 */ "mmmmmaammmmmmm"
		/* 12 */ "mmmmmaammmmmmm"
		/* 13 */ "mmmmmccmmmmmmm"

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmaammmmmmm"
		/*  1 */ "mmmaaddaammmmm"
		/*  2 */ "mmaa....aammmm"
		/*  3 */ "maa......aammm"
		/*  4 */ "ma........aaac"
		/*  5 */ "ae........d..f"
		/*  6 */ "ae........g..f"
		/*  7 */ "ma........aaac"
		/*  8 */ "maa......aammm"
		/*  9 */ "mmaa....aammmm"
		/* 10 */ "mmmaaehaammmmm"
		/* 11 */ "mmmma..ammmmmm"
		/* 12 */ "mmmma..ammmmmm"
		/* 13 */ "mmmmciicmmmmmm"

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmbbmmmmmmm"
		/*  1 */ "mmmbb..bbmmmmm"
		/*  2 */ "mmbb....bbmmmm"
		/*  3 */ "mbb......bbmmm"
		/*  4 */ "mb........bbac"
		/*  5 */ "b............j"
		/*  6 */ "b............k"
		/*  7 */ "mb........bbac"
		/*  8 */ "mbb......bbmmm"
		/*  9 */ "mmbb....bbmmmm"
		/* 10 */ "mmmbb..bbmmmmm"
		/* 11 */ "mmmmb..bmmmmmm"
		/* 12 */ "mmmma..ammmmmm"
		/* 13 */ "mmmmckjcmmmmmm"

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmbbmmmmmmm"
		/*  1 */ "mmmbb..bbmmmmm"
		/*  2 */ "mmbb....bbmmmm"
		/*  3 */ "mbb......bbmmm"
		/*  4 */ "mb........bbac"
		/*  5 */ "b..........bac"
		/*  6 */ "b..........bac"
		/*  7 */ "mb........bbac"
		/*  8 */ "mbb......bbmmm"
		/*  9 */ "mmbb....bbmmmm"
		/* 10 */ "mmmbb..bbmmmmm"
		/* 11 */ "mmmmbbbbmmmmmm"
		/* 12 */ "mmmmaaaammmmmm"
		/* 13 */ "mmmmccccmmmmmm"

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmbbmmmmmmm"
		/*  2 */ "mmmmb..bmmmmmm"
		/*  3 */ "mmmb....bmmmmm"
		/*  4 */ "mmb......bmmmm"
		/*  5 */ "mb........bmmm"
		/*  6 */ "mb........bmmm"
		/*  7 */ "mmb......bmmmm"
		/*  8 */ "mmmb....bmmmmm"
		/*  9 */ "mmmmb..bmmmmmm"
		/* 10 */ "mmmmmbbmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmm"

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmbbmmmmmmm"
		/*  3 */ "mmmmb..bmmmmmm"
		/*  4 */ "mmmb....bmmmmm"
		/*  5 */ "mmb......bmmmm"
		/*  6 */ "mmb......bmmmm"
		/*  7 */ "mmmb....bmmmmm"
		/*  8 */ "mmmmb..bmmmmmm"
		/*  9 */ "mmmmmbbmmmmmmm"
		/* 10 */ "mmmmmmmmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmm"

		// Level 6
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmm"
		/*  3 */ "mmmmmbbmmmmmmm"
		/*  4 */ "mmmmbbbbmmmmmm"
		/*  5 */ "mmmbbbbbbmmmmm"
		/*  6 */ "mmmbbbbbbmmmmm"
		/*  7 */ "mmmmbbbbmmmmmm"
		/*  8 */ "mmmmmbbmmmmmmm"
		/*  9 */ "mmmmmmmmmmmmmm"
		/* 10 */ "mmmmmmmmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmm",

		// Connectors:
		"1: 13, 1, 6: 5\n"  /* Type 1, direction X+ */
		"-1: 13, 1, 5: 5\n"  /* Type -1, direction X+ */
		"1: 5, 1, 13: 3\n"  /* Type 1, direction Z+ */
		"-1: 6, 1, 13: 3\n"  /* Type -1, direction Z+ */,

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
	},  // ViewingCorner



	////////////////////////////////////////////////////////////////////////////////
	// ViewingCorridor:
	// The data has been exported from the gallery Water, area index 27, ID 571, created by LO1ZB
	{
		// Size:
		16, 5, 6,  // SizeX = 16, SizeY = 5, SizeZ = 6

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 4, 5,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 0\n"  /* wood */
		"b:  5: 5\n"  /* wood */
		"c: 20: 0\n"  /* glass */
		"d: 64: 5\n"  /* wooddoorblock */
		"e: 64: 0\n"  /* wooddoorblock */
		"f: 64: 2\n"  /* wooddoorblock */
		"g: 76: 3\n"  /* redstonetorchon */
		"h: 64:12\n"  /* wooddoorblock */
		"i: 64: 8\n"  /* wooddoorblock */
		"j: 64: 9\n"  /* wooddoorblock */
		"k: 76: 4\n"  /* redstonetorchon */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmaaaaaaaaaaaamm"
		/*  1 */ "mmaaaaaaaaaaaamm"
		/*  2 */ "baaccccccccccaab"
		/*  3 */ "baaccccccccccaab"
		/*  4 */ "mmaaaaaaaaaaaamm"
		/*  5 */ "mmmaaaaaaaaaammm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmaccccccccccamm"
		/*  1 */ "ba............ab"
		/*  2 */ "d..............e"
		/*  3 */ "f..............e"
		/*  4 */ "ba............ab"
		/*  5 */ "mmaccccccccccamm"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmaccccccccccamm"
		/*  1 */ "bag..........gab"
		/*  2 */ "h..............i"
		/*  3 */ "i..............j"
		/*  4 */ "bak..........kab"
		/*  5 */ "mmaccccccccccamm"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmaccccccccccamm"
		/*  1 */ "ba............ab"
		/*  2 */ "ba............ab"
		/*  3 */ "ba............ab"
		/*  4 */ "ba............ab"
		/*  5 */ "mmaccccccccccamm"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmaaaaaaaaaammm"
		/*  1 */ "mmaaaaaaaaaaaamm"
		/*  2 */ "mmaccccccccccamm"
		/*  3 */ "mmaccccccccccamm"
		/*  4 */ "mmaaaaaaaaaaaamm"
		/*  5 */ "mmmaaaaaaaaaammm",

		// Connectors:
		"1: 0, 1, 2: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 3: 4\n"  /* Type -1, direction X- */
		"1: 15, 1, 3: 5\n"  /* Type 1, direction X+ */
		"-1: 15, 1, 2: 5\n"  /* Type -1, direction X+ */,

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
	},  // ViewingCorridor



	////////////////////////////////////////////////////////////////////////////////
	// ViewingCorridorBulge:
	// The data has been exported from the gallery Water, area index 42, ID 615, created by LO1ZB
	{
		// Size:
		12, 8, 16,  // SizeX = 12, SizeY = 8, SizeZ = 16

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		11, 7, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 20: 0\n"  /* glass */
		"d: 64: 3\n"  /* wooddoorblock */
		"e: 76: 1\n"  /* redstonetorchon */
		"f: 76: 2\n"  /* redstonetorchon */
		"g: 64: 1\n"  /* wooddoorblock */
		"h: 64: 8\n"  /* wooddoorblock */
		"i: 64: 9\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "mmmmmaammmmm"
		/*  1 */ "mmmmmbbmmmmm"
		/*  2 */ "mmmmmbbmmmmm"
		/*  3 */ "mmmmbbbbmmmm"
		/*  4 */ "mmmbbccbbmmm"
		/*  5 */ "mmbbccccbbmm"
		/*  6 */ "mbbccccccbbm"
		/*  7 */ "mbccccccccbm"
		/*  8 */ "mbccccccccbm"
		/*  9 */ "mbbccccccbbm"
		/* 10 */ "mmbbccccbbmm"
		/* 11 */ "mmmbbccbbmmm"
		/* 12 */ "mmmmbbbbmmmm"
		/* 13 */ "mmmmmbbmmmmm"
		/* 14 */ "mmmmmbbmmmmm"
		/* 15 */ "mmmmmaammmmm"

		// Level 1
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "mmmmaddammmm"
		/*  1 */ "mmmmb..bmmmm"
		/*  2 */ "mmmmb..bmmmm"
		/*  3 */ "mmmbbefbbmmm"
		/*  4 */ "mmbb....bbmm"
		/*  5 */ "mbb......bbm"
		/*  6 */ "mb........bm"
		/*  7 */ "be........fb"
		/*  8 */ "be........fb"
		/*  9 */ "mb........bm"
		/* 10 */ "mbb......bbm"
		/* 11 */ "mmbb....bbmm"
		/* 12 */ "mmmbbefbbmmm"
		/* 13 */ "mmmmb..bmmmm"
		/* 14 */ "mmmmb..bmmmm"
		/* 15 */ "mmmmaggammmm"

		// Level 2
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "mmmmahiammmm"
		/*  1 */ "mmmmb..bmmmm"
		/*  2 */ "mmmmc..cmmmm"
		/*  3 */ "mmmcc..ccmmm"
		/*  4 */ "mmcc....ccmm"
		/*  5 */ "mcc......ccm"
		/*  6 */ "mc........cm"
		/*  7 */ "c..........c"
		/*  8 */ "c..........c"
		/*  9 */ "mc........cm"
		/* 10 */ "mcc......ccm"
		/* 11 */ "mmcc....ccmm"
		/* 12 */ "mmmcc..ccmmm"
		/* 13 */ "mmmmc..cmmmm"
		/* 14 */ "mmmmb..bmmmm"
		/* 15 */ "mmmmaihammmm"

		// Level 3
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "mmmmaaaammmm"
		/*  1 */ "mmmmbbbbmmmm"
		/*  2 */ "mmmmccccmmmm"
		/*  3 */ "mmmcc..ccmmm"
		/*  4 */ "mmcc....ccmm"
		/*  5 */ "mcc......ccm"
		/*  6 */ "mc........cm"
		/*  7 */ "c..........c"
		/*  8 */ "c..........c"
		/*  9 */ "mc........cm"
		/* 10 */ "mcc......ccm"
		/* 11 */ "mmcc....ccmm"
		/* 12 */ "mmmcc..ccmmm"
		/* 13 */ "mmmmccccmmmm"
		/* 14 */ "mmmmbbbbmmmm"
		/* 15 */ "mmmmaaaammmm"

		// Level 4
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "mmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmm"
		/*  3 */ "mmmmmccmmmmm"
		/*  4 */ "mmmmc..cmmmm"
		/*  5 */ "mmmc....cmmm"
		/*  6 */ "mmc......cmm"
		/*  7 */ "mc........cm"
		/*  8 */ "mc........cm"
		/*  9 */ "mmc......cmm"
		/* 10 */ "mmmc....cmmm"
		/* 11 */ "mmmmc..cmmmm"
		/* 12 */ "mmmmmccmmmmm"
		/* 13 */ "mmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmm"

		// Level 5
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "mmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmm"
		/*  4 */ "mmmmmccmmmmm"
		/*  5 */ "mmmmc..cmmmm"
		/*  6 */ "mmmc....cmmm"
		/*  7 */ "mmc......cmm"
		/*  8 */ "mmc......cmm"
		/*  9 */ "mmmc....cmmm"
		/* 10 */ "mmmmc..cmmmm"
		/* 11 */ "mmmmmccmmmmm"
		/* 12 */ "mmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmm"

		// Level 6
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "mmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmm"
		/*  4 */ "mmmmmmmmmmmm"
		/*  5 */ "mmmmmccmmmmm"
		/*  6 */ "mmmmccccmmmm"
		/*  7 */ "mmmccccccmmm"
		/*  8 */ "mmmccccccmmm"
		/*  9 */ "mmmmccccmmmm"
		/* 10 */ "mmmmmccmmmmm"
		/* 11 */ "mmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmm"

		// Level 7
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "mmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmm"
		/*  4 */ "mmmmmmmmmmmm"
		/*  5 */ "mmmmmmmmmmmm"
		/*  6 */ "mmmmmmmmmmmm"
		/*  7 */ "mmmmmmmmmmmm"
		/*  8 */ "mmmmmmmmmmmm"
		/*  9 */ "mmmmmmmmmmmm"
		/* 10 */ "mmmmmmmmmmmm"
		/* 11 */ "mmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmm",

		// Connectors:
		"1: 6, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 5, 1, 0: 2\n"  /* Type -1, direction Z- */
		"1: 5, 1, 15: 3\n"  /* Type 1, direction Z+ */
		"-1: 6, 1, 15: 3\n"  /* Type -1, direction Z+ */,

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
	},  // ViewingCorridorBulge



	////////////////////////////////////////////////////////////////////////////////
	// ViewingCrossing:
	// The data has been exported from the gallery Water, area index 38, ID 611, created by LO1ZB
	{
		// Size:
		16, 7, 16,  // SizeX = 16, SizeY = 7, SizeZ = 16

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 6, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 20: 0\n"  /* glass */
		"d: 64: 3\n"  /* wooddoorblock */
		"e: 76: 1\n"  /* redstonetorchon */
		"f: 76: 2\n"  /* redstonetorchon */
		"g: 64: 2\n"  /* wooddoorblock */
		"h: 76: 3\n"  /* redstonetorchon */
		"i: 64: 0\n"  /* wooddoorblock */
		"j: 76: 4\n"  /* redstonetorchon */
		"k: 64: 1\n"  /* wooddoorblock */
		"l: 64: 8\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */
		"n: 64: 9\n"  /* wooddoorblock */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmaammmmmmm"
		/*  1 */ "mmmmmmmbbmmmmmmm"
		/*  2 */ "mmmmmmmbbmmmmmmm"
		/*  3 */ "mmmmmmbbbbmmmmmm"
		/*  4 */ "mmmmmbbccbbmmmmm"
		/*  5 */ "mmmmbbccccbbmmmm"
		/*  6 */ "mmmbbccccccbbmmm"
		/*  7 */ "abbbccccccccbbba"
		/*  8 */ "abbbccccccccbbba"
		/*  9 */ "mmmbbccccccbbmmm"
		/* 10 */ "mmmmbbccccbbmmmm"
		/* 11 */ "mmmmmbbccbbmmmmm"
		/* 12 */ "mmmmmmbbbbmmmmmm"
		/* 13 */ "mmmmmmmbbmmmmmmm"
		/* 14 */ "mmmmmmmbbmmmmmmm"
		/* 15 */ "mmmmmmmaammmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaddammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmb..bmmmmmm"
		/*  3 */ "mmmmmbbefbbmmmmm"
		/*  4 */ "mmmmbb....bbmmmm"
		/*  5 */ "mmmbb......bbmmm"
		/*  6 */ "abbb........bbba"
		/*  7 */ "g..h........h..i"
		/*  8 */ "g..j........j..i"
		/*  9 */ "abbb........bbba"
		/* 10 */ "mmmbb......bbmmm"
		/* 11 */ "mmmmbb....bbmmmm"
		/* 12 */ "mmmmmbbefbbmmmmm"
		/* 13 */ "mmmmmmb..bmmmmmm"
		/* 14 */ "mmmmmmb..bmmmmmm"
		/* 15 */ "mmmmmmakkammmmmm"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmalnammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmc..cmmmmmm"
		/*  3 */ "mmmmmcc..ccmmmmm"
		/*  4 */ "mmmmcc....ccmmmm"
		/*  5 */ "mmmcc......ccmmm"
		/*  6 */ "abcc........ccba"
		/*  7 */ "n..............l"
		/*  8 */ "l..............n"
		/*  9 */ "abcc........ccba"
		/* 10 */ "mmmcc......ccmmm"
		/* 11 */ "mmmmcc....ccmmmm"
		/* 12 */ "mmmmmcc..ccmmmmm"
		/* 13 */ "mmmmmmc..cmmmmmm"
		/* 14 */ "mmmmmmb..bmmmmmm"
		/* 15 */ "mmmmmmanlammmmmm"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaaaammmmmm"
		/*  1 */ "mmmmmmbbbbmmmmmm"
		/*  2 */ "mmmmmmccccmmmmmm"
		/*  3 */ "mmmmmcc..ccmmmmm"
		/*  4 */ "mmmmcc....ccmmmm"
		/*  5 */ "mmmcc......ccmmm"
		/*  6 */ "abcc........ccba"
		/*  7 */ "abc..........cba"
		/*  8 */ "abc..........cba"
		/*  9 */ "abcc........ccba"
		/* 10 */ "mmmcc......ccmmm"
		/* 11 */ "mmmmcc....ccmmmm"
		/* 12 */ "mmmmmcc..ccmmmmm"
		/* 13 */ "mmmmmmccccmmmmmm"
		/* 14 */ "mmmmmmbbbbmmmmmm"
		/* 15 */ "mmmmmmaaaammmmmm"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmccmmmmmmm"
		/*  4 */ "mmmmmmc..cmmmmmm"
		/*  5 */ "mmmmmc....cmmmmm"
		/*  6 */ "mmmmc......cmmmm"
		/*  7 */ "mmmc........cmmm"
		/*  8 */ "mmmc........cmmm"
		/*  9 */ "mmmmc......cmmmm"
		/* 10 */ "mmmmmc....cmmmmm"
		/* 11 */ "mmmmmmc..cmmmmmm"
		/* 12 */ "mmmmmmmccmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmmmm"
		/*  4 */ "mmmmmmmccmmmmmmm"
		/*  5 */ "mmmmmmc..cmmmmmm"
		/*  6 */ "mmmmmc....cmmmmm"
		/*  7 */ "mmmmc......cmmmm"
		/*  8 */ "mmmmc......cmmmm"
		/*  9 */ "mmmmmc....cmmmmm"
		/* 10 */ "mmmmmmc..cmmmmmm"
		/* 11 */ "mmmmmmmccmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm"

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmmmm"
		/*  4 */ "mmmmmmmmmmmmmmmm"
		/*  5 */ "mmmmmmmccmmmmmmm"
		/*  6 */ "mmmmmmccccmmmmmm"
		/*  7 */ "mmmmmccccccmmmmm"
		/*  8 */ "mmmmmccccccmmmmm"
		/*  9 */ "mmmmmmccccmmmmmm"
		/* 10 */ "mmmmmmmccmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm",

		// Connectors:
		"1: 0, 1, 7: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 8: 4\n"  /* Type -1, direction X- */
		"1: 8, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 7, 1, 0: 2\n"  /* Type -1, direction Z- */
		"1: 15, 1, 8: 5\n"  /* Type 1, direction X+ */
		"-1: 15, 1, 7: 5\n"  /* Type -1, direction X+ */
		"1: 7, 1, 15: 3\n"  /* Type 1, direction Z+ */
		"-1: 8, 1, 15: 3\n"  /* Type -1, direction Z+ */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		true,

		// DefaultWeight:
		50,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // ViewingCrossing



	////////////////////////////////////////////////////////////////////////////////
	// ViewingEnd:
	// The data has been exported from the gallery Water, area index 41, ID 614, created by LO1ZB
	{
		// Size:
		14, 7, 12,  // SizeX = 14, SizeY = 7, SizeZ = 12

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		13, 6, 11,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 0\n"  /* wood */
		"b: 20: 0\n"  /* glass */
		"c:  5: 5\n"  /* wood */
		"d: 76: 3\n"  /* redstonetorchon */
		"e: 76: 1\n"  /* redstonetorchon */
		"f: 64: 0\n"  /* wooddoorblock */
		"g: 76: 4\n"  /* redstonetorchon */
		"h: 64: 8\n"  /* wooddoorblock */
		"i: 64: 9\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmaaaammmmmm"
		/*  2 */ "mmmaabbaammmmm"
		/*  3 */ "mmaabbbbaammmm"
		/*  4 */ "maabbbbbbaammm"
		/*  5 */ "mabbbbbbbbaaac"
		/*  6 */ "mabbbbbbbbaaac"
		/*  7 */ "maabbbbbbaammm"
		/*  8 */ "mmaabbbbaammmm"
		/*  9 */ "mmmaabbaammmmm"
		/* 10 */ "mmmmaaaammmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmaammmmmmm"
		/*  1 */ "mmmaaddaammmmm"
		/*  2 */ "mmaa....aammmm"
		/*  3 */ "maa......aammm"
		/*  4 */ "ma........aaac"
		/*  5 */ "ae........d..f"
		/*  6 */ "ae........g..f"
		/*  7 */ "ma........aaac"
		/*  8 */ "maa......aammm"
		/*  9 */ "mmaa....aammmm"
		/* 10 */ "mmmaaggaammmmm"
		/* 11 */ "mmmmmaammmmmmm"

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmbbmmmmmmm"
		/*  1 */ "mmmbb..bbmmmmm"
		/*  2 */ "mmbb....bbmmmm"
		/*  3 */ "mbb......bbmmm"
		/*  4 */ "mb........bbac"
		/*  5 */ "b............h"
		/*  6 */ "b............i"
		/*  7 */ "mb........bbac"
		/*  8 */ "mbb......bbmmm"
		/*  9 */ "mmbb....bbmmmm"
		/* 10 */ "mmmbb..bbmmmmm"
		/* 11 */ "mmmmmbbmmmmmmm"

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmbbmmmmmmm"
		/*  1 */ "mmmbb..bbmmmmm"
		/*  2 */ "mmbb....bbmmmm"
		/*  3 */ "mbb......bbmmm"
		/*  4 */ "mb........bbac"
		/*  5 */ "b..........bac"
		/*  6 */ "b..........bac"
		/*  7 */ "mb........bbac"
		/*  8 */ "mbb......bbmmm"
		/*  9 */ "mmbb....bbmmmm"
		/* 10 */ "mmmbb..bbmmmmm"
		/* 11 */ "mmmmmbbmmmmmmm"

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmbbmmmmmmm"
		/*  2 */ "mmmmb..bmmmmmm"
		/*  3 */ "mmmb....bmmmmm"
		/*  4 */ "mmb......bmmmm"
		/*  5 */ "mb........bmmm"
		/*  6 */ "mb........bmmm"
		/*  7 */ "mmb......bmmmm"
		/*  8 */ "mmmb....bmmmmm"
		/*  9 */ "mmmmb..bmmmmmm"
		/* 10 */ "mmmmmbbmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmbbmmmmmmm"
		/*  3 */ "mmmmb..bmmmmmm"
		/*  4 */ "mmmb....bmmmmm"
		/*  5 */ "mmb......bmmmm"
		/*  6 */ "mmb......bmmmm"
		/*  7 */ "mmmb....bmmmmm"
		/*  8 */ "mmmmb..bmmmmmm"
		/*  9 */ "mmmmmbbmmmmmmm"
		/* 10 */ "mmmmmmmmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"

		// Level 6
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmm"
		/*  3 */ "mmmmmbbmmmmmmm"
		/*  4 */ "mmmmbbbbmmmmmm"
		/*  5 */ "mmmbbbbbbmmmmm"
		/*  6 */ "mmmbbbbbbmmmmm"
		/*  7 */ "mmmmbbbbmmmmmm"
		/*  8 */ "mmmmmbbmmmmmmm"
		/*  9 */ "mmmmmmmmmmmmmm"
		/* 10 */ "mmmmmmmmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm",

		// Connectors:
		"1: 13, 1, 6: 5\n"  /* Type 1, direction X+ */
		"-1: 13, 1, 5: 5\n"  /* Type -1, direction X+ */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		true,

		// DefaultWeight:
		200,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // ViewingEnd



	////////////////////////////////////////////////////////////////////////////////
	// ViewingTee:
	// The data has been exported from the gallery Water, area index 39, ID 612, created by LO1ZB
	{
		// Size:
		14, 7, 17,  // SizeX = 14, SizeY = 7, SizeZ = 17

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		13, 6, 16,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 20: 0\n"  /* glass */
		"d:  1: 0\n"  /* stone */
		"e: 64: 3\n"  /* wooddoorblock */
		"f: 76: 1\n"  /* redstonetorchon */
		"g: 76: 2\n"  /* redstonetorchon */
		"h: 76: 3\n"  /* redstonetorchon */
		"i: 64: 0\n"  /* wooddoorblock */
		"j: 76: 4\n"  /* redstonetorchon */
		"k: 64: 1\n"  /* wooddoorblock */
		"l: 64: 8\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmaammmmmmm"
		/*  1 */ "mmmmmbbmmmmmmm"
		/*  2 */ "mmmmmbbmmmmmmm"
		/*  3 */ "mmmmbbbbmmmmmm"
		/*  4 */ "mmmbbccbbmmmmm"
		/*  5 */ "mmbbccccbbmmmm"
		/*  6 */ "mbbccccccbbmmm"
		/*  7 */ "mbccccccccbbba"
		/*  8 */ "mbccccccccbbba"
		/*  9 */ "mbbccccccbbmmm"
		/* 10 */ "mmbbccccbbmmmm"
		/* 11 */ "mmmbbccbbmmmmm"
		/* 12 */ "mmmmbbbbmmmmmm"
		/* 13 */ "mmmmmbbmmmmmmm"
		/* 14 */ "mmmmmbbmmmmmmm"
		/* 15 */ "mmmmmaammmmmmm"
		/* 16 */ "dddddddddddddd"

		// Level 1
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmaeeammmmmm"
		/*  1 */ "mmmmb..bmmmmmm"
		/*  2 */ "mmmmb..bmmmmmm"
		/*  3 */ "mmmbbfgbbmmmmm"
		/*  4 */ "mmbb....bbmmmm"
		/*  5 */ "mbb......bbmmm"
		/*  6 */ "mb........bbba"
		/*  7 */ "bf........h..i"
		/*  8 */ "bf........j..i"
		/*  9 */ "mb........bbba"
		/* 10 */ "mbb......bbmmm"
		/* 11 */ "mmbb....bbmmmm"
		/* 12 */ "mmmbbfgbbmmmmm"
		/* 13 */ "mmmmb..bmmmmmm"
		/* 14 */ "mmmmb..bmmmmmm"
		/* 15 */ "mmmmakkammmmmm"
		/* 16 */ "dddddddddddddd"

		// Level 2
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmallammmmmm"
		/*  1 */ "mmmmb..bmmmmmm"
		/*  2 */ "mmmmc..cmmmmmm"
		/*  3 */ "mmmcc..ccmmmmm"
		/*  4 */ "mmcc....ccmmmm"
		/*  5 */ "mcc......ccmmm"
		/*  6 */ "mc........ccba"
		/*  7 */ "c............l"
		/*  8 */ "c............l"
		/*  9 */ "mc........ccba"
		/* 10 */ "mcc......ccmmm"
		/* 11 */ "mmcc....ccmmmm"
		/* 12 */ "mmmcc..ccmmmmm"
		/* 13 */ "mmmmc..cmmmmmm"
		/* 14 */ "mmmmb..bmmmmmm"
		/* 15 */ "mmmmallammmmmm"
		/* 16 */ "dddddddddddddd"

		// Level 3
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmaaaammmmmm"
		/*  1 */ "mmmmbbbbmmmmmm"
		/*  2 */ "mmmmccccmmmmmm"
		/*  3 */ "mmmcc..ccmmmmm"
		/*  4 */ "mmcc....ccmmmm"
		/*  5 */ "mcc......ccmmm"
		/*  6 */ "mc........ccba"
		/*  7 */ "c..........cba"
		/*  8 */ "c..........cba"
		/*  9 */ "mc........ccba"
		/* 10 */ "mcc......ccmmm"
		/* 11 */ "mmcc....ccmmmm"
		/* 12 */ "mmmcc..ccmmmmm"
		/* 13 */ "mmmmccccmmmmmm"
		/* 14 */ "mmmmbbbbmmmmmm"
		/* 15 */ "mmmmaaaammmmmm"
		/* 16 */ "dddddddddddddd"

		// Level 4
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmm"
		/*  3 */ "mmmmmccmmmmmmm"
		/*  4 */ "mmmmc..cmmmmmm"
		/*  5 */ "mmmc....cmmmmm"
		/*  6 */ "mmc......cmmmm"
		/*  7 */ "mc........cmmm"
		/*  8 */ "mc........cmmm"
		/*  9 */ "mmc......cmmmm"
		/* 10 */ "mmmc....cmmmmm"
		/* 11 */ "mmmmc..cmmmmmm"
		/* 12 */ "mmmmmccmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmm"
		/* 16 */ "dddddddddddddd"

		// Level 5
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmm"
		/*  4 */ "mmmmmccmmmmmmm"
		/*  5 */ "mmmmc..cmmmmmm"
		/*  6 */ "mmmc....cmmmmm"
		/*  7 */ "mmc......cmmmm"
		/*  8 */ "mmc......cmmmm"
		/*  9 */ "mmmc....cmmmmm"
		/* 10 */ "mmmmc..cmmmmmm"
		/* 11 */ "mmmmmccmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmm"
		/* 16 */ "dddddddddddddd"

		// Level 6
		/* z\x*             1111 */
		/*    *   01234567890123 */
		/*  0 */ "mmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmm"
		/*  4 */ "mmmmmmmmmmmmmm"
		/*  5 */ "mmmmmccmmmmmmm"
		/*  6 */ "mmmmccccmmmmmm"
		/*  7 */ "mmmccccccmmmmm"
		/*  8 */ "mmmccccccmmmmm"
		/*  9 */ "mmmmccccmmmmmm"
		/* 10 */ "mmmmmccmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmm"
		/* 16 */ "dddddddddddddd",

		// Connectors:
		"",

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		true,

		// DefaultWeight:
		75,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // ViewingTee



	////////////////////////////////////////////////////////////////////////////////
	// WaterfallRoom:
	// The data has been exported from the gallery Water, area index 50, ID 681, created by Aloe_vera
	{
		// Size:
		16, 7, 16,  // SizeX = 16, SizeY = 7, SizeZ = 16

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 6, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 20: 0\n"  /* glass */
		"d: 64: 3\n"  /* wooddoorblock */
		"e: 76: 1\n"  /* redstonetorchon */
		"f: 76: 2\n"  /* redstonetorchon */
		"g: 64: 2\n"  /* wooddoorblock */
		"h: 76: 3\n"  /* redstonetorchon */
		"i: 64: 0\n"  /* wooddoorblock */
		"j: 76: 4\n"  /* redstonetorchon */
		"k: 64: 1\n"  /* wooddoorblock */
		"l: 64: 8\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */
		"n: 64: 9\n"  /* wooddoorblock */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmaammmmmmm"
		/*  1 */ "mmmmmmmbbmmmmmmm"
		/*  2 */ "mmmmmmmbbmmmmmmm"
		/*  3 */ "mmmmmmbbbbmmmmmm"
		/*  4 */ "mmmmmbbccbbmmmmm"
		/*  5 */ "mmmmbbccccbbmmmm"
		/*  6 */ "mmmbbccccccbbmmm"
		/*  7 */ "abbbcccmmcccbbba"
		/*  8 */ "abbbcccmmcccbbba"
		/*  9 */ "mmmbbccccccbbmmm"
		/* 10 */ "mmmmbbccccbbmmmm"
		/* 11 */ "mmmmmbbccbbmmmmm"
		/* 12 */ "mmmmmmbbbbmmmmmm"
		/* 13 */ "mmmmmmmbbmmmmmmm"
		/* 14 */ "mmmmmmmbbmmmmmmm"
		/* 15 */ "mmmmmmmaammmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaddammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmb..bmmmmmm"
		/*  3 */ "mmmmmbbefbbmmmmm"
		/*  4 */ "mmmmbb....bbmmmm"
		/*  5 */ "mmmbb......bbmmm"
		/*  6 */ "abbb...cc...bbba"
		/*  7 */ "g..h..c..c..h..i"
		/*  8 */ "g..j..c..c..j..i"
		/*  9 */ "abbb...cc...bbba"
		/* 10 */ "mmmbb......bbmmm"
		/* 11 */ "mmmmbb....bbmmmm"
		/* 12 */ "mmmmmbbefbbmmmmm"
		/* 13 */ "mmmmmmb..bmmmmmm"
		/* 14 */ "mmmmmmb..bmmmmmm"
		/* 15 */ "mmmmmmakkammmmmm"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmalnammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmc..cmmmmmm"
		/*  3 */ "mmmmmcc..ccmmmmm"
		/*  4 */ "mmmmcc....ccmmmm"
		/*  5 */ "mmmcc......ccmmm"
		/*  6 */ "abcc........ccba"
		/*  7 */ "n..............l"
		/*  8 */ "l..............n"
		/*  9 */ "abcc........ccba"
		/* 10 */ "mmmcc......ccmmm"
		/* 11 */ "mmmmcc....ccmmmm"
		/* 12 */ "mmmmmcc..ccmmmmm"
		/* 13 */ "mmmmmmc..cmmmmmm"
		/* 14 */ "mmmmmmb..bmmmmmm"
		/* 15 */ "mmmmmmanlammmmmm"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaaaammmmmm"
		/*  1 */ "mmmmmmbbbbmmmmmm"
		/*  2 */ "mmmmmmccccmmmmmm"
		/*  3 */ "mmmmmcc..ccmmmmm"
		/*  4 */ "mmmmcc....ccmmmm"
		/*  5 */ "mmmcc......ccmmm"
		/*  6 */ "abcc........ccba"
		/*  7 */ "abc..........cba"
		/*  8 */ "abc..........cba"
		/*  9 */ "abcc........ccba"
		/* 10 */ "mmmcc......ccmmm"
		/* 11 */ "mmmmcc....ccmmmm"
		/* 12 */ "mmmmmcc..ccmmmmm"
		/* 13 */ "mmmmmmccccmmmmmm"
		/* 14 */ "mmmmmmbbbbmmmmmm"
		/* 15 */ "mmmmmmaaaammmmmm"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmccmmmmmmm"
		/*  4 */ "mmmmm.c..cmmmmmm"
		/*  5 */ "mmmmmc....cmmmmm"
		/*  6 */ "mmmmc......cmmmm"
		/*  7 */ "mmmc........cmmm"
		/*  8 */ "mmmc........cmmm"
		/*  9 */ "mmmmc......cmmmm"
		/* 10 */ "mmmmmc....cmmmmm"
		/* 11 */ "mmmmmmc..cmmmmmm"
		/* 12 */ "mmmmmmmccmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmmmm"
		/*  4 */ "mmmmmm.ccmmmmmmm"
		/*  5 */ "mmmmmmc..cmmmmmm"
		/*  6 */ "mmmmmc....cmmmmm"
		/*  7 */ "mmmmc......cmmmm"
		/*  8 */ "mmmmc......cmmmm"
		/*  9 */ "mmmmmc....cmmmmm"
		/* 10 */ "mmmmmmc..cmmmmmm"
		/* 11 */ "mmmmmmmccmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm"

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmmmm"
		/*  4 */ "mmmmmmmmmmmmmmmm"
		/*  5 */ "mmmmmmmccmmmmmmm"
		/*  6 */ "mmmmmmccccmmmmmm"
		/*  7 */ "mmmmmcc..ccmmmmm"
		/*  8 */ "mmmmmcc..ccmmmmm"
		/*  9 */ "mmmmmmccccmmmmmm"
		/* 10 */ "mmmmmmmccmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm",

		// Connectors:
		"1: 15, 1, 8: 5\n"  /* Type 1, direction X+ */
		"-1: 15, 1, 7: 5\n"  /* Type -1, direction X+ */
		"1: 8, 1, 0: 2\n"  /* Type 1, direction Z- */
		"-1: 7, 1, 0: 2\n"  /* Type -1, direction Z- */
		"1: 0, 1, 7: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 8: 4\n"  /* Type -1, direction X- */
		"1: 7, 1, 15: 3\n"  /* Type 1, direction Z+ */
		"-1: 8, 1, 15: 3\n"  /* Type -1, direction Z+ */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		true,

		// DefaultWeight:
		5,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,

		// MoveToGround:
		false,
	},  // WaterfallRoom
};  // g_UnderwaterBasePrefabs






const cPrefab::sDef g_UnderwaterBaseStartingPrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// CentralRoom:
	// The data has been exported from the gallery Water, area index 24, ID 564, created by xoft
	{
		// Size:
		16, 7, 16,  // SizeX = 16, SizeY = 7, SizeZ = 16

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 6, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 5\n"  /* wood */
		"b:  5: 0\n"  /* wood */
		"c: 20: 0\n"  /* glass */
		"d: 64: 3\n"  /* wooddoorblock */
		"e: 64: 2\n"  /* wooddoorblock */
		"f: 64: 0\n"  /* wooddoorblock */
		"g: 64: 1\n"  /* wooddoorblock */
		"h: 64: 8\n"  /* wooddoorblock */
		"i: 64: 9\n"  /* wooddoorblock */
		"j: 76: 3\n"  /* redstonetorchon */
		"k: 76: 1\n"  /* redstonetorchon */
		"l: 76: 2\n"  /* redstonetorchon */
		"m: 19: 0\n"  /* sponge */
		"n: 76: 4\n"  /* redstonetorchon */
		"o:125: 8\n"  /* woodendoubleslab */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmaammmmmmm"
		/*  1 */ "mmmmmmmbbmmmmmmm"
		/*  2 */ "mmmmmmmbbmmmmmmm"
		/*  3 */ "mmmmmmbbbbmmmmmm"
		/*  4 */ "mmmmmbbbbbbmmmmm"
		/*  5 */ "mmmmbbbccbbbmmmm"
		/*  6 */ "mmmbbbccccbbbmmm"
		/*  7 */ "abbbbccccccbbbba"
		/*  8 */ "abbbbccccccbbbba"
		/*  9 */ "mmmbbbccccbbbmmm"
		/* 10 */ "mmmmbbbccbbbmmmm"
		/* 11 */ "mmmmmbbbbbbmmmmm"
		/* 12 */ "mmmmmmbbbbmmmmmm"
		/* 13 */ "mmmmmmmbbmmmmmmm"
		/* 14 */ "mmmmmmmbbmmmmmmm"
		/* 15 */ "mmmmmmmaammmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaddammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmb..bmmmmmm"
		/*  3 */ "mmmmmbb..bbmmmmm"
		/*  4 */ "mmmmbb....bbmmmm"
		/*  5 */ "mmmbb......bbmmm"
		/*  6 */ "abbb........bbba"
		/*  7 */ "e..............f"
		/*  8 */ "e..............f"
		/*  9 */ "abbb........bbba"
		/* 10 */ "mmmbb......bbmmm"
		/* 11 */ "mmmmbb....bbmmmm"
		/* 12 */ "mmmmmbb..bbmmmmm"
		/* 13 */ "mmmmmmb..bmmmmmm"
		/* 14 */ "mmmmmmb..bmmmmmm"
		/* 15 */ "mmmmmmaggammmmmm"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmahiammmmmm"
		/*  1 */ "mmmmmmb..bmmmmmm"
		/*  2 */ "mmmmmmb..bmmmmmm"
		/*  3 */ "mmmmmcc..ccmmmmm"
		/*  4 */ "mmmmcc....ccmmmm"
		/*  5 */ "mmmcc......ccmmm"
		/*  6 */ "abbc........cbba"
		/*  7 */ "i..............h"
		/*  8 */ "h..............i"
		/*  9 */ "abbc........cbba"
		/* 10 */ "mmmcc......ccmmm"
		/* 11 */ "mmmmcc....ccmmmm"
		/* 12 */ "mmmmmcc..ccmmmmm"
		/* 13 */ "mmmmmmb..bmmmmmm"
		/* 14 */ "mmmmmmb..bmmmmmm"
		/* 15 */ "mmmmmmaihammmmmm"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmaaaammmmmm"
		/*  1 */ "mmmmmmbbbbmmmmmm"
		/*  2 */ "mmmmmmbbbbmmmmmm"
		/*  3 */ "mmmmmbb..bbmmmmm"
		/*  4 */ "mmmmbb....bbmmmm"
		/*  5 */ "mmmbb......bbmmm"
		/*  6 */ "abbb........bbba"
		/*  7 */ "abb..........bba"
		/*  8 */ "abb..........bba"
		/*  9 */ "abbb........bbba"
		/* 10 */ "mmmbb......bbmmm"
		/* 11 */ "mmmmbb....bbmmmm"
		/* 12 */ "mmmmmbb..bbmmmmm"
		/* 13 */ "mmmmmmbbbbmmmmmm"
		/* 14 */ "mmmmmmbbbbmmmmmm"
		/* 15 */ "mmmmmmaaaammmmmm"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmbbmmmmmmm"
		/*  4 */ "mmmmmmbjjbmmmmmm"
		/*  5 */ "mmmmmb....bmmmmm"
		/*  6 */ "mmmmb......bmmmm"
		/*  7 */ "mmmbk......lbmmm"
		/*  8 */ "mmmbk......lbmmm"
		/*  9 */ "mmmmb......bmmmm"
		/* 10 */ "mmmmmb....bmmmmm"
		/* 11 */ "mmmmmmbnnbmmmmmm"
		/* 12 */ "mmmmmmmbbmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmmmm"
		/*  4 */ "mmmmmmmbbmmmmmmm"
		/*  5 */ "mmmmmmb..bmmmmmm"
		/*  6 */ "mmmmmb....bmmmmm"
		/*  7 */ "mmmmb......bmmmm"
		/*  8 */ "mmmmb......bmmmm"
		/*  9 */ "mmmmmb....bmmmmm"
		/* 10 */ "mmmmmmboobmmmmmm"
		/* 11 */ "mmmmmmmbbmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm"

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmmm"
		/*  3 */ "mmmmmmmmmmmmmmmm"
		/*  4 */ "mmmmmmmmmmmmmmmm"
		/*  5 */ "mmmmmmmbbmmmmmmm"
		/*  6 */ "mmmmmmbbbbmmmmmm"
		/*  7 */ "mmmmmbbccbbmmmmm"
		/*  8 */ "mmmmmbbccbbmmmmm"
		/*  9 */ "mmmmmmbbbbmmmmmm"
		/* 10 */ "mmmmmmmbbmmmmmmm"
		/* 11 */ "mmmmmmmmmmmmmmmm"
		/* 12 */ "mmmmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmmm",

		// Connectors:
		"1: 0, 1, 7: 4\n"  /* Type 1, direction X- */
		"-1: 0, 1, 8: 4\n"  /* Type -1, direction X- */
		"-1: 7, 1, 0: 2\n"  /* Type -1, direction Z- */
		"1: 8, 1, 0: 2\n"  /* Type 1, direction Z- */
		"1: 15, 1, 8: 5\n"  /* Type 1, direction X+ */
		"-1: 15, 1, 7: 5\n"  /* Type -1, direction X+ */
		"1: 7, 1, 15: 3\n"  /* Type 1, direction Z+ */
		"-1: 8, 1, 15: 3\n"  /* Type -1, direction Z+ */,

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
	},  // CentralRoom
};





// The prefab counts:

const size_t g_UnderwaterBasePrefabsCount = ARRAYCOUNT(g_UnderwaterBasePrefabs);

const size_t g_UnderwaterBaseStartingPrefabsCount = ARRAYCOUNT(g_UnderwaterBaseStartingPrefabs);

