// FinishGenNetherClumpFoliage.cpp

#include "FinishGenNetherClumpFoliage.h"

#include "../../Simulator/FireSimulator.h"
#include "../../BlockInfo.h"

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
		NOISE_DATATYPE BlockType = m_Noise.IntNoise3D(static_cast<int>(ChunkX), y, static_cast<int>(ChunkZ));
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
