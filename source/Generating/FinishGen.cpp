
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





static inline bool IsWater(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_STATIONARY_WATER) || (a_BlockType == E_BLOCK_WATER);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFinishGenSprinkleFoliage:

bool cFinishGenSprinkleFoliage::TryAddSugarcane(
	int a_ChunkX, int a_ChunkZ,
	int a_RelX, int a_RelY, int a_RelZ,
	cChunkDef::BlockTypes & a_BlockTypes,
	cChunkDef::BlockNibbles & a_BlockMeta
)
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
	switch (cChunkDef::GetBlock(a_BlockTypes, a_RelX, a_RelY, a_RelZ))
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
		!IsWater(cChunkDef::GetBlock(a_BlockTypes, a_RelX - 1, a_RelY, a_RelZ)) &&
		!IsWater(cChunkDef::GetBlock(a_BlockTypes, a_RelX + 1, a_RelY, a_RelZ)) &&
		!IsWater(cChunkDef::GetBlock(a_BlockTypes, a_RelX    , a_RelY, a_RelZ - 1)) &&
		!IsWater(cChunkDef::GetBlock(a_BlockTypes, a_RelX    , a_RelY, a_RelZ + 1))
	)
	{
		return false;
	}
	
	// All conditions met, place a sugarcane here:
	cChunkDef::SetBlock(a_BlockTypes, a_RelX, a_RelY + 1, a_RelZ, E_BLOCK_SUGARCANE);
	return true;
}





void cFinishGenSprinkleFoliage::GenFinish(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,   // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
	const cChunkDef::BiomeMap & a_BiomeMap,  // Biomes to adhere to
	cEntityList & a_Entities,                // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
	)
{
	// Generate small foliage (1-block):
	
	// TODO: Update heightmap with 1-block-tall foliage
	cNoise Noise(m_Seed);
	for (int z = 0; z < cChunkDef::Width; z++) 
	{
		int BlockZ = a_ChunkZ * cChunkDef::Width + z;
		const float zz = (float)BlockZ;
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int BlockX = a_ChunkX * cChunkDef::Width + x;
			if (((Noise.IntNoise2DInt(BlockX, BlockZ) / 8) % 128) < 124)
			{
				continue;
			}
			int Top = cChunkDef::GetHeight(a_HeightMap, x, z);
			if (Top > 250)
			{
				// Nothing grows above Y=250
				continue;
			}
			if (cChunkDef::GetBlock(a_BlockTypes, x, Top + 1, z) != E_BLOCK_AIR)
			{
				// Space already taken by something else, don't grow here
				// WEIRD, since we're using heightmap, so there should NOT be anything above it
				continue;
			}
			
			const float xx = (float)BlockX;
			float val1 = Noise.CubicNoise2D(xx * 0.1f,  zz * 0.1f );
			float val2 = Noise.CubicNoise2D(xx * 0.01f, zz * 0.01f );
			switch (cChunkDef::GetBlock(a_BlockTypes, x, Top, z))
			{
				case E_BLOCK_GRASS:
				{
					float val3 = Noise.CubicNoise2D(xx * 0.01f + 10, zz * 0.01f + 10 );
					float val4 = Noise.CubicNoise2D(xx * 0.05f + 20, zz * 0.05f + 20 );
					if (val1 + val2 > 0.2f)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, ++Top, z, E_BLOCK_YELLOW_FLOWER);
					}
					else if (val2 + val3 > 0.2f)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, ++Top, z, E_BLOCK_RED_ROSE);
					}
					else if (val3 + val4 > 0.2f)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, ++Top, z, E_BLOCK_RED_MUSHROOM);
					}
					else if (val1 + val4 > 0.2f)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, ++Top, z, E_BLOCK_BROWN_MUSHROOM);
					}
					else if (val1 + val2 + val3 + val4 < -0.1)
					{
						cChunkDef::SetBlock (a_BlockTypes, x, ++Top, z, E_BLOCK_TALL_GRASS);
						cChunkDef::SetNibble(a_BlockMeta,  x,   Top, z, E_META_TALL_GRASS_GRASS);
					}
					else if (TryAddSugarcane(a_ChunkX, a_ChunkZ, x, Top, z, a_BlockTypes, a_BlockMeta))
					{
						++Top;
					}
					else if ((val1 > 0.5) && (val2 < -0.5))
					{
						cChunkDef::SetBlock (a_BlockTypes, x, ++Top, z, E_BLOCK_PUMPKIN);
						cChunkDef::SetNibble(a_BlockMeta,  x,   Top, z, (int)(val3 * 8) % 4);
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
						(cChunkDef::GetBlock(a_BlockTypes, x + 1, y, z)     == E_BLOCK_AIR) &&
						(cChunkDef::GetBlock(a_BlockTypes, x - 1, y, z)     == E_BLOCK_AIR) &&
						(cChunkDef::GetBlock(a_BlockTypes, x,     y, z + 1) == E_BLOCK_AIR) &&
						(cChunkDef::GetBlock(a_BlockTypes, x,     y, z - 1) == E_BLOCK_AIR)
					)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, ++Top, z, E_BLOCK_CACTUS);
					}
					else if (TryAddSugarcane(a_ChunkX, a_ChunkZ, x, Top, z, a_BlockTypes, a_BlockMeta))
					{
						++Top;
					}
					break;
				}
			}  // switch (TopBlock)
			cChunkDef::SetHeight(a_HeightMap, x, z, Top);
		}  // for y
	}  // for z
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFinishGenSnow:

void cFinishGenSnow::GenFinish(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,   // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
	const cChunkDef::BiomeMap & a_BiomeMap,  // Biomes to adhere to
	cEntityList & a_Entities,                // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
	)
{
	// Add a snow block in snowy biomes onto blocks that can be snowed over
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			switch (cChunkDef::GetBiome(a_BiomeMap, x, z))
			{
				case biIcePlains:
				case biIceMountains:
				case biTaiga:
				case biTaigaHills:
				case biFrozenRiver:
				case biFrozenOcean:
				{
					int Height = cChunkDef::GetHeight(a_HeightMap, x, z);
					if (g_BlockIsSnowable[cChunkDef::GetBlock(a_BlockTypes, x, Height, z)])
					{
						cChunkDef::SetBlock(a_BlockTypes, x, Height + 1, z, E_BLOCK_SNOW);
						cChunkDef::SetHeight(a_HeightMap, x, z, Height + 1);
					}
					break;
				}
			}
		}
	}  // for z
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFinishGenIce:

void cFinishGenIce::GenFinish(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,   // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
	const cChunkDef::BiomeMap & a_BiomeMap,  // Biomes to adhere to
	cEntityList & a_Entities,                // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
	)
{
	// Turn surface water into ice in icy biomes
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			switch (cChunkDef::GetBiome(a_BiomeMap, x, z))
			{
				case biIcePlains:
				case biIceMountains:
				case biTaiga:
				case biTaigaHills:
				case biFrozenRiver:
				case biFrozenOcean:
				{
					int Height = cChunkDef::GetHeight(a_HeightMap, x, z);
					switch (cChunkDef::GetBlock(a_BlockTypes, x, Height, z))
					{
						case E_BLOCK_WATER:
						case E_BLOCK_STATIONARY_WATER:
						{
							cChunkDef::SetBlock(a_BlockTypes, x, Height, z, E_BLOCK_ICE);
							break;
						}
					}
					break;
				}
			}
		}
	}  // for z
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFinishGenLilypads:

int cFinishGenLilypads::GetNumLilypads(const cChunkDef::BiomeMap & a_BiomeMap)
{
	int res = 0;
	for (int i = 0; i < ARRAYCOUNT(a_BiomeMap); i++)
	{
		if (a_BiomeMap[i] == biSwampland)
		{
			res++;
		}
	}  // for i - a_BiomeMap[]
	return res / 64;
}





void cFinishGenLilypads::GenFinish(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes, // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta, // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap, // Height map to read and change by the current data
	const cChunkDef::BiomeMap & a_BiomeMap, // Biomes to adhere to
	cEntityList & a_Entities, // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities // Block entities may be added or deleted
)
{
	// Add Lilypads on top of water surface in Swampland

	int NumLilypads = GetNumLilypads(a_BiomeMap);
	for (int i = 0; i < NumLilypads; i++)
	{
		int x = m_Noise.IntNoise3DInt(a_ChunkX + a_ChunkZ, a_ChunkZ, i) % cChunkDef::Width;
		int z = m_Noise.IntNoise3DInt(a_ChunkX - a_ChunkZ, i, a_ChunkZ) % cChunkDef::Width;

		// Place a lily pad at {x, z} if possible (swampland, empty block, water below):
		if (cChunkDef::GetBiome(a_BiomeMap, x, z) != biSwampland)
		{
			// not swampland
			continue;
		}
		int Height = cChunkDef::GetHeight(a_HeightMap, x, z);
		if (Height >= cChunkDef::Height)
		{
			// Too high up
			continue;
		}
		if (cChunkDef::GetBlock(a_BlockTypes, x, Height + 1, z) != E_BLOCK_AIR)
		{
			// not empty block
			continue;
		}
		switch (cChunkDef::GetBlock(a_BlockTypes, x, Height, z))
		{
			case E_BLOCK_WATER:
			case E_BLOCK_STATIONARY_WATER:
			{
				cChunkDef::SetBlock(a_BlockTypes, x, Height + 1, z, E_BLOCK_LILY_PAD);
				cChunkDef::SetHeight(a_HeightMap, x, z, Height + 1);
				break;
			}
		}  // switch (GetBlock)
	}  // for i
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFinishGenBottomLava:

void cFinishGenBottomLava::GenFinish(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,   // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
	const cChunkDef::BiomeMap & a_BiomeMap,  // Biomes to adhere to
	cEntityList & a_Entities,                // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
)
{
	for (int y = m_Level; y > 0; y--)
	{
		for (int z = 0; z < cChunkDef::Width; z++) for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Index = cChunkDef::MakeIndexNoCheck(x, y, z);
			if (a_BlockTypes[Index] == E_BLOCK_AIR)
			{
				a_BlockTypes[Index] = E_BLOCK_STATIONARY_LAVA;
			}
		}  // for x, for z
	}  // for y
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFinishGenPreSimulator:

cFinishGenPreSimulator::cFinishGenPreSimulator(void)
{
	// Nothing needed yet
}





void cFinishGenPreSimulator::GenFinish(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
	cChunkDef::BlockNibbles & a_BlockMeta,   // Block meta to read and change
	cChunkDef::HeightMap & a_HeightMap,      // Height map to read and change by the current data
	const cChunkDef::BiomeMap & a_BiomeMap,  // Biomes to adhere to
	cEntityList & a_Entities,                // Entities may be added or deleted
	cBlockEntityList & a_BlockEntities       // Block entities may be added or deleted
)
{
	CollapseSandGravel(a_BlockTypes, a_HeightMap);
	StationarizeFluid(a_BlockTypes, a_HeightMap, E_BLOCK_WATER, E_BLOCK_STATIONARY_WATER);
	StationarizeFluid(a_BlockTypes, a_HeightMap, E_BLOCK_LAVA,  E_BLOCK_STATIONARY_LAVA);
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
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				BLOCKTYPE Block = cChunkDef::GetBlock(a_BlockTypes, x, y, z);
				switch (Block)
				{
					default:
					{
						// Set the last block onto which stuff can fall to this height:
						LastY = y;
						break;
					}
					case E_BLOCK_AIR:
					{
						// Do nothing
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
						break;
					}
				}  // switch (GetBlock)
			}  // for y
			cChunkDef::SetHeight(a_HeightMap, x, z, LastY);
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
				for (int i = 0; i < ARRAYCOUNT(Coords); i++)
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
		for (int i = 1; i < cChunkDef::Width; i++)  // i stands for both x and z here
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




