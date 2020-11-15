
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
#include "LuaJson.h"
#include "../Entities/Entity.h"
#include "../BlockEntities/BlockEntity.h"
#include "../DeadlockDetect.h"
#include "../UUID.h"





// Hotpatching the Macro to prevent a Clang Warning (0 for pointer used)
#undef  lua_tostring
#define lua_tostring(L, i) lua_tolstring(L, (i), nullptr)





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
const cLuaState::cNil cLuaState::Nil = {};





////////////////////////////////////////////////////////////////////////////////
// cCanonLuaStates:

/** Tracks the canon cLuaState instances for each lua_State pointer.
Used for tracked refs - the ref needs to be tracked by a single cLuaState (the canon state), despite being created from a different (attached) cLuaState.
The canon state must be available without accessing the Lua state itself (so it cannot be stored within Lua). */
class cCanonLuaStates
{
public:
	/** Returns the canon Lua state for the specified lua_State pointer. */
	static cLuaState * GetCanonState(lua_State * a_LuaState)
	{
		auto & inst = GetInstance();
		cCSLock lock(inst.m_CS);
		auto itr = inst.m_CanonStates.find(a_LuaState);
		if (itr == inst.m_CanonStates.end())
		{
			return nullptr;
		}
		return itr->second;
	}

	/** Adds a new canon cLuaState instance to the map.
	Used when a new Lua state is created, this informs the map that a new canon Lua state should be tracked. */
	static void Add(cLuaState & a_LuaState)
	{
		auto & inst = GetInstance();
		cCSLock lock(inst.m_CS);
		ASSERT(inst.m_CanonStates.find(a_LuaState) == inst.m_CanonStates.end());
		inst.m_CanonStates[a_LuaState.operator lua_State *()] = &a_LuaState;
	}

	/** Removes the bindings between the specified canon state and its lua_State pointer.
	Used when a Lua state is being closed. */
	static void Remove(cLuaState & a_LuaState)
	{
		auto & inst = GetInstance();
		cCSLock lock(inst.m_CS);
		auto itr = inst.m_CanonStates.find(a_LuaState);
		ASSERT(itr != inst.m_CanonStates.end());
		inst.m_CanonStates.erase(itr);
	}

protected:
	/** The mutex protecting m_CanonStates against multithreaded access. */
	cCriticalSection m_CS;

	/** Map of lua_State pointers to their canon cLuaState instances. */
	std::map<lua_State *, cLuaState *> m_CanonStates;


	/** Returns the singleton instance of this class. */
	static cCanonLuaStates & GetInstance(void)
	{
		static cCanonLuaStates canonLuaStates;
		return canonLuaStates;
	}
};





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
// cLuaState::cTrackedRef:

cLuaState::cTrackedRef::cTrackedRef(void):
	m_CS(nullptr)
{
}





bool cLuaState::cTrackedRef::RefStack(cLuaState & a_LuaState, int a_StackPos)
{
	// Clear any previous callback:
	Clear();

	// Add self to LuaState's callback-tracking:
	auto canonState = a_LuaState.QueryCanonLuaState();
	canonState->TrackRef(*this);

	// Store the new callback:
	m_CS = &(canonState->m_CS);
	m_Ref.RefStack(*canonState, a_StackPos);
	return true;
}





void cLuaState::cTrackedRef::Clear(void)
{
	// Free the reference:
	lua_State * luaState = nullptr;
	{
		auto cs = m_CS.load();
		if (cs != nullptr)
		{
			cCSLock Lock(*cs);
			if (!m_Ref.IsValid())
			{
				return;
			}
			luaState = m_Ref.GetLuaState();
			m_Ref.UnRef();
		}
	}
	m_CS = nullptr;

	// Remove from LuaState's callback-tracking:
	if (luaState == nullptr)
	{
		return;
	}
	cLuaState(luaState).UntrackRef(*this);
}





bool cLuaState::cTrackedRef::IsValid(void)
{
	auto cs = m_CS.load();
	if (cs == nullptr)
	{
		return false;
	}
	cCSLock lock(*cs);
	return m_Ref.IsValid();
}





bool cLuaState::cTrackedRef::IsSameLuaState(cLuaState & a_LuaState)
{
	auto cs = m_CS.load();
	if (cs == nullptr)
	{
		return false;
	}
	cCSLock lock(*cs);
	if (!m_Ref.IsValid())
	{
		return false;
	}
	auto canonState = a_LuaState.QueryCanonLuaState();
	if (canonState == nullptr)
	{
		return false;
	}
	return (m_Ref.GetLuaState() == static_cast<lua_State *>(*canonState));
}





void cLuaState::cTrackedRef::Invalidate(void)
{
	auto cs = m_CS.load();
	if (cs == nullptr)
	{
		// Already invalid
		return;
	}
	cCSLock Lock(*cs);
	if (!m_Ref.IsValid())
	{
		LOGD("%s: Inconsistent callback at %p, has a CS but an invalid Ref. This should not happen",
			__FUNCTION__, static_cast<void *>(this)
		);
		return;
	}
	m_Ref.UnRef();
	m_CS = nullptr;
}





////////////////////////////////////////////////////////////////////////////////
// cLuaState::cCallback:

bool cLuaState::cCallback::RefStack(cLuaState & a_LuaState, int a_StackPos)
{
	// Check if the stack contains a function:
	if (!lua_isfunction(a_LuaState, a_StackPos))
	{
		return false;
	}

	return Super::RefStack(a_LuaState, a_StackPos);
}





////////////////////////////////////////////////////////////////////////////////
// cLuaState::cOptionalCallback:

bool cLuaState::cOptionalCallback::RefStack(cLuaState & a_LuaState, int a_StackPos)
{
	// If the stack pos is nil, make this an empty callback:
	if (lua_isnil(a_LuaState, a_StackPos))
	{
		Clear();
		return true;
	}

	// Use default cCallback implementation:
	return Super::RefStack(a_LuaState, a_StackPos);
}





////////////////////////////////////////////////////////////////////////////////
// cLuaState::cTableRef:

bool cLuaState::cTableRef::RefStack(cLuaState & a_LuaState, int a_StackPos)
{
	// Check if the stack contains a table:
	if (!lua_istable(a_LuaState, a_StackPos))
	{
		return false;
	}

	return Super::RefStack(a_LuaState, a_StackPos);
}





////////////////////////////////////////////////////////////////////////////////
// cLuaState::cStackTable:

cLuaState::cStackTable::cStackTable(cLuaState & a_LuaState, int a_StackPos):
	m_LuaState(a_LuaState),
	m_StackPos(a_StackPos)
{
	ASSERT(lua_istable(a_LuaState, a_StackPos));
}





void cLuaState::cStackTable::ForEachArrayElement(cFunctionRef<bool(cLuaState & a_LuaState, int a_Index)> a_ElementCallback) const
{
	auto numElements = luaL_getn(m_LuaState, m_StackPos);
	#ifdef _DEBUG
		auto stackTop = lua_gettop(m_LuaState);
	#endif
	for (int idx = 1; idx <= numElements; idx++)
	{
		// Push the idx-th element of the array onto stack top and call the callback:
		lua_rawgeti(m_LuaState, m_StackPos, idx);
		auto shouldAbort = a_ElementCallback(m_LuaState, idx);
		ASSERT(lua_gettop(m_LuaState) == stackTop + 1);  // The element callback must not change the Lua stack below the value
		lua_pop(m_LuaState, 1);
		if (shouldAbort)
		{
			// The callback wants to abort
			return;
		}
	}
}





void cLuaState::cStackTable::ForEachElement(cFunctionRef<bool(cLuaState & a_LuaState)> a_ElementCallback) const
{
	#ifdef _DEBUG
		auto stackTop = lua_gettop(m_LuaState);
	#endif
	lua_pushvalue(m_LuaState, m_StackPos);  // Stk: <table>
	lua_pushnil(m_LuaState);                // Stk: <table> nil
	while (lua_next(m_LuaState, -2))        // Stk: <table> <key> <val>
	{
		auto shouldAbort = a_ElementCallback(m_LuaState);
		ASSERT(lua_gettop(m_LuaState) == stackTop + 3);  // The element callback must not change the Lua stack below the value
		lua_pop(m_LuaState, 1);  // Stk: <table> <key>
		if (shouldAbort)
		{
			// The callback wants to abort
			lua_pop(m_LuaState, 2);  // Stk: empty
			return;
		}
	}
	// Stk: <table>
	lua_pop(m_LuaState, 1);  // Stk: empty
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
	cCanonLuaStates::Add(*this);
}





void cLuaState::RegisterAPILibs(void)
{
	auto top = lua_gettop(m_LuaState);
	tolua_AllToLua_open(m_LuaState);
	ASSERT(top == lua_gettop(m_LuaState));
	cManualBindings::Bind(m_LuaState);
	ASSERT(top == lua_gettop(m_LuaState));
	DeprecatedBindings::Bind(m_LuaState);
	ASSERT(top == lua_gettop(m_LuaState));
	cLuaJson::Bind(*this);
	ASSERT(top == lua_gettop(m_LuaState));
	luaopen_lsqlite3(m_LuaState);
	if (top == lua_gettop(m_LuaState) - 1)
	{
		// lsqlite3 left the "sqlite3" table on the stack, pop it:
		lua_pop(m_LuaState, 1);
	}
	ASSERT(top == lua_gettop(m_LuaState));
	luaopen_lxp(m_LuaState);
	if (top == lua_gettop(m_LuaState) - 1)
	{
		// lxp left the unregistered "lxp" table on the stack, register and pop it (#3304):
		lua_setglobal(m_LuaState, "lxp");
	}
	ASSERT(top == lua_gettop(m_LuaState));
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

	// Invalidate all tracked refs:
	{
		cCSLock Lock(m_CSTrackedRefs);
		for (auto & r: m_TrackedRefs)
		{
			r->Invalidate();
		}
		m_TrackedRefs.clear();
	}

	cCanonLuaStates::Remove(*this);
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
	ASSERT_LUA_STACK_BALANCE(m_LuaState);

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
	lua_pop(m_LuaState, 1);                                                          // Stk: -
}





bool cLuaState::LoadFile(const AString & a_FileName, bool a_LogWarnings)
{
	ASSERT(IsValid());
	ASSERT_LUA_STACK_BALANCE(m_LuaState);

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
	s = lua_pcall(m_LuaState, 0, 0, 0);
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
	ASSERT_LUA_STACK_BALANCE(m_LuaState);

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

	ASSERT_LUA_STACK_BALANCE(m_LuaState);
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





bool cLuaState::PushFunction(const cRef & a_FnRef)
{
	ASSERT(IsValid());
	ASSERT(m_NumCurrentFunctionArgs == -1);  // If not, there's already something pushed onto the stack

	// Push the error handler for lua_pcall()
	lua_pushcfunction(m_LuaState, &ReportFnCallErrors);

	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, static_cast<int>(a_FnRef));  // same as lua_getref()
	if (!lua_isfunction(m_LuaState, -1))
	{
		lua_pop(m_LuaState, 2);
		return false;
	}
	m_CurrentFunctionName = "<callback>";
	m_NumCurrentFunctionArgs = 0;
	return true;
}





bool cLuaState::PushFunction(const cRef & a_TableRef, const char * a_FnName)
{
	ASSERT(IsValid());
	ASSERT(m_NumCurrentFunctionArgs == -1);  // If not, there's already something pushed onto the stack

	if (!a_TableRef.IsValid())
	{
		return false;
	}

	// Push the error handler for lua_pcall()
	lua_pushcfunction(m_LuaState, &ReportFnCallErrors);

	// Get the function from the table:
	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, static_cast<int>(a_TableRef));
	if (!lua_istable(m_LuaState, -1))
	{
		// Not a table, bail out
		lua_pop(m_LuaState, 2);
		return false;
	}
	lua_getfield(m_LuaState, -1, a_FnName);
	if (lua_isnil(m_LuaState, -1) || !lua_isfunction(m_LuaState, -1))
	{
		// Not a valid function, bail out
		lua_pop(m_LuaState, 3);
		return false;
	}

	// Pop the table off the stack:
	lua_remove(m_LuaState, -2);

	Printf(m_CurrentFunctionName, "<table-callback %s>", a_FnName);
	m_NumCurrentFunctionArgs = 0;
	return true;
}





void cLuaState::Push(const AString & a_String)
{
	ASSERT(IsValid());

	lua_pushlstring(m_LuaState, a_String.data(), a_String.size());
}





void cLuaState::Push(const AStringMap & a_Dictionary)
{
	ASSERT(IsValid());

	lua_createtable(m_LuaState, 0, static_cast<int>(a_Dictionary.size()));
	int newTable = lua_gettop(m_LuaState);
	for (const auto & item: a_Dictionary)
	{
		Push(item.first);   // key
		Push(item.second);  // value
		lua_rawset(m_LuaState, newTable);
	}
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
}





void cLuaState::Push(const char * a_Value)
{
	ASSERT(IsValid());

	tolua_pushstring(m_LuaState, a_Value);
}





void cLuaState::Push(const cItem & a_Item)
{
	ASSERT(IsValid());
	auto c = new cItem(a_Item);
	tolua_pushusertype_and_takeownership(m_LuaState, c, "cItem");
}





void cLuaState::Push(const cNil & a_Nil)
{
	ASSERT(IsValid());

	lua_pushnil(m_LuaState);
}





void cLuaState::Push(const cLuaState::cRef & a_Ref)
{
	ASSERT(IsValid());

	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, static_cast<int>(a_Ref));
}





void cLuaState::Push(const Vector3d & a_Vector)
{
	ASSERT(IsValid());
	auto c = new Vector3d(a_Vector);
	tolua_pushusertype_and_takeownership(m_LuaState, c, "Vector3<double>");
}





void cLuaState::Push(const Vector3i & a_Vector)
{
	ASSERT(IsValid());
	auto c = new Vector3i(a_Vector);
	tolua_pushusertype_and_takeownership(m_LuaState, c, "Vector3<int>");
}





void cLuaState::Push(bool a_Value)
{
	ASSERT(IsValid());

	tolua_pushboolean(m_LuaState, a_Value ? 1 : 0);
}





void cLuaState::Push(const cEntity * a_Entity)
{
	// Once we can make Lua understand constness, this function shall receive a corresponding function body
	Push(const_cast<cEntity * >(a_Entity));
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
		const char * ClassName = [&]
			{
				switch (a_Entity->GetEntityType())
				{
					case cEntity::etBoat:         return "cBoat";
					case cEntity::etExpOrb:       return "cExpOrb";
					case cEntity::etFallingBlock: return "cFallingBlock";
					case cEntity::etFloater:      return "cFloater";
					case cEntity::etItemFrame:    return "cItemFrame";
					case cEntity::etLeashKnot:    return "cLeashKnot";
					case cEntity::etPainting:     return "cPainting";
					case cEntity::etPickup:       return "cPickup";
					case cEntity::etPlayer:       return "cPlayer";
					case cEntity::etTNT:          return "cTNTEntity";

					case cEntity::etMonster:
					{
						// Don't push specific mob types, as those are not exported in the API:
						return "cMonster";
					}
					case cEntity::etProjectile:
					{
						// Push the specific projectile type:
						return a_Entity->GetClass();
					}

					case cEntity::etEntity:
					case cEntity::etEnderCrystal:
					case cEntity::etMinecart:
					{
						// Push the generic entity class type:
						return "cEntity";
					}
				}  // switch (EntityType)
				UNREACHABLE("Unsupported entity type");
			}();
		tolua_pushusertype(m_LuaState, a_Entity, ClassName);
	}
}





void cLuaState::Push(cLuaServerHandle * a_ServerHandle)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_ServerHandle, "cServerHandle");
}





void cLuaState::Push(cLuaTCPLink * a_TCPLink)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_TCPLink, "cTCPLink");
}





void cLuaState::Push(cLuaUDPEndpoint * a_UDPEndpoint)
{
	ASSERT(IsValid());

	tolua_pushusertype(m_LuaState, a_UDPEndpoint, "cUDPEndpoint");
}





void cLuaState::Push(double a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, a_Value);
}





void cLuaState::Push(int a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, a_Value);
}





void cLuaState::Push(long a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, static_cast<lua_Number>(a_Value));
}





void cLuaState::Push(UInt32 a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, a_Value);
}





void cLuaState::Push(std::chrono::milliseconds a_Value)
{
	ASSERT(IsValid());

	tolua_pushnumber(m_LuaState, static_cast<lua_Number>(a_Value.count()));
}





void cLuaState::Pop(int a_NumValuesToPop)
{
	ASSERT(IsValid());

	lua_pop(m_LuaState, a_NumValuesToPop);
}





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





bool cLuaState::GetStackValue(int a_StackPos, AStringMap & a_Value)
{
	// Retrieve all values in a string => string dictionary table:
	if (!lua_istable(m_LuaState, a_StackPos))
	{
		return false;
	}
	cStackTable tbl(*this, a_StackPos);
	bool isValid = true;
	tbl.ForEachElement([&isValid, &a_Value](cLuaState & a_LuaState)
		{
			AString key, val;
			if (a_LuaState.GetStackValues(-2, key, val))
			{
				a_Value[key] = val;
			}
			else
			{
				isValid = false;
				return true;
			}
			return false;
		}
	);
	return isValid;
}





bool cLuaState::GetStackValue(int a_StackPos, AStringVector & a_Value)
{
	// Retrieve all values in an array of string table:
	if (!lua_istable(m_LuaState, a_StackPos))
	{
		return false;
	}
	cStackTable tbl(*this, a_StackPos);
	bool isValid = true;
	tbl.ForEachArrayElement([&](cLuaState & a_LuaState, int a_Index)
		{
			AString tempStr;
			if (a_LuaState.GetStackValue(-1, tempStr))
			{
				a_Value.push_back(std::move(tempStr));
			}
			else
			{
				isValid = false;
				return true;
			}
			return false;
		}
	);
	return isValid;
}





bool cLuaState::GetStackValue(int a_StackPos, bool & a_ReturnedVal)
{
	a_ReturnedVal = (tolua_toboolean(m_LuaState, a_StackPos, a_ReturnedVal ? 1 : 0) > 0);
	return true;
}





bool cLuaState::GetStackValue(int a_StackPos, cCallback & a_Callback)
{
	return a_Callback.RefStack(*this, a_StackPos);
}





bool cLuaState::GetStackValue(int a_StackPos, cCallbackPtr & a_Callback)
{
	if (a_Callback == nullptr)
	{
		a_Callback = std::make_unique<cCallback>();
	}
	return a_Callback->RefStack(*this, a_StackPos);
}





bool cLuaState::GetStackValue(int a_StackPos, cOptionalCallback & a_Callback)
{
	return a_Callback.RefStack(*this, a_StackPos);
}





bool cLuaState::GetStackValue(int a_StackPos, cOptionalCallbackPtr & a_Callback)
{
	if (a_Callback == nullptr)
	{
		a_Callback = std::make_unique<cOptionalCallback>();
	}
	return a_Callback->RefStack(*this, a_StackPos);
}





bool cLuaState::GetStackValue(int a_StackPos, cCallbackSharedPtr & a_Callback)
{
	if (a_Callback == nullptr)
	{
		a_Callback = std::make_shared<cCallback>();
	}
	return a_Callback->RefStack(*this, a_StackPos);
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





bool cLuaState::GetStackValue(int a_StackPos, cStackTablePtr & a_StackTable)
{
	// Only allow tables to be stored in a_StackTable:
	if (!lua_istable(m_LuaState, a_StackPos))
	{
		return false;
	}

	// Assign the StackTable to the specified stack position:
	a_StackTable = std::make_unique<cStackTable>(*this, a_StackPos);
	return true;
}





bool cLuaState::GetStackValue(int a_StackPos, cTableRef & a_TableRef)
{
	return a_TableRef.RefStack(*this, a_StackPos);
}





bool cLuaState::GetStackValue(int a_StackPos, cTableRefPtr & a_TableRef)
{
	if (a_TableRef == nullptr)
	{
		a_TableRef = std::make_unique<cTableRef>();
	}
	return a_TableRef->RefStack(*this, a_StackPos);
}





bool cLuaState::GetStackValue(int a_StackPos, cTrackedRef & a_Ref)
{
	return a_Ref.RefStack(*this, a_StackPos);
}





bool cLuaState::GetStackValue(int a_StackPos, cTrackedRefPtr & a_Ref)
{
	if (a_Ref == nullptr)
	{
		a_Ref = std::make_unique<cTrackedRef>();
	}
	return a_Ref->RefStack(*this, a_StackPos);
}





bool cLuaState::GetStackValue(int a_StackPos, cTrackedRefSharedPtr & a_Ref)
{
	if (a_Ref == nullptr)
	{
		a_Ref = std::make_shared<cTrackedRef>();
	}
	return a_Ref->RefStack(*this, a_StackPos);
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





bool cLuaState::GetStackValue(int a_StackPos, cUUID & a_Value)
{
	if (lua_isnil(m_LuaState, a_StackPos))
	{
		return false;
	}

	tolua_Error tolua_Err;
	if (tolua_isusertype(m_LuaState, a_StackPos, "cUUID", 0, &tolua_Err))
	{
		// Found a cUUID, copy into output value
		cUUID * PtrUUID = nullptr;
		GetStackValue(a_StackPos, PtrUUID);
		if (PtrUUID == nullptr)
		{
			return false;
		}
		a_Value = *PtrUUID;
		return true;
	}

	// Try to get a string and parse as a UUID into the output
	AString StrUUID;
	if (!GetStackValue(a_StackPos, StrUUID))
	{
		return false;
	}
	return a_Value.FromString(StrUUID);
}





template <typename T>
bool cLuaState::GetStackValue(int a_StackPos, Vector3<T> & a_ReturnedVal)
{
	tolua_Error err;
	if (lua_isnil(m_LuaState, a_StackPos))
	{
		return false;
	}
	if (tolua_isusertype(m_LuaState, a_StackPos, "Vector3<double>", 0, &err))
	{
		a_ReturnedVal = **(static_cast<const Vector3d **>(lua_touserdata(m_LuaState, a_StackPos)));
		return true;
	}
	if (tolua_isusertype(m_LuaState, a_StackPos, "Vector3<float>", 0, &err))
	{
		a_ReturnedVal = **(static_cast<const Vector3f **>(lua_touserdata(m_LuaState, a_StackPos)));
		return true;
	}
	if (tolua_isusertype(m_LuaState, a_StackPos, "Vector3<int>", 0, &err))
	{
		a_ReturnedVal = **(static_cast<const Vector3i **>(lua_touserdata(m_LuaState, a_StackPos)));
		return true;
	}

	// Bonus: Allow simple tables to work as Vector3:
	if (lua_istable(m_LuaState, a_StackPos))
	{
		lua_rawgeti(m_LuaState, a_StackPos, 1);
		lua_rawgeti(m_LuaState, a_StackPos, 2);
		lua_rawgeti(m_LuaState, a_StackPos, 3);
		T x, y, z;
		if (!GetStackValues(-3, x, y, z))
		{
			return false;
		}
		a_ReturnedVal = Vector3<T>(x, y, z);
		return true;
	}

	return false;
}

// Explicitly instantiate the previous function for all Vector3 types:
template bool cLuaState::GetStackValue(int a_StackPos, Vector3d & a_ReturnedVal);
template bool cLuaState::GetStackValue(int a_StackPos, Vector3f & a_ReturnedVal);
template bool cLuaState::GetStackValue(int a_StackPos, Vector3i & a_ReturnedVal);





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
	return cStackValue(*this);
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

		// Remove the error handler and error message from the stack:
		auto top = lua_gettop(m_LuaState);
		if (top < 2)
		{
			LogStackValues(Printf("The Lua stack is in an unexpected state, expected at least two values there, but got %d", top).c_str());
		}
		lua_pop(m_LuaState, std::min(2, top));
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





bool cLuaState::CheckParamBool(int a_StartParam, int a_EndParam)
{
	ASSERT(IsValid());

	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}

	tolua_Error tolua_err;
	for (int i = a_StartParam; i <= a_EndParam; i++)
	{
		if (tolua_isboolean(m_LuaState, i, 0, &tolua_err))
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





bool cLuaState::CheckParamVector3(int a_StartParam, int a_EndParam)
{
	ASSERT(IsValid());

	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}

	for (int i = a_StartParam; i <= a_EndParam; ++i)
	{
		if (IsParamVector3(a_StartParam))
		{
			continue;
		}

		ApiParamError("Failed to read parameter #%d. Vector3 expected, got %s", i, GetTypeText(i).c_str());
		return false;
	}
	return true;
}





bool cLuaState::CheckParamUUID(int a_StartParam, int a_EndParam)
{
	ASSERT(IsValid());

	if (a_EndParam < 0)
	{
		a_EndParam = a_StartParam;
	}

	cUUID tempUUID;
	AString tempStr;
	// Accept either a cUUID or a string that contains a valid UUID
	for (int i = a_StartParam; i <= a_EndParam; ++i)
	{
		tolua_Error err;
		if (tolua_isusertype(m_LuaState, i, "cUUID", 0, &err) && !lua_isnil(m_LuaState, i))
		{
			continue;
		}

		if (!tolua_iscppstring(m_LuaState, i, 0, &err))
		{
			ApiParamError("Failed to read parameter #%d. UUID expected, got %s", i, GetTypeText(i).c_str());
			return false;
		}

		// Check string is a valid UUID
		GetStackValue(i, tempStr);
		if (!tempUUID.FromString(tempStr))
		{
			ApiParamError("Failed to read parameter #%d. UUID expected, got non-UUID string:\n\t\"%s\"", i, tempStr.c_str());
			return false;
		}
	}
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





bool cLuaState::CheckParamSelf(const char * a_SelfClassName)
{
	tolua_Error tolua_err;
	if (tolua_isusertype(m_LuaState, 1, a_SelfClassName, 0, &tolua_err) && !lua_isnil(m_LuaState, 1))
	{
		return true;
	}

	// Not the correct parameter
	lua_Debug entry;
	VERIFY(lua_getstack(m_LuaState, 0,   &entry));
	VERIFY(lua_getinfo (m_LuaState, "n", &entry));
	AString ErrMsg = Printf(
		"Error in function '%s'. The 'self' parameter is not of the expected type, \"instance of %s\". " \
		"Make sure you're using the correct calling convention (obj:fn() instead of obj.fn()).",
		(entry.name != nullptr) ? entry.name : "<unknown>", a_SelfClassName
	);
	tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
	return false;
}





bool cLuaState::CheckParamStaticSelf(const char * a_SelfClassName)
{
	tolua_Error tolua_err;
	if (tolua_isusertable(m_LuaState, 1, a_SelfClassName, 0, &tolua_err) && !lua_isnil(m_LuaState, 1))
	{
		return true;
	}

	// Not the correct parameter
	lua_Debug entry;
	VERIFY(lua_getstack(m_LuaState, 0,   &entry));
	VERIFY(lua_getinfo (m_LuaState, "n", &entry));
	AString ErrMsg = Printf(
		"Error in function '%s'. The 'self' parameter is not of the expected type, \"class %s\". " \
		"Make sure you're using the correct calling convention (cClassName:fn() instead of cClassName.fn() or obj:fn()).",
		(entry.name != nullptr) ? entry.name : "<unknown>", a_SelfClassName
	);
	tolua_error(m_LuaState, ErrMsg.c_str(), &tolua_err);
	return false;
}





bool cLuaState::IsParamUserType(int a_ParamIdx, const AString & a_UserType)
{
	ASSERT(IsValid());

	tolua_Error tolua_err;
	return (tolua_isusertype(m_LuaState, a_ParamIdx, a_UserType.c_str(), 0, &tolua_err) == 1);
}





bool cLuaState::IsParamNumber(int a_ParamIdx)
{
	ASSERT(IsValid());

	tolua_Error tolua_err;
	return (tolua_isnumber(m_LuaState, a_ParamIdx, 0, &tolua_err) == 1);
}





bool cLuaState::IsParamVector3(int a_ParamIdx)
{
	ASSERT(IsValid());

	return (
		IsParamUserType(a_ParamIdx, "Vector3<double>") ||
		IsParamUserType(a_ParamIdx, "Vector3<float>") ||
		IsParamUserType(a_ParamIdx, "Vector3<int>") ||
		lua_istable(m_LuaState, a_ParamIdx)  // Assume any table is good enough
	);
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





int cLuaState::ApiParamError(std::string_view a_Msg)
{
	// Retrieve current function name
	lua_Debug entry;
	VERIFY(lua_getstack(m_LuaState, 0, &entry));
	VERIFY(lua_getinfo(m_LuaState, "n", &entry));

	// Compose the error message:
	AString errorMsg = fmt::format("{0}: {1}", (entry.name != nullptr) ? entry.name : "<unknown function>", a_Msg);

	// Log everything into the console:
	LOGWARNING("%s", errorMsg.c_str());
	// cLuaState::LogStackTrace(a_LuaState);  // Do NOT log stack trace, it is already output as part of the Lua error handling
	LogStackValues(m_LuaState, "Parameters on the stack");

	// Raise Lua error:
	lua_pushstring(m_LuaState, errorMsg.c_str());
	return lua_error(m_LuaState);
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





int cLuaState::CopyStackFrom(cLuaState & a_SrcLuaState, int a_SrcStart, int a_SrcEnd, int a_NumAllowedNestingLevels)
{
	/*
	// DEBUG:
	LOGD("Copying stack values from %d to %d", a_SrcStart, a_SrcEnd);
	a_SrcLuaState.LogStack("Src stack before copying:");
	LogStack("Dst stack before copying:");
	*/
	for (int i = a_SrcStart; i <= a_SrcEnd; ++i)
	{
		if (!CopySingleValueFrom(a_SrcLuaState, i, a_NumAllowedNestingLevels))
		{
			lua_pop(m_LuaState, i - a_SrcStart);
			return -1;
		}
	}
	return a_SrcEnd - a_SrcStart + 1;
}





bool cLuaState::CopyTableFrom(cLuaState & a_SrcLuaState, int a_SrcStackIdx, int a_NumAllowedNestingLevels)
{
	// Create the dest table:
	#ifdef _DEBUG
		auto srcTop = lua_gettop(a_SrcLuaState);
		auto dstTop = lua_gettop(m_LuaState);
	#endif
	lua_createtable(m_LuaState, 0, 0);            // DST: <table>
	lua_pushvalue(a_SrcLuaState, a_SrcStackIdx);  // SRC: <table>
	lua_pushnil(a_SrcLuaState);                   // SRC: <table> <key>
	while (lua_next(a_SrcLuaState, -2) != 0)      // SRC: <table> <key> <value>
	{
		ASSERT(lua_gettop(a_SrcLuaState) == srcTop + 3);
		ASSERT(lua_gettop(m_LuaState) == dstTop + 1);

		// Copy the key:
		if (!CopySingleValueFrom(a_SrcLuaState, -2, a_NumAllowedNestingLevels))  // DST: <table> <key>
		{
			lua_pop(m_LuaState, 1);
			lua_pop(a_SrcLuaState, 3);
			ASSERT(lua_gettop(a_SrcLuaState) == srcTop);
			ASSERT(lua_gettop(m_LuaState) == dstTop);
			return false;
		}
		ASSERT(lua_gettop(a_SrcLuaState) == srcTop + 3);
		ASSERT(lua_gettop(m_LuaState) == dstTop + 2);

		// Copy the value:
		if (!CopySingleValueFrom(a_SrcLuaState, -1, a_NumAllowedNestingLevels - 1))  // DST: <table> <key> <value>
		{
			lua_pop(m_LuaState, 2);     // DST: empty
			lua_pop(a_SrcLuaState, 3);  // SRC: empty
			ASSERT(lua_gettop(a_SrcLuaState) == srcTop);
			ASSERT(lua_gettop(m_LuaState) == dstTop);
			return false;
		}
		ASSERT(lua_gettop(a_SrcLuaState) == srcTop + 3);
		ASSERT(lua_gettop(m_LuaState) == dstTop + 3);

		// Set the value and fix up stacks:
		lua_rawset(m_LuaState, -3);  // DST: <table>
		lua_pop(a_SrcLuaState, 1);  // SRC: <table> <key>
		ASSERT(lua_gettop(a_SrcLuaState) == srcTop + 2);
		ASSERT(lua_gettop(m_LuaState) == dstTop + 1);
	}
	lua_pop(a_SrcLuaState, 1);  // SRC: empty
	ASSERT(lua_gettop(a_SrcLuaState) == srcTop);
	ASSERT(lua_gettop(m_LuaState) == dstTop + 1);
	return true;
}





bool cLuaState::CopySingleValueFrom(cLuaState & a_SrcLuaState, int a_StackIdx, int a_NumAllowedNestingLevels)
{
	int t = lua_type(a_SrcLuaState, a_StackIdx);
	switch (t)
	{
		case LUA_TNIL:
		{
			lua_pushnil(m_LuaState);
			return true;
		}
		case LUA_TSTRING:
		{
			AString s;
			a_SrcLuaState.ToString(a_StackIdx, s);
			Push(s);
			return true;
		}
		case LUA_TBOOLEAN:
		{
			bool b = (tolua_toboolean(a_SrcLuaState, a_StackIdx, false) != 0);
			Push(b);
			return true;
		}
		case LUA_TNUMBER:
		{
			lua_Number d = tolua_tonumber(a_SrcLuaState, a_StackIdx, 0);
			Push(d);
			return true;
		}
		case LUA_TUSERDATA:
		{
			// Get the class name:
			const char * type = nullptr;
			if (lua_getmetatable(a_SrcLuaState, a_StackIdx) == 0)
			{
				LOGWARNING("%s: Unknown class in pos %d, cannot copy.", __FUNCTION__, a_StackIdx);
				return false;
			}
			lua_rawget(a_SrcLuaState, LUA_REGISTRYINDEX);  // Stack +1
			type = lua_tostring(a_SrcLuaState, -1);
			lua_pop(a_SrcLuaState, 1);                     // Stack -1

			// Copy the value:
			void * ud = tolua_touserdata(a_SrcLuaState, a_StackIdx, nullptr);
			tolua_pushusertype(m_LuaState, ud, type);
			return true;
		}
		case LUA_TTABLE:
		{
			if (!CopyTableFrom(a_SrcLuaState, a_StackIdx, a_NumAllowedNestingLevels - 1))
			{
				LOGWARNING("%s: Failed to copy table in pos %d.", __FUNCTION__, a_StackIdx);
				return false;
			}
			return true;
		}
		default:
		{
			LOGWARNING("%s: Unsupported value: '%s' at stack position %d. Can only copy numbers, strings, bools, classes and simple tables!",
				__FUNCTION__, lua_typename(a_SrcLuaState, t), a_StackIdx
			);
			return false;
		}
	}
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





void cLuaState::LogStackValues(const char * a_Header)
{
	LogStackValues(m_LuaState, a_Header);
}





void cLuaState::LogStackValues(lua_State * a_LuaState, const char * a_Header)
{
	// Format string consisting only of %s is used to appease the compiler
	ASSERT_LUA_STACK_BALANCE(a_LuaState, false);
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
			case LUA_TSTRING:
			{
				size_t len;
				const char * txt = lua_tolstring(a_LuaState, i, &len);
				Value.assign(txt, std::min<size_t>(len, 50));  // Only log up to 50 characters of the string
				break;
			}
			case LUA_TTABLE:         Printf(Value, "%p", lua_topointer(a_LuaState, i)); break;
			case LUA_TFUNCTION:      Printf(Value, "%p", lua_topointer(a_LuaState, i)); break;
			case LUA_TUSERDATA:
			{
				Printf(Value, "%p (%s)", lua_touserdata(a_LuaState, i), tolua_typename(a_LuaState, i));
				// tolua_typename pushes the string onto Lua stack, pop it off again:
				lua_pop(a_LuaState, 1);
				break;
			}
			default: break;
		}
		LOGD("  Idx %d: type %d (%s) %s", i, Type, lua_typename(a_LuaState, Type), Value.c_str());
	}  // for i - stack idx
}





cLuaState * cLuaState::QueryCanonLuaState(void) const
{
	return cCanonLuaStates::GetCanonState(m_LuaState);
}





void cLuaState::LogApiCallParamFailure(const char * a_FnName, const char * a_ParamNames)
{
	LOGWARNING("%s: Cannot read params: %s, bailing out.", a_FnName, a_ParamNames);
	LogStackTrace();
	LogStackValues("Values on the stack");
}





void cLuaState::TrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect)
{
	a_DeadlockDetect.TrackCriticalSection(m_CS, Printf("cLuaState %s", m_SubsystemName.c_str()));
}





void cLuaState::UntrackInDeadlockDetect(cDeadlockDetect & a_DeadlockDetect)
{
	a_DeadlockDetect.UntrackCriticalSection(m_CS);
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
	ASSERT_LUA_STACK_BALANCE(a_LuaState);
	// Call the BreakIntoDebugger function, if available:
	lua_getglobal(a_LuaState, "BreakIntoDebugger");
	if (!lua_isfunction(a_LuaState, -1))
	{
		LOGD("LUA: BreakIntoDebugger() not found / not a function");
		lua_pop(a_LuaState, 1);
		return 1;
	}
	lua_pushvalue(a_LuaState, -2);  // Copy the string that has been passed to us
	LOGD("Calling BreakIntoDebugger()...");
	lua_call(a_LuaState, 1, 0);
	LOGD("Returned from BreakIntoDebugger().");
	return 0;
}





void cLuaState::TrackRef(cTrackedRef & a_Ref)
{
	// Get the CanonLuaState global from Lua:
	auto canonState = QueryCanonLuaState();
	if (canonState == nullptr)
	{
		LOGWARNING("%s: Lua state %p has invalid CanonLuaState!", __FUNCTION__, static_cast<void *>(m_LuaState));
		return;
	}

	// Add the callback:
	cCSLock Lock(canonState->m_CSTrackedRefs);
	canonState->m_TrackedRefs.push_back(&a_Ref);
}





void cLuaState::UntrackRef(cTrackedRef & a_Ref)
{
	// Get the CanonLuaState global from Lua:
	auto canonState = QueryCanonLuaState();
	if (canonState == nullptr)
	{
		LOGWARNING("%s: Lua state %p has invalid CanonLuaState!", __FUNCTION__, static_cast<void *>(m_LuaState));
		return;
	}

	// Remove the callback (note that another thread may have cleared the callbacks by closing the LuaState):
	cCSLock Lock(canonState->m_CSTrackedRefs);
	auto & trackedRefs = canonState->m_TrackedRefs;
	for (auto itr = trackedRefs.begin(), end = trackedRefs.end(); itr != end; ++itr)
	{
		if (*itr == &a_Ref)
		{
			trackedRefs.erase(itr);
			break;
		}
	}
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
	ASSERT(cCanonLuaStates::GetCanonState(a_LuaState)->m_CS.IsLockedByCurrentThread());
	m_LuaState = a_LuaState;
	lua_pushvalue(a_LuaState, a_StackPos);  // Push a copy of the value at a_StackPos onto the stack
	m_Ref = luaL_ref(a_LuaState, LUA_REGISTRYINDEX);
}





void cLuaState::cRef::UnRef(void)
{
	if (IsValid())
	{
		ASSERT(cCanonLuaStates::GetCanonState(m_LuaState)->m_CS.IsLockedByCurrentThread());
		luaL_unref(m_LuaState, LUA_REGISTRYINDEX, m_Ref);
	}
	m_LuaState = nullptr;
	m_Ref = LUA_REFNIL;
}




