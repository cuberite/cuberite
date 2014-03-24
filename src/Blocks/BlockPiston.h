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





class cBlockPistonHandler :
	public cBlockHandler
{
public:
	cBlockPistonHandler(BLOCKTYPE a_BlockType);
	
	virtual void OnDestroyed(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual bool GetPlacementBlockTypeMeta(
		cChunkInterface & a_ChunkInterface, cPlayer * a_Player,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, 
		int a_CursorX, int a_CursorY, int a_CursorZ,
		BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta
	) override;
} ;





class cBlockPistonHeadHandler :
	public cBlockHandler
{
	typedef cBlockHandler super;
	
public:
	cBlockPistonHeadHandler(void);
	
	virtual void OnDestroyedByPlayer(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ) override;

	virtual void ConvertToPickups(cItems & a_Pickups, NIBBLETYPE a_BlockMeta) override
	{
		// No pickups
		// Also with 1.7, the item forms of these tecnical blocks have been removed, so giving someone this will crash their client...
	}
} ;




