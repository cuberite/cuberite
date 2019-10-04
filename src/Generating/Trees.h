
// Trees.h

// Interfaces to helper functions used for generating trees

/*
Note that all of these functions must generate the same tree image for the same input (x, y, z, seq)
	- cStructGenTrees depends on this
To generate a random image for the (x, y, z) coords, pass an arbitrary value as (seq).
Each function returns two arrays of blocks, "logs" and "other". The point is that logs are of higher priority,
logs can overwrite others(leaves), but others shouldn't overwrite logs. This is an optimization for the generator.
*/





#pragma once

#include "../Noise/Noise.h"





// Blocks that don't block tree growth:
#define CASE_TREE_ALLOWED_BLOCKS \
	case E_BLOCK_AIR: \
	case E_BLOCK_LEAVES: \
	case E_BLOCK_NEW_LEAVES: \
	case E_BLOCK_SNOW: \
	case E_BLOCK_TALL_GRASS: \
	case E_BLOCK_DEAD_BUSH: \
	case E_BLOCK_SAPLING: \
	case E_BLOCK_VINES

// Blocks that a tree may overwrite when growing:
#define CASE_TREE_OVERWRITTEN_BLOCKS \
	case E_BLOCK_AIR: \
	/* case E_BLOCK_LEAVES: LEAVES are a special case, they can be overwritten only by log. Handled in cChunkMap::ReplaceTreeBlocks(). */ \
	case E_BLOCK_SNOW: \
	case E_BLOCK_TALL_GRASS: \
	case E_BLOCK_BIG_FLOWER: \
	case E_BLOCK_DEAD_BUSH: \
	case E_BLOCK_SAPLING: \
	case E_BLOCK_VINES





/** Generates an image of a tree at the specified coords (lowest trunk block) in the specified biome */
void GetTreeImageByBiome(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, EMCSBiome a_Biome, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random apple tree */
void GetAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a small (nonbranching) apple tree */
void GetSmallAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a large (branching) apple tree */
void GetLargeAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates a branch. Returns the position where the branch ends. */
Vector3d GetTreeBranch(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, int a_BranchLength, Vector3d a_StartDirection, Vector3d a_Direction, int a_TreeHeight, sSetBlockVector & a_LogBlocks);

/** Returns the meta for a log from the given direction */
NIBBLETYPE GetLogMetaFromDirection(NIBBLETYPE a_BlockMeta, Vector3d a_Direction);

/** Generates an image of a random birch tree */
void GetBirchTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random acacia tree */
void GetAcaciaTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random darkoak tree */
void GetDarkoakTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random large birch tree */
void GetTallBirchTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random conifer tree */
void GetConiferTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random spruce (short conifer, two layers of leaves) */
void GetSpruceTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random pine (tall conifer, little leaves at top) */
void GetPineTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random swampland tree */
void GetSwampTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random apple bush (for jungles) */
void GetAppleBushImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a random jungle tree */
void GetJungleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large);

/** Generates an image of a large jungle tree (2x2 trunk) */
void GetLargeJungleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Generates an image of a small jungle tree (1x1 trunk) */
void GetSmallJungleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);
