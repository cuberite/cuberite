
// LuaChunkStay.cpp

// Implements the cLuaChunkStay class representing a cChunkStay binding for plugins

#include "Globals.h"
#include "LuaChunkStay.h"
#include "../World.h"





cLuaChunkStay::cLuaChunkStay(void) :
	m_LuaState((lua_State *)NULL)  // Want a detached Lua state by default
{
}





void cLuaChunkStay::Enable(
	cWorld & a_World, lua_State * a_LuaState,
	int a_OnChunkAvailableStackPos, int a_OnAllChunksAvailableStackPos
)
{
	if (m_LuaState.IsValid())
	{
		LOGWARNING("LuaChunkStay: Already enabled. Ignoring this call.");
		cLuaState::LogStackTrace(a_LuaState);
		return;
	}
	m_LuaState.Attach(a_LuaState);
	m_OnChunkAvailable.RefStack(m_LuaState, a_OnChunkAvailableStackPos);
	m_OnAllChunksAvailable.RefStack(m_LuaState, a_OnAllChunksAvailableStackPos);
	super::Enable(*a_World.GetChunkMap());
}





void cLuaChunkStay::Disable(void)
{
	super::Disable();

	// If the state was valid (bound to Lua functions), unbind those functions:
	if (!m_LuaState.IsValid())
	{
		return;
	}
	m_OnAllChunksAvailable.UnRef();
	m_OnChunkAvailable.UnRef();
	m_LuaState.Detach();
}





void cLuaChunkStay::OnChunkAvailable(int a_ChunkX, int a_ChunkZ)
{
	m_LuaState.Call(m_OnChunkAvailable, a_ChunkX, a_ChunkZ);
}





void cLuaChunkStay::OnAllChunksAvailable(void)
{
	m_LuaState.Call(m_OnAllChunksAvailable);
}





