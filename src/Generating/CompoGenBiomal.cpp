
// CompoGenBiomal.cpp

// Implements the cCompoGenBiomal class representing the biome-aware composition generator

#include "Globals.h"

#include "CompoGenBiomal.h"

#include "../IniFile.h"
#include "../Noise/Noise.h"
#include "../LinearUpscale.h"





////////////////////////////////////////////////////////////////////////////////
// cPattern:

/** This class is used to store a column pattern initialized at runtime,
so that the program doesn't need to explicitly set 256 values for each pattern
Each pattern has 256 blocks so that there's no need to check pattern bounds when assigning the
pattern - there will always be enough pattern left, even for the whole-chunk-height columns. */
class cPattern
{
public:
	struct BlockInfo
	{
		BLOCKTYPE  m_BlockType = E_BLOCK_STONE;
		NIBBLETYPE m_BlockMeta = 0;
	};

	constexpr cPattern(std::initializer_list<BlockInfo> a_TopBlocks)
	{
		ASSERT(a_TopBlocks.size() <= cChunkDef::Height);
		// Copy the pattern into the top:
		size_t i = 0;
		for (const auto & Block : a_TopBlocks)
		{
			m_Pattern[i] = Block;
			++i;
		}

		// The remaining blocks default to stone
	}

	const BlockInfo * Get(void) const { return m_Pattern; }

protected:
	BlockInfo m_Pattern[cChunkDef::Height] = {};
} ;





////////////////////////////////////////////////////////////////////////////////
// Land top block patterns:

static constexpr cPattern patGrass =
{
	{E_BLOCK_GRASS, 0},
	{E_BLOCK_DIRT,  E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT,  E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT,  E_META_DIRT_NORMAL},
} ;

static constexpr cPattern patSand =
{
	{ E_BLOCK_SAND, 0},
	{ E_BLOCK_SAND, 0},
	{ E_BLOCK_SAND, 0},
	{ E_BLOCK_SANDSTONE, 0},
} ;

static constexpr cPattern patDirt =
{
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
} ;

static constexpr cPattern patPodzol =
{
	{E_BLOCK_DIRT, E_META_DIRT_PODZOL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
} ;

static constexpr cPattern patGrassLess =
{
	{E_BLOCK_DIRT, E_META_DIRT_GRASSLESS},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
} ;

static constexpr cPattern patMycelium =
{
	{E_BLOCK_MYCELIUM, 0},
	{E_BLOCK_DIRT,     0},
	{E_BLOCK_DIRT,     0},
	{E_BLOCK_DIRT,     0},
} ;

static constexpr cPattern patGravel =
{
	{E_BLOCK_GRAVEL, 0},
	{E_BLOCK_GRAVEL, 0},
	{E_BLOCK_GRAVEL, 0},
	{E_BLOCK_STONE,  0},
} ;

static constexpr cPattern patStone =
{
	{E_BLOCK_STONE,   0},
	{E_BLOCK_STONE,   0},
	{E_BLOCK_STONE,   0},
	{E_BLOCK_STONE,   0},
} ;



////////////////////////////////////////////////////////////////////////////////
// Ocean floor patterns:

static constexpr cPattern patOFSand =
{
	{E_BLOCK_SAND, 0},
	{E_BLOCK_SAND, 0},
	{E_BLOCK_SAND, 0},
	{E_BLOCK_SANDSTONE, 0}
} ;

static constexpr cPattern patOFClay =
{
	{ E_BLOCK_CLAY, 0},
	{ E_BLOCK_CLAY, 0},
	{ E_BLOCK_SAND, 0},
	{ E_BLOCK_SAND, 0},
} ;

static constexpr cPattern patOFOrangeClay =
{
	{ E_BLOCK_STAINED_CLAY, E_META_STAINED_GLASS_ORANGE},
	{ E_BLOCK_STAINED_CLAY, E_META_STAINED_GLASS_ORANGE},
	{ E_BLOCK_STAINED_CLAY, E_META_STAINED_GLASS_ORANGE},
} ;





////////////////////////////////////////////////////////////////////////////////
// cCompoGenBiomal:

class cCompoGenBiomal :
	public cTerrainCompositionGen
{
public:
	cCompoGenBiomal(int a_Seed) :
		m_SeaLevel(62),
		m_OceanFloorSelect(a_Seed + 1),
		m_MesaFloor(a_Seed + 2)
	{
		initMesaPattern(a_Seed);
	}

protected:
	/** The block height at which water is generated instead of air. */
	HEIGHTTYPE m_SeaLevel;

	/** The pattern used for mesa biomes. Initialized by seed on generator creation. */
	cPattern::BlockInfo m_MesaPattern[2 * cChunkDef::Height];

	/** Noise used for selecting between dirt and sand on the ocean floor. */
	cNoise m_OceanFloorSelect;

	/** Noise used for the floor of the clay blocks in mesa biomes. */
	cNoise m_MesaFloor;


	// cTerrainCompositionGen overrides:
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) override
	{
		a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				ComposeColumn(a_ChunkDesc, x, z, &(a_Shape[x * 256 + z * 16 * 256]));
			}  // for x
		}  // for z
	}



	virtual void InitializeCompoGen(cIniFile & a_IniFile) override
	{
		m_SeaLevel = static_cast<HEIGHTTYPE>(a_IniFile.GetValueSetI("Generator", "SeaLevel", m_SeaLevel));
	}



	/** Initializes the m_MesaPattern with a pattern based on the generator's seed. */
	void initMesaPattern(int a_Seed)
	{
		// In a loop, choose whether to use one, two or three layers of stained clay, then choose a color and width for each layer
		// Separate each group with another layer of hardened clay
		cNoise patternNoise(a_Seed);
		static NIBBLETYPE allowedColors[] =
		{
			E_META_STAINED_CLAY_YELLOW,
			E_META_STAINED_CLAY_YELLOW,
			E_META_STAINED_CLAY_RED,
			E_META_STAINED_CLAY_RED,
			E_META_STAINED_CLAY_WHITE,
			E_META_STAINED_CLAY_BROWN,
			E_META_STAINED_CLAY_BROWN,
			E_META_STAINED_CLAY_BROWN,
			E_META_STAINED_CLAY_ORANGE,
			E_META_STAINED_CLAY_ORANGE,
			E_META_STAINED_CLAY_ORANGE,
			E_META_STAINED_CLAY_ORANGE,
			E_META_STAINED_CLAY_ORANGE,
			E_META_STAINED_CLAY_ORANGE,
			E_META_STAINED_CLAY_LIGHTGRAY,
		} ;
		static int layerSizes[] =  // Adjust the chance so that thinner layers occur more commonly
		{
			1, 1, 1, 1, 1, 1,
			2, 2, 2, 2,
			3, 3,
		} ;
		int idx = ARRAYCOUNT(m_MesaPattern) - 1;
		while (idx >= 0)
		{
			// A layer group of 1 - 2 color stained clay:
			int rnd = patternNoise.IntNoise1DInt(idx) / 7;
			int numLayers = (rnd % 2) + 1;
			rnd /= 2;
			for (int lay = 0; lay < numLayers; lay++)
			{
				int numBlocks = layerSizes[(static_cast<size_t>(rnd) % ARRAYCOUNT(layerSizes))];
				NIBBLETYPE Color = allowedColors[static_cast<size_t>(rnd / 4) % ARRAYCOUNT(allowedColors)];
				if (
					((numBlocks == 3) && (numLayers == 2)) ||  // In two-layer mode disallow the 3-high layers:
					(Color == E_META_STAINED_CLAY_WHITE))      // White stained clay can ever be only 1 block high
				{
					numBlocks = 1;
				}
				numBlocks = std::min(idx + 1, numBlocks);  // Limit by idx so that we don't have to check inside the loop
				rnd /= 32;
				for (int block = 0; block < numBlocks; block++, idx--)
				{
					m_MesaPattern[idx].m_BlockMeta = Color;
					m_MesaPattern[idx].m_BlockType = E_BLOCK_STAINED_CLAY;
				}  // for block
			}  // for lay

			// A layer of hardened clay in between the layer group:
			int numBlocks = (rnd % 4) + 1;  // All heights the same probability
			if ((numLayers == 2) && (numBlocks < 4))
			{
				// For two layers of stained clay, add an extra block of hardened clay:
				numBlocks++;
			}
			numBlocks = std::min(idx + 1, numBlocks);  // Limit by idx so that we don't have to check inside the loop
			for (int block = 0; block < numBlocks; block++, idx--)
			{
				m_MesaPattern[idx].m_BlockMeta = 0;
				m_MesaPattern[idx].m_BlockType = E_BLOCK_HARDENED_CLAY;
			}  // for block
		}  // while (idx >= 0)
	}



	/** Composes a single column in a_ChunkDesc. Chooses what to do based on the biome in that column. */
	void ComposeColumn(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelZ, const Byte * a_ShapeColumn)
	{
		// Frequencies for the podzol floor selecting noise:
		const NOISE_DATATYPE FrequencyX = 8;
		const NOISE_DATATYPE FrequencyZ = 8;

		EMCSBiome Biome = a_ChunkDesc.GetBiome(a_RelX, a_RelZ);
		switch (Biome)
		{
			case biOcean:
			case biPlains:
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
			case biExtremeHillsPlus:
			case biExtremeHills:
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
			case biSavanna:
			case biSavannaPlateau:
			case biSunflowerPlains:
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
			case biSavannaM:
			case biSavannaPlateauM:
			{
				FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patGrass.Get(), a_ShapeColumn);
				return;
			}

			case biMegaTaiga:
			case biMegaTaigaHills:
			case biMegaSpruceTaiga:
			case biMegaSpruceTaigaHills:
			{
				// Select the pattern to use - podzol, grass or grassless dirt:
				NOISE_DATATYPE NoiseX = (static_cast<NOISE_DATATYPE>(a_ChunkDesc.GetChunkX() * cChunkDef::Width + a_RelX)) / FrequencyX;
				NOISE_DATATYPE NoiseY = (static_cast<NOISE_DATATYPE>(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + a_RelZ)) / FrequencyZ;
				NOISE_DATATYPE Val = m_OceanFloorSelect.CubicNoise2D(NoiseX, NoiseY);
				const cPattern::BlockInfo * Pattern = (Val < -0.9) ? patGrassLess.Get() : ((Val > 0) ? patPodzol.Get() : patGrass.Get());
				FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, Pattern, a_ShapeColumn);
				return;
			}

			case biDesertHills:
			case biDesert:
			case biDesertM:
			case biBeach:
			{
				FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patSand.Get(), a_ShapeColumn);
				return;
			}

			case biMushroomIsland:
			case biMushroomShore:
			{
				FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patMycelium.Get(), a_ShapeColumn);
				return;
			}

			case biMesa:
			case biMesaPlateauF:
			case biMesaPlateau:
			case biMesaBryce:
			case biMesaPlateauFM:
			case biMesaPlateauM:
			{
				// Mesa biomes need special handling, because they don't follow the usual "4 blocks from top pattern",
				// instead, they provide a "from bottom" pattern with varying base height,
				// usually 4 blocks below the ocean level
				FillColumnMesa(a_ChunkDesc, a_RelX, a_RelZ, a_ShapeColumn);
				return;
			}

			case biExtremeHillsPlusM:
			case biExtremeHillsM:
			{
				// Select the pattern to use - gravel, stone or grass:
				NOISE_DATATYPE NoiseX = (static_cast<NOISE_DATATYPE>(a_ChunkDesc.GetChunkX() * cChunkDef::Width + a_RelX)) / FrequencyX;
				NOISE_DATATYPE NoiseY = (static_cast<NOISE_DATATYPE>(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + a_RelZ)) / FrequencyZ;
				NOISE_DATATYPE Val = m_OceanFloorSelect.CubicNoise2D(NoiseX, NoiseY);
				const cPattern::BlockInfo * Pattern = (Val < 0.0) ? patStone.Get() : patGrass.Get();
				FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, Pattern, a_ShapeColumn);
				return;
			}
			case biInvalidBiome:
			case biNether:
			case biEnd:
			case biNumBiomes:
			case biVariant:
			case biNumVariantBiomes:
			{
				// This generator is not supposed to be used for these biomes, but it has to produce *something*
				// so let's produce stone:
				FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patStone.Get(), a_ShapeColumn);
				return;
			}
		}  // switch (Biome)
	}



	/** Fills the specified column with the specified pattern; restarts the pattern when air is reached,
	switches to ocean floor pattern if ocean is reached. Always adds bedrock at the very bottom. */
	void FillColumnPattern(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelZ, const cPattern::BlockInfo * a_Pattern, const Byte * a_ShapeColumn)
	{
		bool HasHadWater = false;
		int PatternIdx = 0;
		HEIGHTTYPE top = a_ChunkDesc.GetHeight(a_RelX, a_RelZ);
		if (top < m_SeaLevel)
		{
			top = m_SeaLevel;
			a_ChunkDesc.SetHeight(a_RelX, a_RelZ, top - 1);
		}
		for (int y = top; y > 0; y--)
		{
			if (a_ShapeColumn[y] > 0)
			{
				// "ground" part, use the pattern:
				a_ChunkDesc.SetBlockTypeMeta(a_RelX, y, a_RelZ, a_Pattern[PatternIdx].m_BlockType, a_Pattern[PatternIdx].m_BlockMeta);
				PatternIdx++;
				continue;
			}

			// "air" or "water" part:
			// Reset the pattern index to zero, so that the pattern is repeated from the top again:
			PatternIdx = 0;

			if (y >= m_SeaLevel)
			{
				// "air" part, do nothing
				continue;
			}

			a_ChunkDesc.SetBlockType(a_RelX, y, a_RelZ, E_BLOCK_STATIONARY_WATER);
			if (HasHadWater)
			{
				continue;
			}

			// Select the ocean-floor pattern to use:
			if (a_ChunkDesc.GetBiome(a_RelX, a_RelZ) == biDeepOcean)
			{
				a_Pattern = patGravel.Get();
			}
			else
			{
				a_Pattern = ChooseOceanFloorPattern(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ(), a_RelX, a_RelZ);
			}
			HasHadWater = true;
		}  // for y
		a_ChunkDesc.SetBlockType(a_RelX, 0, a_RelZ, E_BLOCK_BEDROCK);
	}



	/** Fills the specified column with mesa pattern, based on the column height */
	void FillColumnMesa(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelZ, const Byte * a_ShapeColumn)
	{
		// Frequencies for the clay floor noise:
		const NOISE_DATATYPE FrequencyX = 50;
		const NOISE_DATATYPE FrequencyZ = 50;

		int Top = a_ChunkDesc.GetHeight(a_RelX, a_RelZ);
		if (Top < m_SeaLevel)
		{
			// The terrain is below sealevel, handle as regular ocean with red sand floor:
			FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patOFOrangeClay.Get(), a_ShapeColumn);
			return;
		}

		NOISE_DATATYPE NoiseX = (static_cast<NOISE_DATATYPE>(a_ChunkDesc.GetChunkX() * cChunkDef::Width + a_RelX)) / FrequencyX;
		NOISE_DATATYPE NoiseY = (static_cast<NOISE_DATATYPE>(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + a_RelZ)) / FrequencyZ;
		int ClayFloor = m_SeaLevel - 6 + static_cast<int>(4.f * m_MesaFloor.CubicNoise2D(NoiseX, NoiseY));
		if (ClayFloor >= Top)
		{
			ClayFloor = Top - 1;
		}

		if (Top - m_SeaLevel < 5)
		{
			// Simple case: top is red sand, then hardened clay down to ClayFloor, then stone:
			a_ChunkDesc.SetBlockTypeMeta(a_RelX, Top, a_RelZ, E_BLOCK_SAND, E_META_SAND_RED);
			for (int y = Top - 1; y >= ClayFloor; y--)
			{
				a_ChunkDesc.SetBlockType(a_RelX, y, a_RelZ, E_BLOCK_HARDENED_CLAY);
			}
			for (int y = ClayFloor - 1; y > 0; y--)
			{
				a_ChunkDesc.SetBlockType(a_RelX, y, a_RelZ, E_BLOCK_STONE);
			}
			a_ChunkDesc.SetBlockType(a_RelX, 0, a_RelZ, E_BLOCK_BEDROCK);
			return;
		}

		// Difficult case: use the mesa pattern and watch for overhangs:
		int PatternIdx = cChunkDef::Height - (Top - ClayFloor);  // We want the block at index ClayFloor to be pattern's 256th block (first stone)
		const cPattern::BlockInfo * Pattern = m_MesaPattern;
		bool HasHadWater = false;
		for (int y = Top; y > 0; y--)
		{
			if (a_ShapeColumn[y] > 0)
			{
				// "ground" part, use the pattern:
				a_ChunkDesc.SetBlockTypeMeta(a_RelX, y, a_RelZ, Pattern[PatternIdx].m_BlockType, Pattern[PatternIdx].m_BlockMeta);
				PatternIdx++;
				continue;
			}

			if (y >= m_SeaLevel)
			{
				// "air" part, do nothing
				continue;
			}

			// "water" part, fill with water and choose new pattern for ocean floor, if not chosen already:
			PatternIdx = 0;
			a_ChunkDesc.SetBlockType(a_RelX, y, a_RelZ, E_BLOCK_STATIONARY_WATER);
			if (HasHadWater)
			{
				continue;
			}

			// Select the ocean-floor pattern to use:
			Pattern = ChooseOceanFloorPattern(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ(), a_RelX, a_RelZ);
			HasHadWater = true;
		}  // for y
		a_ChunkDesc.SetBlockType(a_RelX, 0, a_RelZ, E_BLOCK_BEDROCK);

		EMCSBiome MesaVersion = a_ChunkDesc.GetBiome(a_RelX, a_RelZ);
		if ((MesaVersion == biMesaPlateauF) || (MesaVersion == biMesaPlateauFM))
		{
			if (Top < 95 + static_cast<int>(m_MesaFloor.CubicNoise2D(NoiseY * 2, NoiseX * 2) * 6))
			{
				return;
			}

			BLOCKTYPE Block = (m_MesaFloor.CubicNoise2D(NoiseX * 4, NoiseY * 4) < 0) ? E_BLOCK_DIRT : E_BLOCK_GRASS;
			NIBBLETYPE Meta = (Block == E_BLOCK_GRASS) ? 0 : 1;

			a_ChunkDesc.SetBlockTypeMeta(a_RelX, Top, a_RelZ, Block, Meta);
		}
	}



	/** Returns the pattern to use for an ocean floor in the specified column.
	The returned pattern is guaranteed to be 256 blocks long. */
	const cPattern::BlockInfo * ChooseOceanFloorPattern(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelZ)
	{
		// Frequencies for the ocean floor selecting noise:
		const NOISE_DATATYPE FrequencyX = 3;
		const NOISE_DATATYPE FrequencyZ = 3;

		// Select the ocean-floor pattern to use:
		NOISE_DATATYPE NoiseX = (static_cast<NOISE_DATATYPE>(a_ChunkX * cChunkDef::Width + a_RelX)) / FrequencyX;
		NOISE_DATATYPE NoiseY = (static_cast<NOISE_DATATYPE>(a_ChunkZ * cChunkDef::Width + a_RelZ)) / FrequencyZ;
		NOISE_DATATYPE Val = m_OceanFloorSelect.CubicNoise2D(NoiseX, NoiseY);
		if (Val < -0.95)
		{
			return patOFClay.Get();
		}
		else if (Val < 0)
		{
			return patOFSand.Get();
		}
		else
		{
			return patDirt.Get();
		}
	}
} ;





cTerrainCompositionGenPtr CreateCompoGenBiomal(int a_Seed)
{
	return std::make_shared<cCompoGenBiomal>(a_Seed);
}



