
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cWorldGenerator.h"
#include "cNoise.h"
#include "cWorld.h"
#include "cChunk.h"
#include "cGenSettings.h"
#include "MersenneTwister.h"

#include "BlockID.h"
#include "Vector3i.h"





cWorldGenerator::cWorldGenerator()
{
}

cWorldGenerator::~cWorldGenerator()
{
}


void cWorldGenerator::GenerateChunk( cChunk* a_Chunk )
{
	GenerateTerrain( a_Chunk );
	GenerateFoliage( a_Chunk );
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

void cWorldGenerator::GenerateTerrain( cChunk* a_Chunk )
{
	Vector3i ChunkPos( a_Chunk->GetPosX(), a_Chunk->GetPosY(), a_Chunk->GetPosZ() );
	char* BlockType = a_Chunk->pGetType();

	//const ENUM_BLOCK_ID GrassID 		=	E_BLOCK_GRASS;
	const ENUM_BLOCK_ID DirtID 			=	E_BLOCK_DIRT;
	const ENUM_BLOCK_ID StoneID 		=	E_BLOCK_STONE;
	const ENUM_BLOCK_ID SandID 			=	E_BLOCK_SAND;
	const ENUM_BLOCK_ID SandStoneID		=	E_BLOCK_SANDSTONE;
	const ENUM_BLOCK_ID CaveID 			=	E_BLOCK_AIR;
	const ENUM_BLOCK_ID LavaID 			=	E_BLOCK_STATIONARY_LAVA;
	const ENUM_BLOCK_ID CoalID 			=	E_BLOCK_COAL_ORE;
	const ENUM_BLOCK_ID IronID 			=	E_BLOCK_IRON_ORE;
	const ENUM_BLOCK_ID GoldID 			=	E_BLOCK_GOLD_ORE;
	const ENUM_BLOCK_ID DiamondID 		=	E_BLOCK_DIAMOND_ORE;
	const ENUM_BLOCK_ID RedID 			=	E_BLOCK_REDSTONE_ORE;

	cNoise Noise( a_Chunk->GetWorld()->GetWorldSeed() );
	for(int z = 0; z < 16; z++) 
	{
		const float zz = (float)(ChunkPos.z*16 + z);
		for(int x = 0; x < 16; x++)
		{
			// Place bedrock on bottom layer
			BlockType[ cChunk::MakeIndex(x, 0, z) ] = E_BLOCK_BEDROCK;

			const float xx = (float)(ChunkPos.x*16 + x);
			
			int Height = (int)(GetNoise( xx*0.05f, zz*0.05f, Noise )*16);
			const int Lower = 64;
			if( Height+Lower > 127 ) Height = 127-Lower;
			const int Top = Lower+Height;
			const float WaveNoise = 1;//m_Noise.CubicNoise2D( xx*0.01f, zz*0.01f ) + 0.5f;
			for( int y = 1; y < Top; ++y )
			{
				const float yy = (float)y;
				//   V prevent caves from getting too close to the surface
				if( (Top - y > (WaveNoise*2) ) && cosf(GetMarbleNoise( xx, yy*0.5f, zz, Noise )) * fabs( cosf( yy*0.2f + WaveNoise*2 )*0.75f + WaveNoise ) > 0.5f )
				{
					if( y > 4 )
					{
						BlockType[ cChunk::MakeIndex(x, y, z) ] = CaveID;
						if( z > 0 ) BlockType[ cChunk::MakeIndex(x, y, z-1) ] = CaveID;
						if( z < 15 ) BlockType[ cChunk::MakeIndex(x, y, z+1) ] = CaveID;
						if( x > 0 ) BlockType[ cChunk::MakeIndex(x-1, y, z) ] = CaveID;
						if( x < 15 ) BlockType[ cChunk::MakeIndex(x+1, y, z) ] = CaveID;
					}
					else
					{
						BlockType[ cChunk::MakeIndex(x, y, z) ] = LavaID;
					}
				}
				else if( y < 61 && Top - y < 3 )
					BlockType[ cChunk::MakeIndex(x, y, z) ] = SandID;
				else if( y < 61 && Top - y < 4 )
					BlockType[ cChunk::MakeIndex(x, y, z) ] = SandStoneID;
				else if( Top - y > ((WaveNoise+1.5f)*1.5f) ) // rock and ores between 1.5 .. 4.5 deep
				{
					if( GetOreNoise( xx, yy, zz, Noise ) > 0.5f )
						BlockType[ cChunk::MakeIndex(x, y, z) ] = CoalID;
					else if( GetOreNoise( xx, yy+100.f, zz, Noise ) > 0.6f )
						BlockType[ cChunk::MakeIndex(x, y, z) ] = IronID;
					else if( yy < 20 && GetOreNoise( xx*1.5f, yy+300.f, zz*1.5f, Noise ) > 0.6f )
						BlockType[ cChunk::MakeIndex(x, y, z) ] = RedID;
					else if( yy < 30 && GetOreNoise( xx*2, yy+200.f, zz*2, Noise ) > 0.75f )
						BlockType[ cChunk::MakeIndex(x, y, z) ] = DiamondID;
					else if( yy < 40 && GetOreNoise( xx*2, yy+100.f, zz*2, Noise ) > 0.75f )
						BlockType[ cChunk::MakeIndex(x, y, z) ] = GoldID;
					else
						BlockType[ cChunk::MakeIndex(x, y, z) ] = StoneID;
				}
				else
					BlockType[ cChunk::MakeIndex(x, y, z) ] = DirtID;
			}
			for( int y = Lower+Height; y < 60; ++y )
			{
				BlockType[ cChunk::MakeIndex(x, y, z) ] = E_BLOCK_STATIONARY_WATER;
			}
		}
	}
}



void cWorldGenerator::GenerateFoliage( cChunk* a_Chunk )
{
	const ENUM_BLOCK_ID GrassID 		=	E_BLOCK_GRASS;
	const ENUM_BLOCK_ID DirtID 			=	E_BLOCK_DIRT;
	const ENUM_BLOCK_ID SandID 			=	E_BLOCK_SAND;
	const ENUM_BLOCK_ID SandStoneID		=	E_BLOCK_SANDSTONE;
	const ENUM_BLOCK_ID CaveID 			=	E_BLOCK_AIR;

	int PosX = a_Chunk->GetPosX();
	int PosZ = a_Chunk->GetPosZ();

	cWorld* World = a_Chunk->GetWorld();
	cNoise m_Noise( World->GetWorldSeed() );
	char* BlockType = a_Chunk->pGetType();

	for(int z = 0; z < 16; z++) for(int x = 0; x < 16; x++)
	{
		// Find top most Y
		int TopY = -1;
		for(int y = 127; y > 0; y--)
		{
			int index =  cChunk::MakeIndex( x, y, z );
			if( BlockType[index] != E_BLOCK_AIR )
			{
				TopY = y;
				break;
			}
		}
		if( TopY > 0 )
		{
			// Change top dirt into grass and prevent sand from floating over caves
			int index = cChunk::MakeIndex( x, TopY, z );
			int index1 = cChunk::MakeIndex( x, TopY-1, z );
			int index2 = cChunk::MakeIndex( x, TopY-2, z );
			int index3 = cChunk::MakeIndex( x, TopY-3, z );
			int index4 = cChunk::MakeIndex( x, TopY-4, z );
			int index5 = cChunk::MakeIndex( x, TopY-5, z );

			if( BlockType[index] == SandID ) {

				if( BlockType[index1] == CaveID ) {
					BlockType[ index ] = (char)SandStoneID;
				} else if( BlockType[index2] == CaveID ) {
					BlockType[ index1 ] = (char)SandStoneID;
				} else if( BlockType[index3] == CaveID ) {
					BlockType[ index2 ] = (char)SandStoneID;
				} else if( BlockType[index4] == CaveID ) {
					BlockType[ index3 ] = (char)SandStoneID;
				} else if( BlockType[index5] == CaveID ) {
					BlockType[ index4 ] = (char)SandStoneID;
				}

			}

			if( BlockType[index] == DirtID )
			{
				BlockType[ index ] = (char)GrassID;
			}

			MTRand r1;
			// Plant sum trees
			{
				int xx = x + PosX*16;
				int zz = z + PosZ*16;

				float val1 = m_Noise.CubicNoise2D( xx*0.1f, zz*0.1f );
				float val2 = m_Noise.CubicNoise2D( xx*0.01f, zz*0.01f );
				if( BlockType[index] == SandID )
				{
					if( (val1 + val2 > 0.f) && (r1.randInt()%128) > 124 && BlockType[index] == E_BLOCK_SAND )
					{
						BlockType[ cChunk::MakeIndex(x, TopY+1, z) ] = E_BLOCK_CACTUS;
						if( (r1.randInt() & 3) == 3 )
						{
							BlockType[ cChunk::MakeIndex(x, TopY+2, z) ] = E_BLOCK_CACTUS;
						}
						continue;
					}
				}
				else if( BlockType[index] == GrassID )
				{
					float val3 = m_Noise.CubicNoise2D( xx*0.01f+10, zz*0.01f+10 );
					float val4 = m_Noise.CubicNoise2D( xx*0.05f+20, zz*0.05f+20 );
					if( val1 + val2 > 0.2f && (r1.randInt()%128) > 124 )
						World->GrowTree( xx, TopY, zz );
					else if( val3 > 0.2f && (r1.randInt()%128) > 124 )
						BlockType[ cChunk::MakeIndex(x, TopY+1, z) ] = E_BLOCK_YELLOW_FLOWER;
					else if( val4 > 0.2f && (r1.randInt()%128) > 124 )
						BlockType[ cChunk::MakeIndex(x, TopY+1, z) ] = E_BLOCK_RED_ROSE;
					else if( val1+val2+val3+val4 > 0.2f && (r1.randInt()%128) > 124 )
						BlockType[ cChunk::MakeIndex(x, TopY+1, z) ] = E_BLOCK_RED_MUSHROOM;
					else if( val1+val2+val3+val4 > 0.2f && (r1.randInt()%128) > 124 )
						BlockType[ cChunk::MakeIndex(x, TopY+1, z) ] = E_BLOCK_BROWN_MUSHROOM;
				}
			}

		}
	}
}
