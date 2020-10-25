
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "SandSimulator.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Defines.h"
#include "../Entities/FallingBlock.h"
#include "../Chunk.h"
#include "../IniFile.h"
#include "../EffectID.h"





cSandSimulator::cSandSimulator(cWorld & a_World, cIniFile & a_IniFile) :
	cSimulator(a_World),
	m_TotalBlocks(0)
{
	m_IsInstantFall = a_IniFile.GetValueSetB("Physics", "SandInstantFall", false);
}





void cSandSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
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
			FLOGD(
				"Creating a falling block at {0} of type {1}, block below: {2}",
				Pos, ItemTypeToString(BlockType), ItemTypeToString(BlockBelow)
			);
			*/

			m_World.SpawnFallingBlock(Pos, BlockType, a_Chunk->GetMeta(itr->x, itr->y, itr->z));
			a_Chunk->SetBlock({itr->x, itr->y, itr->z}, E_BLOCK_AIR, 0);
		}
	}
	m_TotalBlocks -= static_cast<int>(ChunkData.size());
	ChunkData.clear();
}





void cSandSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BLOCKTYPE a_Block)
{
	if (!IsAllowedBlock(a_Block))
	{
		return;
	}

	// Check for duplicates:
	cSandSimulatorChunkData & ChunkData = a_Chunk.GetSandSimulatorData();
	for (cSandSimulatorChunkData::iterator itr = ChunkData.begin(); itr != ChunkData.end(); ++itr)
	{
		if ((itr->x == a_Position.x) && (itr->y == a_Position.y) && (itr->z == a_Position.z))
		{
			return;
		}
	}

	m_TotalBlocks += 1;
	ChunkData.push_back(cCoordWithInt(a_Position.x, a_Position.y, a_Position.z));
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
		case E_BLOCK_BEETROOTS:
		case E_BLOCK_BROWN_MUSHROOM:
		case E_BLOCK_CARROTS:
		case E_BLOCK_COBWEB:
		case E_BLOCK_CROPS:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_DETECTOR_RAIL:
		case E_BLOCK_FIRE:
		case E_BLOCK_FLOWER_POT:
		case E_BLOCK_HEAVY_WEIGHTED_PRESSURE_PLATE:
		case E_BLOCK_IRON_TRAPDOOR:
		case E_BLOCK_LAVA:
		case E_BLOCK_LEVER:
		case E_BLOCK_LIGHT_WEIGHTED_PRESSURE_PLATE:
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
		case E_BLOCK_STANDING_BANNER:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_STONE_BUTTON:
		case E_BLOCK_STONE_PRESSURE_PLATE:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_TORCH:
		case E_BLOCK_TRAPDOOR:
		case E_BLOCK_TRIPWIRE:
		case E_BLOCK_TRIPWIRE_HOOK:
		case E_BLOCK_WALL_BANNER:
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
		case E_BLOCK_CHORUS_FLOWER:
		case E_BLOCK_CHORUS_PLANT:
		case E_BLOCK_DEAD_BUSH:
		case E_BLOCK_FIRE:
		case E_BLOCK_LAVA:
		case E_BLOCK_SNOW:
		case E_BLOCK_STATIONARY_LAVA:
		case E_BLOCK_STATIONARY_WATER:
		case E_BLOCK_STRUCTURE_VOID:
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
		case E_BLOCK_PURPUR_SLAB:
		case E_BLOCK_RED_SANDSTONE_SLAB:
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
		if (a_FallingBlockType == E_BLOCK_ANVIL)
		{
			a_World->BroadcastSoundParticleEffect(EffectID::SFX_RANDOM_ANVIL_LAND, {a_BlockX, a_BlockY, a_BlockZ}, 0);
		}
		return;
	}

	// Create a pickup instead:
	cItems Pickups;
	Pickups.Add(static_cast<ENUM_ITEM_TYPE>(a_FallingBlockType), 1, a_FallingBlockMeta);
	a_World->SpawnItemPickups(
		Pickups,
		static_cast<double>(a_BlockX) + 0.5,
		static_cast<double>(a_BlockY) + 0.5,
		static_cast<double>(a_BlockZ) + 0.5
	);
}





bool cSandSimulator::IsAllowedBlock(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_ANVIL:
		case E_BLOCK_CONCRETE_POWDER:
		case E_BLOCK_DRAGON_EGG:
		case E_BLOCK_GRAVEL:
		case E_BLOCK_SAND:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





void cSandSimulator::DoInstantFall(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
{
	// Remove the original block:
	BLOCKTYPE  FallingBlockType;
	NIBBLETYPE FallingBlockMeta;
	a_Chunk->GetBlockTypeMeta(a_RelX, a_RelY, a_RelZ, FallingBlockType, FallingBlockMeta);
	a_Chunk->SetBlock({a_RelX, a_RelY, a_RelZ}, E_BLOCK_AIR, 0);

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
		else if ((FallingBlockType == E_BLOCK_CONCRETE_POWDER) && IsBlockWater(BlockType))
		{
			FallingBlockType = E_BLOCK_CONCRETE;
			BlockY = y;
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




