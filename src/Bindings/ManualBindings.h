
// ManualBindings.h

// Declares the cManualBindings class used as a namespace for functions exported to the Lua API manually





#pragma once

#include "LuaState.h"




// fwd:
struct tolua_Error;





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


public:
	// Helper functions:
	static cPluginLua * GetLuaPlugin(lua_State * L);
	static int tolua_do_error(lua_State * L, const char * a_pMsg, tolua_Error * a_pToLuaError);
	static int lua_do_error(lua_State * L, const char * a_pFormat, ...);


	/** Binds the DoWith(ItemName) functions of regular classes. */
	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*DoWithFn)(const AString &, cItemCallback<Ty2> &)
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

		class cLuaCallback : public cItemCallback<Ty2>
		{
		public:
			cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
				m_LuaState(a_LuaState),
				m_FnRef(a_FnRef)
			{
			}

		private:
			virtual bool Item(Ty2 * a_Item) override
			{
				bool ret = false;
				m_LuaState.Call(m_FnRef, a_Item, cLuaState::Return, ret);
				return ret;
			}
			cLuaState & m_LuaState;
			cLuaState::cRef & m_FnRef;
		} Callback(L, FnRef);

		// Call the DoWith function:
		bool res = (Self->*DoWithFn)(ItemName, Callback);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	/** Template for static functions DoWith(ItemName), on a type that has a static ::Get() function. */
	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*DoWithFn)(const AString &, cItemCallback<Ty2> &)
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

		class cLuaCallback : public cItemCallback<Ty2>
		{
		public:
			cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
				m_LuaState(a_LuaState),
				m_FnRef(a_FnRef)
			{
			}

		private:
			virtual bool Item(Ty2 * a_Item) override
			{
				bool ret = false;
				m_LuaState.Call(m_FnRef, a_Item, cLuaState::Return, ret);
				return ret;
			}
			cLuaState & m_LuaState;
			cLuaState::cRef & m_FnRef;
		} Callback(L, FnRef);

		// Call the DoWith function:
		bool res = (Ty1::Get()->*DoWithFn)(ItemName, Callback);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*DoWithFn)(UInt32, cItemCallback<Ty2> &)
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
		UInt32 ItemID;
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

		class cLuaCallback : public cItemCallback<Ty2>
		{
		public:
			cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
				m_LuaState(a_LuaState),
				m_FnRef(a_FnRef)
			{
			}

		private:
			virtual bool Item(Ty2 * a_Item) override
			{
				bool ret = false;
				m_LuaState.Call(m_FnRef, a_Item, cLuaState::Return, ret);
				return ret;
			}
			cLuaState & m_LuaState;
			cLuaState::cRef & m_FnRef;
		} Callback(L, FnRef);

		// Call the DoWith function:
		bool res = (Self->*DoWithFn)(ItemID, Callback);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*DoWithFn)(int, int, int, cItemCallback<Ty2> &)
	>
	static int DoWithXYZ(lua_State * tolua_S)
	{
		// Check params:
		cLuaState L(tolua_S);
		if (
			!L.CheckParamNumber(2, 4) ||
			!L.CheckParamFunction(5) ||
			!L.CheckParamEnd(6)
		)
		{
			return 0;
		}

		// Get parameters:
		Ty1 * Self = nullptr;
		int BlockX = 0;
		int BlockY = 0;
		int BlockZ = 0;
		cLuaState::cRef FnRef;
		L.GetStackValues(1, Self, BlockX, BlockY, BlockZ, FnRef);
		if (Self == nullptr)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'");
		}
		if (!FnRef.IsValid())
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a valid callback function for parameter #5");
		}

		class cLuaCallback : public cItemCallback<Ty2>
		{
		public:
			cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
				m_LuaState(a_LuaState),
				m_FnRef(a_FnRef)
			{
			}

		private:
			virtual bool Item(Ty2 * a_Item) override
			{
				bool ret = false;
				m_LuaState.Call(m_FnRef, a_Item, cLuaState::Return, ret);
				return ret;
			}
			cLuaState & m_LuaState;
			cLuaState::cRef & m_FnRef;
		} Callback(L, FnRef);

		// Call the DoWith function:
		bool res = (Self->*DoWithFn)(BlockX, BlockY, BlockZ, Callback);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*ForEachFn)(int, int, cItemCallback<Ty2> &)
	>
	static int ForEachInChunk(lua_State * tolua_S)
	{
		// Check params:
		cLuaState L(tolua_S);
		if (
			!L.CheckParamNumber(2, 3) ||
			!L.CheckParamFunction(4) ||
			!L.CheckParamEnd(5)
		)
		{
			return 0;
		}

		// Get parameters:
		Ty1 * Self = nullptr;
		int ChunkX = 0;
		int ChunkZ = 0;
		cLuaState::cRef FnRef;
		L.GetStackValues(1, Self, ChunkX, ChunkZ, FnRef);
		if (Self == nullptr)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Invalid 'self'");
		}
		if (!FnRef.IsValid())
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a valid callback function for parameter #4");
		}

		class cLuaCallback : public cItemCallback<Ty2>
		{
		public:
			cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
				m_LuaState(a_LuaState),
				m_FnRef(a_FnRef)
			{
			}

		private:
			virtual bool Item(Ty2 * a_Item) override
			{
				bool ret = false;
				m_LuaState.Call(m_FnRef, a_Item, cLuaState::Return, ret);
				return ret;
			}
			cLuaState & m_LuaState;
			cLuaState::cRef & m_FnRef;
		} Callback(L, FnRef);

		// Call the DoWith function:
		bool res = (Self->*ForEachFn)(ChunkX, ChunkZ, Callback);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*ForEachFn)(const cBoundingBox &, cItemCallback<Ty2> &)
	>
	static int ForEachInBox(lua_State * tolua_S)
	{
		// Check params:
		cLuaState L(tolua_S);
		if (
			!L.CheckParamUserType(1, "cWorld") ||
			!L.CheckParamUserType(2, "cBoundingBox") ||
			!L.CheckParamFunction(3) ||
			!L.CheckParamEnd(4)
		)
		{
			return 0;
		}

		// Get the params:
		Ty1 * Self = nullptr;
		cBoundingBox * Box = nullptr;
		cLuaState::cRef FnRef;
		L.GetStackValues(1, Self, Box, FnRef);
		if ((Self == nullptr) || (Box == nullptr))
		{
			LOGWARNING("Invalid world (%p) or boundingbox (%p)", static_cast<void *>(Self), static_cast<void *>(Box));
			L.LogStackTrace();
			return 0;
		}
		if (!FnRef.IsValid())
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a valid callback function for parameter #2");
		}

		// Callback wrapper for the Lua function:
		class cLuaCallback : public cItemCallback<Ty2>
		{
		public:
			cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FuncRef) :
				m_LuaState(a_LuaState),
				m_FnRef(a_FuncRef)
			{
			}

		private:
			cLuaState & m_LuaState;
			cLuaState::cRef & m_FnRef;

			// cItemCallback<Ty2> overrides:
			virtual bool Item(Ty2 * a_Item) override
			{
				bool res = false;
				if (!m_LuaState.Call(m_FnRef, a_Item, cLuaState::Return, res))
				{
					LOGWARNING("Failed to call Lua callback");
					m_LuaState.LogStackTrace();
					return true;  // Abort enumeration
				}

				return res;
			}
		} Callback(L, FnRef);

		bool res = (Self->*ForEachFn)(*Box, Callback);

		// Push the result as the return value:
		L.Push(res);
		return 1;
	}





	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*ForEachFn)(cItemCallback<Ty2> &)
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

		class cLuaCallback : public cItemCallback<Ty2>
		{
		public:
			cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
				m_LuaState(a_LuaState),
				m_FnRef(a_FnRef)
			{
			}

		private:
			cLuaState & m_LuaState;
			cLuaState::cRef & m_FnRef;

			virtual bool Item(Ty2 * a_Item) override
			{
				bool res = false;  // By default continue the enumeration
				m_LuaState.Call(m_FnRef, a_Item, cLuaState::Return, res);
				return res;
			}
		} Callback(L, FnRef);

		// Call the enumeration:
		bool res = (Self->*ForEachFn)(Callback);

		// Push the return value:
		L.Push(res);
		return 1;
	}





	/** Implements bindings for ForEach() functions in a class that is static (has a ::Get() static function). */
	template <
		class Ty1,
		class Ty2,
		bool (Ty1::*ForEachFn)(cItemCallback<Ty2> &)
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

		class cLuaCallback : public cItemCallback<Ty2>
		{
		public:
			cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FnRef):
				m_LuaState(a_LuaState),
				m_FnRef(a_FnRef)
			{
			}

		private:
			cLuaState & m_LuaState;
			cLuaState::cRef & m_FnRef;

			virtual bool Item(Ty2 * a_Item) override
			{
				bool res = false;  // By default continue the enumeration
				m_LuaState.Call(m_FnRef, a_Item, cLuaState::Return, res);
				return res;
			}
		} Callback(L, FnRef);

		// Call the enumeration:
		bool res = (Ty1::Get()->*ForEachFn)(Callback);

		// Push the return value:
		L.Push(res);
		return 1;
	}
};




