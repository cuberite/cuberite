
// FinishGenTallGrass.cpp

#include "Globals.h"

#include "FinishGenTallGrass.h"

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

			if (y >= cChunkDef::Height - 1)
			{
				continue;
			}

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
				a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_TALL_GRASS, E_META_TALL_GRASS_GRASS);
			}
			else if ((GrassType < 90) && CanFernGrow(a_ChunkDesc.GetBiome(x, z)))
			{
				a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_TALL_GRASS, E_META_TALL_GRASS_FERN);
			}
			else if (!IsBiomeVeryCold(a_ChunkDesc.GetBiome(x, z)))
			{
				// If double long grass we have to choose what type we should use:
				if (a_ChunkDesc.GetBlockType(x, y + 1, z) == E_BLOCK_AIR)
				{
					NIBBLETYPE Meta = (m_Noise.IntNoise2DInt(xx * 100, zz * 100) / 7 % 100) > 25 ?
						E_META_BIG_FLOWER_DOUBLE_TALL_GRASS : E_META_BIG_FLOWER_LARGE_FERN;

					if ((Meta != E_META_BIG_FLOWER_LARGE_FERN) || CanLargeFernGrow(a_ChunkDesc.GetBiome(x, z)))
					{
						a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_BIG_FLOWER, Meta);
						a_ChunkDesc.SetBlockTypeMeta(x, y + 1, z, E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_TOP);
						a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(y + 1));
					}
				}
			}
			else
			{
				NIBBLETYPE meta = (m_Noise.IntNoise2DInt(xx * 50, zz * 50) / 7 % 2) + 1;
				a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_TALL_GRASS, meta);
				a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(y));
			}
		}
	}
}





bool cFinishGenTallGrass::CanFernGrow(EMCSBiome a_Biome)
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
			return CanLargeFernGrow(a_Biome);
		}
	}
}





bool cFinishGenTallGrass::CanLargeFernGrow(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biColdTaiga:
		case biColdTaigaHills:
		case biColdTaigaM:

		case biTaiga:
		case biTaigaHills:
		case biTaigaM:

		case biMegaSpruceTaiga:
		case biMegaSpruceTaigaHills:
		case biMegaTaiga:
		case biMegaTaigaHills:
		{
			return true;
		}

		default:
		{
			return false;
		}
	}
}





int cFinishGenTallGrass::GetBiomeDensity(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biSavanna:
		case biSavannaM:
		case biSavannaPlateau:
		case biSavannaPlateauM:
		case biPlains:
		{
			return 70;
		}

		case biExtremeHillsEdge:
		case biExtremeHillsPlus:
		case biExtremeHills:
		case biExtremeHillsPlusM:
		case biExtremeHillsM:
		case biIceMountains:
		{
			return 3;
		}

		default:
		{
			return 20;
		}
	}
}
