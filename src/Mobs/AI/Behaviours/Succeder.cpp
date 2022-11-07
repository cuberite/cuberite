
#include "Succeder.h"

namespace BehaviorTree
{

cSucceeder::cSucceeder(cLuaState & a_LuaState)
{
	lua_pushnil(a_LuaState);
	lua_next(a_LuaState, -2);

	lua_pushvalue(a_LuaState, -2);
	auto Name = lua_tostring(a_LuaState, -1);
	a_LuaState.Pop(1);

	m_Child = CreateBehaviour(Name, a_LuaState);

	a_LuaState.Pop(2);
}
}
