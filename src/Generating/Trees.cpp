
// Trees.cpp

// Implements helper functions used for generating trees

#include "Globals.h"
#include "Trees.h"





typedef struct
{
	int x, z;
} sCoords;

typedef struct
{
	int x, z;
	NIBBLETYPE Meta;
} sMetaCoords;

static const sCoords Corners[] =
{
	{-1, -1},
	{-1, 1},
	{1, -1},
	{1, 1},
} ;

// Array with possible directions for a branch to go to.
static const Vector3d & pickBranchDirection(Vector3i a_BlockPos, int a_Seq)
{
	static const std::array<Vector3d, 32> directions =
	{
		{
			{ -1, 0, 0 }, { 0, 0, -1  },
			{ -1, 0, 1 }, { -1, 0, -1 },
			{ 1, 0, 1  }, { 1, 0, -1  },
			{ 1, 0, 0  }, { 0, 0, 1   },

			{ -0.5, 0, 0   }, { 0, 0, -0.5    },
			{ -0.5, 0, 0.5 }, { -0.5, 0, -0.5 },
			{ 0.5, 0, 0.5  }, { 0.5, 0, -0.5  },
			{ 0.5, 0, 0    }, { 0, 0, 0.5     },

			{ -1, 0.5, 0 }, { 0, 0.5, -1  },
			{ -1, 0.5, 1 }, { -1, 0.5, -1 },
			{ 1, 0.5, 1  }, { 1, 0.5, -1  },
			{ 1, 0.5, 0  }, { 0, 0.5, 1   },

			{ -0.5, 0.5, 0   },  { 0, 0.5, -0.5    },
			{ -0.5, 0.5, 0.5 },  { -0.5, 0.5, -0.5 },
			{ 0.5, 0.5, 0.5  },  { 0.5, 0.5, -0.5  },
			{ 0.5, 0.5, 0    },  { 0, 0.5, 0.5     },
		}
	};

	return directions[(static_cast<int>(a_BlockPos.SqrLength()) + a_Seq) % directions.size()];
}

// BigO = a big ring of blocks, used for generating horz slices of treetops, the number indicates the radius

static const sCoords BigO1[] =
{
	/* -1 */           {0, -1},
	/*  0 */ {-1,  0},          {1,  0},
	/*  1 */           {0,  1},
} ;

static const sCoords BigO2[] =
{
	/* -2 */           {-1, -2}, {0, -2}, {1, -2},
	/* -1 */ {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1},
	/*  0 */ {-2,  0}, {-1,  0},          {1,  0}, {2,  0},
	/*  1 */ {-2,  1}, {-1,  1}, {0,  1}, {1,  1}, {2,  1},
	/*  2 */           {-1,  2}, {0,  2}, {1,  2},
} ;

static const sCoords BigO3[] =
{
	/* -3 */           {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3},
	/* -2 */ {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {3, -2},
	/* -1 */ {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1},
	/*  0 */ {-3,  0}, {-2,  0}, {-1,  0},          {1,  0}, {2,  0}, {3,  0},
	/*  1 */ {-3,  1}, {-2,  1}, {-1,  1}, {0,  1}, {1,  1}, {2,  1}, {3,  1},
	/*  2 */ {-3,  2}, {-2,  2}, {-1,  2}, {0,  2}, {1,  2}, {2,  2}, {3,  2},
	/*  3 */           {-2,  3}, {-1,  3}, {0,  3}, {1,  3}, {2,  3},
} ;


static const sCoords BigO4[] =  // Part of dark oak tree
{
	/* -4 */                     {-2, -4}, {-1, -4}, {0, -4}, {1, -4}, {2, -4},
	/* -3 */           {-3, -3}, {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3},
	/* -2 */ {-4, -2}, {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {3, -2}, {4, -2},
	/* -1 */ {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1}, {4, -1},
	/*  0 */ {-4,  0}, {-3,  0}, {-2,  0}, {-1,  0},          {1,  0}, {2,  0}, {3,  0}, {4,  0},
	/*  1 */ {-4,  1}, {-3,  1}, {-2,  1}, {-1,  1}, {0,  1}, {1,  1}, {2,  1}, {3,  1}, {4,  1},
	/*  2 */ {-4,  2}, {-3,  2}, {-2,  2}, {-1,  2}, {0,  2}, {1,  2}, {2,  2}, {3,  2}, {4,  2},
	/*  3 */           {-3,  3}, {-2,  3}, {-1,  3}, {0,  3}, {1,  3}, {2,  3}, {3,  3},
	/*  4 */                     {-2,  4}, {-1,  4}, {0,  4}, {1,  4}, {2,  4},
};

static const sCoords BigO3Jungle[] =
{
	/* -3 */                               {0, -3}, {1, -3},
	/* -2 */                     {-1, -2}, {0, -2}, {1, -2}, {2, -2},
	/* -1 */           {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1},
	/*  0 */ {-3,  0}, {-2,  0}, {-1,  0}, {0,  0}, {1,  0}, {2,  0}, {3,  0}, {4,  0},
	/*  1 */ {-3,  1}, {-2,  1}, {-1,  1}, {0,  1}, {1,  1}, {2,  1}, {3,  1}, {4,  1},
	/*  2 */           {-2,  2}, {-1,  2}, {0,  2}, {1,  2}, {2,  2}, {3,  2},
	/*  3 */                     {-1,  3}, {0,  3}, {1,  3}, {2,  3},
	/*  4 */                               {0,  4}, {1,  4},
};

static const sCoords BigO4Jungle[] =
{
	/* -4 */                                         {0, -4}, {1, -4},
	/* -3 */                     {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3},
	/* -2 */           {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {3, -2}, {4, -2},
	/* -1 */           {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1}, {4, -1},
	/*  0 */ {-4,  0}, {-3,  0}, {-2,  0}, {-1,  0},                   {2,  0}, {3,  0}, {4,  0}, {5,  0},
	/*  1 */ {-4,  1}, {-3,  1}, {-2,  1}, {-1,  1},                   {2,  1}, {3,  1}, {4,  1}, {5,  1},
	/*  2 */           {-3,  2}, {-2,  2}, {-1,  2}, {0,  2}, {1,  2}, {2,  2}, {3,  2}, {4,  2},
	/*  3 */           {-3,  3}, {-2,  3}, {-1,  3}, {0,  3}, {1,  3}, {2,  3}, {3,  3}, {4,  3},
	/*  4 */                     {-2,  4}, {-1,  4}, {0,  4}, {1,  4}, {2,  4}, {3,  4},
	/*  5 */                                         {0,  5}, {1,  5},
};

static const sCoords BigO5Jungle[] =
{
	/* -5 */                                                   {0, -5}, {1, -5},
	/* -4 */                               {-2, -4}, {-1, -4}, {0, -4}, {1, -4}, {2, -4}, {3, -4},
	/* -3 */                     {-3, -3}, {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3}, {4, -3},
	/* -2 */           {-4, -2}, {-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {3, -2}, {4, -2}, {5, -2},
	/* -1 */           {-4, -1}, {-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1}, {4, -1}, {5, -1},
	/*  0 */ {-5,  0}, {-4,  0}, {-3,  0}, {-2,  0}, {-1,  0},                   {2,  0}, {3,  0}, {4,  0}, {5,  0}, {6,  0},
	/*  1 */ {-5,  1}, {-4,  1}, {-3,  1}, {-2,  1}, {-1,  1},                   {2,  1}, {3,  1}, {4,  1}, {5,  1}, {6,  1},
	/*  2 */           {-4,  2}, {-3,  2}, {-2,  2}, {-1,  2}, {0,  2}, {1,  2}, {2,  2}, {3,  2}, {4,  2}, {5,  2},
	/*  3 */           {-4,  3}, {-3,  3}, {-2,  3}, {-1,  3}, {0,  3}, {1,  3}, {2,  3}, {3,  3}, {4,  3}, {5,  3},
	/*  4 */                     {-3,  4}, {-2,  4}, {-1,  4}, {0,  4}, {1,  4}, {2,  4}, {3,  4}, {4,  4},
	/*  5 */                               {-2,  5}, {-1,  5}, {0,  5}, {1,  5}, {2,  5}, {3,  5},
	/*  6 */                                                   {0,  6}, {1,  6},
} ;





typedef struct
{
	const sCoords * Coords;
	size_t          Count;
} sCoordsArr;

static const sCoordsArr BigOLayers[] =
{
	{BigO1, ARRAYCOUNT(BigO1)},
	{BigO2, ARRAYCOUNT(BigO2)},
	{BigO3, ARRAYCOUNT(BigO3)},
	{BigO4, ARRAYCOUNT(BigO4)},
} ;

static const sCoordsArr BigOJungleLayers[] =
{
	{BigO5Jungle, ARRAYCOUNT(BigO5Jungle)},
	{BigO4Jungle, ARRAYCOUNT(BigO4Jungle)},
	{BigO3Jungle, ARRAYCOUNT(BigO3Jungle)},
};




/** Pushes a specified layer of blocks of the same type around (x, h, z) into a_Blocks */
inline void PushCoordBlocks(int a_BlockX, int a_Height, int a_BlockZ, sSetBlockVector & a_Blocks, const sCoords * a_Coords, size_t a_NumCoords, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
{
	for (size_t i = 0; i < a_NumCoords; i++)
	{
		a_Blocks.push_back(sSetBlock(a_BlockX + a_Coords[i].x, a_Height, a_BlockZ + a_Coords[i].z, a_BlockType, a_Meta));
	}
}





inline void PushCornerBlocks(int a_BlockX, int a_Height, int a_BlockZ, int a_Seq, cNoise & a_Noise, int a_Chance, sSetBlockVector & a_Blocks, int a_CornersDist, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
{
	for (size_t i = 0; i < ARRAYCOUNT(Corners); i++)
	{
		int x = a_BlockX + Corners[i].x;
		int z = a_BlockZ + Corners[i].z;
		if (a_Noise.IntNoise3DInt(x + 64 * a_Seq, a_Height, z + 64 * a_Seq) <= a_Chance)
		{
			a_Blocks.push_back(sSetBlock(x, a_Height, z, a_BlockType, a_Meta));
		}
	}  // for i - Corners[]
}





inline void PushSomeColumns(int a_BlockX, int a_Height, int a_BlockZ, int a_ColumnHeight, int a_Seq, cNoise & a_Noise, int a_Chance, sSetBlockVector & a_Blocks, const sMetaCoords * a_Coords, size_t a_NumCoords, BLOCKTYPE a_BlockType)
{
	for (size_t i = 0; i < a_NumCoords; i++)
	{
		int x = a_BlockX + a_Coords[i].x;
		int z = a_BlockZ + a_Coords[i].z;
		if (a_Noise.IntNoise3DInt(x + 64 * a_Seq, a_Height + static_cast<int>(i), z + 64 * a_Seq) <= a_Chance)
		{
			for (int j = 0; j < a_ColumnHeight; j++)
			{
				a_Blocks.push_back(sSetBlock(x, a_Height - j, z, a_BlockType, a_Coords[i].Meta));
			}
		}
	}  // for i - a_Coords[]
}





void GetTreeImageByBiome(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, EMCSBiome a_Biome, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	switch (a_Biome)
	{
		case biPlains:
		case biExtremeHills:
		case biExtremeHillsEdge:
		case biForest:
		case biMushroomIsland:
		case biMushroomShore:
		case biForestHills:
		case biDeepOcean:
		case biStoneBeach:
		case biColdBeach:
		{
			// Apple or birch trees:
			if (a_Noise.IntNoise3DInt(a_BlockX, a_BlockY + 16 * a_Seq, a_BlockZ + 16 * a_Seq) < 0x5fffffff)
			{
				GetAppleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else
			{
				GetBirchTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			return;
		}

		case biTaiga:
		case biIcePlains:
		case biIceMountains:
		case biTaigaHills:
		{
			// Conifers
			GetConiferTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biSwamplandM:
		case biSwampland:
		{
			// Swamp trees:
			GetSwampTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biJungle:
		case biJungleHills:
		case biJungleEdge:
		{
			// Apple bushes, large jungle trees, small jungle trees
			if (a_Noise.IntNoise3DInt(a_BlockX, a_BlockY + 16 * a_Seq, a_BlockZ + 16 * a_Seq) < 0x6fffffff)
			{
				GetAppleBushImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else
			{
				bool IsLarge = a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY + 32 * a_Seq, a_BlockZ) < 0x60000000;
				GetJungleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks, IsLarge);
			}
			return;
		}

		case biBirchForest:
		case biBirchForestHills:
		{
			GetBirchTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biBirchForestM:
		case biBirchForestHillsM:
		{
			GetTallBirchTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biColdTaiga:
		case biColdTaigaHills:
		case biMegaTaiga:
		case biMegaTaigaHills:
		case biExtremeHillsPlus:
		case biSunflowerPlains:
		case biDesertM:
		case biExtremeHillsM:
		case biFlowerForest:
		case biTaigaM:
		case biIcePlainsSpikes:
		case biJungleM:
		case biJungleEdgeM:
		case biColdTaigaM:
		case biMegaSpruceTaiga:
		case biMegaSpruceTaigaHills:
		case biExtremeHillsPlusM:
		{
			// TODO: These need their special trees
			GetBirchTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biSavanna:
		case biSavannaPlateau:
		case biSavannaM:
		case biSavannaPlateauM:
		{
			GetAcaciaTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biRoofedForest:
		case biRoofedForestM:
		{
			GetDarkoakTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biMesa:
		case biMesaPlateauF:
		case biMesaPlateau:
		case biMesaBryce:
		case biMesaPlateauFM:
		case biMesaPlateauM:
		{
			GetSmallAppleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
		}

		case biDesert:
		case biDesertHills:
		case biRiver:
		case biBeach:
		case biHell:
		case biSky:
		case biOcean:
		case biFrozenOcean:
		case biFrozenRiver:
		case biVariant:
		case biNumBiomes:
		case biNumVariantBiomes:
		case biInvalidBiome:
		{
			// These biomes have no trees, or are non-biome members of the enum.
			return;
		}
	}

	ASSERT(!"Invalid biome type!");
}





void GetAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	if (a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY + 32 * a_Seq, a_BlockZ) < 0x60000000)
	{
		GetSmallAppleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
	else
	{
		GetLargeAppleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
}





void GetSmallAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	/* Small apple tree has:
	- a top plus (no log)
	- optional BigO1 + random corners (log)
	- 2 layers of BigO2 + random corners (log)
	- 1 to 3 blocks of trunk
	*/

	int Random = a_Noise.IntNoise3DInt(a_BlockX + 64 * a_Seq, a_BlockY, a_BlockZ) >> 3;

	HEIGHTTYPE Heights[] = {1, 2, 2, 3} ;
	HEIGHTTYPE Height = 1 + Heights[Random & 3];
	Random >>= 2;

	// Pre-alloc so that we don't realloc too often later:
	a_LogBlocks.reserve(static_cast<size_t>(Height + 5));
	a_OtherBlocks.reserve(ARRAYCOUNT(BigO2) * 2 + ARRAYCOUNT(BigO1) + ARRAYCOUNT(Corners) * 3 + 3 + 5);

	// Trunk:
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
	}
	int Hei = a_BlockY + Height;

	// 2 BigO2 + corners layers:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks (a_BlockX, Hei, a_BlockZ, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		PushCornerBlocks(a_BlockX, Hei, a_BlockZ, a_Seq, a_Noise, 0x5000000 - i * 0x10000000, a_OtherBlocks, 2, E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		a_LogBlocks.push_back(sSetBlock(a_BlockX, Hei, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
		Hei++;
	}  // for i - 2*

	// Optional BigO1 + corners layer:
	if ((Random & 1) == 0)
	{
		PushCoordBlocks (a_BlockX, Hei, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		PushCornerBlocks(a_BlockX, Hei, a_BlockZ, a_Seq, a_Noise, 0x6000000, a_OtherBlocks, 1, E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		a_LogBlocks.push_back(sSetBlock(a_BlockX, Hei, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
		Hei++;
	}

	// Top plus:
	PushCoordBlocks(a_BlockX, Hei, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
	a_OtherBlocks.push_back(sSetBlock(a_BlockX, Hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
}





void GetLargeAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	int Height = 7 + a_Noise.IntNoise3DInt(a_BlockX, a_BlockY, a_BlockZ) % 4;

	// Create branches
	for (int i = 4; i < Height; i++)
	{
		// Get a direction for the trunk to go to.
		Vector3d BranchStartDirection = pickBranchDirection({ a_BlockX, a_BlockY + i, a_BlockZ }, a_Seq);
		Vector3d BranchDirection = pickBranchDirection({ a_BlockX, a_BlockY / i, a_BlockZ }, a_Seq) / 3;

		int BranchLength = 2 + a_Noise.IntNoise3DInt(a_BlockX * a_Seq, a_BlockY * a_Seq, a_BlockZ * a_Seq) % 3;
		GetTreeBranch(E_BLOCK_LOG, E_META_LOG_APPLE, a_BlockX, a_BlockY + i, a_BlockZ, BranchLength, BranchStartDirection, BranchDirection, a_BlockY + Height, a_LogBlocks);
	}

	// Place leaves around each log block
	for (auto itr : a_LogBlocks)
	{
		// Get the log's X and Z coordinates
		int X = itr.GetX();
		int Z = itr.GetZ();

		a_OtherBlocks.push_back(sSetBlock(X, itr.m_RelY - 2, Z, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
		PushCoordBlocks(X, itr.m_RelY - 2, Z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		for (int y = -1; y <= 1; y++)
		{
			PushCoordBlocks (X, itr.m_RelY + y, Z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		}
		PushCoordBlocks(X, itr.m_RelY + 2, Z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		a_OtherBlocks.push_back(sSetBlock(X, itr.m_RelY + 2, Z, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
	}

	// Trunk:
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
	}
}





Vector3d GetTreeBranch(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_BlockX, int a_BlockY, int a_BlockZ, int a_BranchLength, Vector3d a_StartDirection, Vector3d a_Direction, int a_TreeHeight, sSetBlockVector & a_LogBlocks)
{
	Vector3d CurrentPos = Vector3d(a_BlockX, a_BlockY, a_BlockZ);
	Vector3d Direction  = a_StartDirection;
	for (int i = 0; i < a_BranchLength; i++)
	{
		CurrentPos += Direction;
		if (CurrentPos.y >= a_TreeHeight)
		{
			return CurrentPos - Direction;
		}
		Direction += a_Direction;
		Direction.Clamp(-1.0, 1.0);
		a_LogBlocks.push_back(sSetBlock(FloorC(CurrentPos.x), FloorC(CurrentPos.y), FloorC(CurrentPos.z), a_BlockType, GetLogMetaFromDirection(a_BlockMeta, Direction)));
	}
	return CurrentPos;
}





NIBBLETYPE GetLogMetaFromDirection(NIBBLETYPE a_BlockMeta, Vector3d a_Direction)
{
	a_Direction.Abs();

	if ((a_Direction.y > a_Direction.x) && (a_Direction.y > a_Direction.z))
	{
		return a_BlockMeta;
	}
	else if (a_Direction.x > a_Direction.z)
	{
		return a_BlockMeta + 4;
	}
	else
	{
		return a_BlockMeta + 8;
	}
}





void GetBirchTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	HEIGHTTYPE Height = 5 + (a_Noise.IntNoise3DInt(a_BlockX + 64 * a_Seq, a_BlockY, a_BlockZ) % 3);

	// Prealloc, so that we don't realloc too often later:
	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(80);

	// The entire trunk, out of logs:
	for (int i = Height - 1; i >= 0; --i)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_BIRCH));
	}
	int h = a_BlockY + Height;

	// Top layer - just the Plus:
	PushCoordBlocks(a_BlockX, h, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	a_OtherBlocks.push_back(sSetBlock(a_BlockX, h, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH));  // There's no log at this layer
	h--;

	// Second layer - log, Plus and maybe Corners:
	PushCoordBlocks (a_BlockX, h, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	PushCornerBlocks(a_BlockX, h, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 1, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	h--;

	// Third and fourth layers - BigO2 and maybe 2 * Corners:
	for (int Row = 0; Row < 2; Row++)
	{
		PushCoordBlocks (a_BlockX, h, a_BlockZ, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
		PushCornerBlocks(a_BlockX, h, a_BlockZ, a_Seq, a_Noise, 0x3fffffff + Row * 0x10000000, a_OtherBlocks, 2, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
		h--;
	}  // for Row - 2*
}





void GetAcaciaTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Calculate a base height
	int Height = 2 + (a_Noise.IntNoise3DInt(a_BlockX, a_BlockY, a_BlockZ) / 11 % 3);

	// Create the trunk
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_NEW_LOG, E_META_NEW_LOG_ACACIA_WOOD));
	}

	// Array with possible directions for a branch to go to.
	const Vector3i AvailableDirections[] =
	{
		{ -1, 1, 0 }, { 0, 1, -1 },
		{ -1, 1, 1 }, { -1, 1, -1 },
		{ 1, 1, 1 }, { 1, 1, -1 },
		{ 1, 1, 0 }, { 0, 1, 1 },
	};

	// Get a direction for the trunk to go to.
	Vector3i BranchDirection = AvailableDirections[a_Noise.IntNoise3DInt(a_BlockX, a_BlockY, a_BlockZ) % 8];

	// Calculate a height for the branch between 1 and 3
	int BranchHeight = a_Noise.IntNoise3DInt(a_BlockX, a_BlockY, a_BlockZ) % 3 + 1;

	Vector3i BranchPos = GetTreeBranch(E_BLOCK_NEW_LOG, E_META_NEW_LOG_ACACIA_WOOD, a_BlockX, a_BlockY + Height - 1, a_BlockZ, BranchHeight, Vector3i(BranchDirection), Vector3i(BranchDirection), a_BlockY + Height + BranchHeight, a_LogBlocks).Floor();

	// Add the leaves to the top of the branch
	PushCoordBlocks(BranchPos.x, BranchPos.y, BranchPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_ACACIA);
	PushCoordBlocks(BranchPos.x, BranchPos.y + 1, BranchPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_ACACIA);
	a_OtherBlocks.push_back(sSetBlock(BranchPos.x, BranchPos.y + 1, BranchPos.z, E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_ACACIA));

	// Choose if we have to add another branch
	bool TwoTop = (a_Noise.IntNoise3D(a_BlockX, a_BlockY, a_BlockZ) < 0 ? true : false);
	if (!TwoTop)
	{
		return;
	}

	// Invert the direction of the previous branch.
	BranchDirection = Vector3d(-BranchDirection.x, 1, -BranchDirection.z);

	// Calculate a new height for the second branch
	BranchHeight = a_Noise.IntNoise3DInt(a_BlockX * a_Seq, a_BlockY * a_Seq * 10, a_BlockZ * a_Seq) % 3 + 1;

	BranchPos = GetTreeBranch(E_BLOCK_NEW_LOG, E_META_NEW_LOG_ACACIA_WOOD, a_BlockX, a_BlockY + Height - 1, a_BlockZ, BranchHeight, BranchDirection, BranchDirection, a_BlockY + Height + BranchHeight, a_LogBlocks).Floor();

	// And add the leaves ontop of the second branch
	PushCoordBlocks(BranchPos.x, BranchPos.y, BranchPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_ACACIA);
	PushCoordBlocks(BranchPos.x, BranchPos.y + 1, BranchPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_ACACIA);
	a_OtherBlocks.push_back(sSetBlock(BranchPos.x, BranchPos.y + 1, BranchPos.z, E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_ACACIA));
}





void GetDarkoakTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Pick a height
	int Height = 5 + (a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY, a_BlockZ + 32 * a_Seq) / 11) % 4;

	// Create the trunk
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_NEW_LOG, E_META_NEW_LOG_DARK_OAK_WOOD));
		a_LogBlocks.push_back(sSetBlock(a_BlockX + 1, a_BlockY + i, a_BlockZ, E_BLOCK_NEW_LOG, E_META_NEW_LOG_DARK_OAK_WOOD));
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ + 1, E_BLOCK_NEW_LOG, E_META_NEW_LOG_DARK_OAK_WOOD));
		a_LogBlocks.push_back(sSetBlock(a_BlockX + 1, a_BlockY + i, a_BlockZ + 1, E_BLOCK_NEW_LOG, E_META_NEW_LOG_DARK_OAK_WOOD));
	}

	// Prevent floating trees by placing dirt under them
	for (int i = 1; i < 5; i++)
	{
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, a_BlockY - i, a_BlockZ, E_BLOCK_DIRT, E_META_DIRT_NORMAL));
		a_OtherBlocks.push_back(sSetBlock(a_BlockX + 1, a_BlockY - i, a_BlockZ, E_BLOCK_DIRT, E_META_DIRT_NORMAL));
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, a_BlockY - i, a_BlockZ + 1, E_BLOCK_DIRT, E_META_DIRT_NORMAL));
		a_OtherBlocks.push_back(sSetBlock(a_BlockX + 1, a_BlockY - i, a_BlockZ + 1, E_BLOCK_DIRT, E_META_DIRT_NORMAL));
	}

	// Create branches
	for (int i = 0; i < 3; i++)
	{
		int x = (a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY * i, a_BlockZ + 32 * a_Seq) % 3) - 1;
		int z = (a_Noise.IntNoise3DInt(a_BlockX - 32 * a_Seq, a_BlockY * i, a_BlockZ - 32 * a_Seq) % 3) - 1;

		// The branches would end up in the trunk.
		if ((x >= a_BlockX) && (x <= a_BlockX + 1) && (z >= a_BlockZ) && (z <= a_BlockZ + 1))
		{
			NOISE_DATATYPE Val1 = a_Noise.IntNoise2D(x, z);
			if (Val1 < 0)
			{
				x = a_BlockX + ((Val1 < -0.5) ? -1 : 3);
			}
			else
			{
				z = a_BlockZ + ((Val1 < 0.5) ? -1 : 3);
			}
		}

		int y = Height - (a_Noise.IntNoise3DInt(a_BlockX + x, a_BlockY * i, a_BlockZ - z) % (Height - (Height / 4)));

		for (int Y = y; Y < Height; Y++)
		{
			a_LogBlocks.push_back(sSetBlock(a_BlockX + x, a_BlockY + Y, a_BlockZ + z, E_BLOCK_NEW_LOG, E_META_NEW_LOG_DARK_OAK_WOOD));
		}
	}

	int hei = a_BlockY + Height - 2;

	// The lower two leaves layers are BigO4 with log in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO4, ARRAYCOUNT(BigO4), E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_DARK_OAK);
		PushCornerBlocks(a_BlockX, hei, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_DARK_OAK);
		hei++;
	}  // for i < 2

	// The top leaves layer is a BigO3 with leaves in the middle and possibly corners:
	PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_DARK_OAK);
	PushCornerBlocks(a_BlockX, hei, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_DARK_OAK);
	a_OtherBlocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_NEW_LEAVES, E_META_NEWLEAVES_DARK_OAK));
}





void GetTallBirchTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	HEIGHTTYPE Height = 9 + (a_Noise.IntNoise3DInt(a_BlockX + 64 * a_Seq, a_BlockY, a_BlockZ) % 3);

	// Prealloc, so that we don't realloc too often later:
	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(80);

	// The entire trunk, out of logs:
	for (int i = Height - 1; i >= 0; --i)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_BIRCH));
	}
	int h = a_BlockY + Height;

	// Top layer - just the Plus:
	PushCoordBlocks(a_BlockX, h, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	a_OtherBlocks.push_back(sSetBlock(a_BlockX, h, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH));  // There's no log at this layer
	h--;

	// Second layer - log, Plus and maybe Corners:
	PushCoordBlocks (a_BlockX, h, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	PushCornerBlocks(a_BlockX, h, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 1, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	h--;

	// Third and fourth layers - BigO2 and maybe 2 * Corners:
	for (int Row = 0; Row < 2; Row++)
	{
		PushCoordBlocks (a_BlockX, h, a_BlockZ, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
		PushCornerBlocks(a_BlockX, h, a_BlockZ, a_Seq, a_Noise, 0x3fffffff + Row * 0x10000000, a_OtherBlocks, 2, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
		h--;
	}  // for Row - 2*
}





void GetConiferTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Half chance for a spruce, half for a pine:
	if (a_Noise.IntNoise3DInt(a_BlockX + 64 * a_Seq, a_BlockY, a_BlockZ + 32 * a_Seq) < 0x40000000)
	{
		GetSpruceTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
	else
	{
		GetPineTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
}





void GetSpruceTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Spruces have a top section with layer sizes of (0, 1, 0) or only (1, 0),
	// then 1 - 3 sections of ascending sizes (1, 2) [most often], (1, 3) or (1, 2, 3)
	// and an optional bottom section of size 1, followed by 1 - 3 clear trunk blocks

	// We'll use bits from this number as partial random numbers; but the noise function has mod8 irregularities
	// (each of the mod8 remainders has a very different chance of occurrence) - that's why we divide by 8
	int MyRandom = a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY + 32 * a_Seq, a_BlockZ) / 8;

	static const HEIGHTTYPE  sHeights[] = {1, 2, 2, 3};
	HEIGHTTYPE Height = sHeights[MyRandom & 3];
	MyRandom >>= 2;

	// Prealloc, so that we don't realloc too often later:
	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(180);

	// Clear trunk blocks:
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
	}
	Height += a_BlockY;

	// Optional size-1 bottom leaves layer:
	if ((MyRandom & 1) == 0)
	{
		PushCoordBlocks(a_BlockX, Height, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, Height, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
		Height++;
	}
	MyRandom >>= 1;

	// 1 to 3 sections of leaves layers:
	static const int sNumSections[] = {1, 2, 2, 3};
	int NumSections = sNumSections[MyRandom & 3];
	MyRandom >>= 2;
	for (int i = 0; i < NumSections; i++)
	{
		switch (MyRandom & 3)  // SectionType; (1, 2) twice as often as the other two
		{
			case 0:
			case 1:
			{
				PushCoordBlocks(a_BlockX, Height,     a_BlockZ, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				PushCoordBlocks(a_BlockX, Height + 1, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				a_LogBlocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				a_LogBlocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				Height += 2;
				break;
			}
			case 2:
			{
				PushCoordBlocks(a_BlockX, Height,     a_BlockZ, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				PushCoordBlocks(a_BlockX, Height + 1, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				a_LogBlocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				a_LogBlocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				Height += 2;
				break;
			}
			case 3:
			{
				PushCoordBlocks(a_BlockX, Height,     a_BlockZ, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				PushCoordBlocks(a_BlockX, Height + 1, a_BlockZ, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				PushCoordBlocks(a_BlockX, Height + 2, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				a_LogBlocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				a_LogBlocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				a_LogBlocks.push_back(sSetBlock(a_BlockX, Height + 2, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				Height += 3;
				break;
			}
		}  // switch (SectionType)
		MyRandom >>= 2;
	}  // for i - Sections

	if ((MyRandom & 1) == 0)
	{
		// (0, 1, 0) top:
		a_LogBlocks.push_back  (sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
		PushCoordBlocks                  (a_BlockX, Height + 1, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, Height + 2, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
	}
	else
	{
		// (1, 0) top:
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
		PushCoordBlocks                  (a_BlockX, Height + 1, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
	}
}





void GetPineTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Tall, little leaves on top. The top leaves are arranged in a shape of two cones joined by their bases.
	// There can be one or two layers representing the cone bases (SameSizeMax)

	int MyRandom = a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY, a_BlockZ + 32 * a_Seq) / 8;
	int TrunkHeight = 8 + (MyRandom % 3);
	int SameSizeMax = ((MyRandom & 8) == 0) ? 1 : 0;
	MyRandom >>= 3;
	int NumLeavesLayers = 2 + (MyRandom % 3);  // Number of layers that have leaves in them
	if (NumLeavesLayers == 2)
	{
		SameSizeMax = 0;
	}

	// Pre-allocate the vector:
	a_LogBlocks.reserve(static_cast<size_t>(TrunkHeight));
	a_OtherBlocks.reserve(static_cast<size_t>(NumLeavesLayers * 25));

	// The entire trunk, out of logs:
	for (int i = TrunkHeight; i >= 0; --i)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
	}
	int h = a_BlockY + TrunkHeight + 2;

	// Top layer - just a single leaves block:
	a_OtherBlocks.push_back(sSetBlock(a_BlockX, h, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
	h--;

	// One more layer is above the trunk, push the central leaves:
	a_OtherBlocks.push_back(sSetBlock(a_BlockX, h, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));

	// Layers expanding in size, then collapsing again:
	// LOGD("Generating %d layers of pine leaves, SameSizeMax = %d", NumLeavesLayers, SameSizeMax);
	for (int i = 0; i < NumLeavesLayers; ++i)
	{
		int LayerSize = std::min(i, NumLeavesLayers - i + SameSizeMax - 1);
		// LOGD("LayerSize %d: %d", i, LayerSize);
		if (LayerSize < 0)
		{
			break;
		}
		ASSERT(static_cast<size_t>(LayerSize) < ARRAYCOUNT(BigOLayers));
		PushCoordBlocks(a_BlockX, h, a_BlockZ, a_OtherBlocks, BigOLayers[LayerSize].Coords, BigOLayers[LayerSize].Count, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
		h--;
	}
}





void GetSwampTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Vines are around the BigO3, but not in the corners; need proper meta for direction
	static const sMetaCoords Vines[] =
	{
		{-2, -4, 1}, {-1, -4, 1}, {0, -4, 1}, {1, -4, 1}, {2, -4, 1},  // North face
		{-2,  4, 4}, {-1,  4, 4}, {0,  4, 4}, {1,  4, 4}, {2,  4, 4},  // South face
		{4,  -2, 2}, {4,  -1, 2}, {4,  0, 2}, {4,  1, 2}, {4,  2, 2},  // East face
		{-4, -2, 8}, {-4, -1, 8}, {-4, 0, 8}, {-4, 1, 8}, {-4, 2, 8},  // West face
	} ;

	int Height = 3 + (a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY, a_BlockZ + 32 * a_Seq) / 8) % 3;

	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(2 * ARRAYCOUNT(BigO2) + 2 * ARRAYCOUNT(BigO3) + static_cast<size_t>(Height) * ARRAYCOUNT(Vines) + 20);

	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
	}
	int hei = a_BlockY + Height - 2;

	// Put vines around the lowermost leaves layer:
	PushSomeColumns(a_BlockX, hei, a_BlockZ, Height, a_Seq, a_Noise, 0x3fffffff, a_OtherBlocks, Vines, ARRAYCOUNT(Vines), E_BLOCK_VINES);

	// The lower two leaves layers are BigO3 with log in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		PushCornerBlocks(a_BlockX, hei, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		hei++;
	}  // for i - 2*

	// The upper two leaves layers are BigO2 with leaves in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		PushCornerBlocks(a_BlockX, hei, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
		hei++;
	}  // for i - 2*
}





void GetAppleBushImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	a_OtherBlocks.reserve(3 + ARRAYCOUNT(BigO2) + ARRAYCOUNT(BigO1));

	int hei = a_BlockY;
	a_LogBlocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LOG, E_META_LOG_JUNGLE));
	PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
	hei++;

	a_OtherBlocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
	PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
	hei++;

	a_OtherBlocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
}





void GetJungleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large)
{
	if (!a_Large)
	{
		GetSmallJungleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
	else
	{
		GetLargeJungleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
}





void GetLargeJungleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	static const sMetaCoords VinesTrunk[] =
	{
		{0, -1, 1}, {1, -1, 1},  // North face
		{0,  2, 4}, {1,  2, 4},  // South face
		{-1, 1, 8}, {-1, 0, 8},  // West face
		{2,  1, 2}, {2,  0, 2},  // East face
	};

	int Height = 20 + (a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY, a_BlockZ + 32 * a_Seq) / 11) % 12;

	a_LogBlocks.reserve(static_cast<size_t>(Height) * 4);
	a_OtherBlocks.reserve(2 * ARRAYCOUNT(BigO5Jungle) + ARRAYCOUNT(BigO4Jungle) + ARRAYCOUNT(BigO3Jungle) + static_cast<size_t>(Height) * 4 + 50);

	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX,     a_BlockY + i, a_BlockZ,     E_BLOCK_LOG, E_META_LOG_JUNGLE));
		a_LogBlocks.push_back(sSetBlock(a_BlockX + 1, a_BlockY + i, a_BlockZ,     E_BLOCK_LOG, E_META_LOG_JUNGLE));
		a_LogBlocks.push_back(sSetBlock(a_BlockX,     a_BlockY + i, a_BlockZ + 1, E_BLOCK_LOG, E_META_LOG_JUNGLE));
		a_LogBlocks.push_back(sSetBlock(a_BlockX + 1, a_BlockY + i, a_BlockZ + 1, E_BLOCK_LOG, E_META_LOG_JUNGLE));

		// Randomly place vines around the trunk
		for (size_t j = 0; j < ARRAYCOUNT(VinesTrunk); j++)
		{
			if (a_Noise.IntNoise3DInt(a_BlockX + VinesTrunk[j].x, a_BlockY + i, a_BlockZ + VinesTrunk[j].z) % 3 != 0)
			{
				continue;
			}
			a_OtherBlocks.push_back(sSetBlock(a_BlockX + VinesTrunk[j].x, a_BlockY + i, a_BlockZ + VinesTrunk[j].z, E_BLOCK_VINES, VinesTrunk[j].Meta));
		}
	}
	int hei = a_BlockY + Height - 2;

	// Prevent floating trees by placing dirt under them
	for (int i = 1; i < 5; i++)
	{
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, a_BlockY - i, a_BlockZ, E_BLOCK_DIRT, E_META_DIRT_NORMAL));
		a_OtherBlocks.push_back(sSetBlock(a_BlockX + 1, a_BlockY - i, a_BlockZ, E_BLOCK_DIRT, E_META_DIRT_NORMAL));
		a_OtherBlocks.push_back(sSetBlock(a_BlockX, a_BlockY - i, a_BlockZ + 1, E_BLOCK_DIRT, E_META_DIRT_NORMAL));
		a_OtherBlocks.push_back(sSetBlock(a_BlockX + 1, a_BlockY - i, a_BlockZ + 1, E_BLOCK_DIRT, E_META_DIRT_NORMAL));
	}

	int numBranches = (a_Noise.IntNoise2DInt(a_BlockX * a_Seq, a_BlockZ * a_Seq) / 11) % 3 + 1;
	int branchStartHeight = 8 + Height % 11;
	int branchInterval = (Height - branchStartHeight) / numBranches;
	for (int i = branchStartHeight; i < (Height - 6); i += branchInterval)
	{
		// Get a direction for the trunk to go to.
		Vector3d BranchStartDirection = pickBranchDirection({ a_BlockX, a_BlockY + i, a_BlockZ }, a_Seq);
		Vector3d BranchDirection = pickBranchDirection({ a_BlockX, a_BlockY / i, a_BlockZ }, a_Seq) / 3;

		int BranchLength = 2 + a_Noise.IntNoise3DInt(a_BlockX * a_Seq, a_BlockY * a_Seq, a_BlockZ * a_Seq) % 2;
		Vector3i BranchEndPosition = GetTreeBranch(E_BLOCK_LOG, E_META_LOG_JUNGLE, a_BlockX, a_BlockY + i, a_BlockZ, BranchLength, BranchStartDirection, BranchDirection, a_BlockY + Height, a_LogBlocks).Floor();
		PushCoordBlocks(BranchEndPosition.x, BranchEndPosition.y, BranchEndPosition.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE);
		PushCoordBlocks(BranchEndPosition.x, BranchEndPosition.y + 1, BranchEndPosition.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE);
		a_OtherBlocks.push_back(sSetBlock(BranchEndPosition.x, BranchEndPosition.y + 1, BranchEndPosition.z, E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE));
	}

	// Place the canopy.
	for (size_t i = 0; i < ARRAYCOUNT(BigOJungleLayers); i++)
	{
		PushCoordBlocks(a_BlockX, hei++, a_BlockZ, a_OtherBlocks, BigOJungleLayers[i].Coords, BigOJungleLayers[i].Count, E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE);
	}
}





void GetSmallJungleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Vines are around the BigO3, but not in the corners; need proper meta for direction
	static const sMetaCoords Vines[] =
	{
		{-2, -4, 1}, {-1, -4, 1}, {0, -4, 1}, {1, -4, 1}, {2, -4, 1},  // North face
		{-2,  4, 4}, {-1,  4, 4}, {0,  4, 4}, {1,  4, 4}, {2,  4, 4},  // South face
		{4,  -2, 2}, {4,  -1, 2}, {4,  0, 2}, {4,  1, 2}, {4,  2, 2},  // East face
		{-4, -2, 8}, {-4, -1, 8}, {-4, 0, 8}, {-4, 1, 8},              // West face
		// TODO: proper metas and height: {0,  1, 1},  {0, -1, 4},  {-1, 0, 2}, {1,  1, 8},  // Around the tunk
	} ;

	int Height = 7 + (a_Noise.IntNoise3DInt(a_BlockX + 5 * a_Seq, a_BlockY, a_BlockZ + 5 * a_Seq) / 5) % 3;

	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(
		2 * ARRAYCOUNT(BigO3) +       // O3 layer, 2x
		2 * ARRAYCOUNT(BigO2) +       // O2 layer, 2x
		ARRAYCOUNT(BigO1) + 1 +       // Plus on the top
		static_cast<size_t>(Height) * ARRAYCOUNT(Vines) +  // Vines
		50  // some safety
	);

	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_JUNGLE));
	}
	int hei = a_BlockY + Height - 3;

	// Put vines around the lowermost leaves layer:
	PushSomeColumns(a_BlockX, hei, a_BlockZ, Height, a_Seq, a_Noise, 0x3fffffff, a_OtherBlocks, Vines, ARRAYCOUNT(Vines), E_BLOCK_VINES);

	// The lower two leaves layers are BigO3 with log in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE);
		PushCornerBlocks(a_BlockX, hei, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE);
		hei++;
	}  // for i - 2*

	// Two layers of BigO2 leaves, possibly with corners:
	for (int i = 0; i < 1; i++)
	{
		PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE);
		PushCornerBlocks(a_BlockX, hei, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 2, E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE);
		hei++;
	}  // for i - 2*

	// Top plus, all leaves:
	PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE);
	a_OtherBlocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_JUNGLE));
}
