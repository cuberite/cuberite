
// LuaChunkStay.h

// Declares the cLuaChunkStay class representing a cChunkStay binding for plugins, used by cWorld:ChunkStay() Lua API





#pragma once

#include "LuaState.h"
#include "../ChunkStay.h"





// fwd:
class cChunkMap;





class cLuaChunkStay:
	public cChunkStay
{
	using Super = cChunkStay;

public:
	cLuaChunkStay();

	virtual ~cLuaChunkStay() override { }

	/** Adds chunks in the specified Lua table.
	Can be called only once.
	Returns true if any chunk added, false (plus log warning) if none. */
	bool AddChunks(const cLuaState::cStackTable & a_ChunkCoords);

	/** Enables the ChunkStay for the specified chunkmap, with the specified Lua callbacks. */
	void Enable(cChunkMap & a_ChunkMap, cLuaState::cCallbackPtr a_OnChunkAvailable, cLuaState::cCallbackPtr a_OnAllChunksAvailable);

protected:
	/** The Lua function to call in OnChunkAvailable. Only valid when enabled. */
	cLuaState::cCallbackPtr m_OnChunkAvailable;

	/** The Lua function to call in OnAllChunksAvailable. Only valid when enabled. */
	cLuaState::cCallbackPtr m_OnAllChunksAvailable;


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





