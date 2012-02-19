
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





static float GetOreNoise( float x, float y, float z, cNoise & a_Noise )
{
	float oct1 = a_Noise.CubicNoise3D( x*0.1f, y*0.1f, z*0.1f );
	float oct2 = a_Noise.CubicNoise3D( x*0.05f, y*0.5f, z*0.05f );

	oct2 *= oct2;
	oct1 = (1 - (oct1 * oct1 *100)) * oct2;
	//if( oct1 < 0.5f ) oct1 = 0;
	//else oct1 = 1.f;

	return oct1;
}





unsigned int cWorldGenerator::MakeIndex(int x, int y, int z )
{
	ASSERT((x < 16) && (x > -1) && (y < 128) && (y > -1) && (z < 16) && (z > -1));

	return y + (z * 128) + (x * 128 * 16);
}





void cWorldGenerator::GenerateTerrain(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData)
{
	const int WATER_LEVEL = 60;
	const int SAND_LEVEL = 3;
	
	memset(a_BlockData, E_BLOCK_AIR, cChunk::c_BlockDataSize);

	cNoise Noise(m_World->GetWorldSeed());
	
	for (int z = 0; z < 16; z++) 
	{
		const float zz = (float)(a_ChunkZ * 16 + z);
		for (int x = 0; x < 16; x++)
		{
			// Place bedrock on bottom layer
			a_BlockData[MakeIndex(x, 0, z)] = E_BLOCK_BEDROCK;

			const float xx = (float)(a_ChunkX * 16 + x);
			
			int Height = (int)(GetNoise( xx * 0.05f, zz * 0.05f, Noise ) * 16);
			const int Lower = 64;
			if ( Height + Lower > 127 )
			{
				Height = 127 - Lower;
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
					if ( GetOreNoise( xx, yy, zz, Noise ) > 0.5f )
					{
						a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_COAL_ORE;
					}
					else if ( GetOreNoise( xx, yy+100.f, zz, Noise ) > 0.6f )
					{
						a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_IRON_ORE;
					}
					else if (( yy < 20) && (GetOreNoise( xx * 1.5f, yy + 300.f, zz * 1.5f, Noise ) > 0.6f ))
					{
						a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_REDSTONE_ORE;
					}
					else if (( yy < 30) && (GetOreNoise( xx * 2, yy + 200.f, zz * 2, Noise ) > 0.75f ))
					{
						a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_DIAMOND_ORE;
					}
					else if (( yy < 40) && (GetOreNoise( xx * 2, yy + 100.f, zz * 2, Noise ) > 0.75f ))
					{
						a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_GOLD_ORE;
					}
					else
					{
						a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_STONE;
					}
				}
				else
				{
					a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_DIRT;
				}
			}  // for y
			
			if (Top + 1 >= WATER_LEVEL + SAND_LEVEL)
			{
				// Replace top dirt with grass:
				a_BlockData[MakeIndex(x, Top - 1, z)] = E_BLOCK_GRASS;
			}
			else
			{
				// Add water up to the WATER_LEVEL:
				for (int y = Top; y < WATER_LEVEL; ++y )
				{
					a_BlockData[ MakeIndex(x, y, z) ] = E_BLOCK_STATIONARY_WATER;
				}
			}
			
			// Generate small foliage (1-block):
			int index = MakeIndex(x, Top - 1, z);
			int TopY = Top - 1;
			float val1 = Noise.CubicNoise2D(xx * 0.1f,  zz * 0.1f );
			float val2 = Noise.CubicNoise2D(xx * 0.01f, zz * 0.01f );
			if( a_BlockData[index] == E_BLOCK_SAND )
			{
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
			else if( a_BlockData[index] == E_BLOCK_GRASS )
			{
				float val3 = Noise.CubicNoise2D(xx * 0.01f + 10, zz * 0.01f + 10 );
				float val4 = Noise.CubicNoise2D(xx * 0.05f + 20, zz * 0.05f + 20 );
				if( val3 > 0.2f && (r1.randInt()%128) > 124 )
				{
					a_BlockData[ MakeIndex(x, TopY+1, z) ] = E_BLOCK_YELLOW_FLOWER;
				}
				else if( val4 > 0.2f && (r1.randInt()%128) > 124 )
				{
					a_BlockData[ MakeIndex(x, TopY+1, z) ] = E_BLOCK_RED_ROSE;
				}
				else if( val1+val2+val3+val4 > 0.2f && (r1.randInt()%128) > 124 )
				{
					a_BlockData[ MakeIndex(x, TopY+1, z) ] = E_BLOCK_RED_MUSHROOM;
				}
				else if( val1+val2+val3+val4 > 0.2f && (r1.randInt()%128) > 124 )
				{
					a_BlockData[ MakeIndex(x, TopY+1, z) ] = E_BLOCK_BROWN_MUSHROOM;
				}
			}
		}  // for x
	}  // for z
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
	for (int z = 0; z < 16; z++) 
	{
		int zz = z + a_ChunkZ * 16;
		for (int x = 0; x < 16; x++)
		{
			int xx = x + a_ChunkX * 16;

			int TopY = m_World->GetHeight(xx, zz);
			int index = MakeIndex(x, TopY - 1, z);
			if (BlockType[index] == E_BLOCK_GRASS )
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




