
// LuaScript.cpp

// Implements the cLuaScript class that loads a Lua script file to produce a web template out of it

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "LuaScript.h"
#include "tolua++.h"





cLuaScript::cLuaScript()
	: m_LuaState("cLuaScript")
{

}





void cLuaScript::Initialize()
{
	// Check to see if this script has not been initialized before
	ASSERT(!m_LuaState.IsValid());

	// Create a Lua state and bind all libraries to it
	m_LuaState.Create();
}





bool cLuaScript::LoadFile(const char * a_FilePath)
{
	// Make sure the plugin is initialized
	ASSERT(m_LuaState.IsValid());

	return m_LuaState.LoadFile(a_FilePath);
}





bool cLuaScript::CallShowPage(cWebAdmin & a_WebAdmin, HTTPTemplateRequest & a_Request, AString & a_ReturnedString)
{
	ASSERT(m_LuaState.IsValid());
	
	m_LuaState.PushFunction("ShowPage");
	m_LuaState.PushUserType(&a_WebAdmin, "cWebAdmin");
	m_LuaState.PushUserType(&a_Request, "HTTPTemplateRequest");
	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}
	if (lua_isstring(m_LuaState, -1))
	{
		a_ReturnedString.assign(tolua_tostring(m_LuaState, -1, ""));
	}
	lua_pop(m_LuaState, 1);
	return true;
}




