#pragma once

struct lua_State;
class DeprecatedBindings
{
public:
	static void Bind( lua_State* tolua_S);
};
