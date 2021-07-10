
// ManualBindings.h

// Declares the cManualBindings class used as a namespace for functions exported to the Lua API manually





#pragma once

#include "LuaState.h"




// fwd:
struct tolua_Error;
class cPluginLua;





/** Provides namespace for the bindings. */
class cManualBindings
{
public:
	/** Binds all the manually implemented functions to tolua_S. */
	static void Bind(lua_State * tolua_S);

protected:
	/** Binds the manually implemented cNetwork-related API to tolua_S.
	Implemented in ManualBindings_Network.cpp. */
	static void BindNetwork(lua_State * tolua_S);

	/** Binds the manually implemented cRankManager glue code to tolua_S.
	Implemented in ManualBindings_RankManager.cpp. */
	static void BindRankManager(lua_State * tolua_S);

	/** Binds the manually implemented cWorld API functions to tolua_S.
	Implemented in ManualBindings_World.cpp. */
	static void BindWorld(lua_State * tolua_S);

	/** Binds the manually implemented cBlockArea API functions to tlua_S.
	Implemented in ManualBindings_BlockArea.cpp. */
	static void BindBlockArea(lua_State * tolua_S);


public:
	// Helper functions:
	static cPluginLua * GetLuaPlugin(lua_State * L);
	static int tolua_do_error(lua_State * L, const char * a_pMsg, tolua_Error * a_pToLuaError);
	static int vlua_do_error(lua_State * L, const char * a_pFormat, fmt::printf_args a_ArgList);
	template <typename... Args>
	static int lua_do_error(lua_State * L, const char * a_Format, const Args & ... a_Args)
	{
		return vlua_do_error(L, a_Format, fmt::make_printf_args(a_Args...));
	}


	/** Binds the DoWith(ItemName) functions of regular classes. */
	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*DoWithFn)(const AString &, cFunctionRef<bool(Ty2 &)>)
	>
	static int DoWith(lua_State * tolua_S)
	{
		// Check params:
		cLuaState L(tolua_S);
		if (
			!L.CheckParamString(2) ||
			!L.CheckParamFunction(3)
		)
		{
			return 0;
		}

		// Get parameters:
		Ty1 * Self;
		AString ItemName;
		cLuaState::cRef FnRef;
		L.GetStackValues(1, Self, ItemName, FnRef);
		if (Self == nullptr)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'");
		}
		if (ItemName.empty() || (ItemName[0] == 0))
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a non-empty string for parameter #1");
		}
		if (!FnRef.IsValid())
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a valid callback function for parameter #2");
		}

		// Call the DoWith function:
		bool res = (Self->*DoWithFn)(ItemName, [&](Ty2 & a_Item)
			{
				bool ret = false;
				L.Call(FnRef, &a_Item, cLuaState::Return, ret);
				return ret;
			}
		);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	/** Template for static functions DoWith(ItemName), on a type that has a static ::Get() function. */
	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*DoWithFn)(const AString &, cFunctionRef<bool(Ty2 &)>)
	>
	static int StaticDoWith(lua_State * tolua_S)
	{
		// Check params:
		cLuaState L(tolua_S);
		if (
			!L.CheckParamString(2) ||
			!L.CheckParamFunction(3)
		)
		{
			return 0;
		}

		// Get parameters:
		AString ItemName;
		cLuaState::cRef FnRef;
		L.GetStackValues(2, ItemName, FnRef);
		if (ItemName.empty() || (ItemName[0] == 0))
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a non-empty string for parameter #1");
		}
		if (!FnRef.IsValid())
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a valid callback function for parameter #2");
		}

		// Call the DoWith function:
		bool res = (Ty1::Get()->*DoWithFn)(ItemName, [&](Ty2 & a_Item)
			{
				bool ret = false;
				L.Call(FnRef, &a_Item, cLuaState::Return, ret);
				return ret;
			}
		);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*DoWithFn)(UInt32, cFunctionRef<bool(Ty2 &)>)
	>
	static int DoWithID(lua_State * tolua_S)
	{
		// Check params:
		cLuaState L(tolua_S);
		if (
			!L.CheckParamNumber(2) ||
			!L.CheckParamFunction(3)
		)
		{
			return 0;
		}

		// Get parameters:
		Ty1 * Self = nullptr;
		UInt32 ItemID = 0;
		cLuaState::cRef FnRef;
		L.GetStackValues(1, Self, ItemID, FnRef);
		if (Self == nullptr)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'");
		}
		if (!FnRef.IsValid())
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a valid callback function for parameter #2");
		}

		// Call the DoWith function:
		bool res = (Self->*DoWithFn)(ItemID, [&](Ty2 & a_Item)
			{
				bool ret = false;
				L.Call(FnRef, &a_Item, cLuaState::Return, ret);
				return ret;
			}
		);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*ForEachFn)(cFunctionRef<bool(Ty2 &)>)
	>
	static int ForEach(lua_State * tolua_S)
	{
		// Check params:
		cLuaState L(tolua_S);
		if (
			!L.CheckParamFunction(2) ||
			!L.CheckParamEnd(3)
		)
		{
			return 0;
		}

		// Get the params:
		Ty1 * Self = nullptr;
		cLuaState::cRef FnRef;
		L.GetStackValues(1, Self, FnRef);
		if (Self == nullptr)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'.");
		}
		if (!FnRef.IsValid())
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a valid callback function for parameter #1");
		}

		// Call the enumeration:
		bool res = (Self->*ForEachFn)([&](Ty2 & a_Item)
			{
				bool ret = false;  // By default continue the enumeration
				L.Call(FnRef, &a_Item, cLuaState::Return, ret);
				return ret;
			}
		);

		// Push the return value:
		L.Push(res);
		return 1;
	}





	/** Implements bindings for ForEach() functions in a class that is static (has a ::Get() static function). */
	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*ForEachFn)(cFunctionRef<bool(Ty2 &)>)
	>
	static int StaticForEach(lua_State * tolua_S)
	{
		// Check params:
		cLuaState L(tolua_S);
		if (
			!L.CheckParamFunction(2) ||
			!L.CheckParamEnd(3)
		)
		{
			return 0;
		}

		// Get the params:
		cLuaState::cRef FnRef(L, 2);
		if (!FnRef.IsValid())
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a valid callback function for parameter #1");
		}

		// Call the enumeration:
		bool res = (Ty1::Get()->*ForEachFn)([&](Ty2 & a_Item)
			{
				bool ret = false;  // By default continue the enumeration
				L.Call(FnRef, &a_Item, cLuaState::Return, ret);
				return ret;
			}
		);

		// Push the return value:
		L.Push(res);
		return 1;
	}
};
