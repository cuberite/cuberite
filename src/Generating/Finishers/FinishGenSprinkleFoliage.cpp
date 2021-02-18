
// FinishGenSprinkleFoliage.cpp

#include "Globals.h"

#include "FinishGenSprinkleFoliage.h"

static inline bool IsWater(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_STATIONARY_WATER) || (a_BlockType == E_BLOCK_WATER);
}





bool cFinishGenSprinkleFoliage::TryAddCactus(cChunkDesc & a_ChunkDesc, int a_RelX, HEIGHTTYPE & a_RelY, int a_RelZ)
{
	if (!IsDesertVariant(a_ChunkDesc.GetBiome(a_RelX, a_RelZ)))
	{
		return false;
	}

	int CactusHeight = 1 + (m_Noise.IntNoise2DInt(a_RelX, a_RelZ) % m_MaxCactusHeight);

	// We'll be doing comparison with blocks above, so the coords should be 1 block away from chunk top
	if (a_RelY + CactusHeight >= cChunkDef::Height - 1)
	{
		CactusHeight = cChunkDef::Height - a_RelY - 1;
	}

	// We'll be doing comparison to neighbors, so require the coords to be 1 block away from the chunk edges:
	if (
		(a_RelX < 1) || (a_RelX >= cChunkDef::Width - 1) ||
		(a_RelZ < 1) || (a_RelZ >= cChunkDef::Width - 1)
		)
	{
		return false;
	}

	for (int i = 0; i < CactusHeight; i++)
	{
		const bool cactusExists = i != 0;

		const int y = a_RelY + 1;
		if (
			cBlockInfo::IsSolid(a_ChunkDesc.GetBlockType(a_RelX + 1, y, a_RelZ)) 	 ||
			cBlockInfo::IsSolid(a_ChunkDesc.GetBlockType(a_RelX - 1, y, a_RelZ)) 	 ||
			cBlockInfo::IsSolid(a_ChunkDesc.GetBlockType(a_RelX, 	 y, a_RelZ + 1)) ||
			cBlockInfo::IsSolid(a_ChunkDesc.GetBlockType(a_RelX, 	 y, a_RelZ - 1))
			)
		{
			return cactusExists;
		}

		// All conditions are met, we can place a cactus here
		a_ChunkDesc.SetBlockType(a_RelX, ++a_RelY, a_RelZ, E_BLOCK_CACTUS);
	}

	return true;
}





bool cFinishGenSprinkleFoliage::TryAddSugarcane(cChunkDesc & a_ChunkDesc, int a_RelX, HEIGHTTYPE & a_RelY, int a_RelZ)
{
	int SugarcaneHeight = 1 + (m_Noise.IntNoise2DInt(a_RelX, a_RelZ) % m_MaxSugarcaneHeight);

	// Only allow dirt, grass, sand and sugarcane below sugarcane:
	switch (a_ChunkDesc.GetBlockType(a_RelX, a_RelY, a_RelZ))
	{
		case E_BLOCK_DIRT:
		case E_BLOCK_GRASS:
		case E_BLOCK_SAND:
		case E_BLOCK_SUGARCANE:
		{
			break;
		}
		default:
		{
			return false;
		}
	}

	// We'll be doing comparison with blocks above, so the coords should be 1 block away from chunk top
	if (a_RelY + SugarcaneHeight >= cChunkDef::Height - 1)
	{
		SugarcaneHeight = cChunkDef::Height - a_RelY - 1;
	}

	// We'll be doing comparison to neighbors, so require the coords to be 1 block away from the chunk edges:
	if (
		(a_RelX < 1) || (a_RelX >= cChunkDef::Width  - 1) ||
		(a_RelZ < 1) || (a_RelZ >= cChunkDef::Width  - 1)
		)
	{
		return false;
	}

	// Water is required next to the block below the sugarcane (if the block below isn't sugarcane already)
	if (
		!IsWater(a_ChunkDesc.GetBlockType(a_RelX - 1, a_RelY, a_RelZ)) &&
		!IsWater(a_ChunkDesc.GetBlockType(a_RelX + 1, a_RelY, a_RelZ)) &&
		!IsWater(a_ChunkDesc.GetBlockType(a_RelX,     a_RelY, a_RelZ - 1)) &&
		!IsWater(a_ChunkDesc.GetBlockType(a_RelX,     a_RelY, a_RelZ + 1)) &&
		a_ChunkDesc.GetBlockType(a_RelX, a_RelY, a_RelZ) != E_BLOCK_SUGARCANE
		)
	{
		return false;
	}

	for (int i = 0; i < SugarcaneHeight; i++)
	{
		// All conditions met, place a sugarcane here
		a_ChunkDesc.SetBlockType(a_RelX, ++a_RelY, a_RelZ, E_BLOCK_SUGARCANE);
	}

	return true;
}





void cFinishGenSprinkleFoliage::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// Generate small foliage (1-block):

	// TODO: Update heightmap with 1-block-tall foliage
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z;
		const float zz = static_cast<float>(BlockZ);
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width + x;
			if (((m_Noise.IntNoise2DInt(BlockX, BlockZ) / 8) % 128) < 124)
			{
				continue;
			}
			HEIGHTTYPE Top = a_ChunkDesc.GetHeight(x, z);
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

			const float xx = static_cast<float>(BlockX);
			float val1 = m_Noise.CubicNoise2D(xx * 0.1f,  zz * 0.1f);
			float val2 = m_Noise.CubicNoise2D(xx * 0.01f, zz * 0.01f);
			switch (a_ChunkDesc.GetBlockType(x, Top, z))
			{
				case E_BLOCK_GRASS:
				{
					if (TryAddSugarcane(a_ChunkDesc, x, Top, z))
					{
						// Checks and block placing are handled in the TryAddSugarcane method
					}
					else if ((val1 > 0.5) && (val2 < -0.5))
					{
						float val3 = m_Noise.CubicNoise2D(xx * 0.01f + 10, zz * 0.01f + 10);
						a_ChunkDesc.SetBlockTypeMeta(x, ++Top, z, E_BLOCK_PUMPKIN, static_cast<int>(val3 * 8) % 4);
					}
					break;
				}  // case E_BLOCK_GRASS

				case E_BLOCK_SAND:
				{
					if (val1 + val2 > 0.5f)
					{
						if (!TryAddCactus(a_ChunkDesc, x, Top, z))
						{
							TryAddSugarcane(a_ChunkDesc, x, Top, z);
						}
					}
					else
					{
						TryAddSugarcane(a_ChunkDesc, x, Top, z);
					}
					break;
				}
			}  // switch (TopBlock)
			a_ChunkDesc.SetHeight(x, z, Top);
		}  // for x
	}  // for z
}





bool cFinishGenSprinkleFoliage::IsDesertVariant(EMCSBiome a_Biome)
{
	return
		(
			(a_Biome == biDesertHills) ||
			(a_Biome == biDesert) ||
			(a_Biome == biDesertM)
		);
}
