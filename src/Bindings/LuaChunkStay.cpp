
// LuaChunkStay.cpp

// Implements the cLuaChunkStay class representing a cChunkStay binding for plugins, used by cWorld:ChunkStay() Lua API

#include "Globals.h"
#include "LuaChunkStay.h"
#include "PluginLua.h"
#include "../World.h"





cLuaChunkStay::cLuaChunkStay(cPluginLua & a_Plugin) :
	m_Plugin(a_Plugin),
	m_LuaState(NULL)
{
}





bool cLuaChunkStay::AddChunks(int a_ChunkCoordTableStackPos)
{
	// This function is expected to be called just once, with all the coords in a table
	ASSERT(m_Chunks.empty());
	
	cPluginLua::cOperation Op(m_Plugin);
	cLuaState & L = Op();
	
	// Check that we got a table:
	if (!lua_istable(L, a_ChunkCoordTableStackPos))
	{
		LOGWARNING("%s: The parameter is not a table of coords (got %s). Ignoring the call.",
			__FUNCTION__, lua_typename(L, lua_type(L, a_ChunkCoordTableStackPos))
		);
		L.LogStackTrace();
		return false;
	}
	
	// Add each set of coords:
	int NumChunks = luaL_getn(L, a_ChunkCoordTableStackPos);
	m_Chunks.reserve((size_t)NumChunks);
	for (int idx = 1; idx <= NumChunks; idx++)
	{
		// Push the idx-th element of the array onto stack top, check that it's a table:
		lua_rawgeti(L, a_ChunkCoordTableStackPos, idx);
		if (!lua_istable(L, -1))
		{
			LOGWARNING("%s: Element #%d is not a table (got %s). Ignoring the element.",
				__FUNCTION__, idx, lua_typename(L, -1)
			);
			L.LogStackTrace();
			lua_pop(L, 1);
			continue;
		}
		AddChunkCoord(L, idx);
		lua_pop(L, 1);
	}
	
	// If there are no chunks, log a warning and return failure:
	if (m_Chunks.empty())
	{
		LOGWARNING("%s: Zero chunks to stay.", __FUNCTION__);
		L.LogStackTrace();
		return false;
	}
	
	// All ok
	return true;
}





void cLuaChunkStay::AddChunkCoord(cLuaState & L, int a_Index)
{
	// Check that the element has 2 coords:
	int NumCoords = luaL_getn(L, -1);
	if (NumCoords != 2)
	{
		LOGWARNING("%s: Element #%d doesn't contain 2 coords (got %d). Ignoring the element.",
			__FUNCTION__, a_Index, NumCoords
		);
		return;
	}
	
	// Read the two coords from the element:
	lua_rawgeti(L, -1, 1);
	lua_rawgeti(L, -2, 2);
	int ChunkX = luaL_checkint(L, -2);
	int ChunkZ = luaL_checkint(L, -1);
	lua_pop(L, 2);
	
	// Check that a coord is not yet present:
	for (cChunkCoordsVector::iterator itr = m_Chunks.begin(), end = m_Chunks.end(); itr != end; ++itr)
	{
		if ((itr->m_ChunkX == ChunkX) && (itr->m_ChunkZ == ChunkZ))
		{
			LOGWARNING("%s: Element #%d is a duplicate, ignoring it.",
				__FUNCTION__, a_Index
			);
			return;
		}
	}  // for itr - m_Chunks[]
	
	m_Chunks.push_back(cChunkCoords(ChunkX, ZERO_CHUNK_Y, ChunkZ));
}





void cLuaChunkStay::Enable(cChunkMap & a_ChunkMap, int a_OnChunkAvailableStackPos, int a_OnAllChunksAvailableStackPos)
{
	// Get the references to the callback functions:
	m_LuaState = &m_Plugin.GetLuaState();
	m_OnChunkAvailable.RefStack(*m_LuaState, a_OnChunkAvailableStackPos);
	m_OnAllChunksAvailable.RefStack(*m_LuaState, a_OnAllChunksAvailableStackPos);
	
	// Enable the ChunkStay:
	super::Enable(a_ChunkMap);
}





void cLuaChunkStay::OnChunkAvailable(int a_ChunkX, int a_ChunkZ)
{
	cPluginLua::cOperation Op(m_Plugin);
	Op().Call((int)m_OnChunkAvailable, a_ChunkX, a_ChunkZ);
}





bool cLuaChunkStay::OnAllChunksAvailable(void)
{
	{
		// Call the callback:
		cPluginLua::cOperation Op(m_Plugin);
		Op().Call((int)m_OnAllChunksAvailable);
		
		// Remove the callback references - they won't be needed anymore
		m_OnChunkAvailable.UnRef();
		m_OnAllChunksAvailable.UnRef();
	}
	
	// Disable the ChunkStay by returning true
	return true;
}





void cLuaChunkStay::OnDisabled(void)
{
	// This object is no longer needed, delete it
	delete this;
}




