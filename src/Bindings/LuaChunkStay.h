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

// LuaChunkStay.h

// Declares the cLuaChunkStay class representing a cChunkStay binding for plugins, used by cWorld:ChunkStay() Lua API





#pragma once

#include "LuaState.h"
#include "../ChunkStay.h"





// fwd:
class cPluginLua;





class cLuaChunkStay
	: public cChunkStay
{
	typedef cChunkStay super;
	
public:
	cLuaChunkStay(cPluginLua & a_Plugin);
	
	~cLuaChunkStay() { }

	/** Adds chunks in the specified on-stack Lua table.
	Returns true if any chunk added, false (plus log warning) if none. */
	bool AddChunks(int a_ChunkCoordTableStackPos);
	
	/** Enables the ChunkStay for the specified chunkmap, with the specified Lua callbacks. */
	void Enable(cChunkMap & a_ChunkMap, int a_OnChunkAvailableStackPos, int a_OnAllChunksAvailableStackPos);
	
protected:
	/** The plugin which has created the ChunkStay, via cWorld:ChunkStay() binding method.  */
	cPluginLua & m_Plugin;
	
	/** The Lua state associated with the callbacks. Only valid when enabled. */
	cLuaState * m_LuaState;
	
	/** The Lua function to call in OnChunkAvailable. Only valid when enabled. */
	cLuaState::cRef m_OnChunkAvailable;
	
	/** The Lua function to call in OnAllChunksAvailable. Only valid when enabled. */
	cLuaState::cRef m_OnAllChunksAvailable;
	
	
	// cChunkStay overrides:
	virtual void OnChunkAvailable(int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnAllChunksAvailable(void) override;
	virtual void OnDisabled(void) override;
	
	/** Adds a single chunk coord from the table at the top of the Lua stack.
	Expects the top element to be a table, checks that it contains two numbers.
	Uses those two numbers as chunk coords appended to m_Chunks.
	If the coords are already present, gives a warning and ignores the pair.
	The a_Index parameter is only for the error messages. */
	void AddChunkCoord(cLuaState & a_LuaState, int a_Index);
} ;





