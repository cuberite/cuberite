
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FireSimulator.h"
#include "../World.h"
#include "../Defines.h"
#include "../Chunk.h"
#include "../Root.h"
#include "../Bindings/PluginManager.h"





// Easy switch for turning on debugging logging:
#if 0
	#define FIRE_FLOG FLOGD
#else
	#define FIRE_FLOG(...)
#endif





#define MAX_CHANCE_REPLACE_FUEL 100000
#define MAX_CHANCE_FLAMMABILITY 100000
// The base chance that in a tick, rain will extinguish a fire block.
#define CHANCE_BASE_RAIN_EXTINGUISH 0.2
// The additional chance, multiplied by the meta of the fire block, that rain
// will extinguish a fire block in a tick.
#define CHANCE_AGE_M_RAIN_EXTINGUISH 0.03





#ifdef __clang__
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

static const Vector3i gCrossCoords[] =
{
	{ 1, 0,  0},
	{-1, 0,  0},
	{ 0, 0,  1},
	{ 0, 0, -1},
} ;





static const Vector3i gNeighborCoords[] =
{
	{ 1,  0,  0},
	{-1,  0,  0},
	{ 0,  1,  0},
	{ 0, -1,  0},
	{ 0,  0,  1},
	{ 0,  0, -1},
} ;

#ifdef __clang__
	#pragma clang diagnostic pop
#endif





////////////////////////////////////////////////////////////////////////////////
// cFireSimulator:

cFireSimulator::cFireSimulator(cWorld & a_World, cIniFile & a_IniFile) :
	cSimulator(a_World)
{
	// Read params from the ini file:
	m_BurnStepTimeFuel    = static_cast<unsigned>(a_IniFile.GetValueSetI("FireSimulator", "BurnStepTimeFuel",     500));
	m_BurnStepTimeNonfuel = static_cast<unsigned>(a_IniFile.GetValueSetI("FireSimulator", "BurnStepTimeNonfuel",  100));
	m_Flammability        = a_IniFile.GetValueSetI("FireSimulator", "Flammability",          50);
	m_ReplaceFuelChance   = a_IniFile.GetValueSetI("FireSimulator", "ReplaceFuelChance",  50000);
}





cFireSimulator::~cFireSimulator()
{
}





void cFireSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	cCoordWithIntList & Data = a_Chunk->GetFireSimulatorData();

	int NumMSecs = static_cast<int>(a_Dt.count());
	for (cCoordWithIntList::iterator itr = Data.begin(); itr != Data.end();)
	{
		Vector3i relPos(itr->x, itr->y, itr->z);
		auto absPos = a_Chunk->RelativeToAbsolute(relPos);
		auto blockType = a_Chunk->GetBlock(relPos);

		if (!IsAllowedBlock(blockType))
		{
			// The block is no longer eligible (not a fire block anymore; a player probably placed a block over the fire)
			FIRE_FLOG("FS: Removing block {0}", absPos);
			itr = Data.erase(itr);
			continue;
		}

		auto BurnsForever = ((relPos.y > 0) && DoesBurnForever(a_Chunk->GetBlock(relPos.addedY(-1))));
		auto BlockMeta = a_Chunk->GetMeta(relPos);

		auto Raining = std::any_of(std::begin(gCrossCoords), std::end(gCrossCoords),
			[this, absPos](Vector3i cc)
			{
				return (m_World.IsWeatherWetAtXYZ(absPos + cc));
			}
		);

		// Randomly burn out the fire if it is raining:
		if (!BurnsForever && Raining && GetRandomProvider().RandBool(CHANCE_BASE_RAIN_EXTINGUISH + (BlockMeta * CHANCE_AGE_M_RAIN_EXTINGUISH)))
		{
			a_Chunk->SetBlock(relPos, E_BLOCK_AIR, 0);
			itr = Data.erase(itr);
			continue;
		}

		// Try to spread the fire:
		TrySpreadFire(a_Chunk, relPos);

		itr->Data -= NumMSecs;
		if (itr->Data >= 0)
		{
			// Not yet, wait for it longer
			++itr;
			continue;
		}

		// FIRE_FLOG("FS: Fire at {0} is stepping", absPos);

		// Has the fire burnt out?
		if (BlockMeta == 0x0f)
		{
			// The fire burnt out completely
			FIRE_FLOG("FS: Fire at {0} burnt out, removing the fire block", absPos);
			a_Chunk->SetBlock(relPos, E_BLOCK_AIR, 0);
			RemoveFuelNeighbors(a_Chunk, relPos);
			itr = Data.erase(itr);
			continue;
		}

		// Burn out the fire one step by increasing the meta:
		if (!BurnsForever)
		{
			a_Chunk->SetMeta(relPos, BlockMeta + 1);
		}

		itr->Data = GetBurnStepTime(a_Chunk, relPos);  // TODO: Add some randomness into this
		++itr;
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
		case E_BLOCK_DOUBLE_WOODEN_SLAB:
		case E_BLOCK_WOODEN_SLAB:
		case E_BLOCK_OAK_WOOD_STAIRS:
		case E_BLOCK_SPRUCE_WOOD_STAIRS:
		case E_BLOCK_BIRCH_WOOD_STAIRS:
		case E_BLOCK_JUNGLE_WOOD_STAIRS:
		case E_BLOCK_LEAVES:
		case E_BLOCK_NEW_LEAVES:
		case E_BLOCK_LOG:
		case E_BLOCK_NEW_LOG:
		case E_BLOCK_WOOL:
		case E_BLOCK_BOOKCASE:
		case E_BLOCK_FENCE:
		case E_BLOCK_SPRUCE_FENCE:
		case E_BLOCK_BIRCH_FENCE:
		case E_BLOCK_JUNGLE_FENCE:
		case E_BLOCK_DARK_OAK_FENCE:
		case E_BLOCK_ACACIA_FENCE:
		case E_BLOCK_OAK_FENCE_GATE:
		case E_BLOCK_SPRUCE_FENCE_GATE:
		case E_BLOCK_BIRCH_FENCE_GATE:
		case E_BLOCK_JUNGLE_FENCE_GATE:
		case E_BLOCK_DARK_OAK_FENCE_GATE:
		case E_BLOCK_ACACIA_FENCE_GATE:
		case E_BLOCK_TNT:
		case E_BLOCK_VINES:
		case E_BLOCK_HAY_BALE:
		case E_BLOCK_TALL_GRASS:
		case E_BLOCK_BIG_FLOWER:
		case E_BLOCK_DANDELION:
		case E_BLOCK_FLOWER:
		case E_BLOCK_CARPET:
		{
			return true;
		}
	}
	return false;
}





bool cFireSimulator::DoesBurnForever(BLOCKTYPE a_BlockType)
{
	return (a_BlockType == E_BLOCK_NETHERRACK);
}





void cFireSimulator::AddBlock(Vector3i a_Block, cChunk * a_Chunk)
{
	if ((a_Chunk == nullptr) || !a_Chunk->IsValid())
	{
		return;
	}

	int RelX = a_Block.x - a_Chunk->GetPosX() * cChunkDef::Width;
	int RelZ = a_Block.z - a_Chunk->GetPosZ() * cChunkDef::Width;
	BLOCKTYPE BlockType = a_Chunk->GetBlock(RelX, a_Block.y, RelZ);
	if (!IsAllowedBlock(BlockType))
	{
		return;
	}

	// Check for duplicates:
	cFireSimulatorChunkData & ChunkData = a_Chunk->GetFireSimulatorData();
	for (cCoordWithIntList::iterator itr = ChunkData.begin(), end = ChunkData.end(); itr != end; ++itr)
	{
		if ((itr->x == RelX) && (itr->y == a_Block.y) && (itr->z == RelZ))
		{
			// Already present, skip adding
			return;
		}
	}  // for itr - ChunkData[]

	FIRE_FLOG("FS: Adding block {0}", a_Block);
	ChunkData.push_back(cCoordWithInt(RelX, a_Block.y, RelZ, 100));
}





int cFireSimulator::GetBurnStepTime(cChunk * a_Chunk, Vector3i a_RelPos)
{
	bool IsBlockBelowSolid = false;
	if (a_RelPos.y > 0)
	{
		BLOCKTYPE BlockBelow = a_Chunk->GetBlock(a_RelPos.addedY(-1));
		if (DoesBurnForever(BlockBelow))
		{
			// Is burning atop of netherrack, burn forever (re-check in 10 sec)
			return 10000;
		}
		if (IsFuel(BlockBelow))
		{
			return static_cast<int>(m_BurnStepTimeFuel);
		}
		IsBlockBelowSolid = cBlockInfo::IsSolid(BlockBelow);
	}

	for (const auto & cross: gCrossCoords)
	{
		BLOCKTYPE  BlockType;
		NIBBLETYPE BlockMeta;
		if (a_Chunk->UnboundedRelGetBlock(a_RelPos + cross, BlockType, BlockMeta))
		{
			if (IsFuel(BlockType))
			{
				return static_cast<int>(m_BurnStepTimeFuel);
			}
		}
	}  // for i - gCrossCoords[]

	if (!IsBlockBelowSolid)
	{
		// Checked through everything, nothing was flammable
		// If block below isn't solid, we can't have fire, it would be a non-fueled fire
		// SetBlock just to make sure fire doesn't spawn
		a_Chunk->SetBlock(a_RelPos, E_BLOCK_AIR, 0);
		return 0;
	}
	return static_cast<int>(m_BurnStepTimeNonfuel);
}





void cFireSimulator::TrySpreadFire(cChunk * a_Chunk, Vector3i a_RelPos)
{
	/*
	if (GetRandomProvider().RandBool(0.99))
	{
		// Make the chance to spread 100x smaller
		return;
	}
	*/

	for (int x = -1; x <= 1; x++)
	{
		for (int z = -1; z <= 1; z++)
		{
			for (int y = 1; y <= 2; y++)  // flames spread up one more block than around
			{
				// No need to check the coords for equality with the parent block,
				// it cannot catch fire anyway (because it's not an air block)

				if (!GetRandomProvider().RandBool(m_Flammability * (1.0 / MAX_CHANCE_FLAMMABILITY)))
				{
					continue;
				}

				// Start the fire in the neighbor a_RelPos + {x, y, z}
				auto dstRelPos = a_RelPos + Vector3i{x, y, z};
				if (CanStartFireInBlock(a_Chunk, dstRelPos))
				{
					auto dstAbsPos = a_Chunk->RelativeToAbsolute(dstRelPos);
					if (cRoot::Get()->GetPluginManager()->CallHookBlockSpread(m_World, dstAbsPos.x, dstAbsPos.y, dstAbsPos.z, ssFireSpread))
					{
						return;
					}

					FIRE_FLOG("FS: Starting new fire at {0}.", dstAbsPos);
					a_Chunk->UnboundedRelSetBlock(dstRelPos, E_BLOCK_FIRE, 0);
				}
			}  // for y
		}  // for z
	}  // for x
}





void cFireSimulator::RemoveFuelNeighbors(cChunk * a_Chunk, Vector3i a_RelPos)
{
	for (auto & coord : gNeighborCoords)
	{
		BLOCKTYPE  BlockType;
		auto relPos = a_RelPos + coord;
		auto neighbor = a_Chunk->GetRelNeighborChunkAdjustCoords(relPos);
		if (neighbor == nullptr)
		{
			continue;
		}
		BlockType = neighbor->GetBlock(relPos);

		if (!IsFuel(BlockType))
		{
			continue;
		}

		auto absPos = neighbor->RelativeToAbsolute(relPos);
		if (BlockType == E_BLOCK_TNT)
		{
			neighbor->SetBlock(relPos, E_BLOCK_AIR, 0);
			m_World.SpawnPrimedTNT(absPos, 0);
			return;
		}

		bool ShouldReplaceFuel = (GetRandomProvider().RandBool(m_ReplaceFuelChance * (1.0 / MAX_CHANCE_REPLACE_FUEL)));
		if (ShouldReplaceFuel && !cRoot::Get()->GetPluginManager()->CallHookBlockSpread(m_World, absPos.x, absPos.y, absPos.z, ssFireSpread))
		{
			neighbor->SetBlock(relPos, E_BLOCK_FIRE, 0);
		}
		else
		{
			neighbor->SetBlock(relPos, E_BLOCK_AIR, 0);
		}
	}  // for i - Coords[]
}





bool cFireSimulator::CanStartFireInBlock(cChunk * a_NearChunk, Vector3i a_RelPos)
{
	BLOCKTYPE  BlockType;
	NIBBLETYPE BlockMeta;
	if (!a_NearChunk->UnboundedRelGetBlock(a_RelPos, BlockType, BlockMeta))
	{
		// The chunk is not accessible
		return false;
	}

	if (BlockType != E_BLOCK_AIR)
	{
		// Only an air block can be replaced by a fire block
		return false;
	}

	for (const auto & neighbor: gNeighborCoords)
	{
		if (!a_NearChunk->UnboundedRelGetBlock(a_RelPos + neighbor, BlockType, BlockMeta))
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
