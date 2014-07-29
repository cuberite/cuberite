
// StructGen.h

#include "Globals.h"
#include "StructGen.h"
#include "../BlockID.h"
#include "Trees.h"
#include "../BlockArea.h"
#include "../LinearUpscale.h"





////////////////////////////////////////////////////////////////////////////////
// cStructGenOreNests configuration:

const int MAX_HEIGHT_COAL = 127;
const int NUM_NESTS_COAL = 50;
const int NEST_SIZE_COAL = 10;

const int MAX_HEIGHT_IRON = 64;
const int NUM_NESTS_IRON = 14;
const int NEST_SIZE_IRON = 6;

const int MAX_HEIGHT_REDSTONE = 16;
const int NUM_NESTS_REDSTONE = 4;
const int NEST_SIZE_REDSTONE = 6;

const int MAX_HEIGHT_GOLD = 32;
const int NUM_NESTS_GOLD = 2;
const int NEST_SIZE_GOLD = 6;

const int MAX_HEIGHT_DIAMOND = 15;
const int NUM_NESTS_DIAMOND = 1;
const int NEST_SIZE_DIAMOND = 4;

const int MAX_HEIGHT_LAPIS = 30;
const int NUM_NESTS_LAPIS = 2;
const int NEST_SIZE_LAPIS = 5;

const int MAX_HEIGHT_DIRT = 127;
const int NUM_NESTS_DIRT = 20;
const int NEST_SIZE_DIRT = 32;

const int MAX_HEIGHT_GRAVEL = 70;
const int NUM_NESTS_GRAVEL = 15;
const int NEST_SIZE_GRAVEL = 32;





////////////////////////////////////////////////////////////////////////////////
// cStructGenTrees:

void cStructGenTrees::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	
	cChunkDesc WorkerDesc(ChunkX, ChunkZ);
	
	// Generate trees:
	for (int x = 0; x <= 2; x++)
	{
		int BaseX = ChunkX + x - 1;
		for (int z = 0; z <= 2; z++)
		{
			int BaseZ = ChunkZ + z - 1;
			
			cChunkDesc * Dest;

			if ((x != 1) || (z != 1))
			{
				Dest = &WorkerDesc;
				WorkerDesc.SetChunkCoords(BaseX, BaseZ);
				
				m_BiomeGen->GenBiomes           (BaseX, BaseZ, WorkerDesc.GetBiomeMap());
				m_HeightGen->GenHeightMap       (BaseX, BaseZ, WorkerDesc.GetHeightMap());
				m_CompositionGen->ComposeTerrain(WorkerDesc);
				// TODO: Free the entity lists
			}
			else
			{
				Dest = &a_ChunkDesc;
			}

			int NumTrees = GetNumTrees(BaseX, BaseZ, Dest->GetBiomeMap());

			sSetBlockVector OutsideLogs, OutsideOther;
			for (int i = 0; i < NumTrees; i++)
			{
				GenerateSingleTree(BaseX, BaseZ, i, *Dest, OutsideLogs, OutsideOther);
			}

			sSetBlockVector IgnoredOverflow;
			IgnoredOverflow.reserve(OutsideOther.size());
			ApplyTreeImage(ChunkX, ChunkZ, a_ChunkDesc, OutsideOther, IgnoredOverflow);
			IgnoredOverflow.clear();
			IgnoredOverflow.reserve(OutsideLogs.size());
			ApplyTreeImage(ChunkX, ChunkZ, a_ChunkDesc, OutsideLogs, IgnoredOverflow);
		}  // for z
	}  // for x
	
	// Update the heightmap:
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = cChunkDef::Height - 1; y >= 0; y--)
			{
				if (a_ChunkDesc.GetBlockType(x, y, z) != E_BLOCK_AIR)
				{
					a_ChunkDesc.SetHeight(x, z, y);
					break;
				}
			}  // for y
		}  // for z
	}  // for x
}





void cStructGenTrees::GenerateSingleTree(
	int a_ChunkX, int a_ChunkZ, int a_Seq,
	cChunkDesc & a_ChunkDesc,
	sSetBlockVector & a_OutsideLogs,
	sSetBlockVector & a_OutsideOther
)
{
	int x = (m_Noise.IntNoise3DInt(a_ChunkX + a_ChunkZ, a_ChunkZ, a_Seq) / 19) % cChunkDef::Width;
	int z = (m_Noise.IntNoise3DInt(a_ChunkX - a_ChunkZ, a_Seq, a_ChunkZ) / 19) % cChunkDef::Width;
	
	int Height = a_ChunkDesc.GetHeight(x, z);
	
	if ((Height <= 0) || (Height > 240))
	{
		return;
	}

	// Check the block underneath the tree:
	BLOCKTYPE TopBlock = a_ChunkDesc.GetBlockType(x, Height, z);
	if ((TopBlock != E_BLOCK_DIRT) && (TopBlock != E_BLOCK_GRASS) && (TopBlock != E_BLOCK_FARMLAND))
	{
		return;
	}
	
	sSetBlockVector TreeLogs, TreeOther;
	GetTreeImageByBiome(
		a_ChunkX * cChunkDef::Width + x, Height + 1, a_ChunkZ * cChunkDef::Width + z,
		m_Noise, a_Seq,
		a_ChunkDesc.GetBiome(x, z),
		TreeLogs, TreeOther
	);

	// Check if the generated image fits the terrain. Only the logs are checked:
	for (sSetBlockVector::const_iterator itr = TreeLogs.begin(); itr != TreeLogs.end(); ++itr)
	{
		if ((itr->ChunkX != a_ChunkX) || (itr->ChunkZ != a_ChunkZ))
		{
			// Outside the chunk
			continue;
		}

		BLOCKTYPE Block = a_ChunkDesc.GetBlockType(itr->x, itr->y, itr->z);
		switch (Block)
		{
			CASE_TREE_ALLOWED_BLOCKS:
			{
				break;
			}
			default:
			{
				// There's something in the way, abort this tree altogether
				return;
			}
		}
	}
	
	ApplyTreeImage(a_ChunkX, a_ChunkZ, a_ChunkDesc, TreeOther, a_OutsideOther);
	ApplyTreeImage(a_ChunkX, a_ChunkZ, a_ChunkDesc, TreeLogs,  a_OutsideLogs);
}





void cStructGenTrees::ApplyTreeImage(
	int a_ChunkX, int a_ChunkZ,
	cChunkDesc & a_ChunkDesc,
	const sSetBlockVector & a_Image,
	sSetBlockVector & a_Overflow
)
{
	// Put the generated image into a_BlockTypes, push things outside this chunk into a_Blocks
	for (sSetBlockVector::const_iterator itr = a_Image.begin(), end = a_Image.end(); itr != end; ++itr)
	{
		if ((itr->ChunkX == a_ChunkX) && (itr->ChunkZ == a_ChunkZ))
		{
			// Inside this chunk, integrate into a_ChunkDesc:
			switch (a_ChunkDesc.GetBlockType(itr->x, itr->y, itr->z))
			{
				case E_BLOCK_LEAVES:
				{
					if (itr->BlockType != E_BLOCK_LOG)
					{
						break;
					}
					// fallthrough:
				}
				CASE_TREE_OVERWRITTEN_BLOCKS:
				{
					a_ChunkDesc.SetBlockTypeMeta(itr->x, itr->y, itr->z, itr->BlockType, itr->BlockMeta);
					break;
				}
				
			}  // switch (GetBlock())
			continue;
		}
		
		// Outside the chunk, push into a_Overflow.
		// Don't check if already present there, by separating logs and others we don't need the checks anymore:
		a_Overflow.push_back(*itr);
	}
}





int cStructGenTrees::GetNumTrees(
	int a_ChunkX, int a_ChunkZ,
	const cChunkDef::BiomeMap & a_Biomes
)
{
	int NumTrees = 0;
	for (int x = 0; x < cChunkDef::Width; x++) for (int z = 0; z < cChunkDef::Width; z++)
	{
		int Add = 0;
		switch (cChunkDef::GetBiome(a_Biomes, x, z))
		{
			case biOcean:                Add =   2; break;
			case biDesert:               Add =   0; break;
			case biPlains:               Add =   1; break;
			case biExtremeHills:         Add =   3; break;
			case biForest:               Add =  30; break;
			case biTaiga:                Add =  30; break;
			case biSwampland:            Add =   8; break;
			case biIcePlains:            Add =   1; break;
			case biIceMountains:         Add =   1; break;
			case biMushroomIsland:       Add =   3; break;
			case biMushroomShore:        Add =   3; break;
			case biForestHills:          Add =  20; break;
			case biTaigaHills:           Add =  20; break;
			case biExtremeHillsEdge:     Add =   5; break;
			case biJungle:               Add = 120; break;
			case biJungleHills:          Add =  90; break;
			case biJungleEdge:           Add =  90; break;
			case biBirchForest:          Add =  30; break;
			case biBirchForestHills:     Add =  20; break;
			case biRoofedForest:         Add =  50; break;
			case biColdTaiga:            Add =  20; break;
			case biColdTaigaHills:       Add =  15; break;
			case biMegaTaiga:            Add =  30; break;
			case biMegaTaigaHills:       Add =  25; break;
			case biExtremeHillsPlus:     Add =   3; break;
			case biSavanna:              Add =   8; break;
			case biSavannaPlateau:       Add =  12; break;
			case biMesa:                 Add =   2; break;
			case biMesaPlateauF:         Add =   8; break;
			case biMesaPlateau:          Add =   8; break;
			case biSunflowerPlains:      Add =   1; break;
			case biDesertM:              Add =   0; break;
			case biExtremeHillsM:        Add =   4; break;
			case biFlowerForest:         Add =  30; break;
			case biTaigaM:               Add =  30; break;
			case biSwamplandM:           Add =   8; break;
			case biIcePlainsSpikes:      Add =   1; break;
			case biJungleM:              Add = 120; break;
			case biJungleEdgeM:          Add =  90; break;
			case biBirchForestM:         Add =  30; break;
			case biBirchForestHillsM:    Add =  20; break;
			case biRoofedForestM:        Add =  40; break;
			case biColdTaigaM:           Add =  30; break;
			case biMegaSpruceTaiga:      Add =  30; break;
			case biMegaSpruceTaigaHills: Add =  30; break;
			case biExtremeHillsPlusM:    Add =   4; break;
			case biSavannaM:             Add =   8; break;
			case biSavannaPlateauM:      Add =  12; break;
			case biMesaBryce:            Add =   4; break;
			case biMesaPlateauFM:        Add =  12; break;
			case biMesaPlateauM:         Add =  12; break;
		}
		NumTrees += Add;
	}
	return NumTrees / 1024;
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenOreNests:

void cStructGenOreNests::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	cChunkDef::BlockTypes & BlockTypes = a_ChunkDesc.GetBlockTypes();
	GenerateOre(ChunkX, ChunkZ, E_BLOCK_COAL_ORE,     MAX_HEIGHT_COAL,     NUM_NESTS_COAL,     NEST_SIZE_COAL,     BlockTypes, 1);
	GenerateOre(ChunkX, ChunkZ, E_BLOCK_IRON_ORE,     MAX_HEIGHT_IRON,     NUM_NESTS_IRON,     NEST_SIZE_IRON,     BlockTypes, 2);
	GenerateOre(ChunkX, ChunkZ, E_BLOCK_REDSTONE_ORE, MAX_HEIGHT_REDSTONE, NUM_NESTS_REDSTONE, NEST_SIZE_REDSTONE, BlockTypes, 3);
	GenerateOre(ChunkX, ChunkZ, E_BLOCK_GOLD_ORE,     MAX_HEIGHT_GOLD,     NUM_NESTS_GOLD,     NEST_SIZE_GOLD,     BlockTypes, 4);
	GenerateOre(ChunkX, ChunkZ, E_BLOCK_DIAMOND_ORE,  MAX_HEIGHT_DIAMOND,  NUM_NESTS_DIAMOND,  NEST_SIZE_DIAMOND,  BlockTypes, 5);
	GenerateOre(ChunkX, ChunkZ, E_BLOCK_LAPIS_ORE,    MAX_HEIGHT_LAPIS,    NUM_NESTS_LAPIS,    NEST_SIZE_LAPIS,    BlockTypes, 6);
	GenerateOre(ChunkX, ChunkZ, E_BLOCK_DIRT,         MAX_HEIGHT_DIRT,     NUM_NESTS_DIRT,     NEST_SIZE_DIRT,     BlockTypes, 10);
	GenerateOre(ChunkX, ChunkZ, E_BLOCK_GRAVEL,       MAX_HEIGHT_GRAVEL,   NUM_NESTS_GRAVEL,   NEST_SIZE_GRAVEL,   BlockTypes, 11);
}





void cStructGenOreNests::GenerateOre(int a_ChunkX, int a_ChunkZ, BLOCKTYPE a_OreType, int a_MaxHeight, int a_NumNests, int a_NestSize, cChunkDef::BlockTypes & a_BlockTypes, int a_Seq)
{
	// This function generates several "nests" of ore, each nest consisting of number of ore blocks relatively adjacent to each other.
	// It does so by making a random XYZ walk and adding ore along the way in cuboids of different (random) sizes
	// Only stone gets replaced with ore, all other blocks stay (so the nest can actually be smaller than specified).
	
	for (int i = 0; i < a_NumNests; i++)
	{
		int rnd = m_Noise.IntNoise3DInt(a_ChunkX + i, a_Seq, a_ChunkZ + 64 * i) / 8;
		int BaseX = rnd % cChunkDef::Width;
		rnd /= cChunkDef::Width;
		int BaseZ = rnd % cChunkDef::Width;
		rnd /= cChunkDef::Width;
		int BaseY = rnd % a_MaxHeight;
		rnd /= a_MaxHeight;
		int NestSize = a_NestSize + (rnd % (a_NestSize / 4));  // The actual nest size may be up to 1/4 larger
		int Num = 0;
		while (Num < NestSize)
		{
			// Put a cuboid around [BaseX, BaseY, BaseZ]
			int rnd = m_Noise.IntNoise3DInt(a_ChunkX + 64 * i, 2 * a_Seq + Num, a_ChunkZ + 32 * i) / 8;
			int xsize = rnd % 2;
			int ysize = (rnd / 4) % 2;
			int zsize = (rnd / 16) % 2;
			rnd >>= 8;
			for (int x = xsize; x >= 0; --x)
			{
				int BlockX = BaseX + x;
				if ((BlockX < 0) || (BlockX >= cChunkDef::Width))
				{
					Num++;  // So that the cycle finishes even if the base coords wander away from the chunk
					continue;
				}
				for (int y = ysize; y >= 0; --y)
				{
					int BlockY = BaseY + y;
					if ((BlockY < 0) || (BlockY >= cChunkDef::Height))
					{
						Num++;  // So that the cycle finishes even if the base coords wander away from the chunk
						continue;
					}
					for (int z = zsize; z >= 0; --z)
					{
						int BlockZ = BaseZ + z;
						if ((BlockZ < 0) || (BlockZ >= cChunkDef::Width))
						{
							Num++;  // So that the cycle finishes even if the base coords wander away from the chunk
							continue;
						}

						int Index = cChunkDef::MakeIndexNoCheck(BlockX, BlockY, BlockZ);
						if (a_BlockTypes[Index] == E_BLOCK_STONE)
						{
							a_BlockTypes[Index] = a_OreType;
						}
						Num++;
					}  // for z
				}  // for y
			}  // for x
			
			// Move the base to a neighbor voxel
			switch (rnd % 4)
			{
				case 0: BaseX--; break;
				case 1: BaseX++; break;
			}
			switch ((rnd >> 3) % 4)
			{
				case 0: BaseY--; break;
				case 1: BaseY++; break;
			}
			switch ((rnd >> 6) % 4)
			{
				case 0: BaseZ--; break;
				case 1: BaseZ++; break;
			}
		}  // while (Num < NumBlocks)
	}  // for i - NumNests
}






////////////////////////////////////////////////////////////////////////////////
// cStructGenLakes:

void cStructGenLakes::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	
	for (int z = -1; z < 2; z++) for (int x = -1; x < 2; x++)
	{
		if (((m_Noise.IntNoise2DInt(ChunkX + x, ChunkZ + z) / 17) % 100) > m_Probability)
		{
			continue;
		}
		
		cBlockArea Lake;
		CreateLakeImage(ChunkX + x, ChunkZ + z, Lake);
		
		int OfsX = Lake.GetOriginX() + x * cChunkDef::Width;
		int OfsZ = Lake.GetOriginZ() + z * cChunkDef::Width;
		
		// Merge the lake into the current data
		a_ChunkDesc.WriteBlockArea(Lake, OfsX, Lake.GetOriginY(), OfsZ, cBlockArea::msLake);
	}  // for x, z - neighbor chunks
}





void cStructGenLakes::CreateLakeImage(int a_ChunkX, int a_ChunkZ, cBlockArea & a_Lake)
{
	a_Lake.Create(16, 8, 16);
	a_Lake.Fill(cBlockArea::baTypes, E_BLOCK_SPONGE);  // Sponge is the NOP blocktype for lake merging strategy
	
	// Find the minimum height in this chunk:
	cChunkDef::HeightMap HeightMap;
	m_HeiGen.GenHeightMap(a_ChunkX, a_ChunkZ, HeightMap);
	HEIGHTTYPE MinHeight = HeightMap[0];
	for (size_t i = 1; i < ARRAYCOUNT(HeightMap); i++)
	{
		if (HeightMap[i] < MinHeight)
		{
			MinHeight = HeightMap[i];
		}
	}
	
	// Make a random position in the chunk by using a random 16 block XZ offset and random height up to chunk's max height minus 6
	MinHeight = std::max(MinHeight - 6, 2);
	int Rnd = m_Noise.IntNoise3DInt(a_ChunkX, 128, a_ChunkZ) / 11;
	// Random offset [-8 .. 8], with higher probability around 0; add up four three-bit-wide randoms [0 .. 28], divide and subtract to get range
	int OffsetX = 4 * ((Rnd & 0x07) + ((Rnd & 0x38) >> 3) + ((Rnd & 0x1c0) >> 6) + ((Rnd & 0xe00) >> 9)) / 7 - 8;
	Rnd >>= 12;
	// Random offset [-8 .. 8], with higher probability around 0; add up four three-bit-wide randoms [0 .. 28], divide and subtract to get range
	int OffsetZ = 4 * ((Rnd & 0x07) + ((Rnd & 0x38) >> 3) + ((Rnd & 0x1c0) >> 6) + ((Rnd & 0xe00) >> 9)) / 7 - 8;
	Rnd = m_Noise.IntNoise3DInt(a_ChunkX, 512, a_ChunkZ) / 13;
	// Random height [1 .. MinHeight] with preference to center heights
	int HeightY = 1 + (((Rnd & 0x1ff) % MinHeight) + (((Rnd >> 9) & 0x1ff) % MinHeight)) / 2;
	
	a_Lake.SetOrigin(OffsetX, HeightY, OffsetZ);
	
	// Hollow out a few bubbles inside the blockarea:
	int NumBubbles = 4 + ((Rnd >> 18) & 0x03);  // 4 .. 7 bubbles
	BLOCKTYPE * BlockTypes = a_Lake.GetBlockTypes();
	for (int i = 0; i < NumBubbles; i++)
	{
		int Rnd = m_Noise.IntNoise3DInt(a_ChunkX, i, a_ChunkZ) / 13;
		const int BubbleR = 2 + (Rnd & 0x03);  // 2 .. 5
		const int Range = 16 - 2 * BubbleR;
		const int BubbleX = BubbleR + (Rnd % Range);
		Rnd >>= 4;
		const int BubbleY = 4 + (Rnd & 0x01);  // 4 .. 5
		Rnd >>= 1;
		const int BubbleZ = BubbleR + (Rnd % Range);
		const int HalfR = BubbleR / 2;  // 1 .. 2
		const int RSquared = BubbleR * BubbleR;
		for (int y = -HalfR; y <= HalfR; y++)
		{
			// BubbleY + y is in the [0, 7] bounds
			int DistY = 4 * y * y / 3;
			int IdxY = (BubbleY + y) * 16 * 16;
			for (int z = -BubbleR; z <= BubbleR; z++)
			{
				int DistYZ = DistY + z * z;
				if (DistYZ >= RSquared)
				{
					continue;
				}
				int IdxYZ = BubbleX + IdxY + (BubbleZ + z) * 16;
				for (int x = -BubbleR; x <= BubbleR; x++)
				{
					if (x * x + DistYZ < RSquared)
					{
						BlockTypes[x + IdxYZ] = E_BLOCK_AIR;
					}
				}  // for x
			}  // for z
		}  // for y
	}  // for i - bubbles

	// Turn air in the bottom half into liquid:
	for (int y = 0; y < 4; y++)
	{
		for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
		{
			if (BlockTypes[x + z * 16 + y * 16 * 16] == E_BLOCK_AIR)
			{
				BlockTypes[x + z * 16 + y * 16 * 16] = m_Fluid;
			}
		}  // for z, x
	}  // for y
	
	// TODO: Turn sponge next to lava into stone
	
	// a_Lake.SaveToSchematicFile(Printf("Lake_%d_%d.schematic", a_ChunkX, a_ChunkZ));
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenDirectOverhangs:

cStructGenDirectOverhangs::cStructGenDirectOverhangs(int a_Seed) :
	m_Noise1(a_Seed),
	m_Noise2(a_Seed + 1000)
{
}





void cStructGenDirectOverhangs::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// If there is no column of the wanted biome, bail out:
	if (!HasWantedBiome(a_ChunkDesc))
	{
		return;
	}
	
	HEIGHTTYPE MaxHeight = a_ChunkDesc.GetMaxHeight();

	const int SEGMENT_HEIGHT = 8;
	const int INTERPOL_X = 16;  // Must be a divisor of 16
	const int INTERPOL_Z = 16;  // Must be a divisor of 16
	// Interpolate the chunk in 16 * SEGMENT_HEIGHT * 16 "segments", each SEGMENT_HEIGHT blocks high and each linearly interpolated separately.
	// Have two buffers, one for the lowest floor and one for the highest floor, so that Y-interpolation can be done between them
	// Then swap the buffers and use the previously-top one as the current-bottom, without recalculating it.
	
	int FloorBuf1[17 * 17];
	int FloorBuf2[17 * 17];
	int * FloorHi = FloorBuf1;
	int * FloorLo = FloorBuf2;
	int BaseX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BaseZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	int BaseY = 63;
	
	// Interpolate the lowest floor:
	for (int z = 0; z <= 16 / INTERPOL_Z; z++) for (int x = 0; x <= 16 / INTERPOL_X; x++)
	{
		FloorLo[INTERPOL_X * x + 17 * INTERPOL_Z * z] =
			m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, BaseY, BaseZ + INTERPOL_Z * z) *
			m_Noise2.IntNoise3DInt(BaseX + INTERPOL_X * x, BaseY, BaseZ + INTERPOL_Z * z) /
			256;
	}  // for x, z - FloorLo[]
	LinearUpscale2DArrayInPlace<17, 17, INTERPOL_X, INTERPOL_Z>(FloorLo);
	
	// Interpolate segments:
	for (int Segment = BaseY; Segment < MaxHeight; Segment += SEGMENT_HEIGHT)
	{
		// First update the high floor:
		for (int z = 0; z <= 16 / INTERPOL_Z; z++) for (int x = 0; x <= 16 / INTERPOL_X; x++)
		{
			FloorHi[INTERPOL_X * x + 17 * INTERPOL_Z * z] =
				m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) *
				m_Noise2.IntNoise3DInt(BaseX + INTERPOL_Z * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) /
				256;
		}  // for x, z - FloorLo[]
		LinearUpscale2DArrayInPlace<17, 17, INTERPOL_X, INTERPOL_Z>(FloorHi);
		
		// Interpolate between FloorLo and FloorHi:
		for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
		{
			EMCSBiome biome = a_ChunkDesc.GetBiome(x, z);
			
			if ((biome == biExtremeHills) || (biome == biExtremeHillsEdge))
			{
				int Lo = FloorLo[x + 17 * z] / 256;
				int Hi = FloorHi[x + 17 * z] / 256;
				for (int y = 0; y < SEGMENT_HEIGHT; y++)
				{
					int Val = Lo + (Hi - Lo) * y / SEGMENT_HEIGHT;
					if (Val < 0)
					{
						a_ChunkDesc.SetBlockType(x, y + Segment, z, E_BLOCK_AIR);
					}
				}  // for y
				break;
			}  // if (biome)
		}  // for z, x
		
		// Swap the floors:
		std::swap(FloorLo, FloorHi);
	}
}





bool cStructGenDirectOverhangs::HasWantedBiome(cChunkDesc & a_ChunkDesc) const
{
	cChunkDef::BiomeMap & Biomes = a_ChunkDesc.GetBiomeMap();
	for (size_t i = 0; i < ARRAYCOUNT(Biomes); i++)
	{
		switch (Biomes[i])
		{
			case biExtremeHills:
			case biExtremeHillsEdge:
			{
				return true;
			}
			default:
			{
				break;
			}
		}
	}  // for i
	return false;
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenDistortedMembraneOverhangs:

cStructGenDistortedMembraneOverhangs::cStructGenDistortedMembraneOverhangs(int a_Seed) :
	m_NoiseX(a_Seed + 1000),
	m_NoiseY(a_Seed + 2000),
	m_NoiseZ(a_Seed + 3000),
	m_NoiseH(a_Seed + 4000)
{
}





void cStructGenDistortedMembraneOverhangs::GenFinish(cChunkDesc & a_ChunkDesc)
{
	const NOISE_DATATYPE Frequency = (NOISE_DATATYPE)16;
	const NOISE_DATATYPE Amount = (NOISE_DATATYPE)1;
	for (int y = 50; y < 128; y++)
	{
		NOISE_DATATYPE NoiseY = (NOISE_DATATYPE)y / 32;
		// TODO: proper water level - where to get?
		BLOCKTYPE ReplacementBlock = (y > 62) ? E_BLOCK_AIR : E_BLOCK_STATIONARY_WATER;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			NOISE_DATATYPE NoiseZ = ((NOISE_DATATYPE)(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z)) / Frequency;
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(a_ChunkDesc.GetChunkX() * cChunkDef::Width + x)) / Frequency;
				NOISE_DATATYPE DistortX = m_NoiseX.CubicNoise3D(NoiseX, NoiseY, NoiseZ) * Amount;
				NOISE_DATATYPE DistortY = m_NoiseY.CubicNoise3D(NoiseX, NoiseY, NoiseZ) * Amount;
				NOISE_DATATYPE DistortZ = m_NoiseZ.CubicNoise3D(NoiseX, NoiseY, NoiseZ) * Amount;
				int MembraneHeight = 96 - (int)((DistortY + m_NoiseH.CubicNoise2D(NoiseX + DistortX, NoiseZ + DistortZ)) * 30);
				if (MembraneHeight < y)
				{
					a_ChunkDesc.SetBlockType(x, y, z, ReplacementBlock);
				}
			}  // for y
		}  // for x
	}  // for z
}




