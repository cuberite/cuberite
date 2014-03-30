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

// VaporizeFluidSimulator.h

// Declares the cVaporizeFluidSimulator class representing a fluid simulator that replaces all fluid blocks with air
// Useful for water simulation in the Nether





#pragma once

#include "FluidSimulator.h"





class cVaporizeFluidSimulator :
	public cFluidSimulator
{
	typedef cFluidSimulator super;
	
public:
	cVaporizeFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid);

	// cSimulator overrides:
	virtual void AddBlock(int a_BlockX, int a_BlockY, int a_BlockZ, cChunk * a_Chunk) override;
	virtual void Simulate(float a_Dt) override;
} ;




