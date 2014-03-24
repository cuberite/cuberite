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

// NoopFluidSimulator.h

// Declares the cNoopFluidSimulator class representing a fluid simulator that performs nothing, it ignores all blocks





#pragma once

#include "FluidSimulator.h"





class cNoopFluidSimulator :
	public cFluidSimulator
{
	typedef cFluidSimulator super;
	
public:
	cNoopFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid) :
		super(a_World, a_Fluid, a_StationaryFluid)
	{
	}

	// cSimulator overrides:
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override 
	{
		UNUSED(a_BlockX);
		UNUSED(a_BlockY);
		UNUSED(a_BlockZ);
		UNUSED(a_Chunk);
	}
	virtual void Simulate(float a_Dt) override { UNUSED(a_Dt);}
} ;




