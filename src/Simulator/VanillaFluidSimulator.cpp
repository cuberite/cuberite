
// VanillaFluidSimulator.cpp

#include "Globals.h"

#include "VanillaFluidSimulator.h"
#include "../BlockInfo.h"
#include "../World.h"
#include "../Chunk.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"
#include "../Blocks/BlockFluid.h"





static const int InfiniteCost = 100;





cVanillaFluidSimulator::cVanillaFluidSimulator(
		cWorld & a_World,
		BlockType a_Fluid,
		unsigned char a_StationaryFlowValue,
		unsigned char a_Falloff,
		size_t a_TickDelay,
		int a_NumNeighborsForSource
):
	Super(a_World, a_Fluid, a_StationaryFlowValue, a_Falloff, a_TickDelay, a_NumNeighborsForSource)
{
}





void cVanillaFluidSimulator::SpreadXZ(cChunk * a_Chunk, Vector3i a_RelPos, unsigned char a_Falloff)
{
	// Calculate the distance to the nearest "hole" in each direction:
	std::array<int, 4> Cost;
	Cost[0] = CalculateFlowCost(a_Chunk, a_RelPos.addedX(1),  FluidDirection::X_PLUS);
	Cost[1] = CalculateFlowCost(a_Chunk, a_RelPos.addedX(-1), FluidDirection::X_MINUS);
	Cost[2] = CalculateFlowCost(a_Chunk, a_RelPos.addedZ(1),  FluidDirection::Z_PLUS);
	Cost[3] = CalculateFlowCost(a_Chunk, a_RelPos.addedZ(-1), FluidDirection::Z_MINUS);

	// Find the minimum distance:
	int MinCost = InfiniteCost;
	for (unsigned int i = 0; i < Cost.size(); ++i)
	{
		if (Cost[i] < MinCost)
		{
			MinCost = Cost[i];
		}
	}

	// Spread in all directions where the distance matches the minimum:
	if (Cost[0] == MinCost)
	{
		SpreadToNeighbor(a_Chunk, a_RelPos.addedX(1), a_Falloff);
	}
	if (Cost[1] == MinCost)
	{
		SpreadToNeighbor(a_Chunk, a_RelPos.addedX(-1), a_Falloff);
	}
	if (Cost[2] == MinCost)
	{
		SpreadToNeighbor(a_Chunk, a_RelPos.addedZ(1), a_Falloff);
	}
	if (Cost[3] == MinCost)
	{
		SpreadToNeighbor(a_Chunk, a_RelPos.addedZ(-1), a_Falloff);
	}
}





int cVanillaFluidSimulator::CalculateFlowCost(cChunk * a_Chunk, Vector3i a_RelPos, FluidDirection a_Dir, unsigned a_Iteration)
{
	int Cost = InfiniteCost;

	BlockState Self = 0;

	// Check if block is passable
	if (!a_Chunk->UnboundedRelGetBlock(a_RelPos, Self))
	{
		return Cost;
	}
	if (
		!IsPassableForFluid(Self) ||                 // The block cannot be passed by the liquid ...
		(IsAllowedBlock(Self) && (cBlockFluidHandler::GetFalloff(Self) == m_StationaryFalloffValue))  // ... or if it is liquid, it is a source block
	)
	{
		return Cost;
	}

	BlockState BlockBelow = 0;
	// Check if block below is passable
	if ((a_RelPos.y > 0) && !a_Chunk->UnboundedRelGetBlock(a_RelPos.addedY(-1), BlockBelow))
	{
		return Cost;
	}
	if (IsPassableForFluid(BlockBelow) || IsBlockLiquid(BlockBelow))
	{
		// Path found, exit
		return static_cast<int>(a_Iteration);
	}

	// 5 blocks away, bail out
	if (a_Iteration > 3)
	{
		return Cost;
	}

	// Recurse
	if (a_Dir != FluidDirection::X_MINUS)
	{
		int NextCost = CalculateFlowCost(a_Chunk, a_RelPos.addedX(1), FluidDirection::X_PLUS, a_Iteration + 1);
		if (NextCost < Cost)
		{
			Cost = NextCost;
		}
	}
	if (a_Dir != FluidDirection::X_PLUS)
	{
		int NextCost = CalculateFlowCost(a_Chunk, a_RelPos.addedX(-1), FluidDirection::X_MINUS, a_Iteration + 1);
		if (NextCost < Cost)
		{
			Cost = NextCost;
		}
	}
	if (a_Dir != FluidDirection::Z_MINUS)
	{
		int NextCost = CalculateFlowCost(a_Chunk, a_RelPos.addedZ(1), FluidDirection::Z_PLUS, a_Iteration + 1);
		if (NextCost < Cost)
		{
			Cost = NextCost;
		}
	}
	if (a_Dir != FluidDirection::Z_PLUS)
	{
		int NextCost = CalculateFlowCost(a_Chunk, a_RelPos.addedZ(-1), FluidDirection::Z_MINUS, a_Iteration + 1);
		if (NextCost < Cost)
		{
			Cost = NextCost;
		}
	}

	return Cost;
}




