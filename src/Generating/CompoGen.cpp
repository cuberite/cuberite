
// CompoGen.cpp

/* Implements the various terrain composition generators:
	- cCompoGenSameBlock
	- cCompoGenDebugBiomes
	- cCompoGenClassic
*/

#include "Globals.h"
#include "CompoGen.h"
#include "../BlockID.h"
#include "../Item.h"
#include "../LinearUpscale.h"
#include "inifile/iniFile.h"





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





void cCompoGenSameBlock::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_BlockType = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "SameBlockType", "stone").m_ItemType);
	m_IsBedrocked = (a_IniFile.GetValueSetI("Generator", "SameBlockBedrocked", 1) != 0);
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

cCompoGenClassic::cCompoGenClassic(void) :
	m_SeaLevel(60),
	m_BeachHeight(2),
	m_BeachDepth(4),
	m_BlockTop(E_BLOCK_GRASS),
	m_BlockMiddle(E_BLOCK_DIRT),
	m_BlockBottom(E_BLOCK_STONE),
	m_BlockBeach(E_BLOCK_SAND),
	m_BlockBeachBottom(E_BLOCK_SANDSTONE),
	m_BlockSea(E_BLOCK_STATIONARY_WATER)
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





void cCompoGenClassic::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_SeaLevel         = a_IniFile.GetValueSetI("Generator", "ClassicSeaLevel",    m_SeaLevel);
	m_BeachHeight      = a_IniFile.GetValueSetI("Generator", "ClassicBeachHeight", m_BeachHeight);
	m_BeachDepth       = a_IniFile.GetValueSetI("Generator", "ClassicBeachDepth",  m_BeachDepth);
	m_BlockTop         = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockTop",         "grass").m_ItemType);
	m_BlockMiddle      = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockMiddle",      "dirt").m_ItemType);
	m_BlockBottom      = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBottom",      "stone").m_ItemType);
	m_BlockBeach       = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBeach",       "sand").m_ItemType);
	m_BlockBeachBottom = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBeachBottom", "sandstone").m_ItemType);
	m_BlockSea         = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockSea",         "stationarywater").m_ItemType);
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
					case biJungleEdge:
					case biDeepOcean:
					case biStoneBeach:
					case biColdBeach:
					case biBirchForest:
					case biBirchForestHills:
					case biRoofedForest:
					case biColdTaiga:
					case biColdTaigaHills:
					case biExtremeHillsPlus:
					case biSavanna:
					case biSavannaPlateau:
					case biSunflowerPlains:
					case biExtremeHillsM:
					case biFlowerForest:
					case biTaigaM:
					case biSwamplandM:
					case biIcePlainsSpikes:
					case biJungleM:
					case biJungleEdgeM:
					case biBirchForestM:
					case biBirchForestHillsM:
					case biRoofedForestM:
					case biColdTaigaM:
					case biExtremeHillsPlusM:
					case biSavannaM:
					case biSavannaPlateauM:
					{
						FillColumnGrass(x, z, Height, a_ChunkDesc.GetBlockTypes());
						break;
					}

					case biMesa:
					case biMesaPlateauF:
					case biMesaPlateau:
					case biMesaBryce:
					case biMesaPlateauFM:
					case biMesaPlateauM:
					{
						FillColumnClay(x, z, Height, a_ChunkDesc.GetBlockTypes());
						break;
					}
					
					case biMegaTaiga:
					case biMegaTaigaHills:
					case biMegaSpruceTaiga:
					case biMegaSpruceTaigaHills:
					{
						FillColumnDirt(x, z, Height, a_ChunkDesc.GetBlockTypes());
						break;
					}

					case biDesertHills:
					case biDesert:
					case biDesertM:
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
				a_ChunkDesc.SetHeight(x, z, m_SeaLevel + 1);
			}  // else (under water)
			a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
		}  // for x
	}  // for z
}





void cCompoGenBiomal::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_SeaLevel = a_IniFile.GetValueSetI("Generator", "BiomalSeaLevel", m_SeaLevel) - 1;
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





void cCompoGenBiomal::FillColumnClay(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes)
{
	BLOCKTYPE Pattern[] =
	{
		E_BLOCK_HARDENED_CLAY,
		E_BLOCK_HARDENED_CLAY,
		E_BLOCK_HARDENED_CLAY,
		E_BLOCK_HARDENED_CLAY,
	} ;
	FillColumnPattern(a_RelX, a_RelZ, a_Height, a_BlockTypes, Pattern, ARRAYCOUNT(Pattern));
	
	for (int y = a_Height - ARRAYCOUNT(Pattern); y > 0; y--)
	{
		cChunkDef::SetBlock(a_BlockTypes, a_RelX, y, a_RelZ, E_BLOCK_STONE);
	}
}





void cCompoGenBiomal::FillColumnDirt(int a_RelX, int a_RelZ, int a_Height, cChunkDef::BlockTypes & a_BlockTypes)
{
	for (int y = 0; y < 4; y++)
	{
		if (a_Height - y < 0)
		{
			return;
		}
		cChunkDef::SetBlock(a_BlockTypes, a_RelX, a_Height - y, a_RelZ, E_BLOCK_DIRT);
	}
	for (int y = a_Height - 4; y > 0; y--)
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
	LinearUpscale2DArrayInPlace(FloorLo, 17, 17, INTERPOL_X, INTERPOL_Z);
	
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
		LinearUpscale2DArrayInPlace(FloorHi, 17, 17, INTERPOL_X, INTERPOL_Z);
		
		// Interpolate between FloorLo and FloorHi:
		for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
		{
			int Lo = FloorLo[x + 17 * z] / 256;
			int Hi = FloorHi[x + 17 * z] / 256;
			for (int y = 0; y < SEGMENT_HEIGHT; y++)
			{
				int Val = Lo + (Hi - Lo) * y / SEGMENT_HEIGHT;
				BLOCKTYPE Block = E_BLOCK_AIR;
				if (Val < m_Threshold)  // Don't calculate if the block should be Netherrack or Soulsand when it's already decided that it's air.
				{
					NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(BaseX + x)) / 8;
					NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)(BaseZ + z)) / 8;
					NOISE_DATATYPE CompBlock = m_Noise1.CubicNoise3D(NoiseX, (float) (y + Segment) / 2, NoiseY);
					if (CompBlock < -0.5)
					{
						Block = E_BLOCK_SOULSAND;
					}
					else
					{
						Block = E_BLOCK_NETHERRACK;
					}
				}
				a_ChunkDesc.SetBlockType(x, y + Segment, z, Block);
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





void cCompoGenNether::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_Threshold = a_IniFile.GetValueSetI("Generator", "NetherThreshold", m_Threshold);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenCache:

cCompoGenCache::cCompoGenCache(cTerrainCompositionGen & a_Underlying, int a_CacheSize) :
	m_Underlying(a_Underlying),
	m_CacheSize(a_CacheSize),
	m_CacheOrder(new int[a_CacheSize]),
	m_CacheData(new sCacheData[a_CacheSize]),
	m_NumHits(0),
	m_NumMisses(0),
	m_TotalChain(0)
{
	for (int i = 0; i < m_CacheSize; i++)
	{
		m_CacheOrder[i] = i;
		m_CacheData[i].m_ChunkX = 0x7fffffff;
		m_CacheData[i].m_ChunkZ = 0x7fffffff;
	}
}





cCompoGenCache::~cCompoGenCache()
{
	delete[] m_CacheData;
	delete[] m_CacheOrder;
}





void cCompoGenCache::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	#ifdef _DEBUG
	if (((m_NumHits + m_NumMisses) % 1024) == 10)
	{
		LOGD("CompoGenCache: %d hits, %d misses, saved %.2f %%", m_NumHits, m_NumMisses, 100.0 * m_NumHits / (m_NumHits + m_NumMisses));
		LOGD("CompoGenCache: Avg cache chain length: %.2f", (float)m_TotalChain / m_NumHits);
	}
	#endif  // _DEBUG
	
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	
	for (int i = 0; i < m_CacheSize; i++)
	{
		if (
			(m_CacheData[m_CacheOrder[i]].m_ChunkX != ChunkX) ||
			(m_CacheData[m_CacheOrder[i]].m_ChunkZ != ChunkZ)
		)
		{
			continue;
		}
		// Found it in the cache
		int Idx = m_CacheOrder[i];
		
		// Move to front:
		for (int j = i; j > 0; j--)
		{
			m_CacheOrder[j] = m_CacheOrder[j - 1];
		}
		m_CacheOrder[0] = Idx;
		
		// Use the cached data:
		memcpy(a_ChunkDesc.GetBlockTypes(),             m_CacheData[Idx].m_BlockTypes, sizeof(a_ChunkDesc.GetBlockTypes()));
		memcpy(a_ChunkDesc.GetBlockMetasUncompressed(), m_CacheData[Idx].m_BlockMetas, sizeof(a_ChunkDesc.GetBlockMetasUncompressed()));
		
		m_NumHits++;
		m_TotalChain += i;
		return;
	}  // for i - cache
	
	// Not in the cache:
	m_NumMisses++;
	m_Underlying.ComposeTerrain(a_ChunkDesc);
	
	// Insert it as the first item in the MRU order:
	int Idx = m_CacheOrder[m_CacheSize - 1];
	for (int i = m_CacheSize - 1; i > 0; i--)
	{
		m_CacheOrder[i] = m_CacheOrder[i - 1];
	}  // for i - m_CacheOrder[]
	m_CacheOrder[0] = Idx;
	memcpy(m_CacheData[Idx].m_BlockTypes, a_ChunkDesc.GetBlockTypes(),             sizeof(a_ChunkDesc.GetBlockTypes()));
	memcpy(m_CacheData[Idx].m_BlockMetas, a_ChunkDesc.GetBlockMetasUncompressed(), sizeof(a_ChunkDesc.GetBlockMetasUncompressed()));
	m_CacheData[Idx].m_ChunkX = ChunkX;
	m_CacheData[Idx].m_ChunkZ = ChunkZ;
}





void cCompoGenCache::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_Underlying.InitializeCompoGen(a_IniFile);
}




