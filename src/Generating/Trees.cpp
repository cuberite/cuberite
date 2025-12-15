
// Trees.cpp

// Implements helper functions used for generating trees

#include "Globals.h"
#include "Trees.h"
#include "../BlockType.h"
#include "../World.h"





typedef struct
{
	int x, z;
} sCoords;

typedef struct
{
	int x, z;
	unsigned char Dir;
} sDirectionCoords;

static const sCoords Corners[] =
{
	{-1, -1},
	{-1, 1},
	{1, -1},
	{1, 1},
} ;

/** Chooses a direction for a branch to go to based on the start position (a_BlockPos) and a_Seq. */
static const Vector3d & pickBranchDirection(const cNoise a_Noise, Vector3i a_BlockPos, int a_Seq)
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

	size_t index = static_cast<size_t>(a_Noise.IntNoise3DInt(a_BlockPos * a_Seq)) % directions.size();
	return directions[index];
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

static const sCoords TopLargePine[] =
{
	/*  0 */ {0,  0}, {1,  0},
	/*  1 */ {0,  1}, {1,  1},
} ;

static const sCoords BigOF2Pine[] =
{
	/* -1 */           {0, -1}, {1, -1},
	/*  0 */ {-1,  0}, {0,  0}, {1,  0}, {2,  0},
	/*  1 */ {-1,  1}, {0,  1}, {1,  1}, {2,  1},
	/*  2 */           {0,  2}, {1,  2},
} ;

static const sCoords BigO2Pine[] =
{
	/* -2 */                     {0, -2}, {1, -2},
	/* -1 */           {-1, -1}, {0, -1}, {1, -1}, {2, -1},
	/*  0 */ {-2,  0}, {-1,  0},                   {2,  0}, {3,  0},
	/*  1 */ {-2,  1}, {-1,  1},                   {2,  1}, {3,  1},
	/*  2 */           {-1,  2}, {0,  2}, {1,  2}, {2,  2},
	/*  3 */                     {0,  3}, {1,  3},
} ;

static const sCoords BigO3Pine[] =
{
	/* -3 */                               {0, -3}, {1, -3},
	/* -2 */                     {-1, -2}, {0, -2}, {1, -2}, {2, -2},
	/* -1 */           {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1},
	/*  0 */ {-3,  0}, {-2,  0}, {-1,  0},                   {2,  0}, {3,  0}, {4,  0},
	/*  1 */ {-3,  1}, {-2,  1}, {-1,  1},                   {2,  1}, {3,  1}, {4,  1},
	/*  2 */           {-2,  2}, {-1,  2}, {0,  2}, {1,  2}, {2,  2}, {3,  2},
	/*  3 */                     {-1,  3}, {0,  3}, {1,  3}, {2,  3},
	/*  4 */                               {0,  4}, {1,  4},
};

static const sCoords BigO1Spruce[] =
{
	/* -1 */           {0, -1}, {1, -1},
	/*  0 */ {-1,  0},                   {2,  0},
	/*  1 */ {-1,  1},                   {2,  1},
	/*  2 */           {0,  2}, {1,  2},
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

static const sCoordsArr BigOPineLayers[] =
{
	{BigO3Pine, ARRAYCOUNT(BigO3Pine)},
	{BigO2Pine, ARRAYCOUNT(BigO2Pine)},
	{BigOF2Pine, ARRAYCOUNT(BigOF2Pine)},
	{TopLargePine, ARRAYCOUNT(TopLargePine)},
};

static const sCoordsArr BigOSpruceLayersV1[] =
{
	{BigO1Spruce, ARRAYCOUNT(BigO1Spruce)},
	{nullptr, 0},
};

static const sCoordsArr BigOSpruceLayersV2[] =
{
	{BigO2Pine, ARRAYCOUNT(BigO2Pine)},
	{BigO1Spruce, ARRAYCOUNT(BigO1Spruce)},
};

static const sCoordsArr BigOSpruceLayersV3[] =
{
	{BigO3Pine, ARRAYCOUNT(BigO3Pine)},
	{BigO2Pine, ARRAYCOUNT(BigO2Pine)},
};

static const sCoordsArr BigOSpruceLayersV4[] =
{
	{BigO4Jungle, ARRAYCOUNT(BigO4Jungle)},
	{BigO3Pine, ARRAYCOUNT(BigO3Pine)},
};

static const sCoordsArr BigOSpruceTop[] =
{
	{BigO1Spruce, ARRAYCOUNT(BigO1Spruce)},
	{TopLargePine, ARRAYCOUNT(TopLargePine)},
};





/** Pushes a specified layer of blocks of the same type around (x, h, z) into a_Blocks */
inline void PushCoordBlocks(int a_BlockX, int a_Height, int a_BlockZ, sSetBlockVector & a_Blocks, const sCoords * a_Coords, size_t a_NumCoords, BlockState a_Block)
{
	for (size_t i = 0; i < a_NumCoords; i++)
	{
		a_Blocks.emplace_back(a_BlockX + a_Coords[i].x, a_Height, a_BlockZ + a_Coords[i].z, a_Block);
	}
}





inline void PushCornerBlocks(int a_BlockX, int a_Height, int a_BlockZ, int a_Seq, cNoise & a_Noise, int a_Chance, sSetBlockVector & a_Blocks, int a_CornersDist, BlockState a_Block)
{
	for (size_t i = 0; i < ARRAYCOUNT(Corners); i++)
	{
		int x = a_BlockX + Corners[i].x;
		int z = a_BlockZ + Corners[i].z;
		if (a_Noise.IntNoise3DInt(x + 64 * a_Seq, a_Height, z + 64 * a_Seq) <= a_Chance)
		{
			a_Blocks.emplace_back(x, a_Height, z, a_Block);
		}
	}  // for i - Corners[]
}





inline BlockState DirToVineBlock(unsigned char a_Dir)
{
	switch (a_Dir)
	{
		case 1: return Block::Vine::Vine(false, false, true, false, false);
		case 2: return Block::Vine::Vine(false, false, false, false, true);
		case 4: return Block::Vine::Vine(false, true, false, false, false);
		case 8: return Block::Vine::Vine(true, false, false, false, false);
	}
	return Block::Vine::Vine();
}





inline void PushSomeVines(int a_BlockX, int a_Height, int a_BlockZ, int a_ColumnHeight, int a_Seq, cNoise & a_Noise, int a_Chance, sSetBlockVector & a_Blocks, const sDirectionCoords * a_Coords, size_t a_NumCoords)
{
	for (size_t i = 0; i < a_NumCoords; i++)
	{
		int x = a_BlockX + a_Coords[i].x;
		int z = a_BlockZ + a_Coords[i].z;
		if (a_Noise.IntNoise3DInt(x + 64 * a_Seq, a_Height + static_cast<int>(i), z + 64 * a_Seq) <= a_Chance)
		{
			for (int j = 0; j < a_ColumnHeight; j++)
			{
				a_Blocks.emplace_back(x, a_Height - j, z, DirToVineBlock(a_Coords[i].Dir));
			}
		}
	}  // for i - a_Coords[]
}





void GetTreeImageByBiome(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, EMCSBiome a_Biome, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	switch (a_Biome)
	{
		case biPlains:
		case biForest:
		case biForestHills:
		case biSunflowerPlains:
		case biFlowerForest:
		case biDeepOcean:
		case biStoneBeach:
		case biColdBeach:
		{
			// Apple or birch trees:
			if (a_Noise.IntNoise3DInt(a_BlockPos.addedY(16 * a_Seq).addedZ(16 * a_Seq)) < 0x5fffffff)
			{
				GetAppleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else
			{
				GetBirchTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			return;
		}

		case biColdTaiga:
		case biColdTaigaM:
		case biColdTaigaHills:
		case biIcePlains:
		case biIceMountains:
		case biTaiga:
		case biTaigaHills:
		case biTaigaM:
		{
			// Conifers
			GetConiferTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks, false);
			return;
		}

		case biSwamplandM:
		case biSwampland:
		{
			// Swamp trees:
			GetSwampTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biJungle:
		case biJungleHills:
		case biJungleEdge:
		case biJungleM:
		case biJungleEdgeM:
		{
			// Apple bushes, large jungle trees, small jungle trees
			if (a_Noise.IntNoise3DInt(a_BlockPos.addedY(16 * a_Seq).addedZ(16 * a_Seq)) < 0x6fffffff)
			{
				GetAppleBushImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else
			{
				bool IsLarge = a_Noise.IntNoise3DInt(a_BlockPos.addedX(32 * a_Seq).addedY(32 * a_Seq)) < 0x60000000;
				GetJungleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks, IsLarge);
			}
			return;
		}

		case biBirchForest:
		case biBirchForestHills:
		{
			GetBirchTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biBirchForestM:
		case biBirchForestHillsM:
		{
			GetTallBirchTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biMegaTaiga:
		case biMegaTaigaHills:
		{
			bool IsLarge = a_Noise.IntNoise3DInt(a_BlockPos.addedX(32 * a_Seq).addedY(32 * a_Seq)) < 0x30000000;
			GetConiferTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks, IsLarge);
			return;
		}

		case biMegaSpruceTaiga:
		case biMegaSpruceTaigaHills:
		{
			bool IsLarge = a_Noise.IntNoise3DInt(a_BlockPos.addedX(32 * a_Seq).addedY(32 * a_Seq)) < 0x30000000;
			GetSpruceTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks, IsLarge);
			return;
		}

		case biExtremeHills:
		case biExtremeHillsM:
		case biExtremeHillsEdge:
		case biExtremeHillsPlus:
		case biExtremeHillsPlusM:
		{
			if (a_Noise.IntNoise3DInt(a_BlockPos.addedY(16 * a_Seq).addedZ(16 * a_Seq)) < 0x6fffffff)
			{
				GetConiferTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else
			{
				GetAppleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			return;
		}

		case biSavanna:
		case biSavannaPlateau:
		case biSavannaM:
		case biSavannaPlateauM:
		{
			GetAcaciaTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}

		case biRoofedForest:
		case biRoofedForestM:
		{
			// 53 % DarkOak
			// 23 % SmallAppleTree
			// 16 % RedMushroom
			// 4 % BrownMushroom
			// 4 % Birch
			const int DarkOakProb = 53;
			const int SmallAppleTreeProb = DarkOakProb + 23;
			const int RedMushroomProb = SmallAppleTreeProb + 16;
			const int BrownMushroomProb = RedMushroomProb + 4;

			static_assert(BrownMushroomProb + 4 == 100);

			auto RandomValue = a_Noise.IntNoise3DInt(a_BlockPos.addedY(16 * a_Seq).addedZ(16 * a_Seq)) % 100;
			if (RandomValue < DarkOakProb)
			{
				GetDarkoakTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else if ((RandomValue >= DarkOakProb) && (RandomValue < SmallAppleTreeProb))
			{
				GetSmallAppleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else if ((RandomValue >= SmallAppleTreeProb) && (RandomValue < RedMushroomProb))
			{
				GetRedMushroomTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else if ((RandomValue >= RedMushroomProb) && (RandomValue < BrownMushroomProb))
			{
				GetBrownMushroomTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else
			{
				GetBirchTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			return;
		}

		case biMesa:
		case biMesaPlateauF:
		case biMesaPlateau:
		case biMesaBryce:
		case biMesaPlateauFM:
		case biMesaPlateauM:
		{
			GetSmallAppleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			return;
		}
		case biMushroomIsland:
		case biMushroomShore:
		{
			if (a_Noise.IntNoise3DInt(a_BlockPos.addedY(16 * a_Seq).addedZ(16 * a_Seq)) < 0x6fffffff)
			{
				GetBrownMushroomTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			else
			{
				GetRedMushroomTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
			}
			return;
		}
		case biDesert:
		case biDesertHills:
		case biDesertM:
		case biRiver:
		case biBeach:
		case biHell:
		case biSky:
		case biOcean:
		case biFrozenOcean:
		case biFrozenRiver:
		case biIcePlainsSpikes:
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





void GetAppleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	if (a_Noise.IntNoise3DInt(a_BlockPos.addedX(32 * a_Seq).addedY(32 * a_Seq)) < 0x73333333)
	{
		GetSmallAppleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
	else
	{
		GetLargeAppleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
}





void GetSmallAppleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	/* Small apple tree has:
	- a top plus (no log)
	- optional BigO1 + random corners (log)
	- 2 layers of BigO2 + random corners (log)
	- 1 to 3 blocks of trunk
	*/

	int Random = a_Noise.IntNoise3DInt(a_BlockPos.addedX(64 * a_Seq)) >> 3;
	HEIGHTTYPE Heights[] = {1, 2, 2, 3} ;
	HEIGHTTYPE Height = 1 + Heights[Random & 3];
	Random >>= 2;

	// Pre-alloc so that we don't realloc too often later:
	a_LogBlocks.reserve(static_cast<size_t>(Height + 5));
	a_OtherBlocks.reserve(ARRAYCOUNT(BigO2) * 2 + ARRAYCOUNT(BigO1) + ARRAYCOUNT(Corners) * 3 + 3 + 5);

	// Trunk:
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::OakLog::OakLog());
	}
	int Hei = a_BlockPos.y + Height;

	// 2 BigO2 + corners layers:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks (a_BlockPos.x, Hei, a_BlockPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::OakLeaves::OakLeaves());
		PushCornerBlocks(a_BlockPos.x, Hei, a_BlockPos.z, a_Seq, a_Noise, 0x5000000 - i * 0x10000000, a_OtherBlocks, 2, Block::OakLeaves::OakLeaves());
		a_LogBlocks.emplace_back(a_BlockPos.x, Hei, a_BlockPos.z, Block::OakLog::OakLog());
		Hei++;
	}  // for i - 2*

	// Optional BigO1 + corners layer:
	if ((Random & 1) == 0)
	{
		PushCoordBlocks (a_BlockPos.x, Hei, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::OakLeaves::OakLeaves());
		PushCornerBlocks(a_BlockPos.x, Hei, a_BlockPos.z, a_Seq, a_Noise, 0x6000000, a_OtherBlocks, 1, Block::OakLeaves::OakLeaves());
		a_LogBlocks.emplace_back(a_BlockPos.x, Hei, a_BlockPos.z, Block::OakLog::OakLog());
		Hei++;
	}

	// Top plus:
	PushCoordBlocks(a_BlockPos.x, Hei, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::OakLeaves::OakLeaves());
	a_OtherBlocks.emplace_back(a_BlockPos.x, Hei, a_BlockPos.z, Block::OakLeaves::OakLeaves());
}





void GetLargeAppleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	int Height = 7 + a_Noise.IntNoise3DInt(a_BlockPos) % 4;

	// Create branches
	for (int i = 4; i < Height; i++)
	{
		// Get a direction for the trunk to go to.
		Vector3d BranchStartDirection = pickBranchDirection(a_Noise, a_BlockPos.addedY(i), a_Seq);
		Vector3d BranchDirection = pickBranchDirection(a_Noise, a_BlockPos.addedY(i * a_Seq), a_Seq) / 3;

		int BranchLength = 2 + a_Noise.IntNoise3DInt(a_BlockPos * a_Seq) % 3;
		GetTreeBranch(Block::OakLog::OakLog(), a_BlockPos.addedY(i), BranchLength, BranchStartDirection, BranchDirection, a_LogBlocks);
	}

	// Place leaves around each log block
	for (auto itr : a_LogBlocks)
	{
		// Get the log's X and Z coordinates
		int X = itr.GetX();
		int Z = itr.GetZ();

		a_OtherBlocks.emplace_back(X, itr.m_RelY - 2, Z, Block::OakLeaves::OakLeaves());
		PushCoordBlocks(X, itr.m_RelY - 2, Z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::OakLeaves::OakLeaves());
		for (int y = -1; y <= 1; y++)
		{
			PushCoordBlocks (X, itr.m_RelY + y, Z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::OakLeaves::OakLeaves());
		}
		PushCoordBlocks(X, itr.m_RelY + 2, Z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::OakLeaves::OakLeaves());
		a_OtherBlocks.emplace_back(X, itr.m_RelY + 2, Z, Block::OakLeaves::OakLeaves());
	}

	// Trunk:
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::OakLog::OakLog());
	}
}





Vector3d GetTreeBranch(BlockState a_Block, Vector3i a_BlockPos, int a_BranchLength, Vector3d a_StartDirection, Vector3d a_Direction, sSetBlockVector & a_LogBlocks)
{
	Vector3d CurrentPos = Vector3d(a_BlockPos);
	Vector3d Direction  = a_StartDirection;
	for (int i = 0; i < a_BranchLength; i++)
	{
		CurrentPos += Direction;
		Direction += a_Direction;
		Direction.Clamp(-1.0, 1.0);
		a_LogBlocks.emplace_back(CurrentPos.Floor(), RotateLogForDirection(a_Block, Direction));
	}
	return CurrentPos;
}





BlockState RotateLogForDirection(BlockState a_Block, Vector3d a_Direction)
{
	using namespace Block;
	a_Direction.Abs();

	if ((a_Direction.y > a_Direction.x) && (a_Direction.y > a_Direction.z))
	{
		return a_Block;
	}
	else if (a_Direction.x > a_Direction.z)
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLog:  return AcaciaLog::AcaciaLog(AcaciaLog::Axis::X);
			case BlockType::BirchLog:   return BirchLog::BirchLog(BirchLog::Axis::X);
			case BlockType::DarkOakLog: return DarkOakLog::DarkOakLog(DarkOakLog::Axis::X);
			case BlockType::JungleLog:  return JungleLog::JungleLog(JungleLog::Axis::X);
			case BlockType::OakLog:     return OakLog::OakLog(OakLog::Axis::X);
			case BlockType::SpruceLog:  return SpruceLog::SpruceLog(SpruceLog::Axis::X);
			default: return a_Block;
		}
	}
	else
	{
		switch (a_Block.Type())
		{
			case BlockType::AcaciaLog:  return AcaciaLog::AcaciaLog(AcaciaLog::Axis::Z);
			case BlockType::BirchLog:   return BirchLog::BirchLog(BirchLog::Axis::Z);
			case BlockType::DarkOakLog: return DarkOakLog::DarkOakLog(DarkOakLog::Axis::Z);
			case BlockType::JungleLog:  return JungleLog::JungleLog(JungleLog::Axis::Z);
			case BlockType::OakLog:     return OakLog::OakLog(OakLog::Axis::Z);
			case BlockType::SpruceLog:  return SpruceLog::SpruceLog(SpruceLog::Axis::Z);
			default: return a_Block;
		}
	}
}





void GetBirchTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	HEIGHTTYPE Height = 5 + static_cast<HEIGHTTYPE>(a_Noise.IntNoise3DInt(a_BlockPos.addedX(64 * a_Seq)) % 3);

	// Prealloc, so that we don't realloc too often later:
	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(80);

	// The entire trunk, out of logs:
	for (int i = Height - 1; i >= 0; --i)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::BirchLog::BirchLog());
	}
	int h = a_BlockPos.y + Height;

	// Top layer - just the Plus:
	PushCoordBlocks(a_BlockPos.x, h, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::BirchLeaves::BirchLeaves());
	a_OtherBlocks.emplace_back(a_BlockPos.x, h, a_BlockPos.z, Block::BirchLeaves::BirchLeaves());  // There's no log at this layer
	h--;

	// Second layer - log, Plus and maybe Corners:
	PushCoordBlocks (a_BlockPos.x, h, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::BirchLeaves::BirchLeaves());
	PushCornerBlocks(a_BlockPos.x, h, a_BlockPos.z, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 1, Block::BirchLeaves::BirchLeaves());
	h--;

	// Third and fourth layers - BigO2 and maybe 2 * Corners:
	for (int Row = 0; Row < 2; Row++)
	{
		PushCoordBlocks (a_BlockPos.x, h, a_BlockPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::BirchLeaves::BirchLeaves());
		PushCornerBlocks(a_BlockPos.x, h, a_BlockPos.z, a_Seq, a_Noise, 0x3fffffff + Row * 0x10000000, a_OtherBlocks, 2, Block::BirchLeaves::BirchLeaves());
		h--;
	}  // for Row - 2*
}





void GetAcaciaTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Calculate a base height
	int Height = 2 + (a_Noise.IntNoise3DInt(a_BlockPos) / 11 % 3);

	// Create the trunk
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::AcaciaLog::AcaciaLog());
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
	Vector3i BranchDirection = AvailableDirections[a_Noise.IntNoise3DInt(a_BlockPos) % 8];

	// Calculate a height for the branch between 1 and 3
	int BranchHeight = a_Noise.IntNoise3DInt(a_BlockPos) % 3 + 1;

	Vector3i BranchPos = GetTreeBranch(Block::AcaciaLog::AcaciaLog(), a_BlockPos.addedY(Height - 1), BranchHeight, Vector3i(BranchDirection), Vector3i(BranchDirection), a_LogBlocks).Floor();

	// Add the leaves to the top of the branch
	PushCoordBlocks(BranchPos.x, BranchPos.y, BranchPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), Block::AcaciaLeaves::AcaciaLeaves());
	PushCoordBlocks(BranchPos.x, BranchPos.y + 1, BranchPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::AcaciaLeaves::AcaciaLeaves());
	a_OtherBlocks.emplace_back(BranchPos.x, BranchPos.y + 1, BranchPos.z, Block::AcaciaLeaves::AcaciaLeaves());

	// Choose if we have to add another branch
	bool TwoTop = (a_Noise.IntNoise3D(a_BlockPos) < 0);
	if (!TwoTop)
	{
		return;
	}

	// Invert the direction of the previous branch.
	BranchDirection = Vector3d(-BranchDirection.x, 1, -BranchDirection.z);

	// Calculate a new height for the second branch
	BranchHeight = a_Noise.IntNoise3DInt(a_BlockPos * a_Seq) % 3 + 1;

	BranchPos = GetTreeBranch(Block::AcaciaLog::AcaciaLog(), a_BlockPos.addedY(Height - 1), BranchHeight, BranchDirection, BranchDirection, a_LogBlocks).Floor();

	// And add the leaves ontop of the second branch
	PushCoordBlocks(BranchPos.x, BranchPos.y, BranchPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), Block::AcaciaLeaves::AcaciaLeaves());
	PushCoordBlocks(BranchPos.x, BranchPos.y + 1, BranchPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::AcaciaLeaves::AcaciaLeaves());
	a_OtherBlocks.emplace_back(BranchPos.x, BranchPos.y + 1, BranchPos.z, Block::AcaciaLeaves::AcaciaLeaves());
}





void GetDarkoakTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Pick a height
	int Height = 5 + (a_Noise.IntNoise3DInt(a_BlockPos.addedX(32 * a_Seq).addedZ(32 * a_Seq)) / 11) % 4;

	// Create the trunk
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::DarkOakLog::DarkOakLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedX(1).addedY(i), Block::DarkOakLog::DarkOakLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedZ(1), Block::DarkOakLog::DarkOakLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedX(1).addedY(i).addedZ(1), Block::DarkOakLog::DarkOakLog());
	}

	// Prevent floating trees by placing dirt under them
	for (int i = 1; i < 5; i++)
	{
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i), Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedX(1).addedY(-i), Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedZ(1), Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedX(1).addedY(-i).addedZ(1), Block::Dirt::Dirt());
	}

	// Create branches
	for (int i = 0; i < 3; i++)
	{
		int x = (a_Noise.IntNoise3DInt(a_BlockPos.addedX(32 * a_Seq).addedZ(32 * a_Seq).addedY(i * a_Seq)) % 3) - 1;
		int z = (a_Noise.IntNoise3DInt(a_BlockPos.addedX(-32 * a_Seq).addedZ(-32 * a_Seq).addedY(i * a_Seq)) % 3) - 1;

		// The branches would end up in the trunk.
		if ((x >= a_BlockPos.x) && (x <= a_BlockPos.x + 1) && (z >= a_BlockPos.z) && (z <= a_BlockPos.z + 1))
		{
			NOISE_DATATYPE Val1 = a_Noise.IntNoise2D(x, z);
			if (Val1 < 0)
			{
				x = a_BlockPos.x + ((Val1 < -0.5) ? -1 : 3);
			}
			else
			{
				z = a_BlockPos.z + ((Val1 < 0.5) ? -1 : 3);
			}
		}

		int y = Height - (a_Noise.IntNoise3DInt(a_BlockPos.addedX(x).addedZ(z).addedY(i * a_Seq)) % (Height - (Height / 4)));

		for (int Y = y; Y < Height; Y++)
		{
			a_LogBlocks.emplace_back(a_BlockPos.addedX(x).addedY(Y).addedZ(z), Block::DarkOakLog::DarkOakLog());
		}
	}

	int hei = a_BlockPos.y + Height - 2;

	// The lower two leaves layers are BigO4 with log in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO4, ARRAYCOUNT(BigO4), Block::DarkOakLeaves::DarkOakLeaves());
		PushCornerBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, Block::DarkOakLeaves::DarkOakLeaves());
		hei++;
	}  // for i < 2

	// The top leaves layer is a BigO3 with leaves in the middle and possibly corners:
	PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), Block::DarkOakLeaves::DarkOakLeaves());
	PushCornerBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, Block::DarkOakLeaves::DarkOakLeaves());
	a_OtherBlocks.emplace_back(a_BlockPos.x, hei, a_BlockPos.z, Block::DarkOakLeaves::DarkOakLeaves());
}





void GetTallBirchTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	HEIGHTTYPE Height = 9 + static_cast<HEIGHTTYPE>(a_Noise.IntNoise3DInt(a_BlockPos.addedX(64 * a_Seq)) % 3);

	// Prealloc, so that we don't realloc too often later:
	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(80);

	// The entire trunk, out of logs:
	for (int i = Height - 1; i >= 0; --i)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::BirchLog::BirchLog());
	}
	int h = a_BlockPos.y + Height;

	// Top layer - just the Plus:
	PushCoordBlocks(a_BlockPos.x, h, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::BirchLeaves::BirchLeaves());
	a_OtherBlocks.emplace_back(a_BlockPos.x, h, a_BlockPos.z, Block::BirchLeaves::BirchLeaves());  // There's no log at this layer
	h--;

	// Second layer - log, Plus and maybe Corners:
	PushCoordBlocks (a_BlockPos.x, h, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::BirchLeaves::BirchLeaves());
	PushCornerBlocks(a_BlockPos.x, h, a_BlockPos.z, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 1, Block::BirchLeaves::BirchLeaves());
	h--;

	// Third and fourth layers - BigO2 and maybe 2 * Corners:
	for (int Row = 0; Row < 2; Row++)
	{
		PushCoordBlocks (a_BlockPos.x, h, a_BlockPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::BirchLeaves::BirchLeaves());
		PushCornerBlocks(a_BlockPos.x, h, a_BlockPos.z, a_Seq, a_Noise, 0x3fffffff + Row * 0x10000000, a_OtherBlocks, 2, Block::BirchLeaves::BirchLeaves());
		h--;
	}  // for Row - 2*
}





void GetConiferTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large)
{
	// Half chance for a spruce, half for a pine and for the large ones 3 chances for a pine and one for spruce:
	if (a_Noise.IntNoise3DInt(a_BlockPos.addedX(64 * a_Seq).addedZ(32 * a_Seq)) < (a_Large ? 0x20000000 : 0x40000000))
	{
		GetSpruceTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks, a_Large);
	}
	else
	{
		GetPineTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks, a_Large);
	}
}





void GetSpruceTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large)
{
	if (a_Large)
	{
		GetLargeSpruceTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
	else
	{
		GetSmallSpruceTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}

}





void GetPineTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large)
{
	if (a_Large)
	{
		GetLargePineTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
	else
	{
		GetSmallPineTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}

}





void GetSmallSpruceTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Spruces have a top section with layer sizes of (0, 1, 0) or only (1, 0),
	// then 1 - 3 sections of ascending sizes (1, 2) [most often], (1, 3) or (1, 2, 3)
	// and an optional bottom section of size 1, followed by 1 - 3 clear trunk blocks

	// We'll use bits from this number as partial random numbers; but the noise function has mod8 irregularities
	// (each of the mod8 remainders has a very different chance of occurrence) - that's why we divide by 8
	int MyRandom = a_Noise.IntNoise3DInt(a_BlockPos.addedX(32 * a_Seq).addedY(32 * a_Seq)) / 8;

	static const HEIGHTTYPE  sHeights[] = {1, 2, 2, 3};
	HEIGHTTYPE Height = sHeights[MyRandom & 3];
	MyRandom >>= 2;

	// Prealloc, so that we don't realloc too often later:
	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(180);

	// Clear trunk blocks:
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::SpruceLog::SpruceLog());
	}
	Height += static_cast<HEIGHTTYPE>(a_BlockPos.y);

	// Optional size-1 bottom leaves layer:
	if ((MyRandom & 1) == 0)
	{
		PushCoordBlocks(a_BlockPos.x, Height, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::SpruceLeaves::SpruceLeaves());
		a_OtherBlocks.emplace_back(a_BlockPos.x, Height, a_BlockPos.z, Block::SpruceLog::SpruceLog());
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
				PushCoordBlocks(a_BlockPos.x, Height,     a_BlockPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::SpruceLeaves::SpruceLeaves());
				PushCoordBlocks(a_BlockPos.x, Height + 1, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::SpruceLeaves::SpruceLeaves());
				a_LogBlocks.emplace_back(a_BlockPos.x, Height,     a_BlockPos.z, Block::SpruceLog::SpruceLog());
				a_LogBlocks.emplace_back(a_BlockPos.x, Height + 1, a_BlockPos.z, Block::SpruceLog::SpruceLog());
				Height += 2;
				break;
			}
			case 2:
			{
				PushCoordBlocks(a_BlockPos.x, Height,     a_BlockPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), Block::SpruceLeaves::SpruceLeaves());
				PushCoordBlocks(a_BlockPos.x, Height + 1, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::SpruceLeaves::SpruceLeaves());
				a_LogBlocks.emplace_back(a_BlockPos.x, Height,     a_BlockPos.z, Block::SpruceLog::SpruceLog());
				a_LogBlocks.emplace_back(a_BlockPos.x, Height + 1, a_BlockPos.z, Block::SpruceLog::SpruceLog());
				Height += 2;
				break;
			}
			case 3:
			{
				PushCoordBlocks(a_BlockPos.x, Height,     a_BlockPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), Block::SpruceLeaves::SpruceLeaves());
				PushCoordBlocks(a_BlockPos.x, Height + 1, a_BlockPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::SpruceLeaves::SpruceLeaves());
				PushCoordBlocks(a_BlockPos.x, Height + 2, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::SpruceLeaves::SpruceLeaves());
				a_LogBlocks.emplace_back(a_BlockPos.x, Height,     a_BlockPos.z, Block::SpruceLog::SpruceLog());
				a_LogBlocks.emplace_back(a_BlockPos.x, Height + 1, a_BlockPos.z, Block::SpruceLog::SpruceLog());
				a_LogBlocks.emplace_back(a_BlockPos.x, Height + 2, a_BlockPos.z, Block::SpruceLog::SpruceLog());
				Height += 3;
				break;
			}
		}  // switch (SectionType)
		MyRandom >>= 2;
	}  // for i - Sections

	if ((MyRandom & 1) == 0)
	{
		// (0, 1, 0) top:
		a_LogBlocks.emplace_back  (a_BlockPos.x, Height,     a_BlockPos.z, Block::SpruceLog::SpruceLog());
		PushCoordBlocks                  (a_BlockPos.x, Height + 1, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::SpruceLeaves::SpruceLeaves());
		a_OtherBlocks.emplace_back(a_BlockPos.x, Height + 1, a_BlockPos.z, Block::SpruceLeaves::SpruceLeaves());
		a_OtherBlocks.emplace_back(a_BlockPos.x, Height + 2, a_BlockPos.z, Block::SpruceLeaves::SpruceLeaves());
	}
	else
	{
		// (1, 0) top:
		a_OtherBlocks.emplace_back(a_BlockPos.x, Height,     a_BlockPos.z, Block::SpruceLeaves::SpruceLeaves());
		PushCoordBlocks                  (a_BlockPos.x, Height + 1, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::SpruceLeaves::SpruceLeaves());
		a_OtherBlocks.emplace_back(a_BlockPos.x, Height + 1, a_BlockPos.z, Block::SpruceLeaves::SpruceLeaves());
	}
}





static void LargeSpruceAddRing(Vector3i a_BlockPos, int & a_Height, const sCoordsArr* const a_Ring, size_t a_RingCount, sSetBlockVector & a_OtherBlocks)
{
	for (size_t i = 0; i < a_RingCount ; i++)
	{
		PushCoordBlocks(a_BlockPos.x, a_Height--, a_BlockPos.z, a_OtherBlocks, a_Ring[a_RingCount - 1 - i].Coords, a_Ring[a_RingCount - 1 - i].Count, Block::SpruceLeaves::SpruceLeaves());
	}
}





const int MIN_LARGE_SPRUCE_TREE_RINGS = 3;
const int MAX_LARGE_SPRUCE_TREE_RINGS = 11;
void GetLargeSpruceTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	int Height = 20 + (a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(32 * a_Seq, 32 * a_Seq)) / 11) % 12;
	int LeavesRingCount =
		MIN_LARGE_SPRUCE_TREE_RINGS +
		(a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(32 * a_Seq, 32 * a_Seq)) / 13) % (1 + MAX_LARGE_SPRUCE_TREE_RINGS - MIN_LARGE_SPRUCE_TREE_RINGS);

	int RingRadius = 0;

	a_LogBlocks.reserve(static_cast<size_t>(Height) * 4);
	a_OtherBlocks.reserve(ARRAYCOUNT(BigO1Spruce) + ARRAYCOUNT(TopLargePine) +
		MAX_LARGE_SPRUCE_TREE_RINGS * (ARRAYCOUNT(BigO4Jungle) + ARRAYCOUNT(BigO3Pine) + ARRAYCOUNT(BigO2Pine)));

	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i),               Block::SpruceLog::SpruceLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedX(1),     Block::SpruceLog::SpruceLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedZ(1),     Block::SpruceLog::SpruceLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(1, 1), Block::SpruceLog::SpruceLog());
	}
	int hei = a_BlockPos.y + Height - 1;

	// Prevent floating trees by placing dirt under them
	for (int i = 1; i < 5; i++)
	{
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i),               Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedX(1),     Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedZ(1),     Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedXZ(1, 1), Block::Dirt::Dirt());
	}

	// Place the top.
	for (size_t i = 0; i < ARRAYCOUNT(BigOSpruceTop); i++)
	{
		PushCoordBlocks(a_BlockPos.x, hei++, a_BlockPos.z, a_OtherBlocks, BigOSpruceTop[i].Coords, BigOSpruceTop[i].Count, Block::SpruceLeaves::SpruceLeaves());
	}

	hei = a_BlockPos.y + Height - 2;

	for (int i = 0; i < LeavesRingCount; i++)
	{
		unsigned int Val = static_cast<unsigned int>(a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(32 * a_Seq, 32 * i)) / 23) % 8;
		if ((Val < 4) && RingRadius < 3)
		{
			RingRadius++;
		}
		else if ((Val == 7) && (RingRadius > 0))
		{
			RingRadius--;
		}

		switch (RingRadius)
		{
			case 0:
			{
				LargeSpruceAddRing(a_BlockPos, hei, BigOSpruceLayersV1, ARRAYCOUNT(BigOSpruceLayersV1), a_OtherBlocks);
				break;
			}
			case 1:
			{
				LargeSpruceAddRing(a_BlockPos, hei, BigOSpruceLayersV2, ARRAYCOUNT(BigOSpruceLayersV2), a_OtherBlocks);
				break;
			}
			case 2:
			{
				LargeSpruceAddRing(a_BlockPos, hei, BigOSpruceLayersV3, ARRAYCOUNT(BigOSpruceLayersV3), a_OtherBlocks);
				break;
			}
			case 3:
			{
				LargeSpruceAddRing(a_BlockPos, hei, BigOSpruceLayersV4, ARRAYCOUNT(BigOSpruceLayersV4), a_OtherBlocks);
				break;
			}
			default:
			{
				break;
			}
		}

		if (hei - a_BlockPos.y <= 2)
		{
			break;
		}

	}
}





void GetSmallPineTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Tall, little leaves on top. The top leaves are arranged in a shape of two cones joined by their bases.
	// There can be one or two layers representing the cone bases (SameSizeMax)

	int MyRandom = a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(32 * a_Seq, 32 * a_Seq)) / 8;
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
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::SpruceLog::SpruceLog());
	}
	int h = a_BlockPos.y + TrunkHeight + 2;

	// Top layer - just a single leaves block:
	a_OtherBlocks.emplace_back(a_BlockPos.x, h, a_BlockPos.z, Block::SpruceLeaves::SpruceLeaves());
	h--;

	// One more layer is above the trunk, push the central leaves:
	a_OtherBlocks.emplace_back(a_BlockPos.x, h, a_BlockPos.z, Block::SpruceLeaves::SpruceLeaves());

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
		PushCoordBlocks(a_BlockPos.x, h, a_BlockPos.z, a_OtherBlocks, BigOLayers[LayerSize].Coords, BigOLayers[LayerSize].Count, Block::SpruceLeaves::SpruceLeaves());
		h--;
	}
}





void GetLargePineTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	int Height = 20 + (a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(32 * a_Seq, 32 * a_Seq)) / 11) % 12;

	a_LogBlocks.reserve(static_cast<size_t>(Height) * 4);
	a_OtherBlocks.reserve(ARRAYCOUNT(BigO3Pine) + ARRAYCOUNT(BigO2Pine) + ARRAYCOUNT(BigOF2Pine) + ARRAYCOUNT(TopLargePine));

	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i),               Block::SpruceLog::SpruceLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedX(1),     Block::SpruceLog::SpruceLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedZ(1),     Block::SpruceLog::SpruceLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(1, 1), Block::SpruceLog::SpruceLog());
	}
	int hei = a_BlockPos.y + Height - 2;

	// Prevent floating trees by placing dirt under them
	for (int i = 1; i < 5; i++)
	{
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i),               Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedX(1),     Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedZ(1),     Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedXZ(1, 1), Block::Dirt::Dirt());
	}

	// Place the canopy.
	for (size_t i = 0; i < ARRAYCOUNT(BigOPineLayers); i++)
	{
		PushCoordBlocks(a_BlockPos.x, hei++, a_BlockPos.z, a_OtherBlocks, BigOPineLayers[i].Coords, BigOPineLayers[i].Count, Block::SpruceLeaves::SpruceLeaves());
	}
}





void GetSwampTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Vines are around the BigO3, but not in the corners; need proper meta for direction
	static const sDirectionCoords Vines[] =
	{
		{-2, -4, 1}, {-1, -4, 1}, {0, -4, 1}, {1, -4, 1}, {2, -4, 1},  // North face
		{-2,  4, 4}, {-1,  4, 4}, {0,  4, 4}, {1,  4, 4}, {2,  4, 4},  // South face
		{4,  -2, 2}, {4,  -1, 2}, {4,  0, 2}, {4,  1, 2}, {4,  2, 2},  // East face
		{-4, -2, 8}, {-4, -1, 8}, {-4, 0, 8}, {-4, 1, 8}, {-4, 2, 8},  // West face
	} ;

	int Height = 3 + (a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(32 * a_Seq, 32 * a_Seq)) / 8) % 3;

	a_LogBlocks.reserve(static_cast<size_t>(Height));
	a_OtherBlocks.reserve(2 * ARRAYCOUNT(BigO2) + 2 * ARRAYCOUNT(BigO3) + static_cast<size_t>(Height) * ARRAYCOUNT(Vines) + 20);

	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::OakLog::OakLog());
	}
	int hei = a_BlockPos.y + Height - 2;

	// Put vines around the lowermost leaves layer:
	PushSomeVines(a_BlockPos.x, hei, a_BlockPos.z, Height, a_Seq, a_Noise, 0x3fffffff, a_OtherBlocks, Vines, ARRAYCOUNT(Vines));

	// The lower two leaves layers are BigO3 with log in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), Block::OakLeaves::OakLeaves());
		PushCornerBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, Block::OakLeaves::OakLeaves());
		hei++;
	}  // for i - 2*

	// The upper two leaves layers are BigO2 with leaves in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::OakLeaves::OakLeaves());
		PushCornerBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, Block::OakLeaves::OakLeaves());
		a_OtherBlocks.emplace_back(a_BlockPos.x, hei, a_BlockPos.z, Block::OakLeaves::OakLeaves());
		hei++;
	}  // for i - 2*
}





void GetAppleBushImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	a_OtherBlocks.reserve(3 + ARRAYCOUNT(BigO2) + ARRAYCOUNT(BigO1));

	int hei = a_BlockPos.y;
	a_LogBlocks.emplace_back(a_BlockPos.x, hei, a_BlockPos.z, Block::JungleLog::JungleLog());
	PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::OakLeaves::OakLeaves());
	hei++;

	a_OtherBlocks.emplace_back(a_BlockPos.x, hei, a_BlockPos.z, Block::OakLeaves::OakLeaves());
	PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::OakLeaves::OakLeaves());
	hei++;

	a_OtherBlocks.emplace_back(a_BlockPos.x, hei, a_BlockPos.z, Block::OakLeaves::OakLeaves());
}





void GetJungleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks, bool a_Large)
{
	if (!a_Large)
	{
		GetSmallJungleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
	else
	{
		GetLargeJungleTreeImage(a_BlockPos, a_Noise, a_Seq, a_LogBlocks, a_OtherBlocks);
	}
}





void GetLargeJungleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	static const sDirectionCoords VinesTrunk[] =
	{
		{0, -1, 1}, {1, -1, 1},  // North face
		{0,  2, 4}, {1,  2, 4},  // South face
		{-1, 1, 8}, {-1, 0, 8},  // West face
		{2,  1, 2}, {2,  0, 2},  // East face
	};

	int Height = 10 + ((a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(32 * a_Seq, 32 * a_Seq)) / 11) % 20);  // 10 < Height < 29

	a_LogBlocks.reserve(static_cast<size_t>(Height) * 4);
	a_OtherBlocks.reserve(2 * ARRAYCOUNT(BigO5Jungle) + ARRAYCOUNT(BigO4Jungle) + ARRAYCOUNT(BigO3Jungle) + static_cast<size_t>(Height) * 4 + 50);

	// Generates the main trunk
	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i),               Block::JungleLog::JungleLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedX(1),     Block::JungleLog::JungleLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedZ(1),     Block::JungleLog::JungleLog());
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(1, 1), Block::JungleLog::JungleLog());

		// Randomly place vines around the trunk
		for (size_t j = 0; j < ARRAYCOUNT(VinesTrunk); j++)
		{
			if (a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(VinesTrunk[j].x, VinesTrunk[j].z).addedY(i)) % 3 != 0)
			{
				continue;
			}
			a_OtherBlocks.emplace_back(a_BlockPos.addedXZ(VinesTrunk[j].x, VinesTrunk[j].z).addedY(i), DirToVineBlock(VinesTrunk[j].Dir));
		}
	}

	// Prevent floating trees by placing dirt under them
	for (int i = 1; i < 5; i++)
	{
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i),               Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedX(1),     Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedZ(1),     Block::Dirt::Dirt());
		a_OtherBlocks.emplace_back(a_BlockPos.addedY(-i).addedXZ(1, 1), Block::Dirt::Dirt());
	}

	int NumBranches = std::max(
		(a_Noise.IntNoise2DInt(a_BlockPos.x * a_Seq, a_BlockPos.z * a_Seq) / 10) % 4,  // The Original Calculation
		FloorC(Height / 10.0f)                                                         // Just to assure that no massive trees spawn with just one branch
		);
	int BranchStartHeight = 6 + Height % 5;                                            // 6 < BranchStartHeight < 10
	int BranchInterval = (Height - 6 - BranchStartHeight) / NumBranches;
	for (int i = BranchStartHeight; i < (Height - 6); i += BranchInterval)             // Stop 6 blocks before reaching the top
	{
		// Get a direction for the trunk to go to.
		Vector3d BranchStartDirection = pickBranchDirection(a_Noise, a_BlockPos.addedY(i), a_Seq);
		Vector3d BranchDirection = pickBranchDirection(a_Noise, a_BlockPos.addedY(i * a_Seq), a_Seq) / 3;
		int BranchLength = 2 + a_Noise.IntNoise3DInt(a_BlockPos * a_Seq) % 2;
		Vector3i BranchEndPosition = GetTreeBranch(Block::JungleLog::JungleLog(), a_BlockPos.addedY(i), BranchLength, BranchStartDirection, BranchDirection, a_LogBlocks).Floor();

		// There's a chance that there is a third leaf level on a branch
		if ((a_Noise.IntNoise3DInt(a_BlockPos.x * a_Seq, a_BlockPos.y * a_Seq, a_BlockPos.z * a_Seq) % 4) == 0)  // A quarter chance
		{
			PushCoordBlocks(BranchEndPosition.x, BranchEndPosition.y, BranchEndPosition.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), Block::JungleLeaves::JungleLeaves());
			PushCoordBlocks(BranchEndPosition.x, BranchEndPosition.y + 1, BranchEndPosition.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::JungleLeaves::JungleLeaves());
			PushCoordBlocks(BranchEndPosition.x, BranchEndPosition.y + 2, BranchEndPosition.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::JungleLeaves::JungleLeaves());
			a_OtherBlocks.emplace_back(BranchEndPosition.x, BranchEndPosition.y + 2, BranchEndPosition.z, Block::JungleLeaves::JungleLeaves());
		}
		else
		{
			PushCoordBlocks(BranchEndPosition.x, BranchEndPosition.y, BranchEndPosition.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::JungleLeaves::JungleLeaves());
			PushCoordBlocks(BranchEndPosition.x, BranchEndPosition.y + 1, BranchEndPosition.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::JungleLeaves::JungleLeaves());
			a_OtherBlocks.emplace_back(BranchEndPosition.x, BranchEndPosition.y + 1, BranchEndPosition.z, Block::JungleLeaves::JungleLeaves());
		}
	}

	// Place the canopy.
	int CanopyHeight = a_BlockPos.y + Height - 2;
	for (size_t i = 0; i < ARRAYCOUNT(BigOJungleLayers); i++)
	{
		PushCoordBlocks(a_BlockPos.x, CanopyHeight++, a_BlockPos.z, a_OtherBlocks, BigOJungleLayers[i].Coords, BigOJungleLayers[i].Count, Block::JungleLeaves::JungleLeaves());
	}
}





void GetSmallJungleTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	// Vines are around the BigO3, but not in the corners; need proper meta for direction
	static const sDirectionCoords Vines[] =
	{
		{-2, -4, 1}, {-1, -4, 1}, {0, -4, 1}, {1, -4, 1}, {2, -4, 1},  // North face
		{-2,  4, 4}, {-1,  4, 4}, {0,  4, 4}, {1,  4, 4}, {2,  4, 4},  // South face
		{4,  -2, 2}, {4,  -1, 2}, {4,  0, 2}, {4,  1, 2}, {4,  2, 2},  // East face
		{-4, -2, 8}, {-4, -1, 8}, {-4, 0, 8}, {-4, 1, 8},              // West face
		// TODO: proper metas and height: {0,  1, 1},  {0, -1, 4},  {-1, 0, 2}, {1,  1, 8},  // Around the tunk
	} ;

	int Height = 7 + (a_Noise.IntNoise3DInt(a_BlockPos.addedXZ(5 * a_Seq, 5 * a_Seq)) / 5) % 3;

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
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::JungleLog::JungleLog());
	}
	int hei = a_BlockPos.y + Height - 3;

	// Put vines around the lowermost leaves layer:
	PushSomeVines(a_BlockPos.x, hei, a_BlockPos.z, Height, a_Seq, a_Noise, 0x3fffffff, a_OtherBlocks, Vines, ARRAYCOUNT(Vines));

	// The lower two leaves layers are BigO3 with log in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO3, ARRAYCOUNT(BigO3), Block::JungleLeaves::JungleLeaves());
		PushCornerBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 3, Block::JungleLeaves::JungleLeaves());
		hei++;
	}  // for i - 2*

	// Two layers of BigO2 leaves, possibly with corners:
	for (int i = 0; i < 1; i++)
	{
		PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO2, ARRAYCOUNT(BigO2), Block::JungleLeaves::JungleLeaves());
		PushCornerBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_Seq, a_Noise, 0x5fffffff, a_OtherBlocks, 2, Block::JungleLeaves::JungleLeaves());
		hei++;
	}  // for i - 2*

	// Top plus, all leaves:
	PushCoordBlocks(a_BlockPos.x, hei, a_BlockPos.z, a_OtherBlocks, BigO1, ARRAYCOUNT(BigO1), Block::JungleLeaves::JungleLeaves());
	a_OtherBlocks.emplace_back(a_BlockPos.x, hei, a_BlockPos.z, Block::JungleLeaves::JungleLeaves());
}





void GetRedMushroomTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	using namespace Block;
	static constexpr int Height = 4;

	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::MushroomStem::MushroomStem(false, true, true, true, false, true));
		// (const bool Down, const bool East, const bool North, const bool South, const bool Up, const bool West)

		if (i != 0)
		{
			// .                                                                                                    Down   East   North South   Up    West)
			// NORTH SIDE
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(-1, -2), Block::RedMushroomBlock::RedMushroomBlock(false, false, true, false, true, true));  // NORTH_WEST
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(0, -2),  Block::RedMushroomBlock::RedMushroomBlock(false, false, true, false, true, false));  // NORTH
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(1, -2),  Block::RedMushroomBlock::RedMushroomBlock(false, true,  true, false, true, false));  // NORTH_EAST
			// WEST SIDE
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(-2, -1), Block::RedMushroomBlock::RedMushroomBlock(false, false, true,  false, true, true));  // NORTH_WEST
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(-2, 0),  Block::RedMushroomBlock::RedMushroomBlock(false, false, false, false, true, true));  // WEST
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(-2, 1),  Block::RedMushroomBlock::RedMushroomBlock(false, false, false, true,  true, true));  // SOUTH_WEST
			// SOUTH SIDE
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(-1, 2), Block::RedMushroomBlock::RedMushroomBlock(false, false, false, true, true, true));  // SOUTH_WEST
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(0, 2),  Block::RedMushroomBlock::RedMushroomBlock(false, false, false, true, true, false));  // WEST
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(1, 2),  Block::RedMushroomBlock::RedMushroomBlock(false, true,  false, true, true, false));  // SOUTH_EAST
			// EAST SIDE
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(2, -1), Block::RedMushroomBlock::RedMushroomBlock(false, true, true,  false, true, false));  // NORTH_EAST
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(2, 0),  Block::RedMushroomBlock::RedMushroomBlock(false, true, false, false, true, false));  // EAST
			a_LogBlocks.emplace_back(a_BlockPos.addedY(i).addedXZ(2, 1),  Block::RedMushroomBlock::RedMushroomBlock(false, true, false, true,  true, false));  // SOUTH_EAST
		}
	}

	// Top Layer
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-1, -1), Block::RedMushroomBlock::RedMushroomBlock(false, false, true,  false, true, true));  // NORTH_WEST
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedX(-1),      Block::RedMushroomBlock::RedMushroomBlock(false, false, false, false, true, true));  // WEST
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-1, 1),  Block::RedMushroomBlock::RedMushroomBlock(false, false, false, true,  true, true));  // SOUTH_WEST

	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedZ(-1),      Block::RedMushroomBlock::RedMushroomBlock(false, true,  true,  false, true, false));  // NORTH
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height),                 Block::RedMushroomBlock::RedMushroomBlock(false, false, false, false, true, false));  // CENTER
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedZ(1),       Block::RedMushroomBlock::RedMushroomBlock(false, false, false, true,  true, false));  // SOUTH

	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(1, -1),  Block::RedMushroomBlock::RedMushroomBlock(false, true, true,  false, true, false));  // NORTH_EAST
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedX(1),       Block::RedMushroomBlock::RedMushroomBlock(false, true, false, false, true, false));  // EAST
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(1, 1),   Block::RedMushroomBlock::RedMushroomBlock(false, true, false, true,  true, false));  // SOUTH_EAST
}





void GetBrownMushroomTreeImage(Vector3i a_BlockPos, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_LogBlocks, sSetBlockVector & a_OtherBlocks)
{
	static constexpr int Height = 4;
	static constexpr int Radius = 2;
	static constexpr int Border = 3;

	for (int i = 0; i < Height; i++)
	{
		a_LogBlocks.emplace_back(a_BlockPos.addedY(i), Block::MushroomStem::MushroomStem(false, true, true, true, false, true));  // Stem all sides
	}

	for (int x = -Radius; x <= Radius; x++)
	{
		for (int z = -Radius; z <= Radius; z++)
		{
			a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(x, z), Block::BrownMushroomBlock::BrownMushroomBlock(false, false, false, false, true, false));  // Center
		}
	}

	for (int i = 0; i < Border; i++)
	{
		// .                                                                                                                        Down   East   North South   Up    West
		// NORTH SIDE
		a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-1, -3).addedX(i), Block::BrownMushroomBlock::BrownMushroomBlock(false, false, true, false, true, false));  // North
		// WEST SIDE
		a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-3, -1).addedZ(i), Block::BrownMushroomBlock::BrownMushroomBlock(false, false, false, false, true, true));  // West
		// SOUTH SIDE
		a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-1, 3).addedX(i),  Block::BrownMushroomBlock::BrownMushroomBlock(false, false, false, true, true, false));  // South
		// EAST SIDE
		a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(3, -1).addedZ(i),  Block::BrownMushroomBlock::BrownMushroomBlock(false, true, false, false, true, false));  // East
	}

	// Corners

	// .                                                                                                            Down   East   North South   Up    West
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(3, 2), Block::BrownMushroomBlock::BrownMushroomBlock(false, true, false, true,  true, false));  // SouthEast
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(2, 3), Block::BrownMushroomBlock::BrownMushroomBlock(false, true, false, true,  true, false));  // SouthEast

	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-2, 3), Block::BrownMushroomBlock::BrownMushroomBlock(false, false, false, true,  true, true));  // SouthWest
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-3, 2), Block::BrownMushroomBlock::BrownMushroomBlock(false, false, false, true,  true, true));  // SouthWest


	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-2, -3), Block::BrownMushroomBlock::BrownMushroomBlock(false, false, false, false,  true, true));  // NorthWest
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(-3, -2), Block::BrownMushroomBlock::BrownMushroomBlock(false, false, false, false,  true, true));  // NorthWest

	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(2, -3), Block::BrownMushroomBlock::BrownMushroomBlock(false, true, false, false,  true, false));  // NorthEast
	a_LogBlocks.emplace_back(a_BlockPos.addedY(Height).addedXZ(3, -2), Block::BrownMushroomBlock::BrownMushroomBlock(false, true, false, false,  true, false));  // NorthEast
}
