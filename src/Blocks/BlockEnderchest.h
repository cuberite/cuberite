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

#include "BlockEntity.h"
#include "MetaRotater.h"




class cBlockEnderchestHandler :
	public cMetaRotater<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>
{
public:
	cBlockEnderchestHandler(BLOCKTYPE a_BlockType)
		: cMetaRotater<cBlockEntityHandler, 0x07, 0x02, 0x05, 0x03, 0x04>(a_BlockType)
	{
	}

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		//todo: Drop Ender Chest if using silk touch pickaxe
		a_Pickups.push_back(cItem(E_BLOCK_OBSIDIAN, 8, 0));
	}

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace,
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
		) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = RotationToMetaData(a_Player->GetYaw());
		return true;
	}

	virtual const char * GetStepSound(void) override
	{
		return "step.stone";
	}
	
	static NIBBLETYPE RotationToMetaData(double a_Rotation)
	{
		a_Rotation += 90 + 45;  // So its not aligned with axis

		if (a_Rotation > 360.f)
		{
			a_Rotation -= 360.f;
		}
		if ((a_Rotation >= 0.f) && (a_Rotation < 90.f))
		{
			return 0x4;
		}
		else if ((a_Rotation >= 180) && (a_Rotation < 270))
		{
			return 0x5;
		}
		else if ((a_Rotation >= 90) && (a_Rotation < 180))
		{
			return 0x2;
		}
		else
		{
			return 0x3;
		}
	}
} ;




