
// StructGen.h

#include "Globals.h"
#include "StructGen.h"
#include "../BlockID.h"
#include "Trees.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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





template <typename T> T Clamp(T a_Value, T a_Min, T a_Max)
{
	return (a_Value < a_Min) ? a_Min : ((a_Value > a_Max) ? a_Max : a_Value);
}





static bool SortTreeBlocks(const sSetBlock & a_First, const sSetBlock & a_Second)
{
	return (a_First.BlockType == E_BLOCK_LOG) && (a_Second.BlockType != E_BLOCK_LOG);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenTrees:

void cStructGenTrees::GenStructures(cChunkDesc & a_ChunkDesc)
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
			
			cChunkDef::BlockTypes *   BlT;
			cChunkDef::BlockNibbles * BlM;
			cChunkDef::HeightMap *    Hei;
			cChunkDef::BiomeMap *     Bio;


			if ((x != 1) || (z != 1))
			{
				BlT = &(WorkerDesc.GetBlockTypes());
				BlM = &(WorkerDesc.GetBlockMetas());
				Hei = &(WorkerDesc.GetHeightMap());
				Bio = &(WorkerDesc.GetBiomeMap());
				WorkerDesc.SetChunkCoords(BaseX, BaseZ);
				
				m_BiomeGen->GenBiomes           (BaseX, BaseZ, WorkerDesc.GetBiomeMap());
				m_HeightGen->GenHeightMap       (BaseX, BaseZ, WorkerDesc.GetHeightMap());
				m_CompositionGen->ComposeTerrain(WorkerDesc);
				// TODO: Free the entity lists
			}
			else
			{
				BlT = &(a_ChunkDesc.GetBlockTypes());
				BlM = &(a_ChunkDesc.GetBlockMetas());
				Hei = &(a_ChunkDesc.GetHeightMap());
				Bio = &(a_ChunkDesc.GetBiomeMap());
			}

			int NumTrees = GetNumTrees(BaseX, BaseZ, *Bio);

			sSetBlockVector OutsideLogs, OutsideOther;
			for (int i = 0; i < NumTrees; i++)
			{
				GenerateSingleTree(BaseX, BaseZ, i, *BlT, *BlM, *Hei, *Bio, OutsideLogs, OutsideOther);
			}

			sSetBlockVector IgnoredOverflow;
			IgnoredOverflow.reserve(OutsideOther.size());
			ApplyTreeImage(ChunkX, ChunkZ, a_ChunkDesc.GetBlockTypes(), a_ChunkDesc.GetBlockMetas(), OutsideOther, IgnoredOverflow);
			IgnoredOverflow.clear();
			IgnoredOverflow.reserve(OutsideLogs.size());
			ApplyTreeImage(ChunkX, ChunkZ, a_ChunkDesc.GetBlockTypes(), a_ChunkDesc.GetBlockMetas(), OutsideLogs, IgnoredOverflow);
		}  // for z
	}  // for x
	
	// Update the heightmap:
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = cChunkDef::Height - 1; y >= 0; y--)
			{
				if (cChunkDef::GetBlock(a_ChunkDesc.GetBlockTypes(), x, y, z) != E_BLOCK_AIR)
				{
					cChunkDef::SetHeight(a_ChunkDesc.GetHeightMap(), x, z, y);
					break;
				}
			}  // for y
		}  // for z
	}  // for x
}





void cStructGenTrees::GenerateSingleTree(
	int a_ChunkX, int a_ChunkZ, int a_Seq,
	cChunkDef::BlockTypes & a_BlockTypes,
	cChunkDef::BlockNibbles & a_BlockMetas,
	const cChunkDef::HeightMap & a_Height,
	const cChunkDef::BiomeMap & a_Biomes,
	sSetBlockVector & a_OutsideLogs,
	sSetBlockVector & a_OutsideOther
)
{
	int x = (m_Noise.IntNoise3DInt(a_ChunkX + a_ChunkZ, a_ChunkZ, a_Seq) / 19) % cChunkDef::Width;
	int z = (m_Noise.IntNoise3DInt(a_ChunkX - a_ChunkZ, a_Seq, a_ChunkZ) / 19) % cChunkDef::Width;
	
	int Height = cChunkDef::GetHeight(a_Height, x, z);
	
	if ((Height <= 0) || (Height > 240))
	{
		return;
	}

	// Check the block underneath the tree:
	BLOCKTYPE TopBlock = cChunkDef::GetBlock(a_BlockTypes, x, Height, z);
	if ((TopBlock != E_BLOCK_DIRT) && (TopBlock != E_BLOCK_GRASS) && (TopBlock != E_BLOCK_FARMLAND))
	{
		return;
	}
	
	sSetBlockVector TreeLogs, TreeOther;
	GetTreeImageByBiome(
		a_ChunkX * cChunkDef::Width + x, Height + 1, a_ChunkZ * cChunkDef::Width + z,
		m_Noise, a_Seq, 
		cChunkDef::GetBiome(a_Biomes, x, z),
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

		BLOCKTYPE Block = cChunkDef::GetBlock(a_BlockTypes, itr->x, itr->y, itr->z);
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
	
	ApplyTreeImage(a_ChunkX, a_ChunkZ, a_BlockTypes, a_BlockMetas, TreeOther, a_OutsideOther);
	ApplyTreeImage(a_ChunkX, a_ChunkZ, a_BlockTypes, a_BlockMetas, TreeLogs,  a_OutsideLogs);	
}





void cStructGenTrees::ApplyTreeImage(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,
	cChunkDef::BlockNibbles & a_BlockMetas,
	const sSetBlockVector & a_Image,
	sSetBlockVector & a_Overflow
)
{
	// Put the generated image into a_BlockTypes, push things outside this chunk into a_Blocks
	for (sSetBlockVector::const_iterator itr = a_Image.begin(); itr != a_Image.end(); ++itr)
	{
		if ((itr->ChunkX == a_ChunkX) && (itr->ChunkZ == a_ChunkZ))
		{
			// Inside this chunk, integrate into a_BlockTypes:
			switch (cChunkDef::GetBlock(a_BlockTypes, itr->x, itr->y, itr->z))
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
					cChunkDef::SetBlock (a_BlockTypes, itr->x, itr->y, itr->z, itr->BlockType);
					cChunkDef::SetNibble(a_BlockMetas, itr->x, itr->y, itr->z, itr->BlockMeta);
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
		switch (a_Biomes[x + cChunkDef::Width * z])
		{
			case biPlains:           Add = 1;  break;
			case biExtremeHills:     Add = 3;  break;
			case biForest:           Add = 30; break;
			case biTaiga:            Add = 30; break;
			case biSwampland:        Add = 8;  break;
			case biIcePlains:        Add = 1;  break;
			case biIceMountains:     Add = 1;  break;
			case biMushroomIsland:   Add = 3;  break;
			case biMushroomShore:    Add = 3;  break;
			case biForestHills:      Add = 20; break;
			case biTaigaHills:       Add = 20; break;
			case biExtremeHillsEdge: Add = 5;  break;
			case biJungle:           Add = 120; break;
			case biJungleHills:      Add = 90; break;
		}
		NumTrees += Add;
	}
	return NumTrees / 1024;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenOreNests:

void cStructGenOreNests::GenStructures(cChunkDesc & a_ChunkDesc)
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





