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
#include "../MersenneTwister.h"
#include "../World.h"





class cBlockOreHandler :
	public cBlockHandler
{
public:
	cBlockOreHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		short ItemType = m_BlockType;
		char Count = 1;
		short Meta = 0;
		
		MTRand r1;
		switch (m_BlockType)
		{
			case E_BLOCK_LAPIS_ORE:
			{
				ItemType = E_ITEM_DYE;
				Count = 4 + (char)r1.randInt(4);
				Meta = 4;
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				Count = 4 + (char)r1.randInt(1);
				break;
			}
			default:
			{
				Count = 1;
				break;
			}
		}

		switch (m_BlockType)
		{
			case E_BLOCK_DIAMOND_ORE:
			{
				ItemType = E_ITEM_DIAMOND;
				break;
			}
			case E_BLOCK_REDSTONE_ORE:
			case E_BLOCK_REDSTONE_ORE_GLOWING:
			{
				ItemType = E_ITEM_REDSTONE_DUST;
				break;
			}
			case E_BLOCK_EMERALD_ORE:
			{
				ItemType = E_ITEM_EMERALD;
				break;
			}
			case E_BLOCK_COAL_ORE:
			{
				ItemType = E_ITEM_COAL;
				break;
			}
		}
		a_Pickups.push_back(cItem(ItemType, Count, Meta));
	}
} ;




