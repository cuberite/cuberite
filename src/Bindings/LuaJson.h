
// LuaJson.h

// Declares the Json exposure bindings to Lua





#pragma once





// fwd:
class cLuaState;





class cLuaJson
{
public:
	/** Registers the Json library in the specified Lua state. */
	static void Bind(cLuaState & a_LuaState);
};




