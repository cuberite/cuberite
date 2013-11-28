
// DistortedHeightmap.cpp

// Implements the cDistortedHeightmap class representing the height and composition generator capable of overhangs

#include "Globals.h"

#include "DistortedHeightmap.h"
#include "../OSSupport/File.h"
#include "inifile/iniFile.h"
#include "../LinearUpscale.h"





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
	/* biMesa             */ { 3.0f,  3.0f},  // 37
	/* biMesaPlateauF     */ { 3.0f,  3.0f},  // 38
	/* biMesaPlateau      */ { 3.0f,  3.0f},  // 39
	
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
	/* biMesaPlateauFM        */ { 3.0f,  3.0f},  // 166
	/* biMesaPlateauM         */ { 3.0f,  3.0f},  // 167
} ;





cDistortedHeightmap::cDistortedHeightmap(int a_Seed, cBiomeGen & a_BiomeGen) :
	m_NoiseDistortX(a_Seed + 1000),
	m_NoiseDistortZ(a_Seed + 2000),
	m_OceanFloorSelect(a_Seed + 3000),
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
	// Frequencies for the ocean floor selecting noise:
	NOISE_DATATYPE FrequencyX = 3;
	NOISE_DATATYPE FrequencyZ = 3;

	// Prepare the internal state for generating this chunk:
	PrepareState(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ());

	// Compose:
	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int NoiseArrayIdx = x + 17 * 257 * z;
			int LastAir = a_ChunkDesc.GetHeight(x, z) + 1;
			bool HasHadWater = false;
			for (int y = LastAir - 1; y > 0; y--)
			{
				int HeightMapHeight = (int)m_DistortedHeightmap[NoiseArrayIdx + 17 * y];

				if (y >= HeightMapHeight)
				{
					// "air" part
					LastAir = y;
					if (y < m_SeaLevel)
					{
						a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_STATIONARY_WATER);
						HasHadWater = true;
					}
					continue;
				}
				// "ground" part:
				if (y < LastAir - 4)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_STONE);
					continue;
				}
				if (HasHadWater)
				{
					// Decide between clay, sand and dirt
					NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(m_CurChunkX * cChunkDef::Width + x)) / FrequencyX;
					NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)(m_CurChunkZ * cChunkDef::Width + z)) / FrequencyZ;
					NOISE_DATATYPE Val = m_OceanFloorSelect.CubicNoise2D(NoiseX, NoiseY);
					if (Val < -0.95)
					{
						// Clay:
						switch (LastAir - y)
						{
							case 0:
							case 1:
							{
								a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_CLAY);
								break;
							}
							case 2:
							case 3:
							{
								a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_SAND);
								break;
							}
							case 4:
							{
								a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_SANDSTONE);
								break;
							}
						}  // switch (floor depth)
					}
					else if (Val < 0)
					{
						a_ChunkDesc.SetBlockType(x, y, z, (y < LastAir - 3) ? E_BLOCK_SANDSTONE : E_BLOCK_SAND);
					}
					else
					{
						a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_DIRT);
					}
				}
				else
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
							a_ChunkDesc.SetBlockType(x, y, z, (y == LastAir - 1) ? E_BLOCK_GRASS : E_BLOCK_DIRT);
							break;
						}
						case biDesertHills:
						case biDesert:
						case biBeach:
						{
							a_ChunkDesc.SetBlockType(x, y, z, (y < LastAir - 3) ? E_BLOCK_SANDSTONE : E_BLOCK_SAND);
							break;
						}
						case biMushroomIsland:
						case biMushroomShore:
						{
							a_ChunkDesc.SetBlockType(x, y, z, (y == LastAir - 1) ? E_BLOCK_MYCELIUM : E_BLOCK_DIRT);
							break;
						}
					}
				}
			}  // for y
			a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
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
	for (unsigned int i = 0; i < ARRAYCOUNT(BiomeCounts); i++)
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




