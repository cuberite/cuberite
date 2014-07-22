
// JapaneseVillagePrefabs.cpp

// Defines the prefabs in the group JapaneseVillage

// NOTE: This file has been generated automatically by GalExport!
// Any manual changes will be overwritten by the next automatic export!

#include "Globals.h"
#include "JapaneseVillagePrefabs.h"





const cPrefab::sDef g_JapaneseVillagePrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// Arch:
	// The data has been exported from the gallery Plains, area index 144, ID 488, created by Aloe_vera
	{
		// Size:
		11, 7, 5,  // SizeX = 11, SizeY = 7, SizeZ = 5

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		11, 6, 4,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  2: 0\n"  /* grass */
		"b: 13: 0\n"  /* gravel */
		"c:113: 0\n"  /* netherbrickfence */
		"d: 50: 5\n"  /* torch */
		"e: 44: 8\n"  /* step */
		"f: 44: 0\n"  /* step */
		"g: 43: 0\n"  /* doubleslab */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaaabbbaaaa"
		/*  1 */ "aaaabbbaaaa"
		/*  2 */ "aaaabbbaaaa"
		/*  3 */ "aaaabbbaaaa"
		/*  4 */ "aaaabbbaaaa"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..c.....c.."
		/*  1 */ "..c.....c.."
		/*  2 */ "..c.....c.."
		/*  3 */ "..c.....c.."
		/*  4 */ "..c.....c.."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..c.....c.."
		/*  1 */ "..........."
		/*  2 */ "..c.....c.."
		/*  3 */ "..........."
		/*  4 */ "..c.....c.."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..d.....d.."
		/*  1 */ "..........."
		/*  2 */ "..c.....c.."
		/*  3 */ "..........."
		/*  4 */ "..d.....d.."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "...eeeee..."
		/*  1 */ "..........."
		/*  2 */ "..c.....c.."
		/*  3 */ "..........."
		/*  4 */ "...eeeee..."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..f.....f.."
		/*  1 */ ".egfffffge."
		/*  2 */ ".egeeeeege."
		/*  3 */ ".egfffffge."
		/*  4 */ "..f.....f.."

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "gf.......fg"
		/*  3 */ "..........."
		/*  4 */ "...........",

		// Connectors:
		"2: 5, 1, 4: 3\n"  /* Type 2, direction Z+ */
		"2: 5, 1, 0: 2\n"  /* Type 2, direction Z- */,

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
	},  // Arch



	////////////////////////////////////////////////////////////////////////////////
	// Farm:
	// The data has been exported from the gallery Plains, area index 166, ID 554, created by Aloe_vera
	{
		// Size:
		11, 8, 13,  // SizeX = 11, SizeY = 8, SizeZ = 13

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 7, 12,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b: 60: 7\n"  /* tilleddirt */
		"c:  8: 0\n"  /* water */
		"d: 43: 0\n"  /* doubleslab */
		"e: 44: 0\n"  /* step */
		"f: 59: 7\n"  /* crops */
		"g: 83: 0\n"  /* reedblock */
		"h:113: 0\n"  /* netherbrickfence */
		"i: 50: 5\n"  /* torch */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "mmmmmmmmmmm"
		/*  1 */ "maaaaaaaaam"
		/*  2 */ "maaaaaaaaam"
		/*  3 */ "maaaaaaaaam"
		/*  4 */ "maaaaaaaaam"
		/*  5 */ "maaaaaaaaam"
		/*  6 */ "maaaaaaaaam"
		/*  7 */ "maaaaaaaaam"
		/*  8 */ "maaaaaaaaam"
		/*  9 */ "maaaaaaaaam"
		/* 10 */ "maaaaaaaaam"
		/* 11 */ "maaaaaaaaam"
		/* 12 */ "mmmmmmmmmmm"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "mmmmmmmmmmm"
		/*  1 */ "maaaaaaaaam"
		/*  2 */ "mabbbbbbbam"
		/*  3 */ "mabbbbbbbam"
		/*  4 */ "mabbbbbbbam"
		/*  5 */ "mabbbbbbbam"
		/*  6 */ "mabcccccaam"
		/*  7 */ "mabbbbbbbam"
		/*  8 */ "mabbbbbbbam"
		/*  9 */ "mabbbbbbbam"
		/* 10 */ "mabbbbbbbam"
		/* 11 */ "maaaaaaaaam"
		/* 12 */ "mmmmmmmmmmm"

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".deeeeeeed."
		/*  2 */ ".efffffffe."
		/*  3 */ ".efffffffe."
		/*  4 */ ".efffffffe."
		/*  5 */ ".efgggggfe."
		/*  6 */ ".eg.....ge."
		/*  7 */ ".efgggggfe."
		/*  8 */ ".efffffffe."
		/*  9 */ ".efffffffe."
		/* 10 */ ".efffffffe."
		/* 11 */ ".deeeeeeed."
		/* 12 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".h.......h."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "...ggggg..."
		/*  6 */ "..g.....g.."
		/*  7 */ "...ggggg..."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."
		/* 11 */ ".h.......h."
		/* 12 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".h.......h."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "...ggggg..."
		/*  6 */ "..g.....g.."
		/*  7 */ "...ggggg..."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."
		/* 11 */ ".h.......h."
		/* 12 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".h.......h."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."
		/* 11 */ ".h.......h."
		/* 12 */ "..........."

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ ".h.......h."
		/*  1 */ "hhh.....hhh"
		/*  2 */ ".h.......h."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ ".h.......h."
		/* 11 */ "hhh.....hhh"
		/* 12 */ ".h.......h."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ ".i.......i."
		/*  1 */ "i.i.....i.i"
		/*  2 */ ".i.......i."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ ".i.......i."
		/* 11 */ "i.i.....i.i"
		/* 12 */ ".i.......i.",

		// Connectors:
		"-1: 10, 2, 6: 5\n"  /* Type -1, direction X+ */,

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
	},  // Farm



	////////////////////////////////////////////////////////////////////////////////
	// Forge:
	// The data has been exported from the gallery Plains, area index 79, ID 145, created by Aloe_vera
	{
		// Size:
		16, 11, 14,  // SizeX = 16, SizeY = 11, SizeZ = 14

		// Hitbox (relative to bounding box):
		0, 0, -1,  // MinX, MinY, MinZ
		16, 10, 14,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  4: 0\n"  /* cobblestone */
		"b: 17: 1\n"  /* tree */
		"c: 67: 0\n"  /* stairs */
		"d:  5: 2\n"  /* wood */
		"e: 67: 2\n"  /* stairs */
		"f:113: 0\n"  /* netherbrickfence */
		"g:118: 2\n"  /* cauldronblock */
		"h: 67: 6\n"  /* stairs */
		"i: 67: 4\n"  /* stairs */
		"j: 87: 0\n"  /* netherstone */
		"k: 67: 7\n"  /* stairs */
		"l: 54: 5\n"  /* chest */
		"m: 19: 0\n"  /* sponge */
		"n: 61: 2\n"  /* furnace */
		"o:101: 0\n"  /* ironbars */
		"p: 51: 0\n"  /* fire */
		"q: 50: 4\n"  /* torch */
		"r: 50: 2\n"  /* torch */
		"s: 35: 0\n"  /* wool */
		"t: 67: 3\n"  /* stairs */
		"u: 50: 3\n"  /* torch */
		"v: 44: 8\n"  /* step */
		"w: 43: 0\n"  /* doubleslab */
		"x: 44: 0\n"  /* step */
		"y: 17: 5\n"  /* tree */
		"z: 17: 9\n"  /* tree */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmmm"
		/*  2 */ "mmaaaaaaaaaaaamm"
		/*  3 */ "mmaaaaaaaaaaaamm"
		/*  4 */ "mmaaaaaaaaaaaamm"
		/*  5 */ "mmaaaaaaaaaaaamm"
		/*  6 */ "mmaaaaaaaaaaaamm"
		/*  7 */ "mmaaaaaaaaaaaamm"
		/*  8 */ "mmaaaaaaaaaaaamm"
		/*  9 */ "mmaaaaaaaaaaaamm"
		/* 10 */ "mmaaaaaaaaaaaamm"
		/* 11 */ "mmaaaaaaaaaaaamm"
		/* 12 */ "mmmmmmmmmmmmmmmm"
		/* 13 */ "mmmmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ ".....bbbbbbbbb.."
		/*  3 */ ".....cdddddddb.."
		/*  4 */ ".....cddaaaadb.."
		/*  5 */ "..beeedaaaaadb.."
		/*  6 */ "..bddddaaaaadb.."
		/*  7 */ "..bddddaaaaadb.."
		/*  8 */ "..bddddaaaaadb.."
		/*  9 */ "..bddddaaaaadb.."
		/* 10 */ "..bddddddddddb.."
		/* 11 */ "..bbbbbbbbbbbb.."
		/* 12 */ "................"
		/* 13 */ "................"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ ".....bfffbfffb.."
		/*  3 */ ".............a.."
		/*  4 */ ".............a.."
		/*  5 */ "..b.....ghh..a.."
		/*  6 */ "..f.....haa..b.."
		/*  7 */ "..f.....ija..b.."
		/*  8 */ "..f.....kaa..a.."
		/*  9 */ "..f..........a.."
		/* 10 */ "..fl.........a.."
		/* 11 */ "..bffffbbffffb.."
		/* 12 */ "................"
		/* 13 */ "................"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ ".....bfffbfffb.."
		/*  3 */ ".............a.."
		/*  4 */ ".............a.."
		/*  5 */ "..b......nn..a.."
		/*  6 */ "..f.....oaa..b.."
		/*  7 */ "..f.....opa..b.."
		/*  8 */ "..f.....oaa..a.."
		/*  9 */ "..f..........a.."
		/* 10 */ "..f..........a.."
		/* 11 */ "..bffffbbffffb.."
		/* 12 */ "................"
		/* 13 */ "................"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".........q...q.."
		/*  2 */ "....rbsssbsssb.."
		/*  3 */ ".............a.."
		/*  4 */ "..q..........a.."
		/*  5 */ "..b......ce..a.."
		/*  6 */ "..s......ea..b.."
		/*  7 */ "..s......aa..b.."
		/*  8 */ "..s......ta..a.."
		/*  9 */ "..s..........a.."
		/* 10 */ "..s..........a.."
		/* 11 */ ".rbssssbbssssb.."
		/* 12 */ "..u....uu....u.."
		/* 13 */ "................"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ ".vwxxxxxxxxxxwv."
		/*  1 */ "vvvvvvvvvvvvvvvv"
		/*  2 */ "wvbyybyyybbyybvw"
		/*  3 */ "xvz..........zvx"
		/*  4 */ "xvz..........zvx"
		/*  5 */ "xvb..........zvx"
		/*  6 */ "xvz.......a..bvx"
		/*  7 */ "xvz......ca..bvx"
		/*  8 */ "xvz.......a..zvx"
		/*  9 */ "xvz..........zvx"
		/* 10 */ "xvz..........zvx"
		/* 11 */ "wvbyyyyyyyyyybvw"
		/* 12 */ "vvvvvvvvvvvvvvvv"
		/* 13 */ ".vwxxxxxxxxxxwv."

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "wx............xw"
		/*  1 */ "x..............x"
		/*  2 */ "..xxxxxxxxxxxx.."
		/*  3 */ "..xwwwwwwwwwwx.."
		/*  4 */ "..xwvvvvvvvvvx.."
		/*  5 */ "..xwv.......vx.."
		/*  6 */ "..xwv.....a.vx.."
		/*  7 */ "..xwv.....a.vx.."
		/*  8 */ "..xwv.....a.vx.."
		/*  9 */ "..xwvvvvvvvvvx.."
		/* 10 */ "..xwwwwwwwwwwx.."
		/* 11 */ "..xxxxxxxxxxxx.."
		/* 12 */ "x..............x"
		/* 13 */ "wx............xw"

		// Level 7
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "................"
		/*  4 */ "....xxxxxxxx...."
		/*  5 */ "....xxxxxxxx...."
		/*  6 */ "....xwwwwwax...."
		/*  7 */ "....xwvvvvax...."
		/*  8 */ "....xwwwwwax...."
		/*  9 */ "....xxxxxxxx...."
		/* 10 */ "................"
		/* 11 */ "................"
		/* 12 */ "................"
		/* 13 */ "................"

		// Level 8
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "................"
		/*  4 */ "................"
		/*  5 */ "................"
		/*  6 */ "..........a....."
		/*  7 */ ".......xx.a....."
		/*  8 */ "..........a....."
		/*  9 */ "................"
		/* 10 */ "................"
		/* 11 */ "................"
		/* 12 */ "................"
		/* 13 */ "................"

		// Level 9
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "................"
		/*  4 */ "................"
		/*  5 */ "................"
		/*  6 */ "..........a....."
		/*  7 */ "..........a....."
		/*  8 */ "..........a....."
		/*  9 */ "................"
		/* 10 */ "................"
		/* 11 */ "................"
		/* 12 */ "................"
		/* 13 */ "................"

		// Level 10
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "................"
		/*  4 */ "................"
		/*  5 */ "................"
		/*  6 */ "..........a....."
		/*  7 */ "..........a....."
		/*  8 */ "..........a....."
		/*  9 */ "................"
		/* 10 */ "................"
		/* 11 */ "................"
		/* 12 */ "................"
		/* 13 */ "................",

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
	},  // Forge



	////////////////////////////////////////////////////////////////////////////////
	// Garden2:
	// The data has been exported from the gallery Plains, area index 147, ID 491, created by Aloe_vera
	{
		// Size:
		16, 5, 16,  // SizeX = 16, SizeY = 5, SizeZ = 16

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 4, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  8: 0\n"  /* water */
		"c:  2: 0\n"  /* grass */
		"d: 17: 1\n"  /* tree */
		"e: 13: 0\n"  /* gravel */
		"f: 31: 2\n"  /* tallgrass */
		"g: 18: 5\n"  /* leaves */
		"h: 38: 7\n"  /* rose */
		"i: 17: 9\n"  /* tree */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "aaaaaaaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaaaaaaa"
		/*  9 */ "aaaaaaaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaaaaaaa"
		/* 11 */ "aaaaaaaaaaaaaaaa"
		/* 12 */ "aaaaaaaaaaaaaaaa"
		/* 13 */ "aaaaaaaaaaaaaaaa"
		/* 14 */ "aaaaaaaaaaaaaaaa"
		/* 15 */ "aaaaaaaaaaaaaaaa"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "aaaaaaaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaaaaaaa"
		/*  6 */ "aaaabbaaaaaaaaaa"
		/*  7 */ "aaabbbaaaaaaaaaa"
		/*  8 */ "aaabbaaaaaaaaaaa"
		/*  9 */ "aaaabaaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaaaaaaa"
		/* 11 */ "aaaaaaaaaaaaaaaa"
		/* 12 */ "aaaaaaaaaaaaaaaa"
		/* 13 */ "aaaaaaaaaaaaaaaa"
		/* 14 */ "aaaaaaaaaaaaaaaa"
		/* 15 */ "aaaaaaaaaaaaaaaa"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "cccccccccccccccc"
		/*  1 */ "ccdccccccccdcccc"
		/*  2 */ "cccccceecccccdcc"
		/*  3 */ "ccccccceeccccccc"
		/*  4 */ "cccccccceccccccc"
		/*  5 */ "cccbbbbceccccccc"
		/*  6 */ "cccbbbbceecccccc"
		/*  7 */ "ccbbbbbcceeeeccc"
		/*  8 */ "ccbbbbbccccceecc"
		/*  9 */ "ccbbbbcccccccecc"
		/* 10 */ "ccccbcccccccceec"
		/* 11 */ "ccccccccccccccec"
		/* 12 */ "ccccccccaaacccec"
		/* 13 */ "cccccccccaccccec"
		/* 14 */ "ccccccccccccceec"
		/* 15 */ "cccccccccccceecc"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "......f...gg.g.."
		/*  1 */ "..gg.....gggggg."
		/*  2 */ "ffgg......ghgggg"
		/*  3 */ ".............gg."
		/*  4 */ "...........f...."
		/*  5 */ "...........h.ff."
		/*  6 */ ".............fh."
		/*  7 */ "...............f"
		/*  8 */ "................"
		/*  9 */ ".......ff.f....."
		/* 10 */ ".f.....ffggf...."
		/* 11 */ ".......gggg.f..."
		/* 12 */ ".f......iddg...."
		/* 13 */ ".....f..gdgg...."
		/* 14 */ "....ff...gg....."
		/* 15 */ "................"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "...........g.g.."
		/*  2 */ ".............gg."
		/*  3 */ "................"
		/*  4 */ "................"
		/*  5 */ "................"
		/*  6 */ "................"
		/*  7 */ "................"
		/*  8 */ "................"
		/*  9 */ "................"
		/* 10 */ ".........g......"
		/* 11 */ "........ggg....."
		/* 12 */ "........ggg....."
		/* 13 */ ".........g......"
		/* 14 */ "................"
		/* 15 */ "................",

		// Connectors:
		"-1: 12, 3, 15: 3\n"  /* Type -1, direction Z+ */,

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
	},  // Garden2



	////////////////////////////////////////////////////////////////////////////////
	// HouseMid:
	// The data has been exported from the gallery Plains, area index 62, ID 119, created by Aloe_vera
	{
		// Size:
		10, 9, 9,  // SizeX = 10, SizeY = 9, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, -1,  // MinX, MinY, MinZ
		10, 8, 9,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b:135: 2\n"  /* 135 */
		"c:135: 0\n"  /* 135 */
		"d: 17: 9\n"  /* tree */
		"e:135: 3\n"  /* 135 */
		"f: 85: 0\n"  /* fence */
		"g: 17: 1\n"  /* tree */
		"h:171: 0\n"  /* carpet */
		"i: 50: 5\n"  /* torch */
		"j: 35: 0\n"  /* wool */
		"k: 17: 5\n"  /* tree */
		"l:124: 0\n"  /* redstonelampon */
		"m: 19: 0\n"  /* sponge */
		"n: 69: 9\n"  /* lever */
		"o: 44: 8\n"  /* step */
		"p: 43: 0\n"  /* doubleslab */
		"q: 44: 0\n"  /* step */,

		// Block data:
		// Level 0
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "maaaaaaaaa"
		/*  1 */ "maaaaaaaaa"
		/*  2 */ "aaaaaaaaaa"
		/*  3 */ "aaaaaaaaaa"
		/*  4 */ "aaaaaaaaaa"
		/*  5 */ "aaaaaaaaaa"
		/*  6 */ "aaaaaaaaaa"
		/*  7 */ "maaaaaaaaa"
		/*  8 */ "maaaaaaaaa"

		// Level 1
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ ".aaaaaaaaa"
		/*  1 */ ".aaaaaaaaa"
		/*  2 */ "baaaaaaaaa"
		/*  3 */ "caaaaaaaaa"
		/*  4 */ "caadaaaaaa"
		/*  5 */ "caaaaaaaaa"
		/*  6 */ "eaaaaaaaaa"
		/*  7 */ ".aaaaaaaaa"
		/*  8 */ ".aaaaaaaaa"

		// Level 2
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ ".fffffffff"
		/*  1 */ ".f.......f"
		/*  2 */ ".f.ggggg.f"
		/*  3 */ "...ghhhg.f"
		/*  4 */ "....hhhg.f"
		/*  5 */ "...ghhhg.f"
		/*  6 */ ".f.ggggg.f"
		/*  7 */ ".f.......f"
		/*  8 */ ".fffffffff"

		// Level 3
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ ".....i...i"
		/*  1 */ ".........."
		/*  2 */ ".i.jjgjj.."
		/*  3 */ "...g...j.."
		/*  4 */ ".......g.i"
		/*  5 */ "...g...j.."
		/*  6 */ ".i.jjgjj.."
		/*  7 */ ".........."
		/*  8 */ ".....i...i"

		// Level 4
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ ".........."
		/*  1 */ ".........."
		/*  2 */ "...jjgjj.."
		/*  3 */ "...g...j.."
		/*  4 */ "...j...g.."
		/*  5 */ "...g...j.."
		/*  6 */ "...jjgjj.."
		/*  7 */ ".........."
		/*  8 */ ".........."

		// Level 5
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ ".........."
		/*  1 */ "...f...f.."
		/*  2 */ "..fgkgkgf."
		/*  3 */ "..fd...d.."
		/*  4 */ "...d.lng.."
		/*  5 */ "..fd...d.."
		/*  6 */ "..fgkgkgf."
		/*  7 */ "...f...f.."
		/*  8 */ ".........."

		// Level 6
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "...ooooo.."
		/*  1 */ "..opppppo."
		/*  2 */ ".opgjjjgpo"
		/*  3 */ ".opjgggjpo"
		/*  4 */ ".opjgggjpo"
		/*  5 */ ".opjgggjpo"
		/*  6 */ ".opgjjjgpo"
		/*  7 */ "..opppppo."
		/*  8 */ "...ooooo.."

		// Level 7
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ ".opq...qpo"
		/*  1 */ ".pq.....qp"
		/*  2 */ ".q.qqqqq.q"
		/*  3 */ "...qpppq.."
		/*  4 */ "...qpppq.."
		/*  5 */ "...qpppq.."
		/*  6 */ ".q.qqqqq.q"
		/*  7 */ ".pq.....qp"
		/*  8 */ ".opq...qpo"

		// Level 8
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ ".q.......q"
		/*  1 */ ".........."
		/*  2 */ ".........."
		/*  3 */ ".........."
		/*  4 */ ".....q...."
		/*  5 */ ".........."
		/*  6 */ ".........."
		/*  7 */ ".........."
		/*  8 */ ".q.......q",

		// Connectors:
		"-1: 0, 1, 4: 4\n"  /* Type -1, direction X- */,

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
	},  // HouseMid



	////////////////////////////////////////////////////////////////////////////////
	// HouseSmall:
	// The data has been exported from the gallery Plains, area index 68, ID 131, created by Aloe_vera
	{
		// Size:
		7, 6, 7,  // SizeX = 7, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		7, 5, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b: 17: 1\n"  /* tree */
		"c: 35: 0\n"  /* wool */
		"d: 50: 4\n"  /* torch */
		"e: 85: 0\n"  /* fence */
		"f: 44: 8\n"  /* step */
		"g: 43: 0\n"  /* doubleslab */
		"h: 44: 0\n"  /* step */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "maaaaam"
		/*  2 */ "maaaaam"
		/*  3 */ "maaaaam"
		/*  4 */ "maaaaam"
		/*  5 */ "maaaaam"
		/*  6 */ "mmmmmmm"

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".bcc.b."
		/*  2 */ ".c...c."
		/*  3 */ ".c...c."
		/*  4 */ ".c...c."
		/*  5 */ ".bcccb."
		/*  6 */ "......."

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ ".....d."
		/*  1 */ ".bee.b."
		/*  2 */ ".c...c."
		/*  3 */ ".e...e."
		/*  4 */ ".c...c."
		/*  5 */ ".beeeb."
		/*  6 */ "......."

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ ".fffff."
		/*  1 */ "fbcccbf"
		/*  2 */ "fc...cf"
		/*  3 */ "fc...cf"
		/*  4 */ "fc...cf"
		/*  5 */ "fbcccbf"
		/*  6 */ ".fffff."

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "gh...hg"
		/*  1 */ "hhhhhhh"
		/*  2 */ ".hgggh."
		/*  3 */ ".hgggh."
		/*  4 */ ".hgggh."
		/*  5 */ "hhhhhhh"
		/*  6 */ "gh...hg"

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "......."
		/*  3 */ "...h..."
		/*  4 */ "......."
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
	},  // HouseSmall



	////////////////////////////////////////////////////////////////////////////////
	// HouseSmallDblWithDoor:
	// The data has been exported from the gallery Plains, area index 113, ID 265, created by Aloe_vera
	{
		// Size:
		11, 6, 7,  // SizeX = 11, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		11, 5, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b: 17: 9\n"  /* tree */
		"c: 17: 1\n"  /* tree */
		"d: 35: 0\n"  /* wool */
		"e: 64: 7\n"  /* wooddoorblock */
		"f:171:12\n"  /* carpet */
		"g:135: 1\n"  /* 135 */
		"h:126: 2\n"  /* woodenslab */
		"i:135: 2\n"  /* 135 */
		"j: 50: 4\n"  /* torch */
		"k: 64:12\n"  /* wooddoorblock */
		"l: 85: 0\n"  /* fence */
		"m: 19: 0\n"  /* sponge */
		"n: 44: 8\n"  /* step */
		"o: 43: 0\n"  /* doubleslab */
		"p: 44: 0\n"  /* step */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "mmmmmmmmmmm"
		/*  1 */ "maaaaaaaaam"
		/*  2 */ "maaaabaaaam"
		/*  3 */ "maaaabaaaam"
		/*  4 */ "maaaabaaaam"
		/*  5 */ "maaaaaaaaam"
		/*  6 */ "mmmmmmmmmmm"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".cdedcdddc."
		/*  2 */ ".dfff.fffd."
		/*  3 */ ".dgffdfhfd."
		/*  4 */ ".diifdfffd."
		/*  5 */ ".cdddcdddc."
		/*  6 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ ".j...j...j."
		/*  1 */ ".cdkdclllc."
		/*  2 */ ".d.......l."
		/*  3 */ ".l...l...l."
		/*  4 */ ".d...l...l."
		/*  5 */ ".clllclllc."
		/*  6 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ ".nnnnnnnnn."
		/*  1 */ "ncdddcdddcn"
		/*  2 */ "nd...d...dn"
		/*  3 */ "nd...d...dn"
		/*  4 */ "nd...d...dn"
		/*  5 */ "ncdddcdddcn"
		/*  6 */ ".nnnnnnnnn."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "op.......po"
		/*  1 */ "ppppppppppp"
		/*  2 */ ".pooooooop."
		/*  3 */ ".ponndnnop."
		/*  4 */ ".pooooooop."
		/*  5 */ "ppppppppppp"
		/*  6 */ "op.......po"

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "...ppppp..."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "...........",

		// Connectors:
		"-1: 3, 1, -1: 2\n"  /* Type -1, direction Z- */,

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
	},  // HouseSmallDblWithDoor



	////////////////////////////////////////////////////////////////////////////////
	// HouseSmallDouble:
	// The data has been exported from the gallery Plains, area index 72, ID 135, created by Aloe_vera
	{
		// Size:
		11, 6, 7,  // SizeX = 11, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		11, 5, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b: 17: 1\n"  /* tree */
		"c: 35: 0\n"  /* wool */
		"d:171:12\n"  /* carpet */
		"e:135: 1\n"  /* 135 */
		"f:126: 2\n"  /* woodenslab */
		"g:135: 2\n"  /* 135 */
		"h: 50: 4\n"  /* torch */
		"i: 85: 0\n"  /* fence */
		"j: 44: 8\n"  /* step */
		"k: 43: 0\n"  /* doubleslab */
		"l: 44: 0\n"  /* step */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "mmmmmmmmmmm"
		/*  1 */ "maaaaaaaaam"
		/*  2 */ "maaaaaaaaam"
		/*  3 */ "maaaaaaaaam"
		/*  4 */ "maaaaaaaaam"
		/*  5 */ "maaaaaaaaam"
		/*  6 */ "mmmmmmmmmmm"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".bcc.bcccb."
		/*  2 */ ".cddd.dddc."
		/*  3 */ ".ceddcdfdc."
		/*  4 */ ".cggdcdddc."
		/*  5 */ ".bcccbcccb."
		/*  6 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ ".h...h...h."
		/*  1 */ ".bii.biiib."
		/*  2 */ ".c.......c."
		/*  3 */ ".i...i...i."
		/*  4 */ ".c...i...c."
		/*  5 */ ".biiibiiib."
		/*  6 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ ".jjjjjjjjj."
		/*  1 */ "jbiiibiiibj"
		/*  2 */ "jc.......cj"
		/*  3 */ "jc...c...cj"
		/*  4 */ "jc...c...cj"
		/*  5 */ "jbcccbcccbj"
		/*  6 */ ".jjjjjjjjj."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "kl...l...lk"
		/*  1 */ "lllllllllll"
		/*  2 */ ".lkkklkkkl."
		/*  3 */ ".lkjklkkkl."
		/*  4 */ ".lkkklkkkl."
		/*  5 */ "lllllllllll"
		/*  6 */ "kl...l...lk"

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "...l...l..."
		/*  4 */ "..........."
		/*  5 */ "..........."
		/*  6 */ "...........",

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
	},  // HouseSmallDouble



	////////////////////////////////////////////////////////////////////////////////
	// HouseSmallWithDoor:
	// The data has been exported from the gallery Plains, area index 112, ID 264, created by Aloe_vera
	{
		// Size:
		7, 6, 7,  // SizeX = 7, SizeY = 6, SizeZ = 7

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		7, 5, 7,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b: 17: 1\n"  /* tree */
		"c: 35: 0\n"  /* wool */
		"d: 64: 7\n"  /* wooddoorblock */
		"e: 50: 4\n"  /* torch */
		"f: 64:12\n"  /* wooddoorblock */
		"g: 85: 0\n"  /* fence */
		"h: 44: 8\n"  /* step */
		"i: 43: 0\n"  /* doubleslab */
		"j: 44: 0\n"  /* step */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "maaaaam"
		/*  2 */ "maaaaam"
		/*  3 */ "maaaaam"
		/*  4 */ "maaaaam"
		/*  5 */ "maaaaam"
		/*  6 */ "mmmmmmm"

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".bcdcb."
		/*  2 */ ".c...c."
		/*  3 */ ".c...c."
		/*  4 */ ".c...c."
		/*  5 */ ".bcccb."
		/*  6 */ "......."

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ ".....e."
		/*  1 */ ".bcfcb."
		/*  2 */ ".g...g."
		/*  3 */ ".g...g."
		/*  4 */ ".g...g."
		/*  5 */ ".bgggb."
		/*  6 */ "......."

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ ".hhhhh."
		/*  1 */ "hbcccbh"
		/*  2 */ "hc...ch"
		/*  3 */ "hc...ch"
		/*  4 */ "hc...ch"
		/*  5 */ "hbcccbh"
		/*  6 */ ".hhhhh."

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "ij...ji"
		/*  1 */ "jjjjjjj"
		/*  2 */ ".jiiij."
		/*  3 */ ".jiiij."
		/*  4 */ ".jiiij."
		/*  5 */ "jjjjjjj"
		/*  6 */ "ij...ji"

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "......."
		/*  3 */ "...j..."
		/*  4 */ "......."
		/*  5 */ "......."
		/*  6 */ ".......",

		// Connectors:
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
		true,
	},  // HouseSmallWithDoor



	////////////////////////////////////////////////////////////////////////////////
	// HouseWide:
	// The data has been exported from the gallery Plains, area index 64, ID 121, created by STR_Warrior
	{
		// Size:
		11, 6, 11,  // SizeX = 11, SizeY = 6, SizeZ = 11

		// Hitbox (relative to bounding box):
		-1, 0, -1,  // MinX, MinY, MinZ
		11, 5, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b: 17: 1\n"  /* tree */
		"c: 35: 0\n"  /* wool */
		"d:171: 0\n"  /* carpet */
		"e:126: 1\n"  /* woodenslab */
		"f: 64: 5\n"  /* wooddoorblock */
		"g: 85: 0\n"  /* fence */
		"h: 50: 1\n"  /* torch */
		"i: 50: 2\n"  /* torch */
		"j: 64:12\n"  /* wooddoorblock */
		"k:126:11\n"  /* woodenslab */
		"l: 17: 5\n"  /* tree */
		"m: 19: 0\n"  /* sponge */
		"n:126: 3\n"  /* woodenslab */
		"o:125: 3\n"  /* woodendoubleslab */
		"p:  5: 3\n"  /* wood */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "mmmmmmmmmmm"
		/*  1 */ "mmaaaaaaamm"
		/*  2 */ "maaaaaaaaam"
		/*  3 */ "maaaaaaaaam"
		/*  4 */ "maaaaaaaaam"
		/*  5 */ "maaaaaaaaam"
		/*  6 */ "maaaaaaaaam"
		/*  7 */ "maaaaaaaaam"
		/*  8 */ "maaaaaaaaam"
		/*  9 */ "mmaaaaaaamm"
		/* 10 */ "mmmmmmmmmmm"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..bcbcbcb.."
		/*  2 */ ".b.d.....b."
		/*  3 */ ".cded....c."
		/*  4 */ ".bded....b."
		/*  5 */ ".c.d.....c."
		/*  6 */ ".b.......b."
		/*  7 */ ".c.......c."
		/*  8 */ ".b.......b."
		/*  9 */ "..bcbfbcb.."
		/* 10 */ "..........."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..bgbgbgb.."
		/*  2 */ ".b.......b."
		/*  3 */ ".g.......g."
		/*  4 */ ".bh.....ib."
		/*  5 */ ".g.......g."
		/*  6 */ ".b.......b."
		/*  7 */ ".g.......g."
		/*  8 */ ".b.......b."
		/*  9 */ "..bgbjbgb.."
		/* 10 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "...kkkkk..."
		/*  1 */ "..bcbcbcb.."
		/*  2 */ ".b.......b."
		/*  3 */ "kc.......ck"
		/*  4 */ "kb.......bk"
		/*  5 */ "kc.......ck"
		/*  6 */ "kb.......bk"
		/*  7 */ "kc.......ck"
		/*  8 */ ".b.......b."
		/*  9 */ "..bcblbcb.."
		/* 10 */ "...kkkkk..."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ ".kn.....nk."
		/*  1 */ "konnnnnnnok"
		/*  2 */ "nnnnnnnnnnn"
		/*  3 */ ".nnpppppnn."
		/*  4 */ ".nnpkkkpnn."
		/*  5 */ ".nnpkkkpnn."
		/*  6 */ ".nnpkkkpnn."
		/*  7 */ ".nnpppppnn."
		/*  8 */ "nnnnnnnnnnn"
		/*  9 */ "kknnnnnnnok"
		/* 10 */ ".kn.....nk."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "n.........n"
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "....nnn...."
		/*  5 */ "....non...."
		/*  6 */ "....nnn...."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "n.........n",

		// Connectors:
		"-1: 5, 1, 10: 3\n"  /* Type -1, direction Z+ */,

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
	},  // HouseWide



	////////////////////////////////////////////////////////////////////////////////
	// HouseWithGarden:
	// The data has been exported from the gallery Plains, area index 67, ID 130, created by Aloe_vera
	{
		// Size:
		16, 9, 16,  // SizeX = 16, SizeY = 9, SizeZ = 16

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		16, 8, 16,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  5: 2\n"  /* wood */
		"c:  2: 0\n"  /* grass */
		"d:113: 0\n"  /* netherbrickfence */
		"e: 17: 1\n"  /* tree */
		"f: 35: 0\n"  /* wool */
		"g:126: 2\n"  /* woodenslab */
		"h: 31: 2\n"  /* tallgrass */
		"i:125: 2\n"  /* woodendoubleslab */
		"j: 38: 3\n"  /* rose */
		"k: 38: 2\n"  /* rose */
		"l: 38: 1\n"  /* rose */
		"m: 19: 0\n"  /* sponge */
		"n: 17: 2\n"  /* tree */
		"o: 50: 4\n"  /* torch */
		"p: 85: 0\n"  /* fence */
		"q:140: 0\n"  /* flowerpotblock */
		"r: 50: 3\n"  /* torch */
		"s: 44: 8\n"  /* step */
		"t: 50: 1\n"  /* torch */
		"u: 50: 2\n"  /* torch */
		"v: 43: 0\n"  /* doubleslab */
		"w: 44: 0\n"  /* step */
		"x: 18:10\n"  /* leaves */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmaammmmm"
		/*  1 */ "aabbbbbbbbbbaaam"
		/*  2 */ "aabbbbbbbbbbaaam"
		/*  3 */ "aabbbbbbbbbbaaam"
		/*  4 */ "aabbbbbbbbbbaaam"
		/*  5 */ "aabbbbbbbbbbaaam"
		/*  6 */ "aabbbbbbbbbbaaam"
		/*  7 */ "aabbbbbbbbbbaaam"
		/*  8 */ "aabbbbbbbbbbaaam"
		/*  9 */ "aabbbbbbbbbbaaam"
		/* 10 */ "aaaaaaaaaaaaaaam"
		/* 11 */ "aaaaaaaaaaaaaaam"
		/* 12 */ "aaaaaaaaaaaaaaam"
		/* 13 */ "aaaaaaaaaaaaaaam"
		/* 14 */ "aaaaaaaaaaaaaaam"
		/* 15 */ "mmmmmmmmmmmmmmmm"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmccmmmmm"
		/*  1 */ "ccbbbbbbbbbbcccm"
		/*  2 */ "ccbbbbbbbbbbcccm"
		/*  3 */ "ccbbbbbbbbbbcccm"
		/*  4 */ "ccbbbbbbbbbbcccm"
		/*  5 */ "ccbbbbbbbbbbcccm"
		/*  6 */ "ccbbbbbbbbbbcccm"
		/*  7 */ "ccbbbbbbbbbbcccm"
		/*  8 */ "ccbbbbbbbbbbcccm"
		/*  9 */ "ccbbbbbbbbbbcccm"
		/* 10 */ "cccccccccccccccm"
		/* 11 */ "cccccccccccccccm"
		/* 12 */ "cccccccccccccccm"
		/* 13 */ "cccccccccccccacm"
		/* 14 */ "cccccccccccccccm"
		/* 15 */ "mmmmmmmmmmmmmmmm"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "ddeffeffe..eddd."
		/*  2 */ "d.fbbgggg..f..d."
		/*  3 */ "d.fbgggggggf.hd."
		/*  4 */ "d.fbgggggggf..d."
		/*  5 */ "d.eggggggggehhd."
		/*  6 */ "d.fgiiggiigf.hd."
		/*  7 */ "d.fgiiggiigf..d."
		/*  8 */ "d.fggggggggf..d."
		/*  9 */ "d.efffeefffe.hd."
		/* 10 */ "d.............d."
		/* 11 */ "djhhk.jhh..hh.d."
		/* 12 */ "d.jlk.hj.h....d."
		/* 13 */ "d..jh.hh..h..nd."
		/* 14 */ "ddddddddddddddd."
		/* 15 */ "................"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "........o..o...."
		/*  1 */ "..eppeffe..e...."
		/*  2 */ "..pqq......p...."
		/*  3 */ "..pq.......p...."
		/*  4 */ "..pq.......p...."
		/*  5 */ "..e........e...."
		/*  6 */ "..p........p...."
		/*  7 */ "..p........p...."
		/*  8 */ "..p........p...."
		/*  9 */ "..epppeepppe...."
		/* 10 */ "......rr........"
		/* 11 */ "................"
		/* 12 */ "................"
		/* 13 */ ".............n.."
		/* 14 */ "................"
		/* 15 */ "................"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "..ssssssssss...."
		/*  1 */ ".seffeffeffes..."
		/*  2 */ ".sf..r.....fs..."
		/*  3 */ ".sf........fs..."
		/*  4 */ ".sf........fs..."
		/*  5 */ ".set......ues..."
		/*  6 */ ".sf........fs..."
		/*  7 */ ".sf........fs..."
		/*  8 */ ".sf........fs..."
		/*  9 */ ".sefffeefffes..."
		/* 10 */ "..ssssssssss...."
		/* 11 */ "................"
		/* 12 */ "................"
		/* 13 */ ".............n.."
		/* 14 */ "................"
		/* 15 */ "................"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ ".vw........wv..."
		/*  1 */ ".wwwwwwwwwwww..."
		/*  2 */ "..wvvvvvvvvw...."
		/*  3 */ "..wvvvvvvvvw...."
		/*  4 */ "..wvvvvvvvvw...."
		/*  5 */ "..wvvvvvvvvw...."
		/*  6 */ "..wvvvvvvvvw...."
		/*  7 */ "..wvvvvvvvvw...."
		/*  8 */ "..wvvvvvvvvw...."
		/*  9 */ ".wwwwwwwwwwww..."
		/* 10 */ ".vw........wv..."
		/* 11 */ "............xxx."
		/* 12 */ "...........xxxxx"
		/* 13 */ "...........xxnxx"
		/* 14 */ "...........xxxxx"
		/* 15 */ "............xxx."

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "....wwwwww......"
		/*  4 */ "....wvvvvw......"
		/*  5 */ "....wvvvvw......"
		/*  6 */ "....wvvvvw......"
		/*  7 */ "....wwwwww......"
		/*  8 */ "................"
		/*  9 */ "................"
		/* 10 */ "................"
		/* 11 */ "............xxx."
		/* 12 */ "...........xxxxx"
		/* 13 */ "...........xxnxx"
		/* 14 */ "...........xxxxx"
		/* 15 */ "............xxx."

		// Level 7
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "................"
		/*  4 */ "................"
		/*  5 */ "......ww........"
		/*  6 */ "................"
		/*  7 */ "................"
		/*  8 */ "................"
		/*  9 */ "................"
		/* 10 */ "................"
		/* 11 */ "................"
		/* 12 */ "............xxx."
		/* 13 */ "............xnx."
		/* 14 */ "............xx.."
		/* 15 */ "................"

		// Level 8
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "................"
		/*  4 */ "................"
		/*  5 */ "................"
		/*  6 */ "................"
		/*  7 */ "................"
		/*  8 */ "................"
		/*  9 */ "................"
		/* 10 */ "................"
		/* 11 */ "................"
		/* 12 */ ".............x.."
		/* 13 */ "............xxx."
		/* 14 */ ".............x.."
		/* 15 */ "................",

		// Connectors:
		"-1: 9, 2, 0: 2\n"  /* Type -1, direction Z- */,

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
	},  // HouseWithGarden



	////////////////////////////////////////////////////////////////////////////////
	// HouseWithSakura1:
	// The data has been exported from the gallery Plains, area index 75, ID 141, created by Aloe_vera
	{
		// Size:
		13, 7, 15,  // SizeX = 13, SizeY = 7, SizeZ = 15

		// Hitbox (relative to bounding box):
		-1, 0, 0,  // MinX, MinY, MinZ
		13, 6, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 17: 5\n"  /* tree */
		"d:  5: 2\n"  /* wood */
		"e: 17: 9\n"  /* tree */
		"f:113: 0\n"  /* netherbrickfence */
		"g: 17: 1\n"  /* tree */
		"h: 35: 0\n"  /* wool */
		"i: 31: 2\n"  /* tallgrass */
		"j: 54: 2\n"  /* chest */
		"k: 38: 6\n"  /* rose */
		"l: 38: 2\n"  /* rose */
		"m: 19: 0\n"  /* sponge */
		"n: 50: 4\n"  /* torch */
		"o: 85: 0\n"  /* fence */
		"p: 44: 8\n"  /* step */
		"q: 35: 6\n"  /* wool */
		"r: 43: 0\n"  /* doubleslab */
		"s: 44: 0\n"  /* step */,

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
		/*  9 */ "aaaaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaaaa"
		/* 11 */ "aaaaaaaaaaaaa"
		/* 12 */ "aaaaaaaaaaaaa"
		/* 13 */ "aaaaaaaaaaaaa"
		/* 14 */ "aaaaaaaaaaaaa"

		// Level 1
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "bbbbbbbbbbbbb"
		/*  1 */ "bbbbbbbbbbbbb"
		/*  2 */ "bbbaccdabbbbb"
		/*  3 */ "bbbedddebbbbb"
		/*  4 */ "bbbedddebbbbb"
		/*  5 */ "bbbedddebbbbb"
		/*  6 */ "bbbacccabbbbb"
		/*  7 */ "bbbbbbbbbbbbb"
		/*  8 */ "bbbbbbbbbbbbb"
		/*  9 */ "bbbbbbbbbbbbb"
		/* 10 */ "bbbbbbbbbbabb"
		/* 11 */ "bbbbbbbbbbbbb"
		/* 12 */ "bbbbbbbbbbbbb"
		/* 13 */ "bbbbbbbbbbbbb"
		/* 14 */ "bbbbbbbbbbbbb"

		// Level 2
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "ffff...ffffff"
		/*  1 */ "f...........f"
		/*  2 */ "f..ghh.g..i.f"
		/*  3 */ "f..h...h..i.f"
		/*  4 */ "f..h...h....f"
		/*  5 */ "fi.h..jh..i.f"
		/*  6 */ "f..ghhhg....f"
		/*  7 */ "f.........i.f"
		/*  8 */ "fii.........f"
		/*  9 */ "f.k..k.i....f"
		/* 10 */ "fl.i..i...g.f"
		/* 11 */ "f.i..i.k....f"
		/* 12 */ "f.l.k.......f"
		/* 13 */ "f.....l.....f"
		/* 14 */ "fffffffffffff"

		// Level 3
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ ".......n....."
		/*  2 */ "...goo.g....."
		/*  3 */ "...h...h....."
		/*  4 */ "...o...o....."
		/*  5 */ "...h...h....."
		/*  6 */ "...gooog....."
		/*  7 */ "............."
		/*  8 */ "............."
		/*  9 */ "............."
		/* 10 */ "..........g.."
		/* 11 */ "............."
		/* 12 */ "............."
		/* 13 */ "............."
		/* 14 */ "............."

		// Level 4
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "...ppppp....."
		/*  2 */ "..pghhhgp...."
		/*  3 */ "..ph...hp...."
		/*  4 */ "..ph...hp...."
		/*  5 */ "..ph...hp...."
		/*  6 */ "..pghhhgp...."
		/*  7 */ "...ppppp....."
		/*  8 */ "............."
		/*  9 */ "..........q.."
		/* 10 */ ".........qgq."
		/* 11 */ "..........q.."
		/* 12 */ "............."
		/* 13 */ "............."
		/* 14 */ "............."

		// Level 5
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "..rs...sr...."
		/*  2 */ "..sssssss...."
		/*  3 */ "...srrrs....."
		/*  4 */ "...srrrs....."
		/*  5 */ "...srrrs....."
		/*  6 */ "..sssssss...."
		/*  7 */ "..rs...sr...."
		/*  8 */ "............."
		/*  9 */ ".........qqq."
		/* 10 */ ".........qqq."
		/* 11 */ ".........qqq."
		/* 12 */ "............."
		/* 13 */ "............."
		/* 14 */ "............."

		// Level 6
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "............."
		/*  1 */ "............."
		/*  2 */ "............."
		/*  3 */ "............."
		/*  4 */ ".....s......."
		/*  5 */ "............."
		/*  6 */ "............."
		/*  7 */ "............."
		/*  8 */ "............."
		/*  9 */ "............."
		/* 10 */ "..........q.."
		/* 11 */ "............."
		/* 12 */ "............."
		/* 13 */ "............."
		/* 14 */ ".............",

		// Connectors:
		"-1: 5, 2, 0: 2\n"  /* Type -1, direction Z- */,

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
	},  // HouseWithSakura1



	////////////////////////////////////////////////////////////////////////////////
	// HouseWithSpa:
	// The data has been exported from the gallery Plains, area index 73, ID 139, created by Aloe_vera
	{
		// Size:
		16, 8, 14,  // SizeX = 16, SizeY = 8, SizeZ = 14

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 7, 13,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b:  3: 0\n"  /* dirt */
		"c:  2: 0\n"  /* grass */
		"d:  8: 0\n"  /* water */
		"e:135: 3\n"  /* 135 */
		"f:135: 1\n"  /* 135 */
		"g:113: 0\n"  /* netherbrickfence */
		"h: 17: 1\n"  /* tree */
		"i: 35: 0\n"  /* wool */
		"j:171:12\n"  /* carpet */
		"k: 64: 6\n"  /* wooddoorblock */
		"l:126: 2\n"  /* woodenslab */
		"m: 19: 0\n"  /* sponge */
		"n:135: 2\n"  /* 135 */
		"o: 64: 7\n"  /* wooddoorblock */
		"p: 50: 4\n"  /* torch */
		"q: 85: 0\n"  /* fence */
		"r: 64:12\n"  /* wooddoorblock */
		"s: 50: 3\n"  /* torch */
		"t: 44: 8\n"  /* step */
		"u: 43: 0\n"  /* doubleslab */
		"v: 44: 0\n"  /* step */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".aaaaaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaaaaa."
		/*  4 */ ".aaaaaaaaaaaaaa."
		/*  5 */ ".aaaaaaaaaaaaaa."
		/*  6 */ ".aaaaaaaaaaaaaa."
		/*  7 */ ".aaaaaabbbbbbbbb"
		/*  8 */ ".aaaaaabbbbbbbbb"
		/*  9 */ ".aaaaaabbbbbbbbb"
		/* 10 */ ".aaaaaabbbbbbbbb"
		/* 11 */ ".aaaaaabbbbbbbbb"
		/* 12 */ ".aaaaaabbbbbbbbb"
		/* 13 */ ".......bbbbbbbbb"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "mmmmmmmmmmmmmmmm"
		/*  1 */ "maaaaaaaaaaaaaam"
		/*  2 */ "maaaaaaaaaaaaaam"
		/*  3 */ "maaaaaaaaaaaaaam"
		/*  4 */ "maaaaaaaaaaaaaam"
		/*  5 */ "maaaaaaaaaaaaaam"
		/*  6 */ "maaaaaaaaaaaaaam"
		/*  7 */ "maaaaaaaaaaccccc"
		/*  8 */ "maaaaaaacccccccc"
		/*  9 */ "maaaaaaacccccccc"
		/* 10 */ "maaaaaaacccccccc"
		/* 11 */ "maaaaaaccccccccc"
		/* 12 */ "maaaaaaccccccccc"
		/* 13 */ "mmmmmmmccccccccc"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".aaaaaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaaaaa."
		/*  4 */ ".aaaaaaaaaaaaaa."
		/*  5 */ ".aaaaaaaaaaaaaa."
		/*  6 */ ".aaddaaaaaaaaaa."
		/*  7 */ ".aaddaaeeef....."
		/*  8 */ ".aaddaaf........"
		/*  9 */ ".aaddaaf........"
		/* 10 */ ".aaddaae........"
		/* 11 */ ".aaddaa........."
		/* 12 */ ".aaaaaa........."
		/* 13 */ "................"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".ggggghiiihiiih."
		/*  2 */ ".geee.ijjjjjjji."
		/*  3 */ ".gf...kjjjijlji."
		/*  4 */ ".gf...innjijjji."
		/*  5 */ ".g....hiiohiiih."
		/*  6 */ ".g....g........."
		/*  7 */ ".g.............."
		/*  8 */ ".g.............."
		/*  9 */ ".g.............."
		/* 10 */ ".g....g........."
		/* 11 */ ".g....g........."
		/* 12 */ ".gggggg........."
		/* 13 */ "................"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "......p...p...p."
		/*  1 */ ".g....hqqqhqqqh."
		/*  2 */ "......i.......i."
		/*  3 */ "......r...q...q."
		/*  4 */ "......i...q...i."
		/*  5 */ "......hqqrhqqqh."
		/*  6 */ "......g...s....."
		/*  7 */ "................"
		/*  8 */ "................"
		/*  9 */ "................"
		/* 10 */ "................"
		/* 11 */ "................"
		/* 12 */ ".g....g........."
		/* 13 */ "................"

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ ".tttttttttttttt."
		/*  1 */ "tggggghqqqhqqqht"
		/*  2 */ "tg....i.......it"
		/*  3 */ "tg....i...i...it"
		/*  4 */ "tg....i...i...it"
		/*  5 */ "tg....hiiihiiiht"
		/*  6 */ "tg....gtttttttt."
		/*  7 */ "tg....gt........"
		/*  8 */ "tg....gt........"
		/*  9 */ "tg....gt........"
		/* 10 */ "tg....gt........"
		/* 11 */ "tg....gt........"
		/* 12 */ "tggggggt........"
		/* 13 */ ".tttttt........."

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "uv............vu"
		/*  1 */ "vvvvvvvvvvvvvvvv"
		/*  2 */ ".vuuuuuuuuuuuuv."
		/*  3 */ ".vuuuuuutuuuuuv."
		/*  4 */ ".vuuuuuuuuuuuuv."
		/*  5 */ ".vuuuuvvvvvvvvvv"
		/*  6 */ ".vuuuuv.......vu"
		/*  7 */ ".vuuuuv........."
		/*  8 */ ".vuuuuv........."
		/*  9 */ ".vuuuuv........."
		/* 10 */ ".vuuuuv........."
		/* 11 */ ".vuuuuv........."
		/* 12 */ "vvvvvvvv........"
		/* 13 */ "uv....vu........"

		// Level 7
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "...vvvvvvvvvv..."
		/*  4 */ "...vv..........."
		/*  5 */ "...vv..........."
		/*  6 */ "...vv..........."
		/*  7 */ "...vv..........."
		/*  8 */ "...vv..........."
		/*  9 */ "...vv..........."
		/* 10 */ "...vv..........."
		/* 11 */ "................"
		/* 12 */ "................"
		/* 13 */ "................",

		// Connectors:
		"",

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
	},  // HouseWithSpa



	////////////////////////////////////////////////////////////////////////////////
	// MediumSakuraTree:
	// The data has been exported from the gallery Plains, area index 146, ID 490, created by STR_Warrior
	{
		// Size:
		7, 10, 7,  // SizeX = 7, SizeY = 10, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 9, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 31: 1\n"  /* tallgrass */
		"d: 38: 7\n"  /* rose */
		"e: 17: 1\n"  /* tree */
		"f: 38: 0\n"  /* rose */
		"g: 38: 8\n"  /* rose */
		"h: 38: 5\n"  /* rose */
		"i: 35: 6\n"  /* wool */
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
		/*  0 */ "bbbbbbb"
		/*  1 */ "bbbbbbb"
		/*  2 */ "bbbbbbb"
		/*  3 */ "bbbabbb"
		/*  4 */ "bbbbbbb"
		/*  5 */ "bbbbbbb"
		/*  6 */ "bbbbbbb"

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "mm...mm"
		/*  1 */ "m.c...m"
		/*  2 */ ".dccdc."
		/*  3 */ "..cefc."
		/*  4 */ ".ccfgh."
		/*  5 */ "m.ccc.m"
		/*  6 */ "mm...mm"

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ "m.....m"
		/*  1 */ "......."
		/*  2 */ "......."
		/*  3 */ "...e..."
		/*  4 */ "......."
		/*  5 */ "......."
		/*  6 */ "m.....m"

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "..i...."
		/*  2 */ "......."
		/*  3 */ "...e.i."
		/*  4 */ ".i....."
		/*  5 */ "......."
		/*  6 */ "......."

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "..i...."
		/*  2 */ "...i..."
		/*  3 */ "..ieii."
		/*  4 */ ".i.ii.."
		/*  5 */ "...i..."
		/*  6 */ "......."

		// Level 6
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "..ii..."
		/*  2 */ "..iii.."
		/*  3 */ ".iieii."
		/*  4 */ ".iiii.."
		/*  5 */ "..iii.."
		/*  6 */ "......."

		// Level 7
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "..iii.."
		/*  2 */ ".iiiii."
		/*  3 */ ".iieii."
		/*  4 */ ".iiiii."
		/*  5 */ "..iii.."
		/*  6 */ "......."

		// Level 8
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "...i..."
		/*  2 */ "..iiii."
		/*  3 */ ".iiiii."
		/*  4 */ "..iii.."
		/*  5 */ "...i..."
		/*  6 */ "......."

		// Level 9
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "...i..."
		/*  3 */ "..iii.."
		/*  4 */ "...i..."
		/*  5 */ "......."
		/*  6 */ ".......",

		// Connectors:
		"-1: 3, 2, 0: 2\n"  /* Type -1, direction Z- */
		"3: 6, 2, 3: 5\n"  /* Type 3, direction X+ */
		"-3: 0, 2, 3: 4\n"  /* Type -3, direction X- */,

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
	},  // MediumSakuraTree



	////////////////////////////////////////////////////////////////////////////////
	// Restaurant:
	// The data has been exported from the gallery Plains, area index 61, ID 117, created by Aloe_vera
	{
		// Size:
		15, 10, 15,  // SizeX = 15, SizeY = 10, SizeZ = 15

		// Hitbox (relative to bounding box):
		-1, 0, -1,  // MinX, MinY, MinZ
		14, 9, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b:135: 0\n"  /* 135 */
		"c:135: 2\n"  /* 135 */
		"d:135: 1\n"  /* 135 */
		"e: 17: 9\n"  /* tree */
		"f:135: 3\n"  /* 135 */
		"g: 85: 0\n"  /* fence */
		"h: 17: 1\n"  /* tree */
		"i:171: 0\n"  /* carpet */
		"j:171:12\n"  /* carpet */
		"k:126: 1\n"  /* woodenslab */
		"l: 50: 5\n"  /* torch */
		"m: 19: 0\n"  /* sponge */
		"n: 35: 0\n"  /* wool */
		"o: 50: 3\n"  /* torch */
		"p: 50: 1\n"  /* torch */
		"q: 50: 4\n"  /* torch */
		"r: 35:14\n"  /* wool */
		"s: 44: 8\n"  /* step */
		"t: 43: 0\n"  /* doubleslab */
		"u: 44: 0\n"  /* step */
		"v: 17: 5\n"  /* tree */,

		// Block data:
		// Level 0
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "mmmmaaaaaaammmm"
		/*  1 */ "maaaaaaaaaaaaam"
		/*  2 */ "maaaaaaaaaaaaam"
		/*  3 */ "maaaaaaaaaaaaam"
		/*  4 */ "aaaaaaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaaaaaa"
		/*  9 */ "aaaaaaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaaaaaa"
		/* 11 */ "maaaaaaaaaaaaam"
		/* 12 */ "maaaaaaaaaaaaam"
		/* 13 */ "maaaaaaaaaaaaam"
		/* 14 */ "mmmmaaaaaaammmm"

		// Level 1
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "....bcccccd...."
		/*  1 */ ".aaaaaaaaaaaaa."
		/*  2 */ ".aaaaaaaaaaaaa."
		/*  3 */ ".aaaaaaaaaaaaa."
		/*  4 */ "caaaaaaaaaaaaac"
		/*  5 */ "baaaaaaaaaaaaad"
		/*  6 */ "baaaaaaaaaaaaad"
		/*  7 */ "baaaaaaaaaaeaad"
		/*  8 */ "baaaaaaaaaaaaad"
		/*  9 */ "baaaaaaaaaaaaad"
		/* 10 */ "faaaaaaaaaaaaaf"
		/* 11 */ ".aaaaaaaaaaaaa."
		/* 12 */ ".aaaaaaaaaaaaa."
		/* 13 */ ".aaaaaaaaaaaaa."
		/* 14 */ "....bfffffd...."

		// Level 2
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".gggg.....gggg."
		/*  2 */ ".g...........g."
		/*  3 */ ".g.hhhhhhhhh.g."
		/*  4 */ ".g.hiiijiiih.g."
		/*  5 */ "...hikijikih..."
		/*  6 */ "...hiiijiiihg.."
		/*  7 */ "...hjjjjjjj...."
		/*  8 */ "...hiiijiiihg.."
		/*  9 */ "...hikijikih..."
		/* 10 */ ".g.hiiijiiih.g."
		/* 11 */ ".g.hhhhhhhhh.g."
		/* 12 */ ".g...........g."
		/* 13 */ ".gggg.....gggg."
		/* 14 */ "..............."

		// Level 3
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".l..g.....g..l."
		/*  2 */ "..............."
		/*  3 */ "...hnnnhnnnh..."
		/*  4 */ ".g.n.......n.g."
		/*  5 */ "...n.......n..."
		/*  6 */ "...n.......hl.."
		/*  7 */ "...h..........."
		/*  8 */ "...n.......hl.."
		/*  9 */ "...n.......n..."
		/* 10 */ ".g.n.......n.g."
		/* 11 */ "...hnnnhnnnh..."
		/* 12 */ "..............."
		/* 13 */ ".l..g.....g..l."
		/* 14 */ "..............."

		// Level 4
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "....g.....g...."
		/*  2 */ "..............."
		/*  3 */ "...hn.nhn.nh..."
		/*  4 */ ".g.n...o...n.g."
		/*  5 */ "...n.......n..."
		/*  6 */ "...n.......h..."
		/*  7 */ "...hp......e..."
		/*  8 */ "...n.......h..."
		/*  9 */ "...n.......n..."
		/* 10 */ ".g.n...q...n.g."
		/* 11 */ "...hn.nhn.nh..."
		/* 12 */ "..............."
		/* 13 */ "....g.....g...."
		/* 14 */ "..............."

		// Level 5
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "....g.....g...."
		/*  2 */ "....ggggggg...."
		/*  3 */ "...hnnnhnnnh..."
		/*  4 */ ".ggn.......ngg."
		/*  5 */ "..gn.......ng.."
		/*  6 */ "..gn.......hg.."
		/*  7 */ "..gh..r.r..ng.."
		/*  8 */ "..gn.......hg.."
		/*  9 */ "..gn.......ng.."
		/* 10 */ ".ggn.......ngg."
		/* 11 */ "...hnnnhnnnh..."
		/* 12 */ "....ggggggg...."
		/* 13 */ "....g.....g...."
		/* 14 */ "..............."

		// Level 6
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "...stuuuuuts..."
		/*  2 */ "..sttttttttts.."
		/*  3 */ ".sthvvvhvvvhts."
		/*  4 */ ".tte.......ett."
		/*  5 */ ".ute.......etu."
		/*  6 */ ".ute.......htu."
		/*  7 */ ".uth..g.g..etu."
		/*  8 */ ".ute.......htu."
		/*  9 */ ".ute.......etu."
		/* 10 */ ".tte.......ett."
		/* 11 */ ".sthvvvhvvvhts."
		/* 12 */ "..sttttttttts.."
		/* 13 */ "...stuuuuuts..."
		/* 14 */ "..............."

		// Level 7
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".stu.......uts."
		/*  2 */ ".tu.........ut."
		/*  3 */ ".u.uuuuuuuuu.u."
		/*  4 */ "...utttttttu..."
		/*  5 */ "...utttttttu..."
		/*  6 */ "...utttttttu..."
		/*  7 */ "...utttttttu..."
		/*  8 */ "...utttttttu..."
		/*  9 */ "...utttttttu..."
		/* 10 */ "...utttttttu..."
		/* 11 */ ".u.uuuuuuuuu.u."
		/* 12 */ ".tu.........ut."
		/* 13 */ ".stu.......uts."
		/* 14 */ "..............."

		// Level 8
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".u...........u."
		/*  2 */ "..............."
		/*  3 */ "..............."
		/*  4 */ "..............."
		/*  5 */ ".....uuuuu....."
		/*  6 */ ".....utttu....."
		/*  7 */ ".....utttu....."
		/*  8 */ ".....utttu....."
		/*  9 */ ".....uuuuu....."
		/* 10 */ "..............."
		/* 11 */ "..............."
		/* 12 */ "..............."
		/* 13 */ ".u...........u."
		/* 14 */ "..............."

		// Level 9
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "..............."
		/*  2 */ "..............."
		/*  3 */ "..............."
		/*  4 */ "..............."
		/*  5 */ "..............."
		/*  6 */ "..............."
		/*  7 */ ".......u......."
		/*  8 */ "..............."
		/*  9 */ "..............."
		/* 10 */ "..............."
		/* 11 */ "..............."
		/* 12 */ "..............."
		/* 13 */ "..............."
		/* 14 */ "...............",

		// Connectors:
		"-1: 14, 1, 7: 5\n"  /* Type -1, direction X+ */,

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
	},  // Restaurant



	////////////////////////////////////////////////////////////////////////////////
	// SakuraDouble:
	// The data has been exported from the gallery Plains, area index 76, ID 142, created by Aloe_vera
	{
		// Size:
		12, 8, 6,  // SizeX = 12, SizeY = 8, SizeZ = 6

		// Hitbox (relative to bounding box):
		-1, 0, -1,  // MinX, MinY, MinZ
		12, 7, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 17: 1\n"  /* tree */
		"d: 35: 6\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "aaaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaaa"

		// Level 1
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "bbbbbbbbbbbb"
		/*  1 */ "bbbbbbbbbbbb"
		/*  2 */ "bbabbbbbbbbb"
		/*  3 */ "bbbbbbbbbabb"
		/*  4 */ "bbbbbbbbbbbb"
		/*  5 */ "bbbbbbbbbbbb"

		// Level 2
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "..c........."
		/*  3 */ ".........c.."
		/*  4 */ "............"
		/*  5 */ "............"

		// Level 3
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "..c........."
		/*  3 */ ".........c.."
		/*  4 */ "............"
		/*  5 */ "............"

		// Level 4
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "..d........."
		/*  1 */ "ddddd......."
		/*  2 */ "ddcdd...ddd."
		/*  3 */ "ddddd...dcd."
		/*  4 */ "..d.....ddd."
		/*  5 */ "............"

		// Level 5
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ ".ddd........"
		/*  1 */ ".ddd....ddd."
		/*  2 */ "ddddd..ddddd"
		/*  3 */ ".ddd...ddcdd"
		/*  4 */ ".ddd...ddddd"
		/*  5 */ "........ddd."

		// Level 6
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "..d......d.."
		/*  2 */ ".ddd....ddd."
		/*  3 */ "..d....ddddd"
		/*  4 */ "........ddd."
		/*  5 */ ".........d.."

		// Level 7
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "............"
		/*  3 */ ".........d.."
		/*  4 */ "............"
		/*  5 */ "............",

		// Connectors:
		"-1: -1, 2, 2: 4\n"  /* Type -1, direction X- */
		"3: 5, 2, 6: 3\n"  /* Type 3, direction Z+ */
		"-3: 6, 2, -1: 2\n"  /* Type -3, direction Z- */
		"-3: 12, 2, 2: 5\n"  /* Type -3, direction X+ */
		"3: 12, 2, 2: 5\n"  /* Type 3, direction X+ */,

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
	},  // SakuraDouble



	////////////////////////////////////////////////////////////////////////////////
	// SakuraSmall:
	// The data has been exported from the gallery Plains, area index 145, ID 489, created by Aloe_vera
	{
		// Size:
		5, 7, 5,  // SizeX = 5, SizeY = 7, SizeZ = 5

		// Hitbox (relative to bounding box):
		-1, 0, -1,  // MinX, MinY, MinZ
		5, 6, 5,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 17: 1\n"  /* tree */
		"d: 35: 6\n"  /* wool */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   01234 */
		/*  0 */ "aaaaa"
		/*  1 */ "aaaaa"
		/*  2 */ "aaaaa"
		/*  3 */ "aaaaa"
		/*  4 */ "aaaaa"

		// Level 1
		/* z\x*   01234 */
		/*  0 */ "bbbbb"
		/*  1 */ "bbbbb"
		/*  2 */ "bbabb"
		/*  3 */ "bbbbb"
		/*  4 */ "bbbbb"

		// Level 2
		/* z\x*   01234 */
		/*  0 */ "....."
		/*  1 */ "....."
		/*  2 */ "..c.."
		/*  3 */ "....."
		/*  4 */ "....."

		// Level 3
		/* z\x*   01234 */
		/*  0 */ "....."
		/*  1 */ "....."
		/*  2 */ "..c.."
		/*  3 */ "....."
		/*  4 */ "....."

		// Level 4
		/* z\x*   01234 */
		/*  0 */ "..d.."
		/*  1 */ "ddddd"
		/*  2 */ "ddcdd"
		/*  3 */ "ddddd"
		/*  4 */ "..d.."

		// Level 5
		/* z\x*   01234 */
		/*  0 */ ".ddd."
		/*  1 */ ".ddd."
		/*  2 */ "ddddd"
		/*  3 */ ".ddd."
		/*  4 */ ".ddd."

		// Level 6
		/* z\x*   01234 */
		/*  0 */ "....."
		/*  1 */ "..d.."
		/*  2 */ ".ddd."
		/*  3 */ "..d.."
		/*  4 */ ".....",

		// Connectors:
		"-1: 2, 2, -1: 2\n"  /* Type -1, direction Z- */
		"3: 5, 2, 2: 5\n"  /* Type 3, direction X+ */
		"-3: -1, 2, 2: 4\n"  /* Type -3, direction X- */,

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
	},  // SakuraSmall
};  // g_JapaneseVillagePrefabs






const cPrefab::sDef g_JapaneseVillageStartingPrefabs[] =
{
	////////////////////////////////////////////////////////////////////////////////
	// HighTemple:
	// The data has been exported from the gallery Plains, area index 70, ID 133, created by Aloe_vera
	{
		// Size:
		11, 19, 11,  // SizeX = 11, SizeY = 19, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 18, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  5: 2\n"  /* wood */
		"b:135: 0\n"  /* 135 */
		"c:135: 2\n"  /* 135 */
		"d:135: 1\n"  /* 135 */
		"e: 17: 9\n"  /* tree */
		"f:135: 3\n"  /* 135 */
		"g: 85: 0\n"  /* fence */
		"h: 17: 1\n"  /* tree */
		"i:171: 0\n"  /* carpet */
		"j: 50: 5\n"  /* torch */
		"k: 35: 0\n"  /* wool */
		"l: 17: 5\n"  /* tree */
		"m: 19: 0\n"  /* sponge */
		"n:124: 0\n"  /* redstonelampon */
		"o: 69: 9\n"  /* lever */
		"p: 44: 8\n"  /* step */
		"q: 43: 0\n"  /* doubleslab */
		"r: 44: 0\n"  /* step */
		"s: 50: 4\n"  /* torch */
		"t: 50: 1\n"  /* torch */
		"u: 50: 3\n"  /* torch */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "mmmaaaaammm"
		/*  1 */ "maaaaaaaaam"
		/*  2 */ "maaaaaaaaam"
		/*  3 */ "aaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaa"
		/*  8 */ "maaaaaaaaam"
		/*  9 */ "maaaaaaaaam"
		/* 10 */ "mmmaaaaammm"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "...bcccd..."
		/*  1 */ ".aaaaaaaaa."
		/*  2 */ ".aaaaaaaaa."
		/*  3 */ "caaaaaaaaac"
		/*  4 */ "baaaaaaaaad"
		/*  5 */ "baaeaaaaaad"
		/*  6 */ "baaaaaaaaad"
		/*  7 */ "faaaaaaaaaf"
		/*  8 */ ".aaaaaaaaa."
		/*  9 */ ".aaaaaaaaa."
		/* 10 */ "...bfffd..."

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".ggg...ggg."
		/*  2 */ ".g.......g."
		/*  3 */ ".g.hhhhh.g."
		/*  4 */ "...hiiih..."
		/*  5 */ "....iiih..."
		/*  6 */ "...hiiih..."
		/*  7 */ ".g.hhhhh.g."
		/*  8 */ ".g.......g."
		/*  9 */ ".ggg...ggg."
		/* 10 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".j.g...g.j."
		/*  2 */ "..........."
		/*  3 */ ".g.kkhkk.g."
		/*  4 */ "...h...k..."
		/*  5 */ ".......h..."
		/*  6 */ "...h...k..."
		/*  7 */ ".g.kkhkk.g."
		/*  8 */ "..........."
		/*  9 */ ".j.g...g.j."
		/* 10 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "...g...g..."
		/*  2 */ "..........."
		/*  3 */ ".g.kkhkk.g."
		/*  4 */ "...h...k..."
		/*  5 */ "...k...h..."
		/*  6 */ "...h...k..."
		/*  7 */ ".g.kkhkk.g."
		/*  8 */ "..........."
		/*  9 */ "...g...g..."
		/* 10 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "...g...g..."
		/*  2 */ "...ggggg..."
		/*  3 */ ".gghlhlhgg."
		/*  4 */ "..ge...eg.."
		/*  5 */ "..ge.nohg.."
		/*  6 */ "..ge...eg.."
		/*  7 */ ".gghlhlhgg."
		/*  8 */ "...ggggg..."
		/*  9 */ "...g...g..."
		/* 10 */ "..........."

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..pqrrrqp.."
		/*  2 */ ".pqqqqqqqp."
		/*  3 */ ".qqhkkkhqq."
		/*  4 */ ".rqkhhhkqr."
		/*  5 */ ".rqkhhhkqr."
		/*  6 */ ".rqkhhhkqr."
		/*  7 */ ".qqhkkkhqq."
		/*  8 */ ".pqqqqqqqp."
		/*  9 */ "..pqrrrqp.."
		/* 10 */ "..........."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".qr.....rq."
		/*  2 */ ".........r."
		/*  3 */ "...hhhhh..."
		/*  4 */ "...hiiih..."
		/*  5 */ "....iiih..."
		/*  6 */ "...hiiih..."
		/*  7 */ "...hhhhh..."
		/*  8 */ ".r.......r."
		/*  9 */ ".qr.....rq."
		/* 10 */ "..........."

		// Level 8
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "...kkhkk..."
		/*  4 */ "...h...k..."
		/*  5 */ ".......h..."
		/*  6 */ "...h...k..."
		/*  7 */ "...kkhkk..."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 9
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ ".....s....."
		/*  3 */ "...kkhkk..."
		/*  4 */ "...h...k..."
		/*  5 */ "...k...ht.."
		/*  6 */ "...h...k..."
		/*  7 */ "...kkhkk..."
		/*  8 */ ".....u....."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 10
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "...ggggg..."
		/*  3 */ "..ghlhlhg.."
		/*  4 */ "..ge...eg.."
		/*  5 */ "..ge.nohg.."
		/*  6 */ "..ge...eg.."
		/*  7 */ "..ghlhlhg.."
		/*  8 */ "...ggggg..."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 11
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..prrrrrp.."
		/*  2 */ ".pqqqqqqqp."
		/*  3 */ ".qqhkkkhqq."
		/*  4 */ ".rqkhhhkqr."
		/*  5 */ ".rqkhhhkqr."
		/*  6 */ ".rqkhhhkqr."
		/*  7 */ ".qqhkkkhqr."
		/*  8 */ ".pqqqqqqqp."
		/*  9 */ "..pqrrrqp.."
		/* 10 */ "..........."

		// Level 12
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".qr.....rq."
		/*  2 */ ".r.......r."
		/*  3 */ "...hhhhh..."
		/*  4 */ "...hiiih..."
		/*  5 */ "....iiih..."
		/*  6 */ "...hiiih..."
		/*  7 */ "...hhhhh..."
		/*  8 */ ".r.......r."
		/*  9 */ ".qr.....rq."
		/* 10 */ "..........."

		// Level 13
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "...kkhkk..."
		/*  4 */ "...h...k..."
		/*  5 */ ".......h..."
		/*  6 */ "...h...k..."
		/*  7 */ "...kkhkk..."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 14
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ ".....s....."
		/*  3 */ "...kkhkk..."
		/*  4 */ "...h...k..."
		/*  5 */ "...k...ht.."
		/*  6 */ "...h...k..."
		/*  7 */ "...kkhkk..."
		/*  8 */ ".....u....."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 15
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "...ggggg..."
		/*  3 */ "..ghlhlhg.."
		/*  4 */ "..ge...eg.."
		/*  5 */ "..ge.nohg.."
		/*  6 */ "..ge...eg.."
		/*  7 */ "..ghlhlhg.."
		/*  8 */ "...ggggg..."
		/*  9 */ "..........."
		/* 10 */ "..........."

		// Level 16
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..pqrrrqp.."
		/*  2 */ ".pqqqqqqqp."
		/*  3 */ ".qqrrrrrqq."
		/*  4 */ ".rqrrrrrqr."
		/*  5 */ ".rqrrrrrqr."
		/*  6 */ ".rqrrrrrqr."
		/*  7 */ ".qqrrrrrqq."
		/*  8 */ ".pqqqqqqqp."
		/*  9 */ "..pqrrrqp.."
		/* 10 */ "..........."

		// Level 17
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".qr.....rq."
		/*  2 */ ".rr.....rr."
		/*  3 */ "...rrrrr..."
		/*  4 */ "...rqqqr..."
		/*  5 */ "...rqqqr..."
		/*  6 */ "...rqqqr..."
		/*  7 */ "...rrrrr..."
		/*  8 */ ".rr.....rr."
		/*  9 */ ".qr.....rq."
		/* 10 */ "..........."

		// Level 18
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "..........."
		/*  4 */ "..........."
		/*  5 */ ".....r....."
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "..........."
		/* 10 */ "...........",

		// Connectors:
		"2: 0, 1, 5: 4\n"  /* Type 2, direction X- */
		"2: 5, 1, 0: 2\n"  /* Type 2, direction Z- */
		"2: 10, 1, 5: 5\n"  /* Type 2, direction X+ */
		"2: 5, 1, 10: 3\n"  /* Type 2, direction Z+ */,

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
	},  // HighTemple



	////////////////////////////////////////////////////////////////////////////////
	// Well:
	// The data has been exported from the gallery Plains, area index 143, ID 487, created by STR_Warrior
	{
		// Size:
		7, 14, 7,  // SizeX = 7, SizeY = 14, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 13, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b:  4: 0\n"  /* cobblestone */
		"c:  8: 0\n"  /* water */
		"d: 13: 0\n"  /* gravel */
		"e: 67: 1\n"  /* stairs */
		"f: 67: 2\n"  /* stairs */
		"g: 67: 0\n"  /* stairs */
		"h: 67: 3\n"  /* stairs */
		"i: 85: 0\n"  /* fence */
		"j: 44: 8\n"  /* step */
		"k: 44: 0\n"  /* step */
		"l: 43: 0\n"  /* doubleslab */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "maaaaam"
		/*  2 */ "maaaaam"
		/*  3 */ "maaaaam"
		/*  4 */ "maaaaam"
		/*  5 */ "maaaaam"
		/*  6 */ "mmmmmmm"

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "mbbbbbm"
		/*  2 */ "mbcc.bm"
		/*  3 */ "mbcccbm"
		/*  4 */ "mbcccbm"
		/*  5 */ "mbbbbbm"
		/*  6 */ "mmmmmmm"

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "mbbbbbm"
		/*  2 */ "mbcccbm"
		/*  3 */ "mbcccbm"
		/*  4 */ "mbcccbm"
		/*  5 */ "mbbbbbm"
		/*  6 */ "mmmmmmm"

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "mbbbbbm"
		/*  2 */ "mbcccbm"
		/*  3 */ "mbcccbm"
		/*  4 */ "mbcccbm"
		/*  5 */ "mbbbbbm"
		/*  6 */ "mmmmmmm"

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "mbbbbbm"
		/*  2 */ "mbcccbm"
		/*  3 */ "mbcccbm"
		/*  4 */ "mbcccbm"
		/*  5 */ "mbbbbbm"
		/*  6 */ "mmmmmmm"

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "mbbbbbm"
		/*  2 */ "mbcccbm"
		/*  3 */ "mbcccbm"
		/*  4 */ "mbcccbm"
		/*  5 */ "mbbbbbm"
		/*  6 */ "mmmmmmm"

		// Level 6
		/* z\x*   0123456 */
		/*  0 */ "mmmmmmm"
		/*  1 */ "mbbbbbm"
		/*  2 */ "mbcccbm"
		/*  3 */ "mbcccbm"
		/*  4 */ "mbcccbm"
		/*  5 */ "mbbbbbm"
		/*  6 */ "mmmmmmm"

		// Level 7
		/* z\x*   0123456 */
		/*  0 */ "mmbbbmm"
		/*  1 */ "mbbbbbm"
		/*  2 */ "bbcccbb"
		/*  3 */ "bbcccbb"
		/*  4 */ "bbcccbb"
		/*  5 */ "mbbbbbm"
		/*  6 */ "mmbbbmm"

		// Level 8
		/* z\x*   0123456 */
		/*  0 */ "mmdddmm"
		/*  1 */ "mbbbbbm"
		/*  2 */ "dbcccbd"
		/*  3 */ "dbcccbd"
		/*  4 */ "dbcccbd"
		/*  5 */ "mbbbbbm"
		/*  6 */ "mmdddmm"

		// Level 9
		/* z\x*   0123456 */
		/*  0 */ "mm...mm"
		/*  1 */ "mbefgbm"
		/*  2 */ ".h...h."
		/*  3 */ ".g...e."
		/*  4 */ ".f...f."
		/*  5 */ "mbehgbm"
		/*  6 */ "mm...mm"

		// Level 10
		/* z\x*   0123456 */
		/*  0 */ "mm...mm"
		/*  1 */ "mi...im"
		/*  2 */ "......."
		/*  3 */ "......."
		/*  4 */ "......."
		/*  5 */ "mi...im"
		/*  6 */ "mm...mm"

		// Level 11
		/* z\x*   0123456 */
		/*  0 */ "mm...mm"
		/*  1 */ "mi...im"
		/*  2 */ "......."
		/*  3 */ "......."
		/*  4 */ "......."
		/*  5 */ "mi...im"
		/*  6 */ "mm...mm"

		// Level 12
		/* z\x*   0123456 */
		/*  0 */ "mjkkkjm"
		/*  1 */ "jlllllj"
		/*  2 */ "klllllk"
		/*  3 */ "klllllk"
		/*  4 */ "klllllk"
		/*  5 */ "jlllllj"
		/*  6 */ "mjkkkjm"

		// Level 13
		/* z\x*   0123456 */
		/*  0 */ "k.....k"
		/*  1 */ "......."
		/*  2 */ "..kkk.."
		/*  3 */ "..klk.."
		/*  4 */ "..kkk.."
		/*  5 */ "......."
		/*  6 */ "k.....k",

		// Connectors:
		"2: 0, 9, 3: 4\n"  /* Type 2, direction X- */
		"2: 3, 9, 0: 2\n"  /* Type 2, direction Z- */
		"2: 6, 9, 3: 5\n"  /* Type 2, direction X+ */
		"2: 3, 9, 6: 3\n"  /* Type 2, direction Z+ */,

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

const size_t g_JapaneseVillagePrefabsCount = ARRAYCOUNT(g_JapaneseVillagePrefabs);

const size_t g_JapaneseVillageStartingPrefabsCount = ARRAYCOUNT(g_JapaneseVillageStartingPrefabs);

