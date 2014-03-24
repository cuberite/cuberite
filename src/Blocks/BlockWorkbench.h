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
#include "../UI/Window.h"
#include "../Entities/Player.h"





class cBlockWorkbenchHandler:
	public cBlockHandler
{
public:
	cBlockWorkbenchHandler(BLOCKTYPE a_BlockType)
		: cBlockHandler(a_BlockType)
	{
	}
	
	
	virtual void OnUse(cChunkInterface & a_ChunkInterface, cWorldInterface & a_WorldInterface, cPlayer * a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) override
	{
		cWindow * Window = new cCraftingWindow(a_BlockX, a_BlockY, a_BlockZ);
		a_Player->OpenWindow(Window);
	}


	virtual bool IsUseable(void) override
	{
		return true;
	}


	virtual const char * GetStepSound(void) override
	{
		return "step.wood";
	}
} ;




