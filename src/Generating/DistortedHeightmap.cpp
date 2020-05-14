
// DistortedHeightmap.cpp

// Implements the cDistortedHeightmap class representing the height and composition generator capable of overhangs

#include "Globals.h"

#include "DistortedHeightmap.h"
#include "../IniFile.h"
#include "../LinearUpscale.h"





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
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 40 .. 49
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 50 .. 59
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 60 .. 69
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 70 .. 79
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 80 .. 89
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 90 .. 99
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 100 .. 109
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 110 .. 119
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},                // 120 .. 128

	// Release 1.7 biome variants:
	/* biSunflowerPlains      */ { 1.0f,  1.0f},  // 129
	/* biDesertM              */ { 1.0f,  1.0f},  // 130
	/* biExtremeHillsM        */ {16.0f, 16.0f},  // 131
	/* biFlowerForest         */ { 4.0f,  4.0f},  // 132
	/* biTaigaM               */ { 3.0f,  3.0f},  // 133
	/* biSwamplandM           */ { 0.0f,  0.0f},  // 134

	// Biomes 135 .. 139 unused, 5 empty placeholders here:
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 135 .. 139

	/* biIcePlainsSpikes      */ { 1.0f,  1.0f},  // 140

	// Biomes 141 .. 148 unused, 8 empty placeholders here:
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},  // 141 .. 148

	/* biJungleM              */ { 4.0f,  4.0f},  // 149
	{0.0f, 0.0f},                                 // 150
	/* biJungleEdgeM          */ { 3.0f,  3.0f},  // 151
	{0.0f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f},     // 152 .. 154
	/* biBirchForestM         */ { 3.0f,  3.0f},  // 155
	/* biBirchForestHillsM    */ { 5.0f,  5.0f},  // 156
	/* biRoofedForestM        */ { 2.0f,  2.0f},  // 157
	/* biColdTaigaM           */ { 1.0f,  1.0f},  // 158
	{0.0f, 0.0f},                                 // 159
	/* biMegaSpruceTaiga      */ { 3.0f,  3.0f},  // 160
	/* biMegaSpruceTaigaHills */ { 3.0f,  3.0f},  // 161
	/* biExtremeHillsPlusM    */ {32.0f, 32.0f},  // 162
	/* biSavannaM             */ { 2.0f,  2.0f},  // 163
	/* biSavannaPlateauM      */ { 3.0f,  3.0f},  // 164
	/* biMesaBryce            */ { 0.5f,  0.5f},  // 165
	/* biMesaPlateauFM        */ { 2.0f,  2.0f},  // 166
	/* biMesaPlateauM         */ { 2.0f,  2.0f},  // 167
} ;





cDistortedHeightmap::cDistortedHeightmap(int a_Seed, const cBiomeGenPtr & a_BiomeGen) :
	m_NoiseDistortX(a_Seed + 1000),
	m_NoiseDistortZ(a_Seed + 2000),
	m_CurChunkCoords(0x7fffffff, 0x7fffffff),  // Set impossible coords for the chunk so that it's always considered stale
	m_BiomeGen(a_BiomeGen),
	m_UnderlyingHeiGen(new cHeiGenBiomal(a_Seed, a_BiomeGen)),
	m_HeightGen(m_UnderlyingHeiGen, 64),
	m_IsInitialized(false)
{
	m_NoiseDistortX.AddOctave(static_cast<NOISE_DATATYPE>(1),    static_cast<NOISE_DATATYPE>(0.5));
	m_NoiseDistortX.AddOctave(static_cast<NOISE_DATATYPE>(0.5),  static_cast<NOISE_DATATYPE>(1));
	m_NoiseDistortX.AddOctave(static_cast<NOISE_DATATYPE>(0.25), static_cast<NOISE_DATATYPE>(2));

	m_NoiseDistortZ.AddOctave(static_cast<NOISE_DATATYPE>(1),    static_cast<NOISE_DATATYPE>(0.5));
	m_NoiseDistortZ.AddOctave(static_cast<NOISE_DATATYPE>(0.5),  static_cast<NOISE_DATATYPE>(1));
	m_NoiseDistortZ.AddOctave(static_cast<NOISE_DATATYPE>(0.25), static_cast<NOISE_DATATYPE>(2));
}





void cDistortedHeightmap::Initialize(cIniFile & a_IniFile)
{
	if (m_IsInitialized)
	{
		return;
	}

	// Read the params from the INI file:
	m_SeaLevel   =                 a_IniFile.GetValueSetI("Generator", "SeaLevel",                     62);
	m_FrequencyX = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "DistortedHeightmapFrequencyX", 10));
	m_FrequencyY = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "DistortedHeightmapFrequencyY", 10));
	m_FrequencyZ = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "DistortedHeightmapFrequencyZ", 10));

	m_IsInitialized = true;
}





void cDistortedHeightmap::PrepareState(cChunkCoords a_ChunkCoords)
{
	if (m_CurChunkCoords == a_ChunkCoords)
	{
		return;
	}
	m_CurChunkCoords = a_ChunkCoords;


	m_HeightGen.GenHeightMap(a_ChunkCoords, m_CurChunkHeights);
	UpdateDistortAmps();
	GenerateHeightArray();
}





void cDistortedHeightmap::GenerateHeightArray(void)
{
	// Generate distortion noise:
	NOISE_DATATYPE DistortNoiseX[DIM_X * DIM_Y * DIM_Z];
	NOISE_DATATYPE DistortNoiseZ[DIM_X * DIM_Y * DIM_Z];
	NOISE_DATATYPE Workspace[DIM_X * DIM_Y * DIM_Z];
	NOISE_DATATYPE StartX = static_cast<NOISE_DATATYPE>(m_CurChunkCoords.m_ChunkX * cChunkDef::Width) / m_FrequencyX;
	NOISE_DATATYPE EndX   = static_cast<NOISE_DATATYPE>((m_CurChunkCoords.m_ChunkX + 1) * cChunkDef::Width - 1) / m_FrequencyX;
	NOISE_DATATYPE StartY = 0;
	NOISE_DATATYPE EndY   = static_cast<NOISE_DATATYPE>(257) / m_FrequencyY;
	NOISE_DATATYPE StartZ = static_cast<NOISE_DATATYPE>(m_CurChunkCoords.m_ChunkZ * cChunkDef::Width) / m_FrequencyZ;
	NOISE_DATATYPE EndZ   = static_cast<NOISE_DATATYPE>((m_CurChunkCoords.m_ChunkZ + 1) * cChunkDef::Width - 1) / m_FrequencyZ;

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
				DistX += static_cast<NOISE_DATATYPE>(m_CurChunkCoords.m_ChunkX * cChunkDef::Width + x * INTERPOL_X);
				DistZ += static_cast<NOISE_DATATYPE>(m_CurChunkCoords.m_ChunkZ * cChunkDef::Width + z * INTERPOL_Z);
				// Adding 0.5 helps alleviate the interpolation artifacts
				DistHei[NoiseArrayIdx + x] = static_cast<NOISE_DATATYPE>(GetHeightmapAt(DistX, DistZ)) + static_cast<NOISE_DATATYPE>(0.5);
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





void cDistortedHeightmap::GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape)
{
	PrepareState(a_ChunkCoords);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int idx = x + 17 * 257 * z;
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				a_Shape[y + x * 256 + z * 16 * 256] = (y < m_DistortedHeightmap[idx + y * 17]) ? 1 : 0;
			}  // for y
		}  // for x
	}  // for z
}





void cDistortedHeightmap::InitializeShapeGen(cIniFile & a_IniFile)
{
	Initialize(a_IniFile);
}





int cDistortedHeightmap::GetHeightmapAt(NOISE_DATATYPE a_X, NOISE_DATATYPE a_Z)
{
	int RelX = FloorC(a_X);
	int RelY = 0;
	int RelZ = FloorC(a_Z);
	int ChunkX, ChunkZ;
	cChunkDef::AbsoluteToRelative(RelX, RelY, RelZ, ChunkX, ChunkZ);

	// If we're within the same chunk, return the pre-cached heightmap:
	if ((ChunkX == m_CurChunkCoords.m_ChunkX) && (ChunkZ == m_CurChunkCoords.m_ChunkZ))
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
	m_HeightGen.GenHeightMap({ChunkX, ChunkZ}, Heightmap);
	return cChunkDef::GetHeight(Heightmap, RelX, RelZ);
}





void cDistortedHeightmap::UpdateDistortAmps(void)
{
	BiomeNeighbors Biomes;
	for (int z = -1; z <= 1; z++)
	{
		for (int x = -1; x <= 1; x++)
		{
			m_BiomeGen->GenBiomes({m_CurChunkCoords.m_ChunkX + x, m_CurChunkCoords.m_ChunkZ + z}, Biomes[x + 1][z + 1]);
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




