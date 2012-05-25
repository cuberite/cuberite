
// StructGen.h

#include "Globals.h"
#include "StructGen.h"
#include "BlockID.h"
#include "Trees.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenOreNests configuration:

const int MAX_HEIGHT_COAL = 127;
const int NUM_NESTS_COAL = 60;
const int NEST_SIZE_COAL = 10;

const int MAX_HEIGHT_IRON = 70;
const int NUM_NESTS_IRON = 30;
const int NEST_SIZE_IRON = 6;

const int MAX_HEIGHT_REDSTONE = 40;
const int NUM_NESTS_REDSTONE = 10;
const int NEST_SIZE_REDSTONE = 6;

const int MAX_HEIGHT_GOLD = 35;
const int NUM_NESTS_GOLD = 6;
const int NEST_SIZE_GOLD = 6;

const int MAX_HEIGHT_DIAMOND = 16;
const int NUM_NESTS_DIAMOND = 6;
const int NEST_SIZE_DIAMOND = 5;

const int MAX_HEIGHT_LAPIS = 30;
const int NUM_NESTS_LAPIS = 6;
const int NEST_SIZE_LAPIS = 5;





template <typename T> T Clamp(T a_Value, T a_Min, T a_Max)
{
	return (a_Value < a_Min) ? a_Min : ((a_Value > a_Max) ? a_Max : a_Value);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenTrees:

void cStructGenTrees::GenStructures(
	int a_ChunkX, int a_ChunkZ, 
	cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMetas,  // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
	cEntityList & a_Entities,                // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
)
{
	cChunkDef::BlockTypes   WorkerBlockTypes;
	cChunkDef::BlockNibbles WorkerBlockMeta;
	cChunkDef::HeightMap    WorkerHeight;

	cEntityList Entities;
	cBlockEntityList BlockEntities;
	
	// Generate trees:
	for (int x = 0; x <= 2; x++)
	{
		int BaseX = a_ChunkX + x - 1;
		for (int z = 0; z <= 2; z++)
		{
			int BaseZ = a_ChunkZ + z - 1;
			sSetBlockVector Outside;
			
			cChunkDef::BlockTypes *   BlT;
			cChunkDef::BlockNibbles * BlM;
			cChunkDef::HeightMap *    Hei;
			
			if ((x != 1) || (z != 1))
			{
				BlT = &WorkerBlockTypes;
				BlM = &WorkerBlockMeta;
				Hei = &WorkerHeight;
				
				m_HeightGen->GenHeightMap       (BaseX, BaseZ, *Hei);
				m_CompositionGen->ComposeTerrain(BaseX, BaseZ, *BlT, *BlM, *Hei, Entities, BlockEntities);
				// TODO: Free the entity lists
			}
			else
			{
				BlT = &a_BlockTypes;
				BlM = &a_BlockMetas;
				Hei = &a_HeightMap;
			}

			cChunkDef::BiomeMap Biomes;
			m_BiomeGen->GenBiomes(BaseX, BaseZ, Biomes);
			int NumTrees = GetNumTrees(BaseX, BaseZ, Biomes);

			for (int i = 0; i < NumTrees; i++)
			{
				GenerateSingleTree(BaseX, BaseZ, i, *BlT, *BlM, *Hei, Biomes, Outside);
			}

			// Integrate blocks in Outside into chunk:
			for (sSetBlockVector::const_iterator itr = Outside.begin(); itr != Outside.end(); ++itr)
			{
				if ((itr->ChunkX != a_ChunkX) || (itr->ChunkZ != a_ChunkZ))
				{
					continue;
				}
				switch (cChunkDef::GetBlock(a_BlockTypes, itr->x, itr->y, itr->z))
				{
					CASE_TREE_OVERWRITTEN_BLOCKS:
					{
						cChunkDef::SetBlock (a_BlockTypes, itr->x, itr->y, itr->z, itr->BlockType);
						cChunkDef::SetNibble(a_BlockMetas, itr->x, itr->y, itr->z, itr->BlockMeta);
						break;
					}
				}  // switch (GetBlock())
			}  // for itr - Outside[]
		}  // for z
	}  // for x
	
	// Update the heightmap:
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = cChunkDef::Height - 1; y >= 0; y--)
			{
				if (cChunkDef::GetBlock(a_BlockTypes, x, y, z) != E_BLOCK_AIR)
				{
					cChunkDef::SetHeight(a_HeightMap, x, z, y);
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
	sSetBlockVector & a_Blocks
)
{
	int x = m_Noise.IntNoise3DInt(a_ChunkX + a_ChunkZ, a_ChunkZ, a_Seq) % cChunkDef::Width;
	int z = m_Noise.IntNoise3DInt(a_ChunkX - a_ChunkZ, a_Seq, a_ChunkZ) % cChunkDef::Width;
	
	int Height = a_Height[x + cChunkDef::Width * z];
	
	if ((Height <= 0) || (Height > 240))
	{
		return;
	}

	// Check the block underneath the tree:
	BLOCKTYPE TopBlock = cChunkDef::GetBlock(a_BlockTypes, x, Height, z);
	if ((TopBlock != E_BLOCK_DIRT) && (TopBlock != E_BLOCK_GRASS) && (TopBlock != E_BLOCK_SOIL))
	{
		return;
	}
	
	sSetBlockVector TreeBlocks;
	GetTreeImageByBiome(a_ChunkX * cChunkDef::Width + x, Height + 1, a_ChunkZ * cChunkDef::Width + z, m_Noise, a_Seq, a_Biomes[x + cChunkDef::Width * z], TreeBlocks);

	// Check if the generated image fits the terrain:
	for (sSetBlockVector::const_iterator itr = TreeBlocks.begin(); itr != TreeBlocks.end(); ++itr)
	{
		if ((itr->ChunkX != a_ChunkX) || (itr->ChunkZ != a_ChunkZ) || (itr->BlockType != E_BLOCK_LOG))
		{
			// Outside the chunk, or not a log (we don't check non-logs)
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
	
	// Put the generated image into a_BlockTypes, push things outside this chunk into a_Blocks
	for (sSetBlockVector::const_iterator itr = TreeBlocks.begin(); itr != TreeBlocks.end(); ++itr)
	{
		if ((itr->ChunkX == a_ChunkX) && (itr->ChunkZ == a_ChunkZ))
		{
			// Inside this chunk, integrate into a_BlockTypes:
			switch (cChunkDef::GetBlock(a_BlockTypes, itr->x, itr->y, itr->z))
			{
				CASE_TREE_OVERWRITTEN_BLOCKS:
				{
					cChunkDef::SetBlock (a_BlockTypes, itr->x, itr->y, itr->z, itr->BlockType);
					cChunkDef::SetNibble(a_BlockMetas, itr->x, itr->y, itr->z, itr->BlockMeta);
					break;
				}
			}  // switch (GetBlock())
			continue;
		}
		
		// Outside the chunk, push into a_Blocks; check if already present there:
		bool Found = false;
		for (sSetBlockVector::iterator itrB = a_Blocks.begin(); itrB != a_Blocks.end(); ++itrB)
		{
			if (
				(itr->ChunkX == itrB->ChunkX) &&
				(itr->ChunkZ == itrB->ChunkZ) &&
				(itr->x == itrB->x) &&
				(itr->y == itrB->y) &&
				(itr->z == itrB->z)
			)
			{
				// Same coords already found in a_Blocks, overwrite with wood, if requested:
				if (itr->BlockType == E_BLOCK_LOG)
				{
					itrB->BlockType = itr->BlockType;
					itrB->BlockMeta = itr->BlockMeta;
				}
				Found = true;
				break;
			}
		}  // for itrB - a_Blocks[]
		if (!Found)
		{
			a_Blocks.push_back(*itr);
		}
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
// cStructGenMarbleCaves:

static float GetMarbleNoise( float x, float y, float z, cNoise & a_Noise )
{
	static const float PI_2 = 1.57079633f;
	float oct1 = (a_Noise.CubicNoise3D(x * 0.1f, y * 0.1f, z * 0.1f )) * 4;

	oct1 = oct1 * oct1 * oct1;
	if (oct1 < 0.f)  oct1 = PI_2;
	if (oct1 > PI_2) oct1 = PI_2;

	return oct1;
}





void cStructGenMarbleCaves::GenStructures(
	int a_ChunkX, int a_ChunkZ, 
	cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
	cEntityList & a_Entities,               // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
)
{
	cNoise Noise(m_Seed);
	for (int z = 0; z < cChunkDef::Width; z++) 
	{
		const float zz = (float)(a_ChunkZ * cChunkDef::Width + z);
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			const float xx = (float)(a_ChunkX * cChunkDef::Width + x);
			
			int Top = cChunkDef::GetHeight(a_HeightMap, x, z);
			for (int y = 1; y < Top; ++y )
			{
				if (cChunkDef::GetBlock(a_BlockTypes, x, y, z) != E_BLOCK_STONE)
				{
					continue;
				}

				const float yy = (float)y;
				const float WaveNoise = 1;
				if (cosf(GetMarbleNoise(xx, yy * 0.5f, zz, Noise)) * fabs(cosf(yy * 0.2f + WaveNoise * 2) * 0.75f + WaveNoise) > 0.0005f)
				{
					if (y > 4)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_AIR);
					}
					else
					{
						cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_STATIONARY_LAVA);
					}
				}
			}  // for y
		}  // for x
	}  // for z
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenOreNests:

void cStructGenOreNests::GenStructures(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,   // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,  // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,     // Height map to read and change by the current data
	cEntityList & a_Entities,               // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities      // Block entities may be added or deleted
)
{
	GenerateOre(a_ChunkX, a_ChunkZ, E_BLOCK_COAL_ORE,     MAX_HEIGHT_COAL,     NUM_NESTS_COAL,     NEST_SIZE_COAL,     a_BlockTypes, 1);
	GenerateOre(a_ChunkX, a_ChunkZ, E_BLOCK_IRON_ORE,     MAX_HEIGHT_IRON,     NUM_NESTS_IRON,     NEST_SIZE_IRON,     a_BlockTypes, 2);
	GenerateOre(a_ChunkX, a_ChunkZ, E_BLOCK_REDSTONE_ORE, MAX_HEIGHT_REDSTONE, NUM_NESTS_REDSTONE, NEST_SIZE_REDSTONE, a_BlockTypes, 3);
	GenerateOre(a_ChunkX, a_ChunkZ, E_BLOCK_GOLD_ORE,     MAX_HEIGHT_GOLD,     NUM_NESTS_GOLD,     NEST_SIZE_GOLD,     a_BlockTypes, 4);
	GenerateOre(a_ChunkX, a_ChunkZ, E_BLOCK_DIAMOND_ORE,  MAX_HEIGHT_DIAMOND,  NUM_NESTS_DIAMOND,  NEST_SIZE_DIAMOND,  a_BlockTypes, 5);
	GenerateOre(a_ChunkX, a_ChunkZ, E_BLOCK_LAPIS_ORE,    MAX_HEIGHT_LAPIS,    NUM_NESTS_LAPIS,    NEST_SIZE_LAPIS,    a_BlockTypes, 6);
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




