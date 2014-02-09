
// LuaChunkStay.cpp

// Implements the cLuaChunkStay class representing a cChunkStay binding for plugins

#include "Globals.h"
#include "LuaChunkStay.h"
#include "../World.h"





void cLuaChunkStay::Enable(
	cWorld & a_World, cLuaState & a_LuaState,
	const cLuaState::cRef & a_OnChunkAvailable, const cLuaState::cRef & a_OnAllChunksAvailable
)
{
	if (m_LuaState != NULL)
	{
		LOGWARNING("LuaChunkStay: Already enabled. Ignoring this call.");
		a_LuaState.LogStackTrace();
		return;
	}
	m_LuaState = &a_LuaState;
	m_OnChunkAvailable = a_OnAllChunksAvailable;
	m_OnAllChunksAvailable = a_OnAllChunksAvailable;
	super::Enable(*a_World.GetChunkMap());
}





void cLuaChunkStay::OnChunkAvailable(int a_ChunkX, int a_ChunkZ)
{
	m_LuaState->Call(m_OnChunkAvailable, a_ChunkX, a_ChunkZ);
}





void cLuaChunkStay::OnAllChunksAvailable(void)
{
	m_LuaState->Call(m_OnAllChunksAvailable);
}





