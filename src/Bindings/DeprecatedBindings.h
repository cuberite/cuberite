#pragma once

struct lua_State;
class DeprecatedBindings
{
public:
	static void Bind( lua_State* a_tolua_S);
};
