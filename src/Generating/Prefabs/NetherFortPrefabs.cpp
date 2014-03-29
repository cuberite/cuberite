
// NetherFortPrefabs.cpp

// Defines all the prefabs for nether forts

#include "Globals.h"
#include "NetherFortPrefabs.h"





/*
The nether fortress has two types of connectors, Outer and Inner. Outer is Type 0, Inner is Type 1.
*/





const cPrefab::sDef g_NetherFortPrefabs1[] =
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BalconyCorridor:
	// The data has been exported from gallery Nether, area index 37, ID 288
	{
		// Size:
		13, 7, 9,  // SizeX = 13, SizeY = 7, SizeZ = 9

		// Block definitions:
		"a:112: 0\n"  /* netherbrick */
		"b: 19: 0\n"  /* sponge */
		"c:114: 4\n"  /* netherbrickstairs */
		"d:114: 7\n"  /* netherbrickstairs */
		"e:114: 5\n"  /* netherbrickstairs */
		"f: 44: 6\n"  /* step */
		"g:113: 0\n"  /* netherbrickfence */
		"h:114: 2\n"  /* netherbrickstairs */
		"i:114: 3\n"  /* netherbrickstairs */
		"j:114: 0\n"  /* netherbrickstairs */
		"k:114: 1\n"  /* netherbrickstairs */
		".:  0: 0\n"  /* air */,

		// Block data:
		// Level 1
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"bbbbaaaaabbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"

		// Level 2
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaa.aaa.aaaa"
		"bbcdaaaaadebb"
		"bbbcdddddebbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"

		// Level 3
		"aaaaaaaaaaaaa"
		"............."
		"............."
		"............."
		"aaaa.fff.aaaa"
		"bbaaaaaaaaabb"
		"bbaaaaaaaaabb"
		"bbaaaaaaaaabb"
		"bbaaaaaaaaabb"

		// Level 4
		"agagagagagaga"
		"............."
		"............."
		"............."
		"agaa.....aaga"
		"bbaaa...aaabb"
		"bbg.......gbb"
		"bbg.......gbb"
		"bbgggggggggbb"

		// Level 5
		"agagagagagaga"
		"............."
		"............."
		"............."
		"agaa.....aaga"
		"bbaaa...aaabb"
		"bb.........bb"
		"bb.........bb"
		"bb.........bb"

		// Level 6
		"agagagagagaga"
		"............."
		"............."
		"............."
		"agaa.....aaga"
		"bbaaa...aaabb"
		"bb.........bb"
		"bb.........bb"
		"bb.........bb"

		// Level 7
		"hhhhhhhhhhhhh"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"iijaaaaaaaiii"
		"bbjiiiiiiikbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb",
		
		// Connections:
		"1:  0, 2, 2: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1: 12, 2, 2: 5\n"  /* Type 1, BLOCK_FACE_XP */,
		
		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BalconyCorridor


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BalconyTee2:
	// The data has been exported from gallery Nether, area index 38, ID 289
	{
		// Size:
		13, 7, 11,  // SizeX = 13, SizeY = 7, SizeZ = 11

		// Block definitions:
		"a: 19: 0\n"  /* sponge */
		"b:112: 0\n"  /* netherbrick */
		"c:114: 4\n"  /* netherbrickstairs */
		"d:114: 7\n"  /* netherbrickstairs */
		"e:114: 5\n"  /* netherbrickstairs */
		"f: 44: 6\n"  /* step */
		"g:113: 0\n"  /* netherbrickfence */
		"h:114: 0\n"  /* netherbrickstairs */
		"i:114: 1\n"  /* netherbrickstairs */
		"j:114: 2\n"  /* netherbrickstairs */
		"k:114: 3\n"  /* netherbrickstairs */
		".:  0: 0\n"  /* air */,

		// Block data:
		// Level 1
		"aaaabbbbbaaaa"
		"aaaabbbbbaaaa"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"aaaabbbbbaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 2
		"aaaabbbbbaaaa"
		"aaaabbbbbaaaa"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbb.bbb.bbbb"
		"aacdbbbbbdeaa"
		"aaacdddddeaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 3
		"aaaab...baaaa"
		"aaaab...baaaa"
		"bbbbb...bbbbb"
		"............."
		"............."
		"............."
		"bbbb.fff.bbbb"
		"aabbbbbbbbbaa"
		"aabbbbbbbbbaa"
		"aabbbbbbbbbaa"
		"aabbbbbbbbbaa"

		// Level 4
		"aaaab...baaaa"
		"aaaag...gaaaa"
		"bgbgb...bgbgb"
		"............."
		"............."
		"............."
		"bgbb.....bbgb"
		"aabbb...bbbaa"
		"aag.......gaa"
		"aag.......gaa"
		"aagggggggggaa"

		// Level 5
		"aaaab...baaaa"
		"aaaag...gaaaa"
		"bgbgb...bgbgb"
		"............."
		"............."
		"............."
		"bgbb.....bbgb"
		"aabbb...bbbaa"
		"aa.........aa"
		"aa.........aa"
		"aa.........aa"

		// Level 6
		"aaaab...baaaa"
		"aaaag...gaaaa"
		"bgbgb...bgbgb"
		"............."
		"............."
		"............."
		"bgbb.....bbgb"
		"aabbb...bbbaa"
		"aa.........aa"
		"aa.........aa"
		"aa.........aa"

		// Level 7
		"aaaahbbbiaaaa"
		"aaaahbbbiaaaa"
		"jjjjjbbbjjjjj"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"
		"kkhbbbbbbbkkk"
		"aahkkkkkkkiaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa",
		
		// Connections:
		"1:  0, 2, 4: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1: 12, 2, 4: 5\n"  /* Type 1, BLOCK_FACE_XP */
		"1:  6, 2, 0: 2\n"  /* Type 1, BLOCK_FACE_ZM */,
		
		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BalconyTee2


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BlazePlatform
	// The data has been exported from gallery Nether, area index 26, ID 276
	{
		// Size:
		10, 7, 7,  // SizeX = 10, SizeY = 7, SizeZ = 7

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b: 52: 0\n"  /* mobspawner */
		"c:113: 0\n"  /* netherbrickfence */,

		// Block data:
		// Level 1
		".........."
		"aaaaaaaaaa"
		"aaaaaaaaaa"
		"aaaaaaaaaa"
		"aaaaaaaaaa"
		"aaaaaaaaaa"
		".........."

		// Level 2
		".........."
		"aaaaaaaaaa"
		"..aaaaaaaa"
		"..aaaaaaaa"
		"..aaaaaaaa"
		"aaaaaaaaaa"
		".........."

		// Level 3
		"....aaaaaa"
		"aaaaaaaaaa"
		"...aaaaaaa"
		"...aaaaaaa"
		"...aaaaaaa"
		"aaaaaaaaaa"
		"....aaaaaa"

		// Level 4
		"....aaaaaa"
		"..aaa....a"
		".........a"
		"......b..a"
		".........a"
		"..aaa....a"
		"....aaaaaa"

		// Level 5
		"....cccccc"
		"...cc....c"
		".........c"
		".........c"
		".........c"
		"...cc....c"
		"....cccccc"

		// Level 6
		".........."
		".........c"
		".........c"
		".........c"
		".........c"
		".........c"
		".........."

		// Level 7
		".........."
		".........."
		".........c"
		".........c"
		".........c"
		".........."
		"..........",

		// Connections:
		"0:  0, 1, 3: 4\n"  /* Type 0, BLOCK_FACE_XM */,
		
		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BlazePlatform


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BlazePlatformOverhang:
	// The data has been exported from gallery Nether, area index 20, ID 162
	{
		// Size:
		14, 9, 7,  // SizeX = 14, SizeY = 9, SizeZ = 7

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:114: 5\n"  /* netherbrickstairs */
		"c: 44:14\n"  /* step */
		"d:114: 6\n"  /* netherbrickstairs */
		"e:114: 7\n"  /* netherbrickstairs */
		"f:114: 0\n"  /* netherbrickstairs */
		"g:114: 4\n"  /* netherbrickstairs */
		"h:113: 0\n"  /* netherbrickfence */
		"i: 52: 0\n"  /* mobspawner */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"mmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmm"
		"aammmmmmmmmmmm"
		"aammmmmmmmmmmm"
		"aammmmmmmmmmmm"
		"mmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmm"

		// Level 2
		"mmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmm"
		"aabcmmmmmmmmmm"
		"aabcmmmmmmmmmm"
		"aabcmmmmmmmmmm"
		"mmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmm"

		// Level 3
		"mmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmm"
		"aaaaabmmmmmmmm"
		"aaaaabmmmmmmmm"
		"aaaaabmmmmmmmm"
		"mmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmm"

		// Level 4
		"mmmmmmmmmmmmmm"
		"dddddddmmmmmmm"
		"aaaaaabmmmmmmm"
		"aaaaaabmmmmmmm"
		"aaaaaabmmmmmmm"
		"eeeeeeemmmmmmm"
		"mmmmmmmmmmmmmm"

		// Level 5
		"mmmmmmmmmmmmmm"
		"aaaaaaadmmmmmm"
		"aaaaaaabmmmmmm"
		"aaaaaaabmmmmmm"
		"aaaaaaabmmmmmm"
		"aaaaaaaemmmmmm"
		"mmmmmmmmmmmmmm"

		// Level 6
		"mmmmmmmmmmmmmm"
		"aaaaaaaabddddm"
		"......faaaaabm"
		"......faaaaabm"
		"......faaaaabm"
		"aaaaaaaaabeebm"
		"mmmmmmmmmmmmmm"

		// Level 7
		"mmmmmmmmgdddbm"
		"......aaaaaaad"
		".......faaaaab"
		".......faaaaab"
		".......faaaaab"
		"......aaaaaaae"
		"mmmmmmmmgeeebm"

		// Level 8
		"mmmmmmmmaaaaam"
		"......haa...aa"
		".............a"
		"..........i..a"
		".............a"
		"......haa...aa"
		"mmmmmmmmaaaaam"

		// Level 9
		"mmmmmmmmhhhhhm"
		"......hhh...hh"
		".............h"
		".............h"
		".............h"
		"......hhh...hh"
		"mmmmmmmmhhhhhm",

		// Connections:
		"0:  0, 5, 3: 4\n"  /* Type 0, BLOCK_FACE_XM */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BlazePlatformOverhang


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BridgeCrossing:
	// The data has been exported from gallery Nether, area index 17, ID 159
	{
		// Size:
		15, 8, 15,  // SizeX = 15, SizeY = 8, SizeZ = 15

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:114: 7\n"  /* netherbrickstairs */
		"c:114: 5\n"  /* netherbrickstairs */
		"d:114: 4\n"  /* netherbrickstairs */
		"e:114: 6\n"  /* netherbrickstairs */
		"f: 44:14\n"  /* step */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"aammmmmmmmmmmaa"
		"aammmmmmmmmmmaa"
		"aammmmmmmmmmmaa"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"

		// Level 2
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmbbbmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"aacmmmmmmmmmdaa"
		"aacmmmmmmmmmdaa"
		"aacmmmmmmmmmdaa"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmeeemmmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"

		// Level 3
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmbbbmmmmmm"
		"mmmmmmfffmmmmmm"
		"mmmmmmmmmmmmmmm"
		"aaacfmmmmmfdaaa"
		"aaacfmmmmmfdaaa"
		"aaacfmmmmmfdaaa"
		"mmmmmmmmmmmmmmm"
		"mmmmmmfffmmmmmm"
		"mmmmmmeeemmmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"

		// Level 4
		"mmmmmdaaacmmmmm"
		"mmmmmdaaacmmmmm"
		"mmmmmdaaacmmmmm"
		"mmmmmdaaacmmmmm"
		"mmmmmdaaacmmmmm"
		"eeeeeeaaaeeeeee"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"bbbbbdaaacbbbbb"
		"mmmmmdaaacmmmmm"
		"mmmmmdaaacmmmmm"
		"mmmmmdaaacmmmmm"
		"mmmmmdaaacmmmmm"
		"mmmmmdaaacmmmmm"

		// Level 5
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"

		// Level 6
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"aaaaaa...aaaaaa"
		"..............."
		"..............."
		"..............."
		"aaaaaa...aaaaaa"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"

		// Level 7
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"..............."
		"..............."
		"..............."
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"

		// Level 8
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"..............."
		"..............."
		"..............."
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm",
		
		// Connections:
		"0:  0, 5,  7: 4\n"  /* Type 0, BLOCK_FACE_XM */
		"0:  7, 5,  0: 2\n"  /* Type 0, BLOCK_FACE_ZM */
		"0: 14, 5,  7: 5\n"  /* Type 0, BLOCK_FACE_XP */
		"0:  7, 5, 14: 3\n"  /* Type 0, BLOCK_FACE_ZP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BridgeCrossing


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BridgeCrumble1:
	// The data has been exported from gallery Nether, area index 19, ID 161
	{
		// Size:
		9, 6, 5,  // SizeX = 9, SizeY = 6, SizeZ = 5

		// Block definitions:
		".: 19: 0\n"  /* sponge */
		"a:112: 0\n"  /* netherbrick */
		"b:114: 5\n"  /* netherbrickstairs */
		"c: 44:14\n"  /* step */
		"d:114: 6\n"  /* netherbrickstairs */
		"e:114: 7\n"  /* netherbrickstairs */,

		// Block data:
		// Level 1
		"........."
		"aa......."
		"aa......."
		"aa......."
		"........."

		// Level 2
		"........."
		"aab......"
		"aab......"
		"aab......"
		"........."

		// Level 3
		"........."
		"aaabc...."
		"aaabc...."
		"aaabc...."
		"........."

		// Level 4
		"ddddddd.."
		"aaaaaaaa."
		"aaaaaaaaa"
		"aaaaaaa.."
		"eeeee...."

		// Level 5
		"aaaaaaaaa"
		"aaaaa...."
		"aaaaaa..."
		"aaaaaa..."
		"aaaaaaaa."

		// Level 6
		"aaaaaa..."
		"........."
		"........."
		"........."
		"aaaaaaa..",

		// Connections:
		"0:  0, 5, 2: 4\n"  /* Type 0, BLOCK_FACE_XM */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BridgeCrumble1


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BridgeCrumble2
	// The data has been exported from gallery Nether, area index 18, ID 160
	{
		// Size:
		13, 6, 5,  // SizeX = 13, SizeY = 6, SizeZ = 5

		// Block definitions:
		".: 19: 0\n"  /* sponge */
		"a:112: 0\n"  /* netherbrick */
		"b:114: 5\n"  /* netherbrickstairs */
		"c: 44:14\n"  /* step */
		"d:114: 6\n"  /* netherbrickstairs */
		"e:114: 7\n"  /* netherbrickstairs */,

		// Block data:
		// Level 1
		"............."
		"aa..........."
		"aa..........."
		"aa..........."
		"............."

		// Level 2
		"............."
		"aab.........."
		"aab.........."
		"aab.........."
		"............."

		// Level 3
		"............."
		"aaabc........"
		"aaabc........"
		"aaabc........"
		"............."

		// Level 4
		"ddddddddd...."
		"aaaaaaaaaaaaa"
		"aaaaaaaaa...."
		"aaaaaaaaaaaa."
		"eeeeeeeee...."

		// Level 5
		"aaaaaaaaaaaa."
		"aaaaaaaaaa..."
		"aaaaaaaaaaa.."
		"aaaaaaaaa...."
		"aaaaaaaaaaaaa"

		// Level 6
		"aaaaaaaaa...."
		"............."
		"............."
		"............."
		"aaaaaaaaaa...",

		// Connections:
		"0:  0, 5, 2: 4\n"  /* Type 0, BLOCK_FACE_XM */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BridgeCrumble2


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// BridgeSegment:
	// The data has been exported from gallery Nether, area index 16, ID 158
	{
		// Size:
		15, 8, 5,  // SizeX = 15, SizeY = 8, SizeZ = 5

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:114: 5\n"  /* netherbrickstairs */
		"c:114: 4\n"  /* netherbrickstairs */
		"d: 44:14\n"  /* step */
		"e:114: 6\n"  /* netherbrickstairs */
		"f:114: 7\n"  /* netherbrickstairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"mmmmmmmmmmmmmmm"
		"aammmmmmmmmmmaa"
		"aammmmmmmmmmmaa"
		"aammmmmmmmmmmaa"
		"mmmmmmmmmmmmmmm"

		// Level 2
		"mmmmmmmmmmmmmmm"
		"aabmmmmmmmmmcaa"
		"aabmmmmmmmmmcaa"
		"aabmmmmmmmmmcaa"
		"mmmmmmmmmmmmmmm"

		// Level 3
		"mmmmmmmmmmmmmmm"
		"aaabdmmmmmdcaaa"
		"aaabdmmmmmdcaaa"
		"aaabdmmmmmdcaaa"
		"mmmmmmmmmmmmmmm"

		// Level 4
		"eeeeeeeeeeeeeee"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"fffffffffffffff"

		// Level 5
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"

		// Level 6
		"aaaaaaaaaaaaaaa"
		"..............."
		"..............."
		"..............."
		"aaaaaaaaaaaaaaa"

		// Level 7
		"mmmmmmmmmmmmmmm"
		"..............."
		"..............."
		"..............."
		"mmmmmmmmmmmmmmm"

		// Level 8
		"mmmmmmmmmmmmmmm"
		"..............."
		"..............."
		"..............."
		"mmmmmmmmmmmmmmm",

		// Connections:
		"0:  0, 5, 2: 4\n"  /* Type 0, BLOCK_FACE_XM */
		"0: 14, 5, 2: 5\n"  /* Type 0, BLOCK_FACE_XP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BridgeSegment


	// BridgeTee:
	// The data has been exported from gallery Nether, area index 39, ID 290
	{
		// Size:
		15, 8, 10,  // SizeX = 15, SizeY = 8, SizeZ = 10

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:114: 5\n"  /* netherbrickstairs */
		"c:114: 4\n"  /* netherbrickstairs */
		"d:114: 6\n"  /* netherbrickstairs */
		"e: 44:14\n"  /* step */
		"f:114: 7\n"  /* netherbrickstairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"mmmmmmmmmmmmmmm"
		"aammmmmmmmmmmaa"
		"aammmmmmmmmmmaa"
		"aammmmmmmmmmmaa"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"

		// Level 2
		"mmmmmmmmmmmmmmm"
		"aabmmmmmmmmmcaa"
		"aabmmmmmmmmmcaa"
		"aabmmmmmmmmmcaa"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmmmmmmmmmm"
		"mmmmmmdddmmmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"

		// Level 3
		"mmmmmmmmmmmmmmm"
		"aaabemmmmmecaaa"
		"aaabemmmmmecaaa"
		"aaabemmmmmecaaa"
		"mmmmmmmmmmmmmmm"
		"mmmmmmeeemmmmmm"
		"mmmmmmdddmmmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"
		"mmmmmmaaammmmmm"

		// Level 4
		"ddddddddddddddd"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"fffffcaaabfffff"
		"mmmmmcaaabmmmmm"
		"mmmmmcaaabmmmmm"
		"mmmmmcaaabmmmmm"
		"mmmmmcaaabmmmmm"
		"mmmmmcaaabmmmmm"

		// Level 5
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"
		"mmmmmaaaaammmmm"

		// Level 6
		"aaaaaaaaaaaaaaa"
		"..............."
		"..............."
		"..............."
		"aaaaaa...aaaaaa"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"
		"mmmmma...ammmmm"

		// Level 7
		"mmmmmmmmmmmmmmm"
		"..............."
		"..............."
		"..............."
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"

		// Level 8
		"mmmmmmmmmmmmmmm"
		"..............."
		"..............."
		"..............."
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm"
		"mmmmmm...mmmmmm",

		// Connections:
		"0:  0, 5, 2: 4\n"  /* Type 0, BLOCK_FACE_XM */
		"0: 14, 5, 2: 5\n"  /* Type 0, BLOCK_FACE_XP */
		"0:  7, 5, 9: 3\n"  /* Type 0, BLOCK_FACE_ZP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // BridgeTee


	// Corridor11:
	// The data has been exported from gallery Nether, area index 36, ID 287
	{
		// Size:
		11, 6, 5,  // SizeX = 11, SizeY = 6, SizeZ = 5

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */
		"c:114: 2\n"  /* netherbrickstairs */
		"d:114: 3\n"  /* netherbrickstairs */,

		// Block data:
		// Level 1
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"

		// Level 2
		"aaaaaaaaaaa"
		"..........."
		"..........."
		"..........."
		"aaaaaaaaaaa"

		// Level 3
		"abababababa"
		"..........."
		"..........."
		"..........."
		"abababababa"

		// Level 4
		"abababababa"
		"..........."
		"..........."
		"..........."
		"abababababa"

		// Level 5
		"abababababa"
		"..........."
		"..........."
		"..........."
		"abababababa"

		// Level 6
		"ccccccccccc"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"ddddddddddd",

		// Connections:
		"1:  0, 1, 2: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1: 10, 1, 2: 5\n"  /* Type 1, BLOCK_FACE_XP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // Corridor11


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Corridor13:
	// The data has been exported from gallery Nether, area index 35, ID 286
	{
		// Size:
		13, 6, 5,  // SizeX = 13, SizeY = 6, SizeZ = 5

		// Block definitions:
		"a:112: 0\n"  /* netherbrick */
		".:  0: 0\n"  /* air */
		"c:113: 0\n"  /* netherbrickfence */
		"d:114: 2\n"  /* netherbrickstairs */
		"e:114: 3\n"  /* netherbrickstairs */,

		// Block data:
		// Level 1
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 2
		"aaaaaaaaaaaaa"
		"............."
		"............."
		"............."
		"aaaaaaaaaaaaa"

		// Level 3
		"acacacacacaca"
		"............."
		"............."
		"............."
		"acacacacacaca"

		// Level 4
		"acacacacacaca"
		"............."
		"............."
		"............."
		"acacacacacaca"

		// Level 5
		"acacacacacaca"
		"............."
		"............."
		"............."
		"acacacacacaca"

		// Level 6
		"ddddddddddddd"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"eeeeeeeeeeeee",
		
		// Connections:
		"1:  0, 1, 2: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1: 12, 1, 2: 5\n"  /* Type 1, BLOCK_FACE_XP */,
		
		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // Corridor13


	// CorridorCorner5:
	// The data has been exported from gallery Nether, area index 10, ID 40
	{
		// Size:
		11, 6, 11,  // SizeX = 11, SizeY = 6, SizeZ = 11

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */
		"c:114: 2\n"  /* netherbrickstairs */
		"d:114: 0\n"  /* netherbrickstairs */
		"e:114: 3\n"  /* netherbrickstairs */
		"f:114: 1\n"  /* netherbrickstairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaammmmmm"
		"aaaaammmmmm"
		"aaaaammmmmm"
		"aaaaammmmmm"
		"aaaaammmmmm"
		"aaaaammmmmm"

		// Level 2
		"aaaaaaaaaaa"
		"a.........."
		"a.........."
		"a.........."
		"a...aaaaaaa"
		"a...ammmmmm"
		"a...ammmmmm"
		"a...ammmmmm"
		"a...ammmmmm"
		"a...ammmmmm"
		"a...ammmmmm"

		// Level 3
		"abababababa"
		"b.........."
		"a.........."
		"b.........."
		"a...abababa"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"

		// Level 4
		"abababababa"
		"b.........."
		"a.........."
		"b.........."
		"a...abababa"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"

		// Level 5
		"abababababa"
		"b.........."
		"a.........."
		"b.........."
		"a...abababa"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"

		// Level 6
		"ccccccccccc"
		"daaaaaaaaaa"
		"daaaaaaaaaa"
		"daaaaaaaaaa"
		"daaaeeeeeee"
		"daaafmmmmmm"
		"daaafmmmmmm"
		"daaafmmmmmm"
		"daaafmmmmmm"
		"daaafmmmmmm"
		"daaafmmmmmm",
		
		// Connections:
		"1: 10, 1,  2: 5\n"  /* Type 1, BLOCK_FACE_XP */
		"1:  2, 1, 10: 3\n"  /* Type 1, BLOCK_FACE_ZP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // CorridorCorner5


	// CorridorCorner5:
	// The data has been exported from gallery Nether, area index 10, ID 40
	{
		// Size:
		11, 6, 11,  // SizeX = 11, SizeY = 6, SizeZ = 11

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */
		"c:114: 2\n"  /* netherbrickstairs */
		"d:114: 0\n"  /* netherbrickstairs */
		"e:114: 3\n"  /* netherbrickstairs */
		"f:114: 1\n"  /* netherbrickstairs */
		"g: 54: 5\n"  /* chest */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaaaaaaaa"
		"aaaaammmmmm"
		"aaaaammmmmm"
		"aaaaammmmmm"
		"aaaaammmmmm"
		"aaaaammmmmm"
		"aaaaammmmmm"

		// Level 2
		"aaaaaaaaaaa"
		"ag........."
		"a.........."
		"a.........."
		"a...aaaaaaa"
		"a...ammmmmm"
		"a...ammmmmm"
		"a...ammmmmm"
		"a...ammmmmm"
		"a...ammmmmm"
		"a...ammmmmm"

		// Level 3
		"abababababa"
		"b.........."
		"a.........."
		"b.........."
		"a...abababa"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"

		// Level 4
		"abababababa"
		"b.........."
		"a.........."
		"b.........."
		"a...abababa"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"

		// Level 5
		"abababababa"
		"b.........."
		"a.........."
		"b.........."
		"a...abababa"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"
		"b...bmmmmmm"
		"a...ammmmmm"

		// Level 6
		"ccccccccccc"
		"daaaaaaaaaa"
		"daaaaaaaaaa"
		"daaaaaaaaaa"
		"daaaeeeeeee"
		"daaafmmmmmm"
		"daaafmmmmmm"
		"daaafmmmmmm"
		"daaafmmmmmm"
		"daaafmmmmmm"
		"daaafmmmmmm",
		
		// Connections:
		"1: 10, 1,  2: 5\n"  /* Type 1, BLOCK_FACE_XP */
		"1:  2, 1, 10: 3\n"  /* Type 1, BLOCK_FACE_ZP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // CorridorCorner5Chest


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CorridorStairs:
	// The data has been exported from gallery Nether, area index 12, ID 42
	{
		// Size:
		9, 13, 5,  // SizeX = 9, SizeY = 13, SizeZ = 5

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:114: 0\n"  /* netherbrickstairs */
		"c:113: 0\n"  /* netherbrickfence */
		"d:114: 2\n"  /* netherbrickstairs */
		"e:114: 3\n"  /* netherbrickstairs */
		"f: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"aaaaaaaaa"
		"aaaaaaaaa"
		"aaaaaaaaa"
		"aaaaaaaaa"
		"aaaaaaaaa"

		// Level 2
		"aaaaaaaaa"
		".baaaaaaa"
		".baaaaaaa"
		".baaaaaaa"
		"aaaaaaaaa"

		// Level 3
		"acaaaaaaa"
		"..baaaaaa"
		"..baaaaaa"
		"..baaaaaa"
		"acaaaaaaa"

		// Level 4
		"acaaaaaaa"
		"...baaaaa"
		"...baaaaa"
		"...baaaaa"
		"acaaaaaaa"

		// Level 5
		"acacaaaaa"
		"....baaaa"
		"....baaaa"
		"....baaaa"
		"acacaaaaa"

		// Level 6
		"aaacaaaaa"
		".....baaa"
		".....baaa"
		".....baaa"
		"aaacaaaaa"

		// Level 7
		"daacacaaa"
		"a.....baa"
		"a.....baa"
		"a.....baa"
		"eaacacaaa"

		// Level 8
		"fdaaacaaa"
		"fa.....ba"
		"fa.....ba"
		"fa.....ba"
		"feaaacaaa"

		// Level 9
		"ffdaacaca"
		"ffa......"
		"ffa......"
		"ffa......"
		"ffeaacaca"

		// Level 10
		"fffdaaaca"
		"fffa....."
		"fffa....."
		"fffa....."
		"fffeaaaca"

		// Level 11
		"ffffdaaca"
		"ffffa...."
		"ffffa...."
		"ffffa...."
		"ffffeaaca"

		// Level 12
		"fffffdaaa"
		"fffffa..."
		"fffffa..."
		"fffffa..."
		"fffffeaaa"

		// Level 13
		"ffffffddd"
		"ffffffaaa"
		"ffffffaaa"
		"ffffffaaa"
		"ffffffeee",

		// Connections:
		"1: 0, 1, 2: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1: 8, 8, 2: 5\n"  /* Type 1, BLOCK_FACE_XP */,
		
		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // CorridorStairs


	// LavaStaircase:
	// The data has been exported from gallery Nether, area index 28, ID 278
	{
		// Size:
		15, 11, 15,  // SizeX = 15, SizeY = 11, SizeZ = 15

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */
		"c: 11: 0\n"  /* lava */,

		// Block data:
		// Level 1
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"

		// Level 2
		"aaaaaaaa...aaaa"
		"aaaaa.........a"
		"aaaaa.........a"
		"aaaaab........a"
		"accca...aaaa..a"
		"accca...acca..a"
		"acccaaaaacca..a"
		"acccccccccca..a"
		"acccaaaaacca..a"
		"accca...acca..a"
		"accca...aaaa..a"
		"aaaaab........a"
		"aaaaa.........a"
		"aaaaa.........a"
		"aaaaaaaa...aaaa"

		// Level 3
		"aaaaaaaa...aaaa"
		"aaaa..........a"
		"aaaa..........a"
		"aaaabb........a"
		"aaaa..........a"
		"a.............a"
		"a.............a"
		"a.............a"
		"a.............a"
		"a.............a"
		"aaaa..........a"
		"aaaabb........a"
		"aaaa..........a"
		"aaaa..........a"
		"aaaaaaaa...aaaa"

		// Level 4
		"aaaaaaaa...aaaa"
		"a.............a"
		"a.............a"
		"a..bb.........a"
		"aaaa..........a"
		"aaaa..........a"
		"a.............a"
		"a.............a"
		"a.............a"
		"aaaa..........a"
		"aaaa..........a"
		"a..bb.........a"
		"a.............a"
		"a.............a"
		"aaaaaaaa...aaaa"

		// Level 5
		"aaaaaaaabbbaaaa"
		"a.............a"
		"a.............a"
		"a..b..........a"
		"a..b..........a"
		"aaaa..........a"
		"aaaa..........a"
		"a.............a"
		"aaaa..........a"
		"aaaa..........a"
		"a..b..........a"
		"a..b..........a"
		"a.............a"
		"a.............a"
		"aaaaaaaabbbaaaa"

		// Level 6
		"aaaaaaaaaaaaaaa"
		"a.............a"
		"a.............a"
		"a.............a"
		"a..b..........a"
		"a..b..........a"
		"aaaa..........a"
		"aaaa..........a"
		"aaaa..........a"
		"a..b..........a"
		"a..b..........a"
		"a.............a"
		"a.............a"
		"a.............a"
		"aaaaaaaaaaaaaaa"

		// Level 7
		"aaaaaaaaaaaaaaa"
		"a.............a"
		"a.............a"
		"a.............a"
		"a.............a"
		"a..b..........a"
		"...b..........a"
		"...b..........a"
		"...b..........a"
		"a..b..........a"
		"a.............a"
		"a.............a"
		"a.............a"
		"a.............a"
		"aaaaaaaaaaaaaaa"

		// Level 8
		"aababababababaa"
		"a.............a"
		"b.............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"..............b"
		"..............a"
		"..............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"aababababababaa"

		// Level 9
		"aababababababaa"
		"a.............a"
		"b.............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"..............b"
		"..............a"
		"..............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"aababababababaa"

		// Level 10
		"aababababababaa"
		"a.............a"
		"b.............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"..............b"
		"..............a"
		"..............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"b.............b"
		"a.............a"
		"aababababababaa"

		// Level 11
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa"
		"aaaaaaaaaaaaaaa",
		
		// Connections:
		"1:  0, 6,  7: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"0:  9, 1,  0: 2\n"  /* Type 0, BLOCK_FACE_ZM */
		"0:  9, 1, 14: 3\n"  /* Type 0, BLOCK_FACE_ZP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // LavaStaircase


	// LavaStaircaseBig:
	// The data has been exported from gallery Nether, area index 31, ID 282
	{
		// Size:
		12, 15, 15,  // SizeX = 12, SizeY = 15, SizeZ = 15

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b: 10: 0\n"  /* lava */
		"c:113: 0\n"  /* netherbrickfence */,

		// Block data:
		// Level 1
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"

		// Level 2
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"abbbbbaaaaaa"
		"abbbbbbaaaaa"
		"abbbbbba...."
		"abbbbbba...."
		"abbbbbba...."
		"abbbbbbaaaaa"
		"abbbbb.aaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"

		// Level 3
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"abbbbbaaaaaa"
		"abbbbbba...a"
		"abbbbbba...."
		"abbbbbba...."
		"abbbbbba...."
		"abbbbbba...a"
		"abbbbb.aaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"

		// Level 4
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"abbbbbaa...a"
		"abbbbbba...a"
		"abbbbbba...."
		"abbbbbba...."
		"abbbbbba...."
		"abbbbbba...a"
		"abbbbbaa...a"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"

		// Level 5
		"aaaaaaaaaaaa"
		"aaaaa......a"
		"aaaaa......a"
		"aaaaacc....a"
		"a.....cc...a"
		"a......c...a"
		"a......c...."
		"a......c...."
		"a......c...."
		"a......c...a"
		"a.....cc...a"
		"aaaaacc....a"
		"aaaaa......a"
		"aaaaa......a"
		"aaaaaaaaaaaa"

		// Level 6
		"aaaaaaaaaaaa"
		"aaaa.......a"
		"aaaa.......a"
		"aaaacc.....a"
		"aaaa.......a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"aaaa.......a"
		"aaaacc.....a"
		"aaaa.......a"
		"aaaa.......a"
		"aaaaaaaaaaaa"

		// Level 7
		"aaaaaaaaaaaa"
		"a..........a"
		"a..........a"
		"a..cc......a"
		"aaaa.......a"
		"aaaa.......a"
		"a..........a"
		"a..........a"
		"a..........a"
		"aaaa.......a"
		"aaaa.......a"
		"a..cc......a"
		"a..........a"
		"a..........a"
		"aaaaaaaaaaaa"

		// Level 8
		"aaaaaaaaaaaa"
		"a..........a"
		"a..........a"
		"a..c.......a"
		"a..c.......a"
		"aaaa.......a"
		"aaaa.......a"
		"a..........a"
		"aaaa.......a"
		"aaaa.......a"
		"a..c.......a"
		"a..c.......a"
		"a..........a"
		"a..........a"
		"aaaaaaaaaaaa"

		// Level 9
		"aaaaaaaaaaaa"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..c.......a"
		"a..c.......a"
		"aaaa.......a"
		"aaaa.......a"
		"aaaa.......a"
		"a..c.......a"
		"a..c.......a"
		"a..........a"
		"a..........a"
		"a..........a"
		"aaaaaaaaaaaa"

		// Level 10
		"aaaaaaaaaaaa"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..c.......a"
		"...c.......a"
		"...c.......a"
		"...c.......a"
		"a..c.......a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"aaaaaaaaaaaa"

		// Level 11
		"aaaaaaaaaaaa"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"...........a"
		"...........a"
		"...........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"aaaaaaaaaaaa"

		// Level 12
		"aaaaaaaaaaaa"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"...........a"
		"...........a"
		"...........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"aaaaaaaaaaaa"

		// Level 13
		"aaaaaaaaaaaa"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"...........a"
		"...........a"
		"...........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"a..........a"
		"aaaaaaaaaaaa"

		// Level 14
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"
		"aaaaaaaaaaaa"

		// Level 15
		"aaaaaaaaaaaa"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"abbbbbbbbbba"
		"aaaaaaaaaaaa",
		
		// Connections:
		"1:  0, 9,  7: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1: 11, 1,  7: 5\n"  /* Type 1, BLOCK_FACE_XP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // LavaStaircaseBig


	// MidStaircase:
	// The data has been exported from gallery Nether, area index 23, ID 165
	{
		// Size:
		13, 8, 13,  // SizeX = 13, SizeY = 8, SizeZ = 13

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b: 88: 0\n"  /* soulsand */
		"c:115: 7\n"  /* netherwartblock */
		"d:114: 3\n"  /* netherbrickstairs */
		"e:114: 0\n"  /* netherbrickstairs */
		"f:114: 1\n"  /* netherbrickstairs */
		"g:114: 2\n"  /* netherbrickstairs */
		"h: 10: 0\n"  /* lava */
		"i:113: 0\n"  /* netherbrickfence */,

		// Block data:
		// Level 1
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaabbbbbaaaa"
		"aaaabbbbbaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaabbbbbaaaa"
		"aaaabbbbbaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 2
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaacccccaaaa"
		"addecccccfdda"
		"...eaaaaad..."
		"...eaaaaa...."
		"...eaaaaag..."
		"agggcccccfgga"
		"aaaacccccaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 3
		"aaaaaaaaaaaaa"
		"aha.......aha"
		"aaa.......aaa"
		"a...........a"
		"a...........a"
		"....eaaaa...."
		"....eaaaa...."
		"....eaaaa...."
		"a...........a"
		"a...........a"
		"aaa.......aaa"
		"aha.......aha"
		"aaaaaaaaaaaaa"

		// Level 4
		"aaaiiaaaiiaaa"
		"a...........a"
		"a...........a"
		"a...........a"
		"a...........a"
		".....eaaa...."
		".....eaaa...."
		".....eaaa...."
		"a...........a"
		"a...........a"
		"a...........a"
		"a...........a"
		"aaaiiaaaiiaaa"

		// Level 5
		"aaaiiaaaiiaaa"
		"a...........a"
		"a...........a"
		"a...........a"
		"a...........a"
		"......eaa...."
		"......eaa...."
		"......eaa...."
		"a...........a"
		"a...........a"
		"a...........a"
		"a...........a"
		"aaaiiaaaiiaaa"

		// Level 6
		"aaaaaaaaaaaaa"
		"a...........a"
		"a...........a"
		"a...........a"
		"a...........a"
		"a......ea...a"
		"a......ea...a"
		"a......ea...a"
		"a...........a"
		"a...........a"
		"a...........a"
		"a...........a"
		"aaaaaaaaaaaaa"

		// Level 7
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaa....eaaaa"
		"aaaa....eaaaa"
		"aaaa....eaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 8
		"iaiaiaiaiaiai"
		"a...........a"
		"i...........i"
		"a...........a"
		"i...........i"
		"a............"
		"i............"
		"a............"
		"i...........i"
		"a...........a"
		"i...........i"
		"a...........a"
		"iaiaiaiaiaiai",

		// Connections:
		"1:  0, 1, 6: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1: 12, 1, 6: 5\n"  /* Type 1, BLOCK_FACE_XP */
		"1: 12, 7, 6: 5\n"  /* Type 1, BLOCK_FACE_XP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // MidStaircase


	// StairsToOpen1:
	// The data has been exported from gallery Nether, area index 27, ID 277
	{
		// Size:
		7, 10, 7,  // SizeX = 7, SizeY = 10, SizeZ = 7

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */
		"m: 19: 0\n"  /* sponge */,

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
		"a.....a"
		"a.....a"
		"a.....a"
		"aaaaaaa"
		"aaaaaaa"

		// Level 3
		"aa...aa"
		"a.....a"
		"b.....b"
		"a.....a"
		"b.....b"
		"a.aaaaa"
		"aabaaba"

		// Level 4
		"aa...aa"
		"a.....a"
		"b.....b"
		"a.....a"
		"b.....b"
		"a..aaaa"
		"aabaaba"

		// Level 5
		"aabbbaa"
		"a.....a"
		"b.....b"
		"a.....a"
		"b.....b"
		"a...aaa"
		"aabaaba"

		// Level 6
		"aaaaaaa"
		"a.....a"
		"a.....a"
		"a.....a"
		"a.....a"
		"a....aa"
		"aaaaaaa"

		// Level 7
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"a.....a"
		"aaaaaaa"

		// Level 8
		"aaaaaaa"
		"a.....a"
		"a......"
		"a......"
		"a......"
		"a.....a"
		"aaaaaaa"

		// Level 9
		"mmmmmmm"
		"m.....m"
		"m......"
		"m......"
		"m......"
		"m.....m"
		"mmmmmmm"

		// Level 10
		"mmmmmmm"
		"m.....m"
		"m......"
		"m......"
		"m......"
		"m.....m"
		"mmmmmmm",
		
		// Connections:
		"0: 3, 1, 0: 2\n"  /* Type 0, BLOCK_FACE_ZM */
		"0: 6, 7, 3: 5\n"  /* Type 0, BLOCK_FACE_XP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // StairsToOpen1


	// StairsToOpen2:
	// The data has been exported from gallery Nether, area index 8, ID 35
	{
		// Size:
		7, 10, 7,  // SizeX = 7, SizeY = 10, SizeZ = 7

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */
		"m: 19: 0\n"  /* sponge */,

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
		"a.....a"
		"a.....a"
		"a.....a"
		"aaaaaaa"
		"aaaaaaa"

		// Level 3
		"aa...aa"
		"a.....a"
		"b.....b"
		"a.....a"
		"b.....b"
		"a.aaaaa"
		"aabaaba"

		// Level 4
		"aa...aa"
		"a.....a"
		"b.....b"
		"a.....a"
		"b.....b"
		"a..aaaa"
		"aabaaba"

		// Level 5
		"aabbbaa"
		"a.....a"
		"b.....b"
		"a.....a"
		"b.....b"
		"a...aaa"
		"aabaaba"

		// Level 6
		"aaaaaaa"
		"a.....a"
		"a.....a"
		"a.....a"
		"a.....a"
		"a....aa"
		"aaaaaaa"

		// Level 7
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"aaaaaaa"
		"a.....a"
		"aaaaaaa"

		// Level 8
		"aaaaaaa"
		"a.....a"
		"......a"
		"......a"
		"......a"
		"a.....a"
		"aaaaaaa"

		// Level 9
		"mmmmmmm"
		"m.....m"
		"......m"
		"......m"
		"......m"
		"m.....m"
		"mmmmmmm"

		// Level 10
		"mmmmmmm"
		"m.....m"
		"......m"
		"......m"
		"......m"
		"m.....m"
		"mmmmmmm",
		
		// Connections:
		"0: 3, 1, 0: 2\n"  /* Type 0, BLOCK_FACE_ZM */
		"0: 0, 7, 3: 4\n"  /* Type 0, BLOCK_FACE_XM */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // StairsToOpen2


	// Tee2x4:
	// The data has been exported from gallery Nether, area index 40, ID 291
	{
		// Size:
		13, 6, 7,  // SizeX = 13, SizeY = 6, SizeZ = 7

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */
		"c:114: 0\n"  /* netherbrickstairs */
		"d:114: 1\n"  /* netherbrickstairs */
		"e:114: 2\n"  /* netherbrickstairs */
		"f:114: 3\n"  /* netherbrickstairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"mmmmaaaaammmm"
		"mmmmaaaaammmm"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 2
		"mmmma...ammmm"
		"mmmma...ammmm"
		"aaaaa...aaaaa"
		"............."
		"............."
		"............."
		"aaaaaaaaaaaaa"

		// Level 3
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"ababa...ababa"
		"............."
		"............."
		"............."
		"ababababababa"

		// Level 4
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"ababa...ababa"
		"............."
		"............."
		"............."
		"ababababababa"

		// Level 5
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"ababa...ababa"
		"............."
		"............."
		"............."
		"ababababababa"

		// Level 6
		"mmmmcaaadmmmm"
		"mmmmcaaadmmmm"
		"eeeecaaadeeee"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"fffffffffffff",

		// Connections:
		"1:  0, 1, 4: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1:  6, 1, 0: 2\n"  /* Type 1, BLOCK_FACE_ZM */
		"1: 12, 1, 4: 5\n"  /* Type 1, BLOCK_FACE_XP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // Tee2x4


	// Tee4x4:
	// The data has been exported from gallery Nether, area index 41, ID 292
	{
		// Size:
		13, 6, 9,  // SizeX = 13, SizeY = 6, SizeZ = 9

		// Block definitions:
		".:  0: 0\n"  /* air */
		"a:112: 0\n"  /* netherbrick */
		"b:113: 0\n"  /* netherbrickfence */
		"c:114: 0\n"  /* netherbrickstairs */
		"d:114: 1\n"  /* netherbrickstairs */
		"e:114: 2\n"  /* netherbrickstairs */
		"f:114: 3\n"  /* netherbrickstairs */
		"m: 19: 0\n"  /* sponge */,

		// Block data:
		// Level 1
		"mmmmaaaaammmm"
		"mmmmaaaaammmm"
		"mmmmaaaaammmm"
		"mmmmaaaaammmm"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 2
		"mmmma...ammmm"
		"mmmma...ammmm"
		"mmmma...ammmm"
		"mmmma...ammmm"
		"aaaaa...aaaaa"
		"............."
		"............."
		"............."
		"aaaaaaaaaaaaa"

		// Level 3
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"ababa...ababa"
		"............."
		"............."
		"............."
		"ababababababa"

		// Level 4
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"ababa...ababa"
		"............."
		"............."
		"............."
		"ababababababa"

		// Level 5
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"mmmma...ammmm"
		"mmmmb...bmmmm"
		"ababa...ababa"
		"............."
		"............."
		"............."
		"ababababababa"

		// Level 6
		"mmmmcaaadmmmm"
		"mmmmcaaadmmmm"
		"mmmmcaaadmmmm"
		"mmmmcaaadmmmm"
		"eeeecaaadeeee"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"fffffffffffff",

		// Connections:
		"1:  0, 1,  6: 4\n"  /* Type 1, BLOCK_FACE_XM */
		"1: 12, 1,  6: 5\n"  /* Type 1, BLOCK_FACE_XP */
		"1:  6, 1,  0: 2\n"  /* Type 1, BLOCK_FACE_ZM */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // Tee4x4

	// Turret:
	// The data has been exported from gallery Nether, area index 7, ID 34
	{
		// Size:
		7, 6, 7,  // SizeX = 7, SizeY = 6, SizeZ = 7

		// Block definitions:
		".:  0: 0\n"  /* air */
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
		"0: 0, 1, 3: 4\n"  /* Type 0, BLOCK_FACE_XM */
		"0: 3, 1, 0: 2\n"  /* Type 0, BLOCK_FACE_ZM */
		"0: 6, 1, 3: 5\n"  /* Type 0, BLOCK_FACE_XP */
		"0: 3, 1, 6: 3\n"  /* Type 0, BLOCK_FACE_ZP */,

		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},  // Turret

} ;  // g_NetherFortPrefabs1





const cPrefab::sDef g_NetherFortStartingPrefabs1[] =
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CentralRoom:
	// The data has been exported from gallery Nether, area index 22, ID 164
	{
		// Size:
		13, 9, 13,  // SizeX = 13, SizeY = 9, SizeZ = 13

		// Block definitions:
		"a:112: 0\n"  /* netherbrick */
		"b:  0: 0\n"  /* air */
		"c: 10: 0\n"  /* lava */
		"d:113: 0\n"  /* netherbrickfence */,

		// Block data:
		// Level 1
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 2
		"aaaaabbbaaaaa"
		"aaaaabbbaaaaa"
		"aabbbbbbbbbaa"
		"aabbbbbbbbbaa"
		"aabbbbbbbbbaa"
		"aabbbaaabbbaa"
		"aabbbacabbbaa"
		"aabbbaaabbbaa"
		"aabbbbbbbbbaa"
		"aabbbbbbbbbaa"
		"aabbbbbbbbbaa"
		"aaaaabbbaaaaa"
		"aaaaabbbaaaaa"

		// Level 3
		"aaaaabbbaaaaa"
		"aaadabbbadaaa"
		"aabbbbbbbbbaa"
		"adbbbbbbbbbda"
		"aabbbbbbbbbaa"
		"adbbbbbbbbbda"
		"aabbbbbbbbbaa"
		"adbbbbbbbbbda"
		"aabbbbbbbbbaa"
		"adbbbbbbbbbda"
		"aabbbbbbbbbaa"
		"aaadabbbadaaa"
		"aaaaabbbaaaaa"

		// Level 4
		"aaaaabbbaaaaa"
		"aaadabbbadaaa"
		"aabbbbbbbbbaa"
		"adbbbbbbbbbda"
		"aabbbbbbbbbaa"
		"adbbbbbbbbbda"
		"aabbbbbbbbbaa"
		"adbbbbbbbbbda"
		"aabbbbbbbbbaa"
		"adbbbbbbbbbda"
		"aabbbbbbbbbaa"
		"aaadabbbadaaa"
		"aaaaabbbaaaaa"

		// Level 5
		"adadadddadada"
		"daaaabbbaaaad"
		"aabbbbbbbbbaa"
		"dabbbbbbbbbad"
		"aabbbbbbbbbaa"
		"dabbbbbbbbbad"
		"aabbbbbbbbbaa"
		"dabbbbbbbbbad"
		"aabbbbbbbbbaa"
		"dabbbbbbbbbad"
		"aabbbbbbbbbaa"
		"daaaabbbaaaad"
		"adadabbbadada"

		// Level 6
		"adadaaaaadada"
		"daaaaaaaaaaad"
		"aabbbbbbbbbaa"
		"dabbbbbbbbbad"
		"aabbbbbbbbbaa"
		"dabbbbbbbbbad"
		"aabbbbbbbbbaa"
		"dabbbbbbbbbad"
		"aabbbbbbbbbaa"
		"dabbbbbbbbbad"
		"aabbbbbbbbbaa"
		"daaaaaaaaaaad"
		"adadaaaaadada"

		// Level 7
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 8
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"
		"aaaaaaaaaaaaa"

		// Level 9
		"dadadadadadad"
		"abbbbbbbbbbba"
		"dbbbbbbbbbbbd"
		"abbbbbbbbbbba"
		"dbbbbbbbbbbbd"
		"abbbbbbbbbbba"
		"dbbbbbbbbbbbd"
		"abbbbbbbbbbba"
		"dbbbbbbbbbbbd"
		"abbbbbbbbbbba"
		"dbbbbbbbbbbbd"
		"abbbbbbbbbbba"
		"dadadadadadad",

		// Connections:
		"0: 6, 1,  0: 2\n"  /* Type 0, BLOCK_FACE_ZM */
		"1: 6, 1, 12: 3\n"  /* Type 1, BLOCK_FACE_ZP */,
		
		// AllowedRotations:
		7,  /* 1, 2, 3 CCW rotations */
		
		// Merge strategy:
		cBlockArea::msSpongePrint,
	},
} ;  // g_NetherFortStartingPrefabs1

const size_t g_NetherFortPrefabs1Count = ARRAYCOUNT(g_NetherFortPrefabs1);
const size_t g_NetherFortStartingPrefabs1Count = ARRAYCOUNT(g_NetherFortStartingPrefabs1);




