
// FinishGenSingleTopBlock.cpp

#include "FinishGenSingleTopBlock.h"

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

		HEIGHTTYPE Height = a_ChunkDesc.GetHeight(x, z);
		if (Height >= cChunkDef::Height - 1)
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
