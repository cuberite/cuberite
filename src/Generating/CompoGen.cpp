
// CompoGen.cpp

/* Implements the various terrain composition generators:
	- cCompoGenSameBlock
	- cCompoGenDebugBiomes
	- cCompoGenClassic
*/

#include "Globals.h"
#include "CompoGen.h"
#include "../Item.h"
#include "../LinearUpscale.h"
#include "../IniFile.h"

#include "../Protocol/Palettes/Upgrade.h"
#include "../Registries/BlockItemConverter.h"





////////////////////////////////////////////////////////////////////////////////
// cCompoGenSameBlock:

void cCompoGenSameBlock::ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape)
{
	a_ChunkDesc.SetHeightFromShape(a_Shape);
	a_ChunkDesc.FillBlocks(Block::Air::Air());
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Start;
			if (m_IsBedrocked)
			{
				a_ChunkDesc.SetBlock({x, 0, z}, Block::Bedrock::Bedrock());
				Start = 1;
			}
			else
			{
				Start = 0;
			}
			for (int y = a_ChunkDesc.GetHeight(x, z); y >= Start; y--)
			{
				a_ChunkDesc.SetBlock({x, y, z}, m_Block);
			}  // for y
		}  // for z
	}  // for x
}





void cCompoGenSameBlock::InitializeCompoGen(cIniFile & a_IniFile)
{
	auto Item = GetIniItemSet(a_IniFile, "Generator", "SameBlockType", "stone");

	m_Block = BlockItemConverter::FromItem(Item.m_ItemType);
	m_IsBedrocked = (a_IniFile.GetValueSetI("Generator", "SameBlockBedrocked", 1) != 0);
}





////////////////////////////////////////////////////////////////////////////////
// cCompoGenDebugBiomes:

void cCompoGenDebugBiomes::ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape)
{
	static std::array<BlockState, 23> Blocks =
	{
		Block::Stone::Stone(),
		Block::Cobblestone::Cobblestone(),
		Block::OakLog::OakLog(),
		Block::OakPlanks::OakPlanks(),
		Block::Sandstone::Sandstone(),
		Block::WhiteWool::WhiteWool(),
		Block::CoalOre::CoalOre(),
		Block::IronOre::IronOre(),
		Block::GoldOre::GoldOre(),
		Block::DiamondOre::DiamondOre(),
		Block::LapisOre::LapisOre(),
		Block::RedstoneOre::RedstoneOre(),
		Block::IronBlock::IronBlock(),
		Block::GoldBlock::GoldBlock(),
		Block::DiamondBlock::DiamondBlock(),
		Block::LapisBlock::LapisBlock(),
		Block::Bricks::Bricks(),
		Block::MossyCobblestone::MossyCobblestone(),
		Block::Obsidian::Obsidian(),
		Block::Netherrack::Netherrack(),
		Block::SoulSand::SoulSand(),
		Block::NetherBricks::NetherBricks(),
		Block::Bedrock::Bedrock(),
	} ;

	a_ChunkDesc.SetHeightFromShape(a_Shape);
	a_ChunkDesc.FillBlocks(Block::Air::Air());

	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			auto Block = Blocks[static_cast<size_t>(a_ChunkDesc.GetBiome(x, z))];
			for (int y = a_ChunkDesc.GetHeight(x, z); y >= 0; y--)
			{
				a_ChunkDesc.SetBlock({x, y, z}, Block);
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
	m_BlockTop(Block::GrassBlock::GrassBlock()),
	m_BlockMiddle(Block::Dirt::Dirt()),
	m_BlockBottom(Block::Stone::Stone()),
	m_BlockBeach(Block::Sand::Sand()),
	m_BlockBeachBottom(Block::Sandstone::Sandstone()),
	m_BlockSea(Block::Water::Water())
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

	a_ChunkDesc.FillBlocks(Block::Air::Air());
	a_ChunkDesc.SetHeightFromShape(a_Shape);

	// The patterns to use for different situations, must be same length!
	const std::array<BlockState, 4> PatternGround = {m_BlockTop,    m_BlockMiddle, m_BlockMiddle, m_BlockMiddle} ;
	const std::array<BlockState, 4> PatternBeach  = {m_BlockBeach,  m_BlockBeach,  m_BlockBeach,  m_BlockBeachBottom} ;
	const std::array<BlockState, 4> PatternOcean  = {m_BlockMiddle, m_BlockMiddle, m_BlockMiddle, m_BlockBottom} ;
	static size_t PatternLength = PatternGround.size();
	ASSERT(PatternGround.size() == PatternBeach.size());
	ASSERT(PatternGround.size() == PatternOcean.size());

	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			auto Height = a_ChunkDesc.GetHeight(x, z);
			const std::array<BlockState, 4> * Pattern;
			if (Height > m_SeaLevel + m_BeachHeight)
			{
				Pattern = &PatternGround;
			}
			else if (Height > m_SeaLevel - m_BeachDepth)
			{
				Pattern = &PatternBeach;
			}
			else
			{
				Pattern = &PatternOcean;
			}

			// Fill water from sealevel down to height (if any):
			for (int y = m_SeaLevel; y >= Height; --y)
			{
				a_ChunkDesc.SetBlock({x, y, z}, m_BlockSea);
			}

			// Fill from height till the bottom:
			for (int y = Height; y >= 1; y--)
			{
				a_ChunkDesc.SetBlock({x, y, z}, (Height - y < static_cast<int>(PatternLength)) ? Pattern->at(static_cast<size_t>(Height - y)) : m_BlockBottom);
			}

			// The last layer is always bedrock:
			a_ChunkDesc.SetBlock({x, 0, z}, Block::Bedrock::Bedrock());
		}  // for x
	}  // for z
}





void cCompoGenClassic::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_SeaLevel         = a_IniFile.GetValueSetI("Generator", "SeaLevel",           m_SeaLevel);
	m_BeachHeight      = a_IniFile.GetValueSetI("Generator", "ClassicBeachHeight", m_BeachHeight);
	m_BeachDepth       = a_IniFile.GetValueSetI("Generator", "ClassicBeachDepth",  m_BeachDepth);

	auto BlockTop         = GetIniItemSet(a_IniFile, "Generator", "ClassicBlockTop",         "grass");
	auto BlockMiddle      = GetIniItemSet(a_IniFile, "Generator", "ClassicBlockMiddle",      "dirt");
	auto BlockBottom      = GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBottom",      "stone");
	auto BlockBeach       = GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBeach",       "sand");
	auto BlockBeachBottom = GetIniItemSet(a_IniFile, "Generator", "ClassicBlockBeachBottom", "sandstone");
	auto BlockSea         = GetIniItemSet(a_IniFile, "Generator", "ClassicBlockSea",         "water");


	m_BlockTop         = BlockItemConverter::FromItem(BlockTop.m_ItemType);
	m_BlockMiddle      = BlockItemConverter::FromItem(BlockMiddle.m_ItemType);
	m_BlockBottom      = BlockItemConverter::FromItem(BlockBottom.m_ItemType);
	m_BlockBeach       = BlockItemConverter::FromItem(BlockBeach.m_ItemType);
	m_BlockBeachBottom = BlockItemConverter::FromItem(BlockBeachBottom.m_ItemType);
	m_BlockSea         = BlockItemConverter::FromItem(BlockSea.m_ItemType);
}





////////////////////////////////////////////////////////////////////////////////
// cCompoGenNether:

cCompoGenNether::cCompoGenNether(int a_Seed) :
	m_Noise1(a_Seed + 10),
	m_Noise2(a_Seed * a_Seed * 10 + a_Seed * 1000 + 6000),
	m_MaxThreshold(25000)
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
		// We need to store the intermediate result in a volatile variable, otherwise gcc -O2 optimizes
		// through the undefined behavior in cNoise and produces different data than the other platforms / build types (#4384)
		volatile int intermediate =
			m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, 0, BaseZ + INTERPOL_Z * z) *
			m_Noise2.IntNoise3DInt(BaseX + INTERPOL_X * x, 0, BaseZ + INTERPOL_Z * z);
		FloorLo[INTERPOL_X * x + 17 * INTERPOL_Z * z] = intermediate / 256;
	}  // for x, z - FloorLo[]
	LinearUpscale2DArrayInPlace<17, 17, INTERPOL_X, INTERPOL_Z>(FloorLo);

	// Interpolate segments:
	for (int Segment = 0; Segment < MaxHeight; Segment += SEGMENT_HEIGHT)
	{
		// First update the high floor:
		for (int z = 0; z <= 16 / INTERPOL_Z; z++) for (int x = 0; x <= 16 / INTERPOL_X; x++)
		{
			// We need to store the intermediate result in a volatile variable, otherwise gcc -O2 optimizes
			// through the undefined behavior in cNoise and produces different data than the other platforms / build types (#4384)
			volatile int intermediate =
				m_Noise1.IntNoise3DInt(BaseX + INTERPOL_X * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z) *
				m_Noise2.IntNoise3DInt(BaseX + INTERPOL_Z * x, Segment + SEGMENT_HEIGHT, BaseZ + INTERPOL_Z * z);
			FloorHi[INTERPOL_X * x + 17 * INTERPOL_Z * z] = intermediate / 256;
		}  // for x, z - FloorLo[]
		LinearUpscale2DArrayInPlace<17, 17, INTERPOL_X, INTERPOL_Z>(FloorHi);

		// Interpolate between FloorLo and FloorHi:
		for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
		{
			int Threshold = static_cast<int>(m_Noise1.CubicNoise2D(static_cast<float>(BaseX + x) / 75, static_cast<float>(BaseZ + z) / 75) * m_MaxThreshold);
			int Lo = FloorLo[x + 17 * z] / 256;
			int Hi = FloorHi[x + 17 * z] / 256;
			for (int y = 0; y < SEGMENT_HEIGHT; y++)
			{
				int Val = Lo + (Hi - Lo) * y / SEGMENT_HEIGHT;
				if (Val < Threshold)
				{
					a_ChunkDesc.SetBlock({x, y + Segment, z}, Block::Netherrack::Netherrack());
				}
			}
		}

		// Swap the floors:
		std::swap(FloorLo, FloorHi);
	}

	// Bedrock at the bottom and at the top, cover ceiling with netherrack:
	for (int z = 0; z < 16; z++) for (int x = 0; x < 16; x++)
	{
		a_ChunkDesc.SetBlock({x, 0, z}, Block::Bedrock::Bedrock());

		int Height = a_ChunkDesc.GetHeight(x, z);
		a_ChunkDesc.SetBlock({x, Height, z}, Block::Bedrock::Bedrock());

		NOISE_DATATYPE CeilingDisguise = (m_Noise1.CubicNoise2D(static_cast<float>(a_ChunkDesc.GetChunkX() * cChunkDef::Width + x) / 10, static_cast<float>(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z) / 10));
		if (CeilingDisguise < 0)
		{
			CeilingDisguise = -CeilingDisguise;
		}

		int CeilingDisguiseHeight = Height - 2 - FloorC(CeilingDisguise * 3);

		for (int y = Height - 1; y > CeilingDisguiseHeight; y--)
		{
			a_ChunkDesc.SetBlock({x, y, z}, Block::Netherrack::Netherrack());
		}
	}
}





void cCompoGenNether::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_MaxThreshold = a_IniFile.GetValueSetF("Generator", "NetherMaxThreshold", m_MaxThreshold);
}





////////////////////////////////////////////////////////////////////////////////
// cCompoGenCache:

cCompoGenCache::cCompoGenCache(std::unique_ptr<cTerrainCompositionGen> a_Underlying, int a_CacheSize) :
	m_Underlying(std::move(a_Underlying)),
	m_CacheSize(a_CacheSize),
	m_CacheOrder(new int[ToUnsigned(a_CacheSize)]),
	m_CacheData(new sCacheData[ToUnsigned(a_CacheSize)]),
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
	#ifndef NDEBUG
	if (((m_NumHits + m_NumMisses) % 1024) == 10)
	{
		// LOGD("CompoGenCache: %d hits, %d misses, saved %.2f %%", m_NumHits, m_NumMisses, 100.0 * m_NumHits / (m_NumHits + m_NumMisses));
		// LOGD("CompoGenCache: Avg cache chain length: %.2f", static_cast<float>(m_TotalChain) / m_NumHits);
	}
	#endif  // !NDEBUG

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
		memcpy(a_ChunkDesc.GetBlocks(),             m_CacheData[Idx].m_BlockTypes,        sizeof(a_ChunkDesc.GetBlocks()));
		memcpy(a_ChunkDesc.GetHeightMap().data(),   m_CacheData[Idx].m_HeightMap.data(),  sizeof(a_ChunkDesc.GetHeightMap()));

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
	memcpy(m_CacheData[Idx].m_BlockTypes,        a_ChunkDesc.GetBlocks(),           sizeof(a_ChunkDesc.GetBlocks()));
	memcpy(m_CacheData[Idx].m_HeightMap.data(),  a_ChunkDesc.GetHeightMap().data(), sizeof(a_ChunkDesc.GetHeightMap()));
	m_CacheData[Idx].m_ChunkX = ChunkX;
	m_CacheData[Idx].m_ChunkZ = ChunkZ;
}





void cCompoGenCache::InitializeCompoGen(cIniFile & a_IniFile)
{
	m_Underlying->InitializeCompoGen(a_IniFile);
}
