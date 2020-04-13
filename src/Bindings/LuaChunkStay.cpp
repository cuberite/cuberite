
// LuaChunkStay.cpp

// Implements the cLuaChunkStay class representing a cChunkStay binding for plugins, used by cWorld:ChunkStay() Lua API

#include "Globals.h"
#include "LuaChunkStay.h"
#include "PluginLua.h"





cLuaChunkStay::cLuaChunkStay()
{
}





bool cLuaChunkStay::AddChunks(const cLuaState::cStackTable & a_ChunkCoordsTable)
{
	// This function is expected to be called just once, with all the coords in a table
	ASSERT(m_Chunks.empty());

	// Add each set of coords:
	a_ChunkCoordsTable.ForEachArrayElement([=](cLuaState & a_LuaState, int a_Index)
		{
			if (!lua_istable(a_LuaState, -1))
			{
				LOGWARNING("%s: Element #%d is not a table (got %s). Ignoring the element.",
					__FUNCTION__, a_Index, lua_typename(a_LuaState, -1)
				);
				a_LuaState.LogStackTrace();
				lua_pop(a_LuaState, 1);
				return false;
			}
			AddChunkCoord(a_LuaState, a_Index);
			return false;
		}
	);

	// If there are no chunks, log a warning and return failure:
	if (m_Chunks.empty())
	{
		LOGWARNING("%s: No valid chunk coords.", __FUNCTION__);
		a_ChunkCoordsTable.GetLuaState().LogStackTrace();
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

	m_Chunks.push_back(cChunkCoords(ChunkX, ChunkZ));
}





void cLuaChunkStay::Enable(cChunkMap & a_ChunkMap, cLuaState::cCallbackPtr a_OnChunkAvailable, cLuaState::cCallbackPtr a_OnAllChunksAvailable)
{
	m_OnChunkAvailable = std::move(a_OnChunkAvailable);
	m_OnAllChunksAvailable = std::move(a_OnAllChunksAvailable);

	// Enable the ChunkStay:
	Super::Enable(a_ChunkMap);
}





void cLuaChunkStay::OnChunkAvailable(int a_ChunkX, int a_ChunkZ)
{
	if (m_OnChunkAvailable != nullptr)
	{
		m_OnChunkAvailable->Call(a_ChunkX, a_ChunkZ);
	}
}





bool cLuaChunkStay::OnAllChunksAvailable(void)
{
	if (m_OnAllChunksAvailable != nullptr)
	{
		// Call the callback:
		m_OnAllChunksAvailable->Call();

		// Remove the callback references - they won't be needed anymore
		m_OnChunkAvailable.reset();
		m_OnAllChunksAvailable.reset();
	}

	// Disable the ChunkStay by returning true
	return true;
}





void cLuaChunkStay::OnDisabled(void)
{
	// This object is no longer needed, delete it
	delete this;
}




