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

#include "BlockHandler.h"





class cBlockClothHandler :
	public cBlockHandler
{
public:
	cBlockClothHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_BLOCK_WOOL, 1, a_BlockMeta));
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




