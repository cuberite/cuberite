
// ManualBindings_Network.cpp

// Implements the cNetwork-related API bindings for Lua
// Also implements the cUrlClient bindings

#include "Globals.h"
#include "ManualBindings.h"
#include "tolua++/include/tolua++.h"
#include "LuaState.h"
#include <sstream>
extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}




using LuaState = lua_State;




std::string lua_getstring(lua_State* L, int aIndex)
{
	size_t len;
	auto s = lua_tolstring(L, aIndex, &len);
	return std::string(s, len);
}





std::string lua_getstringfield(lua_State* L, int aTableIndex, const char* aFieldName)
{
	lua_getfield(L, aTableIndex, aFieldName);
	std::string res = lua_getstring(L, -1);
	lua_pop(L, 1);
	return res;
}





int pushThreadIdOnLuaStack(lua_State* aState, const std::thread::id& aThreadId)
{
	std::stringstream ss;
	ss << aThreadId;
	auto str = ss.str();
	lua_pushlstring(aState, str.data(), str.size());
	return 1;
}




/** The name of the thread's Lua object's metatable within the Lua registry.
Every thread object that is pushed to the Lua side has the metatable of this name set to it. */
static const char* THREAD_METATABLE_NAME = "std::thread *";
static const char* MUTEX_METATABLE_NAME = "std::mutex *";





/** Dumps the contents of the Lua stack to the specified ostream. */
static void dumpLuaStack(LuaState* L, std::ostream& aDest)
{
	aDest << "Lua stack contents:" << std::endl;
	for (int i = lua_gettop(L); i >= 0; --i)
	{
		aDest << "  " << i << "\t";
		aDest << lua_typename(L, lua_type(L, i)) << "\t";
		aDest << lua_getstring(L, i).c_str() << std::endl;
	}
	aDest << "(stack dump completed)" << std::endl;
}





/** Dumps the call stack to the specified ostream. */
static void dumpLuaTraceback(LuaState* L, std::ostream& aDest)
{
	
	
	//luaL_traceback(L, L, "Stack trace: ", 0);
	aDest << lua_getstring(L, -1).c_str() << std::endl;
	lua_pop(L, 1);
	return;
}





/** Called by Lua when it encounters an unhandler error in the script file. */
extern "C" int errorHandler(LuaState * L)
{
	auto err = lua_getstring(L, -1);
	LOGWARNING(err);
	//std::cerr << "Caught an error: " << err << std::endl;
	//dumpLuaStack(L, std::cerr);
	//exit(1);
	return 0;
}






extern "C" static int mutexNew(LuaState * aState)
{
	cLuaState L(aState);
	lua_pushcfunction(aState, errorHandler);

	// Push the (currently empty) mutex object to the Lua side
	auto mutexObj = reinterpret_cast<std::mutex**>(lua_newuserdata(aState, sizeof(std::mutex**)));
	L.Push(mutexObj);
	//luaL_setmetatable(aState, MUTEX_METATABLE_NAME);

	// Create the new mutex:
	*mutexObj = new std::mutex();
	return 1;
}







/** */
extern "C" static int mutexLock(LuaState * aState)
{
	auto mutexObj = reinterpret_cast<std::mutex**>(luaL_checkudata(aState, 1, MUTEX_METATABLE_NAME));
	if (mutexObj == nullptr)
	{
		luaL_argerror(aState, 0, "'mutex' expected");
		return 0;
	}
	(*mutexObj)->lock();
	auto numParams = lua_gettop(aState);
	luaL_checktype(aState, 2, LUA_TFUNCTION);
	lua_pcall(aState, 0, 0, 0);
	(*mutexObj)->unlock();

	return 0;
}





static int threadNew(LuaState * aState)
{
	static std::recursive_mutex mtx;
	std::scoped_lock lock(mtx);
	luaL_checktype(aState, 1, LUA_TFUNCTION);
	lua_pushvalue(aState, 1);                             // Push a copy of the fn to the top of the stack...
	auto luaFnRef = luaL_ref(aState, LUA_REGISTRYINDEX);  // ... move it to the registry...
	auto luaThread = lua_newthread(aState);
	lua_pushcfunction(aState, errorHandler);
	lua_rawgeti(luaThread, LUA_REGISTRYINDEX, luaFnRef);  // ... push it onto the new thread's stack...
	luaL_unref(aState, LUA_REGISTRYINDEX, luaFnRef);      // ... and remove it from the registry

	// Push the (currently empty) thread object to the Lua side
	auto threadObj = reinterpret_cast<std::thread**>(lua_newuserdata(aState, sizeof(std::thread**)));
	//luaL_setmetatable(aState, THREAD_METATABLE_NAME);

	// Start the new thread:
	*threadObj = new std::thread(
		[luaThread, luaFnRef]()
		{
			auto numParams = lua_gettop(luaThread) - 1;
			lua_call(luaThread, numParams, LUA_MULTRET);
			//if (status == LUA_OK)
		}
	);
	cLuaState L(aState);
	L.Push(threadObj);
	return 1;
}





/** Provides the thread.sleep() function.
Parameter: the number of seconds to sleep for (floating-point number). */
extern "C" static int threadSleep(LuaState * aState)
{
	auto seconds = luaL_checknumber(aState, 1);
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
	return 0;
}





/** Implements the thread:join() function.
Joins the specified thread.
Errors if asked to join the current thread. */
extern "C" static int threadObjJoin(LuaState * aState)
{
	auto threadObj = reinterpret_cast<std::thread**>(luaL_checkudata(aState, 1, THREAD_METATABLE_NAME));
	if (threadObj == nullptr)
	{
		luaL_argerror(aState, 0, "`thread' expected");
		return 0;
	}
	if (*threadObj == nullptr)
	{
		luaL_argerror(aState, 0, "thread already joined");
		return 0;
	}
	if ((*threadObj)->get_id() == std::this_thread::get_id())
	{
		luaL_argerror(aState, 0, "`thread' must not be the current thread");
		return 0;
	}
	(*threadObj)->join();
	*threadObj = nullptr;
	return 0;
}





/** Implements the thread:id() function.
Returns the thread's ID, as an implementation-dependent detail.
The ID is guaranteed to be unique within a single process at any single time moment (but not within multiple time moments). */
extern "C" static int threadObjID(LuaState * aState)
{
	auto threadObj = reinterpret_cast<std::thread**>(luaL_checkudata(aState, 1, THREAD_METATABLE_NAME));
	if (threadObj == nullptr)
	{
		luaL_argerror(aState, 0, "`thread' expected");
		return 0;
	}
	if (*threadObj == nullptr)
	{
		luaL_argerror(aState, 0, "thread already joined");
		return 0;
	}
	if ((*threadObj)->get_id() == std::this_thread::get_id())
	{
		luaL_argerror(aState, 0, "`thread' must not be the current thread");
		return 0;
	}
	return pushThreadIdOnLuaStack(aState, (*threadObj)->get_id());
}





/** Implements the thread.currentid() function.
Returns the current thread's ID. This also works on the main thread. */
extern "C" static int threadCurrentID(LuaState * aState)
{
	return pushThreadIdOnLuaStack(aState, std::this_thread::get_id());
}





/** Called when the Lua side GC's the thread object.
Joins the thread, if not already joined. */
extern "C" static int threadObjGc(LuaState * aState)
{
	auto threadObj = reinterpret_cast<std::thread**>(luaL_checkudata(aState, 1, THREAD_METATABLE_NAME));
	// We shouldn't get an invalid thread object, but let's check nevertheless:
	if (threadObj == nullptr)
	{
		luaL_argerror(aState, 0, "`thread' expected");
		return 0;
	}
	if (*threadObj == nullptr)
	{
		return 0;
	}
	if ((*threadObj)->get_id() == std::this_thread::get_id())
	{
		// Current thread is GC-ing self? No idea if that is allowed to happen, but we don't care; just don't join
		return 0;
	}
	(*threadObj)->join();
	*threadObj = nullptr;
	return 0;
}





/** The functions in the thread library. */
static const luaL_Reg threadFuncs[] =
{
	{"new", &threadNew},
	{"sleep", &threadSleep},
	{"currentid", &threadCurrentID},
	{NULL, NULL}
};





/** The functions of the thread object. */
static const luaL_Reg threadObjFuncs[] =
{
	{"join", &threadObjJoin},
	{"id",   &threadObjID},
	{"__gc", &threadObjGc},
	{NULL, NULL}
};





/***/
static const luaL_Reg mutexFuncs[] =
{
	{"new", &mutexNew},
	{NULL,NULL}
};





/** */
static const luaL_Reg mutexObjFuncs[] =
{
	{"lock", &mutexLock},
	{NULL, NULL}
};





/** Registers the thread library into the Lua VM. */
extern "C" static int luaopen_thread(LuaState * aState)
{
	//luaL_newlib(aState, threadFuncs);

	//// Register the metatable for std::thread objects:
	//luaL_newmetatable(aState, THREAD_METATABLE_NAME);
	//lua_pushvalue(aState, -1);
	//lua_setfield(aState, -2, "__index");  // metatable.__index = metatable
	//luaL_setfuncs(aState, threadObjFuncs, 0);  // Add the object functions to the table
	//lua_pop(aState, 1);  // pop the new metatable

	return 1;
}





/**Registers the mutex library into the Lua VM. */
extern "C" static int luaopen_mutex(LuaState * aState)
{
	//luaL_newlib(aState, mutexFuncs);

	//// Register the metatable for std::thread objects:
	//luaL_newmetatable(aState, MUTEX_METATABLE_NAME);
	//lua_pushvalue(aState, -1);
	//lua_setfield(aState, -2, "__index");  // metatable.__index = metatable
	//luaL_setfuncs(aState, mutexObjFuncs, 0);  // Add the object functions to the table
	//lua_pop(aState, 1);  // pop the new metatable

	return 1;
}





void cManualBindings::BindThreading(lua_State * tolua_S)
{
	tolua_usertype(tolua_S, "cThread");
	tolua_cclass(tolua_S, "cThread", "cThread", "", nullptr);
	tolua_beginmodule(tolua_S, nullptr);
		tolua_beginmodule(tolua_S, "cThread");
			tolua_function(tolua_S, "new", threadNew);
			tolua_function(tolua_S, "sleep", threadSleep);
			tolua_function(tolua_S, "currentid", threadCurrentID);
			tolua_function(tolua_S, "join", threadObjJoin);
			tolua_function(tolua_S, "id", threadObjID);
			/*tolua_function(tolua_S, threadObjFuncs)*/
		tolua_endmodule(tolua_S);
	tolua_endmodule(tolua_S);
}
