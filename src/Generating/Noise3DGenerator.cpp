
// Nosie3DGenerator.cpp

// Generates terrain using 3D noise, rather than composing. Is a test.

#include "Globals.h"
#include "Noise3DGenerator.h"
#include "../OSSupport/File.h"
#include "../IniFile.h"
#include "../LinearInterpolation.h"
#include "../LinearUpscale.h"





/*
// Perform an automatic test of upscaling upon program start (use breakpoints to debug):

class Test
{
public:
	Test(void)
	{
		DoTest1();
		DoTest2();
	}


	void DoTest1(void)
	{
		float In[3 * 3 * 3];
		for (size_t i = 0; i < ARRAYCOUNT(In); i++)
		{
			In[i] = (float)(i % 5);
		}
		Debug3DNoise(In, 3, 3, 3, "Upscale3D in");
		float Out[17 * 33 * 35];
		LinearUpscale3DArray(In, 3, 3, 3, Out, 8, 16, 17);
		Debug3DNoise(Out, 17, 33, 35, "Upscale3D test");
	}


	void DoTest2(void)
	{
		float In[3 * 3];
		for (size_t i = 0; i < ARRAYCOUNT(In); i++)
		{
			In[i] = (float)(i % 5);
		}
		Debug2DNoise(In, 3, 3, "Upscale2D in");
		float Out[17 * 33];
		LinearUpscale2DArray(In, 3, 3, Out, 8, 16);
		Debug2DNoise(Out, 17, 33, "Upscale2D test");
	}

} gTest;
//*/





/** Linearly interpolates between two values.
Assumes that a_Ratio is in range [0, 1]. */
inline static NOISE_DATATYPE Lerp(NOISE_DATATYPE a_Val1, NOISE_DATATYPE a_Val2, NOISE_DATATYPE a_Ratio)
{
	return a_Val1 + (a_Val2 - a_Val1) * a_Ratio;
}





/** Linearly interpolates between two values, clamping the ratio to [0, 1] first. */
inline static NOISE_DATATYPE ClampedLerp(NOISE_DATATYPE a_Val1, NOISE_DATATYPE a_Val2, NOISE_DATATYPE a_Ratio)
{
	if (a_Ratio < 0)
	{
		return a_Val1;
	}
	if (a_Ratio > 1)
	{
		return a_Val2;
	}
	return Lerp(a_Val1, a_Val2, a_Ratio);
}





////////////////////////////////////////////////////////////////////////////////
// cNoise3DGenerator:

cNoise3DGenerator::cNoise3DGenerator(cChunkGenerator & a_ChunkGenerator) :
	super(a_ChunkGenerator),
	m_Perlin(1000),
	m_Cubic(1000)
{
	m_Perlin.AddOctave(1, (NOISE_DATATYPE)0.5);
	m_Perlin.AddOctave((NOISE_DATATYPE)0.5, 1);
	m_Perlin.AddOctave((NOISE_DATATYPE)0.5, 2);

	#if 0
	// DEBUG: Test the noise generation:
	// NOTE: In order to be able to run MCS with this code, you need to increase the default thread stack size
	// In MSVC, it is done in Project Settings -> Configuration Properties -> Linker -> System, set Stack reserve size to at least 64M
	m_SeaLevel            = 62;
	m_HeightAmplification = 0;
	m_MidPoint            = 75;
	m_FrequencyX          = 4;
	m_FrequencyY          = 4;
	m_FrequencyZ          = 4;
	m_AirThreshold        = 0.5;

	const int NumChunks = 4;
	NOISE_DATATYPE Noise[NumChunks][cChunkDef::Width * cChunkDef::Width * cChunkDef::Height];
	for (int x = 0; x < NumChunks; x++)
	{
		GenerateNoiseArray(x, 5, Noise[x]);
	}

	// Save in XY cuts:
	cFile f1;
	if (f1.Open("Test_XY.grab", cFile::fmWrite))
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				for (int i = 0; i < NumChunks; i++)
				{
					int idx = y * cChunkDef::Width + z * cChunkDef::Width * cChunkDef::Height;
					unsigned char buf[cChunkDef::Width];
					for (int x = 0; x < cChunkDef::Width; x++)
					{
						buf[x] = (unsigned char)(std::min(256, std::max(0, (int)(128 + 32 * Noise[i][idx++]))));
					}
					f1.Write(buf, cChunkDef::Width);
				}
			}  // for y
		}  // for z
	}  // if (XY file open)

	cFile f2;
	if (f2.Open("Test_XZ.grab", cFile::fmWrite))
	{
		for (int y = 0; y < cChunkDef::Height; y++)
		{
			for (int z = 0; z < cChunkDef::Width; z++)
			{
				for (int i = 0; i < NumChunks; i++)
				{
					int idx = y * cChunkDef::Width + z * cChunkDef::Width * cChunkDef::Height;
					unsigned char buf[cChunkDef::Width];
					for (int x = 0; x < cChunkDef::Width; x++)
					{
						buf[x] = (unsigned char)(std::min(256, std::max(0, (int)(128 + 32 * Noise[i][idx++]))));
					}
					f2.Write(buf, cChunkDef::Width);
				}
			}  // for z
		}  // for y
	}  // if (XZ file open)
	#endif  // 0
}





cNoise3DGenerator::~cNoise3DGenerator()
{
	// Nothing needed yet
}





void cNoise3DGenerator::Initialize(cIniFile & a_IniFile)
{
	// Params:
	m_SeaLevel            =                 a_IniFile.GetValueSetI("Generator", "Noise3DSeaLevel", 62);
	m_HeightAmplification = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DHeightAmplification", 0);
	m_MidPoint            = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DMidPoint", 75);
	m_FrequencyX          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyX", 8);
	m_FrequencyY          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyY", 8);
	m_FrequencyZ          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyZ", 8);
	m_AirThreshold        = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DAirThreshold", 0.5);
}





void cNoise3DGenerator::GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	for (size_t i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		a_BiomeMap[i] = biExtremeHills;
	}
}





void cNoise3DGenerator::DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc)
{
	NOISE_DATATYPE Noise[17 * 257 * 17];
	GenerateNoiseArray(a_ChunkX, a_ChunkZ, Noise);

	// Output noise into chunk:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int y = 0; y < cChunkDef::Height; y++)
		{
			int idx = z * 17 * 257 + y * 17;
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				NOISE_DATATYPE n = Noise[idx++];
				BLOCKTYPE BlockType;
				if (n > m_AirThreshold)
				{
					BlockType = (y > m_SeaLevel) ? E_BLOCK_AIR : E_BLOCK_STATIONARY_WATER;
				}
				else
				{
					BlockType = E_BLOCK_STONE;
				}
				a_ChunkDesc.SetBlockType(x, y, z, BlockType);
			}
		}
	}

	UpdateHeightmap(a_ChunkDesc);
	ComposeTerrain (a_ChunkDesc);
}





void cNoise3DGenerator::GenerateNoiseArray(int a_ChunkX, int a_ChunkZ, NOISE_DATATYPE * a_OutNoise)
{
	NOISE_DATATYPE NoiseO[DIM_X * DIM_Y * DIM_Z];  // Output for the Perlin noise
	NOISE_DATATYPE NoiseW[DIM_X * DIM_Y * DIM_Z];  // Workspace that the noise calculation can use and trash

	// Our noise array has different layout, XZY, instead of regular chunk's XYZ, that's why the coords are "renamed"
	NOISE_DATATYPE StartX = ((NOISE_DATATYPE)(a_ChunkX       * cChunkDef::Width))     / m_FrequencyX;
	NOISE_DATATYPE EndX   = ((NOISE_DATATYPE)((a_ChunkX + 1) * cChunkDef::Width) - 1) / m_FrequencyX;
	NOISE_DATATYPE StartZ = ((NOISE_DATATYPE)(a_ChunkZ       * cChunkDef::Width))     / m_FrequencyZ;
	NOISE_DATATYPE EndZ   = ((NOISE_DATATYPE)((a_ChunkZ + 1) * cChunkDef::Width) - 1) / m_FrequencyZ;
	NOISE_DATATYPE StartY = 0;
	NOISE_DATATYPE EndY   = ((NOISE_DATATYPE)256) / m_FrequencyY;

	m_Perlin.Generate3D(NoiseO, DIM_X, DIM_Y, DIM_Z, StartX, EndX, StartY, EndY, StartZ, EndZ, NoiseW);

	// DEBUG: Debug3DNoise(NoiseO, DIM_X, DIM_Y, DIM_Z, Printf("Chunk_%d_%d_orig", a_ChunkX, a_ChunkZ));

	// Precalculate a "height" array:
	NOISE_DATATYPE Height[DIM_X * DIM_Z];  // Output for the cubic noise heightmap ("source")
	m_Cubic.Generate2D(Height, DIM_X, DIM_Z, StartX / 25, EndX / 25, StartZ / 25, EndZ / 25);
	for (size_t i = 0; i < ARRAYCOUNT(Height); i++)
	{
		Height[i] = std::abs(Height[i]) * m_HeightAmplification + 1;
	}

	// Modify the noise by height data:
	for (int y = 0; y < DIM_Y; y++)
	{
		NOISE_DATATYPE AddHeight = (y * UPSCALE_Y - m_MidPoint) / 20;
		AddHeight *= AddHeight * AddHeight;
		for (int z = 0; z < DIM_Z; z++)
		{
			NOISE_DATATYPE * CurRow = &(NoiseO[y * DIM_X + z * DIM_X * DIM_Y]);
			for (int x = 0; x < DIM_X; x++)
			{
				CurRow[x] += AddHeight / Height[x + DIM_X * z];
			}
		}
	}

	// DEBUG: Debug3DNoise(NoiseO, DIM_X, DIM_Y, DIM_Z, Printf("Chunk_%d_%d_hei", a_ChunkX, a_ChunkZ));

	// Upscale the Perlin noise into full-blown chunk dimensions:
	LinearUpscale3DArray(
		NoiseO, DIM_X, DIM_Y, DIM_Z,
		a_OutNoise, UPSCALE_X, UPSCALE_Y, UPSCALE_Z
	);

	// DEBUG: Debug3DNoise(a_OutNoise, 17, 257, 17, Printf("Chunk_%d_%d_lerp", a_ChunkX, a_ChunkZ));
}





void cNoise3DGenerator::UpdateHeightmap(cChunkDesc & a_ChunkDesc)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			for (int y = cChunkDef::Height - 1; y > 0; y--)
			{
				if (a_ChunkDesc.GetBlockType(x, y, z) != E_BLOCK_AIR)
				{
					a_ChunkDesc.SetHeight(x, z, y);
					break;
				}
			}  // for y
		}  // for x
	}  // for z
}





void cNoise3DGenerator::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	// Make basic terrain composition:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int LastAir = a_ChunkDesc.GetHeight(x, z) + 1;
			bool HasHadWater = false;
			for (int y = LastAir - 1; y > 0; y--)
			{
				switch (a_ChunkDesc.GetBlockType(x, y, z))
				{
					case E_BLOCK_AIR:
					{
						LastAir = y;
						break;
					}
					case E_BLOCK_STONE:
					{
						if (LastAir - y > 3)
						{
							break;
						}
						if (HasHadWater)
						{
							a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_SAND);
						}
						else
						{
							a_ChunkDesc.SetBlockType(x, y, z, (LastAir == y + 1) ? E_BLOCK_GRASS : E_BLOCK_DIRT);
						}
						break;
					}
					case E_BLOCK_STATIONARY_WATER:
					{
						LastAir = y;
						HasHadWater = true;
						break;
					}
				}  // switch (GetBlockType())
			}  // for y
			a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
		}  // for x
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cNoise3DComposable:

cNoise3DComposable::cNoise3DComposable(int a_Seed) :
	m_ChoiceNoise(a_Seed),
	m_DensityNoiseA(a_Seed + 1),
	m_DensityNoiseB(a_Seed + 2),
	m_BaseNoise(a_Seed + 3)
{
}





void cNoise3DComposable::Initialize(cIniFile & a_IniFile)
{
	// Params:
	// The defaults generate extreme hills terrain
	m_SeaLevel            =                 a_IniFile.GetValueSetI("Generator", "Noise3DSeaLevel", 62);
	m_HeightAmplification = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DHeightAmplification", 0.045);
	m_MidPoint            = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DMidPoint", 75);
	m_FrequencyX          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyX", 40);
	m_FrequencyY          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyY", 40);
	m_FrequencyZ          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyZ", 40);
	m_BaseFrequencyX      = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DBaseFrequencyX", 40);
	m_BaseFrequencyZ      = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DBaseFrequencyZ", 40);
	m_ChoiceFrequencyX    = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DChoiceFrequencyX", 40);
	m_ChoiceFrequencyY    = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DChoiceFrequencyY", 80);
	m_ChoiceFrequencyZ    = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DChoiceFrequencyZ", 40);
	m_AirThreshold        = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DAirThreshold", 0);
	int NumChoiceOctaves  = a_IniFile.GetValueSetI("Generator", "Noise3DNumChoiceOctaves",  4);
	int NumDensityOctaves = a_IniFile.GetValueSetI("Generator", "Noise3DNumDensityOctaves", 6);
	int NumBaseOctaves    = a_IniFile.GetValueSetI("Generator", "Noise3DNumBaseOctaves",    6);
	NOISE_DATATYPE BaseNoiseAmplitude = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DBaseAmplitude", 1);

	// Add octaves for the choice noise:
	NOISE_DATATYPE wavlen = 1, ampl = 0.5;
	for (int i = 0; i < NumChoiceOctaves; i++)
	{
		m_ChoiceNoise.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}

	// Add octaves for the density noises:
	wavlen = 1, ampl = 1;
	for (int i = 0; i < NumDensityOctaves; i++)
	{
		m_DensityNoiseA.AddOctave(wavlen, ampl);
		m_DensityNoiseB.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}

	// Add octaves for the base noise:
	wavlen = 1, ampl = BaseNoiseAmplitude;
	for (int i = 0; i < NumBaseOctaves; i++)
	{
		m_BaseNoise.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}
}





void cNoise3DComposable::GenerateNoiseArrayIfNeeded(int a_ChunkX, int a_ChunkZ)
{
	if ((a_ChunkX == m_LastChunkX) && (a_ChunkZ == m_LastChunkZ))
	{
		// The noise for this chunk is already generated in m_NoiseArray
		return;
	}
	m_LastChunkX = a_ChunkX;
	m_LastChunkZ = a_ChunkZ;

	// Generate all the noises:
	NOISE_DATATYPE ChoiceNoise[5 * 5 * 33];
	NOISE_DATATYPE Workspace[5 * 5 * 33];
	NOISE_DATATYPE DensityNoiseA[5 * 5 * 33];
	NOISE_DATATYPE DensityNoiseB[5 * 5 * 33];
	NOISE_DATATYPE BaseNoise[5 * 5];
	NOISE_DATATYPE BlockX = static_cast<NOISE_DATATYPE>(a_ChunkX * cChunkDef::Width);
	NOISE_DATATYPE BlockZ = static_cast<NOISE_DATATYPE>(a_ChunkZ * cChunkDef::Width);
	// Note that we have to swap the coords, because noise generator uses [x + SizeX * y + SizeX * SizeY * z] ordering and we want "BlockY" to be "z":
	m_ChoiceNoise.Generate3D  (ChoiceNoise,   5, 5, 33, BlockX / m_ChoiceFrequencyX, (BlockX + 17) / m_ChoiceFrequencyX, BlockZ / m_ChoiceFrequencyZ, (BlockZ + 17) / m_ChoiceFrequencyZ, 0, 257 / m_ChoiceFrequencyY, Workspace);
	m_DensityNoiseA.Generate3D(DensityNoiseA, 5, 5, 33, BlockX / m_FrequencyX,       (BlockX + 17) / m_FrequencyX,       BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ,       0, 257 / m_FrequencyY,       Workspace);
	m_DensityNoiseB.Generate3D(DensityNoiseB, 5, 5, 33, BlockX / m_FrequencyX,       (BlockX + 17) / m_FrequencyX,       BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ,       0, 257 / m_FrequencyY,       Workspace);
	m_BaseNoise.Generate2D    (BaseNoise,     5, 5,     BlockX / m_BaseFrequencyX,   (BlockX + 17) / m_BaseFrequencyX,   BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ, Workspace);

	// Calculate the final noise based on the partial noises:
	for (int y = 0; y < 33; y++)
	{
		NOISE_DATATYPE AddHeight = (static_cast<NOISE_DATATYPE>(y * 8) - m_MidPoint) * m_HeightAmplification;

		// If "underground", make the terrain smoother by forcing the vertical linear gradient into steeper slope:
		if (AddHeight < 0)
		{
			AddHeight *= 4;
		}

		for (int z = 0; z < 5; z++)
		{
			for (int x = 0; x < 5; x++)
			{
				int idx = x + 5 * z + 5 * 5 * y;
				Workspace[idx] = ClampedLerp(DensityNoiseA[idx], DensityNoiseB[idx], 8 * (ChoiceNoise[idx] + 0.5f)) + AddHeight + BaseNoise[x + 5 * z];
			}
		}
	}
	LinearUpscale3DArray<NOISE_DATATYPE>(Workspace, 5, 5, 33, m_NoiseArray, 4, 4, 8);
}





void cNoise3DComposable::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	GenerateNoiseArrayIfNeeded(a_ChunkX, a_ChunkZ);

	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			cChunkDef::SetHeight(a_HeightMap, x, z, m_SeaLevel);
			for (int y = cChunkDef::Height - 1; y > m_SeaLevel; y--)
			{
				if (m_NoiseArray[y * 17 * 17 + z * 17 + x] <= m_AirThreshold)
				{
					cChunkDef::SetHeight(a_HeightMap, x, z, y);
					break;
				}
			}  // for y
		}  // for x
	}  // for z
}





void cNoise3DComposable::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	GenerateNoiseArrayIfNeeded(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ());

	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);

	// Make basic terrain composition:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int LastAir = a_ChunkDesc.GetHeight(x, z) + 1;
			bool HasHadWater = false;
			for (int y = LastAir; y < m_SeaLevel; y++)
			{
				a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_STATIONARY_WATER);
			}
			for (int y = LastAir - 1; y > 0; y--)
			{
				if (m_NoiseArray[x + 17 * z + 17 * 17 * y] > m_AirThreshold)
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
				if (LastAir - y > 4)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_STONE);
					continue;
				}
				if (HasHadWater)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_SAND);
				}
				else
				{
					a_ChunkDesc.SetBlockType(x, y, z, (LastAir == y + 1) ? E_BLOCK_GRASS : E_BLOCK_DIRT);
				}
			}  // for y
			a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
		}  // for x
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cBiomalNoise3DComposable:

cBiomalNoise3DComposable::cBiomalNoise3DComposable(int a_Seed, cBiomeGenPtr a_BiomeGen) :
	m_ChoiceNoise(a_Seed),
	m_DensityNoiseA(a_Seed + 1),
	m_DensityNoiseB(a_Seed + 2),
	m_BaseNoise(a_Seed + 3),
	m_BiomeGen(a_BiomeGen)
{
	// Generate the weight distribution for summing up neighboring biomes:
	m_WeightSum = 0;
	for (int z = 0; z <= AVERAGING_SIZE * 2; z++)
	{
		for (int x = 0; x <= AVERAGING_SIZE * 2; x++)
		{
			m_Weight[z][x] = static_cast<NOISE_DATATYPE>((5 - std::abs(5 - x)) + (5 - std::abs(5 - z)));
			m_WeightSum += m_Weight[z][x];
		}
	}
}





void cBiomalNoise3DComposable::Initialize(cIniFile & a_IniFile)
{
	// Params:
	// The defaults generate extreme hills terrain
	m_SeaLevel            =                 a_IniFile.GetValueSetI("Generator", "BiomalNoise3DSeaLevel", 62);
	m_FrequencyX          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DFrequencyX", 40);
	m_FrequencyY          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DFrequencyY", 40);
	m_FrequencyZ          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DFrequencyZ", 40);
	m_BaseFrequencyX      = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DBaseFrequencyX", 40);
	m_BaseFrequencyZ      = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DBaseFrequencyZ", 40);
	m_ChoiceFrequencyX    = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DChoiceFrequencyX", 40);
	m_ChoiceFrequencyY    = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DChoiceFrequencyY", 80);
	m_ChoiceFrequencyZ    = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DChoiceFrequencyZ", 40);
	m_AirThreshold        = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DAirThreshold", 0);
	int NumChoiceOctaves  = a_IniFile.GetValueSetI("Generator", "BiomalNoise3DNumChoiceOctaves",  4);
	int NumDensityOctaves = a_IniFile.GetValueSetI("Generator", "BiomalNoise3DNumDensityOctaves", 6);
	int NumBaseOctaves    = a_IniFile.GetValueSetI("Generator", "BiomalNoise3DNumBaseOctaves",    6);
	NOISE_DATATYPE BaseNoiseAmplitude = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "BiomalNoise3DBaseAmplitude", 1);

	// Add octaves for the choice noise:
	NOISE_DATATYPE wavlen = 1, ampl = 0.5;
	for (int i = 0; i < NumChoiceOctaves; i++)
	{
		m_ChoiceNoise.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}

	// Add octaves for the density noises:
	wavlen = 1, ampl = 1;
	for (int i = 0; i < NumDensityOctaves; i++)
	{
		m_DensityNoiseA.AddOctave(wavlen, ampl);
		m_DensityNoiseB.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}

	// Add octaves for the base noise:
	wavlen = 1, ampl = BaseNoiseAmplitude;
	for (int i = 0; i < NumBaseOctaves; i++)
	{
		m_BaseNoise.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}
}





void cBiomalNoise3DComposable::GenerateNoiseArrayIfNeeded(int a_ChunkX, int a_ChunkZ)
{
	if ((a_ChunkX == m_LastChunkX) && (a_ChunkZ == m_LastChunkZ))
	{
		// The noise for this chunk is already generated in m_NoiseArray
		return;
	}
	m_LastChunkX = a_ChunkX;
	m_LastChunkZ = a_ChunkZ;

	// Calculate the parameters for the biomes:
	ChunkParam MidPoint;
	ChunkParam HeightAmp;
	CalcBiomeParamArrays(a_ChunkX, a_ChunkZ, HeightAmp, MidPoint);

	// Generate all the noises:
	NOISE_DATATYPE ChoiceNoise[5 * 5 * 33];
	NOISE_DATATYPE Workspace[5 * 5 * 33];
	NOISE_DATATYPE DensityNoiseA[5 * 5 * 33];
	NOISE_DATATYPE DensityNoiseB[5 * 5 * 33];
	NOISE_DATATYPE BaseNoise[5 * 5];
	NOISE_DATATYPE BlockX = static_cast<NOISE_DATATYPE>(a_ChunkX * cChunkDef::Width);
	NOISE_DATATYPE BlockZ = static_cast<NOISE_DATATYPE>(a_ChunkZ * cChunkDef::Width);
	// Note that we have to swap the coords, because noise generator uses [x + SizeX * y + SizeX * SizeY * z] ordering and we want "BlockY" to be "z":
	m_ChoiceNoise.Generate3D  (ChoiceNoise,   5, 5, 33, BlockX / m_ChoiceFrequencyX, (BlockX + 17) / m_ChoiceFrequencyX, BlockZ / m_ChoiceFrequencyZ, (BlockZ + 17) / m_ChoiceFrequencyZ, 0, 257 / m_ChoiceFrequencyY, Workspace);
	m_DensityNoiseA.Generate3D(DensityNoiseA, 5, 5, 33, BlockX / m_FrequencyX,       (BlockX + 17) / m_FrequencyX,       BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ,       0, 257 / m_FrequencyY,       Workspace);
	m_DensityNoiseB.Generate3D(DensityNoiseB, 5, 5, 33, BlockX / m_FrequencyX,       (BlockX + 17) / m_FrequencyX,       BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ,       0, 257 / m_FrequencyY,       Workspace);
	m_BaseNoise.Generate2D    (BaseNoise,     5, 5,     BlockX / m_BaseFrequencyX,   (BlockX + 17) / m_BaseFrequencyX,   BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ, Workspace);

	// Calculate the final noise based on the partial noises:
	for (int y = 0; y < 33; y++)
	{
		NOISE_DATATYPE BlockHeight = static_cast<NOISE_DATATYPE>(y * 8);
		for (int z = 0; z < 5; z++)
		{
			for (int x = 0; x < 5; x++)
			{
				NOISE_DATATYPE AddHeight = (BlockHeight - MidPoint[x + 5 * z]) * HeightAmp[x + 5 * z];

				// If "underground", make the terrain smoother by forcing the vertical linear gradient into steeper slope:
				if (AddHeight < 0)
				{
					AddHeight *= 4;
				}

				int idx = x + 5 * z + 5 * 5 * y;
				Workspace[idx] = ClampedLerp(DensityNoiseA[idx], DensityNoiseB[idx], 8 * (ChoiceNoise[idx] + 0.5f)) + AddHeight + BaseNoise[x + 5 * z];
			}
		}
	}
	LinearUpscale3DArray<NOISE_DATATYPE>(Workspace, 5, 5, 33, m_NoiseArray, 4, 4, 8);
}





void cBiomalNoise3DComposable::CalcBiomeParamArrays(int a_ChunkX, int a_ChunkZ, ChunkParam & a_HeightAmp, ChunkParam & a_MidPoint)
{
	// Generate the 3*3 chunks of biomes around this chunk:
	cChunkDef::BiomeMap neighborBiomes[3 * 3];
	for (int z = 0; z < 3; z++)
	{
		for (int x = 0; x < 3; x++)
		{
			m_BiomeGen->GenBiomes(a_ChunkX + x - 1, a_ChunkZ + z - 1, neighborBiomes[x + 3 * z]);
		}
	}

	// Sum up the biome values:
	for (int z = 0; z < 5; z++)
	{
		for (int x = 0; x < 5; x++)
		{
			NOISE_DATATYPE totalHeightAmp = 0;
			NOISE_DATATYPE totalMidPoint = 0;
			// Add up the biomes around this point:
			for (int relz = 0; relz <= AVERAGING_SIZE * 2; ++relz)
			{
				int colz = 16 + z * 4 + relz - AVERAGING_SIZE;   // Biome Z coord relative to the neighborBiomes start
				int neicellz = colz / 16;	                       // Chunk Z coord relative to the neighborBiomes start
				int neirelz  = colz % 16;	                       // Biome Z coord relative to cz in neighborBiomes
				for (int relx = 0; relx <= AVERAGING_SIZE * 2; ++relx)
				{
					int colx = 16 + x * 4 + relx - AVERAGING_SIZE;   // Biome X coord relative to the neighborBiomes start
					int neicellx = colx / 16;	                       // Chunk X coord relative to the neighborBiomes start
					int neirelx  = colx % 16;	                       // Biome X coord relative to cz in neighborBiomes
					EMCSBiome biome = cChunkDef::GetBiome(neighborBiomes[neicellx + neicellz * 3], neirelx, neirelz);
					NOISE_DATATYPE heightAmp, midPoint;
					GetBiomeParams(biome, heightAmp, midPoint);
					totalHeightAmp += heightAmp * m_Weight[relz][relx];
					totalMidPoint += midPoint * m_Weight[relz][relx];
				}  // for relx
			}  // for relz
			a_HeightAmp[x + 5 * z] = totalHeightAmp / m_WeightSum;
			a_MidPoint[x + 5 * z] = totalMidPoint / m_WeightSum;
		}  // for x
	}  // for z
}





void cBiomalNoise3DComposable::GetBiomeParams(EMCSBiome a_Biome, NOISE_DATATYPE & a_HeightAmp, NOISE_DATATYPE & a_MidPoint)
{
	switch (a_Biome)
	{
		case biDeepOcean:        a_HeightAmp = 0.17f;  a_MidPoint = 35; break;
		case biDesert:           a_HeightAmp = 0.29f;  a_MidPoint = 62; break;  // Needs verification
		case biExtremeHills:     a_HeightAmp = 0.045f; a_MidPoint = 75; break;
		case biExtremeHillsPlus: a_HeightAmp = 0.04f;  a_MidPoint = 80; break;
		case biFrozenRiver:      a_HeightAmp = 0.4f;   a_MidPoint = 53; break;
		case biFrozenOcean:      a_HeightAmp = 0.17f;  a_MidPoint = 47; break;
		case biJungle:           a_HeightAmp = 0.1f;   a_MidPoint = 63; break;
		case biJungleM:          a_HeightAmp = 0.1f;   a_MidPoint = 63; break;
		case biOcean:            a_HeightAmp = 0.17f;  a_MidPoint = 47; break;
		case biPlains:           a_HeightAmp = 0.3f;   a_MidPoint = 62; break;  // Needs verification
		case biRiver:            a_HeightAmp = 0.4f;   a_MidPoint = 53; break;
		case biSwampland:        a_HeightAmp = 0.25f;  a_MidPoint = 59; break;
		case biSwamplandM:       a_HeightAmp = 0.11f;  a_MidPoint = 59; break;

		default:
		{
			// Make a crazy terrain so that it stands out
			a_HeightAmp = 0.001f;
			a_MidPoint = 90;
			break;
		}
	}
}





void cBiomalNoise3DComposable::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	GenerateNoiseArrayIfNeeded(a_ChunkX, a_ChunkZ);

	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			cChunkDef::SetHeight(a_HeightMap, x, z, m_SeaLevel);
			for (int y = cChunkDef::Height - 1; y > m_SeaLevel; y--)
			{
				if (m_NoiseArray[y * 17 * 17 + z * 17 + x] <= m_AirThreshold)
				{
					cChunkDef::SetHeight(a_HeightMap, x, z, y);
					break;
				}
			}  // for y
		}  // for x
	}  // for z
}





void cBiomalNoise3DComposable::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	GenerateNoiseArrayIfNeeded(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ());

	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);

	// Make basic terrain composition:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int LastAir = a_ChunkDesc.GetHeight(x, z) + 1;
			bool HasHadWater = false;
			for (int y = LastAir; y < m_SeaLevel; y++)
			{
				a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_STATIONARY_WATER);
			}
			for (int y = LastAir - 1; y > 0; y--)
			{
				if (m_NoiseArray[x + 17 * z + 17 * 17 * y] > m_AirThreshold)
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
				if (LastAir - y > 4)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_STONE);
					continue;
				}
				if (HasHadWater)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_SAND);
				}
				else
				{
					a_ChunkDesc.SetBlockType(x, y, z, (LastAir == y + 1) ? E_BLOCK_GRASS : E_BLOCK_DIRT);
				}
			}  // for y
			a_ChunkDesc.SetBlockType(x, 0, z, E_BLOCK_BEDROCK);
		}  // for x
	}  // for z
}




