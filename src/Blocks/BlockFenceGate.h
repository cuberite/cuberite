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
#include "MetaRotater.h"




class cBlockFenceGateHandler :
	public cMetaRotater<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01, true>
{
public:
	cBlockFenceGateHandler(BLOCKTYPE a_BlockType) :
		cMetaRotater<cBlockHandler, 0x03, 0x02, 0x03, 0x00, 0x01, true>(a_BlockType)
	{
	}


	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = PlayerYawToMetaData(a_Player->GetYaw());
		return true;
	}


	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		NIBBLETYPE OldMetaData = a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ);
		NIBBLETYPE NewMetaData = PlayerYawToMetaData(a_Player->GetYaw());
		OldMetaData ^= 4;  // Toggle the gate
		if ((OldMetaData & 1) == (NewMetaData & 1))
		{
			// Standing in front of the gate - apply new direction
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, (OldMetaData & 4) | (NewMetaData & 3));
		}
		else
		{
			// Standing aside - use last direction
			a_ChunkInterface.SetBlockMeta(a_BlockX, a_BlockY, a_BlockZ, OldMetaData);
		}
	}


	virtual void OnCancelRightClick(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace) override
	{
		a_WorldInterface.SendBlockTo(a_BlockX, a_BlockY, a_BlockZ, a_Player);
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}


	/// Converts the player's yaw to placed gate's blockmeta
	inline static NIBBLETYPE PlayerYawToMetaData(double a_Yaw)
	{
		ASSERT((a_Yaw >= -180) && (a_Yaw < 180));
		
		a_Yaw += 360 + 45;
		if (a_Yaw > 360)
		{
			a_Yaw -= 360;
		}
		if ((a_Yaw >= 0) && (a_Yaw < 90))
		{
			return 0x0;
		}
		else if ((a_Yaw >= 180) && (a_Yaw < 270))
		{
			return 0x2;
		}
		else if ((a_Yaw >= 90) && (a_Yaw < 180))
		{
			return 0x1;
		}
		else
		{
			return 0x3;
		}
	}
} ;




