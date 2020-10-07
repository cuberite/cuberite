
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
} ;





////////////////////////////////////////////////////////////////////////////////
// cEndGen:

cEndGen::cEndGen(int a_Seed) :
	m_Seed(a_Seed),
	m_Perlin(a_Seed),
	m_VoidOffsetNoise(a_Seed + 1000),
	m_AirThresholdMainIsland(0.0f),
	m_AirThresholdOtherIslands(0.5f),
	m_MainIslandSize(450),
	m_BaseHeight(64),
	m_TerrainTopMultiplier(10),
	m_TerrainBottomMultiplier(40),
	m_VoidOffsetNoiseMultiplier(50),
	m_FrequencyX(80),
	m_FrequencyY(80),
	m_FrequencyZ(80)
{
	m_Perlin.AddOctave(1, 1);
	m_Perlin.AddOctave(2, 0.5);
	m_Perlin.AddOctave(4, 0.25);

	m_VoidOffsetNoise.AddOctave(1, 1);
	m_VoidOffsetNoise.AddOctave(2, 0.5);
	m_VoidOffsetNoise.AddOctave(4, 0.25);
}





void cEndGen::InitializeShapeGen(cIniFile & a_IniFile)
{
	m_AirThresholdMainIsland = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenAirThresholdMainIsland", m_AirThresholdMainIsland));
	m_AirThresholdOtherIslands = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenAirThresholdOtherIslands", m_AirThresholdOtherIslands));
	m_MainIslandSize = a_IniFile.GetValueSetI("Generator", "EndGenMainIslandSize", m_MainIslandSize);
	m_BaseHeight = a_IniFile.GetValueSetI("Generator", "EndGenBaseHeight", m_BaseHeight);
	m_TerrainTopMultiplier = a_IniFile.GetValueSetI("Generator", "EndGenTerrainTopMultipler", m_TerrainTopMultiplier);
	m_TerrainBottomMultiplier = a_IniFile.GetValueSetI("Generator", "EndGenTerrainBottomMultiplier", m_TerrainBottomMultiplier);
	m_VoidOffsetNoiseMultiplier = a_IniFile.GetValueSetI("Generator", "EndGenVoidOffsetNoiseMultiplier", m_VoidOffsetNoiseMultiplier);

	m_FrequencyX = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenFrequencyX", m_FrequencyX));
	m_FrequencyY = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenFrequencyY", m_FrequencyY));
	m_FrequencyZ = static_cast<NOISE_DATATYPE>(a_IniFile.GetValueSetF("Generator", "EndGenFrequencyZ", m_FrequencyZ));
}





void cEndGen::GenShape(cChunkCoords a_ChunkCoords, cChunkDesc::Shape & a_Shape)
{
	for (size_t i = 0; i < ARRAYCOUNT(a_Shape); i++)
	{
		a_Shape[i] = 0;
	}

	NOISE_DATATYPE NoiseData[cChunkDef::Width * cChunkDef::Width];
	NOISE_DATATYPE VoidOffsetData[cChunkDef::Width * cChunkDef::Width];
	NOISE_DATATYPE Workspace[cChunkDef::Width * cChunkDef::Width];
	NOISE_DATATYPE StartX = static_cast<NOISE_DATATYPE>(a_ChunkCoords.m_ChunkX       * cChunkDef::Width) / m_FrequencyX;
	NOISE_DATATYPE EndX   = static_cast<NOISE_DATATYPE>((a_ChunkCoords.m_ChunkX + 1) * cChunkDef::Width) / m_FrequencyX;
	NOISE_DATATYPE StartZ = static_cast<NOISE_DATATYPE>(a_ChunkCoords.m_ChunkZ       * cChunkDef::Width) / m_FrequencyZ;
	NOISE_DATATYPE EndZ   = static_cast<NOISE_DATATYPE>((a_ChunkCoords.m_ChunkZ + 1) * cChunkDef::Width) / m_FrequencyZ;

	m_Perlin.Generate2D(NoiseData, cChunkDef::Width, cChunkDef::Width, StartX, EndX, StartZ, EndZ, Workspace);
	m_VoidOffsetNoise.Generate2D(VoidOffsetData, cChunkDef::Width, cChunkDef::Width, StartX, EndX, StartZ, EndZ, Workspace);

	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			NOISE_DATATYPE noise = NoiseData[z * cChunkDef::Width + x];

			// The distance from spawn is used to create the void between the main island and the other islands.
			double distanceFromSpawn = cChunkDef::RelativeToAbsolute({ x, 0, z }, a_ChunkCoords).Length();

			// The main island can get a different airthreshold. This way the other island can be more sparse while the main island
			// is one big island.
			if (distanceFromSpawn > m_MainIslandSize)
			{
				if (noise <= m_AirThresholdOtherIslands)
				{
					continue;
				}
				noise -= m_AirThresholdOtherIslands;
			}
			else
			{
				if (noise <= m_AirThresholdMainIsland)
				{
					continue;
				}
				noise -= m_AirThresholdMainIsland;
			}
			NOISE_DATATYPE voidOffset = VoidOffsetData[z * cChunkDef::Width + x];


			double maxHeightLimit;
			if (distanceFromSpawn > m_MainIslandSize * 3)
			{
				// The distance from spawn is so big we don't need to calculate the max height anymore.
				// In fact, if we don't cut it off somewhere there is a chance the maxheight gets too big which
				// can cause corrupted looking terrain.
				maxHeightLimit = static_cast<float>(cChunkDef::Height);
			}
			else
			{
				// Create a void between the main island and the other island using the formula 'x^3 - 3 * x' where x is distance from spawn.
				double pow = std::pow((distanceFromSpawn - m_MainIslandSize) / m_MainIslandSize, 3);
				double mult = 3 * ((distanceFromSpawn - m_MainIslandSize) / m_MainIslandSize);
				maxHeightLimit = Clamp((pow - mult) * 100 + static_cast<double>(voidOffset) * m_VoidOffsetNoiseMultiplier, 0.0, static_cast<double>(cChunkDef::Height));
			}
			int maxHeight = static_cast<int>(Clamp(m_BaseHeight + static_cast<double>(noise) * m_TerrainTopMultiplier, 0.0, maxHeightLimit));
			int minHeight = static_cast<int>(Clamp(m_BaseHeight - static_cast<double>(noise) * m_TerrainBottomMultiplier, 0.0, static_cast<double>(cChunkDef::Height)));

			for (int y = minHeight; y < maxHeight; y++)
			{
				a_Shape[y + x * 256 + z * 256 * 16] = 1;
			}
		}
	}
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




