
// Trees.cpp

// Implements helper functions used for generating trees

#include "Globals.h"
#include "Trees.h"
#include "BlockID.h"





#ifndef min
	#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif





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

static const sCoords BigO1[] =
{
	          {0, -1},
	{-1,  0},          {1,  0},
	          {0,  1},
} ;

static const sCoords BigO2[] =
{
	          {-1, -2}, {0, -2}, {1, -2},
	{-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1},
	{-2,  0}, {-1,  0},          {1,  0}, {2,  0},
	{-2,  1}, {-1,  1}, {0,  1}, {1,  1}, {2,  1},
	          {-1,  2}, {0,  2}, {1,  2},
} ;

static const sCoords BigO3[] =
{
	          {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3},
	{-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {3, -2},
	{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1},
	{-3,  0}, {-2,  0}, {-1,  0},          {1,  0}, {2,  0}, {3,  0},
	{-3,  1}, {-2,  1}, {-1,  1}, {0,  1}, {1,  1}, {2,  1}, {3,  1},
	{-3,  2}, {-2,  2}, {-1,  2}, {0,  2}, {1,  2}, {2,  2}, {3,  2},
	          {-2,  3}, {-1,  3}, {0,  3}, {1,  3}, {2,  3},
} ;

typedef struct 
{
	const sCoords * Coords;
	size_t          Count;
} sCoordsArr;

static const sCoordsArr BigOs[] =
{
	{BigO1, ARRAYCOUNT(BigO1)},
	{BigO2, ARRAYCOUNT(BigO2)},
	{BigO3, ARRAYCOUNT(BigO3)},
} ;





/// Pushes a specified layer of blocks of the same type around (x, h, z) into a_Blocks
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
		if (a_Noise.IntNoise3DInt(x + 64 * a_Seq, a_Height + i, z + 64 * a_Seq) <= a_Chance)
		{
			for (int j = 0; j < a_ColumnHeight; j++)
			{
				a_Blocks.push_back(sSetBlock(x, a_Height - j, z, a_BlockType, a_Coords[i].Meta));
			}
		}
	}  // for i - a_Coords[]
}





void GetTreeImageByBiome(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, EMCSBiome a_Biome, sSetBlockVector & a_Blocks)
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
		{
			// Apple or birch trees:
			if (a_Noise.IntNoise3DInt(a_BlockX, a_BlockY + 16 * a_Seq, a_BlockZ + 16 * a_Seq) < 0x5fffffff)
			{
				GetAppleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
			}
			else
			{
				GetBirchTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
			}
			break;
		}
		
		case biTaiga:
		case biIcePlains:
		case biIceMountains:
		case biTaigaHills:
		{
			// Conifers
			GetConiferTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
			break;
		}
		
		case biSwampland:
		{
			// Swamp trees:
			GetSwampTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
			break;
		}
		
		case biJungle:
		case biJungleHills:
		{
			// Apple bushes, jungle trees
			if (a_Noise.IntNoise3DInt(a_BlockX, a_BlockY + 16 * a_Seq, a_BlockZ + 16 * a_Seq) < 0x5fffffff)
			{
				GetAppleBushImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
			}
			else
			{
				GetJungleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
			}
		}
	}
}





void GetAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
{
	if (a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY + 32 * a_Seq, a_BlockZ) < 0x60000000)
	{
		GetSmallAppleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
	}
	else
	{
		GetLargeAppleTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
	}
}





void GetSmallAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
{
	/* Small apple tree has:
	- a top plus (no log)
	- optional BigO1 + random corners (log)
	- 2 layers of BigO2 + random corners (log)
	- 1 to 3 blocks of trunk
	*/
	
	int Random = a_Noise.IntNoise3DInt(a_BlockX + 64 * a_Seq, a_BlockY, a_BlockZ) >> 3;
	
	// Pre-alloc so that we don't realloc too often later:
	a_Blocks.reserve(ARRAYCOUNT(BigO2) * 2 + ARRAYCOUNT(BigO1) + ARRAYCOUNT(Corners) * 3 + 3 + 5);
	
	int Heights[] = {1, 2, 2, 3} ;
	int Height = 1 + Heights[Random & 3];
	Random >>= 2;
	
	// Trunk:
	for (int i = 0; i < Height; i++)
	{
		a_Blocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
	}
	int Hei = a_BlockY + Height;
	
	// 2 BigO2 + corners layers:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks (a_BlockX, Hei, a_BlockZ, a_Blocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		PushCornerBlocks(a_BlockX, Hei, a_BlockZ, a_Seq, a_Noise, 0x5000000 - i * 0x10000000, a_Blocks, 2, E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		a_Blocks.push_back(sSetBlock(a_BlockX, Hei, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
		Hei++;
	}  // for i - 2*
	
	// Optional BigO1 + corners layer:
	if ((Random & 1) == 0)
	{
		PushCoordBlocks (a_BlockX, Hei, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		PushCornerBlocks(a_BlockX, Hei, a_BlockZ, a_Seq, a_Noise, 0x6000000, a_Blocks, 1, E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		a_Blocks.push_back(sSetBlock(a_BlockX, Hei, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
		Hei++;
	}
	
	// Top plus:
	PushCoordBlocks(a_BlockX, Hei, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
	a_Blocks.push_back(sSetBlock(a_BlockX, Hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
}





void GetLargeAppleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
{
	// TODO
}





void GetBirchTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
{
	int Height = 5 + (a_Noise.IntNoise3DInt(a_BlockX + 64 * a_Seq, a_BlockY, a_BlockZ) % 3);
	
	// Prealloc, so that we don't realloc too often later:
	a_Blocks.reserve(Height + 80);
	
	// The entire trunk, out of logs:
	for (int i = Height - 1; i >= 0; --i)
	{
		a_Blocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_BIRCH));
	}
	int h = a_BlockY + Height;
	
	// Top layer - just the Plus:
	PushCoordBlocks(a_BlockX, h, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	a_Blocks.push_back(sSetBlock(a_BlockX, h, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH));  // There's no log at this layer
	h--;
	
	// Second layer - log, Plus and maybe Corners:
	PushCoordBlocks (a_BlockX, h, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	PushCornerBlocks(a_BlockX, h, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_Blocks, 1, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
	h--;
	
	// Third and fourth layers - BigO2 and maybe 2*Corners:
	for (int Row = 0; Row < 2; Row++)
	{
		PushCoordBlocks (a_BlockX, h, a_BlockZ, a_Blocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
		PushCornerBlocks(a_BlockX, h, a_BlockZ, a_Seq, a_Noise, 0x3fffffff + Row * 0x10000000, a_Blocks, 2, E_BLOCK_LEAVES, E_META_LEAVES_BIRCH);
		h--;
	}  // for Row - 2*
}





void GetConiferTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
{
	// Half chance for a spruce, half for a pine:
	if (a_Noise.IntNoise3DInt(a_BlockX + 64 * a_Seq, a_BlockY, a_BlockZ + 32 * a_Seq) < 0x40000000)
	{
		GetSpruceTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
	}
	else
	{
		GetPineTreeImage(a_BlockX, a_BlockY, a_BlockZ, a_Noise, a_Seq, a_Blocks);
	}
}





void GetSpruceTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
{
	// Spruces have a top section with layer sizes of (0, 1, 0) or only (1, 0),
	// then 1 - 3 sections of ascending sizes (1, 2) [most often], (1, 3) or (1, 2, 3)
	// and an optional bottom section of size 1, followed by 1 - 3 clear trunk blocks
	
	// We'll use bits from this number as partial random numbers; but the noise function has mod8 irregularities
	// (each of the mod8 remainders has a very different chance of occurrence) - that's why we divide by 8
	int MyRandom = a_Noise.IntNoise3DInt(a_BlockX + 32 * a_Seq, a_BlockY + 32 * a_Seq, a_BlockZ) / 8;
	
	// Prealloc, so that we don't realloc too often later:
	a_Blocks.reserve(180);
	
	// Clear trunk blocks:
	static const int  sHeights[] = {1, 2, 2, 3};
	int Height = sHeights[MyRandom & 3];
	MyRandom >>= 2;
	for (int i = 0; i < Height; i++)
	{
		a_Blocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
	}
	Height += a_BlockY;
	
	// Optional size-1 bottom leaves layer:
	if ((MyRandom & 1) == 0)
	{
		PushCoordBlocks(a_BlockX, Height, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
		a_Blocks.push_back(sSetBlock(a_BlockX, Height, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
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
				PushCoordBlocks(a_BlockX, Height,     a_BlockZ, a_Blocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				PushCoordBlocks(a_BlockX, Height + 1, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				a_Blocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				a_Blocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				Height += 2;
				break;
			}
			case 2:
			{
				PushCoordBlocks(a_BlockX, Height,     a_BlockZ, a_Blocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				PushCoordBlocks(a_BlockX, Height + 1, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				a_Blocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				a_Blocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				Height += 2;
				break;
			}
			case 3:
			{
				PushCoordBlocks(a_BlockX, Height,     a_BlockZ, a_Blocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				PushCoordBlocks(a_BlockX, Height + 1, a_BlockZ, a_Blocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				PushCoordBlocks(a_BlockX, Height + 2, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
				a_Blocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				a_Blocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				a_Blocks.push_back(sSetBlock(a_BlockX, Height + 2, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
				Height += 3;
				break;
			}
		}  // switch (SectionType)
		MyRandom >>= 2;
	}  // for i - Sections
	
	if ((MyRandom & 1) == 0)
	{
		// (0, 1, 0) top:
		a_Blocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
		PushCoordBlocks             (a_BlockX, Height + 1, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
		a_Blocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
		a_Blocks.push_back(sSetBlock(a_BlockX, Height + 2, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
	}
	else
	{
		// (1, 0) top:
		a_Blocks.push_back(sSetBlock(a_BlockX, Height,     a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
		PushCoordBlocks             (a_BlockX, Height + 1, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
		a_Blocks.push_back(sSetBlock(a_BlockX, Height + 1, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
	}
}





void GetPineTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
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
	a_Blocks.reserve(TrunkHeight + NumLeavesLayers * 25);

	// The entire trunk, out of logs:
	for (int i = TrunkHeight; i >= 0; --i)
	{
		a_Blocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_CONIFER));
	}
	int h = a_BlockY + TrunkHeight + 2;

	// Top layer - just a single leaves block:	
	a_Blocks.push_back(sSetBlock(a_BlockX, h, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));
	h--;

	// One more layer is above the trunk, push the central leaves:	
	a_Blocks.push_back(sSetBlock(a_BlockX, h, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER));

	// Layers expanding in size, then collapsing again:
	// LOGD("Generating %d layers of pine leaves, SameSizeMax = %d", NumLeavesLayers, SameSizeMax);
	for (int i = 0; i < NumLeavesLayers; ++i)
	{
		int LayerSize = min(i, NumLeavesLayers - i + SameSizeMax - 1);
		// LOGD("LayerSize %d: %d", i, LayerSize);
		if (LayerSize < 0)
		{
			break;
		}
		ASSERT(LayerSize < ARRAYCOUNT(BigOs));
		PushCoordBlocks(a_BlockX, h, a_BlockZ, a_Blocks, BigOs[LayerSize].Coords, BigOs[LayerSize].Count, E_BLOCK_LEAVES, E_META_LEAVES_CONIFER);
		h--;
	}
}





void GetSwampTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
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
	
	a_Blocks.reserve(2 * ARRAYCOUNT(BigO2) + 2 * ARRAYCOUNT(BigO3) + Height * ARRAYCOUNT(Vines) + 20);
	
	for (int i = 0; i < Height; i++)
	{
		a_Blocks.push_back(sSetBlock(a_BlockX, a_BlockY + i, a_BlockZ, E_BLOCK_LOG, E_META_LOG_APPLE));
	}
	int hei = a_BlockY + Height - 2;
	
	// Put vines around the lowermost leaves layer:
	PushSomeColumns(a_BlockX, hei, a_BlockZ, Height, a_Seq, a_Noise, 0x3fffffff, a_Blocks, Vines, ARRAYCOUNT(Vines), E_BLOCK_VINES);
	
	// The lower two leaves layers are BigO3 with log in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_Blocks, BigO3, ARRAYCOUNT(BigO3), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		PushCornerBlocks(a_BlockX, hei, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_Blocks, 3, E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		hei++;
	}  // for i - 2*

	// The upper two leaves layers are BigO2 with leaves in the middle and possibly corners:
	for (int i = 0; i < 2; i++)
	{
		PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_Blocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		PushCornerBlocks(a_BlockX, hei, a_BlockZ, a_Seq, a_Noise, 0x5fffffff, a_Blocks, 3, E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
		a_Blocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
		hei++;
	}  // for i - 2*
}





void GetAppleBushImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
{
	a_Blocks.reserve(3 + ARRAYCOUNT(BigO2) + ARRAYCOUNT(BigO1));
	
	int hei = a_BlockY;
	a_Blocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LOG, E_META_LOG_JUNGLE));
	PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_Blocks, BigO2, ARRAYCOUNT(BigO2), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
	hei++;
	
	a_Blocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
	PushCoordBlocks(a_BlockX, hei, a_BlockZ, a_Blocks, BigO1, ARRAYCOUNT(BigO1), E_BLOCK_LEAVES, E_META_LEAVES_APPLE);
	hei++;
	
	a_Blocks.push_back(sSetBlock(a_BlockX, hei, a_BlockZ, E_BLOCK_LEAVES, E_META_LEAVES_APPLE));
}





void GetJungleTreeImage(int a_BlockX, int a_BlockY, int a_BlockZ, cNoise & a_Noise, int a_Seq, sSetBlockVector & a_Blocks)
{
	// TODO
}




