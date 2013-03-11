
// CompoGen.cpp

/* Implements the various terrain composition generators:
	- cCompoGenSameBlock
	- cCompoGenDebugBiomes
	- cCompoGenClassic
*/

#include "Globals.h"
#include "CompoGen.h"
#include "../BlockID.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenSameBlock:

void cCompoGenSameBlock::ComposeTerrain(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
	cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
	const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
	const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
	cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
	cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
)
{
	memset(a_BlockTypes, E_BLOCK_AIR, sizeof(a_BlockTypes));
	memset(a_BlockMeta, 0, sizeof(a_BlockMeta));
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Start;
			if (m_IsBedrocked)
			{
				a_BlockTypes[cChunkDef::MakeIndex(x, 0, z)] = E_BLOCK_BEDROCK;
				Start = 1;
			}
			else
			{
				Start = 0;
			}
			for (int y = a_HeightMap[x + cChunkDef::Width * z]; y >= Start; y--)
			{
				a_BlockTypes[cChunkDef::MakeIndex(x, y, z)] = m_BlockType;
			}  // for y
		}  // for z
	}  // for x
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenDebugBiomes:

void cCompoGenDebugBiomes::ComposeTerrain(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
	cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
	const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
	const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
	cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
	cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
)
{
	static BLOCKTYPE Blocks[] =
	{
		E_BLOCK_STONE,
		E_BLOCK_COBBLESTONE, 
		E_BLOCK_LOG,
		E_BLOCK_PLANKS,
		E_BLOCK_SANDSTONE,
		E_BLOCK_WOOL,
		E_BLOCK_COAL_ORE,
		E_BLOCK_IRON_ORE,
		E_BLOCK_GOLD_ORE,
		E_BLOCK_DIAMOND_ORE,
		E_BLOCK_LAPIS_ORE,
		E_BLOCK_REDSTONE_ORE,
		E_BLOCK_IRON_BLOCK,
		E_BLOCK_GOLD_BLOCK,
		E_BLOCK_DIAMOND_BLOCK,
		E_BLOCK_LAPIS_BLOCK,
		E_BLOCK_BRICK,
		E_BLOCK_MOSSY_COBBLESTONE,
		E_BLOCK_OBSIDIAN,
		E_BLOCK_NETHERRACK,
		E_BLOCK_SOULSAND,
		E_BLOCK_NETHER_BRICK,
		E_BLOCK_BEDROCK,
	} ;
	
	memset(a_BlockTypes, E_BLOCK_AIR, sizeof(a_BlockTypes));
	memset(a_BlockMeta, 0, sizeof(a_BlockMeta));

	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			BLOCKTYPE BlockType = Blocks[cChunkDef::GetBiome(a_BiomeMap, x, z) % ARRAYCOUNT(Blocks)];
			for (int y = a_HeightMap[x + cChunkDef::Width * z]; y >= 0; y--)
			{
				cChunkDef::SetBlock(a_BlockTypes, x, y, z, BlockType);
			}  // for y
		}  // for z
	}  // for x
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenClassic:

cCompoGenClassic::cCompoGenClassic(
	int a_SeaLevel, int a_BeachHeight, int a_BeachDepth,
	BLOCKTYPE a_BlockTop, BLOCKTYPE a_BlockMiddle, BLOCKTYPE a_BlockBottom,
	BLOCKTYPE a_BlockBeach, BLOCKTYPE a_BlockBeachBottom, BLOCKTYPE a_BlockSea
) :
	m_SeaLevel(a_SeaLevel),
	m_BeachHeight(a_BeachHeight),
	m_BeachDepth(a_BeachDepth),
	m_BlockTop(a_BlockTop),
	m_BlockMiddle(a_BlockMiddle),
	m_BlockBottom(a_BlockBottom),
	m_BlockBeach(a_BlockBeach),
	m_BlockBeachBottom(a_BlockBeachBottom),
	m_BlockSea(a_BlockSea)
{
}





void cCompoGenClassic::ComposeTerrain(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
	cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
	const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
	const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
	cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
	cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
)
{
	/* The classic composition means:
		- 1 layer of grass, 3 of dirt and the rest stone, if the height > sealevel + beachheight
		- 3 sand and a 1 sandstone, rest stone if between sealevel and sealevel + beachheight
		- water from waterlevel to height, then 3 sand, 1 sandstone, the rest stone, if water depth < beachdepth
		- water from waterlevel, then 3 dirt, the rest stone otherwise
		- bedrock at the bottom
	*/

	memset(a_BlockTypes, E_BLOCK_AIR, sizeof(a_BlockTypes));
	memset(a_BlockMeta, 0, sizeof(a_BlockMeta));

	// The patterns to use for different situations, must be same length!
	static const BLOCKTYPE PatternGround[] = {m_BlockTop, m_BlockMiddle, m_BlockMiddle, m_BlockMiddle} ;
	static const BLOCKTYPE PatternBeach[]  = {m_BlockBeach,  m_BlockBeach, m_BlockBeach, m_BlockBeachBottom} ;
	static const BLOCKTYPE PatternOcean[]  = {m_BlockMiddle,  m_BlockMiddle, m_BlockMiddle, m_BlockBottom} ;
	static int PatternLength = ARRAYCOUNT(PatternGround);
	ASSERT(ARRAYCOUNT(PatternGround) == ARRAYCOUNT(PatternBeach));
	ASSERT(ARRAYCOUNT(PatternGround) == ARRAYCOUNT(PatternOcean));
	
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Height = cChunkDef::GetHeight(a_HeightMap, x, z);
			const BLOCKTYPE * Pattern;
			if (Height > m_SeaLevel + m_BeachHeight)
			{
				Pattern = PatternGround;
			}
			else if (Height > m_SeaLevel - m_BeachDepth)
			{
				Pattern = PatternBeach;
			}
			else
			{
				Pattern = PatternOcean;
			}
			
			// Fill water from sealevel down to height (if any):
			for (int y = m_SeaLevel; y >= Height; --y)
			{
				cChunkDef::SetBlock(a_BlockTypes, x, y, z, m_BlockSea);
			}
			
			// Fill from height till the bottom:
			for (int y = Height; y >= 1; y--)
			{
				cChunkDef::SetBlock(a_BlockTypes, x, y, z, (Height - y < PatternLength) ? Pattern[Height - y] : m_BlockBottom);
			}
			
			// The last layer is always bedrock:
			cChunkDef::SetBlock(a_BlockTypes, x, 0, z, E_BLOCK_BEDROCK);
		}  // for x
	}  // for z
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenBiomal:

void cCompoGenBiomal::ComposeTerrain(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
	cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
	const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
	const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
	cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
	cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
)
{
	memset(a_BlockTypes, 0, sizeof(a_BlockTypes));
	memset(a_BlockMeta,  0, sizeof(a_BlockMeta));
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Height = cChunkDef::GetHeight(a_HeightMap, x, z);
			if (Height > m_SeaLevel)
			{
				switch (cChunkDef::GetBiome(a_BiomeMap, x, z))
				{
					case biOcean:
					case biPlains:
					case biExtremeHills:
					case biForest:
					case biTaiga:
					case biSwampland:
					case biRiver:
					case biFrozenOcean:
					case biFrozenRiver:
					case biIcePlains:
					case biIceMountains:
					case biForestHills:
					case biTaigaHills:
					case biExtremeHillsEdge:
					case biJungle:
					case biJungleHills:
					{
						FillColumnGrass(x, z, Height, a_BlockTypes);
						break;
					}
					case biDesertHills:
					case biDesert:
					case biBeach:
					{
						FillColumnSand(x, z, Height, a_BlockTypes);
						break;
					}
					case biMushroomIsland:
					case biMushroomShore:
					{
						FillColumnMycelium(x, z, Height, a_BlockTypes);
						break;
					}
					default:
					{
						// TODO
						ASSERT(!"CompoGenBiomal: Biome not implemented yet!");
						break;
					}
				}
			}
			else
			{
				switch (cChunkDef::GetBiome(a_BiomeMap, x, z))
				{
					case biDesert:
					case biBeach:
					{
						// Fill with water, sand, sandstone and stone
						FillColumnWaterSand(x, z, Height, a_BlockTypes);
						break;
					}
					default:
					{
						// Fill with water, sand/dirt/clay mix and stone
						FillColumnWaterMix(a_ChunkX, a_ChunkZ, x, z, Height, a_BlockTypes);
						break;
					}
				}  // switch (biome)
			}  // else (under water)
			cChunkDef::SetBlock(a_BlockTypes, x, 0, z, E_BLOCK_BEDROCK);
		}  // for x
	}  // for z
}





void cCompoGenBiomal::FillColumnGrass(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes)
{
	BLOCKTYPE Pattern[] =
	{
		E_BLOCK_GRASS,
		E_BLOCK_DIRT,
		E_BLOCK_DIRT,
		E_BLOCK_DIRT,
	} ;
	FillColumnPattern(a_RelX, a_RelZ, a_Height, a_BlockTypes, Pattern, ARRAYCOUNT(Pattern));
	
	for (int y = a_Height - ARRAYCOUNT(Pattern); y > 0; y--)
	{
		cChunkDef::SetBlock(a_BlockTypes, a_RelX, y, a_RelZ, E_BLOCK_STONE);
	}
}





void cCompoGenBiomal::FillColumnSand(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes)
{
	BLOCKTYPE Pattern[] =
	{
		E_BLOCK_SAND,
		E_BLOCK_SAND,
		E_BLOCK_SAND,
		E_BLOCK_SANDSTONE,
	} ;
	FillColumnPattern(a_RelX, a_RelZ, a_Height, a_BlockTypes, Pattern, ARRAYCOUNT(Pattern));
	
	for (int y = a_Height - ARRAYCOUNT(Pattern); y > 0; y--)
	{
		cChunkDef::SetBlock(a_BlockTypes, a_RelX, y, a_RelZ, E_BLOCK_STONE);
	}
}






void cCompoGenBiomal::FillColumnMycelium (int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes)
{
	BLOCKTYPE Pattern[] =
	{
		E_BLOCK_MYCELIUM,
		E_BLOCK_DIRT,
		E_BLOCK_DIRT,
		E_BLOCK_DIRT,
	} ;
	FillColumnPattern(a_RelX, a_RelZ, a_Height, a_BlockTypes, Pattern, ARRAYCOUNT(Pattern));
	
	for (int y = a_Height - ARRAYCOUNT(Pattern); y > 0; y--)
	{
		cChunkDef::SetBlock(a_BlockTypes, a_RelX, y, a_RelZ, E_BLOCK_STONE);
	}
}





void cCompoGenBiomal::FillColumnWaterSand(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes)
{
	FillColumnSand(a_RelX, a_RelZ, a_Height, a_BlockTypes);
	for (int y = a_Height + 1; y <= m_SeaLevel + 1; y++)
	{
		cChunkDef::SetBlock(a_BlockTypes, a_RelX, y, a_RelZ, E_BLOCK_STATIONARY_WATER);
	}
}





void cCompoGenBiomal::FillColumnWaterMix(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes)
{
	if (m_Noise.CubicNoise2D(0.5f * (cChunkDef::Width * a_ChunkX + a_RelX), 0.5f * (cChunkDef::Width * a_ChunkZ + a_RelZ)) < 0)
	{
		FillColumnWaterSand(a_RelX, a_RelZ, a_Height, a_BlockTypes);
	}
	else
	{
		// Dirt
		BLOCKTYPE Pattern[] =
		{
			E_BLOCK_DIRT,
			E_BLOCK_DIRT,
			E_BLOCK_DIRT,
			E_BLOCK_DIRT,
		} ;
		FillColumnPattern(a_RelX, a_RelZ, a_Height, a_BlockTypes, Pattern, ARRAYCOUNT(Pattern));
	
		for (int y = a_Height - ARRAYCOUNT(Pattern); y > 0; y--)
		{
			cChunkDef::SetBlock(a_BlockTypes, a_RelX, y, a_RelZ, E_BLOCK_STONE);
		}
		for (int y = a_Height + 1; y <= m_SeaLevel + 1; y++)
		{
			cChunkDef::SetBlock(a_BlockTypes, a_RelX, y, a_RelZ, E_BLOCK_STATIONARY_WATER);
		}
	}
}






void cCompoGenBiomal::FillColumnPattern(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes, const BLOCKTYPE * a_Pattern, int a_PatternSize)
{
	for (int y = a_Height, idx = 0; (y >= 0) && (idx < a_PatternSize); y--, idx++)
	{
		cChunkDef::SetBlock(a_BlockTypes, a_RelX, y, a_RelZ, a_Pattern[idx]);
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenNether:

cCompoGenNether::cCompoGenNether(int a_Seed) :
	m_Noise1(a_Seed + 10),
	m_Noise2(a_Seed * a_Seed * 10 + a_Seed * 1000 + 6000),
	m_Threshold(0)
{
}





void cCompoGenNether::ComposeTerrain(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
	cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
	const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
	const cChunkDef::BiomeMap & a_BiomeMap,    // Biomes to adhere to
	cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
	cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
)
{
	HEIGHTTYPE MaxHeight = 0;
	for (int i = 0; i < ARRAYCOUNT(a_HeightMap); i++)
	{
		if (a_HeightMap[i] > MaxHeight)
		{
			MaxHeight = a_HeightMap[i];
		}
	}
	
	const int SEGMENT_HEIGHT = 8;
	const int INTERPOL_X = 16;  // Must be a divisor of 16
	const int INTERPOL_Z = 16;  // Must be a divisor of 16
	// Interpolate the chunk in 16 * SEGMENT_HEIGHT * 16 "segments", each 8 blocks high and each linearly interpolated separately.
	// Have two buffers, one for the lowest floor and one for the highest floor, so that Y-interpolation can be done between them
	// Then swap the buffers and use the previously-top one as the current-bottom, without recalculating it.
	
	int FloorBuf1[17 * 17];
	int FloorBuf2[17 * 17];
	int * FloorHi = FloorBuf1;
	int * FloorLo = FloorBuf2;
	int BaseX = a_ChunkX * cChunkDef::Width;
	int BaseZ = a_ChunkZ * cChunkDef::Width;
	
	// Interpolate the lowest floor:
	for (int z = 0; z <= 16 / INTERPOL_Z; z++) for (int x = 0; x <= 16 / INTERPOL_X; x++)
	{
		FloorLo[INTERPOL_X * x + 17 * INTERPOL_Z * z] = 
			m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, 0, BaseZ + INTERPOL_Z * z) * 
			m_Noise2.IntNoise3DInt(BaseX + INTERPOL_X * x, 0, BaseZ + INTERPOL_Z * z) /
			256;
	}  // for x, z - FloorLo[]
	IntArrayLinearInterpolate2D(FloorLo, 17, 17, INTERPOL_X, INTERPOL_Z);
	
	// Interpolate segments:
	for (int Segment = 0; Segment < MaxHeight; Segment += SEGMENT_HEIGHT)
	{
		// First update the high floor:
		for (int z = 0; z <= 16 / INTERPOL_Z; z++) for (int x = 0; x <= 16 / INTERPOL_X; x++)
		{
			FloorHi[INTERPOL_X * x + 17 * INTERPOL_Z * z] =
				m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) *
				m_Noise2.IntNoise3DInt(BaseX + INTERPOL_Z * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) /
				256;
		}  // for x, z - FloorLo[]
		IntArrayLinearInterpolate2D(FloorHi, 17, 17, INTERPOL_X, INTERPOL_Z);
		
		// Interpolate between FloorLo and FloorHi:
		for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
		{
			int Lo = FloorLo[x + 17 * z] / 256;
			int Hi = FloorHi[x + 17 * z] / 256;
			for (int y = 0; y < SEGMENT_HEIGHT; y++)
			{
				int Val = Lo + (Hi - Lo) * y / SEGMENT_HEIGHT;
				cChunkDef::SetBlock(a_BlockTypes, x, y + Segment, z, (Val < m_Threshold) ? E_BLOCK_NETHERRACK : E_BLOCK_AIR);
			}
		}
		
		// Swap the floors:
		std::swap(FloorLo, FloorHi);
	}
	
	// Bedrock at the bottom and at the top:
	for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
	{
		cChunkDef::SetBlock(a_BlockTypes, x, 0, z, E_BLOCK_BEDROCK);
		cChunkDef::SetBlock(a_BlockTypes, x, cChunkDef::GetHeight(a_HeightMap, x, z), z, E_BLOCK_BEDROCK);
	}
}




