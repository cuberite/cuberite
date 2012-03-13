
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWorldGenerator.h"
#include "cNoise.h"
#include "cWorld.h"
#include "cChunk.h"
#include "cGenSettings.h"

#include "BlockID.h"
#include "Vector3i.h"





// An array describing an 8-way neighbor coords deltas
static struct 
{
	int m_X;
	int m_Z;
} g_NeighborCoords[] = 
{
	{-1, -1},
	{-1, 0},
	{-1, 1},
	{0, -1},
	{0, 1},
	{1, -1},
	{1, 0},
	{1, 1},
} ;





// You can use GLASS for these instead for debugging ore generation ;)
// Beware though, client has problems with this much glass!
const char BLOCK_STONE = E_BLOCK_STONE;
const char BLOCK_DIRT = E_BLOCK_DIRT;
const char BLOCK_GRASS = E_BLOCK_GRASS;

const int MAX_HEIGHT_COAL = 127;
const int NUM_NESTS_COAL = 40;
const int NEST_SIZE_COAL = 10;

const int MAX_HEIGHT_IRON = 70;
const int NUM_NESTS_IRON = 10;
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





cWorldGenerator::cWorldGenerator(cWorld * a_World) :
	m_World(a_World)
{
}





cWorldGenerator::~cWorldGenerator()
{
}





void cWorldGenerator::GenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities)
{
	GenerateTerrain(a_ChunkX, a_ChunkY, a_ChunkZ, a_BlockData);
}





void cWorldGenerator::PostGenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	// Check the chunk just generated and all its 8-way neighbors

	// Make the chunks stay loaded in the surrounding 5x5 area:
	cChunkStay Stay(m_World);
	Stay.Add(a_ChunkX, a_ChunkY, a_ChunkZ);
	for (int x = -2; x <= 2; x++)
	{
		for (int z = -2; z <= 2; z++)
		{
			Stay.Add(a_ChunkX + x, a_ChunkY, a_ChunkZ + z);
		}  // for z
	}  // for x
	Stay.Enable();
	
	m_World->LoadChunks(Stay);
	
	CheckNeighbors(a_ChunkX, a_ChunkY, a_ChunkZ);
	for (int i = 0; i < ARRAYCOUNT(g_NeighborCoords); i++)
	{
		CheckNeighbors(a_ChunkX + g_NeighborCoords[i].m_X, a_ChunkY, a_ChunkZ + g_NeighborCoords[i].m_Z);
	}  // for i - g_NeighborCoords[]
}





void cWorldGenerator::CheckNeighbors(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	if (!m_World->IsChunkValid(a_ChunkX, a_ChunkY, a_ChunkZ))
	{
		return;
	}
	
	// Check all 8-way neighbors, if they are all valid, generate foliage in this chunk:
	int NumNeighbors = 0;
	for (int i = 0; i < ARRAYCOUNT(g_NeighborCoords); i++)
	{
		if (m_World->IsChunkValid(a_ChunkX + g_NeighborCoords[i].m_X, a_ChunkY, a_ChunkZ + g_NeighborCoords[i].m_Z))
		{
			NumNeighbors++;
		}
	}  // for i - g_NeighborCoords[]
	if (NumNeighbors == 8)
	{
		GenerateFoliage(a_ChunkX, a_ChunkY, a_ChunkZ);
	}
}





static float GetNoise( float x, float y, cNoise & a_Noise )
{
	float oct1 = a_Noise.CubicNoise2D( x*cGenSettings::HeightFreq1, y*cGenSettings::HeightFreq1 )*cGenSettings::HeightAmp1;
	float oct2 = a_Noise.CubicNoise2D( x*cGenSettings::HeightFreq2, y*cGenSettings::HeightFreq2 )*cGenSettings::HeightAmp2;
	float oct3 = a_Noise.CubicNoise2D( x*cGenSettings::HeightFreq3, y*cGenSettings::HeightFreq3 )*cGenSettings::HeightAmp3;

	float height = a_Noise.CubicNoise2D( x*0.1f, y*0.1f )*2;

	float flatness = ((a_Noise.CubicNoise2D( x*0.5f, y*0.5f ) + 1.f ) * 0.5f) * 1.1f; // 0 ... 1.5
	flatness *= flatness * flatness;

	return (oct1 + oct2 + oct3) * flatness + height;
}





#define PI_2 (1.57079633f)
static float GetMarbleNoise( float x, float y, float z, cNoise & a_Noise )
{
	float oct1 = (a_Noise.CubicNoise3D( x*0.1f, y*0.1f, z*0.1f ))*4;

	oct1 = oct1 * oct1 * oct1;
	if( oct1 < 0.f ) oct1 = PI_2;
	if( oct1 > PI_2 ) oct1 = PI_2;

	return oct1;
}





unsigned int cWorldGenerator::MakeIndex(int x, int y, int z )
{
	ASSERT((x < cChunk::c_ChunkWidth) && (x > -1) && (y < cChunk::c_ChunkHeight) && (y > -1) && (z < cChunk::c_ChunkWidth) && (z > -1));

	return cChunk::MakeIndexNoCheck( x, y, z );
}





void cWorldGenerator::GenerateTerrain(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData)
{
	const int WATER_LEVEL = 60;
	const int SAND_LEVEL = 3;
	
	memset(a_BlockData, E_BLOCK_AIR, cChunk::c_BlockDataSize);

	cNoise Noise(m_World->GetWorldSeed());
	
	for (int z = 0; z < cChunk::c_ChunkWidth; z++) 
	{
		const float zz = (float)(a_ChunkZ * cChunk::c_ChunkWidth + z);
		for (int x = 0; x < cChunk::c_ChunkWidth; x++)
		{
			// Place bedrock on bottom layer
			a_BlockData[MakeIndex(x, 0, z)] = E_BLOCK_BEDROCK;

			const float xx = (float)(a_ChunkX * cChunk::c_ChunkWidth + x);
			
			int Height = (int)(GetNoise( xx * 0.05f, zz * 0.05f, Noise ) * 16);
			const int Lower = 64;
			if ( Height + Lower > 127 )
			{
				Height = 127 - Lower;
			}
			if (Height < -63)
			{
				Height = -63;
			}
			const int Top = Lower + Height;
			const float WaveNoise = 1;  // m_Noise.CubicNoise2D( xx*0.01f, zz*0.01f ) + 0.5f;
			for( int y = 1; y < Top; ++y )
			{
				const float yy = (float)y;
				//   V prevent caves from getting too close to the surface
				if( (Top - y > (WaveNoise*2) ) && cosf(GetMarbleNoise( xx, yy*0.5f, zz, Noise )) * fabs( cosf( yy*0.2f + WaveNoise*2 )*0.75f + WaveNoise ) > 0.5f )
				{
					if( y > 4 )
					{
						a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_AIR;
						if( z > 0 ) a_BlockData[ MakeIndex(x, y, z-1) ] = E_BLOCK_AIR;
						if( z < 15 ) a_BlockData[ MakeIndex(x, y, z+1) ] = E_BLOCK_AIR;
						if( x > 0 ) a_BlockData[ MakeIndex(x-1, y, z) ] = E_BLOCK_AIR;
						if( x < 15 ) a_BlockData[ MakeIndex(x+1, y, z) ] = E_BLOCK_AIR;
					}
					else
					{
						a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_STATIONARY_LAVA;
					}
				}
				else if ((y < 61) && (Top - y < SAND_LEVEL ))
				{
					a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_SAND;
				}
				else if ((y < 61) && (Top - y < 4 ))
				{
					a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_SANDSTONE;
				}
				else if (Top - y > ((WaveNoise + 1.5f) * 1.5f)) // rock and ores between 1.5 .. 4.5 deep
				{
					{
						a_BlockData[ MakeIndex(x, y, z) ] = BLOCK_STONE;
					}
				}
				else
				{
					a_BlockData[ MakeIndex(x, y, z) ] = BLOCK_DIRT;
				}
			}  // for y
			
			if (Top + 1 >= WATER_LEVEL + SAND_LEVEL)
			{
				// Replace top dirt with grass:
				a_BlockData[MakeIndex(x, Top - 1, z)] = BLOCK_GRASS;
			
				// Generate small foliage (1-block):
				int TopY = Top - 1;
				float val1 = Noise.CubicNoise2D(xx * 0.1f,  zz * 0.1f );
				float val2 = Noise.CubicNoise2D(xx * 0.01f, zz * 0.01f );
				float val3 = Noise.CubicNoise2D(xx * 0.01f + 10, zz * 0.01f + 10 );
				float val4 = Noise.CubicNoise2D(xx * 0.05f + 20, zz * 0.05f + 20 );
				if ((val3 > 0.2f) && ((r1.randInt()%128) > 124))
				{
					a_BlockData[ MakeIndex(x, TopY + 1, z) ] = E_BLOCK_YELLOW_FLOWER;
				}
				else if ((val4 > 0.2f) && ((r1.randInt() % 128) > 124))
				{
					a_BlockData[ MakeIndex(x, TopY + 1, z) ] = E_BLOCK_RED_ROSE;
				}
				else if ((val1 + val2 + val3 + val4 > 0.2f) && ((r1.randInt() % 128) > 124))
				{
					a_BlockData[ MakeIndex(x, TopY + 1, z) ] = E_BLOCK_RED_MUSHROOM;
				}
				else if ((val1 + val2 + val3 + val4 > 0.2f) && ((r1.randInt() % 128) > 124))
				{
					a_BlockData[ MakeIndex(x, TopY + 1, z) ] = E_BLOCK_BROWN_MUSHROOM;
				}
			}  // if (Top above beach-level)
			else if (Top > WATER_LEVEL)
			{
				// This is the sandy shore, generate cacti here
				int TopY = Top - 1;
				float val1 = Noise.CubicNoise2D(xx * 0.1f,  zz * 0.1f );
				float val2 = Noise.CubicNoise2D(xx * 0.01f, zz * 0.01f );
				if ((val1 + val2 > 0.f) && ((r1.randInt() % 128) > 124))
				{
					a_BlockData[ MakeIndex(x, TopY + 1, z) ] = E_BLOCK_CACTUS;
					if ((r1.randInt() & 3) == 3)
					{
						a_BlockData[ MakeIndex(x, TopY + 2, z) ] = E_BLOCK_CACTUS;
					}
					continue;
				}
			}
			else
			{
				// Add water up to the WATER_LEVEL:
				for (int y = Top; y < WATER_LEVEL; ++y )
				{
					a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_STATIONARY_WATER;
				}
			}  // else (Top is under waterlevel)
		}  // for x
	}  // for z
	
	// Generate ores:
	GenerateOre(E_BLOCK_COAL_ORE,     MAX_HEIGHT_COAL,     NUM_NESTS_COAL,     NEST_SIZE_COAL,     a_BlockData);
	GenerateOre(E_BLOCK_IRON_ORE,     MAX_HEIGHT_IRON,     NUM_NESTS_IRON,     NEST_SIZE_IRON,     a_BlockData);
	GenerateOre(E_BLOCK_REDSTONE_ORE, MAX_HEIGHT_REDSTONE, NUM_NESTS_REDSTONE, NEST_SIZE_REDSTONE, a_BlockData);
	GenerateOre(E_BLOCK_GOLD_ORE,     MAX_HEIGHT_GOLD,     NUM_NESTS_GOLD,     NEST_SIZE_GOLD,     a_BlockData);
	GenerateOre(E_BLOCK_DIAMOND_ORE,  MAX_HEIGHT_DIAMOND,  NUM_NESTS_DIAMOND,  NEST_SIZE_DIAMOND,  a_BlockData);
}





void cWorldGenerator::GenerateOre(char a_OreType, int a_MaxHeight, int a_NumNests, int a_NestSize, char * a_BlockData)
{
	// This function generates several "nests" of ore, each nest consisting of number of ore blocks relatively adjacent to each other.
	// It does so by making a random XYZ walk and adding ore along the way in cuboids of different (random) sizes
	// Only stone gets replaced with ore, all other blocks stay (so the nest can actually be smaller than specified).
	for (int i = 0; i < a_NumNests; i++)
	{
		int BaseX = r1.randInt(cChunk::c_ChunkWidth);
		int BaseY = r1.randInt(a_MaxHeight);
		int BaseZ = r1.randInt(cChunk::c_ChunkWidth);
		sSetBlockList OreBlocks;
		size_t NestSize = (size_t)(a_NestSize + r1.randInt(a_NestSize / 4));  // The actual nest size may be up to 1/4 larger
		while (OreBlocks.size() < NestSize)
		{
			// Put a cuboid around [BaseX, BaseY, BaseZ]
			for (int x = r1.randInt(2); x >= 0; --x)
			{
				for (int y = r1.randInt(2); y >= 0; --y)
				{
					for (int z = r1.randInt(2); z >= 0; --z)
					{
						if (OreBlocks.size() < NestSize)
						{
							OreBlocks.push_back(sSetBlock(BaseX + x, BaseY + y, BaseZ + z, a_OreType, 0));
						}
					}  // for z
				}  // for y
			}  // for x
			
			// Move the base to a neighbor voxel
			switch (r1.randInt(4))
			{
				case 0: BaseX--; break;
				case 1: BaseX++; break;
			}
			switch (r1.randInt(4))
			{
				case 0: BaseY--; break;
				case 1: BaseY++; break;
			}
			switch (r1.randInt(4))
			{
				case 0: BaseZ--; break;
				case 1: BaseZ++; break;
			}
		}  // while (OreBlocks.size() < NumBlocks)
		
		// Replace stone with the queued ore blocks:
		for (sSetBlockList::iterator itr = OreBlocks.begin(); itr != OreBlocks.end(); ++itr)
		{
			if ((itr->x < 0) || (itr->y < 0) || (itr->z < 0) || (itr->x >= cChunk::c_ChunkWidth) || (itr->y >= cChunk::c_ChunkHeight-1) || (itr->z >= cChunk::c_ChunkWidth))
			{
				continue;
			}
			int Index = MakeIndex(itr->x, itr->y, itr->z);
			if (a_BlockData[Index] == BLOCK_STONE)
			{
				a_BlockData[Index] = a_OreType;
			}
		}  // for itr - OreBlocks[]
		OreBlocks.clear();
	}  // for i
}





void cWorldGenerator::GenerateFoliage(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	char BlockType[cChunk::c_NumBlocks];
	
	if (!m_World->GetChunkBlocks(a_ChunkX, a_ChunkY, a_ChunkZ, BlockType))
	{
		LOGWARNING("Cannot generate foliage on chunk [%d, %d]", a_ChunkX, a_ChunkZ);
		return;
	}
	
	cNoise Noise(m_World->GetWorldSeed());
	for (int z = 0; z < cChunk::c_ChunkWidth; z++) 
	{
		int zz = z + a_ChunkZ * cChunk::c_ChunkWidth;
		for (int x = 0; x < cChunk::c_ChunkWidth; x++)
		{
			int xx = x + a_ChunkX * cChunk::c_ChunkWidth;

			int TopY = m_World->GetHeight(xx, zz);
			int index = cChunk::MakeIndexNoCheck(x, MAX(TopY - 1, 0), z);
			if (BlockType[index] == BLOCK_GRASS)
			{
				float val1 = Noise.CubicNoise2D( xx * 0.1f, zz * 0.1f );
				float val2 = Noise.CubicNoise2D( xx * 0.01f, zz * 0.01f );
				if ((val1 + val2 > 0.2f) && ((r1.randInt() % 128) > 124))
				{
					m_World->GrowTree( xx, TopY, zz );
				}
			}  // if (Grass)
		}  // for x
	}  // for z
}




