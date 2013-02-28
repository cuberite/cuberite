
// FloodyFluidSimulator.cpp

// Interfaces to the cFloodyFluidSimulator that represents a fluid simulator that tries to flood everything :)
// http://forum.mc-server.org/showthread.php?tid=565

#include "Globals.h"

#include "FloodyFluidSimulator.h"
#include "../World.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"





// Enable or disable detailed logging
#if 0
	#define FLOG LOGD
#else
	#define FLOG(...)
#endif





cFloodyFluidSimulator::cFloodyFluidSimulator(
	cWorld & a_World,
	BLOCKTYPE a_Fluid,
	BLOCKTYPE a_StationaryFluid,
	NIBBLETYPE a_Falloff,
	int a_TickDelay,
	int a_NumNeighborsForSource
) :
	super(a_World, a_Fluid, a_StationaryFluid, a_TickDelay),
	m_Falloff(a_Falloff),
	m_NumNeighborsForSource(a_NumNeighborsForSource)
{
}





void cFloodyFluidSimulator::SimulateBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	FLOG("Simulating block {%d, %d, %d}", a_BlockX, a_BlockY, a_BlockZ);
	
	cBlockArea Area;
	int MinBlockY = std::max(0, a_BlockY - 1);
	int MaxBlockY = std::min(+cChunkDef::Height, a_BlockY + 1);
	if (!Area.Read(&m_World, a_BlockX - 1, a_BlockX + 1, MinBlockY, MaxBlockY, a_BlockZ - 1, a_BlockZ + 1))
	{
		// Cannot read the immediate neighborhood, probably too close to an unloaded chunk. Bail out.
		// TODO: Shouldn't we re-schedule?
		FLOG("  Cannot read area, bailing out.");
		return;
	}
	int y = (a_BlockY > 0) ? 1 : 0;  // Relative y-coord of this block in Area
	
	NIBBLETYPE MyMeta = Area.GetRelBlockMeta(1, y, 1);
	if (!IsAnyFluidBlock(Area.GetRelBlockType(1, y, 1)))
	{
		// Can happen - if a block is scheduled for simulating and gets replaced in the meantime.
		FLOG("  Not my type: exp %d, got %d", m_FluidBlock, Area.GetRelBlockType(1, y, 1));
		return;
	}

	if (MyMeta != 0)
	{
		// Source blocks aren't checked for tributaries, others are.
		if (CheckTributaries(a_BlockX, a_BlockY, a_BlockZ, Area, MyMeta))
		{
			// Has no tributary, has been decreased, no more processing needed (neighbors have been scheduled by the decrease)
			return;
		}
	}
	
	// New meta for the spreading to neighbors:
	// If this is a source block or was falling, the new meta is just the falloff
	// Otherwise it is the current meta plus falloff (may be larger than max height, will be checked later)
	NIBBLETYPE NewMeta = ((MyMeta == 0) || ((MyMeta & 0x08) != 0)) ? m_Falloff : (MyMeta + m_Falloff);
	
	BLOCKTYPE Below = Area.GetRelBlockType(1, 0, 1);
	if ((a_BlockY > 0) && (IsPassableForFluid(Below) || IsBlockLava(Below) || IsBlockWater(Below)))
	{
		// Spread only down, possibly washing away what's there or turning lava to stone / cobble / obsidian:
		SpreadToNeighbor(a_BlockX, a_BlockY - 1, a_BlockZ, Area, 8);
	}
	else if (NewMeta < 8)   // Can reach there
	{
		// If source creation is on, check for it here:
		if (
			(m_NumNeighborsForSource > 0) &&  // Source creation is on
			(MyMeta == m_Falloff) &&          // Only exactly one block away from a source (fast bail-out)
			!IsPassableForFluid(Below) &&     // Only exactly 1 block deep
			CheckNeighborsForSource(a_BlockX, a_BlockY, a_BlockZ, Area)  // Did we create a source?
		)
		{
			// We created a source, no more spreading is to be done now
			// Also has been re-scheduled for ticking in the next wave
			return;
		}
		
		// Spread to the neighbors:
		SpreadToNeighbor(a_BlockX - 1, a_BlockY, a_BlockZ,     Area, NewMeta);
		SpreadToNeighbor(a_BlockX + 1, a_BlockY, a_BlockZ,     Area, NewMeta);
		SpreadToNeighbor(a_BlockX,     a_BlockY, a_BlockZ - 1, Area, NewMeta);
		SpreadToNeighbor(a_BlockX,     a_BlockY, a_BlockZ + 1, Area, NewMeta);
	}
	
	// Mark as processed:
	m_World.FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, m_StationaryFluidBlock, MyMeta);
}





bool cFloodyFluidSimulator::CheckTributaries(int a_BlockX, int a_BlockY, int a_BlockZ, const cBlockArea & a_Area, NIBBLETYPE a_MyMeta)
{
	bool IsFed = false;
	int y = (a_BlockY > 0) ? 1 : 0;  // Relative y-coord of this block in Area
	
	// If we have a section above, check if there's fluid above this block that would feed it:
	if (a_BlockY < cChunkDef::Height - 1)
	{
		IsFed = IsAnyFluidBlock(a_Area.GetRelBlockType(1, 2, 1));
	}

	// If not fed from above, check if there's a feed from the side (but not if it's a downward-flowing block):
	if (!IsFed && (a_MyMeta != 8))
	{
		IsFed = (
			(IsAllowedBlock(a_Area.GetRelBlockType(0, y, 1)) && IsHigherMeta(a_Area.GetRelBlockMeta(0, y, 1), a_MyMeta)) ||
			(IsAllowedBlock(a_Area.GetRelBlockType(2, y, 1)) && IsHigherMeta(a_Area.GetRelBlockMeta(2, y, 1), a_MyMeta)) ||
			(IsAllowedBlock(a_Area.GetRelBlockType(1, y, 0)) && IsHigherMeta(a_Area.GetRelBlockMeta(1, y, 0), a_MyMeta)) ||
			(IsAllowedBlock(a_Area.GetRelBlockType(1, y, 2)) && IsHigherMeta(a_Area.GetRelBlockMeta(1, y, 2), a_MyMeta))
		);
	}
	
	// If not fed, decrease by m_Falloff levels:
	if (!IsFed)
	{
		if (a_MyMeta >= 8)
		{
			FLOG("  Not fed and downwards, turning into non-downwards meta %d", m_Falloff);
			m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_StationaryFluidBlock, m_Falloff);
		}
		else
		{
			a_MyMeta += m_Falloff;
			if (a_MyMeta < 8)
			{
				FLOG("  Not fed, decreasing from %d to %d", a_MyMeta, a_MyMeta + m_Falloff);
				m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_StationaryFluidBlock, a_MyMeta);
			}
			else
			{
				FLOG("  Not fed, meta %d, erasing altogether", a_MyMeta);
				m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
			}
		}
		return true;
	}
	return false;
}





void cFloodyFluidSimulator::SpreadToNeighbor(int a_BlockX, int a_BlockY, int a_BlockZ, const cBlockArea & a_Area, NIBBLETYPE a_NewMeta)
{
	ASSERT(a_NewMeta <= 8);  // Invalid meta values
	ASSERT(a_NewMeta > 0);  // Source blocks aren't spread
	
	BLOCKTYPE Block = a_Area.GetBlockType(a_BlockX, a_BlockY, a_BlockZ);
	
	if (IsAllowedBlock(Block))
	{
		NIBBLETYPE Meta = a_Area.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if ((Meta == a_NewMeta) || IsHigherMeta(Meta, a_NewMeta))
		{
			// Don't spread there, there's already a higher or same level there
			return;
		}
	}

	// Check water - lava interaction:
	if (m_FluidBlock == E_BLOCK_LAVA)
	{
		if (IsBlockWater(Block))
		{
			// Lava flowing into water, change to stone / cobblestone based on direction:
			BLOCKTYPE NewBlock = (a_NewMeta == 8) ? E_BLOCK_STONE : E_BLOCK_COBBLESTONE;
			FLOG("  Lava flowing into water, turning water at {%d, %d, %d} into stone", 
				a_BlockX, a_BlockY, a_BlockZ,
				ItemTypeToString(NewBlock).c_str()
			);
			m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, NewBlock, 0);
			
			// TODO: Sound effect
			
			return;
		}
	}
	else if (m_FluidBlock == E_BLOCK_WATER)
	{
		if (IsBlockLava(Block))
		{
			// Water flowing into lava, change to cobblestone / obsidian based on dest block:
			BLOCKTYPE NewBlock = (a_Area.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) == 0) ? E_BLOCK_OBSIDIAN : E_BLOCK_COBBLESTONE;
			FLOG("  Water flowing into lava, turning lava at {%d, %d, %d} into %s", 
				a_BlockX, a_BlockY, a_BlockZ, ItemTypeToString(NewBlock).c_str()
			);
			m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, NewBlock, 0);
			
			// TODO: Sound effect
			
			return;
		}
	}
	else
	{
		ASSERT(!"Unknown fluid!");
	}
	
	if (!IsPassableForFluid(Block))
	{
		// Can't spread there
		return;
	}
	
	// Wash away the block there, if possible:
	if (CanWashAway(Block))
	{
		cBlockHandler * Handler = BlockHandler(Block);
		if (Handler->DoesDropOnUnsuitable())
		{
			Handler->DropBlock(&m_World, NULL, a_BlockX, a_BlockY, a_BlockZ);
		}
	}
	
	// Spread:
	FLOG("  Spreading to {%d, %d, %d} with meta %d", a_BlockX, a_BlockY, a_BlockZ, a_NewMeta);
	m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_FluidBlock, a_NewMeta);
}





bool cFloodyFluidSimulator::CheckNeighborsForSource(int a_BlockX, int a_BlockY, int a_BlockZ, const cBlockArea & a_Area)
{
	FLOG("  Checking neighbors for source creation");
	
	static const Vector3i NeighborCoords[] =
	{
		Vector3i(-1, 0, 0),
		Vector3i( 1, 0, 0),
		Vector3i( 0, 0,-1),
		Vector3i( 0, 0, 1),
	} ;
	
	int NumNeeded = m_NumNeighborsForSource;
	for (int i = 0; i < ARRAYCOUNT(NeighborCoords); i++)
	{
		int x = a_BlockX + NeighborCoords[i].x;
		int y = a_BlockY + NeighborCoords[i].y;
		int z = a_BlockZ + NeighborCoords[i].z;
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		a_Area.GetBlockTypeMeta(x, y, z, BlockType, BlockMeta);
		FLOG("   Neighbor at {%d, %d, %d}: %s", x, y, z, ItemToFullString(cItem(BlockType, 1, BlockMeta)).c_str());
		if ((BlockMeta == 0) && IsAnyFluidBlock(BlockType))
		{
			NumNeeded--;
			FLOG("    Found a neighbor source at {%d, %d, %d}, NumNeeded := %d", x, y, z, NumNeeded);
			if (NumNeeded == 0)
			{
				// Found enough, turn into a source and bail out
				FLOG("    Found enough neighbor sources, turning into a source");
				m_World.SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_FluidBlock, 0);
				return true;
			}
		}
	}
	FLOG("    Not enough neighbors for turning into a source, NumNeeded = %d", NumNeeded);
	return false;
}




