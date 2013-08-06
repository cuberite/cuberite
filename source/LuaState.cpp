
// LuaState.cpp

// Implements the cLuaState class representing the wrapper over lua_State *, provides associated helper functions

#include "Globals.h"
#include "LuaState.h"

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






///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLuaState:

cLuaState::cLuaState(const AString & a_SubsystemName) :
	m_LuaState(NULL),
	m_IsOwned(false),
	m_SubsystemName(a_SubsystemName)
{
}





cLuaState::cLuaState(lua_State * a_AttachState) :
	m_LuaState(a_AttachState),
	m_IsOwned(false),
	m_SubsystemName("<attached>")
{
}





cLuaState::~cLuaState()
{
	if (IsValid())
	{
		Close();
	}
}





void cLuaState::Create(void)
{
	if (m_LuaState != NULL)
	{
		LOGWARNING("%s: Trying to create an already-existing LuaState, ignoring.", __FUNCTION__);
		return;
	}
	m_LuaState = lua_open();
	luaL_openlibs(m_LuaState);
	tolua_AllToLua_open(m_LuaState);
	ManualBindings::Bind(m_LuaState);
	luaopen_lsqlite3(m_LuaState);
	luaopen_lxp(m_LuaState);
	m_IsOwned = true;
}





void cLuaState::Close(void)
{
	if (m_LuaState == NULL)
	{
		LOGWARNING("%s: Trying to close an invalid LuaState, ignoring.", __FUNCTION__);
		return;
	}
	if (!m_IsOwned)
	{
		LOGWARNING(
			"%s: Detected mis-use, calling Close() on an attached state (0x%p). Detaching instead.",
			__FUNCTION__, m_LuaState
		);
		Detach();
		return;
	}
	lua_close(m_LuaState);
	m_LuaState = NULL;
	m_IsOwned = false;
}





void cLuaState::Attach(lua_State * a_State)
{
	if (m_LuaState != NULL)
	{
		LOGINFO("%s: Already contains a LuaState (0x%p), will be closed / detached.", __FUNCTION__, m_LuaState);
		if (m_IsOwned)
		{
			Close();
		}
		else
		{
			Detach();
		}
	}
	m_LuaState = a_State;
	m_IsOwned = false;
}





void cLuaState::Detach(void)
{
	if (m_LuaState == NULL)
	{
		return;
	}
	if (m_IsOwned)
	{
		LOGWARNING(
			"%s: Detected a mis-use, calling Detach() when the state is owned. Closing the owned state (0x%p).",
			__FUNCTION__, m_LuaState
		);
		Close();
		return;
	}
	m_LuaState = NULL;
}





bool cLuaState::LoadFile(const AString & a_FileName)
{
	ASSERT(IsValid());
	
	// Load the file:
	int s = luaL_loadfile(m_LuaState, a_FileName.c_str());
	if (ReportErrors(s))
	{
		LOGWARNING("Can't load %s because of an error in file %s", m_SubsystemName.c_str(), a_FileName.c_str());
		return false;
	}

	// Execute the globals:
	s = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
	if (ReportErrors(s))
	{
		LOGWARNING("Error in %s in file %s", m_SubsystemName.c_str(), a_FileName.c_str());
		return false;
	}
	
	return true;
}





bool cLuaState::PushFunction(const char * a_FunctionName, bool a_ShouldLogFailure /* = true */)
{
	if (!IsValid())
	{
		// This happens if cPlugin::Initialize() fails with an error
		return false;
	}
	
	lua_getglobal(m_LuaState, a_FunctionName);
	if (!lua_isfunction(m_LuaState, -1))
	{
		if (a_ShouldLogFailure)
		{
			LOGWARNING("Error in %s: Could not find function %s()", m_SubsystemName.c_str(), a_FunctionName);
		}
		lua_pop(m_LuaState, 1);
		return false;
	}
	return true;
}





bool cLuaState::PushFunctionFromRegistry(int a_FnRef)
{
	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, a_FnRef);  // same as lua_getref()
	if (!lua_isfunction(m_LuaState, -1))
	{
		lua_pop(m_LuaState, 1);
		return false;
	}
	return true;
}





void cLuaState::PushStringVector(const AStringVector & a_Vector)
{
	lua_createtable(m_LuaState, a_Vector.size(), 0);
	int newTable = lua_gettop(m_LuaState);
	int index = 1;
	for (AStringVector::const_iterator itr = a_Vector.begin(), end = a_Vector.end(); itr != end; ++itr, ++index)
	{
		tolua_pushstring(m_LuaState, itr->c_str());
		lua_rawseti(m_LuaState, newTable, index);
	}
}





bool cLuaState::CallFunction(int a_NumArgs, int a_NumResults, const char * a_FunctionName)
{
	ASSERT(lua_isfunction(m_LuaState, -a_NumArgs - 1));
	
	int s = lua_pcall(m_LuaState, a_NumArgs, a_NumResults, 0);
	if (ReportErrors(s))
	{
		LOGWARNING("Error in %s calling function %s()", m_SubsystemName.c_str(), a_FunctionName);
		return false;
	}
	return true;
}





bool cLuaState::ReportErrors(int a_Status)
{
	return ReportErrors(m_LuaState, a_Status);
}





bool cLuaState::ReportErrors(lua_State * a_LuaState, int a_Status)
{
	if (a_Status == 0)
	{
		// No error to report
		return false;
	}
	
	LOGWARNING("LUA: %d - %s", a_Status, lua_tostring(a_LuaState, -1));
	lua_pop(a_LuaState, 1);
	return true;
}




