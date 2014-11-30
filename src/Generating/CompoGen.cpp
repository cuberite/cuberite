
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
#include "../IniFile.h"





////////////////////////////////////////////////////////////////////////////////
// cCompoGenSameBlock:

void cCompoGenSameBlock::ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape)
{
	a_ChunkDesc.SetHeightFromShape(a_Shape);
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





////////////////////////////////////////////////////////////////////////////////
// cCompoGenDebugBiomes:

void cCompoGenDebugBiomes::ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape)
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
	
	a_ChunkDesc.SetHeightFromShape(a_Shape);
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





////////////////////////////////////////////////////////////////////////////////
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





void cCompoGenClassic::ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape)
{
	/* The classic composition means:
		- 1 layer of grass, 3 of dirt and the rest stone, if the height > sealevel + beachheight
		- 3 sand and a 1 sandstone, rest stone if between sealevel and sealevel + beachheight
		- water from waterlevel to height, then 3 sand, 1 sandstone, the rest stone, if water depth < beachdepth
		- water from waterlevel, then 3 dirt, the rest stone otherwise
		- bedrock at the bottom
	*/

	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);
	a_ChunkDesc.SetHeightFromShape(a_Shape);

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
	m_SeaLevel         = a_IniFile.GetValueSetI("Generator", "SeaLevel",           m_SeaLevel);
	m_BeachHeight      = a_IniFile.GetValueSetI("Generator", "ClassicBeachHeight", m_BeachHeight);
	m_BeachDepth       = a_IniFile.GetValueSetI("Generator", "ClassicBeachDepth",  m_BeachDepth);
	m_BlockTop         = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockTop",         "grass").m_ItemType);
	m_BlockMiddle      = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockMiddle",      "dirt").m_ItemType);
	m_BlockBottom      = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBottom",      "stone").m_ItemType);
	m_BlockBeach       = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBeach",       "sand").m_ItemType);
	m_BlockBeachBottom = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBeachBottom", "sandstone").m_ItemType);
	m_BlockSea         = (BLOCKTYPE)(GetIniItemSet(a_IniFile, "Generator", "ClassicBlockSea",         "stationarywater").m_ItemType);
}





////////////////////////////////////////////////////////////////////////////////
// cCompoGenNether:

cCompoGenNether::cCompoGenNether(int a_Seed) :
	m_Noise1(a_Seed + 10),
	m_Noise2(a_Seed * a_Seed * 10 + a_Seed * 1000 + 6000),
	m_Threshold(0)
{
}





void cCompoGenNether::ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape)
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
		//*
		FloorLo[INTERPOL_X * x + 17 * INTERPOL_Z * z] =
			m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, 0, BaseZ + INTERPOL_Z * z) *
			m_Noise2.IntNoise3DInt(BaseX + INTERPOL_X * x, 0, BaseZ + INTERPOL_Z * z) /
			256;
		//*/
		/*
		FloorLo[INTERPOL_X * x + 17 * INTERPOL_Z * z] =
			m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, 0, BaseZ + INTERPOL_Z * z) / 256;
		//*/
	}  // for x, z - FloorLo[]
	LinearUpscale2DArrayInPlace<17, 17, INTERPOL_X, INTERPOL_Z>(FloorLo);
	
	// Interpolate segments:
	for (int Segment = 0; Segment < MaxHeight; Segment += SEGMENT_HEIGHT)
	{
		// First update the high floor:
		for (int z = 0; z <= 16 / INTERPOL_Z; z++) for (int x = 0; x <= 16 / INTERPOL_X; x++)
		{
			//*
			FloorHi[INTERPOL_X * x + 17 * INTERPOL_Z * z] =
				m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) *
				m_Noise2.IntNoise3DInt(BaseX + INTERPOL_Z * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) /
				256;
			//*/
			/*
			FloorHi[INTERPOL_X * x + 17 * INTERPOL_Z * z] =
				m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) / 256;
			//*/
		}  // for x, z - FloorLo[]
		LinearUpscale2DArrayInPlace<17, 17, INTERPOL_X, INTERPOL_Z>(FloorHi);
		
		// Interpolate between FloorLo and FloorHi:
		for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
		{
			int Lo = FloorLo[x + 17 * z] / 256;
			int Hi = FloorHi[x + 17 * z] / 256;
			for (int y = 0; y < SEGMENT_HEIGHT; y++)
			{
				int Val = Lo + (Hi - Lo) * y / SEGMENT_HEIGHT;
				BLOCKTYPE Block = E_BLOCK_AIR;
				if (Val < m_Threshold)  // It's a solid block
				{
					Block = E_BLOCK_NETHERRACK;
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
		// Place soulsand rims when netherrack gets thin
		for (int y = 0; y < MaxHeight; y++)
		{
			// The current block is air. Lets bail ut.
			BLOCKTYPE Block = a_ChunkDesc.GetBlockType(x, y, z);
			if (Block == E_BLOCK_AIR)
			{
				continue;
			}

			// Check how many blocks there are above the current block. Don't go higher than 2 blocks, because we already bail out if that's the case.
			int NumBlocksAbove = 0;
			for (int I = y + 1; I <= y + 2; I++)
			{
				if (a_ChunkDesc.GetBlockType(x, Clamp(I, 0, cChunkDef::Height), z) != E_BLOCK_AIR)
				{
					NumBlocksAbove++;
				}
				else
				{
					break;
				}
			}

			// There are too many blocks above the current block.
			if (NumBlocksAbove == 2)
			{
				continue;
			}

			// Check how many blocks there below the current block. Don't go lower than 2 blocks, because we already bail out if that's the case.
			int NumBlocksBelow = 0;
			for (int I = y - 1; I >= y - 2; I--)
			{
				if (a_ChunkDesc.GetBlockType(x, Clamp(I, 0, cChunkDef::Height), z) != E_BLOCK_AIR)
				{
					NumBlocksBelow++;
				}
				else
				{
					break;
				}
			}

			// There are too many blocks below the current block
			if (NumBlocksBelow == 2)
			{
				continue;
			}

			NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(BaseX + x)) / 32;
			NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)(BaseZ + z)) / 32;
			NOISE_DATATYPE CompBlock = m_Noise1.CubicNoise3D(NoiseX, (float) (y) / 4, NoiseY);
			if (CompBlock < 0)
			{
				a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_SOULSAND);
			}
		}

		// Place bedrock at the bottom
		a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);

		// Place bedrock at the top
		int Height = a_ChunkDesc.GetHeight(x, z);
		a_ChunkDesc.SetBlockType(x, Height, z, E_BLOCK_BEDROCK);

		// Place netherrack to disguise the top bedrock.
		NOISE_DATATYPE CeilingDisguise = (m_Noise1.CubicNoise2D((float)(a_ChunkDesc.GetChunkX() * cChunkDef::Width + x) / 10, (float)(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z) / 10));
		if (CeilingDisguise < 0)
		{
			CeilingDisguise = -CeilingDisguise;
		}

		int CeilingDisguiseHeight = Height - 2 - FloorC(CeilingDisguise * 3);

		for (int y = Height - 1; y > CeilingDisguiseHeight; y--)
		{
			a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_NETHERRACK);
		}
	}
}





void cCompoGenNether::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_Threshold = a_IniFile.GetValueSetI("Generator", "NetherThreshold", m_Threshold);
}





////////////////////////////////////////////////////////////////////////////////
// cCompoGenCache:

cCompoGenCache::cCompoGenCache(cTerrainCompositionGenPtr a_Underlying, int a_CacheSize) :
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
	m_CacheData = nullptr;
	delete[] m_CacheOrder;
	m_CacheOrder = nullptr;
}





void cCompoGenCache::ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape)
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
		memcpy(a_ChunkDesc.GetHeightMap(),              m_CacheData[Idx].m_HeightMap,  sizeof(a_ChunkDesc.GetHeightMap()));
		
		m_NumHits++;
		m_TotalChain += i;
		return;
	}  // for i - cache
	
	// Not in the cache:
	m_NumMisses++;
	m_Underlying->ComposeTerrain(a_ChunkDesc, a_Shape);
	
	// Insert it as the first item in the MRU order:
	int Idx = m_CacheOrder[m_CacheSize - 1];
	for (int i = m_CacheSize - 1; i > 0; i--)
	{
		m_CacheOrder[i] = m_CacheOrder[i - 1];
	}  // for i - m_CacheOrder[]
	m_CacheOrder[0] = Idx;
	memcpy(m_CacheData[Idx].m_BlockTypes, a_ChunkDesc.GetBlockTypes(),             sizeof(a_ChunkDesc.GetBlockTypes()));
	memcpy(m_CacheData[Idx].m_BlockMetas, a_ChunkDesc.GetBlockMetasUncompressed(), sizeof(a_ChunkDesc.GetBlockMetasUncompressed()));
	memcpy(m_CacheData[Idx].m_HeightMap,  a_ChunkDesc.GetHeightMap(),              sizeof(a_ChunkDesc.GetHeightMap()));
	m_CacheData[Idx].m_ChunkX = ChunkX;
	m_CacheData[Idx].m_ChunkZ = ChunkZ;
}





void cCompoGenCache::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_Underlying->InitializeCompoGen(a_IniFile);
}




