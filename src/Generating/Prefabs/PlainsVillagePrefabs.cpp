
// PlainsVillagePrefabs.cpp

// Defines the prefabs in the group PlainsVillage

// NOTE: This file has been generated automatically by GalExport!
// Any manual changes will be overwritten by the next automatic export!

#include "Globals.h"
#include "PlainsVillagePrefabs.h"





const cPrefab::sDef g_PlainsVillagePrefabs[] =
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BigPlantBed:
	// The data has been exported from the gallery Plains, area index 26, ID 70, created by Taugrammaton
	{
		// Size:
		13, 4, 12,  // SizeX = 13, SizeY = 4, SizeZ = 12

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		12, 3, 11,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  5: 0\n"  /* wood */
		"c: 13: 0\n"  /* gravel */
		"d: 17: 0\n"  /* tree */
		"e: 60: 7\n"  /* tilleddirt */
		"f:  8: 0\n"  /* water */
		"g: 85: 0\n"  /* fence */
		"h: 59: 7\n"  /* crops */
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
		/*  9 */ "aaaaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaaaa"
		/* 11 */ "aaaaaaaaaaaaa"

		// Level 1
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "bbbbbbbbbbbbb"
		/*  1 */ "bcccccccccccb"
		/*  2 */ "bcccccccccccb"
		/*  3 */ "bcccccccccccb"
		/*  4 */ "bcccccccccccb"
		/*  5 */ "bcccccccccccb"
		/*  6 */ "bcccccccccccb"
		/*  7 */ "bcccccccccccb"
		/*  8 */ "bcccccccccccb"
		/*  9 */ "bcccccccccccb"
		/* 10 */ "bcccccccccccb"
		/* 11 */ "bbbbbbbbbbbbb"

		// Level 2
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "ddddddddddddd"
		/*  1 */ "deefeefeefeed"
		/*  2 */ "deefeefeefeed"
		/*  3 */ "deefeefeefeed"
		/*  4 */ "deefeefeefeed"
		/*  5 */ "deefeefeefeed"
		/*  6 */ "deefeefeefeed"
		/*  7 */ "deefeefeefeed"
		/*  8 */ "deefeefeefeed"
		/*  9 */ "deefeefeefeed"
		/* 10 */ "deefeefeefeed"
		/* 11 */ "ddddddddddddd"

		// Level 3
		/* z\x*             111 */
		/*    *   0123456789012 */
		/*  0 */ "g..g..g..g..g"
		/*  1 */ "ghh.h..hh.hhg"
		/*  2 */ "ghh..h.hh.hhg"
		/*  3 */ "ghh.h..h..hhg"
		/*  4 */ "ghh.hh.h..hhg"
		/*  5 */ "ghh.h..hh.hhg"
		/*  6 */ "ghh.hh.hh.hhg"
		/*  7 */ "ghh....h..hhg"
		/*  8 */ "ghh..h....hhg"
		/*  9 */ "ghh.....h.hhg"
		/* 10 */ "ghh.hh.h..hhg"
		/* 11 */ "g..g..g..g..g",

		// Connectors:
		"-1: 7, 1, 11: 3\n"  /* Type -1, direction Z+ */,

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
	},  // BigPlantBed



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CobbleHouse10x5Library:
	// The data has been exported from the gallery Plains, area index 23, ID 66, created by xoft
	{
		// Size:
		12, 8, 7,  // SizeX = 12, SizeY = 8, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		11, 7, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 64: 7\n"  /* wooddoorblock */
		"h: 53: 3\n"  /* woodstairs */
		"i: 53: 1\n"  /* woodstairs */
		"j: 85: 0\n"  /* fence */
		"k: 53: 0\n"  /* woodstairs */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n:102: 0\n"  /* glasspane */
		"o: 64:12\n"  /* wooddoorblock */
		"p: 50: 3\n"  /* torch */
		"q: 72: 0\n"  /* woodplate */
		"r: 50: 4\n"  /* torch */
		"s: 53: 7\n"  /* woodstairs */
		"t: 47: 0\n"  /* bookshelf */
		"u: 50: 1\n"  /* torch */
		"v: 50: 2\n"  /* torch */
		"w: 53: 6\n"  /* woodstairs */
		"x:  5: 0\n"  /* wood */,

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
		/*  6 */ "aaaaaaaaaaaa"

		// Level 1
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "bbbbbbbaaabb"
		/*  1 */ "baaaaaaaaaab"
		/*  2 */ "baaaaaaaaaab"
		/*  3 */ "baaaaaaaaaab"
		/*  4 */ "baaaaaaaaaab"
		/*  5 */ "baaaaaaaaaab"
		/*  6 */ "bbbbbbbbbbbb"

		// Level 2
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ ".......cde.."
		/*  1 */ ".ffffffffff."
		/*  2 */ ".ffffffffff."
		/*  3 */ ".ffffffffff."
		/*  4 */ ".ffffffffff."
		/*  5 */ ".ffffffffff."
		/*  6 */ "............"

		// Level 3
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ ".fffffffgff."
		/*  2 */ ".fh.ijk...f."
		/*  3 */ ".fj.......f."
		/*  4 */ ".fl.ijkijkf."
		/*  5 */ ".ffffffffff."
		/*  6 */ "............"

		// Level 4
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ ".fnnfnnfoff."
		/*  2 */ ".n..pq.p.pn."
		/*  3 */ ".nq.......n."
		/*  4 */ ".n..rq.rq.n."
		/*  5 */ ".fnnfnnfnnf."
		/*  6 */ "............"

		// Level 5
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "llllllllllll"
		/*  1 */ "sffffffffffs"
		/*  2 */ ".fttttttttf."
		/*  3 */ ".fu......vf."
		/*  4 */ ".fttttttttf."
		/*  5 */ "wffffffffffw"
		/*  6 */ "hhhhhhhhhhhh"

		// Level 6
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "llllllllllll"
		/*  2 */ "sxxxxxxxxxxs"
		/*  3 */ ".xxxxxxxxxx."
		/*  4 */ "wxxxxxxxxxxw"
		/*  5 */ "hhhhhhhhhhhh"
		/*  6 */ "............"

		// Level 7
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "llllllllllll"
		/*  3 */ "xxxxxxxxxxxx"
		/*  4 */ "hhhhhhhhhhhh"
		/*  5 */ "............"
		/*  6 */ "............",

		// Connectors:
		"-1: 8, 2, 0: 2\n"  /* Type -1, direction Z- */,

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
	},  // CobbleHouse10x5Library



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// DoublePlantBed:
	// The data has been exported from the gallery Plains, area index 5, ID 20, created by tonibm1999
	{
		// Size:
		15, 2, 9,  // SizeX = 15, SizeY = 2, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		14, 1, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 17: 0\n"  /* tree */
		"b: 60: 7\n"  /* tilleddirt */
		"c:  8: 0\n"  /* water */
		"d: 50: 5\n"  /* torch */
		"e: 59: 7\n"  /* crops */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 0
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "aaaaaaamaaaaaaa"
		/*  1 */ "abbcbbamabbcbba"
		/*  2 */ "abbcbbamabbcbba"
		/*  3 */ "abbcbbamabbcbba"
		/*  4 */ "abbcbbamabbcbba"
		/*  5 */ "abbcbbamabbcbba"
		/*  6 */ "abbcbbamabbcbba"
		/*  7 */ "abbcbbamabbcbba"
		/*  8 */ "aaaaaaamaaaaaaa"

		// Level 1
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "dmmmmmdmdmmmmmd"
		/*  1 */ "meemeemmmeemeem"
		/*  2 */ "memmmemmmeemeem"
		/*  3 */ "memmmmmmmeemeem"
		/*  4 */ "meemmemmmeemeem"
		/*  5 */ "meemmemmmeemeem"
		/*  6 */ "mmemmemmmeemeem"
		/*  7 */ "mmememmmmeemeem"
		/*  8 */ "dmmmmmdmdmmmmmd",

		// Connectors:
		"-1: 7, 0, 8: 3\n"  /* Type -1, direction Z+ */,

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
	},  // DoublePlantBed



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Forge:
	// The data has been exported from the gallery Plains, area index 51, ID 102, created by Aloe_vera
	{
		// Size:
		12, 10, 11,  // SizeX = 12, SizeY = 10, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		11, 9, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 6\n"  /* wooddoorblock */
		"j: 10: 0\n"  /* lava */
		"k: 54: 2\n"  /* chest */
		"l: 61: 2\n"  /* furnace */
		"m: 19: 0\n"  /* sponge */
		"n:102: 0\n"  /* glasspane */
		"o: 64:12\n"  /* wooddoorblock */
		"p:139: 0\n"  /* cobblestonewall */
		"q:101: 0\n"  /* ironbars */
		"r: 53: 2\n"  /* woodstairs */
		"s: 53: 7\n"  /* woodstairs */
		"t: 50: 2\n"  /* torch */
		"u: 50: 1\n"  /* torch */
		"v: 53: 6\n"  /* woodstairs */
		"w: 53: 3\n"  /* woodstairs */
		"x: 43: 0\n"  /* doubleslab */
		"y: 44: 0\n"  /* step */,

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
		/*  6 */ "aaaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaaa"
		/*  9 */ "aaaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaaa"

		// Level 1
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "bbbbbaaaaabb"
		/*  1 */ "baaaaaaaaabb"
		/*  2 */ "baaaaaaaaabb"
		/*  3 */ "baaaaaaaaaaa"
		/*  4 */ "baaaaaaaaaaa"
		/*  5 */ "baaaaaaaaaaa"
		/*  6 */ "baaaaaaaaaaa"
		/*  7 */ "baaaaaaaaaaa"
		/*  8 */ "baaaaabbbbbb"
		/*  9 */ "baaaaabbbbbb"
		/* 10 */ "bbbbbbbbbbbb"

		// Level 2
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ ".....cddde.."
		/*  1 */ ".ffffffffe.."
		/*  2 */ ".ffffffffe.."
		/*  3 */ ".fffffffffff"
		/*  4 */ ".fffffffffff"
		/*  5 */ ".fffffffffff"
		/*  6 */ ".fffffffffff"
		/*  7 */ ".fffffffffff"
		/*  8 */ ".fffff.mmmmm"
		/*  9 */ ".fffff.mmmmm"
		/* 10 */ ".......mmmmm"

		// Level 3
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ ".ghhhg......"
		/*  2 */ ".h...i......"
		/*  3 */ ".h...gf..fff"
		/*  4 */ ".h...h...fjf"
		/*  5 */ ".h...h...fjf"
		/*  6 */ ".h...hkllfjf"
		/*  7 */ ".h...gffffff"
		/*  8 */ ".h...h.mmmmm"
		/*  9 */ ".ghhhg.mmmmm"
		/* 10 */ ".......mmmmm"

		// Level 4
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ ".gnnng......"
		/*  2 */ ".n...o......"
		/*  3 */ ".n...gp..p.f"
		/*  4 */ ".n...n.....q"
		/*  5 */ ".h...n.....q"
		/*  6 */ ".n...n.....q"
		/*  7 */ ".n...gfqqqqf"
		/*  8 */ ".n...h.mmmmm"
		/*  9 */ ".gnnng.mmmmm"
		/* 10 */ ".......mmmmm"

		// Level 5
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "rrrrrrr....."
		/*  1 */ "shhhhhs....."
		/*  2 */ ".h...h......"
		/*  3 */ ".h..thf..fqf"
		/*  4 */ ".h...h...q.f"
		/*  5 */ ".h...h...q.f"
		/*  6 */ ".hu..h...q.f"
		/*  7 */ ".h...hffffff"
		/*  8 */ ".h...h.mmmmm"
		/*  9 */ "vhhhhhvmmmmm"
		/* 10 */ "wwwwwwwmmmmm"

		// Level 6
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "rrrrrrr....."
		/*  2 */ "shhhhhs....."
		/*  3 */ ".h...hxxxxxx"
		/*  4 */ ".h...hxyyyyx"
		/*  5 */ ".h...hxyyyyx"
		/*  6 */ ".h...hxyyyyx"
		/*  7 */ ".h...hxxxxxx"
		/*  8 */ "vhhhhhvmmmmm"
		/*  9 */ "wwwwwwwmmmmm"
		/* 10 */ ".......mmmmm"

		// Level 7
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "rrrrrrr....."
		/*  3 */ "shhhhhs....."
		/*  4 */ ".h...h......"
		/*  5 */ ".h...h......"
		/*  6 */ ".h...h......"
		/*  7 */ "vhhhhhv....."
		/*  8 */ "wwwwwwwmmmmm"
		/*  9 */ ".......mmmmm"
		/* 10 */ ".......mmmmm"

		// Level 8
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "............"
		/*  3 */ "rrrrrrr....."
		/*  4 */ "shhhhhs....."
		/*  5 */ ".h...h......"
		/*  6 */ "vhhhhhv....."
		/*  7 */ "wwwwwww....."
		/*  8 */ ".......mmmmm"
		/*  9 */ ".......mmmmm"
		/* 10 */ ".......mmmmm"

		// Level 9
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "............"
		/*  3 */ "............"
		/*  4 */ "rrrrrrr....."
		/*  5 */ "hhhhhhh....."
		/*  6 */ "wwwwwww....."
		/*  7 */ "............"
		/*  8 */ ".......mmmmm"
		/*  9 */ ".......mmmmm"
		/* 10 */ ".......mmmmm",

		// Connectors:
		"-1: 7, 2, -1: 2\n"  /* Type -1, direction Z- */,

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
	},  // Forge



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SinglePlantBed:
	// The data has been exported from the gallery Plains, area index 17, ID 60, created by Aloe_vera
	{
		// Size:
		10, 3, 7,  // SizeX = 10, SizeY = 3, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		9, 2, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b: 17: 0\n"  /* tree */
		"c: 60: 7\n"  /* tilleddirt */
		"d:  8: 0\n"  /* water */
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
		/*  0 */ "bbbbbbbbbb"
		/*  1 */ "bccccccccb"
		/*  2 */ "bccccccccb"
		/*  3 */ "bddddddddb"
		/*  4 */ "bccccccccb"
		/*  5 */ "bccccccccb"
		/*  6 */ "bbbbbbbbbb"

		// Level 2
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ ".........."
		/*  1 */ ".eeeeeeee."
		/*  2 */ ".eeeeeeee."
		/*  3 */ ".........."
		/*  4 */ ".eeeeeeee."
		/*  5 */ ".eeeeeeee."
		/*  6 */ "..........",

		// Connectors:
		"-1: 9, 1, 3: 5\n"  /* Type -1, direction X+ */,

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
	},  // SinglePlantBed



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenChurchMid:
	// The data has been exported from the gallery Plains, area index 58, ID 109, created by Aloe_vera
	{
		// Size:
		7, 16, 13,  // SizeX = 7, SizeY = 16, SizeZ = 13

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 15, 12,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"A: 54: 2\n"  /* chest */
		"B: 50: 4\n"  /* torch */
		"C: 85: 0\n"  /* fence */
		"D:126: 8\n"  /* woodenslab */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j: 65: 3\n"  /* ladder */
		"k: 53: 3\n"  /* woodstairs */
		"l: 53: 7\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 64:12\n"  /* wooddoorblock */
		"o:102: 0\n"  /* glasspane */
		"p: 50: 1\n"  /* torch */
		"q: 50: 2\n"  /* torch */
		"r:171:14\n"  /* carpet */
		"s: 50: 3\n"  /* torch */
		"t: 53: 2\n"  /* woodstairs */
		"u: 53: 0\n"  /* woodstairs */
		"v: 53: 1\n"  /* woodstairs */
		"w: 53: 5\n"  /* woodstairs */
		"x: 53: 4\n"  /* woodstairs */
		"y: 17: 4\n"  /* tree */
		"z: 17: 8\n"  /* tree */,

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
		/*  7 */ "aaaaaaa"
		/*  8 */ "aaaaaaa"
		/*  9 */ "aaaaaaa"
		/* 10 */ "aaaaaaa"
		/* 11 */ "aaaaaaa"
		/* 12 */ "aaaaaaa"

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "bbaaabb"
		/*  1 */ "baaaaab"
		/*  2 */ "baaaaab"
		/*  3 */ "baaaaab"
		/*  4 */ "baaaaab"
		/*  5 */ "baaaaab"
		/*  6 */ "baaaaab"
		/*  7 */ "baaaaab"
		/*  8 */ "baaaaab"
		/*  9 */ "baaaaab"
		/* 10 */ "baaaaab"
		/* 11 */ "baaaaab"
		/* 12 */ "bbbbbbb"

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "..cde.."
		/*  1 */ ".fffff."
		/*  2 */ ".fffff."
		/*  3 */ ".fffff."
		/*  4 */ ".fffff."
		/*  5 */ ".fffff."
		/*  6 */ ".fffff."
		/*  7 */ ".fffff."
		/*  8 */ ".fffff."
		/*  9 */ ".fffff."
		/* 10 */ ".fffff."
		/* 11 */ ".fffff."
		/* 12 */ "......."

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ghihg."
		/*  2 */ ".h..jh."
		/*  3 */ ".h...h."
		/*  4 */ ".h...h."
		/*  5 */ ".gk.kg."
		/*  6 */ ".h...h."
		/*  7 */ ".hk.kh."
		/*  8 */ ".h...h."
		/*  9 */ ".h.l.h."
		/* 10 */ ".h...h."
		/* 11 */ ".ghhhg."
		/* 12 */ "......."

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ghnhg."
		/*  2 */ ".o..jo."
		/*  3 */ ".o...o."
		/*  4 */ ".o...o."
		/*  5 */ ".g...g."
		/*  6 */ ".o...o."
		/*  7 */ ".o...o."
		/*  8 */ ".hp.qh."
		/*  9 */ ".o.r.o."
		/* 10 */ ".o...o."
		/* 11 */ ".gooog."
		/* 12 */ "......."

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ghhhg."
		/*  2 */ ".h.sjh."
		/*  3 */ ".h...h."
		/*  4 */ ".h...h."
		/*  5 */ "tg...gt"
		/*  6 */ "uh...hv"
		/*  7 */ "uh...hv"
		/*  8 */ "uh...hv"
		/*  9 */ "uh...hv"
		/* 10 */ "uh...hv"
		/* 11 */ "ughhhhv"
		/* 12 */ "uw...xv"

		// Level 6
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".gyyyg."
		/*  2 */ ".zhhjz."
		/*  3 */ ".zhhhz."
		/*  4 */ ".zhhhz."
		/*  5 */ ".gyyyg."
		/*  6 */ ".uh.hv."
		/*  7 */ ".uh.hv."
		/*  8 */ ".uh.hv."
		/*  9 */ ".uh.hv."
		/* 10 */ ".uh.hv."
		/* 11 */ ".uhhhv."
		/* 12 */ ".uw.xv."

		// Level 7
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ghohg."
		/*  2 */ ".h..jh."
		/*  3 */ ".h...h."
		/*  4 */ ".h.A.h."
		/*  5 */ ".ghhhg."
		/*  6 */ "..uhv.."
		/*  7 */ "..uhv.."
		/*  8 */ "..uhv.."
		/*  9 */ "..uhv.."
		/* 10 */ "..uhv.."
		/* 11 */ "..uhv.."
		/* 12 */ "..uhv.."

		// Level 8
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ghohg."
		/*  2 */ ".h..jh."
		/*  3 */ ".o...o."
		/*  4 */ ".h...h."
		/*  5 */ ".ghhhg."
		/*  6 */ "......."
		/*  7 */ "......."
		/*  8 */ "......."
		/*  9 */ "......."
		/* 10 */ "......."
		/* 11 */ "......."
		/* 12 */ "......."

		// Level 9
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ghohg."
		/*  2 */ ".h..jh."
		/*  3 */ ".h...h."
		/*  4 */ ".h.B.h."
		/*  5 */ ".ghhhg."
		/*  6 */ "......."
		/*  7 */ "......."
		/*  8 */ "......."
		/*  9 */ "......."
		/* 10 */ "......."
		/* 11 */ "......."
		/* 12 */ "......."

		// Level 10
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".gyyyg."
		/*  2 */ ".zhhjz."
		/*  3 */ ".zhhhz."
		/*  4 */ ".zhhhz."
		/*  5 */ ".gyyyg."
		/*  6 */ "......."
		/*  7 */ "......."
		/*  8 */ "......."
		/*  9 */ "......."
		/* 10 */ "......."
		/* 11 */ "......."
		/* 12 */ "......."

		// Level 11
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".gCCCg."
		/*  2 */ ".C...C."
		/*  3 */ ".C...C."
		/*  4 */ ".C...C."
		/*  5 */ ".gCCCg."
		/*  6 */ "......."
		/*  7 */ "......."
		/*  8 */ "......."
		/*  9 */ "......."
		/* 10 */ "......."
		/* 11 */ "......."
		/* 12 */ "......."

		// Level 12
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".g...g."
		/*  2 */ "......."
		/*  3 */ "......."
		/*  4 */ "......."
		/*  5 */ ".g...g."
		/*  6 */ "......."
		/*  7 */ "......."
		/*  8 */ "......."
		/*  9 */ "......."
		/* 10 */ "......."
		/* 11 */ "......."
		/* 12 */ "......."

		// Level 13
		/* z\x*   0123456 */
		/*  0 */ "uw...xv"
		/*  1 */ "ughhhgv"
		/*  2 */ "uhDDDhv"
		/*  3 */ "uhDDDhv"
		/*  4 */ "uhDDDhv"
		/*  5 */ "uhhhhhv"
		/*  6 */ "uw...xv"
		/*  7 */ "......."
		/*  8 */ "......."
		/*  9 */ "......."
		/* 10 */ "......."
		/* 11 */ "......."
		/* 12 */ "......."

		// Level 14
		/* z\x*   0123456 */
		/*  0 */ ".uw.xv."
		/*  1 */ ".uhhhv."
		/*  2 */ ".uhhhv."
		/*  3 */ ".uhhhv."
		/*  4 */ ".uhhhv."
		/*  5 */ ".uhhhv."
		/*  6 */ ".uw.xv."
		/*  7 */ "......."
		/*  8 */ "......."
		/*  9 */ "......."
		/* 10 */ "......."
		/* 11 */ "......."
		/* 12 */ "......."

		// Level 15
		/* z\x*   0123456 */
		/*  0 */ "..uhv.."
		/*  1 */ "..uhv.."
		/*  2 */ "..uhv.."
		/*  3 */ "..uhv.."
		/*  4 */ "..uhv.."
		/*  5 */ "..uhv.."
		/*  6 */ "..uhv.."
		/*  7 */ "......."
		/*  8 */ "......."
		/*  9 */ "......."
		/* 10 */ "......."
		/* 11 */ "......."
		/* 12 */ ".......",

		// Connectors:
		"-1: 3, 2, 0: 2\n"  /* Type -1, direction Z- */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotation allowed */

		// Merge strategy:
		cBlockArea::msSpongePrint,

		// ShouldExtendFloor:
		true,

		// DefaultWeight:
		20,

		// DepthWeight:
		"",

		// AddWeightIfSame:
		0,
	},  // WoodenChurchMid



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenGranary:
	// The data has been exported from the gallery Plains, area index 54, ID 105, created by Aloe_vera
	{
		// Size:
		7, 8, 9,  // SizeX = 7, SizeY = 8, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 7, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c:170: 0\n"  /* haybale */
		"d: 67: 0\n"  /* stairs */
		"e: 67: 2\n"  /* stairs */
		"f: 67: 1\n"  /* stairs */
		"g:  4: 0\n"  /* cobblestone */
		"h: 17: 0\n"  /* tree */
		"i:  5: 0\n"  /* wood */
		"j:170: 4\n"  /* haybale */
		"k:170: 8\n"  /* haybale */
		"l: 54: 2\n"  /* chest */
		"m: 19: 0\n"  /* sponge */
		"n: 50: 4\n"  /* torch */
		"o: 53: 0\n"  /* woodstairs */
		"p: 53: 5\n"  /* woodstairs */
		"q: 53: 4\n"  /* woodstairs */
		"r: 53: 1\n"  /* woodstairs */,

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
		/*  7 */ "aaaaaaa"
		/*  8 */ "aaaaaaa"

		// Level 1
		/* z\x*   0123456 */
		/*  0 */ "aaaaaab"
		/*  1 */ "baaaaab"
		/*  2 */ "baaaaab"
		/*  3 */ "baaaaab"
		/*  4 */ "baaaaab"
		/*  5 */ "baaaaab"
		/*  6 */ "baaaaab"
		/*  7 */ "baaaaab"
		/*  8 */ "bbbbbbb"

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "cdeeef."
		/*  1 */ ".ggggg."
		/*  2 */ ".ggggg."
		/*  3 */ ".ggggg."
		/*  4 */ ".ggggg."
		/*  5 */ ".ggggg."
		/*  6 */ ".ggggg."
		/*  7 */ ".ggggg."
		/*  8 */ "......."

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".h..ch."
		/*  2 */ ".i...i."
		/*  3 */ ".ic.ji."
		/*  4 */ ".hkjkh."
		/*  5 */ ".iccci."
		/*  6 */ ".iklci."
		/*  7 */ ".hihih."
		/*  8 */ "......."

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ ".n...n."
		/*  1 */ ".h...h."
		/*  2 */ ".i...i."
		/*  3 */ ".i...i."
		/*  4 */ ".hj..h."
		/*  5 */ ".ijkci."
		/*  6 */ ".ijkki."
		/*  7 */ ".hihih."
		/*  8 */ "......."

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "op...qr"
		/*  1 */ "oiiiiir"
		/*  2 */ "oi...ir"
		/*  3 */ "oi...ir"
		/*  4 */ "oi...ir"
		/*  5 */ "oicc.ir"
		/*  6 */ "oikckir"
		/*  7 */ "oiiiiir"
		/*  8 */ "op...qr"

		// Level 6
		/* z\x*   0123456 */
		/*  0 */ ".op.qr."
		/*  1 */ ".oiiir."
		/*  2 */ ".oi.ir."
		/*  3 */ ".oi.ir."
		/*  4 */ ".oi.ir."
		/*  5 */ ".oi.ir."
		/*  6 */ ".oi.ir."
		/*  7 */ ".oiiir."
		/*  8 */ ".op.qr."

		// Level 7
		/* z\x*   0123456 */
		/*  0 */ "..oir.."
		/*  1 */ "..oir.."
		/*  2 */ "..oir.."
		/*  3 */ "..oir.."
		/*  4 */ "..oir.."
		/*  5 */ "..oir.."
		/*  6 */ "..oir.."
		/*  7 */ "..oir.."
		/*  8 */ "..oir..",

		// Connectors:
		"-1: 3, 2, -1: 2\n"  /* Type -1, direction Z- */,

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
	},  // WoodenGranary



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse10x7Library:
	// The data has been exported from the gallery Plains, area index 47, ID 98, created by Aloe_vera
	{
		// Size:
		12, 9, 9,  // SizeX = 12, SizeY = 9, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		11, 8, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j: 64: 5\n"  /* wooddoorblock */
		"k: 53: 3\n"  /* woodstairs */
		"l: 85: 0\n"  /* fence */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 2\n"  /* woodstairs */
		"o: 53: 1\n"  /* woodstairs */
		"p: 53: 0\n"  /* woodstairs */
		"q:102: 0\n"  /* glasspane */
		"r: 64:12\n"  /* wooddoorblock */
		"s: 50: 3\n"  /* torch */
		"t: 72: 0\n"  /* woodplate */
		"u: 53: 7\n"  /* woodstairs */
		"v: 47: 0\n"  /* bookshelf */
		"w: 50: 1\n"  /* torch */
		"x: 50: 2\n"  /* torch */
		"y: 53: 6\n"  /* woodstairs */,

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
		/*  6 */ "aaaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaaa"

		// Level 1
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "bbbbaaaabbbb"
		/*  1 */ "baaaaaaaaaab"
		/*  2 */ "baaaaaaaaaab"
		/*  3 */ "baaaaaaaaaab"
		/*  4 */ "baaaaaaaaaab"
		/*  5 */ "baaaaaaaaaab"
		/*  6 */ "baaaaaaaaaab"
		/*  7 */ "baaaaaaaaaab"
		/*  8 */ "bbbbbbbbbbbb"

		// Level 2
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "....cdde...."
		/*  1 */ ".ffffffffff."
		/*  2 */ ".ffffffffff."
		/*  3 */ ".ffffffffff."
		/*  4 */ ".ffffffffff."
		/*  5 */ ".ffffffffff."
		/*  6 */ ".ffffffffff."
		/*  7 */ ".ffffffffff."
		/*  8 */ "............"

		// Level 3
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ ".ghhhijhhhg."
		/*  2 */ ".h........h."
		/*  3 */ ".hk......kh."
		/*  4 */ ".hl......lh."
		/*  5 */ ".hn......nh."
		/*  6 */ ".h.olpolp.h."
		/*  7 */ ".ghhhhhhhhg."
		/*  8 */ "............"

		// Level 4
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ ".gqqhrrhqqg."
		/*  2 */ ".q..s..s..q."
		/*  3 */ ".q........q."
		/*  4 */ ".ht......th."
		/*  5 */ ".q........q."
		/*  6 */ ".q..t..t..q."
		/*  7 */ ".gqqhqqhqqg."
		/*  8 */ "............"

		// Level 5
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "nnnnnnnnnnnn"
		/*  1 */ "uhhhhhhhhhhu"
		/*  2 */ ".hvvvvvvvvh."
		/*  3 */ ".h........h."
		/*  4 */ ".hw......xh."
		/*  5 */ ".h........h."
		/*  6 */ ".hvvvvvvvvh."
		/*  7 */ "yhhhhhhhhhhy"
		/*  8 */ "kkkkkkkkkkkk"

		// Level 6
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "nnnnnnnnnnnn"
		/*  2 */ "uhhhhhhhhhhu"
		/*  3 */ ".hvvvvvvvvh."
		/*  4 */ ".h........h."
		/*  5 */ ".hvvvvvvvvh."
		/*  6 */ "yhhhhhhhhhhy"
		/*  7 */ "kkkkkkkkkkkk"
		/*  8 */ "............"

		// Level 7
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "nnnnnnnnnnnn"
		/*  3 */ "uhhhhhhhhhhu"
		/*  4 */ ".h........h."
		/*  5 */ "yhhhhhhhhhhy"
		/*  6 */ "kkkkkkkkkkkk"
		/*  7 */ "............"
		/*  8 */ "............"

		// Level 8
		/* z\x*             11 */
		/*    *   012345678901 */
		/*  0 */ "............"
		/*  1 */ "............"
		/*  2 */ "............"
		/*  3 */ "nnnnnnnnnnnn"
		/*  4 */ "hhhhhhhhhhhh"
		/*  5 */ "kkkkkkkkkkkk"
		/*  6 */ "............"
		/*  7 */ "............"
		/*  8 */ "............",

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
	},  // WoodenHouse10x7Library



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse5x5:
	// The data has been exported from the gallery Plains, area index 49, ID 100, created by Aloe_vera
	{
		// Size:
		7, 8, 7,  // SizeX = 7, SizeY = 8, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 7, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j: 64:12\n"  /* wooddoorblock */
		"k:102: 0\n"  /* glasspane */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 7\n"  /* woodstairs */
		"o: 50: 3\n"  /* torch */
		"p: 53: 6\n"  /* woodstairs */
		"q: 53: 3\n"  /* woodstairs */,

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
		/*  0 */ "bbaaabb"
		/*  1 */ "baaaaab"
		/*  2 */ "baaaaab"
		/*  3 */ "baaaaab"
		/*  4 */ "baaaaab"
		/*  5 */ "baaaaab"
		/*  6 */ "bbbbbbb"

		// Level 2
		/* z\x*   0123456 */
		/*  0 */ "..cde.."
		/*  1 */ ".fffff."
		/*  2 */ ".fffff."
		/*  3 */ ".fffff."
		/*  4 */ ".fffff."
		/*  5 */ ".fffff."
		/*  6 */ "......."

		// Level 3
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ghihg."
		/*  2 */ ".h...h."
		/*  3 */ ".h...h."
		/*  4 */ ".h...h."
		/*  5 */ ".ghhhg."
		/*  6 */ "......."

		// Level 4
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".ghjhg."
		/*  2 */ ".k...k."
		/*  3 */ ".k...k."
		/*  4 */ ".k...k."
		/*  5 */ ".gkkkg."
		/*  6 */ "......."

		// Level 5
		/* z\x*   0123456 */
		/*  0 */ "lllllll"
		/*  1 */ "nhhhhhn"
		/*  2 */ ".ho.oh."
		/*  3 */ ".h...h."
		/*  4 */ ".h...h."
		/*  5 */ "phhhhhp"
		/*  6 */ "qqqqqqq"

		// Level 6
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "lllllll"
		/*  2 */ "nhhhhhn"
		/*  3 */ ".h...h."
		/*  4 */ "phhhhhp"
		/*  5 */ "qqqqqqq"
		/*  6 */ "......."

		// Level 7
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "lllllll"
		/*  3 */ "hhhhhhh"
		/*  4 */ "qqqqqqq"
		/*  5 */ "......."
		/*  6 */ ".......",

		// Connectors:
		"-1: 3, 2, 0: 2\n"  /* Type -1, direction Z- */,

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
	},  // WoodenHouse5x5



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse7x5:
	// The data has been exported from the gallery Plains, area index 40, ID 91, created by xoft
	{
		// Size:
		9, 8, 7,  // SizeX = 9, SizeY = 8, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		8, 7, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j:102: 0\n"  /* glasspane */
		"k: 64:12\n"  /* wooddoorblock */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 7\n"  /* woodstairs */
		"o: 50: 3\n"  /* torch */
		"p: 53: 6\n"  /* woodstairs */
		"q: 53: 3\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*   012345678 */
		/*  0 */ "aaaaaaaaa"
		/*  1 */ "aaaaaaaaa"
		/*  2 */ "aaaaaaaaa"
		/*  3 */ "aaaaaaaaa"
		/*  4 */ "aaaaaaaaa"
		/*  5 */ "aaaaaaaaa"
		/*  6 */ "aaaaaaaaa"

		// Level 1
		/* z\x*   012345678 */
		/*  0 */ "bbbaaabbb"
		/*  1 */ "baaaaaaab"
		/*  2 */ "baaaaaaab"
		/*  3 */ "baaaaaaab"
		/*  4 */ "baaaaaaab"
		/*  5 */ "baaaaaaab"
		/*  6 */ "bbbbbbbbb"

		// Level 2
		/* z\x*   012345678 */
		/*  0 */ "...cde..."
		/*  1 */ ".fffffff."
		/*  2 */ ".fffffff."
		/*  3 */ ".fffffff."
		/*  4 */ ".fffffff."
		/*  5 */ ".fffffff."
		/*  6 */ "........."

		// Level 3
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".ghhihhg."
		/*  2 */ ".h.....h."
		/*  3 */ ".h.....h."
		/*  4 */ ".h.....h."
		/*  5 */ ".ghhhhhg."
		/*  6 */ "........."

		// Level 4
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ ".gjhkhjg."
		/*  2 */ ".j.....j."
		/*  3 */ ".j.....j."
		/*  4 */ ".j.....j."
		/*  5 */ ".gjjhjjg."
		/*  6 */ "........."

		// Level 5
		/* z\x*   012345678 */
		/*  0 */ "lllllllll"
		/*  1 */ "nghhhhhgn"
		/*  2 */ ".h.o.o.h."
		/*  3 */ ".h.....h."
		/*  4 */ ".h.....h."
		/*  5 */ "pghhhhhgp"
		/*  6 */ "qqqqqqqqq"

		// Level 6
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "lllllllll"
		/*  2 */ "nhhhhhhhn"
		/*  3 */ ".h.....h."
		/*  4 */ "phhhhhhhp"
		/*  5 */ "qqqqqqqqq"
		/*  6 */ "........."

		// Level 7
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "lllllllll"
		/*  3 */ "hhhhhhhhh"
		/*  4 */ "qqqqqqqqq"
		/*  5 */ "........."
		/*  6 */ ".........",

		// Connectors:
		"-1: 4, 2, -1: 2\n"  /* Type -1, direction Z- */,

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
	},  // WoodenHouse7x5



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse9x5:
	// The data has been exported from the gallery Plains, area index 41, ID 92, created by xoft
	{
		// Size:
		11, 8, 7,  // SizeX = 11, SizeY = 8, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 7, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j:102: 0\n"  /* glasspane */
		"k: 64:12\n"  /* wooddoorblock */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 7\n"  /* woodstairs */
		"o: 50: 3\n"  /* torch */
		"p: 53: 6\n"  /* woodstairs */
		"q: 53: 3\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaa"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "bbbbaaabbbb"
		/*  1 */ "baaaaaaaaab"
		/*  2 */ "baaaaaaaaab"
		/*  3 */ "baaaaaaaaab"
		/*  4 */ "baaaaaaaaab"
		/*  5 */ "baaaaaaaaab"
		/*  6 */ "bbbbbbbbbbb"

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "....cde...."
		/*  1 */ ".fffffffff."
		/*  2 */ ".fffffffff."
		/*  3 */ ".fffffffff."
		/*  4 */ ".fffffffff."
		/*  5 */ ".fffffffff."
		/*  6 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".ghhhihhhg."
		/*  2 */ ".h.......h."
		/*  3 */ ".h.......h."
		/*  4 */ ".h.......h."
		/*  5 */ ".ghhhhhhhg."
		/*  6 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".gjjhkhjjg."
		/*  2 */ ".j.......j."
		/*  3 */ ".j.......j."
		/*  4 */ ".j.......j."
		/*  5 */ ".gjjjhjjjg."
		/*  6 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "lllllllllll"
		/*  1 */ "nhhhhhhhhhn"
		/*  2 */ ".h..o.o.hh."
		/*  3 */ ".h......hh."
		/*  4 */ ".h......hh."
		/*  5 */ "phhhhhhhhhp"
		/*  6 */ "qqqqqqqqqqq"

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "lllllllllll"
		/*  2 */ "nhhhhhhhhhn"
		/*  3 */ ".hhhhhhhhh."
		/*  4 */ "phhhhhhhhhp"
		/*  5 */ "qqqqqqqqqqq"
		/*  6 */ "..........."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "lllllllllll"
		/*  3 */ "hhhhhhhhhhh"
		/*  4 */ "qqqqqqqqqqq"
		/*  5 */ "..........."
		/*  6 */ "...........",

		// Connectors:
		"-1: 5, 2, -1: 2\n"  /* Type -1, direction Z- */,

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
	},  // WoodenHouse9x5



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse9x5:
	// The data has been exported from the gallery Plains, area index 46, ID 97, created by Aloe_vera
	{
		// Size:
		11, 8, 7,  // SizeX = 11, SizeY = 8, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 7, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j: 53: 3\n"  /* woodstairs */
		"k: 85: 0\n"  /* fence */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 1\n"  /* woodstairs */
		"o: 53: 0\n"  /* woodstairs */
		"p:102: 0\n"  /* glasspane */
		"q: 64:12\n"  /* wooddoorblock */
		"r: 50: 3\n"  /* torch */
		"s: 72: 0\n"  /* woodplate */
		"t: 53: 7\n"  /* woodstairs */
		"u: 47: 0\n"  /* bookshelf */
		"v: 50: 1\n"  /* torch */
		"w: 50: 2\n"  /* torch */
		"x: 53: 6\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaa"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "bbbbaaabbbb"
		/*  1 */ "baaaaaaaaab"
		/*  2 */ "baaaaaaaaab"
		/*  3 */ "baaaaaaaaab"
		/*  4 */ "baaaaaaaaab"
		/*  5 */ "baaaaaaaaab"
		/*  6 */ "bbbbbbbbbbb"

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "....cde...."
		/*  1 */ ".fffffffff."
		/*  2 */ ".fffffffff."
		/*  3 */ ".fffffffff."
		/*  4 */ ".fffffffff."
		/*  5 */ ".fffffffff."
		/*  6 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".ghhhihhhg."
		/*  2 */ ".hj.....jh."
		/*  3 */ ".hk.....kh."
		/*  4 */ ".hl.nko.lh."
		/*  5 */ ".ghhhhhhhg."
		/*  6 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".gpphqhppg."
		/*  2 */ ".p..r.r..p."
		/*  3 */ ".ps.....sp."
		/*  4 */ ".p...s...p."
		/*  5 */ ".gppphpppg."
		/*  6 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "lllllllllll"
		/*  1 */ "thhhhhhhhht"
		/*  2 */ ".huuuuuuuh."
		/*  3 */ ".hv.....wh."
		/*  4 */ ".huuuuuuuh."
		/*  5 */ "xhhhhhhhhhx"
		/*  6 */ "jjjjjjjjjjj"

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "lllllllllll"
		/*  2 */ "thhhhhhhhht"
		/*  3 */ ".h.......h."
		/*  4 */ "xhhhhhhhhhx"
		/*  5 */ "jjjjjjjjjjj"
		/*  6 */ "..........."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "lllllllllll"
		/*  3 */ "hhhhhhhhhhh"
		/*  4 */ "jjjjjjjjjjj"
		/*  5 */ "..........."
		/*  6 */ "...........",

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
	},  // WoodenHouse9x5



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse9x5Fence:
	// The data has been exported from the gallery Plains, area index 9, ID 26, created by Aloe_vera
	{
		// Size:
		10, 8, 11,  // SizeX = 10, SizeY = 8, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		9, 7, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c:  4: 0\n"  /* cobblestone */
		"d:  5: 0\n"  /* wood */
		"e: 67: 2\n"  /* stairs */
		"f: 43: 0\n"  /* doubleslab */
		"g: 67: 0\n"  /* stairs */
		"h: 67: 3\n"  /* stairs */
		"i: 17: 0\n"  /* tree */
		"j: 53: 1\n"  /* woodstairs */
		"k: 85: 0\n"  /* fence */
		"l: 53: 0\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 64: 6\n"  /* wooddoorblock */
		"o: 64: 0\n"  /* wooddoorblock */
		"p:102: 0\n"  /* glasspane */
		"q: 72: 0\n"  /* woodplate */
		"r: 64:12\n"  /* wooddoorblock */
		"s: 64: 8\n"  /* wooddoorblock */
		"t: 53: 5\n"  /* woodstairs */
		"u: 53: 4\n"  /* woodstairs */
		"v: 50: 1\n"  /* torch */
		"w: 50: 2\n"  /* torch */,

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
		/*  7 */ "aaaaaaaaaa"
		/*  8 */ "aaaaaaaaaa"
		/*  9 */ "aaaaaaaaaa"
		/* 10 */ "aaaaaaaaaa"

		// Level 1
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "bbbbbbbbbb"
		/*  1 */ "baaaaabbbb"
		/*  2 */ "baaaaabbbb"
		/*  3 */ "baaaaaaaaa"
		/*  4 */ "aaaaaaaaaa"
		/*  5 */ "aaaaaaaaaa"
		/*  6 */ "aaaaaaaaaa"
		/*  7 */ "baaaaaaaaa"
		/*  8 */ "baaaaaaaaa"
		/*  9 */ "baaaaabbbb"
		/* 10 */ "bbbbbbbbbb"

		// Level 2
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "......mmmm"
		/*  1 */ ".cccccmmmm"
		/*  2 */ ".cdddcmmmm"
		/*  3 */ ".cdddcbbbb"
		/*  4 */ "ecfffcbbbb"
		/*  5 */ "gcffffbbbb"
		/*  6 */ "hcfffcbbbb"
		/*  7 */ ".cfffcbbbb"
		/*  8 */ ".cfffcbbbb"
		/*  9 */ ".cccccmmmm"
		/* 10 */ "......mmmm"

		// Level 3
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "......mmmm"
		/*  1 */ ".idddimmmm"
		/*  2 */ ".djkldmmmm"
		/*  3 */ ".d...dkkkk"
		/*  4 */ ".d...d...k"
		/*  5 */ ".n...o...k"
		/*  6 */ ".d...d...k"
		/*  7 */ ".dff.d...k"
		/*  8 */ ".d...dkkkk"
		/*  9 */ ".idddimmmm"
		/* 10 */ "......mmmm"

		// Level 4
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "......mmmm"
		/*  1 */ ".ipppimmmm"
		/*  2 */ ".p.q.pmmmm"
		/*  3 */ ".p...p...."
		/*  4 */ ".d...d...."
		/*  5 */ ".r...s...."
		/*  6 */ ".d...d...."
		/*  7 */ ".p...p...."
		/*  8 */ ".p...p...."
		/*  9 */ ".ipppimmmm"
		/* 10 */ "......mmmm"

		// Level 5
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "lt...ujmmm"
		/*  1 */ "lidddijmmm"
		/*  2 */ "ld...djmmm"
		/*  3 */ "ld...dj..."
		/*  4 */ "ldv.wdj..."
		/*  5 */ "ld...dj..."
		/*  6 */ "ldv.wdj..."
		/*  7 */ "ld...dj..."
		/*  8 */ "ld...dj..."
		/*  9 */ "lidddijmmm"
		/* 10 */ "lt...ujmmm"

		// Level 6
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "mlt.ujmmmm"
		/*  1 */ "mldddjmmmm"
		/*  2 */ "mld.djmmmm"
		/*  3 */ "mld.djm..."
		/*  4 */ "mld.djm..."
		/*  5 */ "mld.djm..."
		/*  6 */ "mld.djm..."
		/*  7 */ "mld.djm..."
		/*  8 */ "mld.djm..."
		/*  9 */ "mldddjmmmm"
		/* 10 */ "mlt.ujmmmm"

		// Level 7
		/* z\x*              */
		/*    *   0123456789 */
		/*  0 */ "mmldjmmmmm"
		/*  1 */ "mmldjmmmmm"
		/*  2 */ "mmldjmmmmm"
		/*  3 */ "mmldjmm..."
		/*  4 */ "mmldjmm..."
		/*  5 */ "mmldjmm..."
		/*  6 */ "mmldjmm..."
		/*  7 */ "mmldjmm..."
		/*  8 */ "mmldjmm..."
		/*  9 */ "mmldjmmmmm"
		/* 10 */ "mmldjmmmmm",

		// Connectors:
		"-1: 0, 2, 5: 4\n"  /* Type -1, direction X- */,

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
	},  // WoodenHouse9x5Fence



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse9x7:
	// The data has been exported from the gallery Plains, area index 52, ID 103, created by Aloe_vera
	{
		// Size:
		11, 9, 9,  // SizeX = 11, SizeY = 9, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 8, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j:102: 0\n"  /* glasspane */
		"k: 64:12\n"  /* wooddoorblock */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 7\n"  /* woodstairs */
		"o: 50: 3\n"  /* torch */
		"p: 50: 4\n"  /* torch */
		"q: 53: 6\n"  /* woodstairs */
		"r: 53: 3\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaa"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "bbbbaaabbbb"
		/*  1 */ "baaaaaaaaab"
		/*  2 */ "baaaaaaaaab"
		/*  3 */ "baaaaaaaaab"
		/*  4 */ "baaaaaaaaab"
		/*  5 */ "baaaaaaaaab"
		/*  6 */ "baaaaaaaaab"
		/*  7 */ "baaaaaaaaab"
		/*  8 */ "bbbbbbbbbbb"

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "....cde...."
		/*  1 */ ".fffffffff."
		/*  2 */ ".fffffffff."
		/*  3 */ ".fffffffff."
		/*  4 */ ".fffffffff."
		/*  5 */ ".fffffffff."
		/*  6 */ ".fffffffff."
		/*  7 */ ".fffffffff."
		/*  8 */ "..........."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".ghhhihhhg."
		/*  2 */ ".h.......h."
		/*  3 */ ".h.......h."
		/*  4 */ ".h.......h."
		/*  5 */ ".h.......h."
		/*  6 */ ".h.......h."
		/*  7 */ ".ghhhhhhhg."
		/*  8 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".gjjhkhjjg."
		/*  2 */ ".j.......j."
		/*  3 */ ".j.......j."
		/*  4 */ ".h.......h."
		/*  5 */ ".j.......j."
		/*  6 */ ".j.......j."
		/*  7 */ ".gjjjhjjjg."
		/*  8 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "lllllllllll"
		/*  1 */ "nhhhhhhhhhn"
		/*  2 */ ".h..o.o..h."
		/*  3 */ ".h.......h."
		/*  4 */ ".h.......h."
		/*  5 */ ".h.......h."
		/*  6 */ ".h...p...h."
		/*  7 */ "qhhhhhhhhhq"
		/*  8 */ "rrrrrrrrrrr"

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "lllllllllll"
		/*  2 */ "nhhhhhhhhhn"
		/*  3 */ ".h.......h."
		/*  4 */ ".h.......h."
		/*  5 */ ".h.......h."
		/*  6 */ "qhhhhhhhhhq"
		/*  7 */ "rrrrrrrrrrr"
		/*  8 */ "..........."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "lllllllllll"
		/*  3 */ "nhhhhhhhhhn"
		/*  4 */ ".h.......h."
		/*  5 */ "qhhhhhhhhhq"
		/*  6 */ "rrrrrrrrrrr"
		/*  7 */ "..........."
		/*  8 */ "..........."

		// Level 8
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "lllllllllll"
		/*  4 */ "hhhhhhhhhhh"
		/*  5 */ "rrrrrrrrrrr"
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "...........",

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
	},  // WoodenHouse9x7



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse9x7Butcher:
	// The data has been exported from the gallery Plains, area index 48, ID 99, created by Aloe_vera
	{
		// Size:
		11, 9, 13,  // SizeX = 11, SizeY = 9, SizeZ = 13

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 8, 12,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 43: 0\n"  /* doubleslab */
		"h: 17: 0\n"  /* tree */
		"i:  5: 0\n"  /* wood */
		"j: 64: 7\n"  /* wooddoorblock */
		"k: 53: 3\n"  /* woodstairs */
		"l: 85: 0\n"  /* fence */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 2\n"  /* woodstairs */
		"o:102: 0\n"  /* glasspane */
		"p: 64:12\n"  /* wooddoorblock */
		"q: 72: 0\n"  /* woodplate */
		"r: 53: 7\n"  /* woodstairs */
		"s: 50: 1\n"  /* torch */
		"t: 50: 2\n"  /* torch */
		"u: 53: 6\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaa"
		/*  9 */ "aaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaa"
		/* 11 */ "aaaaaaaaaaa"
		/* 12 */ "aaaaaaaaaaa"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "bbbbaaabbbb"
		/*  1 */ "baaaaaaaaab"
		/*  2 */ "baaaaaaaaab"
		/*  3 */ "baaaaaaaaab"
		/*  4 */ "baaaaaaaaab"
		/*  5 */ "baaaaaaaaab"
		/*  6 */ "baaaaaaaaab"
		/*  7 */ "baaaaaaaaab"
		/*  8 */ "bbaaaaaaabb"
		/*  9 */ "bbaaaaaaabb"
		/* 10 */ "bbaaaaaaabb"
		/* 11 */ "bbaaaaaaabb"
		/* 12 */ "bbaaaaaaabb"

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "....cde...."
		/*  1 */ ".fffffffff."
		/*  2 */ ".fggggffff."
		/*  3 */ ".fggggffff."
		/*  4 */ ".fggggffff."
		/*  5 */ ".fggggffff."
		/*  6 */ ".fggggffff."
		/*  7 */ ".fffffffff."
		/*  8 */ "..bbbbbbb.."
		/*  9 */ "mmbbbbbbbmm"
		/* 10 */ "mmbbbbbbbmm"
		/* 11 */ "mmbbbbbbbmm"
		/* 12 */ "mmbbbbbbbmm"

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".hiiijiiih."
		/*  2 */ ".i.g....ki."
		/*  3 */ ".i.g....li."
		/*  4 */ ".i.g....ni."
		/*  5 */ ".i.......i."
		/*  6 */ ".i.......i."
		/*  7 */ ".hiiijiiih."
		/*  8 */ "..l.....l.."
		/*  9 */ "mml.....lmm"
		/* 10 */ "mml.....lmm"
		/* 11 */ "mml.....lmm"
		/* 12 */ "mmlllllllmm"

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".hooipiooh."
		/*  2 */ ".o.......o."
		/*  3 */ ".o......qo."
		/*  4 */ ".i.......i."
		/*  5 */ ".o.......o."
		/*  6 */ ".o.......o."
		/*  7 */ ".hooipiooh."
		/*  8 */ "..........."
		/*  9 */ "mm.......mm"
		/* 10 */ "mm.......mm"
		/* 11 */ "mm.......mm"
		/* 12 */ "mm.......mm"

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "nnnnnnnnnnn"
		/*  1 */ "riiiiiiiiir"
		/*  2 */ ".i.......i."
		/*  3 */ ".i.......i."
		/*  4 */ ".is.....ti."
		/*  5 */ ".i.......i."
		/*  6 */ ".i.......i."
		/*  7 */ "uiiiiiiiiiu"
		/*  8 */ "kkkkkkkkkkk"
		/*  9 */ "mm.......mm"
		/* 10 */ "mm.......mm"
		/* 11 */ "mm.......mm"
		/* 12 */ "mm.......mm"

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "nnnnnnnnnnn"
		/*  2 */ "riiiiiiiiir"
		/*  3 */ ".i.......i."
		/*  4 */ ".i.......i."
		/*  5 */ ".i.......i."
		/*  6 */ "uiiiiiiiiiu"
		/*  7 */ "kkkkkkkkkkk"
		/*  8 */ "..........."
		/*  9 */ "mm.......mm"
		/* 10 */ "mm.......mm"
		/* 11 */ "mm.......mm"
		/* 12 */ "mm.......mm"

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "nnnnnnnnnnn"
		/*  3 */ "riiiiiiiiir"
		/*  4 */ ".i.......i."
		/*  5 */ "uiiiiiiiiiu"
		/*  6 */ "kkkkkkkkkkk"
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "mm.......mm"
		/* 10 */ "mm.......mm"
		/* 11 */ "mm.......mm"
		/* 12 */ "mm.......mm"

		// Level 8
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "nnnnnnnnnnn"
		/*  4 */ "iiiiiiiiiii"
		/*  5 */ "kkkkkkkkkkk"
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "..........."
		/*  9 */ "mm.......mm"
		/* 10 */ "mm.......mm"
		/* 11 */ "mm.......mm"
		/* 12 */ "mm.......mm",

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
	},  // WoodenHouse9x7Butcher



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouse9x7DoubleDoor:
	// The data has been exported from the gallery Plains, area index 38, ID 87, created by Aloe_vera
	{
		// Size:
		11, 9, 9,  // SizeX = 11, SizeY = 9, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 8, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 67: 3\n"  /* stairs */
		"h: 17: 0\n"  /* tree */
		"i:  5: 0\n"  /* wood */
		"j: 64: 7\n"  /* wooddoorblock */
		"k:102: 0\n"  /* glasspane */
		"l: 64:12\n"  /* wooddoorblock */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 2\n"  /* woodstairs */
		"o: 53: 7\n"  /* woodstairs */
		"p: 17: 4\n"  /* tree */
		"q: 17: 8\n"  /* tree */
		"r: 50: 3\n"  /* torch */
		"s: 50: 4\n"  /* torch */
		"t: 53: 6\n"  /* woodstairs */
		"u: 53: 3\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaa"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "bbbbaaabbbb"
		/*  1 */ "baaaaaaaaab"
		/*  2 */ "baaaaaaaaab"
		/*  3 */ "baaaaaaaaab"
		/*  4 */ "baaaaaaaaab"
		/*  5 */ "baaaaaaaaab"
		/*  6 */ "baaaaaaaaab"
		/*  7 */ "baaaaaaaaab"
		/*  8 */ "bbbbaaabbbb"

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "....cde...."
		/*  1 */ ".fffffffff."
		/*  2 */ ".fffffffff."
		/*  3 */ ".fffffffff."
		/*  4 */ ".fffffffff."
		/*  5 */ ".fffffffff."
		/*  6 */ ".fffffffff."
		/*  7 */ ".fffffffff."
		/*  8 */ "....cge...."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".hiihjhiih."
		/*  2 */ ".i.......i."
		/*  3 */ ".i.......i."
		/*  4 */ ".h.......h."
		/*  5 */ ".i.......i."
		/*  6 */ ".i.......i."
		/*  7 */ ".hiihjhiih."
		/*  8 */ "..........."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".hkkhlhkkh."
		/*  2 */ ".k.......k."
		/*  3 */ ".k.......k."
		/*  4 */ ".h.......h."
		/*  5 */ ".k.......k."
		/*  6 */ ".k.......k."
		/*  7 */ ".hkkhlhkkh."
		/*  8 */ "..........."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "nnnnnnnnnnn"
		/*  1 */ "ohpppppppho"
		/*  2 */ ".q..r.r..q."
		/*  3 */ ".q.......q."
		/*  4 */ ".q.......q."
		/*  5 */ ".q.......q."
		/*  6 */ ".q..s.s..q."
		/*  7 */ "thpppppppht"
		/*  8 */ "uuuuuuuuuuu"

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "nnnnnnnnnnn"
		/*  2 */ "oiiiiiiiiio"
		/*  3 */ ".i.......i."
		/*  4 */ ".i.......i."
		/*  5 */ ".i.......i."
		/*  6 */ "tiiiiiiiiit"
		/*  7 */ "uuuuuuuuuuu"
		/*  8 */ "..........."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "nnnnnnnnnnn"
		/*  3 */ "oiiiiiiiiio"
		/*  4 */ ".i.......i."
		/*  5 */ "tiiiiiiiiit"
		/*  6 */ "uuuuuuuuuuu"
		/*  7 */ "..........."
		/*  8 */ "..........."

		// Level 8
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "..........."
		/*  3 */ "nnnnnnnnnnn"
		/*  4 */ "iiiiiiiiiii"
		/*  5 */ "uuuuuuuuuuu"
		/*  6 */ "..........."
		/*  7 */ "..........."
		/*  8 */ "...........",

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
	},  // WoodenHouse9x7DoubleDoor



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouseL13x14:
	// The data has been exported from the gallery Plains, area index 39, ID 90, created by STR_Warrior
	{
		// Size:
		15, 10, 16,  // SizeX = 15, SizeY = 10, SizeZ = 16

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		14, 9, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"A: 50: 4\n"  /* torch */
		"B: 50: 2\n"  /* torch */
		"C: 53: 7\n"  /* woodstairs */
		"D: 53: 4\n"  /* woodstairs */
		"E: 53: 5\n"  /* woodstairs */
		"F: 53: 6\n"  /* woodstairs */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 43: 0\n"  /* doubleslab */
		"h: 17: 0\n"  /* tree */
		"i:  5: 0\n"  /* wood */
		"j: 64: 7\n"  /* wooddoorblock */
		"k: 96: 8\n"  /* trapdoor */
		"l: 61: 2\n"  /* furnace */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 3\n"  /* woodstairs */
		"o: 85: 0\n"  /* fence */
		"p: 53: 2\n"  /* woodstairs */
		"q: 53: 1\n"  /* woodstairs */
		"r: 53: 0\n"  /* woodstairs */
		"s: 47: 0\n"  /* bookshelf */
		"t:102: 0\n"  /* glasspane */
		"u: 64:12\n"  /* wooddoorblock */
		"v: 72: 0\n"  /* woodplate */
		"w: 17: 4\n"  /* tree */
		"x: 17: 8\n"  /* tree */
		"y: 50: 3\n"  /* torch */
		"z: 50: 1\n"  /* torch */,

		// Block data:
		// Level 0
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "aaaaaaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaaaaaa"
		/*  8 */ "mmmmmmmmaaaaaaa"
		/*  9 */ "mmmmmmmmaaaaaaa"
		/* 10 */ "mmmmmmmmaaaaaaa"
		/* 11 */ "mmmmmmmmaaaaaaa"
		/* 12 */ "mmmmmmmmaaaaaaa"
		/* 13 */ "mmmmmmmmaaaaaaa"
		/* 14 */ "mmmmmmmmaaaaaaa"
		/* 15 */ "mmmmmmmmaaaaaaa"

		// Level 1
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "bbbbbbaaabbbbbb"
		/*  1 */ "baaaaaaaaaaaaab"
		/*  2 */ "baaaaaaaaaaaaab"
		/*  3 */ "baaaaaaaaaaaaab"
		/*  4 */ "baaaaaaaaaaaaab"
		/*  5 */ "baaaaaaaaaaaaab"
		/*  6 */ "baaaaaaaaaaaaab"
		/*  7 */ "baaaaaaaaaaaaab"
		/*  8 */ "mmmmmmmmaaaaaab"
		/*  9 */ "mmmmmmmmaaaaaab"
		/* 10 */ "mmmmmmmmaaaaaab"
		/* 11 */ "mmmmmmmmaaaaaab"
		/* 12 */ "mmmmmmmmaaaaaab"
		/* 13 */ "mmmmmmmmaaaaaab"
		/* 14 */ "mmmmmmmmaaaaaab"
		/* 15 */ "mmmmmmmmbbbbbbb"

		// Level 2
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "......cde......"
		/*  1 */ ".fffffffffffff."
		/*  2 */ ".fggggffffffff."
		/*  3 */ ".fggggffffffff."
		/*  4 */ ".fffffffffffff."
		/*  5 */ ".fffffffffffff."
		/*  6 */ ".fffffffffffff."
		/*  7 */ "mfffffffffffff."
		/*  8 */ "mmmmmmmmffffff."
		/*  9 */ "mmmmmmmmffffff."
		/* 10 */ "mmmmmmmmffffff."
		/* 11 */ "mmmmmmmmffffff."
		/* 12 */ "mmmmmmmmffffff."
		/* 13 */ "mmmmmmmmffffff."
		/* 14 */ "mmmmmmmmffffff."
		/* 15 */ "mmmmmmmm......."

		// Level 3
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".hiiiihjhiiiih."
		/*  2 */ ".i...k.......i."
		/*  3 */ ".ilggg......ni."
		/*  4 */ ".h..........oi."
		/*  5 */ ".i..........pi."
		/*  6 */ ".i.qor..qo...i."
		/*  7 */ "mhiiiiiihp...h."
		/*  8 */ "mmmmmmmmi....i."
		/*  9 */ "mmmmmmmmin...i."
		/* 10 */ "mmmmmmmmio..ni."
		/* 11 */ "mmmmmmmmip..oi."
		/* 12 */ "mmmmmmmmi...pi."
		/* 13 */ "mmmmmmmmis..si."
		/* 14 */ "mmmmmmmmhiiiih."
		/* 15 */ "mmmmmmmm......."

		// Level 4
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".hittihuhittih."
		/*  2 */ ".i...........i."
		/*  3 */ ".i...........t."
		/*  4 */ ".h..........vt."
		/*  5 */ ".i...........t."
		/*  6 */ ".i..v....v...i."
		/*  7 */ "mhittttih....h."
		/*  8 */ "mmmmmmmmi....i."
		/*  9 */ "mmmmmmmmt....t."
		/* 10 */ "mmmmmmmmtv...t."
		/* 11 */ "mmmmmmmmt...vt."
		/* 12 */ "mmmmmmmmt....t."
		/* 13 */ "mmmmmmmmis..si."
		/* 14 */ "mmmmmmmmhittih."
		/* 15 */ "mmmmmmmm......."

		// Level 5
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".hwwwwwwwwwwwh."
		/*  2 */ ".x....y.y....x."
		/*  3 */ ".x...........x."
		/*  4 */ ".xz..........x."
		/*  5 */ ".x...........x."
		/*  6 */ ".x......A....x."
		/*  7 */ "mhwwwwwwhz..Bx."
		/*  8 */ "mmmmmmmmx....x."
		/*  9 */ "mmmmmmmmx....x."
		/* 10 */ "mmmmmmmmx....x."
		/* 11 */ "mmmmmmmmx....x."
		/* 12 */ "mmmmmmmmx....x."
		/* 13 */ "mmmmmmmmx.AA.x."
		/* 14 */ "mmmmmmmmhwwwwh."
		/* 15 */ "mmmmmmmm......."

		// Level 6
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "ppppppppppppppq"
		/*  1 */ "riiiiiiiiiiiiiq"
		/*  2 */ "riCCCCCCCCCCDiq"
		/*  3 */ "riE.........Diq"
		/*  4 */ "riE.........Diq"
		/*  5 */ "riE.........Diq"
		/*  6 */ "riEFFFFFFF..Diq"
		/*  7 */ "riiiiiiiiE..Diq"
		/*  8 */ "rnnnnnnriE..Diq"
		/*  9 */ "mmmmmmmriE..Diq"
		/* 10 */ "mmmmmmmriE..Diq"
		/* 11 */ "mmmmmmmriE..Diq"
		/* 12 */ "mmmmmmmriE..Diq"
		/* 13 */ "mmmmmmmriEFFDiq"
		/* 14 */ "mmmmmmmriiiiiiq"
		/* 15 */ "mmmmmmmrnnnnnnn"

		// Level 7
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "mmmmmmmmmmmmmmm"
		/*  1 */ "mrpppppppppppqm"
		/*  2 */ "mriiiiiiiiiiiqm"
		/*  3 */ "mriiiiiiiiiiiqm"
		/*  4 */ "mriiiiiiiiiiiqm"
		/*  5 */ "mriiiiiiiiiiiqm"
		/*  6 */ "mriiiiiiiiiiiqm"
		/*  7 */ "mrnnnnnnniiiiqm"
		/*  8 */ "mmmmmmmmriiiiqm"
		/*  9 */ "mmmmmmmmriiiiqm"
		/* 10 */ "mmmmmmmmriiiiqm"
		/* 11 */ "mmmmmmmmriiiiqm"
		/* 12 */ "mmmmmmmmriiiiqm"
		/* 13 */ "mmmmmmmmriiiiqm"
		/* 14 */ "mmmmmmmmnnnnnqm"
		/* 15 */ "mmmmmmmmmmmmmmm"

		// Level 8
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "mmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmm"
		/*  2 */ "mmpppppppppppmm"
		/*  3 */ "mmriiiiiiiiiqmm"
		/*  4 */ "mmriiiiiiiiiqmm"
		/*  5 */ "mmriiiiiiiiiqmm"
		/*  6 */ "mmnnnnnnnniiqmm"
		/*  7 */ "mmmmmmmmmriiqmm"
		/*  8 */ "mmmmmmmmmriiqmm"
		/*  9 */ "mmmmmmmmmriiqmm"
		/* 10 */ "mmmmmmmmmriiqmm"
		/* 11 */ "mmmmmmmmmriiqmm"
		/* 12 */ "mmmmmmmmmriiqmm"
		/* 13 */ "mmmmmmmmmnnnqmm"
		/* 14 */ "mmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmm"

		// Level 9
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "mmmmmmmmmmmmmmm"
		/*  1 */ "mmmmmmmmmmmmmmm"
		/*  2 */ "mmmmmmmmmmmmmmm"
		/*  3 */ "mmmrpppppppqmmm"
		/*  4 */ "mmmriiiiiiiqmmm"
		/*  5 */ "mmmrnnnnnnrqmmm"
		/*  6 */ "mmmmmmmmmmrqmmm"
		/*  7 */ "mmmmmmmmmmrqmmm"
		/*  8 */ "mmmmmmmmmmrqmmm"
		/*  9 */ "mmmmmmmmmmrqmmm"
		/* 10 */ "mmmmmmmmmmrqmmm"
		/* 11 */ "mmmmmmmmmmrqmmm"
		/* 12 */ "mmmmmmmmmmrnmmm"
		/* 13 */ "mmmmmmmmmmmmmmm"
		/* 14 */ "mmmmmmmmmmmmmmm"
		/* 15 */ "mmmmmmmmmmmmmmm",

		// Connectors:
		"-1: 7, 2, 0: 2\n"  /* Type -1, direction Z- */,

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
	},  // WoodenHouseL13x14



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouseL14x14:
	// The data has been exported from the gallery Plains, area index 0, ID 4, created by Aloe_vera
	{
		// Size:
		16, 7, 16,  // SizeX = 16, SizeY = 7, SizeZ = 16

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		15, 6, 15,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a: 67: 0\n"  /* stairs */
		"b: 67: 2\n"  /* stairs */
		"c: 67: 1\n"  /* stairs */
		"d:  4: 0\n"  /* cobblestone */
		"e:  5: 0\n"  /* wood */
		"f: 67: 3\n"  /* stairs */
		"g: 17: 0\n"  /* tree */
		"h: 64: 7\n"  /* wooddoorblock */
		"i: 64: 5\n"  /* wooddoorblock */
		"j:102: 0\n"  /* glasspane */
		"k: 64:12\n"  /* wooddoorblock */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 1\n"  /* woodstairs */
		"o: 53: 7\n"  /* woodstairs */
		"p: 53: 6\n"  /* woodstairs */
		"q: 53: 3\n"  /* woodstairs */
		"r: 53: 0\n"  /* woodstairs */
		"s: 53: 5\n"  /* woodstairs */
		"t: 53: 4\n"  /* woodstairs */
		"u: 50: 3\n"  /* torch */
		"v: 50: 2\n"  /* torch */
		"w: 50: 4\n"  /* torch */
		"x: 50: 1\n"  /* torch */,

		// Block data:
		// Level 0
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "........abc....."
		/*  1 */ ".dddddddddddddd."
		/*  2 */ ".deeeeeeeeeeeed."
		/*  3 */ ".deeeeeeeeeeeed."
		/*  4 */ ".deeeeeeeeeeeed."
		/*  5 */ ".deeeeeeeeeeeed."
		/*  6 */ ".deeeeeeeeeeeed."
		/*  7 */ ".ddddddddeeeeed."
		/*  8 */ "mmmmmafcdeeeeed."
		/*  9 */ "mmmmmmmmdeeeeed."
		/* 10 */ "mmmmmmmmdeeeeed."
		/* 11 */ "mmmmmmmmdeeeeed."
		/* 12 */ "mmmmmmmmdeeeeed."
		/* 13 */ "mmmmmmmmdeeeeed."
		/* 14 */ "mmmmmmmmddddddd."
		/* 15 */ "mmmmmmmm........"

		// Level 1
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".geeeeeeghgeeeg."
		/*  2 */ ".e............e."
		/*  3 */ ".e............e."
		/*  4 */ ".e............e."
		/*  5 */ ".e............e."
		/*  6 */ ".e............e."
		/*  7 */ ".geeeeieg.....e."
		/*  8 */ "mmmmmm.me.....e."
		/*  9 */ "mmmmmmmme.....e."
		/* 10 */ "mmmmmmmme.....e."
		/* 11 */ "mmmmmmmme.....e."
		/* 12 */ "mmmmmmmme.....e."
		/* 13 */ "mmmmmmmme.....e."
		/* 14 */ "mmmmmmmmgeeeeeg."
		/* 15 */ "mmmmmmmm........"

		// Level 2
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ ".gejjejjgkgjjeg."
		/*  2 */ ".j............e."
		/*  3 */ ".j............j."
		/*  4 */ ".j............j."
		/*  5 */ ".j............e."
		/*  6 */ ".j............j."
		/*  7 */ ".gejjekeg.....j."
		/*  8 */ "mmmmmm.me.....e."
		/*  9 */ "mmmmmmmmj.....j."
		/* 10 */ "mmmmmmmmj.....j."
		/* 11 */ "mmmmmmmme.....e."
		/* 12 */ "mmmmmmmmj.....j."
		/* 13 */ "mmmmmmmmj.....j."
		/* 14 */ "mmmmmmmmgjjjjjg."
		/* 15 */ "mmmmmmmm........"

		// Level 3
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "llllllllllllllln"
		/*  1 */ "ogeeeeeegegeeegn"
		/*  2 */ ".e............en"
		/*  3 */ ".e............en"
		/*  4 */ ".e............en"
		/*  5 */ ".e............en"
		/*  6 */ ".e............en"
		/*  7 */ "pgeeeeeeg.....en"
		/*  8 */ "qqqqqqqre.....en"
		/*  9 */ "mmmmmmmre.....en"
		/* 10 */ "mmmmmmmre.....en"
		/* 11 */ "mmmmmmmre.....en"
		/* 12 */ "mmmmmmmre.....en"
		/* 13 */ "mmmmmmmre.....en"
		/* 14 */ "mmmmmmmrgeeeeegn"
		/* 15 */ "mmmmmmmrs.....tn"

		// Level 4
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "lllllllllllllll."
		/*  2 */ "oeeeeeeeeeeeeen."
		/*  3 */ ".e.........u.en."
		/*  4 */ ".e..........ven."
		/*  5 */ ".e......w....en."
		/*  6 */ "peeeeeeeee...en."
		/*  7 */ "qqqqqqqqrex..en."
		/*  8 */ "mmmmmmmmre...en."
		/*  9 */ "mmmmmmmmre...en."
		/* 10 */ "mmmmmmmmre...en."
		/* 11 */ "mmmmmmmmre...en."
		/* 12 */ "mmmmmmmmre...en."
		/* 13 */ "mmmmmmmmre...en."
		/* 14 */ "mmmmmmmmreeeeen."
		/* 15 */ "mmmmmmmmrs...tn."

		// Level 5
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "llllllllllllln.."
		/*  3 */ "oeeeeeeeeeeeen.."
		/*  4 */ ".ex.........en.."
		/*  5 */ "peeeeeeeeee.en.."
		/*  6 */ "qqqqqqqqqre.en.."
		/*  7 */ ".........re.en.."
		/*  8 */ "mmmmmmmm.re.en.."
		/*  9 */ "mmmmmmmm.re.en.."
		/* 10 */ "mmmmmmmm.re.en.."
		/* 11 */ "mmmmmmmm.re.en.."
		/* 12 */ "mmmmmmmm.re.en.."
		/* 13 */ "mmmmmmmm.rewen.."
		/* 14 */ "mmmmmmmm.reeen.."
		/* 15 */ "mmmmmmmm.rs.tn.."

		// Level 6
		/* z\x*             111111 */
		/*    *   0123456789012345 */
		/*  0 */ "................"
		/*  1 */ "................"
		/*  2 */ "................"
		/*  3 */ "lllllllllllll..."
		/*  4 */ "eeeeeeeeeeeen..."
		/*  5 */ "qqqqqqqqqqren..."
		/*  6 */ "mmmmmmmmmmren..."
		/*  7 */ "mmmmmmmmmmren..."
		/*  8 */ "mmmmmmmmmmren..."
		/*  9 */ "mmmmmmmmmmren..."
		/* 10 */ "mmmmmmmmmmren..."
		/* 11 */ "mmmmmmmmmmren..."
		/* 12 */ "mmmmmmmmmmren..."
		/* 13 */ "mmmmmmmmmmren..."
		/* 14 */ "mmmmmmmmmmren..."
		/* 15 */ "mmmmmmmmmmren...",

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
	},  // WoodenHouseL14x14



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouseL9x9:
	// The data has been exported from the gallery Plains, area index 42, ID 93, created by xoft
	{
		// Size:
		11, 8, 11,  // SizeX = 11, SizeY = 8, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		10, 7, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j:102: 0\n"  /* glasspane */
		"k: 64:12\n"  /* wooddoorblock */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 7\n"  /* woodstairs */
		"o: 53: 1\n"  /* woodstairs */
		"p: 50: 3\n"  /* torch */
		"q: 50: 4\n"  /* torch */
		"r: 53: 6\n"  /* woodstairs */
		"s: 50: 1\n"  /* torch */
		"t: 50: 2\n"  /* torch */
		"u: 53: 3\n"  /* woodstairs */
		"v: 53: 0\n"  /* woodstairs */
		"w: 53: 5\n"  /* woodstairs */
		"x: 53: 4\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "aaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaa"
		/*  9 */ "aaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaa"

		// Level 1
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "bbbbaaabbbb"
		/*  1 */ "baaaaaaaaab"
		/*  2 */ "baaaaaaaaab"
		/*  3 */ "baaaaaaaaab"
		/*  4 */ "baaaaaaaaab"
		/*  5 */ "baaaaaaaaab"
		/*  6 */ "bbbbbaaaaab"
		/*  7 */ "bbbbbaaaaab"
		/*  8 */ "bbbbbaaaaab"
		/*  9 */ "bbbbbaaaaab"
		/* 10 */ "bbbbbbbbbbb"

		// Level 2
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "....cde...."
		/*  1 */ ".fffffffff."
		/*  2 */ ".fffffffff."
		/*  3 */ ".fffffffff."
		/*  4 */ ".fffffffff."
		/*  5 */ ".fffffffff."
		/*  6 */ ".....fffff."
		/*  7 */ "mmmm.fffff."
		/*  8 */ "mmmm.fffff."
		/*  9 */ "mmmm.fffff."
		/* 10 */ "mmmm......."

		// Level 3
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".ghhhihhhg."
		/*  2 */ ".h.......h."
		/*  3 */ ".h.......h."
		/*  4 */ ".h.......h."
		/*  5 */ ".ghhhg...h."
		/*  6 */ ".....h...h."
		/*  7 */ "mmmm.h...h."
		/*  8 */ "mmmm.h...h."
		/*  9 */ "mmmm.ghhhg."
		/* 10 */ "mmmm......."

		// Level 4
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ ".gjjhkhjjg."
		/*  2 */ ".j.......j."
		/*  3 */ ".j.......j."
		/*  4 */ ".j.......j."
		/*  5 */ ".gjjjg...h."
		/*  6 */ ".....j...j."
		/*  7 */ "mmmm.j...j."
		/*  8 */ "mmmm.j...j."
		/*  9 */ "mmmm.gjjjg."
		/* 10 */ "mmmm......."

		// Level 5
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "lllllllllll"
		/*  1 */ "nhhhhhhhhho"
		/*  2 */ ".h..p.p..ho"
		/*  3 */ ".h.......ho"
		/*  4 */ ".h...q...ho"
		/*  5 */ "rhhhhhs.tho"
		/*  6 */ "uuuuuh...ho"
		/*  7 */ "mmmmvh...ho"
		/*  8 */ "mmmmvh...ho"
		/*  9 */ "mmmmvhhhhho"
		/* 10 */ "mmmmvw...xo"

		// Level 6
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "lllllllllo."
		/*  2 */ "nhhhhhhhho."
		/*  3 */ ".h......ho."
		/*  4 */ "rhhhhhh.ho."
		/*  5 */ "uuuuuuh.ho."
		/*  6 */ ".....vh.ho."
		/*  7 */ "mmmm.vh.ho."
		/*  8 */ "mmmm.vh.ho."
		/*  9 */ "mmmm.vhhho."
		/* 10 */ "mmmm.vw.xo."

		// Level 7
		/* z\x*             1 */
		/*    *   01234567890 */
		/*  0 */ "..........."
		/*  1 */ "..........."
		/*  2 */ "lllllllll.."
		/*  3 */ "hhhhhhhho.."
		/*  4 */ "uuuuuuvho.."
		/*  5 */ "......vho.."
		/*  6 */ "......vho.."
		/*  7 */ "mmmm..vho.."
		/*  8 */ "mmmm..vho.."
		/*  9 */ "mmmm..vho.."
		/* 10 */ "mmmm..vho..",

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
	},  // WoodenHouseL9x9



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenHouseU13x9:
	// The data has been exported from the gallery Plains, area index 43, ID 94, created by xoft
	{
		// Size:
		15, 8, 11,  // SizeX = 15, SizeY = 8, SizeZ = 11

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		14, 7, 10,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 64: 7\n"  /* wooddoorblock */
		"j:102: 0\n"  /* glasspane */
		"k: 64:12\n"  /* wooddoorblock */
		"l: 53: 2\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 0\n"  /* woodstairs */
		"o: 53: 1\n"  /* woodstairs */
		"p: 50: 3\n"  /* torch */
		"q: 50: 4\n"  /* torch */
		"r: 50: 2\n"  /* torch */
		"s: 50: 1\n"  /* torch */
		"t: 53: 3\n"  /* woodstairs */
		"u: 53: 5\n"  /* woodstairs */
		"v: 53: 4\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "aaaaaaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaaaaaa"
		/*  9 */ "aaaaaaaaaaaaaaa"
		/* 10 */ "aaaaaaaaaaaaaaa"

		// Level 1
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "bbbbbbaaabbbbbb"
		/*  1 */ "baaaaaaaaaaaaab"
		/*  2 */ "baaaaaaaaaaaaab"
		/*  3 */ "baaaaaaaaaaaaab"
		/*  4 */ "baaaaaaaaaaaaab"
		/*  5 */ "baaaaaaaaaaaaab"
		/*  6 */ "baaaaabbbaaaaab"
		/*  7 */ "baaaaabbbaaaaab"
		/*  8 */ "baaaaabbbaaaaab"
		/*  9 */ "baaaaabbbaaaaab"
		/* 10 */ "bbbbbbbbbbbbbbb"

		// Level 2
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "......cde......"
		/*  1 */ ".fffffffffffff."
		/*  2 */ ".fffffffffffff."
		/*  3 */ ".fffffffffffff."
		/*  4 */ ".fffffffffffff."
		/*  5 */ ".fffffffffffff."
		/*  6 */ ".fffff...fffff."
		/*  7 */ ".fffff...fffff."
		/*  8 */ ".fffff...fffff."
		/*  9 */ ".fffff...fffff."
		/* 10 */ "..............."

		// Level 3
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".ghhhhhihhhhhg."
		/*  2 */ ".h...........h."
		/*  3 */ ".h...........h."
		/*  4 */ ".h...........h."
		/*  5 */ ".h...ghhhg...h."
		/*  6 */ ".h...h...h...h."
		/*  7 */ ".h...h...h...h."
		/*  8 */ ".h...h...h...h."
		/*  9 */ ".ghhhg...ghhhg."
		/* 10 */ "..............."

		// Level 4
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".gjjjjhkhjjjjg."
		/*  2 */ ".j...........j."
		/*  3 */ ".j...........j."
		/*  4 */ ".j...........j."
		/*  5 */ ".h...gjjjg...h."
		/*  6 */ ".j...j...j...j."
		/*  7 */ ".j...j...j...j."
		/*  8 */ ".j...j...j...j."
		/*  9 */ ".gjjjg...gjjjg."
		/* 10 */ "..............."

		// Level 5
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "lllllllllllllll"
		/*  1 */ "nhhhhhhhhhhhhho"
		/*  2 */ "nh....p.p....ho"
		/*  3 */ "nh...........ho"
		/*  4 */ "nh...q...q...ho"
		/*  5 */ "nh..rhhhhhs..ho"
		/*  6 */ "nh...httth...ho"
		/*  7 */ "nh...ho.nh...ho"
		/*  8 */ "nh...ho.nh...ho"
		/*  9 */ "nhhhhho.nhhhhho"
		/* 10 */ "nu...vo.nu...vo"

		// Level 6
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".llllllllllllo."
		/*  2 */ ".nhhhhhhhhhhho."
		/*  3 */ ".nhhhhhhhhhhho."
		/*  4 */ ".nhhhhhhhhhhho."
		/*  5 */ ".nhhhotttthhho."
		/*  6 */ ".nhhho...nhhho."
		/*  7 */ ".nhhho...nhhho."
		/*  8 */ ".nhhho...nhhho."
		/*  9 */ ".nhhho...nhhho."
		/* 10 */ ".nu.vo...nu.vo."

		// Level 7
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "..............."
		/*  2 */ "..nllllllllll.."
		/*  3 */ "..nhhhhhhhhho.."
		/*  4 */ "..nhotttttnho.."
		/*  5 */ "..nho.....nho.."
		/*  6 */ "..nho.....nho.."
		/*  7 */ "..nho.....nho.."
		/*  8 */ "..nho.....nho.."
		/*  9 */ "..nho.....nho.."
		/* 10 */ "..nho.....nho..",

		// Connectors:
		"-1: 7, 2, 0: 2\n"  /* Type -1, direction Z- */,

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
	},  // WoodenHouseU13x9



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenMill5x5:
	// The data has been exported from the gallery Plains, area index 60, ID 111, created by Aloe_vera
	{
		// Size:
		9, 18, 13,  // SizeX = 9, SizeY = 18, SizeZ = 13

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		8, 17, 12,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c:  4: 0\n"  /* cobblestone */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f: 67: 3\n"  /* stairs */
		"g: 17: 0\n"  /* tree */
		"h:  5: 0\n"  /* wood */
		"i: 54: 4\n"  /* chest */
		"j:154: 4\n"  /* hopper */
		"k: 64: 4\n"  /* wooddoorblock */
		"l:102: 0\n"  /* glasspane */
		"m: 19: 0\n"  /* sponge */
		"n: 85: 0\n"  /* fence */
		"o: 64:12\n"  /* wooddoorblock */
		"p: 50: 2\n"  /* torch */
		"q: 35: 0\n"  /* wool */
		"r: 17: 4\n"  /* tree */
		"s: 17: 8\n"  /* tree */
		"t: 53: 2\n"  /* woodstairs */
		"u: 53: 7\n"  /* woodstairs */
		"v: 53: 6\n"  /* woodstairs */
		"w: 53: 3\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*   012345678 */
		/*  0 */ "aaaaaaaaa"
		/*  1 */ "aaaaaaaaa"
		/*  2 */ "aaaaaaaaa"
		/*  3 */ "aaaaaaaaa"
		/*  4 */ "aaaaaaaaa"
		/*  5 */ "aaaaaaaaa"
		/*  6 */ "aaaaaaaaa"
		/*  7 */ "aaaaaaaaa"
		/*  8 */ "aaaaaaaaa"
		/*  9 */ "aaaaaaaaa"
		/* 10 */ "aaaaaaaaa"
		/* 11 */ "aaaaaaaaa"
		/* 12 */ "aaaaaaaaa"

		// Level 1
		/* z\x*   012345678 */
		/*  0 */ "bbbbbbbbb"
		/*  1 */ "bbbbbbbbb"
		/*  2 */ "bbbbbbbbb"
		/*  3 */ "bbbbbbbbb"
		/*  4 */ "baaaaabbb"
		/*  5 */ "baaaaaabb"
		/*  6 */ "baaaaaabb"
		/*  7 */ "baaaaaabb"
		/*  8 */ "baaaaabbb"
		/*  9 */ "bbbbbbbbb"
		/* 10 */ "bbbbbbbbb"
		/* 11 */ "bbbbbbbbb"
		/* 12 */ "bbbbbbbbb"

		// Level 2
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "........."
		/*  4 */ ".ccccc..."
		/*  5 */ ".cccccd.."
		/*  6 */ ".ccccce.."
		/*  7 */ ".cccccf.."
		/*  8 */ ".ccccc..."
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 3
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "........."
		/*  4 */ ".ghhhg..."
		/*  5 */ ".h...h..."
		/*  6 */ ".hij.k..."
		/*  7 */ ".h...h..."
		/*  8 */ ".ghhhg..."
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 4
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "........."
		/*  4 */ ".glllg..."
		/*  5 */ ".l...h..."
		/*  6 */ ".l.n.o..."
		/*  7 */ ".l...h..."
		/*  8 */ ".glllg..."
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 5
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "........."
		/*  4 */ ".ghhhg..."
		/*  5 */ ".h..ph..."
		/*  6 */ ".h.n.h..."
		/*  7 */ ".h..ph..n"
		/*  8 */ ".ghhhg..q"
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 6
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "........."
		/*  4 */ ".grrrg..."
		/*  5 */ ".s...s..."
		/*  6 */ ".s.n.s..."
		/*  7 */ ".s...s..n"
		/*  8 */ ".grrrg..q"
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 7
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "........."
		/*  4 */ ".ghhhg..."
		/*  5 */ ".h...h..."
		/*  6 */ ".h.n.h..n"
		/*  7 */ ".h...h..q"
		/*  8 */ ".ghhhg..q"
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 8
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "........."
		/*  4 */ ".glllg..."
		/*  5 */ ".l...l..."
		/*  6 */ ".l.n.l..n"
		/*  7 */ ".l...l..q"
		/*  8 */ ".glllg..."
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 9
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.q"
		/*  1 */ "mmmmmmm.q"
		/*  2 */ "mmmmmmm.q"
		/*  3 */ "........."
		/*  4 */ ".ghhhg..."
		/*  5 */ ".h...h..n"
		/*  6 */ ".h.n.h..q"
		/*  7 */ ".h...h..q"
		/*  8 */ ".ghhhg..."
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 10
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.n"
		/*  1 */ "mmmmmmm.n"
		/*  2 */ "mmmmmmm.q"
		/*  3 */ "........q"
		/*  4 */ ".grrrg..q"
		/*  5 */ ".s...s..n"
		/*  6 */ ".s.n.s..q"
		/*  7 */ ".s...s..n"
		/*  8 */ ".grrrg..n"
		/*  9 */ "........."
		/* 10 */ "mmmmmmm.."
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 11
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.n"
		/*  3 */ "ttttttt.n"
		/*  4 */ "uhhhhhu.q"
		/*  5 */ ".h...h..q"
		/*  6 */ ".h.nrrrrr"
		/*  7 */ ".h...h..q"
		/*  8 */ "vhhhhhv.q"
		/*  9 */ "wwwwwww.n"
		/* 10 */ "mmmmmmm.n"
		/* 11 */ "mmmmmmm.."
		/* 12 */ "mmmmmmm.."

		// Level 12
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "mmmmmmm.."
		/*  4 */ "ttttttt.n"
		/*  5 */ "uhhhhhu.n"
		/*  6 */ ".h...h..q"
		/*  7 */ "vhhhhhv.n"
		/*  8 */ "wwwwwww.q"
		/*  9 */ "mmmmmmm.q"
		/* 10 */ "mmmmmmm.q"
		/* 11 */ "mmmmmmm.n"
		/* 12 */ "mmmmmmm.n"

		// Level 13
		/* z\x*   012345678 */
		/*  0 */ "mmmmmmm.."
		/*  1 */ "mmmmmmm.."
		/*  2 */ "mmmmmmm.."
		/*  3 */ "mmmmmmm.."
		/*  4 */ "mmmmmmm.."
		/*  5 */ "ttttttt.q"
		/*  6 */ "hhhhhhh.q"
		/*  7 */ "wwwwwww.n"
		/*  8 */ "mmmmmmm.."
		/*  9 */ "mmmmmmm.."
		/* 10 */ "mmmmmmm.q"
		/* 11 */ "mmmmmmm.q"
		/* 12 */ "mmmmmmm.q"

		// Level 14
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "........."
		/*  3 */ "........."
		/*  4 */ "........."
		/*  5 */ "........q"
		/*  6 */ "........n"
		/*  7 */ "........."
		/*  8 */ "........."
		/*  9 */ "........."
		/* 10 */ "........."
		/* 11 */ "........."
		/* 12 */ "........."

		// Level 15
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "........."
		/*  3 */ "........."
		/*  4 */ "........q"
		/*  5 */ "........q"
		/*  6 */ "........n"
		/*  7 */ "........."
		/*  8 */ "........."
		/*  9 */ "........."
		/* 10 */ "........."
		/* 11 */ "........."
		/* 12 */ "........."

		// Level 16
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "........."
		/*  3 */ "........."
		/*  4 */ "........q"
		/*  5 */ "........n"
		/*  6 */ "........."
		/*  7 */ "........."
		/*  8 */ "........."
		/*  9 */ "........."
		/* 10 */ "........."
		/* 11 */ "........."
		/* 12 */ "........."

		// Level 17
		/* z\x*   012345678 */
		/*  0 */ "........."
		/*  1 */ "........."
		/*  2 */ "........."
		/*  3 */ "........."
		/*  4 */ "........q"
		/*  5 */ "........n"
		/*  6 */ "........."
		/*  7 */ "........."
		/*  8 */ "........."
		/*  9 */ "........."
		/* 10 */ "........."
		/* 11 */ "........."
		/* 12 */ ".........",

		// Connectors:
		"-1: 8, 2, 6: 5\n"  /* Type -1, direction X+ */,

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
	},  // WoodenMill5x5



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// WoodenStables:
	// The data has been exported from the gallery Plains, area index 55, ID 106, created by Aloe_vera
	{
		// Size:
		15, 10, 9,  // SizeX = 15, SizeY = 10, SizeZ = 9

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		14, 9, 8,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  3: 0\n"  /* dirt */
		"b:  2: 0\n"  /* grass */
		"c: 67: 0\n"  /* stairs */
		"d: 67: 2\n"  /* stairs */
		"e: 67: 1\n"  /* stairs */
		"f:  4: 0\n"  /* cobblestone */
		"g: 17: 0\n"  /* tree */
		"h:107: 0\n"  /* fencegate */
		"i:107: 4\n"  /* fencegate */
		"j:  5: 0\n"  /* wood */
		"k:107: 6\n"  /* fencegate */
		"l: 85: 0\n"  /* fence */
		"m: 19: 0\n"  /* sponge */
		"n:170: 0\n"  /* haybale */
		"o:170: 4\n"  /* haybale */
		"p:170: 8\n"  /* haybale */
		"q: 50: 1\n"  /* torch */
		"r: 50: 2\n"  /* torch */
		"s: 53: 2\n"  /* woodstairs */
		"t: 53: 7\n"  /* woodstairs */
		"u: 53: 6\n"  /* woodstairs */
		"v: 53: 3\n"  /* woodstairs */,

		// Block data:
		// Level 0
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "aaaaaaaaaaaaaaa"
		/*  1 */ "aaaaaaaaaaaaaaa"
		/*  2 */ "aaaaaaaaaaaaaaa"
		/*  3 */ "aaaaaaaaaaaaaaa"
		/*  4 */ "aaaaaaaaaaaaaaa"
		/*  5 */ "aaaaaaaaaaaaaaa"
		/*  6 */ "aaaaaaaaaaaaaaa"
		/*  7 */ "aaaaaaaaaaaaaaa"
		/*  8 */ "aaaaaaaaaaaaaaa"

		// Level 1
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "baaaaaaaaaaaaab"
		/*  1 */ "baaaaaaaaaaaaab"
		/*  2 */ "baaaaaaaaaaaaab"
		/*  3 */ "baaaaaaaaaaaaab"
		/*  4 */ "baaaaaaaaaaaaab"
		/*  5 */ "baaaaaaaaaaaaab"
		/*  6 */ "baaaaaaaaaaaaab"
		/*  7 */ "baaaaaaaaaaaaab"
		/*  8 */ "bbbbbbbbbbbbbbb"

		// Level 2
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ ".cddddddddddde."
		/*  1 */ ".fffffffffffff."
		/*  2 */ ".faaaaaaaaaaaf."
		/*  3 */ ".faaaaaaaaaaaf."
		/*  4 */ ".faaaaaaaaaaaf."
		/*  5 */ ".faaaaaaaaaaaf."
		/*  6 */ ".faaaaaaaaaaaf."
		/*  7 */ ".fffffffffffff."
		/*  8 */ "..............."

		// Level 3
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".ghihjhihjhkhg."
		/*  2 */ ".l...l...l...l."
		/*  3 */ ".l...l...l...l."
		/*  4 */ ".l...l...l...l."
		/*  5 */ ".l...l...l...l."
		/*  6 */ ".ln..l..olp..l."
		/*  7 */ ".gllljllljlllg."
		/*  8 */ "..............."

		// Level 4
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".g...j...j...g."
		/*  2 */ "..............."
		/*  3 */ "..............."
		/*  4 */ "..............."
		/*  5 */ "..............."
		/*  6 */ "..............."
		/*  7 */ ".g...j...j...g."
		/*  8 */ "..............."

		// Level 5
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ ".gq.rjq.rjq.rg."
		/*  2 */ "..............."
		/*  3 */ "..............."
		/*  4 */ "..............."
		/*  5 */ "..............."
		/*  6 */ "..............."
		/*  7 */ ".g...j...j...g."
		/*  8 */ "..............."

		// Level 6
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "sssssssssssssss"
		/*  1 */ "tjjjjjjjjjjjjjt"
		/*  2 */ ".j...........j."
		/*  3 */ ".j...........j."
		/*  4 */ ".j...........j."
		/*  5 */ ".j...........j."
		/*  6 */ ".j...........j."
		/*  7 */ "ujjjjjjjjjjjjju"
		/*  8 */ "vvvvvvvvvvvvvvv"

		// Level 7
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "sssssssssssssss"
		/*  2 */ "tjjjjjjjjjjjjjt"
		/*  3 */ ".j...........j."
		/*  4 */ ".j...........j."
		/*  5 */ ".j...........j."
		/*  6 */ "ujjjjjjjjjjjjju"
		/*  7 */ "vvvvvvvvvvvvvvv"
		/*  8 */ "..............."

		// Level 8
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "..............."
		/*  2 */ "sssssssssssssss"
		/*  3 */ "tjjjjjjjjjjjjjt"
		/*  4 */ ".j...........j."
		/*  5 */ "ujjjjjjjjjjjjju"
		/*  6 */ "vvvvvvvvvvvvvvv"
		/*  7 */ "..............."
		/*  8 */ "..............."

		// Level 9
		/* z\x*             11111 */
		/*    *   012345678901234 */
		/*  0 */ "..............."
		/*  1 */ "..............."
		/*  2 */ "..............."
		/*  3 */ "sssssssssssssss"
		/*  4 */ "jjjjjjjjjjjjjjj"
		/*  5 */ "vvvvvvvvvvvvvvv"
		/*  6 */ "..............."
		/*  7 */ "..............."
		/*  8 */ "...............",

		// Connectors:
		"-1: 7, 2, -1: 2\n"  /* Type -1, direction Z- */,

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
	},  // WoodenStables
};  // g_PlainsVillagePrefabs






const cPrefab::sDef g_PlainsVillageStartingPrefabs[] =
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CobbleWell4x4:
	// The data has been exported from the gallery Plains, area index 1, ID 5, created by Aloe_vera
	{
		// Size:
		4, 13, 4,  // SizeX = 4, SizeY = 13, SizeZ = 4

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		3, 12, 3,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b:  4: 0\n"  /* cobblestone */
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
		/*  1 */ "bccb"
		/*  2 */ "bccb"
		/*  3 */ "bbbb"

		// Level 9
		/* z\x*   0123 */
		/*  0 */ "bbbb"
		/*  1 */ "b..b"
		/*  2 */ "b..b"
		/*  3 */ "bbbb"

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
		"2: 1, 9, 3: 3\n"  /* Type 2, direction Z+ */
		"2: 2, 9, 0: 2\n"  /* Type 2, direction Z- */
		"2: 0, 9, 1: 4\n"  /* Type 2, direction X- */
		"2: 3, 9, 2: 5\n"  /* Type 2, direction X+ */,

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
	},  // CobbleWell4x4



	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// RoofedWell:
	// The data has been exported from the gallery Plains, area index 119, ID 271, created by STR_Warrior
	{
		// Size:
		7, 15, 7,  // SizeX = 7, SizeY = 15, SizeZ = 7

		// Hitbox (relative to bounding box):
		0, 0, 0,  // MinX, MinY, MinZ
		6, 14, 6,  // MaxX, MaxY, MaxZ

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:  1: 0\n"  /* stone */
		"b:  4: 0\n"  /* cobblestone */
		"c:  8: 0\n"  /* water */
		"d:  3: 0\n"  /* dirt */
		"e:  2: 0\n"  /* grass */
		"f: 13: 0\n"  /* gravel */
		"g:118: 3\n"  /* cauldronblock */
		"h: 85: 0\n"  /* fence */
		"i: 53: 2\n"  /* woodstairs */
		"j: 53: 7\n"  /* woodstairs */
		"k:  5: 0\n"  /* wood */
		"l: 53: 4\n"  /* woodstairs */
		"m: 19: 0\n"  /* sponge */
		"n: 53: 5\n"  /* woodstairs */
		"o: 53: 6\n"  /* woodstairs */
		"p: 53: 3\n"  /* woodstairs */,

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
		/*  0 */ "aaaaaaa"
		/*  1 */ "abbbbba"
		/*  2 */ "abcccba"
		/*  3 */ "abcccba"
		/*  4 */ "abcccba"
		/*  5 */ "abbbbba"
		/*  6 */ "aaaaaaa"

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
		/*  0 */ "ddddddd"
		/*  1 */ "dbbbbbd"
		/*  2 */ "dbcccbd"
		/*  3 */ "dbcccbd"
		/*  4 */ "dbcccbd"
		/*  5 */ "dbbbbbd"
		/*  6 */ "ddddddd"

		// Level 8
		/* z\x*   0123456 */
		/*  0 */ "eefffee"
		/*  1 */ "ebbbbbe"
		/*  2 */ "fbcccbf"
		/*  3 */ "fbcccbf"
		/*  4 */ "fbcccbf"
		/*  5 */ "ebbbbbe"
		/*  6 */ "eefffee"

		// Level 9
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ ".bbbbb."
		/*  2 */ ".b...b."
		/*  3 */ ".b.g.b."
		/*  4 */ ".b...b."
		/*  5 */ ".bbbbb."
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
		/*  0 */ "......."
		/*  1 */ ".h...h."
		/*  2 */ "......."
		/*  3 */ "...h..."
		/*  4 */ "......."
		/*  5 */ ".h...h."
		/*  6 */ "......."

		// Level 12
		/* z\x*   0123456 */
		/*  0 */ "iiiiiii"
		/*  1 */ "jkjjjkj"
		/*  2 */ ".l...n."
		/*  3 */ ".l.h.n."
		/*  4 */ ".l...n."
		/*  5 */ "okoooko"
		/*  6 */ "ppppppp"

		// Level 13
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "iiiiiii"
		/*  2 */ "jkjjjkj"
		/*  3 */ ".k.h.k."
		/*  4 */ "okoooko"
		/*  5 */ "ppppppp"
		/*  6 */ "......."

		// Level 14
		/* z\x*   0123456 */
		/*  0 */ "......."
		/*  1 */ "......."
		/*  2 */ "iiiiiii"
		/*  3 */ "kkkkkkk"
		/*  4 */ "ppppppp"
		/*  5 */ "......."
		/*  6 */ ".......",

		// Connectors:
		"2: 0, 9, 3: 4\n"  /* Type 2, direction X- */
		"2: 3, 9, 6: 3\n"  /* Type 2, direction Z+ */
		"2: 6, 9, 3: 5\n"  /* Type 2, direction X+ */
		"2: 3, 9, 0: 2\n"  /* Type 2, direction Z- */,

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
	},  // RoofedWell
};





// The prefab counts:

const size_t g_PlainsVillagePrefabsCount = ARRAYCOUNT(g_PlainsVillagePrefabs);

const size_t g_PlainsVillageStartingPrefabsCount = ARRAYCOUNT(g_PlainsVillageStartingPrefabs);

