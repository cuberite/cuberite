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

// VanillaFluidSimulator.h





#pragma once

#include "FloodyFluidSimulator.h"





// fwd:
class cBlockArea;





class cVanillaFluidSimulator :
	public cFloodyFluidSimulator
{
	typedef cFloodyFluidSimulator super;
	
public:
	cVanillaFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid, NIBBLETYPE a_Falloff, int a_TickDelay, int a_NumNeighborsForSource);
	
protected:
	// cFloodyFluidSimulator overrides:
	virtual void Spread(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_NewMeta) override;

	/** Recursively calculates the minimum number of blocks needed to descend a level. */
	int CalculateFlowCost(cChunk * a_Chunk, int a_RelX, int a_RelY, int a_RelZ, Direction a_Dir, unsigned a_Iteration = 0);

} ;




