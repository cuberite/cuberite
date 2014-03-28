
// NetherFortPrefabs.cpp

// Defines all the prefabs for nether forts

#include "Globals.h"
#include "NetherFortPrefabs.h"





/*
The nether fortress has two types of connectors, Outer and Inner. Outer is Type 0, Inner is Type 1.
*/





const cPrefab::sDef g_NetherFortPrefabs1[] =
{
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
		"l:  0: 0\n"  /* air */,

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
		"aaaalaaalaaaa"
		"bbcdaaaaadebb"
		"bbbcdddddebbb"
		"bbbbbbbbbbbbb"
		"bbbbbbbbbbbbb"

		// Level 3
		"aaaaaaaaaaaaa"
		"lllllllllllll"
		"lllllllllllll"
		"lllllllllllll"
		"aaaalffflaaaa"
		"bbaaaaaaaaabb"
		"bbaaaaaaaaabb"
		"bbaaaaaaaaabb"
		"bbaaaaaaaaabb"

		// Level 4
		"agagagagagaga"
		"lllllllllllll"
		"lllllllllllll"
		"lllllllllllll"
		"agaalllllaaga"
		"bbaaalllaaabb"
		"bbglllllllgbb"
		"bbglllllllgbb"
		"bbgggggggggbb"

		// Level 5
		"agagagagagaga"
		"lllllllllllll"
		"lllllllllllll"
		"lllllllllllll"
		"agaalllllaaga"
		"bbaaalllaaabb"
		"bblllllllllbb"
		"bblllllllllbb"
		"bblllllllllbb"

		// Level 6
		"agagagagagaga"
		"lllllllllllll"
		"lllllllllllll"
		"lllllllllllll"
		"agaalllllaaga"
		"bbaaalllaaabb"
		"bblllllllllbb"
		"bblllllllllbb"
		"bblllllllllbb"

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
		cBlockArea::msImprint,
	},
} ;  // g_NetherFortPrefabs1





const cPrefab::sDef g_NetherFortStartingPrefabs1[] =
{
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
		"adadabbbadada"
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
		cBlockArea::msImprint,
	},
} ;  // g_NetherFortStartingPrefabs1

const size_t g_NetherFortPrefabs1Count = ARRAYCOUNT(g_NetherFortPrefabs1);
const size_t g_NetherFortStartingPrefabs1Count = ARRAYCOUNT(g_NetherFortStartingPrefabs1);




