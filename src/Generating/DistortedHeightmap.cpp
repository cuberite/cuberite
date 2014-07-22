
// DistortedHeightmap.cpp

// Implements the cDistortedHeightmap class representing the height and composition generator capable of overhangs

#include "Globals.h"

#include "DistortedHeightmap.h"
#include "../OSSupport/File.h"
#include "inifile/iniFile.h"
#include "../LinearUpscale.h"





////////////////////////////////////////////////////////////////////////////////
// cPattern:

/// This class is used to store a column pattern initialized at runtime,
/// so that the program doesn't need to explicitly set 256 values for each pattern
/// Each pattern has 256 blocks so that there's no need to check pattern bounds when assigning the
/// pattern - there will always be enough pattern left, even for the whole chunk height
class cPattern
{
public:
	cPattern(cDistortedHeightmap::sBlockInfo * a_TopBlocks, size_t a_Count)
	{
		// Copy the pattern into the top:
		for (size_t i = 0; i < a_Count; i++)
		{
			m_Pattern[i] = a_TopBlocks[i];
		}
		
		// Fill the rest with stone:
		static cDistortedHeightmap::sBlockInfo Stone = {E_BLOCK_STONE, 0};
		for (size_t i = a_Count; i < cChunkDef::Height; i++)
		{
			m_Pattern[i] = Stone;
		}
	}
	
	const cDistortedHeightmap::sBlockInfo * Get(void) const { return m_Pattern; }
	
protected:
	cDistortedHeightmap::sBlockInfo m_Pattern[cChunkDef::Height];
} ;





////////////////////////////////////////////////////////////////////////////////
// The arrays to use for the top block pattern definitions:

static cDistortedHeightmap::sBlockInfo tbGrass[] =
{
	{E_BLOCK_GRASS, 0},
	{E_BLOCK_DIRT,  E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT,  E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT,  E_META_DIRT_NORMAL},
} ;

static cDistortedHeightmap::sBlockInfo tbSand[] =
{
	{ E_BLOCK_SAND, 0},
	{ E_BLOCK_SAND, 0},
	{ E_BLOCK_SAND, 0},
	{ E_BLOCK_SANDSTONE, 0},
} ;

static cDistortedHeightmap::sBlockInfo tbDirt[] =
{
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
} ;

static cDistortedHeightmap::sBlockInfo tbPodzol[] =
{
	{E_BLOCK_DIRT, E_META_DIRT_PODZOL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
} ;

static cDistortedHeightmap::sBlockInfo tbGrassLess[] =
{
	{E_BLOCK_DIRT, E_META_DIRT_GRASSLESS},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
	{E_BLOCK_DIRT, E_META_DIRT_NORMAL},
} ;

static cDistortedHeightmap::sBlockInfo tbMycelium[] =
{
	{E_BLOCK_MYCELIUM, 0},
	{E_BLOCK_DIRT,     0},
	{E_BLOCK_DIRT,     0},
	{E_BLOCK_DIRT,     0},
} ;

static cDistortedHeightmap::sBlockInfo tbGravel[] =
{
	{E_BLOCK_GRAVEL, 0},
	{E_BLOCK_GRAVEL, 0},
	{E_BLOCK_GRAVEL, 0},
	{E_BLOCK_STONE,  0},
} ;

static cDistortedHeightmap::sBlockInfo tbStone[] =
{
	{E_BLOCK_STONE,   0},
	{E_BLOCK_STONE,   0},
	{E_BLOCK_STONE,   0},
	{E_BLOCK_STONE,   0},
} ;



////////////////////////////////////////////////////////////////////////////////
// Ocean floor pattern top-block definitions:

static cDistortedHeightmap::sBlockInfo tbOFSand[] =
{
	{E_BLOCK_SAND, 0},
	{E_BLOCK_SAND, 0},
	{E_BLOCK_SAND, 0},
	{E_BLOCK_SANDSTONE, 0}
} ;

static cDistortedHeightmap::sBlockInfo tbOFClay[] =
{
	{ E_BLOCK_CLAY, 0},
	{ E_BLOCK_CLAY, 0},
	{ E_BLOCK_SAND, 0},
	{ E_BLOCK_SAND, 0},
} ;

static cDistortedHeightmap::sBlockInfo tbOFRedSand[] =
{
	{ E_BLOCK_SAND,      E_META_SAND_RED},
	{ E_BLOCK_SAND,      E_META_SAND_RED},
	{ E_BLOCK_SAND,      E_META_SAND_RED},
	{ E_BLOCK_SANDSTONE, 0},
} ;






////////////////////////////////////////////////////////////////////////////////
// Individual patterns to use:

static cPattern patGrass    (tbGrass,     ARRAYCOUNT(tbGrass));
static cPattern patSand     (tbSand,      ARRAYCOUNT(tbSand));
static cPattern patDirt     (tbDirt,      ARRAYCOUNT(tbDirt));
static cPattern patPodzol   (tbPodzol,    ARRAYCOUNT(tbPodzol));
static cPattern patGrassLess(tbGrassLess, ARRAYCOUNT(tbGrassLess));
static cPattern patMycelium (tbMycelium,  ARRAYCOUNT(tbMycelium));
static cPattern patGravel   (tbGravel,    ARRAYCOUNT(tbGravel));
static cPattern patStone    (tbStone,     ARRAYCOUNT(tbStone));

static cPattern patOFSand   (tbOFSand,    ARRAYCOUNT(tbOFSand));
static cPattern patOFClay   (tbOFClay,    ARRAYCOUNT(tbOFClay));
static cPattern patOFRedSand(tbOFRedSand, ARRAYCOUNT(tbOFRedSand));





////////////////////////////////////////////////////////////////////////////////
// cDistortedHeightmap:

/** This table assigns a relative maximum overhang size in each direction to biomes.
Both numbers indicate a number which will multiply the noise value for each coord;
this means that you can have different-sized overhangs in each direction.
Usually you'd want to keep both numbers the same.
The numbers are "relative", not absolute maximum; overhangs of a slightly larger size are possible
due to the way that noise is calculated.
*/
const cDistortedHeightmap::sGenParam cDistortedHeightmap::m_GenParam[256] =
{
	/* Biome              |   AmpX | AmpZ */
	/* biOcean            */ { 1.5f,  1.5f},
	/* biPlains           */ { 0.5f,  0.5f},
	/* biDesert           */ { 0.5f,  0.5f},
	/* biExtremeHills     */ {16.0f, 16.0f},
	/* biForest           */ { 3.0f,  3.0f},
	/* biTaiga            */ { 1.5f,  1.5f},

	/* biSwampland        */ { 0.0f,  0.0f},
	/* biRiver            */ { 0.0f,  0.0f},
	/* biNether           */ { 0.0f,  0.0f},  // Unused, but must be here due to indexing
	/* biSky              */ { 0.0f,  0.0f},  // Unused, but must be here due to indexing
	/* biFrozenOcean      */ { 0.0f,  0.0f},
	/* biFrozenRiver      */ { 0.0f,  0.0f},
	/* biIcePlains        */ { 0.0f,  0.0f},
	/* biIceMountains     */ { 8.0f,  8.0f},
	/* biMushroomIsland   */ { 4.0f,  4.0f},
	/* biMushroomShore    */ { 0.0f,  0.0f},
	/* biBeach            */ { 0.0f,  0.0f},
	/* biDesertHills      */ { 5.0f,  5.0f},
	/* biForestHills      */ { 6.0f,  6.0f},
	/* biTaigaHills       */ { 8.0f,  8.0f},
	/* biExtremeHillsEdge */ { 7.0f,  7.0f},
	/* biJungle           */ { 4.0f,  4.0f},
	/* biJungleHills      */ { 8.0f,  8.0f},
	/* biJungleEdge       */ { 3.0f,  3.0f},  // 23
	/* biDeepOcean        */ { 1.0f,  1.0f},  // 24
	/* biStoneBeach       */ { 1.0f,  1.0f},  // 25
	/* biColdBeach        */ { 1.0f,  1.0f},  // 26
	/* biBirchForest      */ { 3.0f,  3.0f},  // 27
	/* biBirchForestHills */ { 6.0f,  6.0f},  // 28
	/* biRoofedForest     */ { 3.0f,  3.0f},  // 29
	/* biColdTaiga        */ { 0.5f,  0.5f},  // 30
	/* biColdTaigaHills   */ { 4.0f,  4.0f},  // 31
	/* biMegaTaiga        */ { 1.0f,  1.0f},  // 32
	/* biMegaTaigaHills   */ { 4.0f,  4.0f},  // 33
	/* biExtremeHillsPlus */ {32.0f, 32.0f},  // 34  - anyone say extreme plus? Make it extreme plus, then :)
	/* biSavanna          */ { 2.0f,  2.0f},  // 35
	/* biSavannaPlateau   */ { 3.0f,  3.0f},  // 36
	/* biMesa             */ { 2.0f,  2.0f},  // 37
	/* biMesaPlateauF     */ { 2.0f,  2.0f},  // 38
	/* biMesaPlateau      */ { 2.0f,  2.0f},  // 39
	
	// biomes 40 .. 128 are unused, 89 empty placeholders here:
	{}, {}, {}, {}, {}, {}, {}, {}, {}, {},  // 40 .. 49
	{}, {}, {}, {}, {}, {}, {}, {}, {}, {},  // 50 .. 59
	{}, {}, {}, {}, {}, {}, {}, {}, {}, {},  // 60 .. 69
	{}, {}, {}, {}, {}, {}, {}, {}, {}, {},  // 70 .. 79
	{}, {}, {}, {}, {}, {}, {}, {}, {}, {},  // 80 .. 89
	{}, {}, {}, {}, {}, {}, {}, {}, {}, {},  // 90 .. 99
	{}, {}, {}, {}, {}, {}, {}, {}, {}, {},  // 100 .. 109
	{}, {}, {}, {}, {}, {}, {}, {}, {}, {},  // 110 .. 119
	{}, {}, {}, {}, {}, {}, {}, {}, {},      // 120 .. 128
	
	// Release 1.7 /* biome variants:
	/* biSunflowerPlains      */ { 1.0f,  1.0f},  // 129
	/* biDesertM              */ { 1.0f,  1.0f},  // 130
	/* biExtremeHillsM        */ {16.0f, 16.0f},  // 131
	/* biFlowerForest         */ { 4.0f,  4.0f},  // 132
	/* biTaigaM               */ { 3.0f,  3.0f},  // 133
	/* biSwamplandM           */ { 0.0f,  0.0f},  // 134
	
	// Biomes 135 .. 139 unused, 5 empty placeholders here:
	{}, {}, {}, {}, {},                           // 135 .. 139

	/* biIcePlainsSpikes      */ { 1.0f,  1.0f},  // 140
	
	// Biomes 141 .. 148 unused, 8 empty placeholders here:
	{}, {}, {}, {}, {}, {}, {}, {},               // 141 .. 148

	/* biJungleM              */ { 4.0f,  4.0f},  // 149
	{},                                           // 150
	/* biJungleEdgeM          */ { 3.0f,  3.0f},  // 151
	{}, {}, {},                                   // 152 .. 154
	/* biBirchForestM         */ { 3.0f,  3.0f},  // 155
	/* biBirchForestHillsM    */ { 5.0f,  5.0f},  // 156
	/* biRoofedForestM        */ { 2.0f,  2.0f},  // 157
	/* biColdTaigaM           */ { 1.0f,  1.0f},  // 158
	{},                                           // 159
	/* biMegaSpruceTaiga      */ { 3.0f,  3.0f},  // 160
	/* biMegaSpruceTaigaHills */ { 3.0f,  3.0f},  // 161
	/* biExtremeHillsPlusM    */ {32.0f, 32.0f},  // 162
	/* biSavannaM             */ { 2.0f,  2.0f},  // 163
	/* biSavannaPlateauM      */ { 3.0f,  3.0f},  // 164
	/* biMesaBryce            */ { 0.5f,  0.5f},  // 165
	/* biMesaPlateauFM        */ { 2.0f,  2.0f},  // 166
	/* biMesaPlateauM         */ { 2.0f,  2.0f},  // 167
} ;





cDistortedHeightmap::cDistortedHeightmap(int a_Seed, cBiomeGen & a_BiomeGen) :
	m_NoiseDistortX(a_Seed + 1000),
	m_NoiseDistortZ(a_Seed + 2000),
	m_OceanFloorSelect(a_Seed + 3000),
	m_MesaFloor(a_Seed + 4000),
	m_BiomeGen(a_BiomeGen),
	m_UnderlyingHeiGen(a_Seed, a_BiomeGen),
	m_HeightGen(m_UnderlyingHeiGen, 64),
	m_IsInitialized(false)
{
	m_NoiseDistortX.AddOctave((NOISE_DATATYPE)1,    (NOISE_DATATYPE)0.5);
	m_NoiseDistortX.AddOctave((NOISE_DATATYPE)0.5,  (NOISE_DATATYPE)1);
	m_NoiseDistortX.AddOctave((NOISE_DATATYPE)0.25, (NOISE_DATATYPE)2);

	m_NoiseDistortZ.AddOctave((NOISE_DATATYPE)1,    (NOISE_DATATYPE)0.5);
	m_NoiseDistortZ.AddOctave((NOISE_DATATYPE)0.5,  (NOISE_DATATYPE)1);
	m_NoiseDistortZ.AddOctave((NOISE_DATATYPE)0.25, (NOISE_DATATYPE)2);
	
	InitMesaPattern(a_Seed);
}





void cDistortedHeightmap::Initialize(cIniFile & a_IniFile)
{
	if (m_IsInitialized)
	{
		return;
	}

	// Read the params from the INI file:
	m_SeaLevel   =                 a_IniFile.GetValueSetI("Generator", "DistortedHeightmapSeaLevel",   62);
	m_FrequencyX = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "DistortedHeightmapFrequencyX", 10);
	m_FrequencyY = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "DistortedHeightmapFrequencyY", 10);
	m_FrequencyZ = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "DistortedHeightmapFrequencyZ", 10);

	m_IsInitialized = true;
}





void cDistortedHeightmap::InitMesaPattern(int a_Seed)
{
	// Stone in the bottom half of the pattern:
	for (int i = cChunkDef::Height; i < 2 * cChunkDef::Height; i++)
	{
		m_MesaPattern[i].BlockMeta = 0;
		m_MesaPattern[i].BlockType = E_BLOCK_STONE;
	}

	// Stained and hardened clay in the top half of the pattern
	// In a loop, choose whether to use one or two layers of stained clay, then choose a color and width for each layer
	// Separate each group with another layer of hardened clay
	cNoise PatternNoise((unsigned)a_Seed);
	static NIBBLETYPE AllowedColors[] =
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
	static int LayerSizes[] =  // Adjust the chance so that thinner layers occur more commonly
	{
		1, 1, 1, 1, 1, 1,
		2, 2, 2, 2,
		3, 3,
	} ;
	int Idx = cChunkDef::Height - 1;
	while (Idx >= 0)
	{
		// A layer group of 1 - 2 color stained clay:
		int Random = PatternNoise.IntNoise1DInt(Idx) / 7;
		int NumLayers = (Random % 2) + 1;
		Random /= 2;
		for (int Lay = 0; Lay < NumLayers; Lay++)
		{
			int NumBlocks = LayerSizes[(Random % ARRAYCOUNT(LayerSizes))];
			NIBBLETYPE Color = AllowedColors[(Random / 4) % ARRAYCOUNT(AllowedColors)];
			if (
				((NumBlocks == 3) && (NumLayers == 2)) ||  // In two-layer mode disallow the 3-high layers:
				(Color == E_META_STAINED_CLAY_WHITE))      // White stained clay can ever be only 1 block high
			{
				NumBlocks = 1;
			}
			NumBlocks = std::min(Idx + 1, NumBlocks);  // Limit by Idx so that we don't have to check inside the loop
			Random /= 32;
			for (int Block = 0; Block < NumBlocks; Block++, Idx--)
			{
				m_MesaPattern[Idx].BlockMeta = Color;
				m_MesaPattern[Idx].BlockType = E_BLOCK_STAINED_CLAY;
			}  // for Block
		}  // for Lay

		// A layer of hardened clay in between the layer group:
		int NumBlocks = (Random % 4) + 1;  // All heights the same probability
		if ((NumLayers == 2) && (NumBlocks < 4))
		{
			// For two layers of stained clay, add an extra block of hardened clay:
			NumBlocks++;
		}
		NumBlocks = std::min(Idx + 1, NumBlocks);  // Limit by Idx so that we don't have to check inside the loop
		for (int Block = 0; Block < NumBlocks; Block++, Idx--)
		{
			m_MesaPattern[Idx].BlockMeta = 0;
			m_MesaPattern[Idx].BlockType = E_BLOCK_HARDENED_CLAY;
		}  // for Block
	}  // while (Idx >= 0)
}





void cDistortedHeightmap::PrepareState(int a_ChunkX, int a_ChunkZ)
{
	if ((m_CurChunkX == a_ChunkX) && (m_CurChunkZ == a_ChunkZ))
	{
		return;
	}
	m_CurChunkX = a_ChunkX;
	m_CurChunkZ = a_ChunkZ;


	m_HeightGen.GenHeightMap(a_ChunkX, a_ChunkZ, m_CurChunkHeights);
	UpdateDistortAmps();
	GenerateHeightArray();
}





void cDistortedHeightmap::GenerateHeightArray(void)
{
	// Generate distortion noise:
	NOISE_DATATYPE DistortNoiseX[DIM_X * DIM_Y * DIM_Z];
	NOISE_DATATYPE DistortNoiseZ[DIM_X * DIM_Y * DIM_Z];
	NOISE_DATATYPE Workspace[DIM_X * DIM_Y * DIM_Z];
	NOISE_DATATYPE StartX = ((NOISE_DATATYPE)(m_CurChunkX * cChunkDef::Width)) / m_FrequencyX;
	NOISE_DATATYPE EndX   = ((NOISE_DATATYPE)((m_CurChunkX + 1) * cChunkDef::Width - 1)) / m_FrequencyX;
	NOISE_DATATYPE StartY = 0;
	NOISE_DATATYPE EndY   = ((NOISE_DATATYPE)(257)) / m_FrequencyY;
	NOISE_DATATYPE StartZ = ((NOISE_DATATYPE)(m_CurChunkZ * cChunkDef::Width)) / m_FrequencyZ;
	NOISE_DATATYPE EndZ   = ((NOISE_DATATYPE)((m_CurChunkZ + 1) * cChunkDef::Width - 1)) / m_FrequencyZ;

	m_NoiseDistortX.Generate3D(DistortNoiseX, DIM_X, DIM_Y, DIM_Z, StartX, EndX, StartY, EndY, StartZ, EndZ, Workspace);
	m_NoiseDistortZ.Generate3D(DistortNoiseZ, DIM_X, DIM_Y, DIM_Z, StartX, EndX, StartY, EndY, StartZ, EndZ, Workspace);

	// The distorted heightmap, before linear upscaling
	NOISE_DATATYPE DistHei[DIM_X * DIM_Y * DIM_Z];

	// Distort the heightmap using the distortion:
	for (int z = 0; z < DIM_Z; z++)
	{
		int AmpIdx = z * DIM_X;
		for (int y = 0; y < DIM_Y; y++)
		{
			int NoiseArrayIdx = z * DIM_X * DIM_Y + y * DIM_X;
			for (int x = 0; x < DIM_X; x++)
			{
				NOISE_DATATYPE DistX = DistortNoiseX[NoiseArrayIdx + x] * m_DistortAmpX[AmpIdx + x];
				NOISE_DATATYPE DistZ = DistortNoiseZ[NoiseArrayIdx + x] * m_DistortAmpZ[AmpIdx + x];
				DistX += (NOISE_DATATYPE)(m_CurChunkX * cChunkDef::Width + x * INTERPOL_X);
				DistZ += (NOISE_DATATYPE)(m_CurChunkZ * cChunkDef::Width + z * INTERPOL_Z);
				// Adding 0.5 helps alleviate the interpolation artifacts
				DistHei[NoiseArrayIdx + x] = (NOISE_DATATYPE)GetHeightmapAt(DistX, DistZ) + (NOISE_DATATYPE)0.5;
			}
		}
	}

	// Upscale the distorted heightmap into full dimensions:
	LinearUpscale3DArray(
		DistHei, DIM_X, DIM_Y, DIM_Z,
		m_DistortedHeightmap, INTERPOL_X, INTERPOL_Y, INTERPOL_Z
	);

	// DEBUG: Debug3DNoise(m_DistortedHeightmap, 17, 257, 17, Printf("DistortedHeightmap_%d_%d", m_CurChunkX, m_CurChunkZ));
}





void cDistortedHeightmap::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	PrepareState(a_ChunkX, a_ChunkZ);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int NoiseArrayIdx = x + 17 * 257 * z;
			cChunkDef::SetHeight(a_HeightMap, x, z, m_SeaLevel - 1);
			for (int y = cChunkDef::Height - 1; y > m_SeaLevel - 1; y--)
			{
				int HeightMapHeight = (int)m_DistortedHeightmap[NoiseArrayIdx + 17 * y];
				if (y < HeightMapHeight)
				{
					cChunkDef::SetHeight(a_HeightMap, x, z, y);
					break;
				}
			}  // for y
		}  // for x
	}  // for z
}





void cDistortedHeightmap::InitializeHeightGen(cIniFile & a_IniFile)
{
	Initialize(a_IniFile);
}





void cDistortedHeightmap::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	// Prepare the internal state for generating this chunk:
	PrepareState(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ());

	// Compose:
	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			ComposeColumn(a_ChunkDesc, x, z);
		}  // for x
	}  // for z
}





void cDistortedHeightmap::InitializeCompoGen(cIniFile & a_IniFile)
{
	Initialize(a_IniFile);
}





int cDistortedHeightmap::GetHeightmapAt(NOISE_DATATYPE a_X, NOISE_DATATYPE a_Z)
{
	int ChunkX = (int)floor(a_X / (NOISE_DATATYPE)16);
	int ChunkZ = (int)floor(a_Z / (NOISE_DATATYPE)16);
	int RelX = (int)(a_X - (NOISE_DATATYPE)ChunkX * cChunkDef::Width);
	int RelZ = (int)(a_Z - (NOISE_DATATYPE)ChunkZ * cChunkDef::Width);

	// If we're withing the same chunk, return the pre-cached heightmap:
	if ((ChunkX == m_CurChunkX) && (ChunkZ == m_CurChunkZ))
	{
		return cChunkDef::GetHeight(m_CurChunkHeights, RelX, RelZ);
	}

	// Ask the cache:
	HEIGHTTYPE res = 0;
	if (m_HeightGen.GetHeightAt(ChunkX, ChunkZ, RelX, RelZ, res))
	{
		// The height was in the cache
		return res;
	}

	// The height is not in the cache, generate full heightmap and get it there:
	cChunkDef::HeightMap Heightmap;
	m_HeightGen.GenHeightMap(ChunkX, ChunkZ, Heightmap);
	return cChunkDef::GetHeight(Heightmap, RelX, RelZ);
}





void cDistortedHeightmap::UpdateDistortAmps(void)
{
	BiomeNeighbors Biomes;
	for (int z = -1; z <= 1; z++)
	{
		for (int x = -1; x <= 1; x++)
		{
			m_BiomeGen.GenBiomes(m_CurChunkX + x, m_CurChunkZ + z, Biomes[x + 1][z + 1]);
		}  // for x
	}  // for z

	for (int z = 0; z < DIM_Z; z++)
	{
		for (int x = 0; x < DIM_Z; x++)
		{
			GetDistortAmpsAt(Biomes, x * INTERPOL_X, z * INTERPOL_Z, m_DistortAmpX[x + DIM_X * z], m_DistortAmpZ[x + DIM_X * z]);
		}
	}
}





void cDistortedHeightmap::GetDistortAmpsAt(BiomeNeighbors & a_Neighbors, int a_RelX, int a_RelZ, NOISE_DATATYPE & a_DistortAmpX, NOISE_DATATYPE & a_DistortAmpZ)
{
	// Sum up how many biomes of each type there are in the neighborhood:
	int BiomeCounts[256];
	memset(BiomeCounts, 0, sizeof(BiomeCounts));
	int Sum = 0;
	for (int z = -8; z <= 8; z++)
	{
		int FinalZ = a_RelZ + z + cChunkDef::Width;
		int IdxZ = FinalZ / cChunkDef::Width;
		int ModZ = FinalZ % cChunkDef::Width;
		int WeightZ = 9 - abs(z);
		for (int x = -8; x <= 8; x++)
		{
			int FinalX = a_RelX + x + cChunkDef::Width;
			int IdxX = FinalX / cChunkDef::Width;
			int ModX = FinalX % cChunkDef::Width;
			EMCSBiome Biome = cChunkDef::GetBiome(a_Neighbors[IdxX][IdxZ], ModX, ModZ);
			int WeightX = 9 - abs(x);
			BiomeCounts[Biome] += WeightX + WeightZ;
			Sum += WeightX + WeightZ;
		}  // for x
	}  // for z

	if (Sum <= 0)
	{
		// No known biome around? Weird. Return a bogus value:
		ASSERT(!"cHeiGenBiomal: Biome sum failed, no known biome around");
		a_DistortAmpX = 16;
		a_DistortAmpZ = 16;
	}

	// For each biome type that has a nonzero count, calc its amps and add it:
	NOISE_DATATYPE AmpX = 0;
	NOISE_DATATYPE AmpZ = 0;
	for (size_t i = 0; i < ARRAYCOUNT(BiomeCounts); i++)
	{
		if (BiomeCounts[i] <= 0)
		{
			continue;
		}
		
		/*
		// Sanity checks for biome parameters, enable them to check the biome param table in runtime (slow):
		ASSERT(m_GenParam[i].m_DistortAmpX >= 0);
		ASSERT(m_GenParam[i].m_DistortAmpX < 100);
		ASSERT(m_GenParam[i].m_DistortAmpX >= 0);
		ASSERT(m_GenParam[i].m_DistortAmpX < 100);
		*/

		AmpX += BiomeCounts[i] * m_GenParam[i].m_DistortAmpX;
		AmpZ += BiomeCounts[i] * m_GenParam[i].m_DistortAmpZ;
	}
	a_DistortAmpX = AmpX / Sum;
	a_DistortAmpZ = AmpZ / Sum;
}




void cDistortedHeightmap::ComposeColumn(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelZ)
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
			FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patGrass.Get());
			return;
		}

		case biMegaTaiga:
		case biMegaTaigaHills:
		case biMegaSpruceTaiga:
		case biMegaSpruceTaigaHills:
		{
			// Select the pattern to use - podzol, grass or grassless dirt:
			NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(m_CurChunkX * cChunkDef::Width + a_RelX)) / FrequencyX;
			NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)(m_CurChunkZ * cChunkDef::Width + a_RelZ)) / FrequencyZ;
			NOISE_DATATYPE Val = m_OceanFloorSelect.CubicNoise2D(NoiseX, NoiseY);
			const sBlockInfo * Pattern = (Val < -0.9) ? patGrassLess.Get() : ((Val > 0) ? patPodzol.Get() : patGrass.Get());
			FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, Pattern);
			return;
		}

		case biDesertHills:
		case biDesert:
		case biDesertM:
		case biBeach:
		{
			FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patSand.Get());
			return;
		}
		
		case biMushroomIsland:
		case biMushroomShore:
		{
			FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patMycelium.Get());
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
			FillColumnMesa(a_ChunkDesc, a_RelX, a_RelZ);
			return;
		}

		case biExtremeHillsPlusM:
		case biExtremeHillsM:
		{
			// Select the pattern to use - gravel, stone or grass:
			NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(m_CurChunkX * cChunkDef::Width + a_RelX)) / FrequencyX;
			NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)(m_CurChunkZ * cChunkDef::Width + a_RelZ)) / FrequencyZ;
			NOISE_DATATYPE Val = m_OceanFloorSelect.CubicNoise2D(NoiseX, NoiseY);
			const sBlockInfo * Pattern = (Val < 0.0) ? patStone.Get() : patGrass.Get();
			FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, Pattern);
			return;
		}
		default:
		{
			ASSERT(!"Unhandled biome");
			return;
		}
	}  // switch (Biome)
}





void cDistortedHeightmap::FillColumnPattern(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelZ, const sBlockInfo * a_Pattern)
{
	int NoiseArrayIdx = a_RelX + 17 * 257 * a_RelZ;
	bool HasHadWater = false;
	int PatternIdx = 0;
	for (int y = a_ChunkDesc.GetHeight(a_RelX, a_RelZ); y > 0; y--)
	{
		int HeightMapHeight = (int)m_DistortedHeightmap[NoiseArrayIdx + 17 * y];

		if (y < HeightMapHeight)
		{
			// "ground" part, use the pattern:
			a_ChunkDesc.SetBlockTypeMeta(a_RelX, y, a_RelZ, a_Pattern[PatternIdx].BlockType, a_Pattern[PatternIdx].BlockMeta);
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
		a_Pattern = ChooseOceanFloorPattern(a_RelX, a_RelZ);
		HasHadWater = true;
	}  // for y
	a_ChunkDesc.SetBlockType(a_RelX, 0, a_RelZ, E_BLOCK_BEDROCK);
}





void cDistortedHeightmap::FillColumnMesa(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelZ)
{
	// Frequencies for the clay floor noise:
	const NOISE_DATATYPE FrequencyX = 50;
	const NOISE_DATATYPE FrequencyZ = 50;

	int Top = a_ChunkDesc.GetHeight(a_RelX, a_RelZ);
	if (Top < m_SeaLevel)
	{
		// The terrain is below sealevel, handle as regular ocean:
		FillColumnPattern(a_ChunkDesc, a_RelX, a_RelZ, patOFRedSand.Get());
		return;
	}

	NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(m_CurChunkX * cChunkDef::Width + a_RelX)) / FrequencyX;
	NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)(m_CurChunkZ * cChunkDef::Width + a_RelZ)) / FrequencyZ;
	int ClayFloor = m_SeaLevel - 6 + (int)(4.f * m_MesaFloor.CubicNoise2D(NoiseX, NoiseY));
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
	int NoiseArrayIdx = a_RelX + 17 * 257 * a_RelZ;
	int PatternIdx = cChunkDef::Height - (Top - ClayFloor);  // We want the block at index ClayFloor to be pattern's 256th block (first stone)
	const sBlockInfo * Pattern = m_MesaPattern;
	bool HasHadWater = false;
	for (int y = Top; y > 0; y--)
	{
		int HeightMapHeight = (int)m_DistortedHeightmap[NoiseArrayIdx + 17 * y];
		if (y < HeightMapHeight)
		{
			// "ground" part, use the pattern:
			a_ChunkDesc.SetBlockTypeMeta(a_RelX, y, a_RelZ, Pattern[PatternIdx].BlockType, Pattern[PatternIdx].BlockMeta);
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
		Pattern = ChooseOceanFloorPattern(a_RelX, a_RelZ);
		HasHadWater = true;
	}  // for y
	a_ChunkDesc.SetBlockType(a_RelX, 0, a_RelZ, E_BLOCK_BEDROCK);
}





const cDistortedHeightmap::sBlockInfo * cDistortedHeightmap::ChooseOceanFloorPattern(int a_RelX, int a_RelZ)
{
	// Frequencies for the ocean floor selecting noise:
	const NOISE_DATATYPE FrequencyX = 3;
	const NOISE_DATATYPE FrequencyZ = 3;

	// Select the ocean-floor pattern to use:
	NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(m_CurChunkX * cChunkDef::Width + a_RelX)) / FrequencyX;
	NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)(m_CurChunkZ * cChunkDef::Width + a_RelZ)) / FrequencyZ;
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




