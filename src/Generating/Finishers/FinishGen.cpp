
// FinishGen.cpp

/* Implements the various finishing generators:
	- cFinishGenSnow
	- cFinishGenIce
	- cFinishGenSprinkleFoliage
*/

#include "Globals.h"

#include "FinishGen.h"
#include "../../MobSpawner.h"
#include "../../BlockInfo.h"





////////////////////////////////////////////////////////////////////////////////
// cFinishGenVines

bool cFinishGenVines::IsJungleVariant(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biJungle:
		case biJungleEdge:
		case biJungleEdgeM:
		case biJungleHills:
		case biJungleM:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





void cFinishGenVines::GenFinish(cChunkDesc & a_ChunkDesc)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		int xx = x + a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			int zz = z + a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
			if (!IsJungleVariant(a_ChunkDesc.GetBiome(x, z)))
			{
				// Current biome isn't a jungle
				continue;
			}

			if ((m_Noise.IntNoise2DInt(xx, zz) % 101) < 50)
			{
				continue;
			}

			int Height = a_ChunkDesc.GetHeight(x, z);
			for (int y = Height; y > m_Level; y--)
			{
				if (a_ChunkDesc.GetBlockType(x, y, z) != E_BLOCK_AIR)
				{
					// Can't place vines in non-air blocks
					continue;
				}

				if ((m_Noise.IntNoise3DInt(xx, y, zz) % 101) < 50)
				{
					continue;
				}

				std::vector<NIBBLETYPE> Places;
				if ((x + 1 < cChunkDef::Width) && cBlockInfo::FullyOccupiesVoxel(a_ChunkDesc.GetBlockType(x + 1, y, z)))
				{
					Places.push_back(8);
				}

				if ((x - 1 > 0) && cBlockInfo::FullyOccupiesVoxel(a_ChunkDesc.GetBlockType(x - 1, y, z)))
				{
					Places.push_back(2);
				}

				if ((z + 1 < cChunkDef::Width) && cBlockInfo::FullyOccupiesVoxel(a_ChunkDesc.GetBlockType(x, y, z + 1)))
				{
					Places.push_back(1);
				}

				if ((z - 1 > 0) && cBlockInfo::FullyOccupiesVoxel(a_ChunkDesc.GetBlockType(x, y, z - 1)))
				{
					Places.push_back(4);
				}

				if (Places.size() == 0)
				{
					continue;
				}

				NIBBLETYPE Meta = Places[static_cast<size_t>(m_Noise.IntNoise3DInt(xx, y, zz)) % Places.size()];
				a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_VINES, Meta);
			}
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenSoulsandRims

void cFinishGenSoulsandRims::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int ChunkZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	HEIGHTTYPE MaxHeight = a_ChunkDesc.GetMaxHeight();

	for (int x = 0; x < 16; x++)
	{
		int xx = ChunkX + x;
		for (int z = 0; z < 16; z++)
		{
			int zz = ChunkZ + z;

			// Place soulsand rims when netherrack gets thin
			for (int y = 2; y < MaxHeight - 2; y++)
			{
				// The current block is air. Let's bail ut.
				BLOCKTYPE Block = a_ChunkDesc.GetBlockType(x, y, z);
				if (Block != E_BLOCK_NETHERRACK)
				{
					continue;
				}

				if (
					((a_ChunkDesc.GetBlockType(x, y + 1, z) != E_BLOCK_AIR) &&
					( a_ChunkDesc.GetBlockType(x, y + 2, z) != E_BLOCK_AIR)) ||
					((a_ChunkDesc.GetBlockType(x, y - 1, z) != E_BLOCK_AIR) &&
					( a_ChunkDesc.GetBlockType(x, y - 2, z) != E_BLOCK_AIR))
				)
				{
					continue;
				}

				NOISE_DATATYPE NoiseX = (static_cast<NOISE_DATATYPE>(xx)) / 32;
				NOISE_DATATYPE NoiseY = (static_cast<NOISE_DATATYPE>(zz)) / 32;
				NOISE_DATATYPE CompBlock = m_Noise.CubicNoise3D(NoiseX, static_cast<float>(y) / 4, NoiseY);
				if (CompBlock < 0)
				{
					a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_SOULSAND);
				}
			}
		}
	}
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
			HEIGHTTYPE Height = a_ChunkDesc.GetHeight(x, z);
			if (GetSnowStartHeight(a_ChunkDesc.GetBiome(x, z)) > Height)
			{
				// Height isn't high enough for snow to start forming.
				continue;
			}

			if (!cBlockInfo::IsSnowable(a_ChunkDesc.GetBlockType(x, Height, z)) || (Height >= cChunkDef::Height - 1))
			{
				// The top block can't be snown over.
				continue;
			}

			a_ChunkDesc.SetBlockType(x, Height + 1, z, E_BLOCK_SNOW);
			a_ChunkDesc.SetHeight(x, z, Height + 1);
		}  // for x
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
			int Height = a_ChunkDesc.GetHeight(x, z);
			if (GetSnowStartHeight(a_ChunkDesc.GetBiome(x, z)) > Height)
			{
				// Height isn't high enough for snow to start forming.
				continue;
			}

			if (!IsBlockWater(a_ChunkDesc.GetBlockType(x, Height, z)))
			{
				// The block isn't a water block.
				continue;
			}

			if (a_ChunkDesc.GetBlockMeta(x, Height, z) != 0)
			{
				// The water block isn't a source block.
				continue;
			}

			a_ChunkDesc.SetBlockType(x, Height, z, E_BLOCK_ICE);
		}  // for x
	}  // for z
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
