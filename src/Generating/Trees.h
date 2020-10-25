
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
#include "../ChunkDef.h"  // For sSetBlockVector




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





/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a tree at the specified coords (lowest trunk block) in the specified biome */
void GetTreeImageByBiome(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, EMCSBiome a_Biome, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random apple tree */
void GetAppleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a small (nonbranching) apple tree */
void GetSmallAppleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a large (branching) apple tree */
void GetLargeAppleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks with the logs of a tree branch of the provided log type.
The length of the branch can be changed with the a_BranchLength.
The initial direction is a_StartDirection. The direction can be manipulated with a_Direction to create a curve.
Returns the position of the last log block placed. */
Vector3d GetTreeBranch(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, Vector3i a_BlockPos, int a_BranchLength, Vector3d a_StartDirection, Vector3d a_Direction, sSetBlockVector & a_LogBlocks);

/** Returns the meta for a log from the given direction */
NIBBLETYPE GetLogMetaFromDirection(NIBBLETYPE a_BlockMeta, Vector3d a_Direction);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random birch tree */
void GetBirchTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random acacia tree */
void GetAcaciaTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random darkoak tree */
void GetDarkoakTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random large birch tree */
void GetTallBirchTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random conifer tree. The probability to get a large pine is higher than a spruce tree */
void GetConiferTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large = false);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random spruce */
void GetSpruceTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large = false);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random pine */
void GetPineTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large = false);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random small spruce (short conifer, two layers of leaves) */
void GetSmallSpruceTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random small pine (tall conifer, little leaves at top) */
void GetSmallPineTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random large spruce (short conifer, multiple layers of leaves) */
void GetLargeSpruceTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random large pine (tall conifer, little leaves at top) */
void GetLargePineTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random swampland tree */
void GetSwampTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random apple bush (for jungles) */
void GetAppleBushImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a random jungle tree */
void GetJungleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a large jungle tree (2x2 trunk) */
void GetLargeJungleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);

/** Fills a_LogBlocks and a_OtherBlocks (dirt & leaves) with the blocks required to form a small jungle tree (1x1 trunk) */
void GetSmallJungleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks);
