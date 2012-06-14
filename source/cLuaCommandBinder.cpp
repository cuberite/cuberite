
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cLuaCommandBinder.h"
#include "cPlayer.h"
#include "cPlugin.h"
#include "cPlugin_Lua.h"

#include "tolua++.h"





extern bool report_errors(lua_State* lua, int status);

cLuaCommandBinder::cLuaCommandBinder()
{
}

cLuaCommandBinder::~cLuaCommandBinder()
{
}

void cLuaCommandBinder::ClearBindings()
{
	m_BoundCommands.clear();
}

void cLuaCommandBinder::RemoveBindingsForPlugin( cPlugin* a_Plugin )
{
	for( CommandMap::iterator itr = m_BoundCommands.begin(); itr != m_BoundCommands.end(); )
	{
		if( itr->second.Plugin == a_Plugin )
		{
			LOGINFO("Unbinding %s ", itr->first.c_str( ) );
			luaL_unref( itr->second.LuaState, LUA_REGISTRYINDEX, itr->second.Reference ); // unreference
			CommandMap::iterator eraseme = itr;
			++itr;
			m_BoundCommands.erase( eraseme );
			continue;
		}
		++itr;
	}
}

bool cLuaCommandBinder::BindCommand( const std::string & a_Command, const std::string & a_Permission, cPlugin* a_Plugin, lua_State * a_LuaState, int a_FunctionReference )
{
	if( !a_Plugin->CanBindCommands() )
	{
		LOGERROR("ERROR: Trying to bind command \"%s\" to a plugin that is not initialized.", a_Command.c_str() );
		return false;
	}
	if( m_BoundCommands.find( a_Command ) != m_BoundCommands.end() )
	{
		LOGERROR("ERROR: Trying to bind command \"%s\" that has already been bound.", a_Command.c_str() );
		return false;
	}
	LOGINFO("Binding %s (%s)", a_Command.c_str(), a_Permission.c_str() );
	m_BoundCommands[ a_Command ] = BoundFunction( a_Plugin, a_LuaState, a_FunctionReference, a_Permission );
	return true;
}

bool cLuaCommandBinder::HandleCommand( const std::string & a_Command, cPlayer* a_Player )
{
	AStringVector Split = StringSplit(a_Command, " ");
	if (Split.size() == 0)
	{
		return false;
	}
	
	CommandMap::iterator FoundCommand = m_BoundCommands.find( Split[0] );
	if( FoundCommand != m_BoundCommands.end() )
	{
		const BoundFunction & func = FoundCommand->second;
		if( func.Permission.size() > 0 )
		{
			if( !a_Player->HasPermission( func.Permission.c_str() ) )
			{
				return false;
			}
		}

		// For enabling 'self' in the function, it's kind of a hack I'm not sure this is the way to go
		lua_pushvalue(func.LuaState, LUA_GLOBALSINDEX);
			lua_pushstring(func.LuaState, "self");
			tolua_pushusertype( func.LuaState, func.Plugin, "cPlugin" );
			lua_rawset(func.LuaState, -3);
		lua_pop(func.LuaState, 1);
		
		LOGINFO("1. Stack size: %i", lua_gettop(func.LuaState) );
		lua_rawgeti( func.LuaState, LUA_REGISTRYINDEX, func.Reference); // same as lua_getref()

		// Push the split
		LOGINFO("2. Stack size: %i", lua_gettop(func.LuaState) );
		lua_createtable(func.LuaState, Split.size(), 0);
		int newTable = lua_gettop(func.LuaState);
		int index = 1;
		std::vector<std::string>::const_iterator iter = Split.begin();
		while(iter != Split.end()) {
			tolua_pushstring( func.LuaState, (*iter).c_str() );
			lua_rawseti(func.LuaState, newTable, index);
			++iter;
			++index;
		}
		LOGINFO("3. Stack size: %i", lua_gettop(func.LuaState) );
		// Push player
		tolua_pushusertype( func.LuaState, a_Player, "cPlayer" );
		LOGINFO("Calling bound function! :D");
		int s = lua_pcall(func.LuaState, 2, 1, 0);
		if( report_errors( func.LuaState, s ) )
		{
			LOGINFO("error. Stack size: %i", lua_gettop(func.LuaState) );
			return false;
		}
		bool RetVal = (tolua_toboolean(func.LuaState, -1, 0) > 0);
		lua_pop(func.LuaState, 1); // Pop return value
		LOGINFO("ok. Stack size: %i", lua_gettop(func.LuaState) );
		return RetVal;
	}
	return false;
}
