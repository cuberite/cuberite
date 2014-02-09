
// LuaChunkStay.h

// Declares the cLuaChunkStay class representing a cChunkStay binding for plugins





#pragma once

#include "LuaState.h"
#include "../ChunkStay.h"





// tolua_begin
class cLuaChunkStay
	: public cChunkStay
{
	typedef cChunkStay super;
	
public:
	// Allow Lua to construct objects of this class:
	cLuaChunkStay(void);
	
	// Allow Lua to garbage-collect objects of this class:
	~cLuaChunkStay() { }

	// tolua_end	
	
	/** Enables the ChunkStay for the specified world, with the specified Lua callbacks.
	Exported in ManualBindings. */
	void Enable(
		cWorld & a_World, lua_State * a_LuaState,
		int a_OnChunkAvailableStackPos, int a_OnAllChunksAvailableStackPos
	);
	
	// tolua_begin
	
	/** Disables the ChunkStay. Cleans up the bound Lua state and callbacks */
	virtual void Disable(void);
	
protected:
	/** The Lua state associated with the callbacks. Only valid when enabled. */
	cLuaState m_LuaState;
	
	/** The Lua function to call in OnChunkAvailable. Only valid when enabled. */
	cLuaState::cRef m_OnChunkAvailable;
	
	/** The Lua function to call in OnAllChunksAvailable. Only valid when enabled. */
	cLuaState::cRef m_OnAllChunksAvailable;
	
	// cChunkStay overrides:
	virtual void OnChunkAvailable(int a_ChunkX, int a_ChunkZ) override;
	virtual void OnAllChunksAvailable(void) override;
} ;

// tolua_end




