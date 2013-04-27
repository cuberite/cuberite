
// Nosie3DGenerator.cpp

// Generates terrain using 3D noise, rather than composing. Is a test.

#include "Globals.h"
#include "Noise3DGenerator.h"
#include "../OSSupport/File.h"





cNoise3DGenerator::cNoise3DGenerator(cChunkGenerator & a_ChunkGenerator) :
	super(a_ChunkGenerator),
	m_Noise1(0),
	m_Noise2(0)
{
}





cNoise3DGenerator::~cNoise3DGenerator()
{
	// Nothing needed yet
}





void cNoise3DGenerator::Initialize(cWorld * a_World, cIniFile & a_IniFile)
{
	m_World = a_World;
	// TODO: params
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
	const int INTERPOL_X = 8;
	const int INTERPOL_Y = 4;
	const int INTERPOL_Z = 8;
	const NOISE_DATATYPE FrequencyX = 20;
	const NOISE_DATATYPE FrequencyY = 20;
	const NOISE_DATATYPE FrequencyZ = 20;
	NOISE_DATATYPE Noise[257 * 17 * 17];  // x + 17 * z + 17 * 17 * y
	int idx = 0;
	for (int y = 0; y < 257; y += INTERPOL_Y)
	{
		NOISE_DATATYPE NoiseY = ((NOISE_DATATYPE)y) / FrequencyY;
		NOISE_DATATYPE AddHeight = NoiseY - ((NOISE_DATATYPE)128 / FrequencyY);
		AddHeight *= AddHeight * AddHeight * AddHeight * AddHeight;
		NOISE_DATATYPE * CurFloor = &(Noise[y * 17 * 17]);
		for (int z = 0; z < 17; z += INTERPOL_Z)
		{
			NOISE_DATATYPE NoiseZ = ((NOISE_DATATYPE)(a_ChunkZ * cChunkDef::Width + z)) / FrequencyZ;
			for (int x = 0; x < 17; x += INTERPOL_X)
			{
				NOISE_DATATYPE NoiseX = ((NOISE_DATATYPE)(a_ChunkX * cChunkDef::Width + x)) / FrequencyX;
				CurFloor[x + 17 * z] = 
					m_Noise1.CubicNoise3D(NoiseX, NoiseY, NoiseZ) +
					m_Noise2.CubicNoise3D(NoiseX / 2, NoiseY / 2, NoiseZ / 2) +
					AddHeight;
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
		NOISE_DATATYPE * LoFloor  = &(Noise[LoFloorY * 17 * 17]);
		NOISE_DATATYPE * HiFloor  = &(Noise[HiFloorY * 17 * 17]);
		NOISE_DATATYPE * CurFloor = &(Noise[y * 17 * 17]);
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
					buf[x] = (unsigned char)(std::min(256, std::max(0, (int)(128 + 128 * Noise[idx++]))));
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
					buf[x] = (unsigned char)(std::min(256, std::max(0, (int)(128 + 128 * Noise[idx++]))));
				}
				f2.Write(buf, 16);
			}  // for z
		}  // for y
	}  // if (XZ file open)
	*/
	
	// Output into chunk:
	for (int y = 0; y < cChunkDef::Height; y++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			int idx = y * 17 * 17 + z * 17;
			for (int x = 0; x < cChunkDef::Width; x++)
			{
				a_ChunkDesc.SetBlockType(x, y, z, (Noise[idx++] > 0.5) ? E_BLOCK_AIR : E_BLOCK_STONE);
			}
		}
	}
	
	// Update the heightmap:
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




