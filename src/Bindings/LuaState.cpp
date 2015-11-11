
// LuaState.cpp

// Implements the cLuaState class representing the wrapper over lua_State *, provides associated helper functions

#include "Globals.h"
#include "LuaState.h"

extern "C"
{
	#include "lua/src/lualib.h"
}

#undef TOLUA_TEMPLATE_BIND
#include "tolua++/include/tolua++.h"
#include "Bindings.h"
#include "ManualBindings.h"
#include "DeprecatedBindings.h"
#include "../Entities/Entity.h"
#include "../BlockEntities/BlockEntity.h"

// fwd: "SQLite/lsqlite3.c"
extern "C"
{
	int luaopen_lsqlite3(lua_State * L);
}

// fwd: "LuaExpat/lxplib.c":
extern "C"
{
	int luaopen_lxp(lua_State * L);
}






const cLuaState::cRet cLuaState::Return = {};





////////////////////////////////////////////////////////////////////////////////
// cLuaStateTracker:

void cLuaStateTracker::Add(cLuaState & a_LuaState)
{
	auto & Instance = Get();
	cCSLock Lock(Instance.m_CSLuaStates);
	Instance.m_LuaStates.push_back(&a_LuaState);
}




void cLuaStateTracker::Del(cLuaState & a_LuaState)
{
	auto & Instance = Get();
	cCSLock Lock(Instance.m_CSLuaStates);
	Instance.m_LuaStates.erase(
		std::remove_if(
			Instance.m_LuaStates.begin(), Instance.m_LuaStates.end(),
			[&a_LuaState](cLuaStatePtr a_StoredLuaState)
			{
				return (&a_LuaState == a_StoredLuaState);
			}
		),
		Instance.m_LuaStates.end()
	);
}





AString cLuaStateTracker::GetStats(void)
{
	auto & Instance = Get();
	cCSLock Lock(Instance.m_CSLuaStates);
	AString res;
	int Total = 0;
	for (auto state: Instance.m_LuaStates)
	{
		int Mem = 0;
		if (!state->Call("collectgarbage", "count", cLuaState::Return, Mem))
		{
			res.append(Printf("Cannot query memory for state \"%s\"\n", state->GetSubsystemName().c_str()));
		}
		else
		{
			res.append(Printf("State \"%s\" is using %d KiB of memory\n", state->GetSubsystemName().c_str(), Mem));
			Total += Mem;
		}
	}
	res.append(Printf("Total memory used by Lua: %d KiB\n", Total));
	return res;
}





cLuaStateTracker & cLuaStateTracker::Get(void)
{
	static cLuaStateTracker Inst;  // The singleton
	return Inst;
}





////////////////////////////////////////////////////////////////////////////////
// cLuaState:

cLuaState::cLuaState(const AString & a_SubsystemName) :
	m_LuaState(nullptr),
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
	if (m_LuaState != nullptr)
	{
		LOGWARNING("%s: Trying to create an already-existing LuaState, ignoring.", __FUNCTION__);
		return;
	}
	m_LuaState = lua_open();
	luaL_openlibs(m_LuaState);
	m_IsOwned = true;
	cLuaStateTracker::Add(*this);
}





void cLuaState::RegisterAPILibs(void)
{
	tolua_AllToLua_open(m_LuaState);
	cManualBindings::Bind(m_LuaState);
	DeprecatedBindings::Bind(m_LuaState);
	luaopen_lsqlite3(m_LuaState);
	luaopen_lxp(m_LuaState);
}





void cLuaState::Close(void)
{
	if (m_LuaState == nullptr)
	{
		LOGWARNING("%s: Trying to close an invalid LuaState, ignoring.", __FUNCTION__);
		return;
	}
	if (!m_IsOwned)
	{
		LOGWARNING(
			"%s: Detected mis-use, calling Close() on an attached state (0x%p). Detaching instead.",
			__FUNCTION__, static_cast<void *>(m_LuaState)
		);
		Detach();
		return;
	}
	cLuaStateTracker::Del(*this);
	lua_close(m_LuaState);
	m_LuaState = nullptr;
	m_IsOwned = false;
}





void cLuaState::Attach(lua_State * a_State)
{
	if (m_LuaState != nullptr)
	{
		LOGINFO("%s: Already contains a LuaState (0x%p), will be closed / detached.", __FUNCTION__, static_cast<void *>(m_LuaState));
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
	if (m_LuaState == nullptr)
	{
		return;
	}
	if (m_IsOwned)
	{
		LOGWARNING(
			"%s: Detected a mis-use, calling Detach() when the state is owned. Closing the owned state (0x%p).",
			__FUNCTION__, static_cast<void *>(m_LuaState)
		);
		Close();
		return;
	}
	m_LuaState = nullptr;
}





void cLuaState::AddPackagePath(const AString & a_PathVariable, const AString & a_Path)
{
	// Get the current path:
	lua_getfield(m_LuaState, LUA_GLOBALSINDEX, "package");                           // Stk: <package>
	lua_getfield(m_LuaState, -1, a_PathVariable.c_str());                            // Stk: <package> <package.path>
	size_t len = 0;
	const char * PackagePath = lua_tolstring(m_LuaState, -1, &len);
	
	// Append the new path:
	AString NewPackagePath(PackagePath, len);
	NewPackagePath.append(LUA_PATHSEP);
	NewPackagePath.append(a_Path);
	
	// Set the new path to the environment:
	lua_pop(m_LuaState, 1);                                                          // Stk: <package>
	lua_pushlstring(m_LuaState, NewPackagePath.c_str(), NewPackagePath.length());    // Stk: <package> <NewPackagePath>
	lua_setfield(m_LuaState, -2, a_PathVariable.c_str());                            // Stk: <package>
	lua_pop(m_LuaState, 1);
	lua_pop(m_LuaState, 1);                                                          // Stk: -
}





bool cLuaState::LoadFile(const AString & a_FileName, bool a_LogWarnings)
{
	ASSERT(IsValid());

	// Load the file:
	int s = luaL_loadfile(m_LuaState, a_FileName.c_str());
	if (s != 0)
	{
		if (a_LogWarnings)
		{
			LOGWARNING("Can't load %s because of a load error in file %s: %d (%s)", m_SubsystemName.c_str(), a_FileName.c_str(), s, lua_tostring(m_LuaState, -1));
		}
		lua_pop(m_LuaState, 1);
		return false;
	}

	// Execute the globals:
	s = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
	if (s != 0)
	{
		if (a_LogWarnings)
		{
			LOGWARNING("Can't load %s because of an initialization error in file %s: %d (%s)", m_SubsystemName.c_str(), a_FileName.c_str(), s, lua_tostring(m_LuaState, -1));
		}
		lua_pop(m_LuaState, 1);
		return false;
	}

	return true;
}





bool cLuaState::LoadString(const AString & a_StringToLoad, const AString & a_FileName, bool a_LogWarnings)
{
	ASSERT(IsValid());

	// Load the file:
	int s = luaL_loadstring(m_LuaState, a_StringToLoad.c_str());
	if (s != 0)
	{
		if (a_LogWarnings)
		{
			LOGWARNING("Can't load %s because of a load error in string from \"%s\": %d (%s)", m_SubsystemName.c_str(), a_FileName.c_str(), s, lua_tostring(m_LuaState, -1));
		}
		lua_pop(m_LuaState, 1);
		return false;
	}

	// Execute the globals:
	s = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
	if (s != 0)
	{
		if (a_LogWarnings)
		{
			LOGWARNING("Can't load %s because of an initialization error in string from \"%s\": %d (%s)", m_SubsystemName.c_str(), a_FileName.c_str(), s, lua_tostring(m_LuaState, -1));
		}
		lua_pop(m_LuaState, 1);
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
		lua_pop(m_LuaState, 2);
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
		lua_pop(m_LuaState, 2);
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
		lua_pop(m_LuaState, 2);
		return false;
	}
	lua_getfield(m_LuaState, -1, a_TableRef.GetFnName());
	if (lua_isnil(m_LuaState, -1) || !lua_isfunction(m_LuaState, -1))
	{
		// Not a valid function, bail out
		lua_pop(m_LuaState, 3);
		return false;
	}
	
	// Pop the table off the stack:
	lua_remove(m_LuaState, -2);
	
	Printf(m_CurrentFunctionName, "<table-callback %s>", a_TableRef.GetFnName());
	m_NumCurrentFunctionArgs = 0;
	return true;
}





void cLuaState::PushNil(void)
{
	ASSERT(IsValid());

	lua_pushnil(m_LuaState);
	m_NumCurrentFunctionArgs += 1;
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

	lua_createtable(m_LuaState, static_cast<int>(a_Vector.size()), 0);
	int newTable = lua_gettop(m_LuaState);
	int index = 1;
	for (AStringVector::const_iterator itr = a_Vector.begin(), end = a_Vector.end(); itr != end; ++itr, ++index)
	{
		tolua_pushstring(m_LuaState, itr->c_str());
		lua_rawseti(m_LuaState, newTable, index);
	}
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cCraftingGrid * a_Grid)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<cCraftingGrid *>(a_Grid)), "cCraftingGrid");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cCraftingRecipe * a_Recipe)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<cCraftingRecipe *>(a_Recipe)), "cCraftingRecipe");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const char * a_Value)
{
	ASSERT(IsValid());

	tolua_pushstring(m_LuaState, a_Value);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cItems & a_Items)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<cItems *>(&a_Items)), "cItems");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cPlayer * a_Player)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<cPlayer *>(a_Player)), "cPlayer");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const cLuaState::cRef & a_Ref)
{
	ASSERT(IsValid());

	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, static_cast<int>(a_Ref));
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const HTTPRequest * a_Request)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<HTTPRequest *>(a_Request)), "HTTPRequest");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const HTTPTemplateRequest * a_Request)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<HTTPTemplateRequest *>(a_Request)), "HTTPTemplateRequest");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const Vector3d & a_Vector)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<Vector3d *>(&a_Vector)), "Vector3<double>");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const Vector3d * a_Vector)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<Vector3d *>(a_Vector)), "Vector3<double>");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const Vector3i & a_Vector)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<Vector3i *>(&a_Vector)), "Vector3<int>");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(const Vector3i * a_Vector)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, reinterpret_cast<void *>(const_cast<Vector3i *>(a_Vector)), "Vector3<int>");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(bool a_Value)
{
	ASSERT(IsValid());

	tolua_pushboolean(m_LuaState, a_Value ? 1 : 0);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cEntity * a_Entity)
{
	ASSERT(IsValid());

	if (a_Entity == nullptr)
	{
		lua_pushnil(m_LuaState);
	}
	else
	{
		switch (a_Entity->GetEntityType())
		{
			case cEntity::etMonster:
			{
				// Don't push specific mob types, as those are not exported in the API:
				tolua_pushusertype(m_LuaState, a_Entity, "cMonster");
				break;
			}
			case cEntity::etPlayer:
			{
				tolua_pushusertype(m_LuaState, a_Entity, "cPlayer");
				break;
			}
			case cEntity::etPickup:
			{
				tolua_pushusertype(m_LuaState, a_Entity, "cPickup");
				break;
			}
			case cEntity::etTNT:
			{
				tolua_pushusertype(m_LuaState, a_Entity, "cTNTEntity");
				break;
			}
			case cEntity::etProjectile:
			{
				tolua_pushusertype(m_LuaState, a_Entity, a_Entity->GetClass());
				break;
			}
			case cEntity::etFloater:
			{
				tolua_pushusertype(m_LuaState, a_Entity, "cFloater");
				break;
			}

			case cEntity::etEntity:
			case cEntity::etEnderCrystal:
			case cEntity::etFallingBlock:
			case cEntity::etMinecart:
			case cEntity::etBoat:
			case cEntity::etExpOrb:
			case cEntity::etItemFrame:
			case cEntity::etPainting:
			{
				// Push the generic entity class type:
				tolua_pushusertype(m_LuaState, a_Entity, "cEntity");
			}
		}  // switch (EntityType)
	}

	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cLuaServerHandle * a_ServerHandle)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_ServerHandle, "cServerHandle");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cLuaTCPLink * a_TCPLink)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_TCPLink, "cTCPLink");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(cLuaUDPEndpoint * a_UDPEndpoint)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_UDPEndpoint, "cUDPEndpoint");
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(double a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, a_Value);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(int a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, a_Value);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(long a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, static_cast<lua_Number>(a_Value));
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(UInt32 a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, a_Value);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(void * a_Ptr)
{
	UNUSED(a_Ptr);
	ASSERT(IsValid());

	// Investigate the cause of this - what is the callstack?
	// One code path leading here is the OnHookExploding / OnHookExploded with exotic parameters. Need to decide what to do with them
	LOGWARNING("Lua engine: attempting to push a plain pointer, pushing nil instead.");
	LOGWARNING("This indicates an unimplemented part of MCS bindings");
	LogStackTrace();
	
	lua_pushnil(m_LuaState);
	m_NumCurrentFunctionArgs += 1;
}





void cLuaState::Push(std::chrono::milliseconds a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, static_cast<lua_Number>(a_Value.count()));
	m_NumCurrentFunctionArgs += 1;
}





/*
void cLuaState::PushUserType(void * a_Object, const char * a_Type)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_Object, a_Type);
	m_NumCurrentFunctionArgs += 1;
}
*/





bool cLuaState::GetStackValue(int a_StackPos, AString & a_Value)
{
	size_t len = 0;
	const char * data = lua_tolstring(m_LuaState, a_StackPos, &len);
	if (data != nullptr)
	{
		a_Value.assign(data, len);
		return true;
	}
	return false;
}





bool cLuaState::GetStackValue(int a_StackPos, bool & a_ReturnedVal)
{
	a_ReturnedVal = (tolua_toboolean(m_LuaState, a_StackPos, a_ReturnedVal ? 1 : 0) > 0);
	return true;
}





bool cLuaState::GetStackValue(int a_StackPos, cPluginManager::CommandResult & a_Result)
{
	if (lua_isnumber(m_LuaState, a_StackPos))
	{
		a_Result = static_cast<cPluginManager::CommandResult>(static_cast<int>((tolua_tonumber(m_LuaState, a_StackPos, a_Result))));
		return true;
	}
	return false;
}





bool cLuaState::GetStackValue(int a_StackPos, cRef & a_Ref)
{
	a_Ref.RefStack(*this, a_StackPos);
	return true;
}





bool cLuaState::GetStackValue(int a_StackPos, double & a_ReturnedVal)
{
	if (lua_isnumber(m_LuaState, a_StackPos))
	{
		a_ReturnedVal = tolua_tonumber(m_LuaState, a_StackPos, a_ReturnedVal);
		return true;
	}
	return false;
}





bool cLuaState::GetStackValue(int a_StackPos, eBlockFace & a_ReturnedVal)
{
	if (!lua_isnumber(m_LuaState, a_StackPos))
	{
		return false;
	}
	a_ReturnedVal = static_cast<eBlockFace>(Clamp(
		static_cast<int>(tolua_tonumber(m_LuaState, a_StackPos, a_ReturnedVal)),
		static_cast<int>(BLOCK_FACE_MIN), static_cast<int>(BLOCK_FACE_MAX))
	);
	return true;
}





bool cLuaState::GetStackValue(int a_StackPos, eWeather & a_ReturnedVal)
{
	if (!lua_isnumber(m_LuaState, a_StackPos))
	{
		return false;
	}
	a_ReturnedVal = static_cast<eWeather>(Clamp(
		static_cast<int>(tolua_tonumber(m_LuaState, a_StackPos, a_ReturnedVal)),
		static_cast<int>(wSunny), static_cast<int>(wThunderstorm))
	);
	return true;
}





bool cLuaState::GetStackValue(int a_StackPos, float & a_ReturnedVal)
{
	if (lua_isnumber(m_LuaState, a_StackPos))
	{
		a_ReturnedVal = static_cast<float>(tolua_tonumber(m_LuaState, a_StackPos, a_ReturnedVal));
		return true;
	}
	return false;
}





cLuaState::cStackValue cLuaState::WalkToValue(const AString & a_Name)
{
	// There needs to be at least one value on the stack:
	ASSERT(lua_gettop(m_LuaState) > 0);

	// Iterate over path and replace the top of the stack with the walked element
	lua_pushvalue(m_LuaState, -1);  // Copy the stack value into the "working area"
	auto path = StringSplit(a_Name, ".");
	for (const auto & elem: path)
	{
		// If the value is not a table, bail out (error):
		if (!lua_istable(m_LuaState, -1))
		{
			lua_pop(m_LuaState, 1);
			return cStackValue();
		}

		// Get the next part of the path:
		lua_getfield(m_LuaState, -1, elem.c_str());

		// Remove the previous value from the stack (keep only the new one):
		lua_remove(m_LuaState, -2);
	}  // for elem - path[]
	if (lua_isnil(m_LuaState, -1))
	{
		lua_pop(m_LuaState, 1);
		return cStackValue();
	}
	return cStackValue(*this);
}





cLuaState::cStackValue cLuaState::WalkToNamedGlobal(const AString & a_Name)
{
	// Iterate over path and replace the top of the stack with the walked element
	lua_getglobal(m_LuaState, "_G");
	auto path = StringSplit(a_Name, ".");
	for (const auto & elem: path)
	{
		// If the value is not a table, bail out (error):
		if (!lua_istable(m_LuaState, -1))
		{
			lua_pop(m_LuaState, 1);
			return cStackValue();
		}

		// Get the next part of the path:
		lua_getfield(m_LuaState, -1, elem.c_str());

		// Remove the previous value from the stack (keep only the new one):
		lua_remove(m_LuaState, -2);
	}  // for elem - path[]
	if (lua_isnil(m_LuaState, -1))
	{
		lua_pop(m_LuaState, 1);
		return cStackValue();
	}
	return std::move(cStackValue(*this));
}





bool cLuaState::CallFunction(int a_NumResults)
{
	ASSERT (m_NumCurrentFunctionArgs >= 0);  // A function must be pushed to stack first
	ASSERT(lua_isfunction(m_LuaState, -m_NumCurrentFunctionArgs - 1));  // The function to call
	ASSERT(lua_isfunction(m_LuaState, -m_NumCurrentFunctionArgs - 2));  // The error handler
	
	// Save the current "stack" state and reset, in case the callback calls another function:
	AString CurrentFunctionName;
	std::swap(m_CurrentFunctionName, CurrentFunctionName);
	int NumArgs = m_NumCurrentFunctionArgs;
	m_NumCurrentFunctionArgs = -1;
	
	// Call the function:
	int s = lua_pcall(m_LuaState, NumArgs, a_NumResults, -NumArgs - 2);
	if (s != 0)
	{
		// The error has already been printed together with the stacktrace
		LOGWARNING("Error in %s calling function %s()", m_SubsystemName.c_str(), CurrentFunctionName.c_str());
		return false;
	}
	
	// Remove the error handler from the stack:
	lua_remove(m_LuaState, -a_NumResults - 1);
	
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
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != nullptr) ? entry.name : "?");
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
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != nullptr) ? entry.name : "?");
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
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != nullptr) ? entry.name : "?");

		BreakIntoDebugger(m_LuaState);

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
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != nullptr) ? entry.name : "?");
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
		if (lua_isstring(m_LuaState, i))
		{
			continue;
		}
		// Not the correct parameter
		lua_Debug entry;
		VERIFY(lua_getstack(m_LuaState, 0,   &entry));
		VERIFY(lua_getinfo (m_LuaState, "n", &entry));
		tolua_err.array = 0;
		tolua_err.type = "string";
		tolua_err.index = i;
		AString ErrMsg = Printf("#ferror in function '%s'.", (entry.name != nullptr) ? entry.name : "?");
		tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
		return false;
	}  // for i - Param
	
	// All params checked ok
	return true;
}





bool cLuaState::CheckParamFunction(int a_StartParam, int a_EndParam)
{
	ASSERT(IsValid());
	
	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}
	
	for (int i = a_StartParam; i <= a_EndParam; i++)
	{
		if (lua_isfunction(m_LuaState, i))
		{
			continue;
		}
		// Not the correct parameter
		lua_Debug entry;
		VERIFY(lua_getstack(m_LuaState, 0,   &entry));
		VERIFY(lua_getinfo (m_LuaState, "n", &entry));
		luaL_error(m_LuaState, "Error in function '%s' parameter #%d. Function expected, got %s",
			(entry.name != nullptr) ? entry.name : "?", i, GetTypeText(i).c_str()
		);
		return false;
	}  // for i - Param
	
	// All params checked ok
	return true;
}





bool cLuaState::CheckParamFunctionOrNil(int a_StartParam, int a_EndParam)
{
	ASSERT(IsValid());
	
	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}
	
	for (int i = a_StartParam; i <= a_EndParam; i++)
	{
		if (lua_isfunction(m_LuaState, i) || lua_isnil(m_LuaState, i))
		{
			continue;
		}
		// Not the correct parameter
		lua_Debug entry;
		VERIFY(lua_getstack(m_LuaState, 0,   &entry));
		VERIFY(lua_getinfo (m_LuaState, "n", &entry));
		luaL_error(m_LuaState, "Error in function '%s' parameter #%d. Function expected, got %s",
			(entry.name != nullptr) ? entry.name : "?", i, GetTypeText(i).c_str()
		);
		return false;
	}  // for i - Param
	
	// All params checked ok
	return true;
}





bool cLuaState::CheckParamEnd(int a_Param)
{
	tolua_Error tolua_err;
	if (tolua_isnoobj(m_LuaState, a_Param, &tolua_err) == 1)
	{
		return true;
	}
	// Not the correct parameter
	lua_Debug entry;
	VERIFY(lua_getstack(m_LuaState, 0,   &entry));
	VERIFY(lua_getinfo (m_LuaState, "n", &entry));
	AString ErrMsg = Printf("#ferror in function '%s': Too many arguments.", (entry.name != nullptr) ? entry.name : "?");
	tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
	return false;
}





bool cLuaState::IsParamUserType(int a_Param, AString a_UserType)
{
	ASSERT(IsValid());
	
	tolua_Error tolua_err;
	return (tolua_isusertype(m_LuaState, a_Param, a_UserType.c_str(), 0, &tolua_err) == 1);
}





bool cLuaState::IsParamNumber(int a_Param)
{
	ASSERT(IsValid());
	
	tolua_Error tolua_err;
	return (tolua_isnumber(m_LuaState, a_Param, 0, &tolua_err) == 1);
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





void cLuaState::LogStackTrace(int a_StartingDepth)
{
	LogStackTrace(m_LuaState, a_StartingDepth);
}





void cLuaState::LogStackTrace(lua_State * a_LuaState, int a_StartingDepth)
{
	LOGWARNING("Stack trace:");
	lua_Debug entry;
	int depth = a_StartingDepth;
	while (lua_getstack(a_LuaState, depth, &entry))
	{
		lua_getinfo(a_LuaState, "Sln", &entry);
		LOGWARNING("  %s(%d): %s", entry.short_src, entry.currentline, entry.name ? entry.name : "(no name)");
		depth++;
	}
	LOGWARNING("Stack trace end");
}





AString cLuaState::GetTypeText(int a_StackPos)
{
	return lua_typename(m_LuaState, lua_type(m_LuaState, a_StackPos));
}





int cLuaState::CallFunctionWithForeignParams(
	const AString & a_FunctionName,
	cLuaState & a_SrcLuaState,
	int a_SrcParamStart,
	int a_SrcParamEnd
)
{
	ASSERT(IsValid());
	ASSERT(a_SrcLuaState.IsValid());
	
	// Store the stack position before any changes
	int OldTop = lua_gettop(m_LuaState);
	
	// Push the function to call, including the error handler:
	if (!PushFunction(a_FunctionName.c_str()))
	{
		LOGWARNING("Function '%s' not found", a_FunctionName.c_str());
		lua_settop(m_LuaState, OldTop);
		return -1;
	}

	// Copy the function parameters to the target state
	if (CopyStackFrom(a_SrcLuaState, a_SrcParamStart, a_SrcParamEnd) < 0)
	{
		// Something went wrong, fix the stack and exit
		lua_settop(m_LuaState, OldTop);
		m_NumCurrentFunctionArgs = -1;
		m_CurrentFunctionName.clear();
		return -1;
	}
	
	// Call the function, with an error handler:
	int s = lua_pcall(m_LuaState, a_SrcParamEnd - a_SrcParamStart + 1, LUA_MULTRET, OldTop + 1);
	if (ReportErrors(s))
	{
		LOGWARN("Error while calling function '%s' in '%s'", a_FunctionName.c_str(), m_SubsystemName.c_str());
		// Reset the stack:
		lua_settop(m_LuaState, OldTop);
		
		// Reset the internal checking mechanisms:
		m_NumCurrentFunctionArgs = -1;
		m_CurrentFunctionName.clear();
		
		// Make Lua think everything is okay and return 0 values, so that plugins continue executing.
		// The failure is indicated by the zero return values.
		return 0;
	}
	
	// Reset the internal checking mechanisms:
	m_NumCurrentFunctionArgs = -1;
	m_CurrentFunctionName.clear();
	
	// Remove the error handler from the stack:
	lua_remove(m_LuaState, OldTop + 1);
	
	// Return the number of return values:
	return lua_gettop(m_LuaState) - OldTop;
}





int cLuaState::CopyStackFrom(cLuaState & a_SrcLuaState, int a_SrcStart, int a_SrcEnd)
{
	/*
	// DEBUG:
	LOGD("Copying stack values from %d to %d", a_SrcStart, a_SrcEnd);
	a_SrcLuaState.LogStack("Src stack before copying:");
	LogStack("Dst stack before copying:");
	*/
	for (int i = a_SrcStart; i <= a_SrcEnd; ++i)
	{
		int t = lua_type(a_SrcLuaState, i);
		switch (t)
		{
			case LUA_TNIL:
			{
				lua_pushnil(m_LuaState);
				break;
			}
			case LUA_TSTRING:
			{
				AString s;
				a_SrcLuaState.ToString(i, s);
				Push(s);
				break;
			}
			case LUA_TBOOLEAN:
			{
				bool b = (tolua_toboolean(a_SrcLuaState, i, false) != 0);
				Push(b);
				break;
			}
			case LUA_TNUMBER:
			{
				lua_Number d = tolua_tonumber(a_SrcLuaState, i, 0);
				Push(d);
				break;
			}
			case LUA_TUSERDATA:
			{
				// Get the class name:
				const char * type = nullptr;
				if (lua_getmetatable(a_SrcLuaState, i) == 0)
				{
					LOGWARNING("%s: Unknown class in pos %d, cannot copy.", __FUNCTION__, i);
					lua_pop(m_LuaState, i - a_SrcStart);
					return -1;
				}
				lua_rawget(a_SrcLuaState, LUA_REGISTRYINDEX);  // Stack +1
				type = lua_tostring(a_SrcLuaState, -1);
				lua_pop(a_SrcLuaState, 1);                     // Stack -1
				
				// Copy the value:
				void * ud = tolua_touserdata(a_SrcLuaState, i, nullptr);
				tolua_pushusertype(m_LuaState, ud, type);
				break;
			}
			default:
			{
				LOGWARNING("%s: Unsupported value: '%s' at stack position %d. Can only copy numbers, strings, bools and classes!",
					__FUNCTION__, lua_typename(a_SrcLuaState, t), i
				);
				a_SrcLuaState.LogStack("Stack where copying failed:");
				lua_pop(m_LuaState, i - a_SrcStart);
				return -1;
			}
		}
	}
	return a_SrcEnd - a_SrcStart + 1;
}





void cLuaState::ToString(int a_StackPos, AString & a_String)
{
	size_t len;
	const char * s = lua_tolstring(m_LuaState, a_StackPos, &len);
	if (s != nullptr)
	{
		a_String.assign(s, len);
	}
}





void cLuaState::LogStack(const char * a_Header)
{
	LogStack(m_LuaState, a_Header);
}





void cLuaState::LogStack(lua_State * a_LuaState, const char * a_Header)
{
	// Format string consisting only of %s is used to appease the compiler
	LOG("%s", (a_Header != nullptr) ? a_Header : "Lua C API Stack contents:");
	for (int i = lua_gettop(a_LuaState); i > 0; i--)
	{
		AString Value;
		int Type = lua_type(a_LuaState, i);
		switch (Type)
		{
			case LUA_TBOOLEAN: Value.assign((lua_toboolean(a_LuaState, i) != 0) ? "true" : "false"); break;
			case LUA_TLIGHTUSERDATA: Printf(Value, "%p", lua_touserdata(a_LuaState, i)); break;
			case LUA_TNUMBER:        Printf(Value, "%f", static_cast<double>(lua_tonumber(a_LuaState, i))); break;
			case LUA_TSTRING:        Printf(Value, "%s", lua_tostring(a_LuaState, i)); break;
			case LUA_TTABLE:         Printf(Value, "%p", lua_topointer(a_LuaState, i)); break;
			default: break;
		}
		LOGD("  Idx %d: type %d (%s) %s", i, Type, lua_typename(a_LuaState, Type), Value.c_str());
	}  // for i - stack idx
}





int cLuaState::ReportFnCallErrors(lua_State * a_LuaState)
{
	LOGWARNING("LUA: %s", lua_tostring(a_LuaState, -1));
	LogStackTrace(a_LuaState, 1);
	BreakIntoDebugger(a_LuaState);
	return 1;  // We left the error message on the stack as the return value
}





int cLuaState::BreakIntoDebugger(lua_State * a_LuaState)
{
	// Call the BreakIntoDebugger function, if available:
	lua_getglobal(a_LuaState, "BreakIntoDebugger");
	if (!lua_isfunction(a_LuaState, -1))
	{
		LOGD("LUA: BreakIntoDebugger() not found / not a function");
		lua_pop(a_LuaState, 1);
		return 1;
	}
	lua_insert(a_LuaState, -2);  // Copy the string that has been passed to us
	LOGD("Calling BreakIntoDebugger()...");
	lua_call(a_LuaState, 1, 0);
	LOGD("Returned from BreakIntoDebugger().");

	return 0;
}





////////////////////////////////////////////////////////////////////////////////
// cLuaState::cRef:

cLuaState::cRef::cRef(void) :
	m_LuaState(nullptr),
	m_Ref(LUA_REFNIL)
{
}





cLuaState::cRef::cRef(cLuaState & a_LuaState, int a_StackPos) :
	m_LuaState(nullptr),
	m_Ref(LUA_REFNIL)
{
	RefStack(a_LuaState, a_StackPos);
}





cLuaState::cRef::cRef(cRef && a_FromRef):
	m_LuaState(a_FromRef.m_LuaState),
	m_Ref(a_FromRef.m_Ref)
{
	a_FromRef.m_LuaState = nullptr;
	a_FromRef.m_Ref = LUA_REFNIL;
}





cLuaState::cRef::~cRef()
{
	if (m_LuaState != nullptr)
	{
		UnRef();
	}
}





void cLuaState::cRef::RefStack(cLuaState & a_LuaState, int a_StackPos)
{
	ASSERT(a_LuaState.IsValid());
	if (m_LuaState != nullptr)
	{
		UnRef();
	}
	m_LuaState = &a_LuaState;
	lua_pushvalue(a_LuaState, a_StackPos);  // Push a copy of the value at a_StackPos onto the stack
	m_Ref = luaL_ref(a_LuaState, LUA_REGISTRYINDEX);
}





void cLuaState::cRef::UnRef(void)
{
	ASSERT(m_LuaState->IsValid());  // The reference should be destroyed before destroying the LuaState
	
	if (IsValid())
	{
		luaL_unref(*m_LuaState, LUA_REGISTRYINDEX, m_Ref);
	}
	m_LuaState = nullptr;
	m_Ref = LUA_REFNIL;
}




