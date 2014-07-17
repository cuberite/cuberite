
// EndGen.cpp

// Implements the cEndGen class representing the generator for the End, both as a HeightGen and CompositionGen

#include "Globals.h"
#include "EndGen.h"
#include "inifile/iniFile.h"
#include "../LinearUpscale.h"





enum
{
	// Interpolation cell size:
	INTERPOL_X = 4,
	INTERPOL_Y = 4,
	INTERPOL_Z = 4,

	// Size of chunk data, downscaled before interpolation:
	DIM_X = 16  / INTERPOL_X + 1,
	DIM_Y = 256 / INTERPOL_Y + 1,
	DIM_Z = 16  / INTERPOL_Z + 1,
} ;





////////////////////////////////////////////////////////////////////////////////
// cEndGen:

cEndGen::cEndGen(int a_Seed) :
	m_Seed(a_Seed),
	m_IslandSizeX(256),
	m_IslandSizeY(96),
	m_IslandSizeZ(256),
	m_FrequencyX(80),
	m_FrequencyY(80),
	m_FrequencyZ(80)
{
	m_Perlin.AddOctave(1, 1);
	m_Perlin.AddOctave(2, 0.5);
	m_Perlin.AddOctave(4, 0.25);
}





void cEndGen::Initialize(cIniFile & a_IniFile)
{
	m_IslandSizeX = a_IniFile.GetValueSetI("Generator", "EndGenIslandSizeX", m_IslandSizeX);
	m_IslandSizeY = a_IniFile.GetValueSetI("Generator", "EndGenIslandSizeY", m_IslandSizeY);
	m_IslandSizeZ = a_IniFile.GetValueSetI("Generator", "EndGenIslandSizeZ", m_IslandSizeZ);

	m_FrequencyX = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "EndGenFrequencyX", m_FrequencyX);
	m_FrequencyY = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "EndGenFrequencyY", m_FrequencyY);
	m_FrequencyZ = (NOISE_DATATYPE)a_IniFile.GetValueSetF("Generator", "EndGenFrequencyZ", m_FrequencyZ);

	// Recalculate the min and max chunk coords of the island
	m_MaxChunkX = (m_IslandSizeX + cChunkDef::Width - 1) / cChunkDef::Width;
	m_MinChunkX = -m_MaxChunkX;
	m_MaxChunkZ = (m_IslandSizeZ + cChunkDef::Width - 1) / cChunkDef::Width;
	m_MinChunkZ = -m_MaxChunkZ;
}





/// Unless the LastChunk coords are equal to coords given, prepares the internal state (noise array)
void cEndGen::PrepareState(int a_ChunkX, int a_ChunkZ)
{
	ASSERT(!IsChunkOutsideRange(a_ChunkX, a_ChunkZ));  // Should be filtered before calling this function

	if ((m_LastChunkX == a_ChunkX) && (m_LastChunkZ == a_ChunkZ))
	{
		return;
	}

	m_LastChunkX = a_ChunkX;
	m_LastChunkZ = a_ChunkZ;

	GenerateNoiseArray();
}





/// Generates the m_NoiseArray array for the current chunk
void cEndGen::GenerateNoiseArray(void)
{
	NOISE_DATATYPE NoiseData[DIM_X * DIM_Y * DIM_Z];  // [x + DIM_X * z + DIM_X * DIM_Z * y]
	NOISE_DATATYPE Workspace[DIM_X * DIM_Y * DIM_Z];  // [x + DIM_X * z + DIM_X * DIM_Z * y]

	// Generate the downscaled noise:
	NOISE_DATATYPE StartX = ((NOISE_DATATYPE)(m_LastChunkX       * cChunkDef::Width)) / m_FrequencyX;
	NOISE_DATATYPE EndX   = ((NOISE_DATATYPE)((m_LastChunkX + 1) * cChunkDef::Width)) / m_FrequencyX;
	NOISE_DATATYPE StartZ = ((NOISE_DATATYPE)(m_LastChunkZ       * cChunkDef::Width)) / m_FrequencyZ;
	NOISE_DATATYPE EndZ   = ((NOISE_DATATYPE)((m_LastChunkZ + 1) * cChunkDef::Width)) / m_FrequencyZ;
	NOISE_DATATYPE StartY = 0;
	NOISE_DATATYPE EndY   = ((NOISE_DATATYPE)257) / m_FrequencyY;
	m_Perlin.Generate3D(NoiseData, DIM_X, DIM_Z, DIM_Y, StartX, EndX, StartZ, EndZ, StartY, EndY, Workspace);

	// Add distance:
	int idx = 0;
	for (int y = 0; y < DIM_Y; y++)
	{
		NOISE_DATATYPE ValY = (NOISE_DATATYPE)(2 * INTERPOL_Y * y - m_IslandSizeY) / m_IslandSizeY;
		ValY = ValY * ValY;
		for (int z = 0; z < DIM_Z; z++)
		{
			NOISE_DATATYPE ValZ = (NOISE_DATATYPE)(m_LastChunkZ * cChunkDef::Width + (z * cChunkDef::Width / (DIM_Z - 1))) / m_IslandSizeZ;
			ValZ = ValZ * ValZ;
			for (int x = 0; x < DIM_X; x++)
			{
				// NOISE_DATATYPE ValX = StartX + (EndX - StartX) * x / (DIM_X - 1);
				NOISE_DATATYPE ValX = (NOISE_DATATYPE)(m_LastChunkX * cChunkDef::Width + (x * cChunkDef::Width / (DIM_X - 1))) / m_IslandSizeX;
				ValX = ValX * ValX;
				NoiseData[idx++] += ValX + ValZ + ValY;
			}  // for x
		}  // for z
	}  // for y

	// Upscale into real chunk size:
	LinearUpscale3DArray(NoiseData, DIM_X, DIM_Z, DIM_Y, m_NoiseArray, INTERPOL_X, INTERPOL_Z, INTERPOL_Y);
}





/// Returns true if the chunk is outside of the island's dimensions
bool cEndGen::IsChunkOutsideRange(int a_ChunkX, int a_ChunkZ)
{
	return (
		(a_ChunkX < m_MinChunkX) || (a_ChunkX > m_MaxChunkX) ||
		(a_ChunkZ < m_MinChunkZ) || (a_ChunkZ > m_MaxChunkZ)
	);
}





void cEndGen::GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap)
{
	if (IsChunkOutsideRange(a_ChunkX, a_ChunkZ))
	{
		for (size_t i = 0; i < ARRAYCOUNT(a_HeightMap); i++)
		{
			a_HeightMap[i] = 0;
		}
		return;
	}

	PrepareState(a_ChunkX, a_ChunkZ);

	int MaxY = std::min((int)(1.75 * m_IslandSizeY + 1), cChunkDef::Height - 1);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			cChunkDef::SetHeight(a_HeightMap, x, z, MaxY);
			for (int y = MaxY; y > 0; y--)
			{
				if (m_NoiseArray[y * 17 * 17 + z * 17 + x] <= 0)
				{
					cChunkDef::SetHeight(a_HeightMap, x, z, y);
					break;
				}
			}  // for y
		}  // for x
	}  // for z
}





void cEndGen::ComposeTerrain(cChunkDesc & a_ChunkDesc)
{
	if (IsChunkOutsideRange(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ()))
	{
		a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);
		return;
	}

	PrepareState(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ());

	int MaxY = std::min((int)(1.75 * m_IslandSizeY + 1), cChunkDef::Height - 1);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			for (int y = MaxY; y > 0; y--)
			{
				if (m_NoiseArray[y * 17 * 17 + z * 17 + x] <= 0)
				{
					a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_END_STONE, 0);
				}
				else
				{
					a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_AIR, 0);
				}
			}  // for y
		}  // for x
	}  // for z
}




