#define LUA_USE_POSIX
#include "cMCLogger.h"
#include <string>
#include "cPlugin_Lua.h"
#include "cPluginManager.h"
#include "cRoot.h"

extern "C"
{
	#include "lualib.h"
}

#include "tolua++.h"
#include "Bindings.h"
#include "ManualBindings.h"

bool report_errors(lua_State* lua, int status)
{
	if ( status!=0 )
	{
		std::string s = lua_tostring(lua, -1);
		LOGERROR("-- %s", s.c_str() );
		lua_pop(lua, 1);
		return true;
	}
	return false;
}

cPlugin_Lua::~cPlugin_Lua()
{
	UnloadPlugins();
	if( m_LuaState )
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
	}
}

cPlugin_Lua::cPlugin_Lua(const char* a_Plugin)
:	m_LuaState( 0 )
{
	m_FileName.assign( a_Plugin );
}

bool cPlugin_Lua::Initialize()
{
	if( !m_LuaState ) 
	{	
		m_LuaState = lua_open();
		luaL_openlibs( m_LuaState );
		tolua_AllToLua_open(m_LuaState);
		ManualBindings::Bind( m_LuaState );
	}

	int s = luaL_loadfile(m_LuaState, (std::string("Plugins/") + m_FileName ).c_str() );
	if( report_errors( m_LuaState, s ) )
	{
		LOGERROR("Can't load plugin %s", m_FileName.c_str() );
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	s = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
	if( report_errors( m_LuaState, s ) )
	{
		LOGERROR("Error in plugin %s", m_FileName.c_str() );
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}
	return true;
}

void cPlugin_Lua::AddPlugin( cPlugin* a_Plugin )
{
	m_Plugins.push_back( a_Plugin );
}

void cPlugin_Lua::RemovePlugin( cPlugin* a_Plugin )
{
	m_Plugins.remove( a_Plugin );
	cRoot::Get()->GetPluginManager()->RemovePlugin( a_Plugin, true );
}

void cPlugin_Lua::UnloadPlugins()
{
	while( m_Plugins.begin() != m_Plugins.end() )
	{
		RemovePlugin( *m_Plugins.begin() );
	}
}

lua_State* cPlugin_Lua::GetLuaState()
{
	return m_LuaState;
}