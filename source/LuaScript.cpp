
// LuaScript.cpp

// Implements the cLuaScript class that loads a Lua script file to produce a web template out of it

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LuaScript.h"

extern "C"
{
#include "lualib.h"
}

#include "tolua++.h"
#include "Bindings.h"
#include "ManualBindings.h"

// fwd: SQLite/lsqlite3.c
extern "C"
{
	LUALIB_API int luaopen_lsqlite3(lua_State * L);
}

// fwd: LuaExpat/lxplib.c:
extern "C"
{
	int luaopen_lxp(lua_State * L);
}





cLuaScript::cLuaScript()
	: m_LuaState(NULL)
{

}





cLuaScript::~cLuaScript()
{
	if( m_LuaState )
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
	}
}





void cLuaScript::Initialize()
{
	// Check to see if this script has not been initialized before
	ASSERT(!m_LuaState);

	// Create a Lua state and bind all libraries to it
	m_LuaState = lua_open();
	luaL_openlibs(m_LuaState);
	tolua_AllToLua_open(m_LuaState);
	ManualBindings::Bind(m_LuaState);
	luaopen_lsqlite3(m_LuaState);
	luaopen_lxp(m_LuaState);
}





bool cLuaScript::LoadFile( const char* a_FilePath )
{
	// Make sure the plugin is initialized
	ASSERT(m_LuaState);

	// Load the file into the Lua state
	int s = luaL_loadfile(m_LuaState, a_FilePath );
	if (ReportErrors(s))
	{
		return false;
	}
	return true;
}





bool cLuaScript::Execute()
{
	// Make sure we got a Lua state
	ASSERT(m_LuaState);

	// Execute the script as it is right now
	int s = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
	if( ReportErrors( s ) )
	{
		return false;
	}
	return true;
}





bool cLuaScript::ReportErrors( int a_Status )
{
	if (a_Status == 0)
	{
		// No error to report
		return false;
	}

	// Status was set to error so get the error from the Lua state and log it
	LOGERROR("LUA: %s", lua_tostring(m_LuaState, -1));
	lua_pop(m_LuaState, 1);

	// Return true to indicate that an error was returned
	return true;
}





bool cLuaScript::LuaPushFunction( const char * a_FunctionName, bool a_bLogError /*= true*/ )
{
	ASSERT(m_LuaState);

	// Find and push the function on the Lua stack
	lua_getglobal(m_LuaState, a_FunctionName);

	// Make sure we found a function
	if (!lua_isfunction(m_LuaState, -1))
	{
		if (a_bLogError)
		{
			LOGWARN("LUA: Could not find function %s()", a_FunctionName);
		}

		// Pop the pushed 'object' back
		lua_pop(m_LuaState, 1);
		return false;
	}

	// Successfully pushed a function to the Lua stack
	return true;
}





bool cLuaScript::LuaCallFunction( int a_NumArgs, int a_NumResults, const char * a_FunctionName )
{
	ASSERT(m_LuaState);

	// Make sure there's a lua function on the stack
	ASSERT(lua_isfunction(m_LuaState, -a_NumArgs - 1));

	// Call the desired function
	int s = lua_pcall(m_LuaState, a_NumArgs, a_NumResults, 0);

	// Check for errors
	if (ReportErrors(s))
	{
		LOGWARN("LUA: Error calling function %s()", a_FunctionName);
		return false;
	}

	// Successfully executed function
	return true;
}





bool cLuaScript::CallFunction( const char* a_Function, AString& ReturnedString )
{
	// Make sure we have the required things to call a function
	ASSERT(m_LuaState);
	ASSERT(a_Function);

	// Push the desired function on the stack
	if (!LuaPushFunction(a_Function))
	{
		return false;
	}

	if (!LuaCallFunction(0, 1, a_Function))
	{
		return false;
	}

	if (lua_isstring(m_LuaState, -1))
	{
		ReturnedString = tolua_tostring(m_LuaState, -1, "");
	}
	lua_pop(m_LuaState, 1);
	return true;
}





bool cLuaScript::CallFunction( const char* a_Function, const sLuaUsertype& a_UserType, AString& ReturnedString )
{
	// Make sure we have the required things to call a function
	ASSERT(m_LuaState);
	ASSERT(a_Function);

	// Push the desired function on the stack
	if (!LuaPushFunction(a_Function))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_UserType.Object, a_UserType.ClassName);

	if (!LuaCallFunction(1, 1, a_Function))
	{
		return false;
	}

	if (lua_isstring(m_LuaState, -1))
	{
		ReturnedString = tolua_tostring(m_LuaState, -1, "");
	}
	lua_pop(m_LuaState, 1);
	return true;
}





bool cLuaScript::CallFunction( const char* a_Function, const sLuaUsertype& a_UserType1, const sLuaUsertype& a_UserType2, AString& ReturnedString )
{
	// Make sure we have the required things to call a function
	ASSERT(m_LuaState);
	ASSERT(a_Function);

	// Push the desired function on the stack
	if (!LuaPushFunction(a_Function))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_UserType1.Object, a_UserType1.ClassName);
	tolua_pushusertype(m_LuaState, a_UserType2.Object, a_UserType2.ClassName);

	if (!LuaCallFunction(2, 1, a_Function))
	{
		return false;
	}

	if (lua_isstring(m_LuaState, -1))
	{
		ReturnedString = tolua_tostring(m_LuaState, -1, "");
	}
	lua_pop(m_LuaState, 1);
	return true;
}







