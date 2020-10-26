
// Nosie3DGenerator.cpp

// Generates terrain using 3D noise, rather than composing. Is a test.

#include "Globals.h"
#include "Noise3DGenerator.h"
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





#if 0
// Perform speed test of the cInterpolNoise class
static class cInterpolNoiseSpeedTest
{
public:
	cInterpolNoiseSpeedTest(void)
	{
		TestSpeed2D();
		TestSpeed3D();
		printf("InterpolNoise speed comparison finished.\n");
	}


	/** Compare the speed of the 3D InterpolNoise vs 3D CubicNoise. */
	void TestSpeed3D(void)
	{
		printf("Evaluating 3D noise performance...\n");
		static const int SIZE_X = 128;
		static const int SIZE_Y = 128;
		static const int SIZE_Z = 128;
		static const NOISE_DATATYPE MUL = 80;
		std::unique_ptr<NOISE_DATATYPE[]> arr(new NOISE_DATATYPE[SIZE_X * SIZE_Y * SIZE_Z]);
		cTimer timer;

		// Test the cInterpolNoise:
		cInterpolNoise<Interp5Deg> interpNoise(1);
		long long start = timer.GetNowTime();
		for (int i = 0; i < 30; i++)
		{
			interpNoise.Generate3D(arr.get(), SIZE_X, SIZE_Y, SIZE_Z, MUL * i, MUL * i + MUL, 0, MUL, 0, MUL);
		}
		long long end = timer.GetNowTime();
		printf("InterpolNoise took %.02f sec\n", static_cast<float>(end - start) / 1000);

		// Test the cCubicNoise:
		cCubicNoise cubicNoise(1);
		start = timer.GetNowTime();
		for (int i = 0; i < 30; i++)
		{
			cubicNoise.Generate3D(arr.get(), SIZE_X, SIZE_Y, SIZE_Z, MUL * i, MUL * i + MUL, 0, MUL, 0, MUL);
		}
		end = timer.GetNowTime();
		printf("CubicNoise took %.02f sec\n", static_cast<float>(end - start) / 1000);
		printf("3D noise performance comparison finished.\n");
	}


	/** Compare the speed of the 2D InterpolNoise vs 2D CubicNoise. */
	void TestSpeed2D(void)
	{
		printf("Evaluating 2D noise performance...\n");
		static const int SIZE_X = 128;
		static const int SIZE_Y = 128;
		static const NOISE_DATATYPE MUL = 80;
		std::unique_ptr<NOISE_DATATYPE[]> arr(new NOISE_DATATYPE[SIZE_X * SIZE_Y]);
		cTimer timer;

		// Test the cInterpolNoise:
		cInterpolNoise<Interp5Deg> interpNoise(1);
		long long start = timer.GetNowTime();
		for (int i = 0; i < 500; i++)
		{
			interpNoise.Generate2D(arr.get(), SIZE_X, SIZE_Y, MUL * i, MUL * i + MUL, 0, MUL);
		}
		long long end = timer.GetNowTime();
		printf("InterpolNoise took %.02f sec\n", static_cast<float>(end - start) / 1000);

		// Test the cCubicNoise:
		cCubicNoise cubicNoise(1);
		start = timer.GetNowTime();
		for (int i = 0; i < 500; i++)
		{
			cubicNoise.Generate2D(arr.get(), SIZE_X, SIZE_Y, MUL * i, MUL * i + MUL, 0, MUL);
		}
		end = timer.GetNowTime();
		printf("CubicNoise took %.02f sec\n", static_cast<float>(end - start) / 1000);
		printf("2D noise performance comparison finished.\n");
	}
} g_InterpolNoiseSpeedTest;
#endif





////////////////////////////////////////////////////////////////////////////////
// cNoise3DGenerator:

cNoise3DGenerator::cNoise3DGenerator():
	Super(),
	m_Perlin(1000),
	m_Cubic(1000)
{
	m_Perlin.AddOctave(1,  1);
	m_Perlin.AddOctave(2,  0.5);
	m_Perlin.AddOctave(4,  0.25);
	m_Perlin.AddOctave(8,  0.125);
	m_Perlin.AddOctave(16, 0.0625);

	m_Cubic.AddOctave(1,  1);
	m_Cubic.AddOctave(2,  0.5);
	m_Cubic.AddOctave(4,  0.25);
	m_Cubic.AddOctave(8,  0.125);
	m_Cubic.AddOctave(16, 0.0625);
}





cNoise3DGenerator::~cNoise3DGenerator()
{
	// Nothing needed yet
}





void cNoise3DGenerator::Initialize(cIniFile & a_IniFile)
{
	// Params:
	m_SeaLevel            =                 a_IniFile.GetValueSetI("Generator", "SeaLevel", 62);
	m_HeightAmplification = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DHeightAmplification", 0.1));
	m_MidPoint            = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DMidPoint", 68));
	m_FrequencyX          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyX", 8));
	m_FrequencyY          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyY", 8));
	m_FrequencyZ          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyZ", 8));
	m_AirThreshold        = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DAirThreshold", 0.5));
}





void cNoise3DGenerator::GenerateBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap)
{
	UNUSED(a_ChunkCoords);
	for (size_t i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		a_BiomeMap[i] = biExtremeHills;
	}
}





void cNoise3DGenerator::Generate(cChunkDesc & a_ChunkDesc)
{
	NOISE_DATATYPE Noise[17 * 257 * 17];
	GenerateNoiseArray(a_ChunkDesc.GetChunkCoords(), Noise);

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

	a_ChunkDesc.UpdateHeightmap();
	ComposeTerrain (a_ChunkDesc);
}





void cNoise3DGenerator::GenerateNoiseArray(cChunkCoords a_ChunkCoords, NOISE_DATATYPE * a_OutNoise)
{
	NOISE_DATATYPE NoiseO[DIM_X * DIM_Y * DIM_Z];  // Output for the Perlin noise
	NOISE_DATATYPE NoiseW[DIM_X * DIM_Y * DIM_Z];  // Workspace that the noise calculation can use and trash

	// Our noise array has different layout, XZY, instead of regular chunk's XYZ, that's why the coords are "renamed"
	NOISE_DATATYPE StartX = static_cast<NOISE_DATATYPE>(a_ChunkCoords.m_ChunkX       * cChunkDef::Width) / m_FrequencyX;
	NOISE_DATATYPE EndX   = static_cast<NOISE_DATATYPE>((a_ChunkCoords.m_ChunkX + 1) * cChunkDef::Width) / m_FrequencyX;
	NOISE_DATATYPE StartZ = static_cast<NOISE_DATATYPE>(a_ChunkCoords.m_ChunkZ       * cChunkDef::Width) / m_FrequencyZ;
	NOISE_DATATYPE EndZ   = static_cast<NOISE_DATATYPE>((a_ChunkCoords.m_ChunkZ + 1) * cChunkDef::Width) / m_FrequencyZ;
	NOISE_DATATYPE StartY = 0;
	NOISE_DATATYPE EndY   = static_cast<NOISE_DATATYPE>(256) / m_FrequencyY;

	m_Perlin.Generate3D(NoiseO, DIM_X, DIM_Y, DIM_Z, StartX, EndX, StartY, EndY, StartZ, EndZ, NoiseW);

	// Precalculate a "height" array:
	NOISE_DATATYPE Height[DIM_X * DIM_Z];  // Output for the cubic noise heightmap ("source")
	m_Cubic.Generate2D(Height, DIM_X, DIM_Z, StartX / 5, EndX / 5, StartZ / 5, EndZ / 5);
	for (size_t i = 0; i < ARRAYCOUNT(Height); i++)
	{
		Height[i] = Height[i] * m_HeightAmplification;
	}

	// Modify the noise by height data:
	for (int y = 0; y < DIM_Y; y++)
	{
		NOISE_DATATYPE AddHeight = (y * UPSCALE_Y - m_MidPoint) / 30;
		// AddHeight *= AddHeight * AddHeight;
		for (int z = 0; z < DIM_Z; z++)
		{
			NOISE_DATATYPE * CurRow = &(NoiseO[y * DIM_X + z * DIM_X * DIM_Y]);
			for (int x = 0; x < DIM_X; x++)
			{
				CurRow[x] += AddHeight + Height[x + DIM_X * z];
			}
		}
	}

	// Upscale the Perlin noise into full-blown chunk dimensions:
	LinearUpscale3DArray(
		NoiseO, DIM_X, DIM_Y, DIM_Z,
		a_OutNoise, UPSCALE_X, UPSCALE_Y, UPSCALE_Z
	);
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
	m_BaseNoise(a_Seed + 3),
	m_HeightAmplification(0.0),
	m_MidPoint(0.0),
	m_FrequencyX(0.0),
	m_FrequencyY(0.0),
	m_FrequencyZ(0.0),
	m_BaseFrequencyX(0.0),
	m_BaseFrequencyZ(0.0),
	m_ChoiceFrequencyX(0.0),
	m_ChoiceFrequencyY(0.0),
	m_ChoiceFrequencyZ(0.0),
	m_AirThreshold(0.0),
	m_LastChunkCoords(0x7fffffff, 0x7fffffff)  // Use dummy coords that won't ever be used by real chunks
{
}





void cNoise3DComposable::Initialize(cIniFile & a_IniFile)
{
	// Params:
	// The defaults generate extreme hills terrain
	m_HeightAmplification = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DHeightAmplification", 0.045));
	m_MidPoint            = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DMidPoint", 75));
	m_FrequencyX          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyX", 40));
	m_FrequencyY          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyY", 40));
	m_FrequencyZ          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyZ", 40));
	m_BaseFrequencyX      = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DBaseFrequencyX", 40));
	m_BaseFrequencyZ      = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DBaseFrequencyZ", 40));
	m_ChoiceFrequencyX    = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DChoiceFrequencyX", 40));
	m_ChoiceFrequencyY    = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DChoiceFrequencyY", 80));
	m_ChoiceFrequencyZ    = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DChoiceFrequencyZ", 40));
	m_AirThreshold        = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DAirThreshold", 0));
	int NumChoiceOctaves  = a_IniFile.GetValueSetI("Generator", "Noise3DNumChoiceOctaves",  4);
	int NumDensityOctaves = a_IniFile.GetValueSetI("Generator", "Noise3DNumDensityOctaves", 6);
	int NumBaseOctaves    = a_IniFile.GetValueSetI("Generator", "Noise3DNumBaseOctaves",    6);
	NOISE_DATATYPE BaseNoiseAmplitude = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "Noise3DBaseAmplitude", 1));

	// Add octaves for the choice noise:
	NOISE_DATATYPE wavlen = 1, ampl = 0.5;
	for (int i = 0; i < NumChoiceOctaves; i++)
	{
		m_ChoiceNoise.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}

	// Add octaves for the density noises:
	wavlen = 1; ampl = 1;
	for (int i = 0; i < NumDensityOctaves; i++)
	{
		m_DensityNoiseA.AddOctave(wavlen, ampl);
		m_DensityNoiseB.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}

	// Add octaves for the base noise:
	wavlen = 1; ampl = BaseNoiseAmplitude;
	for (int i = 0; i < NumBaseOctaves; i++)
	{
		m_BaseNoise.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}
}





void cNoise3DComposable::GenerateNoiseArrayIfNeeded(cChunkCoords a_ChunkCoords)
{
	if (a_ChunkCoords == m_LastChunkCoords)
	{
		// The noise for this chunk is already generated in m_NoiseArray
		return;
	}
	m_LastChunkCoords = a_ChunkCoords;

	// Generate all the noises:
	NOISE_DATATYPE ChoiceNoise[5 * 5 * 33];
	NOISE_DATATYPE Workspace[5 * 5 * 33];
	NOISE_DATATYPE DensityNoiseA[5 * 5 * 33];
	NOISE_DATATYPE DensityNoiseB[5 * 5 * 33];
	NOISE_DATATYPE BaseNoise[5 * 5];
	NOISE_DATATYPE BlockX = static_cast<NOISE_DATATYPE>(a_ChunkCoords.m_ChunkX * cChunkDef::Width);
	NOISE_DATATYPE BlockZ = static_cast<NOISE_DATATYPE>(a_ChunkCoords.m_ChunkZ * cChunkDef::Width);
	// Note that we have to swap the X and Y coords, because noise generator uses [x + SizeX * y + SizeX * SizeY * z] ordering and we want "BlockY" to be "x":
	m_ChoiceNoise.Generate3D  (ChoiceNoise,   33, 5, 5, 0, 257 / m_ChoiceFrequencyY, BlockX / m_ChoiceFrequencyX, (BlockX + 17) / m_ChoiceFrequencyX, BlockZ / m_ChoiceFrequencyZ, (BlockZ + 17) / m_ChoiceFrequencyZ, Workspace);
	m_DensityNoiseA.Generate3D(DensityNoiseA, 33, 5, 5, 0, 257 / m_FrequencyY,       BlockX / m_FrequencyX,       (BlockX + 17) / m_FrequencyX,       BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ,       Workspace);
	m_DensityNoiseB.Generate3D(DensityNoiseB, 33, 5, 5, 0, 257 / m_FrequencyY,       BlockX / m_FrequencyX,       (BlockX + 17) / m_FrequencyX,       BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ,       Workspace);
	m_BaseNoise.Generate2D    (BaseNoise,     5, 5,     BlockX / m_BaseFrequencyX,   (BlockX + 17) / m_BaseFrequencyX,   BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ, Workspace);

	// Calculate the final noise based on the partial noises:
	for (int z = 0; z < 5; z++)
	{
		for (int x = 0; x < 5; x++)
		{
			NOISE_DATATYPE curBaseNoise = BaseNoise[x + 5 * z];
			for (int y = 0; y < 33; y++)
			{
				NOISE_DATATYPE AddHeight = (static_cast<NOISE_DATATYPE>(y * 8) - m_MidPoint) * m_HeightAmplification;

				// If "underground", make the terrain smoother by forcing the vertical linear gradient into steeper slope:
				if (AddHeight < 0)
				{
					AddHeight *= 4;
				}

				// If too high, cut off any terrain:
				if (y > 28)
				{
					AddHeight = AddHeight + static_cast<NOISE_DATATYPE>(y - 28) / 4;
				}

				// Decide between the two density noises:
				int idx = 33 * x + 33 * 5 * z + y;
				Workspace[idx] = ClampedLerp(DensityNoiseA[idx], DensityNoiseB[idx], 8 * (ChoiceNoise[idx] + 0.5f)) + AddHeight + curBaseNoise;
			}
		}
	}
	LinearUpscale3DArray<NOISE_DATATYPE>(Workspace, 33, 5, 5, m_NoiseArray, 8, 4, 4);
}





void cNoise3DComposable::GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape)
{
	GenerateNoiseArrayIfNeeded(a_ChunkCoords);

	// Translate the noise array into Shape:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				a_Shape[y + x * 256 + z * 256 * 16] = (m_NoiseArray[y + 257 * x + 257 * 17 * z] > m_AirThreshold) ? 0 : 1;
			}
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
	m_BiomeGen(std::move(a_BiomeGen)),
	m_LastChunkCoords(0x7fffffff, 0x7fffffff)  // Set impossible coords for the chunk so that it's always considered stale
{
	// Generate the weight distribution for summing up neighboring biomes:
	m_WeightSum = 0;
	for (int z = 0; z <= AVERAGING_SIZE * 2; z++)
	{
		for (int x = 0; x <= AVERAGING_SIZE * 2; x++)
		{
			m_Weight[z][x] = static_cast<NOISE_DATATYPE>((AVERAGING_SIZE - std::abs(AVERAGING_SIZE - x)) + (AVERAGING_SIZE - std::abs(AVERAGING_SIZE - z)));
			m_WeightSum += m_Weight[z][x];
		}
	}
}





void cBiomalNoise3DComposable::Initialize(cIniFile & a_IniFile)
{
	// Params:
	// The defaults generate extreme hills terrain
	m_SeaLevel            =                 a_IniFile.GetValueSetI("Generator", "SeaLevel", 62);
	m_FrequencyX          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DFrequencyX", 40));
	m_FrequencyY          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DFrequencyY", 40));
	m_FrequencyZ          = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DFrequencyZ", 40));
	m_BaseFrequencyX      = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DBaseFrequencyX", 40));
	m_BaseFrequencyZ      = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DBaseFrequencyZ", 40));
	m_ChoiceFrequencyX    = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DChoiceFrequencyX", 40));
	m_ChoiceFrequencyY    = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DChoiceFrequencyY", 80));
	m_ChoiceFrequencyZ    = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DChoiceFrequencyZ", 40));
	m_AirThreshold        = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DAirThreshold", 0));
	int NumChoiceOctaves  = a_IniFile.GetValueSetI("Generator", "BiomalNoise3DNumChoiceOctaves",  4);
	int NumDensityOctaves = a_IniFile.GetValueSetI("Generator", "BiomalNoise3DNumDensityOctaves", 6);
	int NumBaseOctaves    = a_IniFile.GetValueSetI("Generator", "BiomalNoise3DNumBaseOctaves",    6);
	NOISE_DATATYPE BaseNoiseAmplitude = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "BiomalNoise3DBaseAmplitude", 1));

	// Add octaves for the choice noise:
	NOISE_DATATYPE wavlen = 1, ampl = 0.5;
	for (int i = 0; i < NumChoiceOctaves; i++)
	{
		m_ChoiceNoise.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}

	// Add octaves for the density noises:
	wavlen = 1; ampl = 1;
	for (int i = 0; i < NumDensityOctaves; i++)
	{
		m_DensityNoiseA.AddOctave(wavlen, ampl);
		m_DensityNoiseB.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}

	// Add octaves for the base noise:
	wavlen = 1; ampl = BaseNoiseAmplitude;
	for (int i = 0; i < NumBaseOctaves; i++)
	{
		m_BaseNoise.AddOctave(wavlen, ampl);
		wavlen = wavlen * 2;
		ampl = ampl / 2;
	}
}





void cBiomalNoise3DComposable::GenerateNoiseArrayIfNeeded(cChunkCoords a_ChunkCoords)
{
	if (a_ChunkCoords == m_LastChunkCoords)
	{
		// The noise for this chunk is already generated in m_NoiseArray
		return;
	}
	m_LastChunkCoords = a_ChunkCoords;

	// Calculate the parameters for the biomes:
	ChunkParam MidPoint;
	ChunkParam HeightAmp;
	CalcBiomeParamArrays(a_ChunkCoords, HeightAmp, MidPoint);

	// Generate all the noises:
	NOISE_DATATYPE ChoiceNoise[5 * 5 * 33];
	NOISE_DATATYPE Workspace[5 * 5 * 33];
	NOISE_DATATYPE DensityNoiseA[5 * 5 * 33];
	NOISE_DATATYPE DensityNoiseB[5 * 5 * 33];
	NOISE_DATATYPE BaseNoise[5 * 5];
	NOISE_DATATYPE BlockX = static_cast<NOISE_DATATYPE>(a_ChunkCoords.m_ChunkX * cChunkDef::Width);
	NOISE_DATATYPE BlockZ = static_cast<NOISE_DATATYPE>(a_ChunkCoords.m_ChunkZ * cChunkDef::Width);
	// Note that we have to swap the X and Y coords, because noise generator uses [x + SizeX * y + SizeX * SizeY * z] ordering and we want "BlockY" to be "x":
	m_ChoiceNoise.Generate3D  (ChoiceNoise,   33, 5, 5, 0, 257 / m_ChoiceFrequencyY, BlockX / m_ChoiceFrequencyX, (BlockX + 17) / m_ChoiceFrequencyX, BlockZ / m_ChoiceFrequencyZ, (BlockZ + 17) / m_ChoiceFrequencyZ, Workspace);
	m_DensityNoiseA.Generate3D(DensityNoiseA, 33, 5, 5, 0, 257 / m_FrequencyY,       BlockX / m_FrequencyX,       (BlockX + 17) / m_FrequencyX,       BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ,       Workspace);
	m_DensityNoiseB.Generate3D(DensityNoiseB, 33, 5, 5, 0, 257 / m_FrequencyY,       BlockX / m_FrequencyX,       (BlockX + 17) / m_FrequencyX,       BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ,       Workspace);
	m_BaseNoise.Generate2D    (BaseNoise,     5, 5,     BlockX / m_BaseFrequencyX,   (BlockX + 17) / m_BaseFrequencyX,   BlockZ / m_FrequencyZ,       (BlockZ + 17) / m_FrequencyZ, Workspace);

	// Calculate the final noise based on the partial noises:
	for (int z = 0; z < 5; z++)
	{
		for (int x = 0; x < 5; x++)
		{
			NOISE_DATATYPE curMidPoint = MidPoint[x + 5 * z];
			NOISE_DATATYPE curHeightAmp = HeightAmp[x + 5 * z];
			NOISE_DATATYPE curBaseNoise = BaseNoise[x + 5 * z];
			for (int y = 0; y < 33; y++)
			{
				NOISE_DATATYPE AddHeight = (static_cast<NOISE_DATATYPE>(y * 8) - curMidPoint) * curHeightAmp;

				// If "underground", make the terrain smoother by forcing the vertical linear gradient into steeper slope:
				if (AddHeight < 0)
				{
					AddHeight *= 4;
				}
				// If too high, cut off any terrain:
				if (y > 28)
				{
					AddHeight = AddHeight + static_cast<NOISE_DATATYPE>(y - 28) / 4;
				}

				// Decide between the two density noises:
				int idx = 33 * x + y + 33 * 5 * z;
				Workspace[idx] = ClampedLerp(DensityNoiseA[idx], DensityNoiseB[idx], 8 * (ChoiceNoise[idx] + 0.5f)) + AddHeight + curBaseNoise;
			}
		}
	}
	LinearUpscale3DArray<NOISE_DATATYPE>(Workspace, 33, 5, 5, m_NoiseArray, 8, 4, 4);
}





void cBiomalNoise3DComposable::CalcBiomeParamArrays(cChunkCoords a_ChunkCoords, ChunkParam & a_HeightAmp, ChunkParam & a_MidPoint)
{
	// Generate the 3 * 3 chunks of biomes around this chunk:
	cChunkDef::BiomeMap neighborBiomes[3 * 3];
	for (int z = 0; z < 3; z++)
	{
		for (int x = 0; x < 3; x++)
		{
			m_BiomeGen->GenBiomes({a_ChunkCoords.m_ChunkX + x - 1, a_ChunkCoords.m_ChunkZ + z - 1}, neighborBiomes[x + 3 * z]);
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
		case biBeach:                a_HeightAmp = 0.2f;   a_MidPoint =  60; break;
		case biBirchForest:          a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biBirchForestHills:     a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biBirchForestHillsM:    a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biBirchForestM:         a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biColdBeach:            a_HeightAmp = 0.3f;   a_MidPoint =  62; break;
		case biColdTaiga:            a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biColdTaigaM:           a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biColdTaigaHills:       a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biDesertHills:          a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biDeepOcean:            a_HeightAmp = 0.17f;  a_MidPoint =  35; break;
		case biDesert:               a_HeightAmp = 0.29f;  a_MidPoint =  62; break;
		case biDesertM:              a_HeightAmp = 0.29f;  a_MidPoint =  62; break;
		case biEnd:                  a_HeightAmp = 0.15f;  a_MidPoint =  64; break;
		case biExtremeHills:         a_HeightAmp = 0.045f; a_MidPoint =  75; break;
		case biExtremeHillsEdge:     a_HeightAmp = 0.1f;   a_MidPoint =  70; break;
		case biExtremeHillsM:        a_HeightAmp = 0.045f; a_MidPoint =  75; break;
		case biExtremeHillsPlus:     a_HeightAmp = 0.04f;  a_MidPoint =  80; break;
		case biExtremeHillsPlusM:    a_HeightAmp = 0.04f;  a_MidPoint =  80; break;
		case biFlowerForest:         a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biForest:               a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biForestHills:          a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biFrozenRiver:          a_HeightAmp = 0.4f;   a_MidPoint =  54; break;
		case biFrozenOcean:          a_HeightAmp = 0.12f;  a_MidPoint =  45; break;
		case biIceMountains:         a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biIcePlains:            a_HeightAmp = 0.3f;   a_MidPoint =  62; break;
		case biIcePlainsSpikes:      a_HeightAmp = 0.3f;   a_MidPoint =  62; break;
		case biJungle:               a_HeightAmp = 0.1f;   a_MidPoint =  63; break;
		case biJungleEdge:           a_HeightAmp = 0.15f;  a_MidPoint =  62; break;
		case biJungleEdgeM:          a_HeightAmp = 0.15f;  a_MidPoint =  62; break;
		case biJungleHills:          a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biJungleM:              a_HeightAmp = 0.1f;   a_MidPoint =  63; break;
		case biMegaSpruceTaiga:      a_HeightAmp = 0.09f;  a_MidPoint =  64; break;
		case biMegaSpruceTaigaHills: a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biMegaTaiga:            a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biMegaTaigaHills:       a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biMesa:                 a_HeightAmp = 0.09f;  a_MidPoint =  61; break;
		case biMesaBryce:            a_HeightAmp = 0.15f;  a_MidPoint =  61; break;
		case biMesaPlateau:          a_HeightAmp = 0.25f;  a_MidPoint =  86; break;
		case biMesaPlateauF:         a_HeightAmp = 0.25f;  a_MidPoint =  96; break;
		case biMesaPlateauFM:        a_HeightAmp = 0.25f;  a_MidPoint =  96; break;
		case biMesaPlateauM:         a_HeightAmp = 0.25f;  a_MidPoint =  86; break;
		case biMushroomShore:        a_HeightAmp = 0.075f; a_MidPoint =  60; break;
		case biMushroomIsland:       a_HeightAmp = 0.06f;  a_MidPoint =  80; break;
		case biNether:               a_HeightAmp = 0.01f;  a_MidPoint =  64; break;
		case biOcean:                a_HeightAmp = 0.12f;  a_MidPoint =  45; break;
		case biPlains:               a_HeightAmp = 0.3f;   a_MidPoint =  62; break;
		case biRiver:                a_HeightAmp = 0.4f;   a_MidPoint =  54; break;
		case biRoofedForest:         a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biRoofedForestM:        a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biSavanna:              a_HeightAmp = 0.3f;   a_MidPoint =  62; break;
		case biSavannaM:             a_HeightAmp = 0.3f;   a_MidPoint =  62; break;
		case biSavannaPlateau:       a_HeightAmp = 0.3f;   a_MidPoint =  85; break;
		case biSavannaPlateauM:      a_HeightAmp = 0.012f; a_MidPoint = 105; break;
		case biStoneBeach:           a_HeightAmp = 0.075f; a_MidPoint =  60; break;
		case biSunflowerPlains:      a_HeightAmp = 0.3f;   a_MidPoint =  62; break;
		case biSwampland:            a_HeightAmp = 0.25f;  a_MidPoint =  59; break;
		case biSwamplandM:           a_HeightAmp = 0.11f;  a_MidPoint =  59; break;
		case biTaiga:                a_HeightAmp = 0.1f;   a_MidPoint =  64; break;
		case biTaigaM:               a_HeightAmp = 0.1f;   a_MidPoint =  70; break;
		case biTaigaHills:           a_HeightAmp = 0.075f; a_MidPoint =  68; break;
		case biInvalidBiome:
		case biNumBiomes:
		case biVariant:
		case biNumVariantBiomes:
		{
			// Make a crazy terrain so that it stands out
			a_HeightAmp = 0.001f;
			a_MidPoint = 90;
			break;
		}
	}
}





void cBiomalNoise3DComposable::GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape)
{
	GenerateNoiseArrayIfNeeded(a_ChunkCoords);

	// Translate the noise array into Shape:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				a_Shape[y + x * 256 + z * 256 * 16] = (m_NoiseArray[y + 257 * x + 257 * 17 * z] > m_AirThreshold) ? 0 : 1;
			}
		}  // for x
	}  // for z
}





