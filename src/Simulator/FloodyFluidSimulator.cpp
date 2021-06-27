
// FloodyFluidSimulator.cpp

// Interfaces to the cFloodyFluidSimulator that represents a fluid simulator that tries to flood everything :)
// https://forum.cuberite.org/thread-565.html

#include "Globals.h"

#include "FloodyFluidSimulator.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Chunk.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"
#include "../Blocks/ChunkInterface.h"
#include "../Blocks/BlockFluid.h"



// #define LOGFLUID

// Enable or disable detailed logging
#ifdef LOGFLUID
	#define FLUID_FLOG FLOGD
#else
	#define FLUID_FLOG(...)
#endif





cFloodyFluidSimulator::cFloodyFluidSimulator(
	cWorld & a_World,
	BlockType a_Fluid,
	unsigned char a_StationaryFalloffValue,
	unsigned char a_Falloff,
	size_t a_TickDelay,
	int a_NumNeighborsForSource
) :
	Super(a_World, a_Fluid, a_StationaryFalloffValue, a_TickDelay),
	m_Falloff(a_Falloff),
	m_NumNeighborsForSource(a_NumNeighborsForSource)
{
}





void cFloodyFluidSimulator::SimulateBlock(cChunk * a_Chunk, Vector3i a_RelPos)
{
	auto Self = a_Chunk->GetBlock(a_RelPos);
	FLUID_FLOG("Simulating block {0}: block {1}. {2} block queued. \t\t SimSlot {3}, AddSlot {4}",
		a_Chunk->PositionToWorldPosition(a_RelPos),
		Self,
		m_TotalBlocks,
		m_SimSlotNum,
		m_AddSlotNum
	);

	if (!IsFluidBlock(Self.Type()))
	{
		// Can happen - if a block is scheduled for simulating and gets replaced in the meantime.
		FLUID_FLOG("  BadBlockType exit");
		return;
	}

	auto OldFalloff = cBlockFluidHandler::GetFalloff(Self);

	// When in contact with water, lava should harden
	if (HardenBlock(a_Chunk, a_RelPos, Self.Type(), OldFalloff))
	{
		// Block was changed, bail out
		return;
	}

	if (OldFalloff != 0)
	{
		// Source blocks aren't checked for tributaries, others are.
		if (CheckTributaries(a_Chunk, a_RelPos, OldFalloff))
		{
			// Has no tributary, has been decreased (in CheckTributaries()),
			// no more processing needed (neighbors have been scheduled by the decrease)
			FLUID_FLOG("  CheckTributaries exit");
			return;
		}
	}

	// New meta for the spreading to neighbors:
	// If this is a source block or was falling, the new meta is just the falloff
	// Otherwise it is the current meta plus falloff (may be larger than max height, will be checked later)
	unsigned char NewFalloff = ((OldFalloff == 0) || (OldFalloff == 8)) ? m_Falloff : (OldFalloff + m_Falloff);
	if (a_RelPos.y > 0)
	{
		bool SpreadFurther = true;
		auto Below = a_Chunk->GetBlock(a_RelPos.addedY(-1));
		if (IsPassableForFluid(Below) || (Below.Type() == BlockType::Water) || (Below.Type() == BlockType::Lava))
		{
			// Spread only down, possibly washing away what's there or turning lava to stone / cobble / obsidian:
			SpreadToNeighbor(a_Chunk, a_RelPos.addedY(-1), 8);
			// Source blocks spread both downwards and sideways
			if (OldFalloff != 0)
			{
				SpreadFurther = false;
			}
		}

		// Spread to the neighbors:
		if (SpreadFurther && (NewFalloff < 8))
		{
			SpreadXZ(a_Chunk, a_RelPos, NewFalloff);
		}

		// If source creation is on, check for it here:
		if (
			(m_NumNeighborsForSource > 0) &&    // Source creation is on
			(OldFalloff == m_Falloff) &&        // Only exactly one block away from a source (fast bail-out)
			(
				!IsPassableForFluid(Below) ||   // Only exactly 1 block deep
				((Below.Type() == m_FluidBlock) && (cBlockFluidHandler::GetFalloff(Below) == m_StationaryFalloffValue))  // Or a source block underneath
			) &&
			CheckNeighborsForSource(a_Chunk, a_RelPos)  // Did we create a source?
		)
		{
			// We created a source, no more spreading is to be done now
			// Also has been re-scheduled for ticking in the next wave, so no marking is needed
			return;
		}
	}
}





void cFloodyFluidSimulator::SpreadXZ(cChunk * a_Chunk, Vector3i a_RelPos, unsigned char a_Falloff)
{
	SpreadToNeighbor(a_Chunk, a_RelPos.addedX(-1), a_Falloff);
	SpreadToNeighbor(a_Chunk, a_RelPos.addedX(1),  a_Falloff);
	SpreadToNeighbor(a_Chunk, a_RelPos.addedZ(-1), a_Falloff);
	SpreadToNeighbor(a_Chunk, a_RelPos.addedZ(1),  a_Falloff);
}





bool cFloodyFluidSimulator::CheckTributaries(cChunk * a_Chunk, Vector3i a_RelPos, unsigned char a_OldFalloff)
{
	// If we have a section above, check if there's fluid above this block that would feed it:
	if (a_RelPos.y < cChunkDef::Height - 1)
	{
		if (IsFluidBlock(a_Chunk->GetBlock(a_RelPos.addedY(1)).Type()))
		{
			// This block is fed from above, no more processing needed
			FLUID_FLOG("  Fed from above");
			return false;
		}
	}

	// Not fed from above, check if there's a feed from the side (but not if it's a downward-flowing block):
	if (a_OldFalloff != 8)
	{
		for (const auto & Offset : FlatCrossCoords)
		{
			BlockState Neighbor;
			if (!a_Chunk->UnboundedRelGetBlock(a_RelPos + Offset, Neighbor))
			{
				continue;
			}
			if (!IsFluidBlock(Neighbor.Type()))
			{
				continue;
			}
			if (IsAllowedBlock(Neighbor) && IsHigherMeta(cBlockFluidHandler::GetFalloff(Neighbor), a_OldFalloff))
			{
				// This block is fed, no more processing needed
				FLUID_FLOG("  Fed from {0}, type {1}",
					a_Chunk->PositionToWorldPosition(a_RelPos + Offset),
					Neighbor
				);
				return false;
			}
		}
	}  // if not fed from above

	// Block is not fed, decrease by m_Falloff levels:
	if (a_OldFalloff >= 8)
	{
		FLUID_FLOG("  Not fed and downwards, turning into non-downwards meta {0}", m_Falloff);
		switch (m_FluidBlock)
		{
			case BlockType::Lava:  a_Chunk->SetBlock(a_RelPos,  Block::Lava::Lava (m_Falloff)); break;
			case BlockType::Water: a_Chunk->SetBlock(a_RelPos, Block::Water::Water(m_Falloff)); break;
			default: UNREACHABLE("Unsupported Fluid Block");
		}
	}
	else
	{
		a_OldFalloff += m_Falloff;
		if (a_OldFalloff < 8)
		{
			FLUID_FLOG("  Not fed, decreasing from {0} to {1}", a_OldFalloff - m_Falloff, a_OldFalloff);
			switch (m_FluidBlock)
			{
				case BlockType::Lava:  a_Chunk->SetBlock(a_RelPos, Block::Lava::Lava(a_OldFalloff));   break;
				case BlockType::Water: a_Chunk->SetBlock(a_RelPos, Block::Water::Water(a_OldFalloff)); break;
				default: UNREACHABLE("Unsupported Fluid Block");
			}
		}
		else
		{
			FLUID_FLOG("  Not fed, meta {0}, erasing altogether", a_OldFalloff);
			a_Chunk->SetBlock(a_RelPos, Block::Air::Air());
		}
	}
	return true;
}





void cFloodyFluidSimulator::SpreadToNeighbor(cChunk * a_NearChunk, Vector3i a_RelPos, unsigned char a_NewFalloff)
{
	ASSERT(a_NewFalloff <= 8);  // Invalid meta values
	ASSERT(a_NewFalloff > 0);  // Source blocks aren't spread

	Vector3i RelPos(a_RelPos);
	a_NearChunk = a_NearChunk->GetRelNeighborChunkAdjustCoords(RelPos);
	if ((a_NearChunk == nullptr) || (!a_NearChunk->IsValid()))
	{
		// Chunk not available
		return;
	}


	const auto AbsPos = a_NearChunk->RelativeToAbsolute(RelPos);
	auto Neighbour = a_NearChunk->GetBlock(RelPos);
	auto OldFalloff = cBlockFluidHandler::GetFalloff(Neighbour);

	if (IsAllowedBlock(Neighbour.Type()))
	{
		if ((OldFalloff == a_NewFalloff) || IsHigherMeta(OldFalloff, a_NewFalloff))
		{
			// Don't spread there, there's already a higher or same level there
			return;
		}
	}

	// Check water - lava interaction:
	if (m_FluidBlock == BlockType::Lava)
	{
		if (Neighbour.Type() == BlockType::Water)
		{
			// Lava flowing into water, change to stone / cobblestone based on direction:
			auto NewBlock = (cBlockFluidHandler::GetFalloff(Neighbour) == 8) ? Block::Stone::Stone() : Block::Cobblestone::Cobblestone();
			FLUID_FLOG("  Lava flowing into water, turning water at rel {0} into {1}",
				RelPos, NewBlock
			);
			a_NearChunk->SetBlock(RelPos, NewBlock);

			m_World.BroadcastSoundEffect(
				"block.lava.extinguish",
				AbsPos,
				0.5f,
				1.5f
			);
			return;
		}
	}
	else if (m_FluidBlock == BlockType::Water)
	{
		if (Neighbour.Type() == BlockType::Lava)
		{
			// Water flowing into lava, change to cobblestone / obsidian based on dest block:
			auto NewBlock = (cBlockFluidHandler::GetFalloff(Neighbour) == 0) ? Block::Obsidian::Obsidian() : Block::Cobblestone::Cobblestone();
			FLUID_FLOG("  Water flowing into lava, turning lava at rel {0} into {1}",
				RelPos, NewBlock
			);
			a_NearChunk->SetBlock(RelPos, NewBlock);

			m_World.BroadcastSoundEffect(
				"block.lava.extinguish",
				AbsPos,
				0.5f,
				1.5f
			);
			return;
		}
	}
	else
	{
		ASSERT(!"Unknown fluid!");
	}

	if (!IsPassableForFluid(Neighbour))
	{
		// Can't spread there
		return;
	}

	// Wash away the block there, if possible:
	if (CanWashAway(Neighbour))
	{
		m_World.DropBlockAsPickups(AbsPos, nullptr, nullptr);
	}

	// Spread:
	FLUID_FLOG("  Spreading to {0} with meta {1}", AbsPos, a_NewFalloff);
	switch (m_FluidBlock)
	{
		case BlockType::Lava:  a_NearChunk->SetBlock(RelPos, Block::Lava::Lava(a_NewFalloff));   break;
		case BlockType::Water: a_NearChunk->SetBlock(RelPos, Block::Water::Water(a_NewFalloff)); break;
		default: UNREACHABLE("Unsupported Fluid Block");
	}

	m_World.GetSimulatorManager()->WakeUp(*a_NearChunk, RelPos);
	HardenBlock(a_NearChunk, RelPos, m_FluidBlock, a_NewFalloff);
}





bool cFloodyFluidSimulator::CheckNeighborsForSource(cChunk * a_Chunk, Vector3i a_RelPos)
{
	FLUID_FLOG("  Checking neighbors for source creation");

	int NumNeeded = m_NumNeighborsForSource;
	for (const auto & Offset : FlatCrossCoords)
	{
		BlockState Neighbor;
		if (!a_Chunk->UnboundedRelGetBlock(a_RelPos + Offset, Neighbor))
		{
			// Neighbor not available, skip it
			continue;
		}
		FLUID_FLOG("   Neighbor at {0}: {1}", a_RelPos + Offset, Neighbor);
		if ((cBlockFluidHandler::GetFalloff(Neighbor) == 0) && IsFluidBlock(Neighbor.Type()))
		{
			NumNeeded--;
			FLUID_FLOG("    Found a neighbor source at {0}, NumNeeded := {1}", a_RelPos + Offset, NumNeeded);
			if (NumNeeded == 0)
			{
				// Found enough, turn into a source and bail out
				FLUID_FLOG("    Found enough neighbor sources, turning into a source");
				switch (m_FluidBlock)
				{
					case BlockType::Lava:  a_Chunk->SetBlock(a_RelPos, Block::Lava::Lava());   break;
					case BlockType::Water: a_Chunk->SetBlock(a_RelPos, Block::Water::Water()); break;
					default: UNREACHABLE("Unsupported Fluid Block");
				}
				return true;
			}
		}
	}
	FLUID_FLOG("    Not enough neighbors for turning into a source, NumNeeded = {0}", NumNeeded);
	return false;
}





bool cFloodyFluidSimulator::HardenBlock(cChunk * a_Chunk, Vector3i a_RelPos, BlockType a_Block, unsigned char a_NewFalloff)
{
	ASSERT(cChunkDef::IsValidRelPos(a_RelPos));

	// Only lava blocks can harden
	if (a_Block != BlockType::Lava)
	{
		return false;
	}

	bool ShouldHarden = false;

	BlockState Neighbor;
	for (const auto & Offset : FlatCrossCoords)
	{
		if (!a_Chunk->UnboundedRelGetBlock(a_RelPos + Offset, Neighbor))
		{
			continue;
		}
		if (Neighbor.Type() == BlockType::Water)
		{
			ShouldHarden = true;
		}
	}

	if (ShouldHarden)
	{
		if (a_NewFalloff == 0)
		{
			// Source lava block
			a_Chunk->SetBlock(a_RelPos, Block::Obsidian::Obsidian());
			return true;
		}
		// Ignore last lava level
		else if (a_NewFalloff <= 4)
		{
			a_Chunk->SetBlock(a_RelPos, Block::Cobblestone::Cobblestone());
			return true;
		}
	}

	return false;
}



