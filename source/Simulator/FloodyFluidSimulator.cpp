
// FloodyFluidSimulator.cpp

// Interfaces to the cFloodyFluidSimulator that represents a fluid simulator that tries to flood everything :)
// http://forum.mc-server.org/showthread.php?tid=565

#include "Globals.h"

#include "FloodyFluidSimulator.h"
#include "../World.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"





cFloodyFluidSimulator::cFloodyFluidSimulator(cWorld * a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, NIBBLETYPE a_Falloff, int a_TickDelay) :
	super(a_World, a_Fluid, a_StationaryFluid, a_TickDelay),
	m_Falloff(a_Falloff)
{
}





void cFloodyFluidSimulator::SimulateBlock(int a_BlockX, int a_BlockY, int a_BlockZ)
{
	cBlockArea Area;
	int MinBlockY = std::max(0, a_BlockY - 1);
	int MaxBlockY = std::min(cChunkDef::Height, a_BlockY + 1);
	if (!Area.Read(m_World, a_BlockX - 1, a_BlockX + 1, MinBlockY, MaxBlockY, a_BlockZ - 1, a_BlockZ + 1))
	{
		// Cannot read the immediate neighborhood, probably too close to an unloaded chunk. Bail out.
		// TODO: Shouldn't we re-schedule?
		return;
	}
	int y = (a_BlockY > 0) ? 1 : 0;  // Relative y-coord of this block in Area
	
	NIBBLETYPE MyMeta = Area.GetRelBlockMeta(1, y, 1);
	if (!IsAnyFluidBlock(Area.GetRelBlockType(1, y, 1)))
	{
		// Can happen - if a block is scheduled for simulating and gets replaced in the meantime.
		return;
	}

	if (MyMeta != 0)
	{
		if (CheckTributaries(a_BlockX, a_BlockY, a_BlockZ, Area, MyMeta))
		{
			return;
		}
	}
	
	// New meta for the spreading to neighbors:
	// If this is a source block or was falling, the new meta is just the falloff
	// Otherwise it is the current meta plus falloff (may be larger than max height, will be checked later)
	NIBBLETYPE NewMeta = ((MyMeta == 0) || ((MyMeta & 0x08) != 0)) ? m_Falloff : (MyMeta + m_Falloff);
	
	BLOCKTYPE Below = Area.GetRelBlockType(1, 0, 1);
	if ((a_BlockY > 0) && IsPassableForFluid(Below))
	{
		// Spread only down, possibly washing away what's there:
		SpreadToNeighbor(a_BlockX, a_BlockY - 1, a_BlockZ, Area, 8);
	}
	else if (NewMeta < 8)   // Can reach there
	{
		// Spread to the neighbors:
		SpreadToNeighbor(a_BlockX - 1, a_BlockY, a_BlockZ,     Area, NewMeta);
		SpreadToNeighbor(a_BlockX + 1, a_BlockY, a_BlockZ,     Area, NewMeta);
		SpreadToNeighbor(a_BlockX,     a_BlockY, a_BlockZ - 1, Area, NewMeta);
		SpreadToNeighbor(a_BlockX,     a_BlockY, a_BlockZ + 1, Area, NewMeta);
	}
	
	// Mark as processed:
	m_World->FastSetBlock(a_BlockX, a_BlockY, a_BlockZ, m_StationaryFluidBlock, MyMeta);
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

	// If not fed from above, check if there's a feed from the side:
	if (!IsFed)
	{
		IsFed = (
			(IsAnyFluidBlock(a_Area.GetRelBlockType(0, y, 1)) && IsHigherMeta(a_Area.GetRelBlockMeta(0, y, 1), a_MyMeta)) ||
			(IsAnyFluidBlock(a_Area.GetRelBlockType(2, y, 1)) && IsHigherMeta(a_Area.GetRelBlockMeta(2, y, 1), a_MyMeta)) ||
			(IsAnyFluidBlock(a_Area.GetRelBlockType(1, y, 0)) && IsHigherMeta(a_Area.GetRelBlockMeta(1, y, 0), a_MyMeta)) ||
			(IsAnyFluidBlock(a_Area.GetRelBlockType(1, y, 2)) && IsHigherMeta(a_Area.GetRelBlockMeta(1, y, 2), a_MyMeta))
		);
	}
	
	// If not fed, decrease by m_Falloff levels:
	if (!IsFed)
	{
		a_MyMeta += m_Falloff;
		if (a_MyMeta < 8)
		{
			m_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_StationaryFluidBlock, a_MyMeta);
		}
		else
		{
			m_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_AIR, 0);
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
	
	if (IsAnyFluidBlock(Block))
	{
		NIBBLETYPE Meta = a_Area.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		if (!IsHigherMeta(a_NewMeta, Meta))
		{
			// Don't spread there, there's already a higher level there
			return;
		}
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
			Handler->DropBlock(m_World, a_BlockX, a_BlockY, a_BlockZ);
		}
	}
	
	// Spread:
	m_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, m_FluidBlock, a_NewMeta);
}




