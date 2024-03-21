
// EndGen.cpp

// Implements the cEndGen class representing the generator for the End, both as a HeightGen and CompositionGen

#include "Globals.h"
#include "EndGen.h"
#include "../IniFile.h"
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
};





////////////////////////////////////////////////////////////////////////////////
// cEndGen:

cEndGen::cEndGen(int a_Seed) :
	m_Seed(a_Seed),
	m_Perlin(m_Seed),
	m_MainIslandSize(200),
	m_IslandThickness(32),
	m_IslandYOffset(30),
	m_MainIslandFrequencyX(100),
	m_MainIslandFrequencyY(80),
	m_MainIslandFrequencyZ(100),
	m_MainIslandMinThreshold(0.2),
	m_SmallIslandFrequencyX(50),
	m_SmallIslandFrequencyY(80),
	m_SmallIslandFrequencyZ(50),
	m_SmallIslandMinThreshold(-0.5),
	m_LastChunkCoords(0x7fffffff, 0x7fffffff)  // Use dummy coords that won't ever be used by real chunks
{
	m_Perlin.AddOctave(1, 1);
	m_Perlin.AddOctave(2, 0.5);
	m_Perlin.AddOctave(4, 0.25);
}





void cEndGen::InitializeShapeGen(cIniFile & a_IniFile)
{
	m_MainIslandSize = a_IniFile.GetValueSetI("Generator", "EndGenMainIslandSize", m_MainIslandSize);
	m_IslandThickness = a_IniFile.GetValueSetI("Generator", "EndGenIslandFlatness", m_IslandThickness);
	m_IslandYOffset = a_IniFile.GetValueSetI("Generator", "EndGenIslandYOffset", m_IslandYOffset);

	m_MainIslandFrequencyX   = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenMainFrequencyX", m_MainIslandFrequencyX));
	m_MainIslandFrequencyY   = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenMainFrequencyY", m_MainIslandFrequencyY));
	m_MainIslandFrequencyZ   = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenMainFrequencyZ", m_MainIslandFrequencyZ));
	m_MainIslandMinThreshold = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenMainMinThreshold", m_MainIslandMinThreshold));

	m_SmallIslandFrequencyX   = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenSmallFrequencyX", m_SmallIslandFrequencyX));
	m_SmallIslandFrequencyY   = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenSmallFrequencyY", m_SmallIslandFrequencyY));
	m_SmallIslandFrequencyZ   = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenSmallFrequencyZ", m_SmallIslandFrequencyZ));
	m_SmallIslandMinThreshold = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenSmallMinThreshold", m_SmallIslandMinThreshold));

}





void cEndGen::PrepareState(cChunkCoords a_ChunkCoords)
{
	if (m_LastChunkCoords == a_ChunkCoords)
	{
		return;
	}

	m_LastChunkCoords = a_ChunkCoords;

	GenerateNoiseArray();
}





void cEndGen::GenerateNoiseArray(void)
{
	NOISE_DATATYPE NoiseData[DIM_X * DIM_Y * DIM_Z];  // [x + DIM_X * z + DIM_X * DIM_Z * y]
	NOISE_DATATYPE Workspace[DIM_X * DIM_Y * DIM_Z];  // [x + DIM_X * z + DIM_X * DIM_Z * y]

	// Choose the frequency to use depending on the distance from spawn.
	double distanceFromSpawn = cChunkDef::RelativeToAbsolute({ cChunkDef::Width / 2, 0, cChunkDef::Width / 2 }, m_LastChunkCoords).Length();
	NOISE_DATATYPE frequencyX = distanceFromSpawn > m_MainIslandSize * 2 ? m_SmallIslandFrequencyX : m_MainIslandFrequencyX;
	NOISE_DATATYPE frequencyY = distanceFromSpawn > m_MainIslandSize * 2 ? m_SmallIslandFrequencyY : m_MainIslandFrequencyY;
	NOISE_DATATYPE frequencyZ = distanceFromSpawn > m_MainIslandSize * 2 ? m_SmallIslandFrequencyZ : m_MainIslandFrequencyZ;

	// Generate the downscaled noise:
	NOISE_DATATYPE StartX = static_cast<NOISE_DATATYPE>(m_LastChunkCoords.m_ChunkX       * cChunkDef::Width) / frequencyX;
	NOISE_DATATYPE EndX   = static_cast<NOISE_DATATYPE>((m_LastChunkCoords.m_ChunkX + 1) * cChunkDef::Width) / frequencyX;
	NOISE_DATATYPE StartZ = static_cast<NOISE_DATATYPE>(m_LastChunkCoords.m_ChunkZ       * cChunkDef::Width) / frequencyZ;
	NOISE_DATATYPE EndZ   = static_cast<NOISE_DATATYPE>((m_LastChunkCoords.m_ChunkZ + 1) * cChunkDef::Width) / frequencyZ;
	NOISE_DATATYPE StartY = 0;
	NOISE_DATATYPE EndY   = static_cast<NOISE_DATATYPE>(257) / frequencyY;
	m_Perlin.Generate3D(NoiseData, DIM_X, DIM_Z, DIM_Y, StartX, EndX, StartZ, EndZ, StartY, EndY, Workspace);

	// Add distance:
	for (int y = 0; y < DIM_Y; y++)
	{
		NOISE_DATATYPE ValY = static_cast<NOISE_DATATYPE>(2 * INTERPOL_Y * y - m_IslandThickness) / m_IslandThickness;
		ValY = std::pow(ValY, 6);
		for (int z = 0; z < DIM_Z; z++)
		{
			for (int x = 0; x < DIM_X; x++)
			{
				NoiseData[x + DIM_X * z + DIM_X * DIM_Z * y] += ValY;
			}  // for x
		}  // for z
	}  // for y

	// Upscale into real chunk size:
	LinearUpscale3DArray(NoiseData, DIM_X, DIM_Z, DIM_Y, m_NoiseArray, INTERPOL_X, INTERPOL_Z, INTERPOL_Y);
}





void cEndGen::GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape)
{
	PrepareState(a_ChunkCoords);

	int MaxY = std::min(static_cast<int>(1.75 * m_IslandThickness + m_IslandYOffset), cChunkDef::Height - 1);

	// Choose which threshold to use depending on the distance from spawn.
	double distanceFromSpawn = cChunkDef::RelativeToAbsolute({ cChunkDef::Width / 2, 0, cChunkDef::Width / 2 }, a_ChunkCoords).Length();
	double minThreshold = distanceFromSpawn > m_MainIslandSize * 2 ? m_SmallIslandMinThreshold : m_MainIslandMinThreshold;
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			// Calculate the required treshold based on the distance from spawn.
			// This way a void can be generated between the main island and the other islands.
			double distanceFromSpawn = cChunkDef::RelativeToAbsolute({ x, 0, z }, a_ChunkCoords).Length();
			double pow = std::pow((distanceFromSpawn - m_MainIslandSize) / m_MainIslandSize / 2, 3);
			double mult = 3 * ((distanceFromSpawn - m_MainIslandSize) / m_MainIslandSize);
			double threshold = std::min(pow - mult, minThreshold);

			for (int y = 0; y < m_IslandYOffset; y++)
			{
				a_Shape[y + x * 256 + z * 256 * 16] = 0;
			}
			for (int y = m_IslandYOffset; y < MaxY; y++)
			{
				a_Shape[y + x * 256 + z * 256 * 16] = (m_NoiseArray[(y - m_IslandYOffset) * 17 * 17 + z * 17 + x] <= threshold) ? 1 : 0;
			}
			for (int y = MaxY; y < cChunkDef::Height; y++)
			{
				a_Shape[y + x * 256 + z * 256 * 16] = 0;
			}
		}  // for x
	}  // for z
}





void cEndGen::ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape)
{
	a_ChunkDesc.FillBlocks(E_BLOCK_AIR, 0);
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				if (a_Shape[(x + 16 * z) * 256 + y] != 0)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_END_STONE);
				}
			}  // for y
		}  // for x
	}  // for z
}




