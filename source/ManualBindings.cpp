
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ManualBindings.h"
#include "tolua++.h"

#include "Root.h"
#include "World.h"
#include "Plugin.h"
#include "Plugin_NewLua.h"
#include "PluginManager.h"
#include "Player.h"
#include "WebAdmin.h"
#include "StringMap.h"
#include "ClientHandle.h"
#include "ChestEntity.h"
#include "FurnaceEntity.h"
#include "md5/md5.h"





// fwd: LuaCommandBinder.cpp
bool report_errors(lua_State* lua, int status);





/****************************
 * Better error reporting for Lua
 **/
int tolua_do_error(lua_State* L, const char * a_pMsg, tolua_Error * a_pToLuaError)
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





int lua_do_error(lua_State* L, const char * a_pFormat, ...)
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





/****************************
 * Lua bound functions with special return types
 **/

static int tolua_StringSplit(lua_State* tolua_S)
{
	std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
	std::string delim = ((std::string)  tolua_tocppstring(tolua_S,2,0));

	AStringVector Split = StringSplit( str, delim );

	lua_createtable(tolua_S, Split.size(), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	std::vector<std::string>::const_iterator iter = Split.begin();
	while(iter != Split.end()) {
		tolua_pushstring( tolua_S, (*iter).c_str() );
		lua_rawseti(tolua_S, newTable, index);
		++iter;
		++index;
	}
	return 1;
}





static int tolua_LOG(lua_State* tolua_S)
{
	const char* str = tolua_tocppstring(tolua_S,1,0);
	cMCLogger::GetInstance()->LogSimple( str, 0 );
	return 0;
}





static int tolua_LOGINFO(lua_State* tolua_S)
{
	const char* str = tolua_tocppstring(tolua_S,1,0);
	cMCLogger::GetInstance()->LogSimple( str, 1 );
	return 0;
}





static int tolua_LOGWARN(lua_State* tolua_S)
{
	const char* str = tolua_tocppstring(tolua_S,1,0);
	cMCLogger::GetInstance()->LogSimple( str, 2 );
	return 0;
}





static int tolua_LOGERROR(lua_State* tolua_S)
{
	const char* str = tolua_tocppstring(tolua_S,1,0);
	cMCLogger::GetInstance()->LogSimple( str, 3 );
	return 0;
}





template<
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

	Ty1 * self = (Ty1 *)  tolua_tousertype(tolua_S, 1, 0); 

	const char * ItemName = tolua_tocppstring(tolua_S, 2, ""); 
	if ((ItemName == NULL) || (ItemName[0] == 0)) 
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
			: LuaState( a_LuaState ) 
			, FuncRef( a_FuncRef ) 
			, TableRef( a_TableRef ) 
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
			report_errors(LuaState, s); 
			return true; 
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
	tolua_pushboolean(tolua_S, bRetVal ); 
	return 1; 
}





template< class Ty1,
          class Ty2,
          bool (Ty1::*Func1)(int, int, int, cItemCallback<Ty2> &) >
static int tolua_DoWithXYZ(lua_State* tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */ 
	if ((NumArgs != 4) && (NumArgs != 5)) 
	{ 
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Requires 4 or 5 arguments, got %i", NumArgs);
	} 

	Ty1 * self = (Ty1 *)  tolua_tousertype(tolua_S, 1, 0); 
	if (!lua_isnumber(tolua_S, 2) || !lua_isnumber(tolua_S, 3) || !lua_isnumber(tolua_S, 4)) 
	{ 
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Expected a number for parameters #1, #2 and #3");
	} 

	int ItemX = ((int)tolua_tonumber(tolua_S, 2, 0)); 
	int ItemY = ((int)tolua_tonumber(tolua_S, 3, 0)); 
	int ItemZ = ((int)tolua_tonumber(tolua_S, 4, 0)); 
	LOG("x %i y %i z %i", ItemX, ItemY, ItemZ ); 
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
			: LuaState( a_LuaState ) 
			, FuncRef( a_FuncRef ) 
			, TableRef( a_TableRef ) 
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
			report_errors(LuaState, s); 
			return true; 
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
	tolua_pushboolean(tolua_S, bRetVal ); 
	return 1; 
}





template< class Ty1,
          class Ty2,
          bool (Ty1::*Func1)(int, int, cItemCallback<Ty2> &) >
static int tolua_ForEachInChunk(lua_State* tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */ 
	if ((NumArgs != 3) && (NumArgs != 4)) 
	{ 
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Requires 3 or 4 arguments, got %i", NumArgs);
	} 

	Ty1 * self = (Ty1 *)  tolua_tousertype(tolua_S, 1, 0); 
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
			: LuaState( a_LuaState ) 
			, FuncRef( a_FuncRef ) 
			, TableRef( a_TableRef ) 
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
			if (report_errors(LuaState, s)) 
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

	bool bRetVal = (self->*Func1)(ChunkX, ChunkZ, Callback);

	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */ 
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef); 
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef); 

	/* Push return value on stack */ 
	tolua_pushboolean(tolua_S, bRetVal ); 
	return 1; 
}





template< class Ty1,
          class Ty2, 
          bool (Ty1::*Func1)(cItemCallback<Ty2> &) >
static int tolua_ForEach(lua_State * tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if( NumArgs != 1 && NumArgs != 2)
	{
		return lua_do_error(tolua_S, "Error in function call '#funcname#': Requires 1 or 2 arguments, got %i", NumArgs);
	}

	Ty1 * self = (Ty1 *)  tolua_tousertype(tolua_S, 1, 0);
	if (self == NULL)
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
			: LuaState( a_LuaState )
			, FuncRef( a_FuncRef )
			, TableRef( a_TableRef )
		{}

	private:
		virtual bool Item(Ty2 * a_Item) override
		{
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushusertype( LuaState, a_Item, Ty2::GetClassStatic() );
			if (TableRef != LUA_REFNIL)
			{
				lua_rawgeti( LuaState, LUA_REGISTRYINDEX, TableRef);  /* Push table reference */
			}

			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0);
			if (report_errors(LuaState, s))
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
	tolua_pushboolean(tolua_S, bRetVal );
	return 1;
}





static int tolua_cPluginManager_GetAllPlugins(lua_State * tolua_S)
{
	cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);

	const cPluginManager::PluginMap & AllPlugins = self->GetAllPlugins();

	lua_newtable(tolua_S);
	//lua_createtable(tolua_S, AllPlugins.size(), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	cPluginManager::PluginMap::const_iterator iter = AllPlugins.begin();
	while(iter != AllPlugins.end())
	{
		const cPlugin* Plugin = iter->second;
		tolua_pushstring( tolua_S, iter->first.c_str() );
		if( Plugin != NULL )
		{
			tolua_pushusertype( tolua_S, (void*)Plugin, "const cPlugin" );
		}
		else
		{
			tolua_pushboolean(tolua_S, 0);
		}
		//lua_rawseti(tolua_S, newTable, index);
		lua_rawset(tolua_S, -3);
		++iter;
		++index;
	}
	return 1;
}





static int tolua_cPluginManager_ForEachCommand(lua_State * tolua_S)
{
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */
	if( NumArgs != 1)
	{
		LOGWARN("Error in function call 'ForEachCommand': Requires 1 argument, got %i", NumArgs);
		return 0;
	}

	cPluginManager * self = (cPluginManager *)tolua_tousertype(tolua_S, 1, 0);
	if (self == NULL)
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
			: LuaState( a_LuaState )
			, FuncRef( a_FuncRef )
		{}

	private:
		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) override
		{
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushcppstring(LuaState, a_Command);
			tolua_pushcppstring(LuaState, a_Permission);
			tolua_pushcppstring(LuaState, a_HelpString);

			int s = lua_pcall(LuaState, 3, 1, 0);
			if (report_errors(LuaState, s))
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
	if( NumArgs != 1)
	{
		LOGWARN("Error in function call 'ForEachConsoleCommand': Requires 1 argument, got %i", NumArgs);
		return 0;
	}

	cPluginManager * self = (cPluginManager *)tolua_tousertype(tolua_S, 1, 0);
	if (self == NULL)
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
			: LuaState( a_LuaState )
			, FuncRef( a_FuncRef )
		{}

	private:
		virtual bool Command(const AString & a_Command, const cPlugin * a_Plugin, const AString & a_Permission, const AString & a_HelpString) override
		{
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */
			tolua_pushcppstring(LuaState, a_Command);
			tolua_pushcppstring(LuaState, a_HelpString);

			int s = lua_pcall(LuaState, 2, 1, 0);
			if (report_errors(LuaState, s))
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
	// Function signature: cPluginManager:BindCommand(Command, Permission, Function, HelpString)
	
	// Get the plugin identification out of LuaState:
	lua_getglobal(L, LUA_PLUGIN_INSTANCE_VAR_NAME);
	if (!lua_islightuserdata(L, -1))
	{
		LOGERROR("cPluginManager:BindCommand() cannot get plugin instance, what have you done to my Lua state? Command-binding aborted.");
	}
	cPlugin_NewLua * Plugin = (cPlugin_NewLua *)lua_topointer(L, -1);
	lua_pop(L, 1);
	
	// Read the arguments to this API call:
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (L, 1, "cPluginManager", 0, &tolua_err) ||
		!tolua_iscppstring(L, 2, 0, &tolua_err) ||
		!tolua_iscppstring(L, 3, 0, &tolua_err) ||
		!tolua_iscppstring(L, 5, 0, &tolua_err) ||
		!tolua_isnoobj    (L, 6, &tolua_err)
	)
	{
		tolua_error(L, "#ferror in function 'BindCommand'.", &tolua_err);
		return 0;
	}
	if (!lua_isfunction(L, 4))
	{
		luaL_error(L, "\"BindCommand\" function expects a function as its 3rd parameter. Command-binding aborted.");
		return 0;
	}
	cPluginManager * self = (cPluginManager *)tolua_tousertype(L, 1, 0);
	AString Command   (tolua_tocppstring(L, 2, ""));
	AString Permission(tolua_tocppstring(L, 3, ""));
	AString HelpString(tolua_tocppstring(L, 5, ""));
	
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
		// Refused. Possibly already bound. Error message has been given, bail out silently.
		return 0;
	}
	
	Plugin->BindCommand(Command, FnRef);
	return 0;
}





static int tolua_cPluginManager_BindConsoleCommand(lua_State * L)
{
	// Function signature: cPluginManager:BindConsoleCommand(Command, Function, HelpString)
	
	// Get the plugin identification out of LuaState:
	lua_getglobal(L, LUA_PLUGIN_INSTANCE_VAR_NAME);
	if (!lua_islightuserdata(L, -1))
	{
		LOGERROR("cPluginManager:BindConsoleCommand() cannot get plugin instance, what have you done to my Lua state? Command-binding aborted.");
	}
	cPlugin_NewLua * Plugin = (cPlugin_NewLua *)lua_topointer(L, -1);
	lua_pop(L, 1);
	
	// Read the arguments to this API call:
	tolua_Error tolua_err;
	if (
		!tolua_isusertype (L, 1, "cPluginManager", 0, &tolua_err) ||  // self
		!tolua_iscppstring(L, 2, 0, &tolua_err) ||  // Command
		!tolua_iscppstring(L, 4, 0, &tolua_err) ||  // HelpString
		!tolua_isnoobj    (L, 5, &tolua_err)
	)
	{
		tolua_error(L, "#ferror in function 'BindConsoleCommand'.", &tolua_err);
		return 0;
	}
	if (!lua_isfunction(L, 3))
	{
		luaL_error(L, "\"BindConsoleCommand\" function expects a function as its 2nd parameter. Command-binding aborted.");
		return 0;
	}
	cPluginManager * self = (cPluginManager *)tolua_tousertype(L, 1, 0);
	AString Command   (tolua_tocppstring(L, 2, ""));
	AString HelpString(tolua_tocppstring(L, 4, ""));
	
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
		// Refused. Possibly already bound. Error message has been given, bail out silently.
		return 0;
	}
	
	Plugin->BindConsoleCommand(Command, FnRef);
	return 0;
}





static int tolua_cPlayer_GetGroups(lua_State* tolua_S)
{
	cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);

	const cPlayer::GroupList & AllGroups = self->GetGroups();

	lua_createtable(tolua_S, AllGroups.size(), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	cPlayer::GroupList::const_iterator iter = AllGroups.begin();
	while(iter != AllGroups.end())
	{
		const cGroup* Group = *iter;
		tolua_pushusertype( tolua_S, (void*)Group, "const cGroup" );
		lua_rawseti(tolua_S, newTable, index);
		++iter;
		++index;
	}
	return 1;
}





static int tolua_cPlayer_GetResolvedPermissions(lua_State* tolua_S)
{
	cPlayer* self = (cPlayer*)  tolua_tousertype(tolua_S,1,0);

	cPlayer::StringList AllPermissions = self->GetResolvedPermissions();

	lua_createtable(tolua_S, AllPermissions.size(), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	cPlayer::StringList::iterator iter = AllPermissions.begin();
	while(iter != AllPermissions.end())
	{
		std::string& Permission = *iter;
		tolua_pushstring( tolua_S, Permission.c_str() );
		lua_rawseti(tolua_S, newTable, index);
		++iter;
		++index;
	}
	return 1;
}





static int tolua_cPlugin_NewLua_AddWebTab(lua_State * tolua_S)
{
	cPlugin_NewLua * self = (cPlugin_NewLua*)tolua_tousertype(tolua_S,1,0);

	tolua_Error tolua_err;
	tolua_err.array = 0;
	tolua_err.index = 0;
	tolua_err.type = 0;

	std::string Title = "";
	int Reference = LUA_REFNIL;

	if( tolua_isstring( tolua_S, 2, 0, &tolua_err ) &&
		lua_isfunction( tolua_S, 3 ) )
	{
		Reference = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		Title = ((std::string)  tolua_tocppstring(tolua_S,2,0));
	}
	else
	{
		return tolua_do_error(tolua_S, "#ferror calling function '#funcname#'", &tolua_err);
	}

	if( Reference != LUA_REFNIL )
	{
		if( !self->AddWebTab( Title.c_str(), tolua_S, Reference ) )
		{
			luaL_unref( tolua_S, LUA_REGISTRYINDEX, Reference );
		}
	}
	else
	{
		LOGERROR("ERROR: cPlugin_NewLua:AddWebTab invalid function reference in 2nd argument (Title: \"%s\")", Title.c_str() );
	}

	return 0;
}





static int tolua_cPlugin_NewLua_AddTab(lua_State* tolua_S)
{
	cPlugin_NewLua * self = (cPlugin_NewLua *) tolua_tousertype(tolua_S, 1, 0);
	LOGWARN("WARNING: Using deprecated function AddTab()! Use AddWebTab() instead. (plugin \"%s\" in folder \"%s\")",
		self->GetName().c_str(), self->GetDirectory().c_str()
	);
	return tolua_cPlugin_NewLua_AddWebTab( tolua_S );
}




// Perhaps use this as well for copying tables https://github.com/keplerproject/rings/pull/1
static int copy_lua_values(lua_State * a_Source, lua_State * a_Destination, int i, int top)
{
	for(; i <= top; ++i )
	{
		int t = lua_type(a_Source, i);
		switch (t) {
			case LUA_TSTRING:  /* strings */
				{
					const char * s = lua_tostring(a_Source, i);
					LOGD("%i push string: %s", i, s);
					tolua_pushstring(a_Destination, s);
				}
				break;
			case LUA_TBOOLEAN:  /* booleans */
				{
					int b = tolua_toboolean(a_Source, i, false);
					LOGD("%i push bool: %i", i, b);
					tolua_pushboolean(a_Destination, b );
				}
				break;
			case LUA_TNUMBER:  /* numbers */
				{
					lua_Number d = tolua_tonumber(a_Source, i, 0);
					LOGD("%i push number: %0.2f", i, d);
					tolua_pushnumber(a_Destination, d );
				}
				break;
			case LUA_TUSERDATA:
				{
					const char * type = 0;
					if (lua_getmetatable(a_Source,i))
					{
						lua_rawget(a_Source, LUA_REGISTRYINDEX);
						type = lua_tostring(a_Source, -1);
						lua_pop(a_Source, 1); // Pop.. something?! I don't knooow~~ T_T
					}
					
					// don't need tolua_tousertype we already have the type
					void * ud = tolua_touserdata(a_Source, i, 0);
					LOGD("%i push usertype: %p of type '%s'", i, ud, type);
					if( type == 0 )
					{
						LOGERROR("Call(): Something went wrong when trying to get usertype name!");
						return 0;
					}
					tolua_pushusertype(a_Destination, ud, type);
				}
				break;
			default:  /* other values */
				LOGERROR("Call(): Unsupported value: '%s'. Can only use numbers and strings!", lua_typename(a_Source, t));
				return 0;
		}
	}
	return 1;
}





static int tolua_cPlugin_Call(lua_State* tolua_S)
{
	cPlugin_NewLua * self = (cPlugin_NewLua *) tolua_tousertype(tolua_S, 1, 0);
	lua_State* targetState = self->GetLuaState();
	int targetTop = lua_gettop(targetState);

	int top = lua_gettop(tolua_S);
	LOGD("total in stack: %i", top );

	std::string funcName = tolua_tostring(tolua_S, 2, "");
	LOGD("Func name: %s", funcName.c_str() );

	lua_getglobal(targetState, funcName.c_str());
	if(!lua_isfunction(targetState,-1))
	{
		LOGWARN("Error could not find function '%s' in plugin '%s'", funcName.c_str(), self->GetName().c_str() );
		lua_pop(targetState,1);
		return 0;
	}

	if( copy_lua_values(tolua_S, targetState, 3, top) == 0 ) // Start at 3 because 1 and 2 are the plugin and function name respectively
	{
		// something went wrong, exit
		return 0;
	}
	
	int s = lua_pcall(targetState, top-2, LUA_MULTRET, 0);
	if( report_errors( targetState, s ) )
	{
		LOGWARN("Error while calling function '%s' in plugin '%s'", funcName.c_str(), self->GetName().c_str() );
		return 0;
	}

	int nresults = lua_gettop(targetState) - targetTop;
	LOGD("num results: %i", nresults);
	int ttop = lua_gettop(targetState);
	if( copy_lua_values(targetState, tolua_S, targetTop+1, ttop) == 0 ) // Start at targetTop+1 and I have no idea why xD
	{
		// something went wrong, exit
		return 0;
	}

	lua_pop(targetState, nresults); // I have no idea what I'm doing, but it works

	return nresults;
}





static int tolua_md5(lua_State* tolua_S)
{
	std::string SourceString = tolua_tostring(tolua_S, 1, 0);
	std::string CryptedString = md5( SourceString );
	tolua_pushstring( tolua_S, CryptedString.c_str() );
	return 1;
}





static int tolua_push_StringStringMap(lua_State* tolua_S, std::map< std::string, std::string >& a_StringStringMap )
{
	lua_newtable(tolua_S);
	int top = lua_gettop(tolua_S);

	for( std::map< std::string, std::string >::iterator it = a_StringStringMap.begin(); it != a_StringStringMap.end(); ++it )
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
	HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
	return tolua_push_StringStringMap(tolua_S, self->Params);
}





static int tolua_get_HTTPRequest_PostParams(lua_State* tolua_S)
{
	HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
	return tolua_push_StringStringMap(tolua_S, self->PostParams);
}





static int tolua_get_HTTPRequest_FormData(lua_State* tolua_S)
{
	HTTPRequest* self = (HTTPRequest*)  tolua_tousertype(tolua_S,1,0);
	std::map< std::string, HTTPFormData >& FormData = self->FormData;

	lua_newtable(tolua_S);
	int top = lua_gettop(tolua_S);

	for( std::map< std::string, HTTPFormData >::iterator it = FormData.begin(); it != FormData.end(); ++it )
	{
		lua_pushstring(tolua_S, it->first.c_str() );
		tolua_pushusertype(tolua_S, &(it->second), "HTTPFormData" );
		//lua_pushlstring(tolua_S, it->second.Value.c_str(), it->second.Value.size() ); // Might contain binary data
		lua_settable(tolua_S, top);
	}

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
		const cItemGrid * self = (const cItemGrid *)tolua_tousertype(L, 1, 0);
		int SlotNum = (int)tolua_tonumber(L, 2, 0);
		if (self == NULL)
		{
			tolua_error(L, "invalid 'self' in function 'cItemGrid:GetSlotCoords'", NULL);
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





void ManualBindings::Bind( lua_State* tolua_S )
{
	tolua_beginmodule(tolua_S,NULL);
		tolua_function(tolua_S, "StringSplit", tolua_StringSplit);
		tolua_function(tolua_S, "LOG",         tolua_LOG);
		tolua_function(tolua_S, "LOGINFO",     tolua_LOGINFO);
		tolua_function(tolua_S, "LOGWARN",     tolua_LOGWARN);
		tolua_function(tolua_S, "LOGERROR",    tolua_LOGERROR);
		tolua_function(tolua_S, "Log",         tolua_LOG); // Deprecated
		
		tolua_beginmodule(tolua_S, "cRoot");
			tolua_function(tolua_S, "ForEachWorld",        tolua_ForEach<cRoot, cWorld,  &cRoot::ForEachWorld>);
			tolua_function(tolua_S, "FindAndDoWithPlayer", tolua_DoWith <cRoot, cPlayer, &cRoot::FindAndDoWithPlayer>);
			tolua_function(tolua_S, "ForEachPlayer",       tolua_ForEach<cRoot, cPlayer, &cRoot::ForEachPlayer>);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cWorld");
			tolua_function(tolua_S, "ForEachPlayer",         tolua_ForEach<cWorld, cPlayer, &cWorld::ForEachPlayer>);
			tolua_function(tolua_S, "ForEachEntity",         tolua_ForEach<cWorld, cEntity, &cWorld::ForEachEntity>);
			tolua_function(tolua_S, "ForEachEntityInChunk",  tolua_ForEachInChunk<cWorld, cEntity,        &cWorld::ForEachEntityInChunk>);
			tolua_function(tolua_S, "ForEachChestInChunk",   tolua_ForEachInChunk<cWorld, cChestEntity,   &cWorld::ForEachChestInChunk>);
			tolua_function(tolua_S, "ForEachFurnaceInChunk", tolua_ForEachInChunk<cWorld, cFurnaceEntity, &cWorld::ForEachFurnaceInChunk>);
			tolua_function(tolua_S, "DoWithPlayer",          tolua_DoWith<cWorld, cPlayer, &cWorld::DoWithPlayer>);
			tolua_function(tolua_S, "FindAndDoWithPlayer",   tolua_DoWith<cWorld, cPlayer, &cWorld::FindAndDoWithPlayer>);
			tolua_function(tolua_S, "DoWithChestAt",         tolua_DoWithXYZ<cWorld, cChestEntity, &cWorld::DoWithChestAt>);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPlugin");
			tolua_function(tolua_S, "Call", tolua_cPlugin_Call);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPluginManager");
			tolua_function(tolua_S, "GetAllPlugins",         tolua_cPluginManager_GetAllPlugins);
			tolua_function(tolua_S, "BindCommand",           tolua_cPluginManager_BindCommand);
			tolua_function(tolua_S, "BindConsoleCommand",    tolua_cPluginManager_BindConsoleCommand);
			tolua_function(tolua_S, "ForEachCommand",        tolua_cPluginManager_ForEachCommand);
			tolua_function(tolua_S, "ForEachConsoleCommand", tolua_cPluginManager_ForEachConsoleCommand);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPlayer");
			tolua_function(tolua_S, "GetGroups",              tolua_cPlayer_GetGroups);
			tolua_function(tolua_S, "GetResolvedPermissions", tolua_cPlayer_GetResolvedPermissions);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPlugin_NewLua");
			tolua_function(tolua_S, "AddWebTab", tolua_cPlugin_NewLua_AddWebTab);
			tolua_function(tolua_S, "AddTab", tolua_cPlugin_NewLua_AddTab);
		tolua_endmodule(tolua_S);

		tolua_cclass(tolua_S,"HTTPRequest","HTTPRequest","",NULL);
		tolua_beginmodule(tolua_S,"HTTPRequest");
			//tolua_variable(tolua_S,"Method",tolua_get_HTTPRequest_Method,tolua_set_HTTPRequest_Method);
			//tolua_variable(tolua_S,"Path",tolua_get_HTTPRequest_Path,tolua_set_HTTPRequest_Path);
			tolua_variable(tolua_S,"Params",tolua_get_HTTPRequest_Params,0);
			tolua_variable(tolua_S,"PostParams",tolua_get_HTTPRequest_PostParams,0);
			tolua_variable(tolua_S,"FormData",tolua_get_HTTPRequest_FormData,0);
		tolua_endmodule(tolua_S);
			
		tolua_beginmodule(tolua_S, "cClientHandle");
			tolua_constant(tolua_S, "MIN_VIEW_DISTANCE", cClientHandle::MIN_VIEW_DISTANCE);
			tolua_constant(tolua_S, "MAX_VIEW_DISTANCE", cClientHandle::MAX_VIEW_DISTANCE);
		tolua_endmodule(tolua_S);

		tolua_beginmodule(tolua_S, "cItemGrid");
			tolua_function(tolua_S, "GetSlotCoords", Lua_ItemGrid_GetSlotCoords);
		tolua_endmodule(tolua_S);

		tolua_function(tolua_S, "md5", tolua_md5);

	tolua_endmodule(tolua_S);
}




