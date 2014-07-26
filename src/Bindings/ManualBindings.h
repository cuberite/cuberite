#pragma once

struct lua_State;
class ManualBindings
{
public:
	static void Bind( lua_State* tolua_S);
};
