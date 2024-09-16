
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "FireSimulator.h"
#include "../BlockInfo.h"
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





void cFireSimulator::SimulateChunk(std::chrono::milliseconds a_Dt, int a_ChunkX, int a_ChunkZ, cChunk * a_Chunk)
{
	cCoordWithIntList & Data = a_Chunk->GetFireSimulatorData();

	int NumMSecs = static_cast<int>(a_Dt.count());
	for (cCoordWithIntList::iterator itr = Data.begin(); itr != Data.end();)
	{
		Vector3i RelPos(itr->x, itr->y, itr->z);
		auto Self = a_Chunk->GetBlock(RelPos);

		if (!IsAllowedBlock(Self))
		{
			// The block is no longer eligible (not a fire block anymore; a player probably placed a block over the fire)
			FIRE_FLOG("FS: Removing block {0}", AbsPos);
			itr = Data.erase(itr);
			continue;
		}

		auto BurnsForever = ((RelPos.y > 0) && DoesBurnForever(a_Chunk->GetBlock(RelPos.addedY(-1))));

		auto Raining = std::any_of(FlatCrossCoords.begin(), FlatCrossCoords.end(), [a_Chunk, RelPos](Vector3i Offset)
		{
			auto Adjusted = RelPos + Offset;
			const auto Chunk = a_Chunk->GetRelNeighborChunkAdjustCoords(Adjusted);
			if ((Chunk != nullptr) && Chunk->IsValid())
			{
				return Chunk->IsWeatherWetAt(Adjusted);
			}
			return false;
		});

		// Randomly burn out the fire if it is raining:
		if (!BurnsForever && Raining && GetRandomProvider().RandBool(CHANCE_BASE_RAIN_EXTINGUISH + (a_Chunk->GetBlock(RelPos.addedY(-1)).ID * CHANCE_AGE_M_RAIN_EXTINGUISH)))
		{
			a_Chunk->SetBlock(RelPos, Block::Air::Air());
			itr = Data.erase(itr);
			continue;
		}

		// Try to spread the fire:
		TrySpreadFire(a_Chunk, RelPos);

		itr->Data -= NumMSecs;
		if (itr->Data >= 0)
		{
			// Not yet, wait for it longer
			++itr;
			continue;
		}

		// FIRE_FLOG("FS: Fire at {0} is stepping", AbsPos);

		// TODO: Add some randomness into this
		const auto BurnStep = GetBurnStepTime(a_Chunk, RelPos);
		if (BurnStep == 0)
		{
			// Fire has no fuel or ground block, extinguish flame
			a_Chunk->SetBlock(RelPos, Block::Air::Air());
			itr = Data.erase(itr);
			continue;
		}

		// Has the fire burnt out?
		if (Block::Fire::Age(Self) == 15)
		{
			// The fire burnt out completely
			FIRE_FLOG("FS: Fire at {0} burnt out, removing the fire block", AbsPos);
			a_Chunk->SetBlock(RelPos, Block::Air::Air());
			RemoveFuelNeighbors(a_Chunk, RelPos);
			itr = Data.erase(itr);
			continue;
		}

		// Burn out the fire one step by increasing the meta:
		if (!BurnsForever)
		{
			using namespace Block;
			a_Chunk->SetBlock(RelPos, Fire::Fire
			(
				Fire::Age(Self) + 1,
				Fire::East(Self),
				Fire::North(Self),
				Fire::South(Self),
				Fire::Up(Self),
				Fire::West(Self)
			));
		}

		itr->Data = BurnStep;
		++itr;
	}  // for itr - Data[]
}





bool cFireSimulator::IsAllowedBlock(BlockState a_Block)
{
	return (a_Block.Type() == BlockType::Fire);
}





bool cFireSimulator::IsFuel(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Bookshelf:
		case BlockType::Tnt:
		case BlockType::Vine:
		case BlockType::HayBlock:
			// Carpets
		case BlockType::BlackCarpet:
		case BlockType::BlueCarpet:
		case BlockType::BrownCarpet:
		case BlockType::CyanCarpet:
		case BlockType::GrayCarpet:
		case BlockType::GreenCarpet:
		case BlockType::LightBlueCarpet:
		case BlockType::LightGrayCarpet:
		case BlockType::LimeCarpet:
		case BlockType::MagentaCarpet:
		case BlockType::OrangeCarpet:
		case BlockType::PinkCarpet:
		case BlockType::PurpleCarpet:
		case BlockType::RedCarpet:
		case BlockType::WhiteCarpet:
		case BlockType::YellowCarpet:
			// Fence
		case BlockType::AcaciaFence:
		case BlockType::BirchFence:
		case BlockType::DarkOakFence:
		case BlockType::JungleFence:
		case BlockType::NetherBrickFence:
		case BlockType::OakFence:
		case BlockType::SpruceFence:
		case BlockType::WarpedFence:
			// Fence Gate
		case BlockType::AcaciaFenceGate:
		case BlockType::BirchFenceGate:
		case BlockType::CrimsonFenceGate:
		case BlockType::DarkOakFenceGate:
		case BlockType::JungleFenceGate:
		case BlockType::OakFenceGate:
		case BlockType::SpruceFenceGate:
		case BlockType::WarpedFenceGate:
			// Flowers
		case BlockType::TallGrass:
		case BlockType::LargeFern:
		case BlockType::Lilac:
		case BlockType::Peony:
		case BlockType::RoseBush:
		case BlockType::Sunflower:
		case BlockType::Allium:
		case BlockType::AzureBluet:
		case BlockType::BlueOrchid:
		case BlockType::Cornflower:
		case BlockType::LilyOfTheValley:
		case BlockType::OrangeTulip:
		case BlockType::OxeyeDaisy:
		case BlockType::PinkTulip:
		case BlockType::Poppy:
		case BlockType::RedTulip:
		case BlockType::WhiteTulip:
			// Leaves
		case BlockType::AcaciaLeaves:
		case BlockType::BirchLeaves:
		case BlockType::DarkOakLeaves:
		case BlockType::JungleLeaves:
		case BlockType::OakLeaves:
		case BlockType::SpruceLeaves:
			// Log
		case BlockType::AcaciaLog:
		case BlockType::BirchLog:
		case BlockType::DarkOakLog:
		case BlockType::JungleLog:
		case BlockType::OakLog:
		case BlockType::SpruceLog:
		case BlockType::StrippedAcaciaLog:
		case BlockType::StrippedBirchLog:
		case BlockType::StrippedDarkOakLog:
		case BlockType::StrippedJungleLog:
		case BlockType::StrippedOakLog:
		case BlockType::StrippedSpruceLog:
			// Planks
		case BlockType::AcaciaPlanks:
		case BlockType::BirchPlanks:
		case BlockType::CrimsonPlanks:
		case BlockType::DarkOakPlanks:
		case BlockType::JunglePlanks:
		case BlockType::OakPlanks:
		case BlockType::SprucePlanks:
		case BlockType::WarpedPlanks:
			// Slabs
		case BlockType::AcaciaSlab:
		case BlockType::BirchSlab:
		case BlockType::BrickSlab:
		case BlockType::CrimsonSlab:
		case BlockType::DarkOakSlab:
		case BlockType::JungleSlab:
		case BlockType::OakSlab:
		case BlockType::PetrifiedOakSlab:
		case BlockType::SpruceSlab:
		case BlockType::WarpedSlab:
			// Stairs
		case BlockType::AcaciaStairs:
		case BlockType::BirchStairs:
		case BlockType::CrimsonStairs:
		case BlockType::DarkOakStairs:
		case BlockType::JungleStairs:
		case BlockType::OakStairs:
		case BlockType::SpruceStairs:
		case BlockType::WarpedStairs:
			// Wool
		case BlockType::BlackWool:
		case BlockType::BlueWool:
		case BlockType::BrownWool:
		case BlockType::CyanWool:
		case BlockType::GrayWool:
		case BlockType::GreenWool:
		case BlockType::LightBlueWool:
		case BlockType::LightGrayWool:
		case BlockType::LimeWool:
		case BlockType::MagentaWool:
		case BlockType::OrangeWool:
		case BlockType::PinkWool:
		case BlockType::PurpleWool:
		case BlockType::RedWool:
		case BlockType::WhiteWool:
		case BlockType::YellowWool:
		{
			return true;
		}
		default: return false;
	}
}





bool cFireSimulator::DoesBurnForever(BlockState a_Block)
{
	switch (a_Block.Type())
	{
		case BlockType::Netherrack:
			return true;
		default: return false;
	}
}





void cFireSimulator::AddBlock(cChunk & a_Chunk, Vector3i a_Position, BlockState a_Block)
{
	if (!IsAllowedBlock(a_Block))
	{
		return;
	}

	// Check for duplicates:
	cFireSimulatorChunkData & ChunkData = a_Chunk.GetFireSimulatorData();
	for (cCoordWithIntList::iterator itr = ChunkData.begin(), end = ChunkData.end(); itr != end; ++itr)
	{
		const Vector3i ItrPos{itr->x, itr->y, itr->z};
		if (ItrPos == a_Position)
		{
			// Block already present, check if burn step should decrease
			// This means if fuel is removed, then the fire burns out sooner
			const auto NewBurnStep = GetBurnStepTime(&a_Chunk, a_Position);
			if (itr->Data > NewBurnStep)
			{
				FIRE_FLOG("FS: Block lost its fuel at {0}", a_Block);
				itr->Data = NewBurnStep;
			}

			return;
		}
	}  // for itr - ChunkData[]

	FIRE_FLOG("FS: Adding block {0}", a_Block);
	ChunkData.emplace_back(a_Position.x, a_Position.y, a_Position.z, 100);
}





int cFireSimulator::GetBurnStepTime(cChunk * a_Chunk, Vector3i a_RelPos)
{
	bool IsBlockBelowSolid = false;
	if (a_RelPos.y > 0)
	{
		auto BlockBelow = a_Chunk->GetBlock(a_RelPos.addedY(-1));
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

	for (const auto & Cross: FlatCrossCoords)
	{
		BlockState Other = 0;
		if (a_Chunk->UnboundedRelGetBlock(a_RelPos + Cross, Other))
		{
			if (IsFuel(Other))
			{
				return static_cast<int>(m_BurnStepTimeFuel);
			}
		}
	}

	if (!IsBlockBelowSolid)
	{
		// Checked through everything, nothing was flammable
		// If block below isn't solid, we can't have fire, it would be a non-fueled fire
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
					if (cRoot::Get()->GetPluginManager()->CallHookBlockSpread(m_World, dstAbsPos, ssFireSpread))
					{
						return;
					}

					FIRE_FLOG("FS: Starting new fire at {0}.", dstAbsPos);
					a_Chunk->UnboundedRelSetBlock(dstRelPos, Block::Fire::Fire());
				}
			}  // for y
		}  // for z
	}  // for x
}





void cFireSimulator::RemoveFuelNeighbors(cChunk * a_Chunk, Vector3i a_RelPos)
{
	for (auto & Coord : ThreeDimensionalNeighborCoords)
	{
		BlockState Self = 0;
		auto RelPos = a_RelPos + Coord;
		auto NeighborChunk = a_Chunk->GetRelNeighborChunkAdjustCoords(RelPos);
		if ((NeighborChunk == nullptr) || !NeighborChunk->IsValid())
		{
			continue;
		}
		Self = NeighborChunk->GetBlock(RelPos);

		if (!IsFuel(Self))
		{
			continue;
		}

		auto AbsPos = NeighborChunk->RelativeToAbsolute(RelPos);
		if (Self.Type() == BlockType::Tnt)
		{
			NeighborChunk->SetBlock(RelPos, Block::Air::Air());
			m_World.SpawnPrimedTNT(Vector3d(AbsPos) + Vector3d(0.5, 0.5, 0.5));  // 80 ticks to boom
			return;
		}

		bool ShouldReplaceFuel = (GetRandomProvider().RandBool(m_ReplaceFuelChance * (1.0 / MAX_CHANCE_REPLACE_FUEL)));
		if (ShouldReplaceFuel && !cRoot::Get()->GetPluginManager()->CallHookBlockSpread(m_World, AbsPos, ssFireSpread))
		{
			NeighborChunk->SetBlock(RelPos, Block::Fire::Fire());
		}
		else
		{
			NeighborChunk->SetBlock(RelPos, Block::Air::Air());
		}
	}  // for i - Coords[]
}





bool cFireSimulator::CanStartFireInBlock(cChunk * a_NearChunk, Vector3i a_RelPos)
{
	BlockState Self = 0;
	if (!a_NearChunk->UnboundedRelGetBlock(a_RelPos, Self))
	{
		// The chunk is not accessible
		return false;
	}

	if (Self.Type() != BlockType::Air)
	{
		// Only an air block can be replaced by a fire block
		return false;
	}

	for (const auto & NeighborCoord : ThreeDimensionalNeighborCoords)
	{
		if (!a_NearChunk->UnboundedRelGetBlock(a_RelPos + NeighborCoord, Self))
		{
			// Neighbor inaccessible, skip it while evaluating
			continue;
		}
		if (IsFuel(Self))
		{
			return true;
		}
	}
	return false;
}
