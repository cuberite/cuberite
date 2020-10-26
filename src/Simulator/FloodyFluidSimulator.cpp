
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





// Enable or disable detailed logging
#if 0
	#define FLUID_FLOG FLOGD
#else
	#define FLUID_FLOG(...)
#endif





cFloodyFluidSimulator::cFloodyFluidSimulator(
	cWorld & a_World,
	BLOCKTYPE a_Fluid,
	BLOCKTYPE a_StationaryFluid,
	NIBBLETYPE a_Falloff,
	int a_TickDelay,
	int a_NumNeighborsForSource
) :
	Super(a_World, a_Fluid, a_StationaryFluid, a_TickDelay),
	m_Falloff(a_Falloff),
	m_NumNeighborsForSource(a_NumNeighborsForSource)
{
}





void cFloodyFluidSimulator::SimulateBlock(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
{
	FLUID_FLOG("Simulating block {0}: block {1}, meta {2}",
		a_Chunk->PositionToWorldPosition(a_RelX, a_RelY, a_RelZ),
		a_Chunk->GetBlock(a_RelX, a_RelY, a_RelZ),
		a_Chunk->GetMeta(a_RelX, a_RelY, a_RelZ)
	);

	BLOCKTYPE MyBlock; NIBBLETYPE MyMeta;
	a_Chunk->GetBlockTypeMeta(a_RelX, a_RelY, a_RelZ, MyBlock, MyMeta);

	if (!IsAnyFluidBlock(MyBlock))
	{
		// Can happen - if a block is scheduled for simulating and gets replaced in the meantime.
		FLUID_FLOG("  BadBlockType exit");
		return;
	}

	// When in contact with water, lava should harden
	if (HardenBlock(a_Chunk, {a_RelX, a_RelY, a_RelZ}, MyBlock, MyMeta))
	{
		// Block was changed, bail out
		return;
	}

	if (MyMeta != 0)
	{
		// Source blocks aren't checked for tributaries, others are.
		if (CheckTributaries(a_Chunk, a_RelX, a_RelY, a_RelZ, MyMeta))
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
	NIBBLETYPE NewMeta = ((MyMeta == 0) || ((MyMeta & 0x08) != 0)) ? m_Falloff : (MyMeta + m_Falloff);
	if (a_RelY > 0)
	{
		bool SpreadFurther = true;
		BLOCKTYPE Below = a_Chunk->GetBlock(a_RelX, a_RelY - 1, a_RelZ);
		if (IsPassableForFluid(Below) || IsBlockLava(Below) || IsBlockWater(Below))
		{
			// Spread only down, possibly washing away what's there or turning lava to stone / cobble / obsidian:
			SpreadToNeighbor(a_Chunk, a_RelX, a_RelY - 1, a_RelZ, 8);

			// Source blocks spread both downwards and sideways
			if (MyMeta != 0)
			{
				SpreadFurther = false;
			}
		}
		// Spread to the neighbors:
		if (SpreadFurther && (NewMeta < 8))
		{
			SpreadXZ(a_Chunk, a_RelX, a_RelY, a_RelZ, NewMeta);
		}

		// If source creation is on, check for it here:
		if (
			(m_NumNeighborsForSource > 0) &&    // Source creation is on
			(MyMeta == m_Falloff) &&            // Only exactly one block away from a source (fast bail-out)
			(
				!IsPassableForFluid(Below) ||  // Only exactly 1 block deep
				(Below == m_StationaryFluidBlock)  // Or a source block underneath
			) &&
			CheckNeighborsForSource(a_Chunk, a_RelX, a_RelY, a_RelZ)  // Did we create a source?
		)
		{
			// We created a source, no more spreading is to be done now
			// Also has been re-scheduled for ticking in the next wave, so no marking is needed
			return;
		}
	}

	// Mark as processed:
	a_Chunk->FastSetBlock(a_RelX, a_RelY, a_RelZ, m_StationaryFluidBlock, MyMeta);
}





void cFloodyFluidSimulator::SpreadXZ(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_NewMeta)
{
	SpreadToNeighbor(a_Chunk, a_RelX - 1, a_RelY, a_RelZ,     a_NewMeta);
	SpreadToNeighbor(a_Chunk, a_RelX + 1, a_RelY, a_RelZ,     a_NewMeta);
	SpreadToNeighbor(a_Chunk, a_RelX,     a_RelY, a_RelZ - 1, a_NewMeta);
	SpreadToNeighbor(a_Chunk, a_RelX,     a_RelY, a_RelZ + 1, a_NewMeta);
}





bool cFloodyFluidSimulator::CheckTributaries(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_MyMeta)
{
	// If we have a section above, check if there's fluid above this block that would feed it:
	if (a_RelY < cChunkDef::Height - 1)
	{
		if (IsAnyFluidBlock(a_Chunk->GetBlock(a_RelX, a_RelY + 1, a_RelZ)))
		{
			// This block is fed from above, no more processing needed
			FLUID_FLOG("  Fed from above");
			return false;
		}
	}

	// Not fed from above, check if there's a feed from the side (but not if it's a downward-flowing block):
	if (a_MyMeta != 8)
	{
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		static const Vector3i Coords[] =
		{
			Vector3i( 1, 0,  0),
			Vector3i(-1, 0,  0),
			Vector3i( 0, 0,  1),
			Vector3i( 0, 0, -1),
		} ;
		for (size_t i = 0; i < ARRAYCOUNT(Coords); i++)
		{
			if (!a_Chunk->UnboundedRelGetBlock(a_RelX + Coords[i].x, a_RelY, a_RelZ + Coords[i].z, BlockType, BlockMeta))
			{
				continue;
			}
			if (IsAllowedBlock(BlockType) && IsHigherMeta(BlockMeta, a_MyMeta))
			{
				// This block is fed, no more processing needed
				FLUID_FLOG("  Fed from {0}, type {1}, meta {2}",
					a_Chunk->PositionToWorldPosition(a_RelX+ Coords[i].x, a_RelY, a_RelZ + Coords[i].z),
					BlockType, BlockMeta
				);
				return false;
			}
		}  // for i - Coords[]
	}  // if not fed from above

	// Block is not fed, decrease by m_Falloff levels:
	if (a_MyMeta >= 8)
	{
		FLUID_FLOG("  Not fed and downwards, turning into non-downwards meta {0}", m_Falloff);
		a_Chunk->SetBlock({a_RelX, a_RelY, a_RelZ}, m_StationaryFluidBlock, m_Falloff);
	}
	else
	{
		a_MyMeta += m_Falloff;
		if (a_MyMeta < 8)
		{
			FLUID_FLOG("  Not fed, decreasing from {0} to {1}", a_MyMeta - m_Falloff, a_MyMeta);
			a_Chunk->SetBlock({a_RelX, a_RelY, a_RelZ}, m_StationaryFluidBlock, a_MyMeta);
		}
		else
		{
			FLUID_FLOG("  Not fed, meta {0}, erasing altogether", a_MyMeta);
			a_Chunk->SetBlock({a_RelX, a_RelY, a_RelZ}, E_BLOCK_AIR, 0);
		}
	}
	return true;
}





void cFloodyFluidSimulator::SpreadToNeighbor(cChunk * a_NearChunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_NewMeta)
{
	ASSERT(a_NewMeta <= 8);  // Invalid meta values
	ASSERT(a_NewMeta > 0);  // Source blocks aren't spread

	Vector3i relPos(a_RelX, a_RelY, a_RelZ);
	a_NearChunk = a_NearChunk->GetRelNeighborChunkAdjustCoords(relPos);
	if ((a_NearChunk == nullptr) || (!a_NearChunk->IsValid()))
	{
		// Chunk not available
		return;
	}

	const auto absPos = a_NearChunk->RelativeToAbsolute(relPos);
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	a_NearChunk->GetBlockTypeMeta(relPos, BlockType, BlockMeta);

	if (IsAllowedBlock(BlockType))
	{
		if ((BlockMeta == a_NewMeta) || IsHigherMeta(BlockMeta, a_NewMeta))
		{
			// Don't spread there, there's already a higher or same level there
			return;
		}
	}

	// Check water - lava interaction:
	if (m_FluidBlock == E_BLOCK_LAVA)
	{
		if (IsBlockWater(BlockType))
		{
			// Lava flowing into water, change to stone / cobblestone based on direction:
			BLOCKTYPE NewBlock = (a_NewMeta == 8) ? E_BLOCK_STONE : E_BLOCK_COBBLESTONE;
			FLUID_FLOG("  Lava flowing into water, turning water at rel {0} into {1}",
				relPos, ItemTypeToString(NewBlock)
			);
			a_NearChunk->SetBlock(relPos, NewBlock, 0);

			m_World.BroadcastSoundEffect(
				"block.lava.extinguish",
				absPos,
				0.5f,
				1.5f
			);
			return;
		}
	}
	else if (m_FluidBlock == E_BLOCK_WATER)
	{
		if (IsBlockLava(BlockType))
		{
			// Water flowing into lava, change to cobblestone / obsidian based on dest block:
			BLOCKTYPE NewBlock = (BlockMeta == 0) ? E_BLOCK_OBSIDIAN : E_BLOCK_COBBLESTONE;
			FLUID_FLOG("  Water flowing into lava, turning lava at rel {0} into {1}",
				relPos, ItemTypeToString(NewBlock)
			);
			a_NearChunk->SetBlock(relPos, NewBlock, 0);

			m_World.BroadcastSoundEffect(
				"block.lava.extinguish",
				absPos,
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

	if (!IsPassableForFluid(BlockType))
	{
		// Can't spread there
		return;
	}

	// Wash away the block there, if possible:
	if (CanWashAway(BlockType))
	{
		if (cBlockHandler::For(BlockType).DoesDropOnUnsuitable())
		{
			m_World.DropBlockAsPickups(absPos, nullptr, nullptr);
		}
	}  // if (CanWashAway)

	// Spread:
	FLUID_FLOG("  Spreading to {0} with meta {1}", absPos, a_NewMeta);
	a_NearChunk->SetBlock(relPos, m_FluidBlock, a_NewMeta);
	m_World.GetSimulatorManager()->WakeUp(*a_NearChunk, relPos);

	HardenBlock(a_NearChunk, relPos, m_FluidBlock, a_NewMeta);
}





bool cFloodyFluidSimulator::CheckNeighborsForSource(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ)
{
	FLUID_FLOG("  Checking neighbors for source creation");

	static const Vector3i NeighborCoords[] =
	{
		Vector3i(-1, 0,  0),
		Vector3i( 1, 0,  0),
		Vector3i( 0, 0, -1),
		Vector3i( 0, 0,  1),
	} ;

	int NumNeeded = m_NumNeighborsForSource;
	for (size_t i = 0; i < ARRAYCOUNT(NeighborCoords); i++)
	{
		int x = a_RelX + NeighborCoords[i].x;
		int y = a_RelY + NeighborCoords[i].y;
		int z = a_RelZ + NeighborCoords[i].z;
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		if (!a_Chunk->UnboundedRelGetBlock(x, y, z, BlockType, BlockMeta))
		{
			// Neighbor not available, skip it
			continue;
		}
		// FLUID_FLOG("   Neighbor at {0}: {1}", Vector3i{x, y, z}, ItemToFullString(cItem(BlockType, 1, BlockMeta)));
		if ((BlockMeta == 0) && IsAnyFluidBlock(BlockType))
		{
			NumNeeded--;
			// FLUID_FLOG("    Found a neighbor source at {0}, NumNeeded := {1}", Vector3i{x, y, z}, NumNeeded);
			if (NumNeeded == 0)
			{
				// Found enough, turn into a source and bail out
				// FLUID_FLOG("    Found enough neighbor sources, turning into a source");
				a_Chunk->SetBlock({a_RelX, a_RelY, a_RelZ}, m_FluidBlock, 0);
				return true;
			}
		}
	}
	// FLUID_FLOG("    Not enough neighbors for turning into a source, NumNeeded = {0}", NumNeeded);
	return false;
}





bool cFloodyFluidSimulator::HardenBlock(cChunk * a_Chunk, Vector3i a_RelPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_Meta)
{
	ASSERT(cChunkDef::IsValidRelPos(a_RelPos));

	// Only lava blocks can harden
	if (!IsBlockLava(a_BlockType))
	{
		return false;
	}

	bool ShouldHarden = false;

	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	static const Vector3i neighborOffsets[] =
	{
		Vector3i( 1, 0,  0),
		Vector3i(-1, 0,  0),
		Vector3i( 0, 0,  1),
		Vector3i( 0, 0, -1),
	};
	for (const auto & ofs: neighborOffsets)
	{
		if (!a_Chunk->UnboundedRelGetBlock(a_RelPos + ofs, BlockType, BlockMeta))
		{
			continue;
		}
		if (IsBlockWater(BlockType))
		{
			ShouldHarden = true;
		}
	}  // for i - Coords[]

	if (ShouldHarden)
	{
		if (a_Meta == 0)
		{
			// Source lava block
			a_Chunk->SetBlock(a_RelPos, E_BLOCK_OBSIDIAN, 0);
			return true;
		}
		// Ignore last lava level
		else if (a_Meta <= 4)
		{
			a_Chunk->SetBlock(a_RelPos, E_BLOCK_COBBLESTONE, 0);
			return true;
		}
	}

	return false;
}



