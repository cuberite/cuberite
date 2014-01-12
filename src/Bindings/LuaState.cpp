
// LuaState.cpp

// Implements the cLuaState class representing the wrapper over lua_State *, provides associated helper functions

#include "Globals.h"
#include "LuaState.h"

extern "C"
{
	#include "lua/src/lualib.h"
}

#include "tolua++/include/tolua++.h"
#include "Bindings.h"
#include "ManualBindings.h"

// fwd: SQLite/lsqlite3.c
extern "C"
{
	int luaopen_lsqlite3(lua_State * L);
}

// fwd: LuaExpat/lxplib.c:
extern "C"
{
	int luaopen_lxp(lua_State * L);
}






const cLuaState::cRet cLuaState::Return = {};





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLuaState:

cLuaState::cLuaState(const AString & a_SubsystemName) :
	m_LuaState(NULL),
	m_IsOwned(false),
	m_SubsystemName(a_SubsystemName),
	m_NumCurrentFunctionArgs(-1)
{
}





cLuaState::cLuaState(lua_State * a_AttachState) :
	m_LuaState(a_AttachState),
	m_IsOwned(false),
	m_SubsystemName("<attached>"),
	m_NumCurrentFunctionArgs(-1)
{
}





cLuaState::~cLuaState()
{
	if (IsValid())
	{
		if (m_IsOwned)
		{
			Close();
		}
		else
		{
			Detach();
		}
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





bool cLuaState::HasFunction(const char * a_FunctionName)
{
	if (!IsValid())
	{
		// This happens if cPlugin::Initialize() fails with an error
		return false;
	}

	lua_getglobal(m_LuaState, a_FunctionName);
	bool res = (!lua_isnil(m_LuaState, -1) && lua_isfunction(m_LuaState, -1));
	lua_pop(m_LuaState, 1);
	return res;
}





bool cLuaState::PushFunction(const char * a_FunctionName)
{
	ASSERT(m_NumCurrentFunctionArgs == -1);  // If not, there's already something pushed onto the stack

	if (!IsValid())
	{
		// This happens if cPlugin::Initialize() fails with an error
		return false;
	}
	
	// Push the error handler for lua_pcall()
	lua_pushcfunction(m_LuaState, &ReportFnCallErrors);
	
	lua_getglobal(m_LuaState, a_FunctionName);
	if (!lua_isfunction(m_LuaState, -1))
	{
		LOGWARNING("Error in %s: Could not find function %s()", m_SubsystemName.c_str(), a_FunctionName);
		lua_pop(m_LuaState, 1);
		return false;
	}
	m_CurrentFunctionName.assign(a_FunctionName);
	m_NumCurrentFunctionArgs = 0;
	return true;
}





bool cLuaState::PushFunction(int a_FnRef)
{
	ASSERT(IsValid());
	ASSERT(m_NumCurrentFunctionArgs == -1);  // If not, there's already something pushed onto the stack
	
	// Push the error handler for lua_pcall()
	lua_pushcfunction(m_LuaState, &ReportFnCallErrors);
	
	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, a_FnRef);  // same as lua_getref()
	if (!lua_isfunction(m_LuaState, -1))
	{
		lua_pop(m_LuaState, 1);
		return false;
	}
	m_CurrentFunctionName = "<callback>";
	m_NumCurrentFunctionArgs = 0;
	return true;
}





bool cLuaState::PushFunction(const cTableRef & a_TableRef)
{
	ASSERT(IsValid());
	ASSERT(m_NumCurrentFunctionArgs == -1);  // If not, there's already something pushed onto the stack
	
	// Push the error handler for lua_pcall()
	lua_pushcfunction(m_LuaState, &ReportFnCallErrors);
	
	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, a_TableRef.GetTableRef());  // Get the table ref
	if (!lua_istable(m_LuaState, -1))
	{
		// Not a table, bail out
		lua_pop(m_LuaState, 1);
		return false;
	}
	lua_getfield(m_LuaState, -1, a_TableRef.GetFnName());
	if (lua_isnil(m_LuaState, -1) || !lua_isfunction(m_LuaState, -1))
	{
		// Not a valid function, bail out
		lua_pop(m_LuaState, 2);
		return false;
	}
	Printf(m_CurrentFunctionName, "<table-callback %s>", a_TableRef.GetFnName());
	m_NumCurrentFunctionArgs = 0;
	return true;
}





void cLuaState::Push(const AString & a_String)
{
	ASSERT(IsValid());

	lua_pushlstring(m_LuaState, a_String.data(), a_String.size());
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const AStringVector & a_Vector)
{
	ASSERT(IsValid());

	lua_createtable(m_LuaState, (int)a_Vector.size(), 0);
	int newTable = lua_gettop(m_LuaState);
	int index = 1;
	for (AStringVector::const_iterator itr = a_Vector.begin(), end = a_Vector.end(); itr != end; ++itr, ++index)
	{
		tolua_pushstring(m_LuaState, itr->c_str());
		lua_rawseti(m_LuaState, newTable, index);
	}
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::PushUserType(void * a_Object, const char * a_Type)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Object, a_Type);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(int a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, a_Value);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(double a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, a_Value);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const char * a_Value)
{
	ASSERT(IsValid());

	tolua_pushstring(m_LuaState, a_Value);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(bool a_Value)
{
	ASSERT(IsValid());

	tolua_pushboolean(m_LuaState, a_Value ? 1 : 0);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cWorld * a_World)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_World, "cWorld");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cPlayer * a_Player)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cPlayer * a_Player)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cEntity * a_Entity)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Entity, "cEntity");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cMonster * a_Monster)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Monster, "cMonster");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cItem * a_Item)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Item, "cItem");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cItems * a_Items)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Items, "cItems");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cItems & a_Items)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, (void *)&a_Items, "cItems");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cClientHandle * a_Client)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Client, "cClientHandle");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cPickup * a_Pickup)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Pickup, "cPickup");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cChunkDesc * a_ChunkDesc)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_ChunkDesc, "cChunkDesc");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cCraftingGrid * a_Grid)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, (void *)a_Grid, "cCraftingGrid");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cCraftingRecipe * a_Recipe)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, (void *)a_Recipe, "cCraftingRecipe");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(TakeDamageInfo * a_TDI)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_TDI, "TakeDamageInfo");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cWindow * a_Window)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Window, "cWindow");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cPluginLua * a_Plugin)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Plugin, "cPluginLua");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const HTTPRequest * a_Request)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, (void *)a_Request, "HTTPRequest");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cWebAdmin * a_WebAdmin)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_WebAdmin, "cWebAdmin");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const HTTPTemplateRequest * a_Request)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, (void *)a_Request, "HTTPTemplateRequest");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cTNTEntity * a_TNTEntity)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_TNTEntity, "cTNTEntity");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cCreeper * a_Creeper)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Creeper, "cCreeper");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(Vector3i * a_Vector)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Vector, "Vector3i");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(void * a_Ptr)
{
	ASSERT(IsValid());

	lua_pushnil(m_LuaState);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cHopperEntity * a_Hopper)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Hopper, "cHopperEntity");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cBlockEntity * a_BlockEntity)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_BlockEntity, "cBlockEntity");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::GetReturn(int a_StackPos, bool & a_ReturnedVal)
{
	a_ReturnedVal = (tolua_toboolean(m_LuaState, a_StackPos, a_ReturnedVal ? 1 : 0) > 0);
}





void cLuaState::GetReturn(int a_StackPos, AString & a_ReturnedVal)
{
	if (lua_isstring(m_LuaState, a_StackPos))
	{
		a_ReturnedVal = tolua_tocppstring(m_LuaState, a_StackPos, a_ReturnedVal.c_str());
	}
}





void cLuaState::GetReturn(int a_StackPos, int & a_ReturnedVal)
{
	if (lua_isnumber(m_LuaState, a_StackPos))
	{
		a_ReturnedVal = (int)tolua_tonumber(m_LuaState, a_StackPos, a_ReturnedVal);
	}
}





void cLuaState::GetReturn(int a_StackPos, double & a_ReturnedVal)
{
	if (lua_isnumber(m_LuaState, a_StackPos))
	{
		a_ReturnedVal = tolua_tonumber(m_LuaState, a_StackPos, a_ReturnedVal);
	}
}





bool cLuaState::CallFunction(int a_NumResults)
{
	ASSERT (m_NumCurrentFunctionArgs >= 0);  // A function must be pushed to stack first
	ASSERT(lua_isfunction(m_LuaState, -m_NumCurrentFunctionArgs - 1));  // The function to call
	ASSERT(lua_isfunction(m_LuaState, -m_NumCurrentFunctionArgs - 2));  // The error handler
	
	int s = lua_pcall(m_LuaState, m_NumCurrentFunctionArgs, a_NumResults, -m_NumCurrentFunctionArgs - 2);
	if (s != 0)
	{
		// The error has already been printed together with the stacktrace
		LOGWARNING("Error in %s calling function %s()", m_SubsystemName.c_str(), m_CurrentFunctionName.c_str());
		m_NumCurrentFunctionArgs = -1;
		m_CurrentFunctionName.clear();
		return false;
	}
	m_NumCurrentFunctionArgs = -1;
	m_CurrentFunctionName.clear();
	return true;
}





bool cLuaState::CheckParamUserTable(int a_StartParam, const char * a_UserTable, int a_EndParam)
{
	ASSERT(IsValid());
	
	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}
	
	tolua_Error tolua_err;
	for (int i = a_StartParam; i <= a_EndParam; i++)
	{
		if (tolua_isusertable(m_LuaState, i, a_UserTable, 0, &tolua_err))
		{
			continue;
		}
		// Not the correct parameter
		lua_Debug entry;
		VERIFY(lua_getstack(m_LuaState, 0,   &entry));
		VERIFY(lua_getinfo (m_LuaState, "n", &entry));
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != NULL) ? entry.name : "?");
		tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
		return false;
	}  // for i - Param
	
	// All params checked ok
	return true;
}





bool cLuaState::CheckParamUserType(int a_StartParam, const char * a_UserType, int a_EndParam)
{
	ASSERT(IsValid());
	
	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}
	
	tolua_Error tolua_err;
	for (int i = a_StartParam; i <= a_EndParam; i++)
	{
		if (tolua_isusertype(m_LuaState, i, a_UserType, 0, &tolua_err))
		{
			continue;
		}
		// Not the correct parameter
		lua_Debug entry;
		VERIFY(lua_getstack(m_LuaState, 0,   &entry));
		VERIFY(lua_getinfo (m_LuaState, "n", &entry));
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != NULL) ? entry.name : "?");
		tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
		return false;
	}  // for i - Param
	
	// All params checked ok
	return true;
}





bool cLuaState::CheckParamTable(int a_StartParam, int a_EndParam)
{
	ASSERT(IsValid());
	
	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}
	
	tolua_Error tolua_err;
	for (int i = a_StartParam; i <= a_EndParam; i++)
	{
		if (tolua_istable(m_LuaState, i, 0, &tolua_err))
		{
			continue;
		}
		// Not the correct parameter
		lua_Debug entry;
		VERIFY(lua_getstack(m_LuaState, 0,   &entry));
		VERIFY(lua_getinfo (m_LuaState, "n", &entry));
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != NULL) ? entry.name : "?");
		tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
		return false;
	}  // for i - Param
	
	// All params checked ok
	return true;
}





bool cLuaState::CheckParamNumber(int a_StartParam, int a_EndParam)
{
	ASSERT(IsValid());
	
	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}
	
	tolua_Error tolua_err;
	for (int i = a_StartParam; i <= a_EndParam; i++)
	{
		if (tolua_isnumber(m_LuaState, i, 0, &tolua_err))
		{
			continue;
		}
		// Not the correct parameter
		lua_Debug entry;
		VERIFY(lua_getstack(m_LuaState, 0,   &entry));
		VERIFY(lua_getinfo (m_LuaState, "n", &entry));
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != NULL) ? entry.name : "?");
		tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
		return false;
	}  // for i - Param
	
	// All params checked ok
	return true;
}





bool cLuaState::CheckParamString(int a_StartParam, int a_EndParam)
{
	ASSERT(IsValid());
	
	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}
	
	tolua_Error tolua_err;
	for (int i = a_StartParam; i <= a_EndParam; i++)
	{
		if (tolua_isstring(m_LuaState, i, 0, &tolua_err))
		{
			continue;
		}
		// Not the correct parameter
		lua_Debug entry;
		VERIFY(lua_getstack(m_LuaState, 0,   &entry));
		VERIFY(lua_getinfo (m_LuaState, "n", &entry));
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != NULL) ? entry.name : "?");
		tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
		return false;
	}  // for i - Param
	
	// All params checked ok
	return true;
}





bool cLuaState::CheckParamEnd(int a_Param)
{
	tolua_Error tolua_err;
	if (tolua_isnoobj(m_LuaState, a_Param, &tolua_err))
	{
		return true;
	}
	// Not the correct parameter
	lua_Debug entry;
	VERIFY(lua_getstack(m_LuaState, 0,   &entry));
	VERIFY(lua_getinfo (m_LuaState, "n", &entry));
	AString ErrMsg = Printf("#ferror in function '%s': Too many arguments.", (entry.name != NULL) ? entry.name : "?");
	tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
	return false;
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





void cLuaState::LogStackTrace(void)
{
	LogStackTrace(m_LuaState);
}





void cLuaState::LogStackTrace(lua_State * a_LuaState)
{
	LOGWARNING("Stack trace:");
	lua_Debug entry;
	int depth = 0;
	while (lua_getstack(a_LuaState, depth, &entry))
	{
		int status = lua_getinfo(a_LuaState, "Sln", &entry);
		assert(status);

		LOGWARNING("  %s(%d): %s", entry.short_src, entry.currentline, entry.name ? entry.name : "(no name)");
		depth++;
	}
	LOGWARNING("Stack trace end");
}





AString cLuaState::GetTypeText(int a_StackPos)
{
	int Type = lua_type(m_LuaState, a_StackPos);
	switch (Type)
	{
		case LUA_TNONE:          return "TNONE";
		case LUA_TNIL:           return "TNIL";
		case LUA_TBOOLEAN:       return "TBOOLEAN";
		case LUA_TLIGHTUSERDATA: return "TLIGHTUSERDATA";
		case LUA_TNUMBER:        return "TNUMBER";
		case LUA_TSTRING:        return "TSTRING";
		case LUA_TTABLE:         return "TTABLE";
		case LUA_TFUNCTION:      return "TFUNCTION";
		case LUA_TUSERDATA:      return "TUSERDATA";
		case LUA_TTHREAD:        return "TTHREAD";
	}
	return Printf("Unknown (%d)", Type);
}





int cLuaState::ReportFnCallErrors(lua_State * a_LuaState)
{
	LOGWARNING("LUA: %s", lua_tostring(a_LuaState, -1));
	LogStackTrace(a_LuaState);
	return 1;  // We left the error message on the stack as the return value
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLuaState::cRef:

cLuaState::cRef::cRef(cLuaState & a_LuaState, int a_StackPos) :
	m_LuaState(a_LuaState)
{
	ASSERT(m_LuaState.IsValid());
	
	lua_pushvalue(m_LuaState, a_StackPos);  // Push a copy of the value at a_StackPos onto the stack
	m_Ref = luaL_ref(m_LuaState, LUA_REGISTRYINDEX);
}





cLuaState::cRef::~cRef()
{
	ASSERT(m_LuaState.IsValid());
	
	if (IsValid())
	{
		luaL_unref(m_LuaState, LUA_REGISTRYINDEX, m_Ref);
	}
}




