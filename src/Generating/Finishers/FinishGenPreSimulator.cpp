
// FinishGenPreSimulator.cpp

#include "FinishGenPreSimulator.h"
#include "../../Simulator/FluidSimulator.h"  // for cFluidSimulator::CanWashAway()



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
		CollapseSandGravel(a_ChunkDesc);
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





void cFinishGenPreSimulator::CollapseSandGravel(cChunkDesc & a_ChunkDesc)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int LastY = -1;
			int HeightY = 0;
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				BLOCKTYPE Block = a_ChunkDesc.GetBlockType(x, y, z);
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
							auto BlockMeta = a_ChunkDesc.GetBlockMeta(x, y, z);
							a_ChunkDesc.SetBlockTypeMeta(x, LastY + 1, z, Block, BlockMeta);
							a_ChunkDesc.SetBlockTypeMeta(x, y, z, E_BLOCK_AIR, 0);
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
			a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(HeightY));
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
