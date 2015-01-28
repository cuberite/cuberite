
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ManualBindings.h"
#undef TOLUA_TEMPLATE_BIND
#include "tolua++/include/tolua++.h"
#include "polarssl/md5.h"
#include "PluginLua.h"
#include "PluginManager.h"
#include "LuaWindow.h"
#include "LuaChunkStay.h"
#include "../Root.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../WebAdmin.h"
#include "../ClientHandle.h"
#include "../BlockArea.h"
#include "../BlockEntities/BeaconEntity.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/CommandBlockEntity.h"
#include "../BlockEntities/DispenserEntity.h"
#include "../BlockEntities/DropperEntity.h"
#include "../BlockEntities/FurnaceEntity.h"
#include "../BlockEntities/HopperEntity.h"
#include "../BlockEntities/NoteEntity.h"
#include "../BlockEntities/MobHeadEntity.h"
#include "../BlockEntities/FlowerPotEntity.h"
#include "../LineBlockTracer.h"
#include "../WorldStorage/SchematicFileSerializer.h"
#include "../CompositeChat.h"





// Better error reporting for Lua
static int tolua_do_error(lua_State* L, const char * a_pMsg, tolua_Error * a_pToLuaError)
{
	// Retrieve current function name
	lua_Debug entry;
	VERIFY(lua_getstack(L, 0, &entry));
	VERIFY(lua_getinfo(L, "n", &entry));

	// Insert function name into error msg
	AString msg(a_pMsg);
	ReplaceString(msg, "#funcname#", entry.name?entry.name:"?");

	// Send the error to Lua
	tolua_error(L, msg.c_str(), a_pToLuaError);
	return 0;
}





static int lua_do_error(lua_State* L, const char * a_pFormat, ...)
{
	// Retrieve current function name
	lua_Debug entry;
	VERIFY(lua_getstack(L, 0, &entry));
	VERIFY(lua_getinfo(L, "n", &entry));

	// Insert function name into error msg
	AString msg(a_pFormat);
	ReplaceString(msg, "#funcname#", entry.name?entry.name:"?");

	// Copied from luaL_error and modified
	va_list argp;
	va_start(argp, a_pFormat);
	luaL_where(L, 1);
	lua_pushvfstring(L, msg.c_str(), argp);
	va_end(argp);
	lua_concat(L, 2);
	return lua_error(L);
}





// Lua bound functions with special return types
static int tolua_Clamp(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);
	int NumArgs = lua_gettop(LuaState);
	if (NumArgs != 3)
	{
		return lua_do_error(LuaState, "Error in function call '#funcname#': Requires 3 arguments, got %i", NumArgs);
	}

	if (!lua_isnumber(LuaState, 1) || !lua_isnumber(LuaState, 2) || !lua_isnumber(LuaState, 3))
	{
		return lua_do_error(LuaState, "Error in function call '#funcname#': Expected a number for parameters #1, #2 and #3");
	}

	lua_Number Number = tolua_tonumber(LuaState, 1, 0);
	lua_Number Min = tolua_tonumber(LuaState, 2, 0);
	lua_Number Max = tolua_tonumber(LuaState, 3, 0);

	lua_Number Result = Clamp(Number, Min, Max);
	LuaState.Push(Result);
	return 1;
}





static int tolua_StringSplit(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);
	std::string str   = (std::string)tolua_tocppstring(LuaState, 1, 0);
	std::string delim = (std::string)tolua_tocppstring(LuaState, 2, 0);

	AStringVector Split = StringSplit(str, delim);
	LuaState.Push(Split);
	return 1;
}





static int tolua_StringSplitAndTrim(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);
	std::string str   = (std::string)tolua_tocppstring(LuaState, 1, 0);
	std::string delim = (std::string)tolua_tocppstring(LuaState, 2, 0);

	AStringVector Split = StringSplitAndTrim(str, delim);
	LuaState.Push(Split);
	return 1;
}





/** Retrieves the log message from the first param on the Lua stack.
Can take either a string or a cCompositeChat.
*/
static AString GetLogMessage(lua_State * tolua_S)
{
	tolua_Error err;
	if (tolua_isusertype(tolua_S, 1, "cCompositeChat", false, &err))
	{
		return ((cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr))->ExtractText();
	}
	else
	{
		size_t len = 0;
		const char * str = lua_tolstring(tolua_S, 1, &len);
		if (str != nullptr)
		{
			return AString(str, len);
		}
	}
	return "";
}





static int tolua_LOG(lua_State * tolua_S)
{
	// If the param is a cCompositeChat, read the log level from it:
	cLogger::eLogLevel LogLevel = cLogger::llRegular;
	tolua_Error err;
	if (tolua_isusertype(tolua_S, 1, "cCompositeChat", false, &err))
	{
		LogLevel = cCompositeChat::MessageTypeToLogLevel(((cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr))->GetMessageType());
	}
	
	// Log the message:
	cLogger::GetInstance().LogSimple(GetLogMessage(tolua_S).c_str(), LogLevel);
	return 0;
}





static int tolua_LOGINFO(lua_State * tolua_S)
{
	cLogger::GetInstance().LogSimple(GetLogMessage(tolua_S).c_str(), cLogger::llInfo);
	return 0;
}





static int tolua_LOGWARN(lua_State * tolua_S)
{
	cLogger::GetInstance().LogSimple(GetLogMessage(tolua_S).c_str(), cLogger::llWarning);
	return 0;
}





static int tolua_LOGERROR(lua_State * tolua_S)
{
	cLogger::GetInstance().LogSimple(GetLogMessage(tolua_S).c_str(), cLogger::llError);
	return 0;
}





static int tolua_Base64Encode(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	if (
		!L.CheckParamString(1) ||
		!L.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	AString Src;
	L.GetStackValue(1, Src);
	AString res = Base64Encode(Src);
	L.Push(res);
	return 1;
}





static int tolua_Base64Decode(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	if (
		!L.CheckParamString(1) ||
		!L.CheckParamEnd(2)
	)
	{
		return 0;
	}
	
	AString Src;
	L.GetStackValue(1, Src);
	AString res = Base64Decode(Src);
	L.Push(res);
	return 1;
}





cPluginLua * GetLuaPlugin(lua_State * L)
{
	// Get the plugin identification out of LuaState:
	lua_getglobal(L, LUA_PLUGIN_INSTANCE_VAR_NAME);
	if (!lua_islightuserdata(L, -1))
	{
		LOGWARNING("%s: cannot get plugin instance, what have you done to my Lua state?", __FUNCTION__);
		lua_pop(L, 1);
		return nullptr;
	}
	cPluginLua * Plugin = (cPluginLua *)lua_topointer(L, -1);
	lua_pop(L, 1);
	
	return Plugin;
}





static int tolua_cFile_GetFolderContents(lua_State * tolua_S)
{
	cLuaState LuaState(tolua_S);
	if (
		!LuaState.CheckParamUserTable(1, "cFile") ||
		!LuaState.CheckParamString   (2) ||
		!LuaState.CheckParamEnd      (3)
	)
	{
		return 0;
	}
	
	AString Folder = (AString)tolua_tocppstring(LuaState, 2, 0);

	AStringVector Contents = cFile::GetFolderContents(Folder);
	LuaState.Push(Contents);
	return 1;
}





template <
	class Ty1,
	class Ty2,
	bool (Ty1::*Func1)(const AString &, cItemCallback<Ty2> &)
>
static int tolua_DoWith(lua_State* tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if ((NumArgs != 2) && (NumArgs != 3))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Requires 2 or 3 arguments, got %i", NumArgs);
	}

	Ty1 * self = (Ty1 *)  tolua_tousertype(tolua_S, 1, nullptr);

	const char * ItemName = tolua_tocppstring(tolua_S, 2, "");
	if ((ItemName == nullptr) || (ItemName[0] == 0))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a non-empty string for parameter #1", NumArgs);
	}
	if (!lua_isfunction( tolua_S, 3))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a function for parameter #2", NumArgs);
	}

	/* luaL_ref gets reference to value on top of the stack, the table is the last argument and therefore on the top */
	int TableRef = LUA_REFNIL;
	if (NumArgs == 3)
	{
		TableRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		if (TableRef == LUA_REFNIL)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get value reference of parameter #3", NumArgs);
		}
	}

	/* table value is popped, and now function is on top of the stack */
	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FuncRef == LUA_REFNIL)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #2", NumArgs);
	}

	class cLuaCallback : public cItemCallback<Ty2>
	{
	public:
		cLuaCallback(lua_State* a_LuaState, int a_FuncRef, int a_TableRef)
			: LuaState( a_LuaState)
			, FuncRef( a_FuncRef)
			, TableRef( a_TableRef)
		{}

	private:
		virtual bool Item(Ty2 * a_Item) override
		{
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushusertype(LuaState, a_Item, Ty2::GetClassStatic());
			if (TableRef != LUA_REFNIL)
			{
				lua_rawgeti( LuaState, LUA_REGISTRYINDEX, TableRef);  /* Push table reference */
			}

			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0);
			if (cLuaState::ReportErrors(LuaState, s))
			{
				return true;  // Abort enumeration
			}
			if (lua_isboolean(LuaState, -1))
			{
				return (tolua_toboolean(LuaState, -1, 0) > 0);
			}
			return false;  /* Continue enumeration */
		}
		lua_State * LuaState;
		int FuncRef;
		int TableRef;
	} Callback(tolua_S, FuncRef, TableRef);

	
	bool bRetVal = (self->*Func1)(ItemName, Callback);

	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef);
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef);

	/* Push return value on stack */
	tolua_pushboolean(tolua_S, bRetVal);
	return 1;
}





template <
	class Ty1,
	class Ty2,
	bool (Ty1::*Func1)(int, cItemCallback<Ty2> &)
>
static int tolua_DoWithID(lua_State* tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if ((NumArgs != 2) && (NumArgs != 3))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Requires 2 or 3 arguments, got %i", NumArgs);
	}

	Ty1 * self = (Ty1 *)tolua_tousertype(tolua_S, 1, nullptr);

	int ItemID = (int)tolua_tonumber(tolua_S, 2, 0);
	if (!lua_isfunction(tolua_S, 3))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a function for parameter #2", NumArgs);
	}

	/* luaL_ref gets reference to value on top of the stack, the table is the last argument and therefore on the top */
	int TableRef = LUA_REFNIL;
	if (NumArgs == 3)
	{
		TableRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		if (TableRef == LUA_REFNIL)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get value reference of parameter #3", NumArgs);
		}
	}

	/* table value is popped, and now function is on top of the stack */
	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FuncRef == LUA_REFNIL)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #2", NumArgs);
	}

	class cLuaCallback : public cItemCallback<Ty2>
	{
	public:
		cLuaCallback(lua_State * a_LuaState, int a_FuncRef, int a_TableRef) :
			LuaState(a_LuaState),
			FuncRef(a_FuncRef),
			TableRef(a_TableRef)
		{}

	private:
		virtual bool Item(Ty2 * a_Item) override
		{
			lua_rawgeti(LuaState, LUA_REGISTRYINDEX, FuncRef);            // Push function to call
			tolua_pushusertype(LuaState, a_Item, Ty2::GetClassStatic());  // Push the item
			if (TableRef != LUA_REFNIL)
			{
				lua_rawgeti(LuaState, LUA_REGISTRYINDEX, TableRef);         // Push the optional callbackdata param
			}

			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0);
			if (cLuaState::ReportErrors(LuaState, s))
			{
				return true;  // Abort enumeration
			}
			if (lua_isboolean(LuaState, -1))
			{
				return (tolua_toboolean(LuaState, -1, 0) > 0);
			}
			return false;  /* Continue enumeration */
		}
		lua_State * LuaState;
		int FuncRef;
		int TableRef;
	} Callback(tolua_S, FuncRef, TableRef);

	
	bool bRetVal = (self->*Func1)(ItemID, Callback);

	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef);
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef);

	/* Push return value on stack */
	tolua_pushboolean(tolua_S, bRetVal);
	return 1;
}





template <
	class Ty1,
	class Ty2,
	bool (Ty1::*Func1)(int, int, int, cItemCallback<Ty2> &)
>
static int tolua_DoWithXYZ(lua_State* tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if ((NumArgs != 4) && (NumArgs != 5))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Requires 4 or 5 arguments, got %i", NumArgs);
	}

	Ty1 * self = (Ty1 *)  tolua_tousertype(tolua_S, 1, nullptr);
	if (!lua_isnumber(tolua_S, 2) || !lua_isnumber(tolua_S, 3) || !lua_isnumber(tolua_S, 4))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a number for parameters #1, #2 and #3");
	}

	int ItemX = ((int)tolua_tonumber(tolua_S, 2, 0));
	int ItemY = ((int)tolua_tonumber(tolua_S, 3, 0));
	int ItemZ = ((int)tolua_tonumber(tolua_S, 4, 0));
	if (!lua_isfunction( tolua_S, 5))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a function for parameter #4");
	}

	/* luaL_ref gets reference to value on top of the stack, the table is the last argument and therefore on the top */
	int TableRef = LUA_REFNIL;
	if (NumArgs == 5)
	{
		TableRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		if (TableRef == LUA_REFNIL)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get value reference of parameter #5");
		}
	}

	/* table value is popped, and now function is on top of the stack */
	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FuncRef == LUA_REFNIL)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #4");
	}

	class cLuaCallback : public cItemCallback<Ty2>
	{
	public:
		cLuaCallback(lua_State* a_LuaState, int a_FuncRef, int a_TableRef)
			: LuaState( a_LuaState)
			, FuncRef( a_FuncRef)
			, TableRef( a_TableRef)
		{}

	private:
		virtual bool Item(Ty2 * a_Item) override
		{
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushusertype(LuaState, a_Item, Ty2::GetClassStatic());
			if (TableRef != LUA_REFNIL)
			{
				lua_rawgeti( LuaState, LUA_REGISTRYINDEX, TableRef);  /* Push table reference */
			}

			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0);
			if (cLuaState::ReportErrors(LuaState, s))
			{
				return true;  // Abort enumeration
			}
			if (lua_isboolean(LuaState, -1))
			{
				return (tolua_toboolean(LuaState, -1, 0) > 0);
			}
			return false;  /* Continue enumeration */
		}
		lua_State * LuaState;
		int FuncRef;
		int TableRef;
	} Callback(tolua_S, FuncRef, TableRef);

	bool bRetVal = (self->*Func1)(ItemX, ItemY, ItemZ, Callback);

	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef);
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef);

	/* Push return value on stack */
	tolua_pushboolean(tolua_S, bRetVal);
	return 1;
}





template <
	class Ty1,
	class Ty2,
	bool (Ty1::*Func1)(int, int, cItemCallback<Ty2> &)
>
static int tolua_ForEachInChunk(lua_State * tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if ((NumArgs != 3) && (NumArgs != 4))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Requires 3 or 4 arguments, got %i", NumArgs);
	}

	Ty1 * self = (Ty1 *)  tolua_tousertype(tolua_S, 1, nullptr);
	if (!lua_isnumber(tolua_S, 2) || !lua_isnumber(tolua_S, 3))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a number for parameters #1 and #2");
	}

	int ChunkX = ((int)tolua_tonumber(tolua_S, 2, 0));
	int ChunkZ = ((int)tolua_tonumber(tolua_S, 3, 0));

	if (!lua_isfunction( tolua_S, 4))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a function for parameter #3");
	}

	/* luaL_ref gets reference to value on top of the stack, the table is the last argument and therefore on the top */
	int TableRef = LUA_REFNIL;
	if (NumArgs == 4)
	{
		TableRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		if (TableRef == LUA_REFNIL)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get value reference of parameter #4");
		}
	}

	/* table value is popped, and now function is on top of the stack */
	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FuncRef == LUA_REFNIL)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #3");
	}

	class cLuaCallback : public cItemCallback<Ty2>
	{
	public:
		cLuaCallback(lua_State* a_LuaState, int a_FuncRef, int a_TableRef)
			: LuaState( a_LuaState)
			, FuncRef( a_FuncRef)
			, TableRef( a_TableRef)
		{}

	private:
		virtual bool Item(Ty2 * a_Item) override
		{
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushusertype(LuaState, a_Item, Ty2::GetClassStatic());
			if (TableRef != LUA_REFNIL)
			{
				lua_rawgeti( LuaState, LUA_REGISTRYINDEX, TableRef);  /* Push table reference */
			}

			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0);
			if (cLuaState::ReportErrors(LuaState, s))
			{
				return true;  /* Abort enumeration */
			}

			if (lua_isboolean(LuaState, -1))
			{
				return (tolua_toboolean(LuaState, -1, 0) > 0);
			}
			return false;  /* Continue enumeration */
		}
		lua_State * LuaState;
		int FuncRef;
		int TableRef;
	} Callback(tolua_S, FuncRef, TableRef);

	bool bRetVal = (self->*Func1)(ChunkX, ChunkZ, Callback);

	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef);
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef);

	/* Push return value on stack */
	tolua_pushboolean(tolua_S, bRetVal);
	return 1;
}





template <
	class Ty1,
	class Ty2,
	bool (Ty1::*Func1)(const cBoundingBox &, cItemCallback<Ty2> &)
>
static int tolua_ForEachInBox(lua_State * tolua_S)
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
	L.GetStackValues(1, Self, Box);
	if ((Self == nullptr) || (Box == nullptr))
	{
		LOGWARNING("Invalid world (%p) or boundingbox (%p)", Self, Box);
		L.LogStackTrace();
		return 0;
	}
	
	// Create a reference for the function:
	cLuaState::cRef FnRef(L, 3);

	// Callback wrapper for the Lua function:
	class cLuaCallback : public cItemCallback<Ty2>
	{
	public:
		cLuaCallback(cLuaState & a_LuaState, cLuaState::cRef & a_FuncRef) :
			m_LuaState(a_LuaState),
			m_FnRef(a_FuncRef)
		{}

	private:
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
		cLuaState & m_LuaState;
		cLuaState::cRef & m_FnRef;
	} Callback(L, FnRef);

	bool bRetVal = (Self->*Func1)(*Box, Callback);
	
	FnRef.UnRef();

	/* Push return value on stack */
	tolua_pushboolean(tolua_S, bRetVal);
	return 1;
}





template <
	class Ty1,
	class Ty2,
	bool (Ty1::*Func1)(cItemCallback<Ty2> &)
>
static int tolua_ForEach(lua_State * tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if ((NumArgs != 1) && (NumArgs != 2))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Requires 1 or 2 arguments, got %i", NumArgs);
	}

	Ty1 * self = (Ty1 *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Not called on an object instance");
	}

	if (!lua_isfunction( tolua_S, 2))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a function for parameter #1");
	}

	/* luaL_ref gets reference to value on top of the stack, the table is the last argument and therefore on the top */
	int TableRef = LUA_REFNIL;
	if (NumArgs == 2)
	{
		TableRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		if (TableRef == LUA_REFNIL)
		{
			return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get value reference of parameter #2");
		}
	}

	/* table value is popped, and now function is on top of the stack */
	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FuncRef == LUA_REFNIL)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #1");
	}

	class cLuaCallback : public cItemCallback<Ty2>
	{
	public:
		cLuaCallback(lua_State* a_LuaState, int a_FuncRef, int a_TableRef)
			: LuaState( a_LuaState)
			, FuncRef( a_FuncRef)
			, TableRef( a_TableRef)
		{}

	private:
		virtual bool Item(Ty2 * a_Item) override
		{
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushusertype( LuaState, a_Item, Ty2::GetClassStatic());
			if (TableRef != LUA_REFNIL)
			{
				lua_rawgeti( LuaState, LUA_REGISTRYINDEX, TableRef);  /* Push table reference */
			}

			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0);
			if (cLuaState::ReportErrors(LuaState, s))
			{
				return true;  /* Abort enumeration */
			}

			if (lua_isboolean(LuaState, -1))
			{
				return (tolua_toboolean( LuaState, -1, 0) > 0);
			}
			return false;  /* Continue enumeration */
		}
		lua_State * LuaState;
		int FuncRef;
		int TableRef;
	} Callback(tolua_S, FuncRef, TableRef);

	bool bRetVal = (self->*Func1)(Callback);

	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef);
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef);

	/* Push return value on stack */
	tolua_pushboolean(tolua_S, bRetVal);
	return 1;
}





static int tolua_cWorld_GetBlockInfo(lua_State * tolua_S)
{
	// Exported manually, because tolua would generate useless additional parameters (a_BlockType .. a_BlockSkyLight)
	// Function signature: GetBlockInfo(BlockX, BlockY, BlockZ) -> BlockValid, [BlockType, BlockMeta, BlockSkyLight, BlockBlockLight]
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (tolua_S, 1, "cWorld", 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 4, 0, &tolua_err) ||
		!tolua_isnoobj    (tolua_S, 5, &tolua_err)
		)
		goto tolua_lerror;
	else
	#endif
	{
		cWorld * self       = (cWorld *) tolua_tousertype (tolua_S, 1, nullptr);
		int BlockX          = (int)      tolua_tonumber   (tolua_S, 2, 0);
		int BlockY          = (int)      tolua_tonumber   (tolua_S, 3, 0);
		int BlockZ          = (int)      tolua_tonumber   (tolua_S, 4, 0);
		#ifndef TOLUA_RELEASE
		if (self == nullptr)
		{
			tolua_error(tolua_S, "invalid 'self' in function 'GetBlockInfo'", nullptr);
		}
		#endif
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta, BlockSkyLight, BlockBlockLight;
			bool res = self->GetBlockInfo(BlockX, BlockY, BlockZ, BlockType, BlockMeta, BlockSkyLight, BlockBlockLight);
			tolua_pushboolean(tolua_S, res ? 1 : 0);
			if (res)
			{
				tolua_pushnumber(tolua_S, BlockType);
				tolua_pushnumber(tolua_S, BlockMeta);
				tolua_pushnumber(tolua_S, BlockSkyLight);
				tolua_pushnumber(tolua_S, BlockBlockLight);
				return 5;
			}
		}
	}
	return 1;
	
	#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'GetBlockInfo'.", &tolua_err);
	return 0;
	#endif
}





static int tolua_cWorld_GetBlockTypeMeta(lua_State * tolua_S)
{
	// Exported manually, because tolua would generate useless additional parameters (a_BlockType, a_BlockMeta)
	// Function signature: GetBlockTypeMeta(BlockX, BlockY, BlockZ) -> BlockValid, [BlockType, BlockMeta]
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (tolua_S, 1, "cWorld", 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 4, 0, &tolua_err) ||
		!tolua_isnoobj    (tolua_S, 5, &tolua_err)
		)
		goto tolua_lerror;
	else
	#endif
	{
		cWorld * self       = (cWorld *) tolua_tousertype (tolua_S, 1, nullptr);
		int BlockX          = (int)      tolua_tonumber   (tolua_S, 2, 0);
		int BlockY          = (int)      tolua_tonumber   (tolua_S, 3, 0);
		int BlockZ          = (int)      tolua_tonumber   (tolua_S, 4, 0);
		#ifndef TOLUA_RELEASE
		if (self == nullptr)
		{
			tolua_error(tolua_S, "invalid 'self' in function 'GetBlockTypeMeta'", nullptr);
		}
		#endif
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			bool res = self->GetBlockTypeMeta(BlockX, BlockY, BlockZ, BlockType, BlockMeta);
			tolua_pushboolean(tolua_S, res ? 1 : 0);
			if (res)
			{
				tolua_pushnumber(tolua_S, BlockType);
				tolua_pushnumber(tolua_S, BlockMeta);
				return 3;
			}
		}
	}
	return 1;
	
	#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'GetBlockTypeMeta'.", &tolua_err);
	return 0;
	#endif
}





static int tolua_cWorld_GetSignLines(lua_State * tolua_S)
{
	// Exported manually, because tolua would generate useless additional parameters (a_Line1 .. a_Line4)
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (tolua_S, 1, "cWorld", 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 4, 0, &tolua_err) ||
		!tolua_isnoobj    (tolua_S, 10, &tolua_err)
		)
		goto tolua_lerror;
	else
	#endif
	{
		cWorld * self       = (cWorld *) tolua_tousertype (tolua_S, 1, nullptr);
		int BlockX          = (int)      tolua_tonumber   (tolua_S, 2, 0);
		int BlockY          = (int)      tolua_tonumber   (tolua_S, 3, 0);
		int BlockZ          = (int)      tolua_tonumber   (tolua_S, 4, 0);
		#ifndef TOLUA_RELEASE
		if (self == nullptr)
		{
			tolua_error(tolua_S, "invalid 'self' in function 'GetSignLines'", nullptr);
		}
		#endif
		{
			AString Line1, Line2, Line3, Line4;
			bool res = self->GetSignLines(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4);
			tolua_pushboolean(tolua_S, res ? 1 : 0);
			if (res)
			{
				tolua_pushstring(tolua_S, Line1.c_str());
				tolua_pushstring(tolua_S, Line2.c_str());
				tolua_pushstring(tolua_S, Line3.c_str());
				tolua_pushstring(tolua_S, Line4.c_str());
				return 5;
			}
		}
	}
	return 1;
	
	#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'GetSignLines'.", &tolua_err);
	return 0;
	#endif
}





static int tolua_cWorld_SetSignLines(lua_State * tolua_S)
{
	// Exported manually, because tolua would generate useless additional return values (a_Line1 .. a_Line4)
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (tolua_S, 1, "cWorld", 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 4, 0, &tolua_err) ||
		!tolua_iscppstring(tolua_S, 5, 0, &tolua_err) ||
		!tolua_iscppstring(tolua_S, 6, 0, &tolua_err) ||
		!tolua_iscppstring(tolua_S, 7, 0, &tolua_err) ||
		!tolua_iscppstring(tolua_S, 8, 0, &tolua_err) ||
		!tolua_isusertype (tolua_S, 9, "cPlayer", 1, &tolua_err) ||
		!tolua_isnoobj    (tolua_S, 10, &tolua_err)
		)
		goto tolua_lerror;
	else
	#endif
	{
		cWorld * self       = (cWorld *) tolua_tousertype (tolua_S, 1, nullptr);
		int BlockX          = (int)      tolua_tonumber   (tolua_S, 2, 0);
		int BlockY          = (int)      tolua_tonumber   (tolua_S, 3, 0);
		int BlockZ          = (int)      tolua_tonumber   (tolua_S, 4, 0);
		const AString Line1 =            tolua_tocppstring(tolua_S, 5, 0);
		const AString Line2 =            tolua_tocppstring(tolua_S, 6, 0);
		const AString Line3 =            tolua_tocppstring(tolua_S, 7, 0);
		const AString Line4 =            tolua_tocppstring(tolua_S, 8, 0);
		cPlayer * Player    = (cPlayer *)tolua_tousertype (tolua_S, 9, nullptr);
		#ifndef TOLUA_RELEASE
		if (self == nullptr)
		{
			tolua_error(tolua_S, "invalid 'self' in function 'SetSignLines'", nullptr);
		}
		#endif
		{
			bool res = self->SetSignLines(BlockX, BlockY, BlockZ, Line1, Line2, Line3, Line4, Player);
			tolua_pushboolean(tolua_S, res ? 1 : 0);
		}
	}
	return 1;
	
	#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'SetSignLines'.", &tolua_err);
	return 0;
	#endif
}




static int tolua_cWorld_TryGetHeight(lua_State * tolua_S)
{
	// Exported manually, because tolua would require the out-only param a_Height to be used when calling
	// Takes a_World, a_BlockX, a_BlockZ
	// Returns Height, IsValid
	#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (tolua_S, 1, "cWorld", 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber   (tolua_S, 3, 0, &tolua_err) ||
		!tolua_isnoobj    (tolua_S, 4, &tolua_err)
		)
		goto tolua_lerror;
	else
	#endif
	{
		cWorld * self       = (cWorld *) tolua_tousertype (tolua_S, 1, nullptr);
		int BlockX          = (int)      tolua_tonumber   (tolua_S, 2, 0);
		int BlockZ          = (int)      tolua_tonumber   (tolua_S, 3, 0);
		#ifndef TOLUA_RELEASE
		if (self == nullptr)
		{
			tolua_error(tolua_S, "Invalid 'self' in function 'TryGetHeight'", nullptr);
		}
		#endif
		{
			int Height = 0;
			bool res = self->TryGetHeight(BlockX, BlockZ, Height);
			tolua_pushboolean(tolua_S, res ? 1 : 0);
			if (res)
			{
				tolua_pushnumber(tolua_S, Height);
				return 2;
			}
		}
	}
	return 1;
	
	#ifndef TOLUA_RELEASE
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'TryGetHeight'.", &tolua_err);
	return 0;
	#endif
}





class cLuaWorldTask :
	public cWorld::cTask
{
public:
	cLuaWorldTask(cPluginLua & a_Plugin, int a_FnRef) :
		m_Plugin(a_Plugin),
		m_FnRef(a_FnRef)
	{
	}

protected:
	cPluginLua & m_Plugin;
	int m_FnRef;
	
	// cWorld::cTask overrides:
	virtual void Run(cWorld & a_World) override
	{
		m_Plugin.Call(m_FnRef, &a_World);
	}
} ;





static int tolua_cWorld_QueueTask(lua_State * tolua_S)
{
	// Binding for cWorld::QueueTask
	// Params: function
	
	// Retrieve the cPlugin from the LuaState:
	cPluginLua * Plugin = GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		return 0;
	}

	// Retrieve the args:
	cWorld * self = (cWorld *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Not called on an object instance");
	}
	if (!lua_isfunction(tolua_S, 2))
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a function for parameter #1");
	}

	// Create a reference to the function:
	int FnRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FnRef == LUA_REFNIL)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #1");
	}

	self->QueueTask(make_unique<cLuaWorldTask>(*Plugin, FnRef));
	return 0;
}





class cLuaScheduledWorldTask :
	public cWorld::cTask
{
public:
	cLuaScheduledWorldTask(cPluginLua & a_Plugin, int a_FnRef) :
		m_Plugin(a_Plugin),
		m_FnRef(a_FnRef)
	{
	}

protected:
	cPluginLua & m_Plugin;
	int m_FnRef;
	
	// cWorld::cTask overrides:
	virtual void Run(cWorld & a_World) override
	{
		m_Plugin.Call(m_FnRef, &a_World);
	}
};





static int tolua_cWorld_ScheduleTask(lua_State * tolua_S)
{
	// Binding for cWorld::ScheduleTask
	// Params: function, Ticks
	
	// Retrieve the cPlugin from the LuaState:
	cPluginLua * Plugin = GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		return 0;
	}

	// Retrieve the args:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cWorld") ||
		!L.CheckParamNumber  (2) ||
		!L.CheckParamFunction(3)
	)
	{
		return 0;
	}
	cWorld * World = (cWorld *)tolua_tousertype(tolua_S, 1, nullptr);
	if (World == nullptr)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Not called on an object instance");
	}

	// Create a reference to the function:
	int FnRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FnRef == LUA_REFNIL)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Could not get function reference of parameter #1");
	}
	
	int DelayTicks = (int)tolua_tonumber(tolua_S, 2, 0);

	World->ScheduleTask(DelayTicks, new cLuaScheduledWorldTask(*Plugin, FnRef));
	return 0;
}





static int tolua_cPluginManager_GetAllPlugins(lua_State * tolua_S)
{
	cPluginManager * self = (cPluginManager *)tolua_tousertype(tolua_S, 1, nullptr);

	const cPluginManager::PluginMap & AllPlugins = self->GetAllPlugins();

	lua_newtable(tolua_S);
	int index = 1;
	cPluginManager::PluginMap::const_iterator iter = AllPlugins.begin();
	while (iter != AllPlugins.end())
	{
		const cPlugin* Plugin = iter->second;
		tolua_pushstring(tolua_S, iter->first.c_str());
		if (Plugin != nullptr)
		{
			tolua_pushusertype(tolua_S, (void *)Plugin, "const cPlugin");
		}
		else
		{
			tolua_pushboolean(tolua_S, 0);
		}
		lua_rawset(tolua_S, -3);
		++iter;
		++index;
	}
	return 1;
}





static int tolua_cPluginManager_GetCurrentPlugin(lua_State * S)
{
	cPluginLua * Plugin = GetLuaPlugin(S);
	if (Plugin == nullptr)
	{
		// An error message has already been printed in GetLuaPlugin()
		return 0;
	}
	tolua_pushusertype(S, Plugin, "cPluginLua");
	return 1;
}





static int tolua_cPluginManager_LogStackTrace(lua_State * S)
{
	cLuaState::LogStackTrace(S);
	return 0;
}





static int tolua_cPluginManager_AddHook_FnRef(cPluginManager * a_PluginManager, cLuaState & S, int a_ParamIdx)
{
	// Helper function for cPluginmanager:AddHook() binding
	// Takes care of the new case (#121): args are HOOK_TYPE and CallbackFunction
	// The arg types have already been checked
	
	// Retrieve the cPlugin from the LuaState:
	cPluginLua * Plugin = GetLuaPlugin(S);
	if (Plugin == nullptr)
	{
		// An error message has been already printed in GetLuaPlugin()
		return 0;
	}
	
	// Retrieve and check the hook type
	int HookType = (int)tolua_tonumber(S, a_ParamIdx, -1);
	if (!a_PluginManager->IsValidHookType(HookType))
	{
		LOGWARNING("cPluginManager.AddHook(): Invalid HOOK_TYPE parameter: %d", HookType);
		S.LogStackTrace();
		return 0;
	}
	
	// Add the hook to the plugin
	if (!Plugin->AddHookRef(HookType, a_ParamIdx + 1))
	{
		LOGWARNING("cPluginManager.AddHook(): Cannot add hook %d, unknown error.", HookType);
		S.LogStackTrace();
		return 0;
	}
	a_PluginManager->AddHook(Plugin, HookType);
	
	// Success
	return 0;
}





static int tolua_cPluginManager_AddHook_DefFn(cPluginManager * a_PluginManager, cLuaState & S, int a_ParamIdx)
{
	// Helper function for cPluginmanager:AddHook() binding
	// Takes care of the old case (#121): args are cPluginLua and HOOK_TYPE
	// The arg types have already been checked
	
	// Retrieve and check the cPlugin parameter
	cPluginLua * Plugin = (cPluginLua *)tolua_tousertype(S, a_ParamIdx, nullptr);
	if (Plugin == nullptr)
	{
		LOGWARNING("cPluginManager.AddHook(): Invalid Plugin parameter, expected a valid cPlugin object. Hook not added");
		S.LogStackTrace();
		return 0;
	}
	if (Plugin != GetLuaPlugin(S))
	{
		// The plugin parameter passed to us is not our stored plugin. Disallow this!
		LOGWARNING("cPluginManager.AddHook(): Invalid Plugin parameter, cannot add hook to foreign plugins. Hook not added.");
		S.LogStackTrace();
		return 0;
	}
	
	// Retrieve and check the hook type
	int HookType = (int)tolua_tonumber(S, a_ParamIdx + 1, -1);
	if (!a_PluginManager->IsValidHookType(HookType))
	{
		LOGWARNING("cPluginManager.AddHook(): Invalid HOOK_TYPE parameter: %d", HookType);
		S.LogStackTrace();
		return 0;
	}
	
	// Get the standard name for the callback function:
	const char * FnName = cPluginLua::GetHookFnName(HookType);
	if (FnName == nullptr)
	{
		LOGWARNING("cPluginManager.AddHook(): Unknown hook type (%d). Hook not added.", HookType);
		S.LogStackTrace();
		return 0;
	}

	// Retrieve the function to call and add it to the plugin:
	lua_pushstring(S, FnName);
	bool res = Plugin->AddHookRef(HookType, 1);
	lua_pop(S, 1);  // Pop the function off the stack
	if (!res)
	{
		LOGWARNING("cPluginManager.AddHook(): Function %s not found. Hook not added.", FnName);
		S.LogStackTrace();
		return 0;
	}
	a_PluginManager->AddHook(Plugin, HookType);
	
	// Success
	return 0;
}





static int tolua_cPluginManager_AddHook(lua_State * tolua_S)
{
	/*
	Function signatures:
	cPluginManager:AddHook(HOOK_TYPE, CallbackFunction)        -- (1) recommended
	cPluginManager.AddHook(HOOK_TYPE, CallbackFunction)        -- (2) accepted silently (#401 deprecates this)
	cPluginManager:Get():AddHook(HOOK_TYPE, CallbackFunction)  -- (3) accepted silently
	cPluginManager:Get():AddHook(Plugin, HOOK_TYPE)            -- (4) old style (#121), accepted but complained about in the console
	cPluginManager.AddHook(Plugin, HOOK_TYPE)                  -- (5) old style (#121) mangled, accepted but complained about in the console
	*/
	
	cLuaState S(tolua_S);
	cPluginManager * PlgMgr = cPluginManager::Get();

	// If the first param is a cPluginManager instance, use it instead of the global one:
	int ParamIdx = 1;
	tolua_Error err;
	if (tolua_isusertype(S, 1, "cPluginManager", 0, &err))
	{
		// Style 2 or 3, retrieve the PlgMgr instance
		PlgMgr = (cPluginManager *)tolua_tousertype(S, 1, nullptr);
		if (PlgMgr == nullptr)
		{
			LOGWARNING("Malformed plugin, use cPluginManager.AddHook(HOOK_TYPE, CallbackFunction). Fixing the call for you.");
			S.LogStackTrace();
			PlgMgr = cPluginManager::Get();
		}
		ParamIdx += 1;
	}
	else if (tolua_isusertable(S, 1, "cPluginManager", 0, &err))
	{
		// Style 1, use the global PlgMgr, but increment ParamIdx
		ParamIdx++;
	}
	
	if (lua_isnumber(S, ParamIdx) && lua_isfunction(S, ParamIdx + 1))
	{
		// The next params are a number and a function, assume style 1 or 2
		return tolua_cPluginManager_AddHook_FnRef(PlgMgr, S, ParamIdx);
	}
	else if (tolua_isusertype(S, ParamIdx, "cPlugin", 0, &err) && lua_isnumber(S, ParamIdx + 1))
	{
		// The next params are a cPlugin and a number, assume style 3 or 4
		LOGINFO("cPluginManager.AddHook(): Deprecated format used, use cPluginManager.AddHook(HOOK_TYPE, CallbackFunction) instead. Fixing the call for you.");
		S.LogStackTrace();
		return tolua_cPluginManager_AddHook_DefFn(PlgMgr, S, ParamIdx);
	}
	
	AString ParamDesc;
	Printf(ParamDesc, "%s, %s, %s", S.GetTypeText(1).c_str(), S.GetTypeText(2).c_str(), S.GetTypeText(3).c_str());
	LOGWARNING("cPluginManager:AddHook(): bad parameters. Expected HOOK_TYPE and CallbackFunction, got %s. Hook not added.", ParamDesc.c_str());
	S.LogStackTrace();
	return 0;
}





static int tolua_cPluginManager_ForEachCommand(lua_State * tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if (NumArgs != 1)
	{
		LOGWARN("Error in function call 'ForEachCommand': Requires 1 argument, got %i", NumArgs);
		return 0;
	}

	cPluginManager * self = (cPluginManager *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		LOGWARN("Error in function call 'ForEachCommand': Not called on an object instance");
		return 0;
	}

	if (!lua_isfunction(tolua_S, 2))
	{
		LOGWARN("Error in function call 'ForEachCommand': Expected a function for parameter #1");
		return 0;
	}

	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FuncRef == LUA_REFNIL)
	{
		LOGWARN("Error in function call 'ForEachCommand': Could not get function reference of parameter #1");
		return 0;
	}

	class cLuaCallback : public cPluginManager::cCommandEnumCallback
	{
	public:
		cLuaCallback(lua_State * a_LuaState, int a_FuncRef)
			: LuaState( a_LuaState)
			, FuncRef( a_FuncRef)
		{}

	private:
		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) override
		{
			UNUSED(a_Plugin);
			
			lua_rawgeti(LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushcppstring(LuaState, a_Command);
			tolua_pushcppstring(LuaState, a_Permission);
			tolua_pushcppstring(LuaState, a_HelpString);

			int s = lua_pcall(LuaState, 3, 1, 0);
			if (cLuaState::ReportErrors(LuaState, s))
			{
				return true;  /* Abort enumeration */
			}

			if (lua_isboolean(LuaState, -1))
			{
				return (tolua_toboolean( LuaState, -1, 0) > 0);
			}
			return false;  /* Continue enumeration */
		}
		lua_State * LuaState;
		int FuncRef;
	} Callback(tolua_S, FuncRef);

	bool bRetVal = self->ForEachCommand(Callback);

	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef);

	/* Push return value on stack */
	tolua_pushboolean(tolua_S, bRetVal);
	return 1;
}





static int tolua_cPluginManager_ForEachConsoleCommand(lua_State * tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if (NumArgs != 1)
	{
		LOGWARN("Error in function call 'ForEachConsoleCommand': Requires 1 argument, got %i", NumArgs);
		return 0;
	}

	cPluginManager * self = (cPluginManager *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		LOGWARN("Error in function call 'ForEachConsoleCommand': Not called on an object instance");
		return 0;
	}

	if (!lua_isfunction(tolua_S, 2))
	{
		LOGWARN("Error in function call 'ForEachConsoleCommand': Expected a function for parameter #1");
		return 0;
	}

	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
	if (FuncRef == LUA_REFNIL)
	{
		LOGWARN("Error in function call 'ForEachConsoleCommand': Could not get function reference of parameter #1");
		return 0;
	}

	class cLuaCallback : public cPluginManager::cCommandEnumCallback
	{
	public:
		cLuaCallback(lua_State * a_LuaState, int a_FuncRef)
			: LuaState( a_LuaState)
			, FuncRef( a_FuncRef)
		{}

	private:
		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) override
		{
			UNUSED(a_Plugin);
			UNUSED(a_Permission);
			
			lua_rawgeti(LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushcppstring(LuaState, a_Command);
			tolua_pushcppstring(LuaState, a_HelpString);

			int s = lua_pcall(LuaState, 2, 1, 0);
			if (cLuaState::ReportErrors(LuaState, s))
			{
				return true;  /* Abort enumeration */
			}

			if (lua_isboolean(LuaState, -1))
			{
				return (tolua_toboolean( LuaState, -1, 0) > 0);
			}
			return false;  /* Continue enumeration */
		}
		lua_State * LuaState;
		int FuncRef;
	} Callback(tolua_S, FuncRef);

	bool bRetVal = self->ForEachConsoleCommand(Callback);

	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef);

	/* Push return value on stack */
	tolua_pushboolean(tolua_S, bRetVal);
	return 1;
}





static int tolua_cPluginManager_BindCommand(lua_State * L)
{
	/* Function signatures:
		cPluginManager:BindCommand(Command, Permission, Function, HelpString)
		cPluginManager.BindCommand(Command, Permission, Function, HelpString)  -- without the "self" param
	*/
	cPluginLua * Plugin = GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		return 0;
	}
	
	// Read the arguments to this API call:
	tolua_Error tolua_err;
	int idx = 1;
	if (
		tolua_isusertype (L, 1, "cPluginManager", 0, &tolua_err) ||
		tolua_isusertable(L, 1, "cPluginManager", 0, &tolua_err)
	)
	{
		idx++;
	}
	if (
		!tolua_iscppstring(L, idx,     0, &tolua_err) ||
		!tolua_iscppstring(L, idx + 1, 0, &tolua_err) ||
		!tolua_iscppstring(L, idx + 3, 0, &tolua_err) ||
		!tolua_isnoobj    (L, idx + 4, &tolua_err)
	)
	{
		tolua_error(L, "#ferror in function 'BindCommand'.", &tolua_err);
		return 0;
	}
	if (!lua_isfunction(L, idx + 2))
	{
		luaL_error(L, "\"BindCommand\" function expects a function as its 3rd parameter. Command-binding aborted.");
		return 0;
	}
	cPluginManager * self = cPluginManager::Get();
	AString Command   (tolua_tocppstring(L, idx,     ""));
	AString Permission(tolua_tocppstring(L, idx + 1, ""));
	AString HelpString(tolua_tocppstring(L, idx + 3, ""));
	
	// Store the function reference:
	lua_pop(L, 1);  // Pop the help string off the stack
	int FnRef = luaL_ref(L, LUA_REGISTRYINDEX);  // Store function reference
	if (FnRef == LUA_REFNIL)
	{
		LOGERROR("\"BindCommand\": Cannot create a function reference. Command \"%s\" not bound.", Command.c_str());
		return 0;
	}
	
	if (!self->BindCommand(Command, Plugin, Permission, HelpString))
	{
		// Refused. Possibly already bound. Error message has been given, display the callstack:
		cLuaState LS(L);
		LS.LogStackTrace();
		return 0;
	}
	
	Plugin->BindCommand(Command, FnRef);
	lua_pushboolean(L, true);
	return 1;
}





static int tolua_cPluginManager_BindConsoleCommand(lua_State * L)
{
	/* Function signatures:
		cPluginManager:BindConsoleCommand(Command, Function, HelpString)
		cPluginManager.BindConsoleCommand(Command, Function, HelpString)  -- without the "self" param
	*/
	
	// Get the plugin identification out of LuaState:
	cPluginLua * Plugin = GetLuaPlugin(L);
	if (Plugin == nullptr)
	{
		return 0;
	}
	
	// Read the arguments to this API call:
	tolua_Error tolua_err;
	int idx = 1;
	if (
		tolua_isusertype(L, 1, "cPluginManager", 0, &tolua_err) ||
		tolua_isusertable(L, 1, "cPluginManager", 0, &tolua_err)
	)
	{
		idx++;
	}
	if (
		!tolua_iscppstring(L, idx,     0, &tolua_err) ||  // Command
		!tolua_iscppstring(L, idx + 2, 0, &tolua_err) ||  // HelpString
		!tolua_isnoobj    (L, idx + 3, &tolua_err)
	)
	{
		tolua_error(L, "#ferror in function 'BindConsoleCommand'.", &tolua_err);
		return 0;
	}
	if (!lua_isfunction(L, idx + 1))
	{
		luaL_error(L, "\"BindConsoleCommand\" function expects a function as its 2nd parameter. Command-binding aborted.");
		return 0;
	}
	cPluginManager * self = cPluginManager::Get();
	AString Command   (tolua_tocppstring(L, idx,     ""));
	AString HelpString(tolua_tocppstring(L, idx + 2, ""));
	
	// Store the function reference:
	lua_pop(L, 1);  // Pop the help string off the stack
	int FnRef = luaL_ref(L, LUA_REGISTRYINDEX);  // Store function reference
	if (FnRef == LUA_REFNIL)
	{
		LOGERROR("\"BindConsoleCommand\": Cannot create a function reference. Console Command \"%s\" not bound.", Command.c_str());
		return 0;
	}
	
	if (!self->BindConsoleCommand(Command, Plugin, HelpString))
	{
		// Refused. Possibly already bound. Error message has been given, display the callstack:
		cLuaState LS(L);
		LS.LogStackTrace();
		return 0;
	}
	
	Plugin->BindConsoleCommand(Command, FnRef);
	lua_pushboolean(L, true);
	return 1;
}





static int tolua_cPluginManager_CallPlugin(lua_State * tolua_S)
{
	/*
	Function signature:
	cPluginManager:CallPlugin("PluginName", "FunctionName", args...)
	*/
	
	// Check the parameters:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cPluginManager") ||
		!L.CheckParamString(2, 3))
	{
		return 0;
	}
	
	// Retrieve the plugin name and function name
	AString PluginName, FunctionName;
	L.ToString(2, PluginName);
	L.ToString(3, FunctionName);
	if (PluginName.empty() || FunctionName.empty())
	{
		LOGWARNING("cPluginManager:CallPlugin(): Invalid plugin name or function name");
		L.LogStackTrace();
		return 0;
	}
	
	// If requesting calling the current plugin, refuse:
	cPluginLua * ThisPlugin = GetLuaPlugin(L);
	if (ThisPlugin == nullptr)
	{
		return 0;
	}
	if (ThisPlugin->GetName() == PluginName)
	{
		LOGWARNING("cPluginManager::CallPlugin(): Calling self is not implemented (why would it?)");
		L.LogStackTrace();
		return 0;
	}
	
	// Call the destination plugin using a plugin callback:
	class cCallback :
		public cPluginManager::cPluginCallback
	{
	public:
		int m_NumReturns;

		cCallback(const AString & a_FunctionName, cLuaState & a_SrcLuaState) :
			m_NumReturns(0),
			m_FunctionName(a_FunctionName),
			m_SrcLuaState(a_SrcLuaState)
		{
		}
	protected:
		const AString & m_FunctionName;
		cLuaState & m_SrcLuaState;
		
		virtual bool Item(cPlugin * a_Plugin) override
		{
			m_NumReturns = ((cPluginLua *)a_Plugin)->CallFunctionFromForeignState(
				m_FunctionName, m_SrcLuaState, 4, lua_gettop(m_SrcLuaState)
			);
			return true;
		}
	} Callback(FunctionName, L);
	if (!cPluginManager::Get()->DoWithPlugin(PluginName, Callback))
	{
		// TODO 2014_01_20 _X: This might be too much logging, plugins cannot know if other plugins are loaded (async)
		LOGWARNING("cPluginManager::CallPlugin: No such plugin name (\"%s\")", PluginName.c_str());
		L.LogStackTrace();
		return 0;
	}
	return Callback.m_NumReturns;
}





static int tolua_cWorld_ChunkStay(lua_State * tolua_S)
{
	/* Function signature:
	World:ChunkStay(ChunkCoordTable, OnChunkAvailable, OnAllChunksAvailable)
	ChunkCoordTable == { {Chunk1x, Chunk1z}, {Chunk2x, Chunk2z}, ... }
	*/
	
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType     (1, "cWorld") ||
		!L.CheckParamTable        (2) ||
		!L.CheckParamFunctionOrNil(3, 4)
	)
	{
		return 0;
	}
	
	cPluginLua * Plugin = GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		return 0;
	}
	
	// Read the params:
	cWorld * World = (cWorld *)tolua_tousertype(tolua_S, 1, nullptr);
	if (World == nullptr)
	{
		LOGWARNING("World:ChunkStay(): invalid world parameter");
		L.LogStackTrace();
		return 0;
	}

	cLuaChunkStay * ChunkStay = new cLuaChunkStay(*Plugin);

	if (!ChunkStay->AddChunks(2))
	{
		delete ChunkStay;
		ChunkStay = nullptr;
		return 0;
	}

	ChunkStay->Enable(*World->GetChunkMap(), 3, 4);
	return 0;
}





static int tolua_cWorld_PrepareChunk(lua_State * tolua_S)
{
	/* Function signature:
	World:PrepareChunk(ChunkX, ChunkZ, Callback)
	*/
	
	// Check the param types:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType     (1, "cWorld") ||
		!L.CheckParamNumber       (2, 3) ||
		!L.CheckParamFunctionOrNil(4)
	)
	{
		return 0;
	}
	
	// Read the params:
	cWorld * world = nullptr;
	int chunkX = 0, chunkZ = 0;
	L.GetStackValues(1, world, chunkX, chunkZ);
	if (world == nullptr)
	{
		LOGWARNING("World:PrepareChunk(): invalid world parameter");
		L.LogStackTrace();
		return 0;
	}

	// Wrap the Lua callback inside a C++ callback class:
	class cCallback:
		public cChunkCoordCallback
	{
	public:
		cCallback(lua_State * a_LuaState):
			m_LuaState(a_LuaState),
			m_Callback(m_LuaState, 4)
		{
		}

		// cChunkCoordCallback override:
		virtual void Call(int a_CBChunkX, int a_CBChunkZ) override
		{
			if (m_Callback.IsValid())
			{
				m_LuaState.Call(m_Callback, a_CBChunkX, a_CBChunkZ);
			}

			// This is the last reference of this object, we must delete it so that it doesn't leak:
			delete this;
		}

	protected:
		cLuaState m_LuaState;
		cLuaState::cRef m_Callback;
	};
	cCallback * callback = new cCallback(tolua_S);

	// Call the chunk preparation:
	world->PrepareChunk(chunkX, chunkZ, callback);
	return 0;
}





static int tolua_cPlayer_GetPermissions(lua_State * tolua_S)
{
	// Function signature: cPlayer:GetPermissions() -> {permissions-array}

	// Check the params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cPlayer") ||
		!L.CheckParamEnd     (2)
	)
	{
		return 0;
	}

	// Get the params:
	cPlayer * self = (cPlayer *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		LOGWARNING("%s: invalid self (%p)", __FUNCTION__, self);
		return 0;
	}
	
	// Push the permissions:
	L.Push(self->GetPermissions());
	return 1;
}





static int tolua_cPlayer_OpenWindow(lua_State * tolua_S)
{
	// Function signature: cPlayer:OpenWindow(Window)

	// Retrieve the plugin instance from the Lua state
	cPluginLua * Plugin = GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		return 0;
	}

	// Get the parameters:
	cPlayer * self = (cPlayer *)tolua_tousertype(tolua_S, 1, nullptr);
	cWindow * wnd  = (cWindow *)tolua_tousertype(tolua_S, 2, nullptr);
	if ((self == nullptr) || (wnd == nullptr))
	{
		LOGWARNING("%s: invalid self (%p) or wnd (%p)", __FUNCTION__, self, wnd);
		return 0;
	}
	
	// If cLuaWindow, add a reference, so that Lua won't delete the cLuaWindow object mid-processing
	tolua_Error err;
	if (tolua_isusertype(tolua_S, 2, "cLuaWindow", 0, &err))
	{
		cLuaWindow * LuaWnd = (cLuaWindow *)wnd;
		// Only if not already referenced
		if (!LuaWnd->IsLuaReferenced())
		{
			int LuaRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
			if (LuaRef == LUA_REFNIL)
			{
				LOGWARNING("%s: Cannot create a window reference. Cannot open window \"%s\".",
					__FUNCTION__, wnd->GetWindowTitle().c_str()
				);
				return 0;
			}
			LuaWnd->SetLuaRef(Plugin, LuaRef);
		}
	}
	
	// Open the window
	self->OpenWindow(wnd);
	return 0;
}





static int tolua_cPlayer_PermissionMatches(lua_State * tolua_S)
{
	// Function signature: cPlayer:PermissionMatches(PermissionStr, TemplateStr) -> bool

	// Check the params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cPlayer") ||
		!L.CheckParamString   (2, 3) ||
		!L.CheckParamEnd      (4)
	)
	{
		return 0;
	}

	// Get the params:
	AString Permission, Template;
	L.GetStackValues(2, Permission, Template);
	
	// Push the result of the match:
	L.Push(cPlayer::PermissionMatches(StringSplit(Permission, "."), StringSplit(Template, ".")));
	return 1;
}





template <
	class OBJTYPE,
	void (OBJTYPE::*SetCallback)(cPluginLua * a_Plugin, int a_FnRef)
>
static int tolua_SetObjectCallback(lua_State * tolua_S)
{
	// Function signature: OBJTYPE:SetWhateverCallback(CallbackFunction)

	// Retrieve the plugin instance from the Lua state
	cPluginLua * Plugin = GetLuaPlugin(tolua_S);
	if (Plugin == nullptr)
	{
		// Warning message has already been printed by GetLuaPlugin(), bail out silently
		return 0;
	}

	// Get the parameters - self and the function reference:
	OBJTYPE * self = (OBJTYPE *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		LOGWARNING("%s: invalid self (%p)", __FUNCTION__, self);
		return 0;
	}
	int FnRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX);  // Store function reference for later retrieval
	if (FnRef == LUA_REFNIL)
	{
		LOGERROR("%s: Cannot create a function reference. Callback not set.", __FUNCTION__);
		return 0;
	}
	
	// Set the callback
	(self->*SetCallback)(Plugin, FnRef);
	return 0;
}





static int tolua_cPluginLua_AddWebTab(lua_State * tolua_S)
{
	cPluginLua * self = (cPluginLua *)tolua_tousertype(tolua_S, 1, nullptr);

	tolua_Error tolua_err;
	tolua_err.array = 0;
	tolua_err.index = 3;
	tolua_err.type = "function";

	std::string Title = "";
	int Reference = LUA_REFNIL;

	if (
		tolua_isstring(tolua_S, 2, 0, &tolua_err) &&
		lua_isfunction(tolua_S, 3)
	)
	{
		Reference = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		Title = ((std::string)tolua_tocppstring(tolua_S, 2, 0));
	}
	else
	{
		return tolua_do_error(tolua_S, "#ferror calling function '#funcname#'", &tolua_err);
	}

	if (Reference != LUA_REFNIL)
	{
		if (!self->AddWebTab(Title.c_str(), tolua_S, Reference))
		{
			luaL_unref(tolua_S, LUA_REGISTRYINDEX, Reference);
		}
	}
	else
	{
		LOGWARNING("cPluginLua:AddWebTab: invalid function reference in 2nd argument (Title: \"%s\")", Title.c_str());
	}

	return 0;
}





static int tolua_cPluginLua_AddTab(lua_State* tolua_S)
{
	cPluginLua * self = (cPluginLua *) tolua_tousertype(tolua_S, 1, nullptr);
	LOGWARN("WARNING: Using deprecated function AddTab()! Use AddWebTab() instead. (plugin \"%s\" in folder \"%s\")",
		self->GetName().c_str(), self->GetDirectory().c_str()
	);
	return tolua_cPluginLua_AddWebTab( tolua_S);
}





static int tolua_cPlugin_Call(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	
	// Log the obsoletion warning:
	LOGWARNING("cPlugin:Call() is obsolete and unsafe, use cPluginManager:CallPlugin() instead.");
	L.LogStackTrace();
	
	// Retrieve the params: plugin and the function name to call
	cPluginLua * TargetPlugin = (cPluginLua *) tolua_tousertype(tolua_S, 1, nullptr);
	AString FunctionName = tolua_tostring(tolua_S, 2, "");
	
	// Call the function:
	int NumReturns = TargetPlugin->CallFunctionFromForeignState(FunctionName, L, 3, lua_gettop(L));
	if (NumReturns < 0)
	{
		LOGWARNING("cPlugin::Call() failed to call destination function");
		L.LogStackTrace();
		return 0;
	}
	return NumReturns;
}





static int tolua_md5(lua_State* tolua_S)
{
	unsigned char Output[16];
	size_t len = 0;
	const unsigned char * SourceString = (const unsigned char *)lua_tolstring(tolua_S, 1, &len);
	md5(SourceString, len, Output);
	lua_pushlstring(tolua_S, (const char *)Output, ARRAYCOUNT(Output));
	return 1;
}





static int tolua_push_StringStringMap(lua_State* tolua_S, std::map< std::string, std::string >& a_StringStringMap)
{
	lua_newtable(tolua_S);
	int top = lua_gettop(tolua_S);

	for (std::map<std::string, std::string>::iterator it = a_StringStringMap.begin(); it != a_StringStringMap.end(); ++it)
	{
		const char* key = it->first.c_str();
		const char* value = it->second.c_str();
		lua_pushstring(tolua_S, key);
		lua_pushstring(tolua_S, value);
		lua_settable(tolua_S, top);
	}

	return 1;
}





static int tolua_get_HTTPRequest_Params(lua_State* tolua_S)
{
	HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S, 1, nullptr);
	return tolua_push_StringStringMap(tolua_S, self->Params);
}





static int tolua_get_HTTPRequest_PostParams(lua_State* tolua_S)
{
	HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S, 1, nullptr);
	return tolua_push_StringStringMap(tolua_S, self->PostParams);
}





static int tolua_get_HTTPRequest_FormData(lua_State* tolua_S)
{
	HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S, 1, nullptr);
	std::map< std::string, HTTPFormData >& FormData = self->FormData;

	lua_newtable(tolua_S);
	int top = lua_gettop(tolua_S);

	for (std::map<std::string, HTTPFormData>::iterator it = FormData.begin(); it != FormData.end(); ++it)
	{
		lua_pushstring(tolua_S, it->first.c_str());
		tolua_pushusertype(tolua_S, &(it->second), "HTTPFormData");
		// lua_pushlstring(tolua_S, it->second.Value.c_str(), it->second.Value.size());  // Might contain binary data
		lua_settable(tolua_S, top);
	}

	return 1;
}





static int tolua_cWebAdmin_GetPlugins(lua_State * tolua_S)
{
	cWebAdmin * self = (cWebAdmin *)tolua_tousertype(tolua_S, 1, nullptr);

	const cWebAdmin::PluginList & AllPlugins = self->GetPlugins();

	lua_createtable(tolua_S, (int)AllPlugins.size(), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	cWebAdmin::PluginList::const_iterator iter = AllPlugins.begin();
	while (iter != AllPlugins.end())
	{
		const cWebPlugin * Plugin = *iter;
		tolua_pushusertype(tolua_S, (void *)Plugin, "const cWebPlugin");
		lua_rawseti(tolua_S, newTable, index);
		++iter;
		++index;
	}
	return 1;
}





/** Binding for cWebAdmin::GetHTMLEscapedString.
Manual code required because ToLua generates an extra return value */
static int tolua_AllToLua_cWebAdmin_GetHTMLEscapedString(lua_State * tolua_S)
{
	// Check the param types:
	cLuaState S(tolua_S);
	if (
		!S.CheckParamUserTable(1, "cWebAdmin") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the parameters:
	AString Input;
	S.GetStackValue(2, Input);
	
	// Convert and return:
	S.Push(cWebAdmin::GetHTMLEscapedString(Input));
	return 1;
}





/** Binding for cWebAdmin::GetURLEncodedString.
Manual code required because ToLua generates an extra return value */
static int tolua_AllToLua_cWebAdmin_GetURLEncodedString(lua_State * tolua_S)
{
	// Check the param types:
	cLuaState S(tolua_S);
	if (
		!S.CheckParamUserTable(1, "cWebAdmin") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the parameters:
	AString Input;
	S.GetStackValue(2, Input);
	
	// Convert and return:
	S.Push(cWebAdmin::GetURLEncodedString(Input));
	return 1;
}





static int tolua_cWebPlugin_GetTabNames(lua_State * tolua_S)
{
	cWebPlugin* self = (cWebPlugin*)  tolua_tousertype(tolua_S, 1, nullptr);

	const cWebPlugin::TabNameList & TabNames = self->GetTabNames();

	lua_newtable(tolua_S);
	int index = 1;
	cWebPlugin::TabNameList::const_iterator iter = TabNames.begin();
	while (iter != TabNames.end())
	{
		const AString & FancyName = iter->first;
		const AString & WebName = iter->second;
		tolua_pushstring( tolua_S, WebName.c_str());  // Because the WebName is supposed to be unique, use it as key
		tolua_pushstring( tolua_S, FancyName.c_str());
		//
		lua_rawset(tolua_S, -3);
		++iter;
		++index;
	}
	return 1;
}





static int tolua_cClientHandle_SendPluginMessage(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserType(1, "cClientHandle") ||
		!S.CheckParamString(2, 3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	cClientHandle * Client = (cClientHandle *)tolua_tousertype(L, 1, nullptr);
	if (Client == nullptr)
	{
		LOGWARNING("ClientHandle is nil in cClientHandle:SendPluginMessage()");
		S.LogStackTrace();
		return 0;
	}
	AString Channel, Message;
	Channel.assign(lua_tostring(L, 2), lua_strlen(L, 2));
	Message.assign(lua_tostring(L, 3), lua_strlen(L, 3));
	Client->SendPluginMessage(Channel, Message);
	return 0;
}





static int tolua_cMojangAPI_AddPlayerNameToUUIDMapping(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamString(3) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Retrieve the parameters:
	AString UUID, PlayerName;
	S.GetStackValue(2, PlayerName);
	S.GetStackValue(3, UUID);
	
	// Store in the cache:
	cRoot::Get()->GetMojangAPI().AddPlayerNameToUUIDMapping(PlayerName, UUID);
	return 0;
}





static int tolua_cMojangAPI_GetPlayerNameFromUUID(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	AString UUID;
	S.GetStackValue(2, UUID);
	
	// If the UseOnlyCached param was given, read it; default to false
	bool ShouldUseCacheOnly = false;
	if (lua_gettop(L) == 3)
	{
		ShouldUseCacheOnly = (lua_toboolean(L, 3) != 0);
		lua_pop(L, 1);
	}

	// Return the PlayerName:
	AString PlayerName = cRoot::Get()->GetMojangAPI().GetPlayerNameFromUUID(UUID, ShouldUseCacheOnly);
	S.Push(PlayerName);
	return 1;
}





static int tolua_cMojangAPI_GetUUIDFromPlayerName(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	AString PlayerName;
	S.GetStackValue(2, PlayerName);
	
	// If the UseOnlyCached param was given, read it; default to false
	bool ShouldUseCacheOnly = false;
	if (lua_gettop(L) == 3)
	{
		ShouldUseCacheOnly = (lua_toboolean(L, 3) != 0);
		lua_pop(L, 1);
	}

	// Return the UUID:
	AString UUID = cRoot::Get()->GetMojangAPI().GetUUIDFromPlayerName(PlayerName, ShouldUseCacheOnly);
	S.Push(UUID);
	return 1;
}





static int tolua_cMojangAPI_GetUUIDsFromPlayerNames(lua_State * L)
{
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamTable(2) ||
		!S.CheckParamEnd(4)
	)
	{
		return 0;
	}
	
	// Convert the input table into AStringVector:
	AStringVector PlayerNames;
	int NumNames = luaL_getn(L, 2);
	PlayerNames.reserve(NumNames);
	for (int i = 1; i <= NumNames; i++)
	{
		lua_rawgeti(L, 2, i);
		AString Name;
		S.GetStackValue(-1, Name);
		if (!Name.empty())
		{
			PlayerNames.push_back(Name);
		}
		lua_pop(L, 1);
	}
	
	// If the UseOnlyCached param was given, read it; default to false
	bool ShouldUseCacheOnly = false;
	if (lua_gettop(L) == 3)
	{
		ShouldUseCacheOnly = (lua_toboolean(L, 3) != 0);
		lua_pop(L, 1);
	}

	// Push the output table onto the stack:
	lua_newtable(L);
	
	// Get the UUIDs:
	AStringVector UUIDs = cRoot::Get()->GetMojangAPI().GetUUIDsFromPlayerNames(PlayerNames, ShouldUseCacheOnly);
	if (UUIDs.size() != PlayerNames.size())
	{
		// A hard error has occured while processing the request, no UUIDs were returned. Return an empty table:
		return 1;
	}
	
	// Convert to output table, PlayerName -> UUID:
	size_t len = UUIDs.size();
	for (size_t i = 0; i < len; i++)
	{
		if (UUIDs[i].empty())
		{
			// No UUID was provided for PlayerName[i], skip it in the resulting table
			continue;
		}
		lua_pushlstring(L, UUIDs[i].c_str(), UUIDs[i].length());
		lua_setfield(L, 3, PlayerNames[i].c_str());
	}
	return 1;
}





static int tolua_cMojangAPI_MakeUUIDDashed(lua_State * L)
{
	// Function signature: cMojangAPI:MakeUUIDDashed(UUID) -> string

	// Check params:
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString UUID;
	S.GetStackValue(2, UUID);

	// Push the result:
	S.Push(cRoot::Get()->GetMojangAPI().MakeUUIDDashed(UUID));
	return 1;
}





static int tolua_cMojangAPI_MakeUUIDShort(lua_State * L)
{
	// Function signature: cMojangAPI:MakeUUIDShort(UUID) -> string

	// Check params:
	cLuaState S(L);
	if (
		!S.CheckParamUserTable(1, "cMojangAPI") ||
		!S.CheckParamString(2) ||
		!S.CheckParamEnd(3)
	)
	{
		return 0;
	}
	
	// Get the params:
	AString UUID;
	S.GetStackValue(2, UUID);

	// Push the result:
	S.Push(cRoot::Get()->GetMojangAPI().MakeUUIDShort(UUID));
	return 1;
}





static int Lua_ItemGrid_GetSlotCoords(lua_State * L)
{
	tolua_Error tolua_err;
	if (
		!tolua_isusertype(L, 1, "const cItemGrid", 0, &tolua_err) ||
		!tolua_isnumber  (L, 2, 0, &tolua_err) ||
		!tolua_isnoobj   (L, 3, &tolua_err)
	)
	{
		goto tolua_lerror;
	}

	{
		const cItemGrid * self = (const cItemGrid *)tolua_tousertype(L, 1, nullptr);
		int SlotNum = (int)tolua_tonumber(L, 2, 0);
		if (self == nullptr)
		{
			tolua_error(L, "invalid 'self' in function 'cItemGrid:GetSlotCoords'", nullptr);
			return 0;
		}
		int X, Y;
		self->GetSlotCoords(SlotNum, X, Y);
		tolua_pushnumber(L, (lua_Number)X);
		tolua_pushnumber(L, (lua_Number)Y);
		return 2;
	}
	
tolua_lerror:
	tolua_error(L, "#ferror in function 'cItemGrid:GetSlotCoords'.", &tolua_err);
	return 0;
}





/// Provides interface between a Lua table of callbacks and the cBlockTracer::cCallbacks
class cLuaBlockTracerCallbacks :
	public cBlockTracer::cCallbacks
{
public:
	cLuaBlockTracerCallbacks(cLuaState & a_LuaState, int a_ParamNum) :
		m_LuaState(a_LuaState),
		m_TableRef(a_LuaState, a_ParamNum)
	{
	}
	
	virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, char a_EntryFace) override
	{
		bool res = false;
		if (!m_LuaState.Call(
			cLuaState::cTableRef(m_TableRef, "OnNextBlock"),
			a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, a_EntryFace,
			cLuaState::Return, res
		))
		{
			// No such function in the table, skip the callback
			return false;
		}
		return res;
	}
	
	virtual bool OnNextBlockNoData(int a_BlockX, int a_BlockY, int a_BlockZ, char a_EntryFace) override
	{
		bool res = false;
		if (!m_LuaState.Call(
			cLuaState::cTableRef(m_TableRef, "OnNextBlockNoData"),
			a_BlockX, a_BlockY, a_BlockZ, a_EntryFace,
			cLuaState::Return, res
		))
		{
			// No such function in the table, skip the callback
			return false;
		}
		return res;
	}
	
	virtual bool OnOutOfWorld(double a_BlockX, double a_BlockY, double a_BlockZ) override
	{
		bool res = false;
		if (!m_LuaState.Call(
			cLuaState::cTableRef(m_TableRef, "OnOutOfWorld"),
			a_BlockX, a_BlockY, a_BlockZ,
			cLuaState::Return, res
		))
		{
			// No such function in the table, skip the callback
			return false;
		}
		return res;
	}
	
	virtual bool OnIntoWorld(double a_BlockX, double a_BlockY, double a_BlockZ) override
	{
		bool res = false;
		if (!m_LuaState.Call(
			cLuaState::cTableRef(m_TableRef, "OnIntoWorld"),
			a_BlockX, a_BlockY, a_BlockZ,
			cLuaState::Return, res
		))
		{
			// No such function in the table, skip the callback
			return false;
		}
		return res;
	}
	
	virtual void OnNoMoreHits(void) override
	{
		m_LuaState.Call(cLuaState::cTableRef(m_TableRef, "OnNoMoreHits"));
	}
	
	virtual void OnNoChunk(void) override
	{
		m_LuaState.Call(cLuaState::cTableRef(m_TableRef, "OnNoChunk"));
	}

protected:
	cLuaState & m_LuaState;
	cLuaState::cRef m_TableRef;
} ;





static int tolua_cLineBlockTracer_Trace(lua_State * tolua_S)
{
	/* Supported function signatures:
	cLineBlockTracer:Trace(World, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ)  // Canonical
	cLineBlockTracer.Trace(World, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ)
	*/
	
	// If the first param is the cLineBlockTracer class, shift param index by one:
	int idx = 1;
	tolua_Error err;
	if (tolua_isusertable(tolua_S, 1, "cLineBlockTracer", 0, &err))
	{
		idx = 2;
	}
	
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(idx, "cWorld") ||
		!L.CheckParamTable   (idx + 1) ||
		!L.CheckParamNumber  (idx + 2, idx + 7) ||
		!L.CheckParamEnd     (idx + 8)
	)
	{
		return 0;
	}

	// Trace:
	cWorld * World = (cWorld *)tolua_tousertype(L, idx, nullptr);
	cLuaBlockTracerCallbacks Callbacks(L, idx + 1);
	double StartX = tolua_tonumber(L, idx + 2, 0);
	double StartY = tolua_tonumber(L, idx + 3, 0);
	double StartZ = tolua_tonumber(L, idx + 4, 0);
	double EndX   = tolua_tonumber(L, idx + 5, 0);
	double EndY   = tolua_tonumber(L, idx + 6, 0);
	double EndZ   = tolua_tonumber(L, idx + 7, 0);
	bool res = cLineBlockTracer::Trace(*World, Callbacks, StartX, StartY, StartZ, EndX, EndY, EndZ);
	tolua_pushboolean(L, res ? 1 : 0);
	return 1;
}





static int tolua_cRoot_GetFurnaceRecipe(lua_State * tolua_S)
{
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserTable(1, "cRoot") ||
		!L.CheckParamUserType (2, "const cItem") ||
		!L.CheckParamEnd      (3)
	)
	{
		return 0;
	}
	
	// Check the input param:
	cItem * Input = (cItem *)tolua_tousertype(L, 2, nullptr);
	if (Input == nullptr)
	{
		LOGWARNING("cRoot:GetFurnaceRecipe: the Input parameter is nil or missing.");
		return 0;
	}
	
	// Get the recipe for the input
	cFurnaceRecipe * FR = cRoot::Get()->GetFurnaceRecipe();
	const cFurnaceRecipe::cRecipe * Recipe = FR->GetRecipeFrom(*Input);
	if (Recipe == nullptr)
	{
		// There is no such furnace recipe for this input, return no value
		return 0;
	}
	
	// Push the output, number of ticks and input as the three return values:
	tolua_pushusertype(L, Recipe->Out, "const cItem");
	tolua_pushnumber  (L, (lua_Number)(Recipe->CookTime));
	tolua_pushusertype(L, Recipe->In, "const cItem");
	return 3;
}





static int tolua_cHopperEntity_GetOutputBlockPos(lua_State * tolua_S)
{
	// function cHopperEntity::GetOutputBlockPos()
	// Exported manually because tolua would require meaningless params
	
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cHopperEntity") ||
		!L.CheckParamNumber  (2) ||
		!L.CheckParamEnd     (3)
	)
	{
		return 0;
	}
	cHopperEntity * self = (cHopperEntity *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cHopperEntity::GetOutputBlockPos()'", nullptr);
		return 0;
	}

	NIBBLETYPE a_BlockMeta = ((NIBBLETYPE)tolua_tonumber(tolua_S, 2, 0));
	int a_OutputX, a_OutputY, a_OutputZ;
	bool res = self->GetOutputBlockPos(a_BlockMeta, a_OutputX, a_OutputY, a_OutputZ);
	tolua_pushboolean(tolua_S, res);
	if (res)
	{
		tolua_pushnumber(tolua_S, (lua_Number)a_OutputX);
		tolua_pushnumber(tolua_S, (lua_Number)a_OutputY);
		tolua_pushnumber(tolua_S, (lua_Number)a_OutputZ);
		return 4;
	}
	return 1;
}






static int tolua_cBlockArea_GetBlockTypeMeta(lua_State * tolua_S)
{
	// function cBlockArea::GetBlockTypeMeta()
	// Exported manually because tolua generates extra input params for the outputs
	
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamNumber  (2, 4)
	)
	{
		return 0;
	}
	
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetRelBlockTypeMeta'", nullptr);
		return 0;
	}
	int BlockX = (int)tolua_tonumber(tolua_S, 2, 0);
	int BlockY = (int)tolua_tonumber(tolua_S, 3, 0);
	int BlockZ = (int)tolua_tonumber(tolua_S, 4, 0);
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	self->GetBlockTypeMeta(BlockX, BlockY, BlockZ, BlockType, BlockMeta);
	tolua_pushnumber(tolua_S, BlockType);
	tolua_pushnumber(tolua_S, BlockMeta);
	return 2;
}





static int tolua_cBlockArea_GetOrigin(lua_State * tolua_S)
{
	// function cBlockArea::GetOrigin()
	// Returns all three coords of the origin point
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs
	
	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cBlockArea"))
	{
		return 0;
	}
	
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetOrigin'", nullptr);
		return 0;
	}
	
	// Push the three origin coords:
	lua_pushnumber(tolua_S, self->GetOriginX());
	lua_pushnumber(tolua_S, self->GetOriginY());
	lua_pushnumber(tolua_S, self->GetOriginZ());
	return 3;
}





static int tolua_cBlockArea_GetRelBlockTypeMeta(lua_State * tolua_S)
{
	// function cBlockArea::GetRelBlockTypeMeta()
	// Exported manually because tolua generates extra input params for the outputs
	
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamNumber  (2, 4)
	)
	{
		return 0;
	}
	
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetRelBlockTypeMeta'", nullptr);
		return 0;
	}
	int BlockX = (int)tolua_tonumber(tolua_S, 2, 0);
	int BlockY = (int)tolua_tonumber(tolua_S, 3, 0);
	int BlockZ = (int)tolua_tonumber(tolua_S, 4, 0);
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;
	self->GetRelBlockTypeMeta(BlockX, BlockY, BlockZ, BlockType, BlockMeta);
	tolua_pushnumber(tolua_S, BlockType);
	tolua_pushnumber(tolua_S, BlockMeta);
	return 2;
}





static int tolua_cBlockArea_GetSize(lua_State * tolua_S)
{
	// function cBlockArea::GetSize()
	// Returns all three sizes of the area
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs
	
	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cBlockArea"))
	{
		return 0;
	}
	
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetSize'", nullptr);
		return 0;
	}
	
	// Push the three origin coords:
	lua_pushnumber(tolua_S, self->GetSizeX());
	lua_pushnumber(tolua_S, self->GetSizeY());
	lua_pushnumber(tolua_S, self->GetSizeZ());
	return 3;
}





static int tolua_cBlockArea_GetCoordRange(lua_State * tolua_S)
{
	// function cBlockArea::GetCoordRange()
	// Returns all three sizes of the area, miuns one, so that they represent the maximum coord value
	// Exported manually because there's no direct C++ equivalent,
	// plus tolua would generate extra input params for the outputs
	
	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cBlockArea"))
	{
		return 0;
	}
	
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea:GetSize'", nullptr);
		return 0;
	}
	
	// Push the three origin coords:
	lua_pushnumber(tolua_S, self->GetSizeX() - 1);
	lua_pushnumber(tolua_S, self->GetSizeY() - 1);
	lua_pushnumber(tolua_S, self->GetSizeZ() - 1);
	return 3;
}





static int tolua_cBlockArea_LoadFromSchematicFile(lua_State * tolua_S)
{
	// function cBlockArea::LoadFromSchematicFile
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamString  (2) ||
		!L.CheckParamEnd     (3)
	)
	{
		return 0;
	}
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea::LoadFromSchematicFile'", nullptr);
		return 0;
	}

	AString Filename = tolua_tostring(tolua_S, 2, 0);
	bool res = cSchematicFileSerializer::LoadFromSchematicFile(*self, Filename);
	tolua_pushboolean(tolua_S, res);
	return 1;
}





static int tolua_cBlockArea_LoadFromSchematicString(lua_State * tolua_S)
{
	// function cBlockArea::LoadFromSchematicString
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamString  (2) ||
		!L.CheckParamEnd     (3)
	)
	{
		return 0;
	}
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea::LoadFromSchematicFile'", nullptr);
		return 0;
	}

	AString Data;
	L.GetStackValue(2, Data);
	bool res = cSchematicFileSerializer::LoadFromSchematicString(*self, Data);
	tolua_pushboolean(tolua_S, res);
	return 1;
}





static int tolua_cBlockArea_SaveToSchematicFile(lua_State * tolua_S)
{
	// function cBlockArea::SaveToSchematicFile
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamString  (2) ||
		!L.CheckParamEnd     (3)
	)
	{
		return 0;
	}
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea::SaveToSchematicFile'", nullptr);
		return 0;
	}
	AString Filename = tolua_tostring(tolua_S, 2, 0);
	bool res = cSchematicFileSerializer::SaveToSchematicFile(*self, Filename);
	tolua_pushboolean(tolua_S, res);
	return 1;
}





static int tolua_cBlockArea_SaveToSchematicString(lua_State * tolua_S)
{
	// function cBlockArea::SaveToSchematicString
	// Exported manually because function has been moved to SchematicFileSerializer.cpp
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cBlockArea") ||
		!L.CheckParamEnd     (2)
	)
	{
		return 0;
	}
	cBlockArea * self = (cBlockArea *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cBlockArea::SaveToSchematicFile'", nullptr);
		return 0;
	}
	
	AString Data;
	if (cSchematicFileSerializer::SaveToSchematicString(*self, Data))
	{
		L.Push(Data);
		return 1;
	}
	return 0;
}





static int tolua_cCompositeChat_AddRunCommandPart(lua_State * tolua_S)
{
	// function cCompositeChat:AddRunCommandPart(Message, Command, [Style])
	// Exported manually to support call-chaining (return *this)
	
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2, 3)
	)
	{
		return 0;
	}
	cCompositeChat * self = (cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddRunCommandPart'", nullptr);
		return 0;
	}
	
	// Add the part:
	AString Text, Command, Style;
	L.GetStackValue(2, Text);
	L.GetStackValue(3, Command);
	L.GetStackValue(4, Style);
	self->AddRunCommandPart(Text, Command, Style);
	
	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_AddSuggestCommandPart(lua_State * tolua_S)
{
	// function cCompositeChat:AddSuggestCommandPart(Message, Command, [Style])
	// Exported manually to support call-chaining (return *this)
	
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2, 3)
	)
	{
		return 0;
	}
	cCompositeChat * self = (cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddSuggestCommandPart'", nullptr);
		return 0;
	}
	
	// Add the part:
	AString Text, Command, Style;
	L.GetStackValue(2, Text);
	L.GetStackValue(3, Command);
	L.GetStackValue(4, Style);
	self->AddSuggestCommandPart(Text, Command, Style);
	
	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_AddTextPart(lua_State * tolua_S)
{
	// function cCompositeChat:AddTextPart(Message, [Style])
	// Exported manually to support call-chaining (return *this)
	
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2)
	)
	{
		return 0;
	}
	cCompositeChat * self = (cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddTextPart'", nullptr);
		return 0;
	}
	
	// Add the part:
	AString Text, Style;
	L.GetStackValue(2, Text);
	L.GetStackValue(3, Style);
	self->AddTextPart(Text, Style);
	
	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_AddUrlPart(lua_State * tolua_S)
{
	// function cCompositeChat:AddTextPart(Message, Url, [Style])
	// Exported manually to support call-chaining (return *this)
	
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2, 3)
	)
	{
		return 0;
	}
	cCompositeChat * self = (cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:AddUrlPart'", nullptr);
		return 0;
	}
	
	// Add the part:
	AString Text, Url, Style;
	L.GetStackValue(2, Text);
	L.GetStackValue(3, Url);
	L.GetStackValue(4, Style);
	self->AddUrlPart(Text, Url, Style);
	
	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_ParseText(lua_State * tolua_S)
{
	// function cCompositeChat:ParseText(TextMessage)
	// Exported manually to support call-chaining (return *this)
	
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamString(2)
	)
	{
		return 0;
	}
	cCompositeChat * self = (cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:ParseText'", nullptr);
		return 0;
	}
	
	// Parse the text:
	AString Text;
	L.GetStackValue(2, Text);
	self->ParseText(Text);
	
	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_SetMessageType(lua_State * tolua_S)
{
	// function cCompositeChat:SetMessageType(MessageType)
	// Exported manually to support call-chaining (return *this)
	
	// Check params:
	cLuaState L(tolua_S);
	if (
		!L.CheckParamUserType(1, "cCompositeChat") ||
		!L.CheckParamNumber(2)
	)
	{
		return 0;
	}
	cCompositeChat * self = (cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:SetMessageType'", nullptr);
		return 0;
	}
	
	// Set the type:
	int MessageType = mtCustom;
	L.GetStackValue(2, MessageType);
	self->SetMessageType((eMessageType)MessageType);
	
	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





static int tolua_cCompositeChat_UnderlineUrls(lua_State * tolua_S)
{
	// function cCompositeChat:UnderlineUrls()
	// Exported manually to support call-chaining (return *this)
	
	// Check params:
	cLuaState L(tolua_S);
	if (!L.CheckParamUserType(1, "cCompositeChat"))
	{
		return 0;
	}
	cCompositeChat * self = (cCompositeChat *)tolua_tousertype(tolua_S, 1, nullptr);
	if (self == nullptr)
	{
		tolua_error(tolua_S, "invalid 'self' in function 'cCompositeChat:UnderlineUrls'", nullptr);
		return 0;
	}
	
	// Call the processing
	self->UnderlineUrls();
	
	// Cut away everything from the stack except for the cCompositeChat instance; return that:
	lua_settop(L, 1);
	return 1;
}





void ManualBindings::Bind(lua_State * tolua_S)
{
	tolua_beginmodule(tolua_S, nullptr);
		tolua_function(tolua_S, "Clamp",              tolua_Clamp);
		tolua_function(tolua_S, "StringSplit",        tolua_StringSplit);
		tolua_function(tolua_S, "StringSplitAndTrim", tolua_StringSplitAndTrim);
		tolua_function(tolua_S, "LOG",                tolua_LOG);
		tolua_function(tolua_S, "LOGINFO",            tolua_LOGINFO);
		tolua_function(tolua_S, "LOGWARN",            tolua_LOGWARN);
		tolua_function(tolua_S, "LOGWARNING",         tolua_LOGWARN);
		tolua_function(tolua_S, "LOGERROR",           tolua_LOGERROR);
		tolua_function(tolua_S, "Base64Encode",       tolua_Base64Encode);
		tolua_function(tolua_S, "Base64Decode",       tolua_Base64Decode);
		
		tolua_beginmodule(tolua_S, "cFile");
			tolua_function(tolua_S, "GetFolderContents", tolua_cFile_GetFolderContents);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cBlockArea");
			tolua_function(tolua_S, "GetBlockTypeMeta",        tolua_cBlockArea_GetBlockTypeMeta);
			tolua_function(tolua_S, "GetCoordRange",           tolua_cBlockArea_GetCoordRange);
			tolua_function(tolua_S, "GetOrigin",               tolua_cBlockArea_GetOrigin);
			tolua_function(tolua_S, "GetRelBlockTypeMeta",     tolua_cBlockArea_GetRelBlockTypeMeta);
			tolua_function(tolua_S, "GetSize",                 tolua_cBlockArea_GetSize);
			tolua_function(tolua_S, "LoadFromSchematicFile",   tolua_cBlockArea_LoadFromSchematicFile);
			tolua_function(tolua_S, "LoadFromSchematicString", tolua_cBlockArea_LoadFromSchematicString);
			tolua_function(tolua_S, "SaveToSchematicFile",     tolua_cBlockArea_SaveToSchematicFile);
			tolua_function(tolua_S, "SaveToSchematicString",   tolua_cBlockArea_SaveToSchematicString);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cCompositeChat");
			tolua_function(tolua_S, "AddRunCommandPart",     tolua_cCompositeChat_AddRunCommandPart);
			tolua_function(tolua_S, "AddSuggestCommandPart", tolua_cCompositeChat_AddSuggestCommandPart);
			tolua_function(tolua_S, "AddTextPart",           tolua_cCompositeChat_AddTextPart);
			tolua_function(tolua_S, "AddUrlPart",            tolua_cCompositeChat_AddUrlPart);
			tolua_function(tolua_S, "ParseText",             tolua_cCompositeChat_ParseText);
			tolua_function(tolua_S, "SetMessageType",        tolua_cCompositeChat_SetMessageType);
			tolua_function(tolua_S, "UnderlineUrls",         tolua_cCompositeChat_UnderlineUrls);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cHopperEntity");
			tolua_function(tolua_S, "GetOutputBlockPos", tolua_cHopperEntity_GetOutputBlockPos);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cLineBlockTracer");
			tolua_function(tolua_S, "Trace", tolua_cLineBlockTracer_Trace);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cRoot");
			tolua_function(tolua_S, "FindAndDoWithPlayer", tolua_DoWith <cRoot, cPlayer, &cRoot::FindAndDoWithPlayer>);
			tolua_function(tolua_S, "DoWithPlayerByUUID",  tolua_DoWith <cRoot, cPlayer, &cRoot::DoWithPlayerByUUID>);
			tolua_function(tolua_S, "ForEachPlayer",       tolua_ForEach<cRoot, cPlayer, &cRoot::ForEachPlayer>);
			tolua_function(tolua_S, "ForEachWorld",        tolua_ForEach<cRoot, cWorld,  &cRoot::ForEachWorld>);
			tolua_function(tolua_S, "GetFurnaceRecipe",    tolua_cRoot_GetFurnaceRecipe);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cWorld");
			tolua_function(tolua_S, "ChunkStay",                 tolua_cWorld_ChunkStay);
			tolua_function(tolua_S, "DoWithBlockEntityAt",       tolua_DoWithXYZ<cWorld, cBlockEntity,        &cWorld::DoWithBlockEntityAt>);
			tolua_function(tolua_S, "DoWithBeaconAt",            tolua_DoWithXYZ<cWorld, cBeaconEntity,       &cWorld::DoWithBeaconAt>);
			tolua_function(tolua_S, "DoWithChestAt",             tolua_DoWithXYZ<cWorld, cChestEntity,        &cWorld::DoWithChestAt>);
			tolua_function(tolua_S, "DoWithDispenserAt",         tolua_DoWithXYZ<cWorld, cDispenserEntity,    &cWorld::DoWithDispenserAt>);
			tolua_function(tolua_S, "DoWithDropSpenserAt",       tolua_DoWithXYZ<cWorld, cDropSpenserEntity,  &cWorld::DoWithDropSpenserAt>);
			tolua_function(tolua_S, "DoWithDropperAt",           tolua_DoWithXYZ<cWorld, cDropperEntity,      &cWorld::DoWithDropperAt>);
			tolua_function(tolua_S, "DoWithEntityByID",          tolua_DoWithID< cWorld, cEntity,             &cWorld::DoWithEntityByID>);
			tolua_function(tolua_S, "DoWithFurnaceAt",           tolua_DoWithXYZ<cWorld, cFurnaceEntity,      &cWorld::DoWithFurnaceAt>);
			tolua_function(tolua_S, "DoWithNoteBlockAt",         tolua_DoWithXYZ<cWorld, cNoteEntity,         &cWorld::DoWithNoteBlockAt>);
			tolua_function(tolua_S, "DoWithCommandBlockAt",      tolua_DoWithXYZ<cWorld, cCommandBlockEntity, &cWorld::DoWithCommandBlockAt>);
			tolua_function(tolua_S, "DoWithMobHeadAt",           tolua_DoWithXYZ<cWorld, cMobHeadEntity,      &cWorld::DoWithMobHeadAt>);
			tolua_function(tolua_S, "DoWithFlowerPotAt",         tolua_DoWithXYZ<cWorld, cFlowerPotEntity,    &cWorld::DoWithFlowerPotAt>);
			tolua_function(tolua_S, "DoWithPlayer",              tolua_DoWith<   cWorld, cPlayer,             &cWorld::DoWithPlayer>);
			tolua_function(tolua_S, "FindAndDoWithPlayer",       tolua_DoWith<   cWorld, cPlayer,             &cWorld::FindAndDoWithPlayer>);
			tolua_function(tolua_S, "DoWithPlayerByUUID",        tolua_DoWith<   cWorld, cPlayer,             &cWorld::DoWithPlayerByUUID>);
			tolua_function(tolua_S, "ForEachBlockEntityInChunk", tolua_ForEachInChunk<cWorld, cBlockEntity,   &cWorld::ForEachBlockEntityInChunk>);
			tolua_function(tolua_S, "ForEachChestInChunk",       tolua_ForEachInChunk<cWorld, cChestEntity,   &cWorld::ForEachChestInChunk>);
			tolua_function(tolua_S, "ForEachEntity",             tolua_ForEach<       cWorld, cEntity,        &cWorld::ForEachEntity>);
			tolua_function(tolua_S, "ForEachEntityInBox",        tolua_ForEachInBox<  cWorld, cEntity,        &cWorld::ForEachEntityInBox>);
			tolua_function(tolua_S, "ForEachEntityInChunk",      tolua_ForEachInChunk<cWorld, cEntity,        &cWorld::ForEachEntityInChunk>);
			tolua_function(tolua_S, "ForEachFurnaceInChunk",     tolua_ForEachInChunk<cWorld, cFurnaceEntity, &cWorld::ForEachFurnaceInChunk>);
			tolua_function(tolua_S, "ForEachPlayer",             tolua_ForEach<       cWorld, cPlayer,        &cWorld::ForEachPlayer>);
			tolua_function(tolua_S, "GetBlockInfo",              tolua_cWorld_GetBlockInfo);
			tolua_function(tolua_S, "GetBlockTypeMeta",          tolua_cWorld_GetBlockTypeMeta);
			tolua_function(tolua_S, "GetSignLines",              tolua_cWorld_GetSignLines);
			tolua_function(tolua_S, "PrepareChunk",              tolua_cWorld_PrepareChunk);
			tolua_function(tolua_S, "QueueTask",                 tolua_cWorld_QueueTask);
			tolua_function(tolua_S, "ScheduleTask",              tolua_cWorld_ScheduleTask);
			tolua_function(tolua_S, "SetSignLines",              tolua_cWorld_SetSignLines);
			tolua_function(tolua_S, "TryGetHeight",              tolua_cWorld_TryGetHeight);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cMapManager");
			tolua_function(tolua_S, "DoWithMap", tolua_DoWithID<cMapManager, cMap, &cMapManager::DoWithMap>);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cScoreboard");
			tolua_function(tolua_S, "ForEachObjective", tolua_ForEach<cScoreboard, cObjective, &cScoreboard::ForEachObjective>);
			tolua_function(tolua_S, "ForEachTeam",      tolua_ForEach<cScoreboard, cTeam,      &cScoreboard::ForEachTeam>);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPlugin");
			tolua_function(tolua_S, "Call", tolua_cPlugin_Call);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPluginManager");
			tolua_function(tolua_S, "AddHook",               tolua_cPluginManager_AddHook);
			tolua_function(tolua_S, "BindCommand",           tolua_cPluginManager_BindCommand);
			tolua_function(tolua_S, "BindConsoleCommand",    tolua_cPluginManager_BindConsoleCommand);
			tolua_function(tolua_S, "CallPlugin",            tolua_cPluginManager_CallPlugin);
			tolua_function(tolua_S, "ForEachCommand",        tolua_cPluginManager_ForEachCommand);
			tolua_function(tolua_S, "ForEachConsoleCommand", tolua_cPluginManager_ForEachConsoleCommand);
			tolua_function(tolua_S, "GetAllPlugins",         tolua_cPluginManager_GetAllPlugins);
			tolua_function(tolua_S, "GetCurrentPlugin",      tolua_cPluginManager_GetCurrentPlugin);
			tolua_function(tolua_S, "LogStackTrace",         tolua_cPluginManager_LogStackTrace);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPlayer");
			tolua_function(tolua_S, "GetPermissions",    tolua_cPlayer_GetPermissions);
			tolua_function(tolua_S, "OpenWindow",        tolua_cPlayer_OpenWindow);
			tolua_function(tolua_S, "PermissionMatches", tolua_cPlayer_PermissionMatches);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cLuaWindow");
			tolua_function(tolua_S, "SetOnClosing",     tolua_SetObjectCallback<cLuaWindow, &cLuaWindow::SetOnClosing>);
			tolua_function(tolua_S, "SetOnSlotChanged", tolua_SetObjectCallback<cLuaWindow, &cLuaWindow::SetOnSlotChanged>);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cPluginLua");
			tolua_function(tolua_S, "AddTab", tolua_cPluginLua_AddTab);
			tolua_function(tolua_S, "AddWebTab", tolua_cPluginLua_AddWebTab);
		tolua_endmodule(tolua_S);

		tolua_cclass(tolua_S, "HTTPRequest", "HTTPRequest", "", nullptr);
		tolua_beginmodule(tolua_S, "HTTPRequest");
			// tolua_variable(tolua_S, "Method", tolua_get_HTTPRequest_Method, tolua_set_HTTPRequest_Method);
			// tolua_variable(tolua_S, "Path", tolua_get_HTTPRequest_Path, tolua_set_HTTPRequest_Path);
			tolua_variable(tolua_S, "FormData", tolua_get_HTTPRequest_FormData, 0);
			tolua_variable(tolua_S, "Params", tolua_get_HTTPRequest_Params, 0);
			tolua_variable(tolua_S, "PostParams", tolua_get_HTTPRequest_PostParams, 0);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cWebAdmin");
			tolua_function(tolua_S, "GetHTMLEscapedString", tolua_AllToLua_cWebAdmin_GetHTMLEscapedString);
			tolua_function(tolua_S, "GetPlugins",           tolua_cWebAdmin_GetPlugins);
			tolua_function(tolua_S, "GetURLEncodedString",  tolua_AllToLua_cWebAdmin_GetURLEncodedString);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cWebPlugin");
			tolua_function(tolua_S, "GetTabNames", tolua_cWebPlugin_GetTabNames);
		tolua_endmodule(tolua_S);
			
		tolua_beginmodule(tolua_S, "cClientHandle");
			tolua_constant(tolua_S, "MAX_VIEW_DISTANCE", cClientHandle::MAX_VIEW_DISTANCE);
			tolua_constant(tolua_S, "MIN_VIEW_DISTANCE", cClientHandle::MIN_VIEW_DISTANCE);
			tolua_function(tolua_S, "SendPluginMessage", tolua_cClientHandle_SendPluginMessage);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cMojangAPI");
			tolua_function(tolua_S, "AddPlayerNameToUUIDMapping", tolua_cMojangAPI_AddPlayerNameToUUIDMapping);
			tolua_function(tolua_S, "GetPlayerNameFromUUID",      tolua_cMojangAPI_GetPlayerNameFromUUID);
			tolua_function(tolua_S, "GetUUIDFromPlayerName",      tolua_cMojangAPI_GetUUIDFromPlayerName);
			tolua_function(tolua_S, "GetUUIDsFromPlayerNames",    tolua_cMojangAPI_GetUUIDsFromPlayerNames);
			tolua_function(tolua_S, "MakeUUIDDashed",             tolua_cMojangAPI_MakeUUIDDashed);
			tolua_function(tolua_S, "MakeUUIDShort",              tolua_cMojangAPI_MakeUUIDShort);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cItemGrid");
			tolua_function(tolua_S, "GetSlotCoords", Lua_ItemGrid_GetSlotCoords);
		tolua_endmodule(tolua_S);

		tolua_function(tolua_S, "md5", tolua_md5);
		
		BindRankManager(tolua_S);
		BindNetwork(tolua_S);

	tolua_endmodule(tolua_S);
}




