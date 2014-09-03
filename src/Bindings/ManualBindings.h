#pragma once

struct lua_State;





/** Provides namespace for the bindings. */
class ManualBindings
{
public:
	/** Binds all the manually implemented functions to tolua_S. */
	static void Bind(lua_State * tolua_S);
	
protected:
	/** Binds the manually implemented cRankManager glue code to tolua_S.
	Implemented in ManualBindings_RankManager.cpp. */
	static void BindRankManager(lua_State * tolua_S);
};




