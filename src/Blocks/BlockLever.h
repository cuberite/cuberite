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





class cBlockLeverHandler :
	public cBlockHandler
{
public:
	cBlockLeverHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		// Flip the ON bit on/off using the XOR bitwise operation
		NIBBLETYPE Meta = (a_ChunkInterface.GetBlockMeta(a_BlockX, a_BlockY, a_BlockZ) ^ 0x08);

		a_ChunkInterface.SetBlock(a_WorldInterface, a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_LEVER, Meta); // SetMeta doesn't work for unpowering levers, so setblock
		a_WorldInterface.GetBroadcastManager().BroadcastSoundEffect("random.click", a_BlockX * 8, a_BlockY * 8, a_BlockZ * 8, 0.5f, (Meta & 0x08) ? 0.6f : 0.5f);
	}


	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// Reset meta to 0
		a_Pickups.push_back(cItem(E_BLOCK_LEVER, 1, 0));
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}
	
	
	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override
	{
		a_BlockType = m_BlockType;
		a_BlockMeta = LeverDirectionToMetaData(a_BlockFace);
		return true;
	}


	inline static NIBBLETYPE LeverDirectionToMetaData(eBlockFace a_Dir)
	{
		// Determine lever direction:
		switch (a_Dir)
		{
			case BLOCK_FACE_YP: return 0x6;
			case BLOCK_FACE_XP: return 0x1;
			case BLOCK_FACE_XM: return 0x2;
			case BLOCK_FACE_ZP: return 0x3;
			case BLOCK_FACE_ZM: return 0x4;
			case BLOCK_FACE_YM: return 0x0;
			default:            return 0x6;
		}
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}


	inline static eBlockFace BlockMetaDataToBlockFace(NIBBLETYPE a_Meta)
	{
		switch (a_Meta & 0x7)
		{
			case 0x1: return BLOCK_FACE_XP;
			case 0x2: return BLOCK_FACE_XM;
			case 0x3: return BLOCK_FACE_ZP;
			case 0x4: return BLOCK_FACE_ZM;
			case 0x5:
			case 0x6: return BLOCK_FACE_YP;
			case 0x7:
			case 0x0: return BLOCK_FACE_YM;
			default:
			{
				ASSERT(!"Unhandled block meta!");
				return BLOCK_FACE_NONE;
			}
		}
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		NIBBLETYPE Meta;
		a_Chunk.UnboundedRelGetBlockMeta(a_RelX, a_RelY, a_RelZ, Meta);

		AddFaceDirection(a_RelX, a_RelY, a_RelZ, BlockMetaDataToBlockFace(Meta), true);
		BLOCKTYPE BlockIsOn; a_Chunk.UnboundedRelGetBlockType(a_RelX, a_RelY, a_RelZ, BlockIsOn);

		return (a_RelY > 0) && cBlockInfo::IsSolid(BlockIsOn);
	}
} ;




