
// FinishGen.cpp

/* Implements the various finishing generators:
	- cFinishGenSnow
	- cFinishGenIce
	- cFinishGenSprinkleFoliage
*/

#include "Globals.h"

#include "FinishGen.h"
#include "cNoise.h"
#include "BlockID.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFinishGenSprinkleFoliage:

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
					break;
				}  // case E_BLOCK_GRASS
				
				case E_BLOCK_SAND:
				{
					if (val1 + val2 > 0.f)
					{
						cChunkDef::SetBlock(a_BlockTypes, x, ++Top, z, E_BLOCK_CACTUS);
						if (val1 > val2)
						{
							cChunkDef::SetBlock(a_BlockTypes, x, ++Top, z, E_BLOCK_CACTUS);
						}
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
	// TODO: Add snow block in snowy biomes onto blocks that can be snowed over
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
	// TODO: Turn surface water into ice in icy biomes
}




