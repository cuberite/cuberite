
// FinishGenFluidSprings.cpp

#include "FinishGenFluidSprings.h"
#include "../../IniFile.h"


#define DEF_NETHER_WATER_SPRINGS    "0, 1; 255, 1"
#define DEF_NETHER_LAVA_SPRINGS     "0, 0; 30, 0; 31, 50; 120, 50; 127, 0"
#define DEF_OVERWORLD_WATER_SPRINGS "0, 0; 10, 10; 11, 75; 16, 83; 20, 83; 24, 78; 32, 62; 40, 40; 44, 15; 48, 7; 56, 2; 64, 1; 255, 0"
#define DEF_OVERWORLD_LAVA_SPRINGS  "0, 0; 10, 5; 11, 45; 48, 2; 64, 1; 255, 0"
#define DEF_END_WATER_SPRINGS       "0, 1; 255, 1"
#define DEF_END_LAVA_SPRINGS        "0, 1; 255, 1"

cFinishGenFluidSprings::cFinishGenFluidSprings(int a_Seed, BLOCKTYPE a_Fluid, cIniFile & a_IniFile, eDimension a_Dimension) :
	m_Noise(a_Seed + a_Fluid * 100),  // Need to take fluid into account, otherwise water and lava springs generate next to each other
	m_HeightDistribution(cChunkDef::Height - 1),
	m_Fluid(a_Fluid)
{
	bool IsWater = (a_Fluid == E_BLOCK_WATER);
	AString SectionName = IsWater ? "WaterSprings" : "LavaSprings";
	AString DefaultHeightDistribution;
	int DefaultChance = 0;
	switch (a_Dimension)
	{
		case dimNether:
		{
			DefaultHeightDistribution = IsWater ? DEF_NETHER_WATER_SPRINGS : DEF_NETHER_LAVA_SPRINGS;
			DefaultChance = IsWater ? 0 : 15;
			break;
		}
		case dimOverworld:
		{
			DefaultHeightDistribution = IsWater ? DEF_OVERWORLD_WATER_SPRINGS : DEF_OVERWORLD_LAVA_SPRINGS;
			DefaultChance = IsWater ? 24 : 9;
			break;
		}
		case dimEnd:
		{
			DefaultHeightDistribution = IsWater ? DEF_END_WATER_SPRINGS : DEF_END_LAVA_SPRINGS;
			DefaultChance = 0;
			break;
		}
		default:
		{
			ASSERT(!"Unhandled world dimension");
			break;
		}
	}  // switch (dimension)
	AString HeightDistribution = a_IniFile.GetValueSet(SectionName, "HeightDistribution", DefaultHeightDistribution);
	if (!m_HeightDistribution.SetDefString(HeightDistribution) || (m_HeightDistribution.GetSum() <= 0))
	{
		LOGWARNING("[%sSprings]: HeightDistribution is invalid, using the default of \"%s\".",
				   (a_Fluid == E_BLOCK_WATER) ? "Water" : "Lava",
				   DefaultHeightDistribution.c_str()
		);
		m_HeightDistribution.SetDefString(DefaultHeightDistribution);
	}
	m_Chance = a_IniFile.GetValueSetI(SectionName, "Chance", DefaultChance);
}





void cFinishGenFluidSprings::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChanceRnd = (m_Noise.IntNoise3DInt(128 * a_ChunkDesc.GetChunkX(), 512, 256 * a_ChunkDesc.GetChunkZ()) / 13) % 100;
	if (ChanceRnd > m_Chance)
	{
		// Not in this chunk
		return;
	}

	// Get the height at which to try:
	int Height = m_Noise.IntNoise3DInt(128 * a_ChunkDesc.GetChunkX(), 1024, 256 * a_ChunkDesc.GetChunkZ()) / 11;
	Height %= m_HeightDistribution.GetSum();
	Height = m_HeightDistribution.MapValue(Height);

	// Try adding the spring at the height, if unsuccessful, move lower:
	for (int y = Height; y > 1; y--)
	{
		// TODO: randomize the order in which the coords are being checked
		for (int z = 1; z < cChunkDef::Width - 1; z++)
		{
			for (int x = 1; x < cChunkDef::Width - 1; x++)
			{
				switch (a_ChunkDesc.GetBlockType(x, y, z))
				{
					case E_BLOCK_NETHERRACK:
					case E_BLOCK_STONE:
					{
						if (TryPlaceSpring(a_ChunkDesc, x, y, z))
						{
							// Succeeded, bail out
							return;
						}
					}
				}  // switch (BlockType)
			}  // for x
		}  // for y
	}  // for y
}





bool cFinishGenFluidSprings::TryPlaceSpring(cChunkDesc & a_ChunkDesc, int x, int y, int z)
{
	// In order to place a spring, it needs exactly one of the XZ neighbors or a below neighbor to be air
	// Also, its neighbor on top of it must be non-air
	if (a_ChunkDesc.GetBlockType(x, y + 1, z) == E_BLOCK_AIR)
	{
		return false;
	}

	static const struct
	{
		int x, y, z;
	} Coords[] =
		{
			{-1,  0,  0},
			{ 1,  0,  0},
			{ 0, -1,  0},
			{ 0,  0, -1},
			{ 0,  0,  1},
		} ;
	int NumAirNeighbors = 0;
	for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
	{
		switch (a_ChunkDesc.GetBlockType(x + Coords[i].x, y + Coords[i].y, z + Coords[i].z))
		{
			case E_BLOCK_AIR:
			{
				NumAirNeighbors += 1;
				if (NumAirNeighbors > 1)
				{
					return false;
				}
			}
		}
	}
	if (NumAirNeighbors == 0)
	{
		return false;
	}

	// Has exactly one air neighbor, place a spring:
	a_ChunkDesc.SetBlockTypeMeta(x, y, z, m_Fluid, 0);
	return true;
}
