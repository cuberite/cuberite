#define LUA_USE_POSIX
#include "cPlugin_NewLua.h"
#include "cMCLogger.h"

extern "C"
{
#include "lualib.h"
}

#include "tolua++.h"
#include "Bindings.h"
#include "ManualBindings.h"

extern bool report_errors(lua_State* lua, int status);

cPlugin_NewLua::cPlugin_NewLua( const char* a_PluginName )
	: m_LuaState( 0 )
{
	m_Directory = a_PluginName;
}

cPlugin_NewLua::~cPlugin_NewLua()
{
	if( m_LuaState )
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
	}
}

bool cPlugin_NewLua::Initialize()
{
	if( !m_LuaState ) 
	{	
		m_LuaState = lua_open();
		luaL_openlibs( m_LuaState );
		tolua_AllToLua_open(m_LuaState);
		ManualBindings::Bind( m_LuaState );
	}

	std::string PluginFiles[] = {	 std::string("Plugins/") + m_Directory + "/tick.lua"
									,std::string("Plugins/") + m_Directory + "/main.lua" };


	// Load all files for this plugin, and execute them
	for( int i = 0; i < 2; ++i )
	{
		int s = luaL_loadfile(m_LuaState, PluginFiles[i].c_str() );
		if( report_errors( m_LuaState, s ) )
		{
			LOGERROR("Can't load plugin %s because of an error in file %s", m_Directory.c_str(), PluginFiles[i].c_str() );
			lua_close( m_LuaState );
			m_LuaState = 0;
			return false;
		}

		s = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
		if( report_errors( m_LuaState, s ) )
		{
			LOGERROR("Error in plugin %s in file %s", m_Directory.c_str(), PluginFiles[i].c_str() );
			lua_close( m_LuaState );
			m_LuaState = 0;
			return false;
		}
	}


	// Call intialize function
	lua_getglobal(m_LuaState, "Initialize");
	if(!lua_isfunction(m_LuaState,-1))
	{
		LOGWARN("Error in plugin %s: Could not find function Initialize()", m_Directory.c_str() );
		lua_pop(m_LuaState,1);
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	tolua_pushusertype(m_LuaState, this, "cPlugin_NewLua");

	// do the call (1 arguments, 1 result)
	int s = lua_pcall(m_LuaState, 1, 1, 0);
	if( report_errors( m_LuaState, s ) ) 
	{
		LOGWARN("Error in plugin %s calling function Initialize()", m_Directory.c_str() );
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	if( !lua_isboolean( m_LuaState, -1 ) )
	{
		LOGWARN("Error in plugin %s Initialize() must return a boolean value!", m_Directory.c_str() );
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	bool bSuccess = (tolua_toboolean( m_LuaState, -1, 0) > 0);
	return bSuccess;
}

void cPlugin_NewLua::Tick(float a_Dt)
{
	lua_getglobal(m_LuaState, "Tick");
	if(!lua_isfunction(m_LuaState,-1))
	{
		LOGWARN("Error in plugin %s: Could not find function Tick()", m_Directory.c_str() );
		lua_pop(m_LuaState,1);
		return;
	}

	tolua_pushnumber( m_LuaState, a_Dt );

	// do the call (1 arguments, 0 result)/
	int s = lua_pcall(m_LuaState, 1, 0, 0);
	if( report_errors( m_LuaState, s ) )
	{
		LOGWARN("Error in plugin %s calling function Tick()", m_Directory.c_str() );
	}
}