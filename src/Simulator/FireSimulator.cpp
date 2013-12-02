
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FireSimulator.h"
#include "../World.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../Chunk.h"





// Easy switch for turning on debugging logging:
#if 0
	#define FLOG LOGD
#else
	#define FLOG(...)
#endif





#define MAX_CHANCE_REPLACE_FUEL 100000
#define MAX_CHANCE_FLAMMABILITY 100000





static const struct
{
	int x, y, z;
} gCrossCoords[] =
{
	{ 1, 0,  0},
	{-1, 0,  0},
	{ 0, 0,  1},
	{ 0, 0, -1},
} ;





static const struct
{
	int x, y, z;
} gNeighborCoords[] =
{
	{ 1,  0,  0},
	{-1,  0,  0},
	{ 0,  1,  0},
	{ 0, -1,  0},
	{ 0,  0,  1},
	{ 0,  0, -1},
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cFireSimulator:

cFireSimulator::cFireSimulator(cWorld & a_World, cIniFile & a_IniFile) :
	cSimulator(a_World)
{
	// Read params from the ini file:
	m_BurnStepTimeFuel    = a_IniFile.GetValueSetI("FireSimulator", "BurnStepTimeFuel",     500);
	m_BurnStepTimeNonfuel = a_IniFile.GetValueSetI("FireSimulator", "BurnStepTimeNonfuel",  100);
	m_Flammability        = a_IniFile.GetValueSetI("FireSimulator", "Flammability",          50);
	m_ReplaceFuelChance   = a_IniFile.GetValueSetI("FireSimulator", "ReplaceFuelChance",  50000);
}





cFireSimulator::~cFireSimulator()
{
}





void cFireSimulator::SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	cCoordWithIntList & Data = a_Chunk->GetFireSimulatorData();

	int NumMSecs = (int)a_Dt;
	for (cCoordWithIntList::iterator itr = Data.begin(); itr != Data.end();)
	{
		int idx = cChunkDef::MakeIndexNoCheck(itr->x, itr->y, itr->z);
		int idb = cChunkDef::MakeIndexNoCheck(itr->x, itr->y - 1, itr->z);
		BLOCKTYPE BlockType = a_Chunk->GetBlock(idx);
		BLOCKTYPE Burnee = a_Chunk->GetBlock(idb);

		if (!IsAllowedBlock(BlockType))
		{
			// The block is no longer eligible (not a fire block anymore; a player probably placed a block over the fire)
			FLOG("FS: Removing block {%d, %d, %d}",
				itr->x + a_ChunkX * cChunkDef::Width, itr->y, itr->z + a_ChunkZ * cChunkDef::Width
			);
			itr = Data.erase(itr);
			continue;
		}

		// Try to spread the fire:
		TrySpreadFire(a_Chunk, itr->x, itr->y, itr->z);

		itr->Data -= NumMSecs;
		if (itr->Data >= 0)
		{
			// Not yet, wait for it longer
			++itr;
			continue;
		}
		
		// Burn out the fire one step by increasing the meta:
		/*
		FLOG("FS: Fire at {%d, %d, %d} is stepping",
			itr->x + a_ChunkX * cChunkDef::Width, itr->y, itr->z + a_ChunkZ * cChunkDef::Width
		);
		*/
		NIBBLETYPE BlockMeta = a_Chunk->GetMeta(idx);
		if (BlockMeta == 0x0f)
		{
			// The fire burnt out completely
			FLOG("FS: Fire at {%d, %d, %d} burnt out, removing the fire block",
				itr->x + a_ChunkX * cChunkDef::Width, itr->y, itr->z + a_ChunkZ * cChunkDef::Width
			);
			a_Chunk->SetBlock(itr->x, itr->y, itr->z, E_BLOCK_AIR, 0);
			RemoveFuelNeighbors(a_Chunk, itr->x, itr->y, itr->z);
			itr = Data.erase(itr);
			continue;
		}
		if(Burnee != E_BLOCK_NETHERRACK)
		{
			a_Chunk->SetMeta(idx, BlockMeta + 1);
		}
		itr->Data = GetBurnStepTime(a_Chunk, itr->x, itr->y, itr->z);  // TODO: Add some randomness into this
	}  // for itr - Data[]
}





bool cFireSimulator::IsAllowedBlock(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_FIRE);
}





bool cFireSimulator::IsFuel(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_PLANKS:
		case E_BLOCK_LEAVES:
		case E_BLOCK_LOG:
		case E_BLOCK_WOOL:
		case E_BLOCK_BOOKCASE:
		case E_BLOCK_FENCE:
		case E_BLOCK_TNT:
		case E_BLOCK_VINES:
		{
			return true;
		}
	}
	return false;
}





bool cFireSimulator::IsForever(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_NETHERRACK);
}





void cFireSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if ((a_Chunk == NULL) || !a_Chunk->IsValid())
	{
		return;
	}
	
	int RelX = a_BlockX - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_BlockZ - a_Chunk->GetPosZ() * cChunkDef::Width;
	BLOCKTYPE BlockType = a_Chunk->GetBlock(RelX, a_BlockY, RelZ);
	if (!IsAllowedBlock(BlockType))
	{
		return;
	}
	
	// Check for duplicates:
	cFireSimulatorChunkData & ChunkData = a_Chunk->GetFireSimulatorData();
	for (cCoordWithIntList::iterator itr = ChunkData.begin(), end = ChunkData.end(); itr != end; ++itr)
	{
		if ((itr->x == RelX) && (itr->y == a_BlockY) && (itr->z == RelZ))
		{
			// Already present, skip adding
			return;
		}
	}  // for itr - ChunkData[]

	FLOG("FS: Adding block {%d, %d, %d}", a_BlockX, a_BlockY, a_BlockZ);
	ChunkData.push_back(cCoordWithInt(RelX, a_BlockY, RelZ, 100));
}





int cFireSimulator::GetBurnStepTime(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
{
	bool IsBlockBelowSolid = false;
	if (a_RelY > 0)
	{
		BLOCKTYPE BlockBelow = a_Chunk->GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		if (IsForever(BlockBelow))
		{
			// Is burning atop of netherrack, burn forever (re-check in 10 sec)
			return 10000;
		}
		if (IsFuel(BlockBelow))
		{
			return m_BurnStepTimeFuel;
		}
		IsBlockBelowSolid = g_BlockIsSolid[BlockBelow];
	}
	
	for (int i = 0; i < ARRAYCOUNT(gCrossCoords); i++)
	{
		BLOCKTYPE  BlockType;
		NIBBLETYPE BlockMeta;
		if (a_Chunk->UnboundedRelGetBlock(a_RelX + gCrossCoords[i].x, a_RelY, a_RelZ + gCrossCoords[i].z, BlockType, BlockMeta))
		{
			if (IsFuel(BlockType))
			{
				return m_BurnStepTimeFuel;
			}
		}
	}  // for i - gCrossCoords[]

	if (!IsBlockBelowSolid && (a_RelY >= 0))
	{
		// Checked through everything, nothing was flammable
		// If block below isn't solid, we can't have fire, it would be a non-fueled fire
		// SetBlock just to make sure fire doesn't spawn
		a_Chunk->SetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_AIR, 0);
		return 0;
	}
	return m_BurnStepTimeNonfuel;
}





void cFireSimulator::TrySpreadFire(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
{
	/*
	if (m_World.GetTickRandomNumber(10000) > 100)
	{
		// Make the chance to spread 100x smaller
		return;
	}
	*/
	
	for (int x = a_RelX - 1; x <= a_RelX + 1; x++)
	{
		for (int z = a_RelZ - 1; z <= a_RelZ + 1; z++)
		{
			for (int y = a_RelY - 1; y <= a_RelY + 2; y++)  // flames spread up one more block than around
			{
				// No need to check the coords for equality with the parent block,
				// it cannot catch fire anyway (because it's not an air block)
				
				if (m_World.GetTickRandomNumber(MAX_CHANCE_FLAMMABILITY) > m_Flammability) 
				{
					continue;
				}
				
				// Start the fire in the neighbor {x, y, z}
				/*
				FLOG("FS: Trying to start fire at {%d, %d, %d}.", 
					x + a_Chunk->GetPosX() * cChunkDef::Width, y, z + a_Chunk->GetPosZ() * cChunkDef::Width
				);
				*/
				if (CanStartFireInBlock(a_Chunk, x, y, z))
				{
					FLOG("FS: Starting new fire at {%d, %d, %d}.", 
						x + a_Chunk->GetPosX() * cChunkDef::Width, y, z + a_Chunk->GetPosZ() * cChunkDef::Width
					);
					a_Chunk->UnboundedRelSetBlock(x, y, z, E_BLOCK_FIRE, 0);
				}
			}  // for y
		}  // for z
	}  // for x
}





void cFireSimulator::RemoveFuelNeighbors(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
{
	for (int i = 0; i < ARRAYCOUNT(gNeighborCoords); i++)
	{
		BLOCKTYPE  BlockType;
		NIBBLETYPE BlockMeta;
		if (!a_Chunk->UnboundedRelGetBlock(a_RelX + gNeighborCoords[i].x, a_RelY + gNeighborCoords[i].y, a_RelZ + gNeighborCoords[i].z, BlockType, BlockMeta))
		{
			// Neighbor not accessible, ignore it
			continue;
		}
		if (!IsFuel(BlockType))
		{
			continue;
		}
		bool ShouldReplaceFuel = (m_World.GetTickRandomNumber(MAX_CHANCE_REPLACE_FUEL) < m_ReplaceFuelChance);
		a_Chunk->UnboundedRelSetBlock(
			a_RelX + gNeighborCoords[i].x, a_RelY + gNeighborCoords[i].y, a_RelZ + gNeighborCoords[i].z,
			ShouldReplaceFuel ? E_BLOCK_FIRE : E_BLOCK_AIR, 0
		);
	}  // for i - Coords[]
}





bool cFireSimulator::CanStartFireInBlock(cChunk * a_NearChunk, int a_RelX, int a_RelY, int a_RelZ)
{
	BLOCKTYPE  BlockType;
	NIBBLETYPE BlockMeta;
	if (!a_NearChunk->UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ, BlockType, BlockMeta))
	{
		// The chunk is not accessible
		return false;
	}
	
	if (BlockType != E_BLOCK_AIR)
	{
		// Only an air block can be replaced by a fire block
		return false;
	}
	
	for (int i = 0; i < ARRAYCOUNT(gNeighborCoords); i++)
	{
		if (!a_NearChunk->UnboundedRelGetBlock(a_RelX + gNeighborCoords[i].x, a_RelY + gNeighborCoords[i].y, a_RelZ + gNeighborCoords[i].z, BlockType, BlockMeta))
		{
			// Neighbor inaccessible, skip it while evaluating
			continue;
		}
		if (IsFuel(BlockType))
		{
			return true;
		}
	}  // for i - Coords[]
	return false;
}




