/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// VanillaFluidSimulator.cpp

#include "Globals.h"

#include "VanillaFluidSimulator.h"
#include "../World.h"
#include "../Chunk.h"
#include "../BlockArea.h"
#include "../Blocks/BlockHandler.h"
#include "../BlockInServerPluginInterface.h"





static const int InfiniteCost = 100;





cVanillaFluidSimulator::cVanillaFluidSimulator(
	cWorld & a_World,
	BLOCKTYPE a_Fluid,
	BLOCKTYPE a_StationaryFluid,
	NIBBLETYPE a_Falloff,
	int a_TickDelay,
	int a_NumNeighborsForSource
) : super(a_World, a_Fluid, a_StationaryFluid, a_Falloff, a_TickDelay, a_NumNeighborsForSource)
{
}





void cVanillaFluidSimulator::Spread(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_NewMeta)
{
	int Cost[4];
	Cost[0] = CalculateFlowCost(a_Chunk, a_RelX + 1, a_RelY, a_RelZ,     X_PLUS);
	Cost[1] = CalculateFlowCost(a_Chunk, a_RelX - 1, a_RelY, a_RelZ,     X_MINUS);
	Cost[2] = CalculateFlowCost(a_Chunk, a_RelX,     a_RelY, a_RelZ + 1, Z_PLUS);
	Cost[3] = CalculateFlowCost(a_Chunk, a_RelX,     a_RelY, a_RelZ - 1, Z_MINUS);

	int MinCost = InfiniteCost;
	for (unsigned int i = 0; i < ARRAYCOUNT(Cost); ++i)
	{
		if (Cost[i] < MinCost)
		{
			MinCost = Cost[i];
		}
	}

	if (Cost[0] == MinCost)
	{
		SpreadToNeighbor(a_Chunk, a_RelX + 1, a_RelY, a_RelZ, a_NewMeta);
	}
	if (Cost[1] == MinCost)
	{
		SpreadToNeighbor(a_Chunk, a_RelX - 1, a_RelY, a_RelZ, a_NewMeta);
	}
	if (Cost[2] == MinCost)
	{
		SpreadToNeighbor(a_Chunk, a_RelX, a_RelY, a_RelZ + 1, a_NewMeta);
	}
	if (Cost[3] == MinCost)
	{
		SpreadToNeighbor(a_Chunk, a_RelX, a_RelY, a_RelZ - 1, a_NewMeta);
	}
}





int cVanillaFluidSimulator::CalculateFlowCost(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, Direction a_Dir, unsigned a_Iteration)
{
	int Cost = InfiniteCost;

	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;

	// Check if block is passable
	if (!a_Chunk->UnboundedRelGetBlock(a_RelX, a_RelY, a_RelZ, BlockType, BlockMeta))
	{
		return Cost;
	}
	if (!IsPassableForFluid(BlockType) && !IsBlockLiquid(BlockType))
	{
		return Cost;
	}

	// Check if block below is passable
	if (!a_Chunk->UnboundedRelGetBlock(a_RelX, a_RelY - 1, a_RelZ, BlockType, BlockMeta))
	{
		return Cost;
	}
	if (IsPassableForFluid(BlockType) || IsBlockLiquid(BlockType))
	{
		// Path found, exit
		return a_Iteration;
	}

	// 5 blocks away, bail out
	if (a_Iteration > 3)
	{
		return Cost;
	}

	// Recurse
	if (a_Dir != X_MINUS)
	{
		int NextCost = CalculateFlowCost(a_Chunk, a_RelX + 1, a_RelY, a_RelZ, X_PLUS, a_Iteration + 1);
		if (NextCost < Cost)
		{
			Cost = NextCost;
		}
	}
	if (a_Dir != X_PLUS)
	{
		int NextCost = CalculateFlowCost(a_Chunk, a_RelX - 1, a_RelY, a_RelZ, X_MINUS, a_Iteration + 1);
		if (NextCost < Cost)
		{
			Cost = NextCost;
		}
	}
	if (a_Dir != Z_MINUS)
	{
		int NextCost = CalculateFlowCost(a_Chunk, a_RelX, a_RelY, a_RelZ + 1, Z_PLUS, a_Iteration + 1);
		if (NextCost < Cost)
		{
			Cost = NextCost;
		}
	}
	if (a_Dir != Z_PLUS)
	{
		int NextCost = CalculateFlowCost(a_Chunk, a_RelX, a_RelY, a_RelZ - 1, Z_MINUS, a_Iteration + 1);
		if (NextCost < Cost)
		{
			Cost = NextCost;
		}
	}

	return Cost;
}




