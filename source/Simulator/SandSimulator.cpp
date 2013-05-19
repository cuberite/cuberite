
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SandSimulator.h"
#include "../World.h"
#include "../BlockID.h"
#include "../Defines.h"
#include "../FallingBlock.h"
#include "../Chunk.h"





cSandSimulator::cSandSimulator(cWorld & a_World, cIniFile & a_IniFile) :
	cSimulator(a_World),
	m_TotalBlocks(0)
{
	m_IsInstantFall = a_IniFile.GetValueSetB("Physics", "SandInstantFall", false);
}





void cSandSimulator::SimulateChunk(float a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	cSandSimulatorChunkData & ChunkData = a_Chunk->GetSandSimulatorData();
	if (ChunkData.empty())
	{
		return;
	}

	int BaseX = a_Chunk->GetPosX() * cChunkDef::Width;
	int BaseZ = a_Chunk->GetPosZ() * cChunkDef::Width;
	for (cSandSimulatorChunkData::const_iterator itr = ChunkData.begin(), end = ChunkData.end(); itr != end; ++itr)
	{
		BLOCKTYPE BlockType = a_Chunk->GetBlock(itr->x, itr->y, itr->z);
		if (!IsAllowedBlock(BlockType) || (itr->y <= 0))
		{
			continue;
		}

		BLOCKTYPE BlockBelow = (itr->y > 0) ? a_Chunk->GetBlock(itr->x, itr->y - 1, itr->z) : E_BLOCK_AIR;
		if (CanStartFallingThrough(BlockBelow))
		{
			if (m_IsInstantFall)
			{
				DoInstantFall(a_Chunk, itr->x, itr->y, itr->z);
				continue;
			}
			Vector3i Pos;
			Pos.x = itr->x + BaseX;
			Pos.y = itr->y;
			Pos.z = itr->z + BaseZ;
			/*
			LOGD(
				"Creating a falling block at {%d, %d, %d} of type %s, block below: %s",
				Pos.x, Pos.y, Pos.z, ItemTypeToString(BlockType).c_str(), ItemTypeToString(BlockBelow).c_str()
			);
			*/
			cFallingBlock * FallingBlock = new cFallingBlock(Pos, BlockType, a_Chunk->GetMeta(itr->x, itr->y, itr->z));
			FallingBlock->Initialize(&m_World);
			a_Chunk->SetBlock(itr->x, itr->y, itr->z, E_BLOCK_AIR, 0);
		}
	}
	m_TotalBlocks -= ChunkData.size();
	ChunkData.clear();
}





bool cSandSimulator::IsAllowedBlock(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_SAND:
		case E_BLOCK_GRAVEL:
		case E_BLOCK_ANVIL:
		{
			return true;
		}
	}
	return false;
}





void cSandSimulator::AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk)
{
	if (a_Chunk == NULL)
	{
		return;
	}
	int RelX = a_BlockX - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_BlockZ - a_Chunk->GetPosZ() * cChunkDef::Width;
	if (!IsAllowedBlock(a_Chunk->GetBlock(RelX, a_BlockY, RelZ)))
	{
		return;
	}

	// Check for duplicates:
	cSandSimulatorChunkData & ChunkData = a_Chunk->GetSandSimulatorData();
	for (cSandSimulatorChunkData::iterator itr = ChunkData.begin(); itr != ChunkData.end(); ++itr)
	{
		if ((itr->x == RelX) && (itr->y == a_BlockY) && (itr->z == RelZ))
		{
			return;
		}
	}

	m_TotalBlocks += 1;
	ChunkData.push_back(cCoordWithInt(RelX, a_BlockY, RelZ));
}





bool cSandSimulator::CanStartFallingThrough(BLOCKTYPE a_BlockType)
{
	// Please keep the list alpha-sorted
	switch (a_BlockType)
	{
		case E_BLOCK_AIR:
		case E_BLOCK_FIRE:
		case E_BLOCK_LAVA:
		case E_BLOCK_SNOW:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_WATER:
		{
			return true;
		}
	}
	return false;
}





bool cSandSimulator::CanContinueFallThrough(BLOCKTYPE a_BlockType)
{
	// Please keep the list alpha-sorted
	switch (a_BlockType)
	{
		case E_BLOCK_AIR:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_COBWEB:
		case E_BLOCK_CROPS:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_FIRE:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_LAVA:
		case E_BLOCK_LEVER:
		case E_BLOCK_MINECART_TRACKS:
		case E_BLOCK_MELON_STEM:
		case E_BLOCK_POWERED_RAIL:
		case E_BLOCK_PUMPKIN_STEM:
		case E_BLOCK_REDSTONE_REPEATER_OFF:
		case E_BLOCK_REDSTONE_REPEATER_ON:
		case E_BLOCK_REDSTONE_TORCH_OFF:
		case E_BLOCK_REDSTONE_TORCH_ON:
		case E_BLOCK_REDSTONE_WIRE:
		case E_BLOCK_RED_MUSHROOM:
		case E_BLOCK_RED_ROSE:
		case E_BLOCK_SIGN_POST:
		case E_BLOCK_SNOW:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_TRIPWIRE:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_WALLSIGN:
		case E_BLOCK_WATER:
		case E_BLOCK_WOODEN_BUTTON:
		case E_BLOCK_WOODEN_PRESSURE_PLATE:
		case E_BLOCK_YELLOW_FLOWER:
		{
			return true;
		}
	}
	return false;
}





bool cSandSimulator::IsReplacedOnRematerialization(BLOCKTYPE a_BlockType)
{
	// Please keep the list alpha-sorted
	switch (a_BlockType)
	{
		case E_BLOCK_AIR:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_FIRE:
		case E_BLOCK_LAVA:
		case E_BLOCK_SNOW:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_WATER:
		{
			return true;
		}
	}
	return false;
}





bool cSandSimulator::DoesBreakFallingThrough(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	switch (a_BlockType)
	{
		case E_BLOCK_STONE_SLAB:
		case E_BLOCK_WOODEN_SLAB:
		{
			return ((a_BlockMeta & 0x08) == 0);  // Only a bottom-slab breaks the block
		}
	}
	return false;
}





void cSandSimulator::FinishFalling(
	cWorld * a_World, int a_BlockX, int a_BlockY, int a_BlockZ,
	BLOCKTYPE a_FallingBlockType, NIBBLETYPE a_FallingBlockMeta
)
{
	ASSERT(a_BlockY < cChunkDef::Height);
	
	BLOCKTYPE CurrentBlockType = a_World->GetBlock(a_BlockX, a_BlockY, a_BlockZ);
	if ((a_FallingBlockType == E_BLOCK_ANVIL) || IsReplacedOnRematerialization(CurrentBlockType))
	{
		// Rematerialize the material here:
		a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, a_FallingBlockType, a_FallingBlockMeta);
		return;
	}
	
	// Create a pickup instead:
	cItems Pickups;
	Pickups.Add((ENUM_ITEM_ID)a_FallingBlockType, 1, a_FallingBlockMeta);
	a_World->SpawnItemPickups(Pickups, (double)a_BlockX + 0.5, (double)a_BlockY + 0.5, (double)a_BlockZ + 0.5, 0);
}





void cSandSimulator::DoInstantFall(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
{
	// Remove the original block:
	BLOCKTYPE  FallingBlockType;
	NIBBLETYPE FallingBlockMeta;
	a_Chunk->GetBlockTypeMeta(a_RelX, a_RelY, a_RelZ, FallingBlockType, FallingBlockMeta);
	a_Chunk->SetBlock(a_RelX, a_RelY, a_RelZ, E_BLOCK_AIR, 0);
	
	// Search for a place to put it:
	for (int y = a_RelY - 1; y >= 0; y--)
	{
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		a_Chunk->GetBlockTypeMeta(a_RelX, y, a_RelZ, BlockType, BlockMeta);
		int BlockY;
		if (DoesBreakFallingThrough(BlockType, BlockMeta))
		{
			BlockY = y;
		}
		else if (!CanContinueFallThrough(BlockType))
		{
			BlockY = y + 1;
		}
		else
		{
			// Can fall further down
			continue;
		}
		
		// Finish the fall at the found bottom:
		int BlockX = a_RelX + a_Chunk->GetPosX() * cChunkDef::Width;
		int BlockZ = a_RelZ + a_Chunk->GetPosZ() * cChunkDef::Width;
		FinishFalling(&m_World, BlockX, BlockY, BlockZ, FallingBlockType, FallingBlockMeta);
		return;
	}
	
	// The block just "fell off the world" without leaving a trace
}




