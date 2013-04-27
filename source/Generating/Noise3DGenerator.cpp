
// Nosie3DGenerator.cpp

// Generates terrain using 3D noise, rather than composing. Is a test.

#include "Globals.h"
#include "Noise3DGenerator.h"
#include "../OSSupport/File.h"
#include "../../iniFile/iniFile.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNoise3DGenerator:

cNoise3DGenerator::cNoise3DGenerator(cChunkGenerator & a_ChunkGenerator) :
	super(a_ChunkGenerator),
	m_Noise1(1000),
	m_Noise2(2000),
	m_Noise3(3000)
{
}





cNoise3DGenerator::~cNoise3DGenerator()
{
	// Nothing needed yet
}





void cNoise3DGenerator::Initialize(cWorld * a_World, cIniFile & a_IniFile)
{
	m_World = a_World;
	
	// Params:
	m_SeaLevel            =                 a_IniFile.GetValueSetI("Generator", "Noise3DSeaLevel", 62);
	m_HeightAmplification = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DHeightAmplification", 0);
	m_MidPoint            = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DMidPoint", 75);
	m_FrequencyX          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyX", 10);
	m_FrequencyY          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyY", 10);
	m_FrequencyZ          = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DFrequencyZ", 10);
	m_AirThreshold        = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "Noise3DAirThreshold", 0.5);
}





void cNoise3DGenerator::GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap)
{
	for (int i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		a_BiomeMap[i] = biExtremeHills;
	}
}





void cNoise3DGenerator::DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc)
{
	NOISE_DATATYPE Noise[257 * 17 * 17];  // x + 17 * z + 17 * 17 * y
	GenerateNoiseArray(a_ChunkX, a_ChunkZ, Noise);
	
	// Output noise into chunk:
	for (int y = 0; y < cChunkDef::Height; y++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			int idx = y * 17 * 17 + z * 17;
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





void cNoise3DGenerator::GenerateNoiseArray(int a_ChunkX, int a_ChunkZ, NOISE_DATATYPE * a_Noise)
{
	// Parameters:
	const int INTERPOL_X = 8;
	const int INTERPOL_Y = 4;
	const int INTERPOL_Z = 8;

	// Precalculate a "height" array:
	NOISE_DATATYPE Height[17 * 17];  // x + 17 * z
	for (int z = 0; z < 17; z += INTERPOL_Z)
	{
		NOISE_DATATYPE NoiseZ = ((NOISE_DATATYPE)(a_ChunkZ * cChunkDef::Width + z)) / m_FrequencyZ;
		for (int x = 0; x < 17; x += INTERPOL_X)
		{
			NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(a_ChunkX * cChunkDef::Width + x)) / m_FrequencyX;
			NOISE_DATATYPE val = abs(m_Noise1.CubicNoise2D(NoiseX / 5, NoiseZ / 5)) * m_HeightAmplification + 1;
			Height[x + 17 * z] = val * val * val;
		}
	}
		
	int idx = 0;
	for (int y = 0; y < 257; y += INTERPOL_Y)
	{
		NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)y) / m_FrequencyY;
		NOISE_DATATYPE AddHeight = (y - m_MidPoint) / 20;
		AddHeight *= AddHeight * AddHeight;  // * AddHeight * AddHeight;
		NOISE_DATATYPE * CurFloor = &(a_Noise[y * 17 * 17]);
		for (int z = 0; z < 17; z += INTERPOL_Z)
		{
			NOISE_DATATYPE NoiseZ = ((NOISE_DATATYPE)(a_ChunkZ * cChunkDef::Width + z)) / m_FrequencyZ;
			for (int x = 0; x < 17; x += INTERPOL_X)
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
		ArrayLinearInterpolate2D(CurFloor, 17, 17, INTERPOL_X, INTERPOL_Z);
	}
	
	// Finish the 3D linear interpolation by interpolating between each XZ-floors on the Y axis
	for (int y = 1; y < cChunkDef::Height; y++)
	{
		if ((y % INTERPOL_Y) == 0)
		{
			// This is the interpolation source floor, already calculated
			continue;
		}
		int LoFloorY = (y / INTERPOL_Y) * INTERPOL_Y;
		int HiFloorY = LoFloorY + INTERPOL_Y;
		NOISE_DATATYPE * LoFloor  = &(a_Noise[LoFloorY * 17 * 17]);
		NOISE_DATATYPE * HiFloor  = &(a_Noise[HiFloorY * 17 * 17]);
		NOISE_DATATYPE * CurFloor = &(a_Noise[y * 17 * 17]);
		NOISE_DATATYPE Ratio = ((NOISE_DATATYPE)(y % INTERPOL_Y)) / INTERPOL_Y;
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
					buf[x] = (unsigned char)(std::min(256, std::max(0, (int)(128 + 128 * a_Noise[idx++]))));
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
					buf[x] = (unsigned char)(std::min(256, std::max(0, (int)(128 + 128 * a_Noise[idx++]))));
				}
				f2.Write(buf, 16);
			}  // for z
		}  // for y
	}  // if (XZ file open)
	*/
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
	
	// Parameters:
	const int INTERPOL_X = 8;
	const int INTERPOL_Y = 4;
	const int INTERPOL_Z = 8;

	// Precalculate a "height" array:
	NOISE_DATATYPE Height[17 * 17];  // x + 17 * z
	for (int z = 0; z < 17; z += INTERPOL_Z)
	{
		NOISE_DATATYPE NoiseZ = ((NOISE_DATATYPE)(a_ChunkZ * cChunkDef::Width + z)) / m_FrequencyZ;
		for (int x = 0; x < 17; x += INTERPOL_X)
		{
			NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(a_ChunkX * cChunkDef::Width + x)) / m_FrequencyX;
			NOISE_DATATYPE val = abs(m_Noise1.CubicNoise2D(NoiseX / 5, NoiseZ / 5)) * m_HeightAmplification + 1;
			Height[x + 17 * z] = val * val * val;
		}
	}
		
	int idx = 0;
	for (int y = 0; y < 257; y += INTERPOL_Y)
	{
		NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)y) / m_FrequencyY;
		NOISE_DATATYPE AddHeight = (y - m_MidPoint) / 20;
		AddHeight *= AddHeight * AddHeight;
		NOISE_DATATYPE * CurFloor = &(m_NoiseArray[y * 17 * 17]);
		for (int z = 0; z < 17; z += INTERPOL_Z)
		{
			NOISE_DATATYPE NoiseZ = ((NOISE_DATATYPE)(a_ChunkZ * cChunkDef::Width + z)) / m_FrequencyZ;
			for (int x = 0; x < 17; x += INTERPOL_X)
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
		ArrayLinearInterpolate2D(CurFloor, 17, 17, INTERPOL_X, INTERPOL_Z);
	}
	
	// Finish the 3D linear interpolation by interpolating between each XZ-floors on the Y axis
	for (int y = 1; y < cChunkDef::Height; y++)
	{
		if ((y % INTERPOL_Y) == 0)
		{
			// This is the interpolation source floor, already calculated
			continue;
		}
		int LoFloorY = (y / INTERPOL_Y) * INTERPOL_Y;
		int HiFloorY = LoFloorY + INTERPOL_Y;
		NOISE_DATATYPE * LoFloor  = &(m_NoiseArray[LoFloorY * 17 * 17]);
		NOISE_DATATYPE * HiFloor  = &(m_NoiseArray[HiFloorY * 17 * 17]);
		NOISE_DATATYPE * CurFloor = &(m_NoiseArray[y * 17 * 17]);
		NOISE_DATATYPE Ratio = ((NOISE_DATATYPE)(y % INTERPOL_Y)) / INTERPOL_Y;
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
	
	// Make basic terrain composition:
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int LastAir = a_ChunkDesc.GetHeight(x, z) + 1;
			bool HasHadWater = false;
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




