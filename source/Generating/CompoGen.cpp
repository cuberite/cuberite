
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

void cCompoGenSameBlock::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Start;
			if (m_IsBedrocked)
			{
				a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
				Start = 1;
			}
			else
			{
				Start = 0;
			}
			for (int y = a_ChunkDesc.GetHeight(x, z); y >= Start; y--)
			{
				a_ChunkDesc.SetBlockType(x, y, z, m_BlockType);
			}  // for y
		}  // for z
	}  // for x
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenDebugBiomes:

void cCompoGenDebugBiomes::ComposeTerrain(cChunkDesc & a_ChunkDesc)
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
	
	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);

	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			BLOCKTYPE BlockType = Blocks[a_ChunkDesc.GetBiome(x, z)];
			for (int y = a_ChunkDesc.GetHeight(x, z); y >= 0; y--)
			{
				a_ChunkDesc.SetBlockType(x, y, z, BlockType);
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





void cCompoGenClassic::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	/* The classic composition means:
		- 1 layer of grass, 3 of dirt and the rest stone, if the height > sealevel + beachheight
		- 3 sand and a 1 sandstone, rest stone if between sealevel and sealevel + beachheight
		- water from waterlevel to height, then 3 sand, 1 sandstone, the rest stone, if water depth < beachdepth
		- water from waterlevel, then 3 dirt, the rest stone otherwise
		- bedrock at the bottom
	*/

	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);

	// The patterns to use for different situations, must be same length!
	const BLOCKTYPE PatternGround[] = {m_BlockTop,    m_BlockMiddle, m_BlockMiddle, m_BlockMiddle} ;
	const BLOCKTYPE PatternBeach[]  = {m_BlockBeach,  m_BlockBeach,  m_BlockBeach,  m_BlockBeachBottom} ;
	const BLOCKTYPE PatternOcean[]  = {m_BlockMiddle, m_BlockMiddle, m_BlockMiddle, m_BlockBottom} ;
	static int PatternLength = ARRAYCOUNT(PatternGround);
	ASSERT(ARRAYCOUNT(PatternGround) == ARRAYCOUNT(PatternBeach));
	ASSERT(ARRAYCOUNT(PatternGround) == ARRAYCOUNT(PatternOcean));
	
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Height = a_ChunkDesc.GetHeight(x, z);
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
				a_ChunkDesc.SetBlockType(x, y, z, m_BlockSea);
			}
			
			// Fill from height till the bottom:
			for (int y = Height; y >= 1; y--)
			{
				a_ChunkDesc.SetBlockType(x, y, z, (Height - y < PatternLength) ? Pattern[Height - y] : m_BlockBottom);
			}
			
			// The last layer is always bedrock:
			a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
		}  // for x
	}  // for z
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenBiomal:

void cCompoGenBiomal::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);

	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	
	/*
	_X 2013_04_22:
	There's no point in generating the whole cubic noise at once, because the noise values are used in
	only about 20 % of the cases, so the speed gained by precalculating is lost by precalculating too much data
	*/
	
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Height = a_ChunkDesc.GetHeight(x, z);
			if (Height > m_SeaLevel)
			{
				switch (a_ChunkDesc.GetBiome(x, z))
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
						FillColumnGrass(x, z, Height, a_ChunkDesc.GetBlockTypes());
						break;
					}
					case biDesertHills:
					case biDesert:
					case biBeach:
					{
						FillColumnSand(x, z, Height, a_ChunkDesc.GetBlockTypes());
						break;
					}
					case biMushroomIsland:
					case biMushroomShore:
					{
						FillColumnMycelium(x, z, Height, a_ChunkDesc.GetBlockTypes());
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
				switch (a_ChunkDesc.GetBiome(x, z))
				{
					case biDesert:
					case biBeach:
					{
						// Fill with water, sand, sandstone and stone
						FillColumnWaterSand(x, z, Height, a_ChunkDesc.GetBlockTypes());
						break;
					}
					default:
					{
						// Fill with water, sand/dirt/clay mix and stone
						if (m_Noise.CubicNoise2D(0.3f * (cChunkDef::Width * ChunkX + x), 0.3f * (cChunkDef::Width * ChunkZ + z)) < 0)
						{
							FillColumnWaterSand(x, z, Height, a_ChunkDesc.GetBlockTypes());
						}
						else
						{
							FillColumnWaterDirt(x, z, Height, a_ChunkDesc.GetBlockTypes());
						}
						break;
					}
				}  // switch (biome)
			}  // else (under water)
			a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
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





void cCompoGenBiomal::FillColumnWaterDirt(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes)
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





void cCompoGenNether::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
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
				a_ChunkDesc.SetBlockType(x, y + Segment, z, (Val < m_Threshold) ? E_BLOCK_NETHERRACK : E_BLOCK_AIR);
			}
		}
		
		// Swap the floors:
		std::swap(FloorLo, FloorHi);
	}
	
	// Bedrock at the bottom and at the top:
	for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
	{
		a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
		a_ChunkDesc.SetBlockType(x, a_ChunkDesc.GetHeight(x, z), z, E_BLOCK_BEDROCK);
	}
}




