
// Nosie3DGenerator.cpp

// Generates terrain using 3D noise, rather than composing. Is a test.

#include "Globals.h"
#include "Noise3DGenerator.h"
#include "../OSSupport/File.h"
#include "inifile/iniFile.h"
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		Height[i] = abs(Height[i]) * m_HeightAmplification + 1;
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





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNoise3DComposable:

cNoise3DComposable::cNoise3DComposable(int a_Seed) :
	m_Noise1(a_Seed + 1000),
	m_Noise2(a_Seed + 2000),
	m_Noise3(a_Seed + 3000)
{
}





void cNoise3DComposable::Initialize(cIniFile & a_IniFile)
{
	// Params:
	m_SeaLevel            =                 a_IniFile.GetValueSetI("Generator", "Noise3DSeaLevel", 62);
	m_HeightAmplification = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DHeightAmplification", 0);
	m_MidPoint            = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DMidPoint", 75);
	m_FrequencyX          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyX", 10);
	m_FrequencyY          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyY", 10);
	m_FrequencyZ          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyZ", 10);
	m_AirThreshold        = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DAirThreshold", 0.5);
}





void cNoise3DComposable::GenerateNoiseArrayIfNeeded(int a_ChunkX, int a_ChunkZ)
{
	if ((a_ChunkX == m_LastChunkX) && (a_ChunkZ == m_LastChunkZ))
	{
		// The noise for this chunk is already generated in m_Noise
		return;
	}
	m_LastChunkX = a_ChunkX;
	m_LastChunkZ = a_ChunkZ;

	// Upscaling parameters:
	const int UPSCALE_X = 8;
	const int UPSCALE_Y = 4;
	const int UPSCALE_Z = 8;

	// Precalculate a "height" array:
	NOISE_DATATYPE Height[17 * 17];  // x + 17 * z
	for (int z = 0; z < 17; z += UPSCALE_Z)
	{
		NOISE_DATATYPE NoiseZ = ((NOISE_DATATYPE)(a_ChunkZ * cChunkDef::Width + z)) / m_FrequencyZ;
		for (int x = 0; x < 17; x += UPSCALE_X)
		{
			NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(a_ChunkX * cChunkDef::Width + x)) / m_FrequencyX;
			NOISE_DATATYPE val = abs(m_Noise1.CubicNoise2D(NoiseX / 5, NoiseZ / 5)) * m_HeightAmplification + 1;
			Height[x + 17 * z] = val * val * val;
		}
	}

	for (int y = 0; y < 257; y += UPSCALE_Y)
	{
		NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)y) / m_FrequencyY;
		NOISE_DATATYPE AddHeight = (y - m_MidPoint) / 20;
		AddHeight *= AddHeight * AddHeight;
		NOISE_DATATYPE * CurFloor = &(m_NoiseArray[y * 17 * 17]);
		for (int z = 0; z < 17; z += UPSCALE_Z)
		{
			NOISE_DATATYPE NoiseZ = ((NOISE_DATATYPE)(a_ChunkZ * cChunkDef::Width + z)) / m_FrequencyZ;
			for (int x = 0; x < 17; x += UPSCALE_X)
			{
				NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(a_ChunkX * cChunkDef::Width + x)) / m_FrequencyX;
				CurFloor[x + 17 * z] =
					m_Noise1.CubicNoise3D(NoiseX, NoiseY, NoiseZ) * (NOISE_DATATYPE)0.5 +
					m_Noise2.CubicNoise3D(NoiseX / 2, NoiseY / 2, NoiseZ / 2) +
					m_Noise3.CubicNoise3D(NoiseX / 4, NoiseY / 4, NoiseZ / 4) * 2 +
					AddHeight / Height[x + 17 * z];
			}
		}
		// Linear-interpolate this XZ floor:
		LinearUpscale2DArrayInPlace(CurFloor, 17, 17, UPSCALE_X, UPSCALE_Z);
	}

	// Finish the 3D linear interpolation by interpolating between each XZ-floors on the Y axis
	for (int y = 1; y < cChunkDef::Height; y++)
	{
		if ((y % UPSCALE_Y) == 0)
		{
			// This is the interpolation source floor, already calculated
			continue;
		}
		int LoFloorY = (y / UPSCALE_Y) * UPSCALE_Y;
		int HiFloorY = LoFloorY + UPSCALE_Y;
		NOISE_DATATYPE * LoFloor  = &(m_NoiseArray[LoFloorY * 17 * 17]);
		NOISE_DATATYPE * HiFloor  = &(m_NoiseArray[HiFloorY * 17 * 17]);
		NOISE_DATATYPE * CurFloor = &(m_NoiseArray[y * 17 * 17]);
		NOISE_DATATYPE Ratio = ((NOISE_DATATYPE)(y % UPSCALE_Y)) / UPSCALE_Y;
		int idx = 0;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				CurFloor[idx] = LoFloor[idx] + (HiFloor[idx] - LoFloor[idx]) * Ratio;
				idx += 1;
			}
			idx += 1;  // Skipping one X column
		}
	}

	// The noise array is now fully interpolated
	/*
	// DEBUG: Output two images of the array, sliced by XY and XZ:
	cFile f1;
	if (f1.Open(Printf("Chunk_%d_%d_XY.raw", a_ChunkX, a_ChunkZ), cFile::fmWrite))
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				int idx = y * 17 * 17 + z * 17;
				unsigned char buf[16];
				for (int x = 0; x < cChunkDef::Width; x++)
				{
					buf[x] = (unsigned char)(std::min(256, std::max(0, (int)(128 + 128 * m_Noise[idx++]))));
				}
				f1.Write(buf, 16);
			}  // for y
		}  // for z
	}  // if (XY file open)

	cFile f2;
	if (f2.Open(Printf("Chunk_%d_%d_XZ.raw", a_ChunkX, a_ChunkZ), cFile::fmWrite))
	{
		for (int y = 0; y < cChunkDef::Height; y++)
		{
			for (int z = 0; z < cChunkDef::Width; z++)
			{
				int idx = y * 17 * 17 + z * 17;
				unsigned char buf[16];
				for (int x = 0; x < cChunkDef::Width; x++)
				{
					buf[x] = (unsigned char)(std::min(256, std::max(0, (int)(128 + 128 * m_Noise[idx++]))));
				}
				f2.Write(buf, 16);
			}  // for z
		}  // for y
	}  // if (XZ file open)
	*/
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




