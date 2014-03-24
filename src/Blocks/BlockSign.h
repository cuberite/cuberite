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
#include "../Entities/Player.h"
#include "Chunk.h"





class cBlockSignHandler :
	public cBlockHandler
{
public:
	cBlockSignHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SIGN, 1, 0));
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}


	static char RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 180 + (180 / 16);  // So it's not aligned with axis
		if (a_Rotation > 360)
		{
			a_Rotation -= 360;
		}
		
		a_Rotation = (a_Rotation / 360) * 16;
		
		return ((char)a_Rotation) % 16;
	}
	
	
	static char DirectionToMetaData(eBlockFace a_Direction)
	{
		switch (a_Direction)
		{
			case 0x2: return 0x2;
			case 0x3: return 0x3;
			case 0x4: return 0x4;
			case 0x5: return 0x5;
			default:
			{
				break;
			}
		}
		return 0x2;
	}	


	virtual void OnPlacedByPlayer(
		cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta
	) override
	{
		a_Player->GetClientHandle()->SendEditSign(a_BlockX, a_BlockY, a_BlockZ);
	}
} ;




