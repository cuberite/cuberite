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





class cBlockSnowHandler :
	public cBlockHandler
{
public:
	cBlockSnowHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
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

		BLOCKTYPE BlockBeforePlacement;
		NIBBLETYPE MetaBeforePlacement;
		a_ChunkInterface.GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, BlockBeforePlacement, MetaBeforePlacement);

		if ((BlockBeforePlacement == E_BLOCK_SNOW) && (MetaBeforePlacement < 7))
		{
			// Only increment if:
			//	A snow block was already there (not first time placement) AND
			//	Height is smaller than 7, the maximum possible height
			MetaBeforePlacement++;
		}
		
		a_BlockMeta = MetaBeforePlacement;
		return true;
	}


	virtual bool DoesIgnoreBuildCollision(cPlayer * a_Player, NIBBLETYPE a_Meta) override
	{
		if ((a_Player->GetEquippedItem().m_ItemType == E_BLOCK_SNOW) && (a_Meta < 7))
		{
			return true; // If a player is holding a (thin) snow block and it's size can be increased, return collision ignored
		}

		if (a_Meta == 0)
		{
			return true; // If at normal snowfall height (lowest), we ignore collision
		}

		return false;
	}
	

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		a_Pickups.push_back(cItem(E_ITEM_SNOWBALL, 1, 0));
	}


	virtual bool CanBeAt(cChunkInterface & a_ChunkInterface, int a_RelX, int a_RelY, int a_RelZ, const cChunk & a_Chunk) override
	{
		if (a_RelY > 0)
		{
			BLOCKTYPE BlockBelow = a_Chunk.GetBlock(a_RelX, a_RelY - 1, a_RelZ);
			NIBBLETYPE MetaBelow = a_Chunk.GetMeta(a_RelX, a_RelY - 1, a_RelZ);
			
			if (cBlockInfo::IsSnowable(BlockBelow) || ((BlockBelow == E_BLOCK_SNOW) && (MetaBelow == 7)))
			{
				// If block below is snowable, or it is a thin slow block and has a meta of 7 (full thin snow block), say yay
				return true;
			}
		}
		
		return false;
	}
	
	
	virtual bool DoesDropOnUnsuitable(void) override
	{
		return false;
	}
	

	virtual const char * GetStepSound(void) override
	{
		return "step.cloth";
	}
} ;




