
// FinishGen.cpp

/* Implements the various finishing generators:
	- cFinishGenSnow
	- cFinishGenIce
	- cFinishGenSprinkleFoliage
*/

#include "Globals.h"

#include "FinishGen.h"
#include "../Noise.h"
#include "../BlockID.h"
#include "../Simulator/FluidSimulator.h"  // for cFluidSimulator::CanWashAway()
#include "../Simulator/FireSimulator.h"
#include "../World.h"





#define DEF_NETHER_WATER_SPRINGS    "0, 1; 255, 1"
#define DEF_NETHER_LAVA_SPRINGS     "0, 0; 30, 0; 31, 50; 120, 50; 127, 0"
#define DEF_OVERWORLD_WATER_SPRINGS "0, 0; 10, 10; 11, 75; 16, 83; 20, 83; 24, 78; 32, 62; 40, 40; 44, 15; 48, 7; 56, 2; 64, 1; 255, 0"
#define DEF_OVERWORLD_LAVA_SPRINGS  "0, 0; 10, 5; 11, 45; 48, 2; 64, 1; 255, 0"
#define DEF_END_WATER_SPRINGS       "0, 1; 255, 1"
#define DEF_END_LAVA_SPRINGS        "0, 1; 255, 1"





static inline bool IsWater(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_STATIONARY_WATER) || (a_BlockType == E_BLOCK_WATER);
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenNetherClumpFoliage:

void cFinishGenNetherClumpFoliage::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();

	int Val1 = m_Noise.IntNoise2DInt(ChunkX ^ ChunkZ, ChunkZ + ChunkX);
	int Val2 = m_Noise.IntNoise2DInt(ChunkZ ^ ChunkX, ChunkZ - ChunkX);

	int PosX = Val1 % 16;
	int PosZ = Val2 % 16;

	for (int y = 1; y < cChunkDef::Height; y++)
	{
		if (a_ChunkDesc.GetBlockType(PosX, y, PosZ) != E_BLOCK_AIR)
		{
			continue;
		}

		if (!cBlockInfo::IsSolid(a_ChunkDesc.GetBlockType(PosX, y - 1, PosZ)))  // Only place on solid blocks
		{
			continue;
		}
		
		// Choose what block to use.
		NOISE_DATATYPE BlockType = m_Noise.IntNoise3D((int) ChunkX, y, (int) ChunkZ);
		if (BlockType < -0.7)
		{
			TryPlaceClump(a_ChunkDesc, PosX, y, PosZ, E_BLOCK_BROWN_MUSHROOM);
		}
		else if (BlockType < 0)
		{
			TryPlaceClump(a_ChunkDesc, PosX, y, PosZ, E_BLOCK_RED_MUSHROOM);
		}
		else if (BlockType < 0.7)
		{
			TryPlaceClump(a_ChunkDesc, PosX, y, PosZ, E_BLOCK_FIRE);
		}
	}
}





void cFinishGenNetherClumpFoliage::TryPlaceClump(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_Block)
{
	bool IsFireBlock = a_Block == E_BLOCK_FIRE;

	int MinX = a_RelX - 4;
	if (MinX < 0)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MinX = 0;
	}

	int MaxX = a_RelX + 4;
	if (MaxX > cChunkDef::Width)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MaxX = cChunkDef::Width;
	}

	int MinZ = a_RelZ - 4;
	if (MinZ < 0)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MinZ = 0;
	}

	int MaxZ = a_RelZ + 4;
	if (MaxZ > cChunkDef::Width)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MaxZ = cChunkDef::Width;
	}

	int MinY = a_RelY - 2;
	if (MinY < 0)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MinY = 0;
	}

	int MaxY = a_RelY + 2;
	if (MaxY > cChunkDef::Height)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MaxY = cChunkDef::Height;
	}

	for (int x = MinX; x < MaxX; x++)
	{
		int xx = a_ChunkDesc.GetChunkX() * cChunkDef::Width + x;
		for (int z = MinZ; z < MaxZ; z++)
		{
			int zz = a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z;
			for (int y = MinY; y < MaxY; y++)
			{
				if (
					((x < 0) || (x >= cChunkDef::Width)) ||
					((y < 0) || (y >= cChunkDef::Height)) ||
					((z < 0) || (z >= cChunkDef::Width))
					)
				{
					continue;
				}

				if (a_ChunkDesc.GetBlockType(x, y, z) != E_BLOCK_AIR)  // Don't replace non air blocks.
				{
					continue;
				}

				BLOCKTYPE BlockBelow = a_ChunkDesc.GetBlockType(x, y - 1, z);
				if (!cBlockInfo::FullyOccupiesVoxel(BlockBelow))  // Only place on solid blocks
				{
					continue;
				}

				if (IsFireBlock)  // don't place fire on non-forever burning blocks.
				{
					if (!cFireSimulator::DoesBurnForever(BlockBelow))
					{
						continue;
					}
				}

				NOISE_DATATYPE Val = m_Noise.IntNoise2D(xx, zz);
				if (Val < -0.5)
				{
					a_ChunkDesc.SetBlockType(x, y, z, a_Block);
				}
			}
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenTallGrass:

void cFinishGenTallGrass::GenFinish(cChunkDesc & a_ChunkDesc)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		int xx = x + a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			int zz = z + a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
			int BiomeDensity = GetBiomeDensity(a_ChunkDesc.GetBiome(x, z));

			// Choose if we want to place long grass here. If not then bail out:
			if ((m_Noise.IntNoise2DInt(xx + m_Noise.IntNoise1DInt(xx), zz + m_Noise.IntNoise1DInt(zz)) / 7 % 100) > BiomeDensity)
			{
				continue;
			}
			
			// Get the top block + 1. This is the place where the grass would finaly be placed:
			int y = a_ChunkDesc.GetHeight(x, z) + 1;

			// Check if long grass can be placed:
			if (
				(a_ChunkDesc.GetBlockType(x, y, z) != E_BLOCK_AIR) ||
				((a_ChunkDesc.GetBlockType(x, y - 1, z) != E_BLOCK_GRASS) && (a_ChunkDesc.GetBlockType(x, y - 1, z) != E_BLOCK_DIRT))
				)
			{
				continue;
			}

			// Choose what long grass meta we should use:
			int GrassType = m_Noise.IntNoise2DInt(xx * 50, zz * 50) / 7 % 100;
			if (GrassType < 60)
			{
				a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_TALL_GRASS, 1);
			}
			else if (GrassType < 90)
			{
				a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_TALL_GRASS, 2);
			}
			else
			{
				// If double long grass we have to choose what type we should use:
				if (a_ChunkDesc.GetBlockType(x, y + 1, z) == E_BLOCK_AIR)
				{
					NIBBLETYPE Meta = (m_Noise.IntNoise2DInt(xx * 100, zz * 100) / 7 % 100) > 25 ? 2 : 3;
					a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_BIG_FLOWER, Meta);
					a_ChunkDesc.SetBlockTypeMeta(x, y + 1, z, E_BLOCK_BIG_FLOWER, 8);
				}
			}
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenSprinkleFoliage:

bool cFinishGenSprinkleFoliage::TryAddSugarcane(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ)
{
	// We'll be doing comparison to neighbors, so require the coords to be 1 block away from the chunk edges:
	if (
		(a_RelX < 1) || (a_RelX >= cChunkDef::Width  - 1) ||
		(a_RelY < 1) || (a_RelY >= cChunkDef::Height - 2) ||
		(a_RelZ < 1) || (a_RelZ >= cChunkDef::Width  - 1)
	)
	{
		return false;
	}

	// Only allow dirt, grass or sand below sugarcane:
	switch (a_ChunkDesc.GetBlockType(a_RelX, a_RelY, a_RelZ))
	{
		case E_BLOCK_DIRT:
		case E_BLOCK_GRASS:
		case E_BLOCK_SAND:
		{
			break;
		}
		default:
		{
			return false;
		}
	}

	// Water is required next to the block below the sugarcane:
	if (
		!IsWater(a_ChunkDesc.GetBlockType(a_RelX - 1, a_RelY, a_RelZ)) &&
		!IsWater(a_ChunkDesc.GetBlockType(a_RelX + 1, a_RelY, a_RelZ)) &&
		!IsWater(a_ChunkDesc.GetBlockType(a_RelX,     a_RelY, a_RelZ - 1)) &&
		!IsWater(a_ChunkDesc.GetBlockType(a_RelX,     a_RelY, a_RelZ + 1))
	)
	{
		return false;
	}
	
	// All conditions met, place a sugarcane here:
	a_ChunkDesc.SetBlockType(a_RelX, a_RelY + 1, a_RelZ, E_BLOCK_SUGARCANE);
	return true;
}





void cFinishGenSprinkleFoliage::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// Generate small foliage (1-block):
	
	// TODO: Update heightmap with 1-block-tall foliage
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z;
		const float zz = (float)BlockZ;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width + x;
			if (((m_Noise.IntNoise2DInt(BlockX, BlockZ) / 8) % 128) < 124)
			{
				continue;
			}
			int Top = a_ChunkDesc.GetHeight(x, z);
			if (Top > 250)
			{
				// Nothing grows above Y=250
				continue;
			}
			if (a_ChunkDesc.GetBlockType(x, Top + 1, z) != E_BLOCK_AIR)
			{
				// Space already taken by something else, don't grow here
				// WEIRD, since we're using heightmap, so there should NOT be anything above it
				continue;
			}
			
			const float xx = (float)BlockX;
			float val1 = m_Noise.CubicNoise2D(xx * 0.1f,  zz * 0.1f);
			float val2 = m_Noise.CubicNoise2D(xx * 0.01f, zz * 0.01f);
			switch (a_ChunkDesc.GetBlockType(x, Top, z))
			{
				case E_BLOCK_GRASS:
				{
					float val3 = m_Noise.CubicNoise2D(xx * 0.01f + 10, zz * 0.01f + 10);
					float val4 = m_Noise.CubicNoise2D(xx * 0.05f + 20, zz * 0.05f + 20);
					if (val1 + val2 > 0.2f)
					{
						a_ChunkDesc.SetBlockType(x, ++Top, z, E_BLOCK_YELLOW_FLOWER);
					}
					else if (val2 + val3 > 0.2f)
					{
						a_ChunkDesc.SetBlockType(x, ++Top, z, E_BLOCK_RED_ROSE);
					}
					else if (val3 + val4 > 0.2f)
					{
						a_ChunkDesc.SetBlockType(x, ++Top, z, E_BLOCK_RED_MUSHROOM);
					}
					else if (val1 + val4 > 0.2f)
					{
						a_ChunkDesc.SetBlockType(x, ++Top, z, E_BLOCK_BROWN_MUSHROOM);
					}
					else if (val1 + val2 + val3 + val4 < -0.1)
					{
						a_ChunkDesc.SetBlockTypeMeta(x, ++Top, z, E_BLOCK_TALL_GRASS, E_META_TALL_GRASS_GRASS);
					}
					else if (TryAddSugarcane(a_ChunkDesc, x, Top, z))
					{
						++Top;
					}
					else if ((val1 > 0.5) && (val2 < -0.5))
					{
						a_ChunkDesc.SetBlockTypeMeta(x, ++Top, z, E_BLOCK_PUMPKIN, (int)(val3 * 8) % 4);
					}
					break;
				}  // case E_BLOCK_GRASS
				
				case E_BLOCK_SAND:
				{
					int y = Top + 1;
					if (
						(x > 0) && (x < cChunkDef::Width - 1) &&
						(z > 0) && (z < cChunkDef::Width - 1) &&
						(val1 + val2 > 0.5f) &&
						(a_ChunkDesc.GetBlockType(x + 1, y, z)     == E_BLOCK_AIR) &&
						(a_ChunkDesc.GetBlockType(x - 1, y, z)     == E_BLOCK_AIR) &&
						(a_ChunkDesc.GetBlockType(x,     y, z + 1) == E_BLOCK_AIR) &&
						(a_ChunkDesc.GetBlockType(x,     y, z - 1) == E_BLOCK_AIR)
					)
					{
						a_ChunkDesc.SetBlockType(x, ++Top, z, E_BLOCK_CACTUS);
					}
					else if (TryAddSugarcane(a_ChunkDesc, x, Top, z))
					{
						++Top;
					}
					break;
				}
			}  // switch (TopBlock)
			a_ChunkDesc.SetHeight(x, z, Top);
		}  // for y
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenSnow:

void cFinishGenSnow::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// Add a snow block in snowy biomes onto blocks that can be snowed over
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			switch (a_ChunkDesc.GetBiome(x, z))
			{
				case biIcePlains:
				case biIceMountains:
				case biTaiga:
				case biTaigaHills:
				case biFrozenRiver:
				case biFrozenOcean:
				{
					int Height = a_ChunkDesc.GetHeight(x, z);
					if (cBlockInfo::IsSnowable(a_ChunkDesc.GetBlockType(x, Height, z)))
					{
						a_ChunkDesc.SetBlockType(x, Height + 1, z, E_BLOCK_SNOW);
						a_ChunkDesc.SetHeight(x, z, Height + 1);
					}
					break;
				}
			}
		}
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenIce:

void cFinishGenIce::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// Turn surface water into ice in icy biomes
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			switch (a_ChunkDesc.GetBiome(x, z))
			{
				case biIcePlains:
				case biIceMountains:
				case biTaiga:
				case biTaigaHills:
				case biFrozenRiver:
				case biFrozenOcean:
				{
					int Height = a_ChunkDesc.GetHeight(x, z);
					switch (a_ChunkDesc.GetBlockType(x, Height, z))
					{
						case E_BLOCK_WATER:
						case E_BLOCK_STATIONARY_WATER:
						{
							a_ChunkDesc.SetBlockType(x, Height, z, E_BLOCK_ICE);
							break;
						}
					}
					break;
				}
			}
		}
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenSingleTopBlock:

int cFinishGenSingleTopBlock::GetNumToGen(const cChunkDef::BiomeMap & a_BiomeMap)
{
	int res = 0;
	for (size_t i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		if (IsAllowedBiome(a_BiomeMap[i]))
		{
			res++;
		}
	}  // for i - a_BiomeMap[]
	return m_Amount * res / 256;
}





void cFinishGenSingleTopBlock::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int NumToGen = GetNumToGen(a_ChunkDesc.GetBiomeMap());
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	for (int i = 0; i < NumToGen; i++)
	{
		int x = (m_Noise.IntNoise3DInt(ChunkX + ChunkZ, ChunkZ, i) / 13) % cChunkDef::Width;
		int z = (m_Noise.IntNoise3DInt(ChunkX - ChunkZ, i, ChunkZ) / 11) % cChunkDef::Width;

		// Place the block at {x, z} if possible:
		EMCSBiome Biome = a_ChunkDesc.GetBiome(x, z);
		if (!IsAllowedBiome(Biome))
		{
			// Incorrect biome
			continue;
		}

		int Height = a_ChunkDesc.GetHeight(x, z);
		if (Height >= cChunkDef::Height)
		{
			// Too high up
			continue;
		}
		if (a_ChunkDesc.GetBlockType(x, Height + 1, z) != E_BLOCK_AIR)
		{
			// Not an empty block
			continue;
		}

		BLOCKTYPE BlockBelow = a_ChunkDesc.GetBlockType(x, Height, z);
		if (!IsAllowedBlockBelow(BlockBelow))
		{
			continue;
		}

		a_ChunkDesc.SetBlockType(x, Height + 1, z, m_BlockType);
		a_ChunkDesc.SetHeight(x, z, Height + 1);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenBottomLava:

void cFinishGenBottomLava::GenFinish(cChunkDesc & a_ChunkDesc)
{
	cChunkDef::BlockTypes & BlockTypes = a_ChunkDesc.GetBlockTypes();
	for (int y = m_Level; y > 0; y--)
	{
		for (int z = 0; z < cChunkDef::Width; z++) for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Index = cChunkDef::MakeIndexNoCheck(x, y, z);
			if (BlockTypes[Index] == E_BLOCK_AIR)
			{
				BlockTypes[Index] = E_BLOCK_STATIONARY_LAVA;
			}
		}  // for x, for z
	}  // for y
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenPreSimulator:

cFinishGenPreSimulator::cFinishGenPreSimulator(bool a_PreSimulateFallingBlocks, bool a_PreSimulateWater, bool a_PreSimulateLava) :
	m_PreSimulateFallingBlocks(a_PreSimulateFallingBlocks),
	m_PreSimulateWater(a_PreSimulateWater),
	m_PreSimulateLava(a_PreSimulateLava)
{
	// Nothing needed yet
}





void cFinishGenPreSimulator::GenFinish(cChunkDesc & a_ChunkDesc)
{
	if (m_PreSimulateFallingBlocks)
	{
		CollapseSandGravel(a_ChunkDesc.GetBlockTypes(), a_ChunkDesc.GetHeightMap());
	}

	if (m_PreSimulateWater)
	{
		StationarizeFluid(a_ChunkDesc.GetBlockTypes(), a_ChunkDesc.GetHeightMap(), E_BLOCK_WATER, E_BLOCK_STATIONARY_WATER);
	}

	if (m_PreSimulateLava)
	{
		StationarizeFluid(a_ChunkDesc.GetBlockTypes(), a_ChunkDesc.GetHeightMap(), E_BLOCK_LAVA, E_BLOCK_STATIONARY_LAVA);
	}
	// TODO: other operations
}





void cFinishGenPreSimulator::CollapseSandGravel(
	cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
	cChunkDef::HeightMap & a_HeightMap       // Height map to update by the current data
)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int LastY = -1;
			int HeightY = 0;
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				BLOCKTYPE Block = cChunkDef::GetBlock(a_BlockTypes, x, y, z);
				switch (Block)
				{
					default:
					{
						// Set the last block onto which stuff can fall to this height:
						LastY = y;
						HeightY = y;
						break;
					}
					case E_BLOCK_AIR:
					{
						// Do nothing
						break;
					}
					case E_BLOCK_FIRE:
					case E_BLOCK_WATER:
					case E_BLOCK_STATIONARY_WATER:
					case E_BLOCK_LAVA:
					case E_BLOCK_STATIONARY_LAVA:
					{
						// Do nothing, only remember this height as potentially highest
						HeightY = y;
						break;
					}
					case E_BLOCK_SAND:
					case E_BLOCK_GRAVEL:
					{
						if (LastY < y - 1)
						{
							cChunkDef::SetBlock(a_BlockTypes, x, LastY + 1, z, Block);
							cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_AIR);
						}
						LastY++;
						if (LastY > HeightY)
						{
							HeightY = LastY;
						}
						break;
					}
				}  // switch (GetBlock)
			}  // for y
			cChunkDef::SetHeight(a_HeightMap, x, z, HeightY);
		}  // for x
	}  // for z
}





void cFinishGenPreSimulator::StationarizeFluid(
	cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
	cChunkDef::HeightMap & a_HeightMap,      // Height map to read
	BLOCKTYPE a_Fluid,
	BLOCKTYPE a_StationaryFluid
)
{
	// Turn fluid in the middle to stationary, unless it has air or washable block next to it:
	for (int z = 1; z < cChunkDef::Width - 1; z++)
	{
		for (int x = 1; x < cChunkDef::Width - 1; x++)
		{
			for (int y = cChunkDef::GetHeight(a_HeightMap, x, z); y >= 0; y--)
			{
				BLOCKTYPE Block = cChunkDef::GetBlock(a_BlockTypes, x, y, z);
				if ((Block != a_Fluid) && (Block != a_StationaryFluid))
				{
					continue;
				}
				static const struct
				{
					int x, y, z;
				} Coords[] =
				{
					{1, 0, 0},
					{-1, 0, 0},
					{0, 0, 1},
					{0, 0, -1},
					{0, -1, 0}
				} ;
				BLOCKTYPE BlockToSet = a_StationaryFluid;  // By default, don't simulate this block
				for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
				{
					if ((y == 0) && (Coords[i].y < 0))
					{
						continue;
					}
					BLOCKTYPE Neighbor = cChunkDef::GetBlock(a_BlockTypes, x + Coords[i].x, y + Coords[i].y, z + Coords[i].z);
					if ((Neighbor == E_BLOCK_AIR) || cFluidSimulator::CanWashAway(Neighbor))
					{
						// There is an air / washable neighbor, simulate this block
						BlockToSet = a_Fluid;
						break;
					}
				}  // for i - Coords[]
				cChunkDef::SetBlock(a_BlockTypes, x, y, z, BlockToSet);
			}  // for y
		}  // for x
	}  // for z
	
	// Turn fluid at the chunk edges into non-stationary fluid:
	for (int y = 0; y < cChunkDef::Height; y++)
	{
		for (int i = 0; i < cChunkDef::Width; i++)  // i stands for both x and z here
		{
			if (cChunkDef::GetBlock(a_BlockTypes, 0, y, i) == a_StationaryFluid)
			{
				cChunkDef::SetBlock(a_BlockTypes, 0, y, i, a_Fluid);
			}
			if (cChunkDef::GetBlock(a_BlockTypes, i, y, 0) == a_StationaryFluid)
			{
				cChunkDef::SetBlock(a_BlockTypes, i, y, 0, a_Fluid);
			}
			if (cChunkDef::GetBlock(a_BlockTypes, cChunkDef::Width - 1, y, i) == a_StationaryFluid)
			{
				cChunkDef::SetBlock(a_BlockTypes, cChunkDef::Width - 1, y, i, a_Fluid);
			}
			if (cChunkDef::GetBlock(a_BlockTypes, i, y, cChunkDef::Width - 1) == a_StationaryFluid)
			{
				cChunkDef::SetBlock(a_BlockTypes, i, y, cChunkDef::Width - 1, a_Fluid);
			}
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenFluidSprings:

cFinishGenFluidSprings::cFinishGenFluidSprings(int a_Seed, BLOCKTYPE a_Fluid, cIniFile & a_IniFile, eDimension a_Dimension) :
	m_Noise(a_Seed + a_Fluid * 100),  // Need to take fluid into account, otherwise water and lava springs generate next to each other
	m_HeightDistribution(255),
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




