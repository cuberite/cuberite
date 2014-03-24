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

#pragma once

#include "BlockRedstone.h"
#include "BlockTorch.h"





class cBlockRedstoneTorchHandler :
	public cBlockTorchHandler
{
public:
	cBlockRedstoneTorchHandler(BLOCKTYPE a_BlockType)
		: cBlockTorchHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Always drop the ON torch, meta 0
		a_Pickups.push_back(cItem(E_BLOCK_REDSTONE_TORCH_ON, 1, 0));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




