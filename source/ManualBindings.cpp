
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "ManualBindings.h"
#include "tolua++.h"

#include "cRoot.h"
#include "cWorld.h"
#include "cPlugin.h"
#include "cPlugin_NewLua.h"
#include "cPluginManager.h"
#include "cLuaCommandBinder.h"
#include "cPlayer.h"
#include "cWebAdmin.h"
#include "cStringMap.h"
#include "cClientHandle.h"
#include "md5/md5.h"





static bool report_errors(lua_State* lua, int status)
{
	if ( status!=0 )
	{
		std::string s = lua_tostring(lua, -1);
		LOGERROR("-- %s", s.c_str() );
		lua_pop(lua, 1);
		return true;
	}
	return false;
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





#define DEFINE_LUA_DOWITH(CONTAINER,ITEM,FOREACH,FNNAME) \
static int FNNAME(lua_State * tolua_S) \
{ \
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */ \
	if ((NumArgs != 2) && (NumArgs != 3)) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Requires 2 or 3 arguments, got %i", NumArgs ); \
		return 0; \
	} \
	\
	CONTAINER * self = (CONTAINER *)  tolua_tousertype(tolua_S, 1, 0); \
	\
	const char * ItemName = tolua_tocppstring(tolua_S, 2, ""); \
	if ((ItemName == NULL) || (ItemName[0] == 0)) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Expected a non-empty string for parameter #1"); \
		return 0; \
	} \
	if (!lua_isfunction( tolua_S, 3)) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Expected a function for parameter #2"); \
		return 0; \
	} \
	\
	/* luaL_ref gets reference to value on top of the stack, the table is the last argument and therefore on the top */ \
	int TableRef = LUA_REFNIL; \
	if (NumArgs == 3) \
	{ \
		TableRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX); \
		if (TableRef == LUA_REFNIL) \
		{ \
			LOGWARN("Error in function call '" #FOREACH "': Could not get value reference of parameter #3"); \
			return 0; \
		} \
	} \
	\
	/* table value is popped, and now function is on top of the stack */ \
	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX); \
	if (FuncRef == LUA_REFNIL) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Could not get function reference of parameter #2"); \
		return 0; \
	} \
	\
	class cLuaCallback : public cItemCallback<ITEM> \
	{ \
	public: \
		cLuaCallback(lua_State* a_LuaState, int a_FuncRef, int a_TableRef) \
			: LuaState( a_LuaState ) \
			, FuncRef( a_FuncRef ) \
			, TableRef( a_TableRef ) \
		{} \
	\
	private: \
		virtual bool Item(ITEM * a_Item) override \
		{ \
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */ \
			tolua_pushusertype(LuaState, a_Item, #ITEM); \
			if (TableRef != LUA_REFNIL) \
			{ \
				lua_rawgeti( LuaState, LUA_REGISTRYINDEX, TableRef);  /* Push table reference */ \
			} \
			\
			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0); \
			report_errors(LuaState, s); \
			return true; \
		} \
		lua_State * LuaState; \
		int FuncRef; \
		int TableRef; \
	} Callback(tolua_S, FuncRef, TableRef); \
	\
	bool bRetVal = self->FOREACH(ItemName, Callback); \
	\
	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */ \
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef); \
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef); \
	\
	/* Push return value on stack */ \
	tolua_pushboolean(tolua_S, bRetVal ); \
	return 1; \
}





#define DEFINE_LUA_FOREACHINCHUNK(CONTAINER,ITEM,FOREACH,FNNAME) \
static int FNNAME(lua_State * tolua_S) \
{ \
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */ \
	if ((NumArgs != 3) && (NumArgs != 4)) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Requires 3 or 4 arguments, got %i", NumArgs); \
		return 0; \
	} \
	\
	CONTAINER * self = (CONTAINER *)  tolua_tousertype(tolua_S, 1, 0); \
	if (!lua_isnumber(tolua_S, 2) || !lua_isnumber(tolua_S, 3)) \
	{ \
		LOGWARN("Errorin function call '" #FOREACH "': Expected a number for parameters #1 and #2"); \
		return 0; \
	} \
	\
	int ChunkX = ((int)tolua_tonumber(tolua_S, 2, 0)); \
	int ChunkZ = ((int)tolua_tonumber(tolua_S, 3, 0)); \
	\
	if (!lua_isfunction( tolua_S, 4)) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Expected a function for parameter #3"); \
		return 0; \
	} \
	\
	/* luaL_ref gets reference to value on top of the stack, the table is the last argument and therefore on the top */ \
	int TableRef = LUA_REFNIL; \
	if (NumArgs == 4) \
	{ \
		TableRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX); \
		if (TableRef == LUA_REFNIL) \
		{ \
			LOGWARN("Error in function call '" #FOREACH "': Could not get value reference of parameter #4"); \
			return 0; \
		} \
	} \
	\
	/* table value is popped, and now function is on top of the stack */ \
	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX); \
	if (FuncRef == LUA_REFNIL) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Could not get function reference of parameter #3"); \
		return 0; \
	} \
	\
	class cLuaCallback : public cItemCallback<ITEM> \
	{ \
	public: \
		cLuaCallback(lua_State* a_LuaState, int a_FuncRef, int a_TableRef) \
			: LuaState( a_LuaState ) \
			, FuncRef( a_FuncRef ) \
			, TableRef( a_TableRef ) \
		{} \
	\
	private: \
		virtual bool Item(ITEM * a_Item) override \
		{ \
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */ \
			tolua_pushusertype(LuaState, a_Item, #ITEM); \
			if (TableRef != LUA_REFNIL) \
			{ \
				lua_rawgeti( LuaState, LUA_REGISTRYINDEX, TableRef);  /* Push table reference */ \
			} \
			\
			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0); \
			if (report_errors(LuaState, s)) \
			{ \
				return true;  /* Abort enumeration */ \
			} \
			\
			if (lua_isboolean(LuaState, -1)) \
			{ \
				return (tolua_toboolean( LuaState, -1, 0) > 0); \
			} \
			return false;  /* Continue enumeration */ \
		} \
		lua_State * LuaState; \
		int FuncRef; \
		int TableRef; \
	} Callback(tolua_S, FuncRef, TableRef); \
	\
	bool bRetVal = self->FOREACH(ChunkX, ChunkZ, Callback); \
	\
	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */ \
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef); \
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef); \
	\
	/* Push return value on stack */ \
	tolua_pushboolean(tolua_S, bRetVal ); \
	return 1; \
}





#define DEFINE_LUA_FOREACH(CONTAINER,ITEM,FOREACH,FNNAME) \
static int FNNAME(lua_State * tolua_S) \
{ \
	int NumArgs = lua_gettop(tolua_S) - 1;  /* This includes 'self' */ \
	if( NumArgs != 1 && NumArgs != 2) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Requires 1 or 2 arguments, got %i", NumArgs ); \
		return 0; \
	} \
	\
	CONTAINER * self = (CONTAINER *)  tolua_tousertype(tolua_S, 1, 0); \
	\
	if (!lua_isfunction( tolua_S, 2)) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Expected a function for parameter #1"); \
		return 0; \
	} \
	\
	/* luaL_ref gets reference to value on top of the stack, the table is the last argument and therefore on the top */ \
	int TableRef = LUA_REFNIL; \
	if (NumArgs == 2) \
	{ \
		TableRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX); \
		if (TableRef == LUA_REFNIL) \
		{ \
			LOGWARN("Error in function call '" #FOREACH "': Could not get value reference of parameter #2"); \
			return 0; \
		} \
	} \
	\
	/* table value is popped, and now function is on top of the stack */ \
	int FuncRef = luaL_ref(tolua_S, LUA_REGISTRYINDEX); \
	if (FuncRef == LUA_REFNIL) \
	{ \
		LOGWARN("Error in function call '" #FOREACH "': Could not get function reference of parameter #1"); \
		return 0; \
	} \
	\
	class cLuaCallback : public cItemCallback<ITEM> \
	{ \
	public: \
		cLuaCallback(lua_State* a_LuaState, int a_FuncRef, int a_TableRef) \
			: LuaState( a_LuaState ) \
			, FuncRef( a_FuncRef ) \
			, TableRef( a_TableRef ) \
		{} \
		\
	private: \
		virtual bool Item(ITEM * a_Item) override \
		{ \
			lua_rawgeti( LuaState, LUA_REGISTRYINDEX, FuncRef);  /* Push function reference */ \
			tolua_pushusertype( LuaState, a_Item, #ITEM ); \
			if (TableRef != LUA_REFNIL) \
			{ \
				lua_rawgeti( LuaState, LUA_REGISTRYINDEX, TableRef);  /* Push table reference */ \
			} \
			\
			int s = lua_pcall(LuaState, (TableRef == LUA_REFNIL ? 1 : 2), 1, 0); \
			if (report_errors(LuaState, s)) \
			{ \
				return true;  /* Abort enumeration */ \
			} \
			\
			if (lua_isboolean(LuaState, -1)) \
			{ \
				return (tolua_toboolean( LuaState, -1, 0) > 0); \
			} \
			return false;  /* Continue enumeration */ \
		} \
		lua_State * LuaState; \
		int FuncRef; \
		int TableRef; \
	} Callback(tolua_S, FuncRef, TableRef); \
	\
	bool bRetVal = self->FOREACH(Callback); \
	\
	/* Unreference the values again, so the LUA_REGISTRYINDEX can make place for other references */ \
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, TableRef); \
	luaL_unref(tolua_S, LUA_REGISTRYINDEX, FuncRef); \
	\
	/* Push return value on stack */ \
	tolua_pushboolean(tolua_S, bRetVal ); \
	return 1; \
}





// Define the DoWith enumerators: (they take a string and a callback class)
DEFINE_LUA_DOWITH(cWorld, cPlayer, DoWithPlayer,        tolua_cWorld_DoWithPlayer);
DEFINE_LUA_DOWITH(cWorld, cPlayer, FindAndDoWithPlayer, tolua_cWorld_FindAndDoWithPlayer);
DEFINE_LUA_DOWITH(cRoot,  cPlayer, FindAndDoWithPlayer, tolua_cRoot_FindAndDoWithPlayer);

// Define the ForEach enumerators:
DEFINE_LUA_FOREACH(cWorld, cPlayer, ForEachPlayer, tolua_cWorld_ForEachPlayer);
DEFINE_LUA_FOREACH(cWorld, cEntity, ForEachEntity, tolua_cWorld_ForEachEntity);
DEFINE_LUA_FOREACH(cRoot,  cWorld,  ForEachWorld,  tolua_cRoot_ForEachWorld);
DEFINE_LUA_FOREACH(cRoot,  cPlayer, ForEachPlayer, tolua_cRoot_ForEachPlayer);

DEFINE_LUA_FOREACHINCHUNK(cWorld, cEntity,        ForEachEntityInChunk,  tolua_cWorld_ForEachEntityInChunk);
DEFINE_LUA_FOREACHINCHUNK(cWorld, cChestEntity,   ForEachChestInChunk,   tolua_cWorld_ForEachChestInChunk);
DEFINE_LUA_FOREACHINCHUNK(cWorld, cFurnaceEntity, ForEachFurnaceInChunk, tolua_cWorld_ForEachFurnaceInChunk);





static int tolua_cPlugin_GetCommands(lua_State * tolua_S)
{
	cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);

	const std::vector< cPlugin::CommandStruct > & AllCommands = self->GetCommands();

	lua_createtable(tolua_S, AllCommands.size(), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	std::vector< cPlugin::CommandStruct >::const_iterator iter = AllCommands.begin();
	while(iter != AllCommands.end())
	{
		const cPlugin::CommandStruct & CS = *iter;
		tolua_pushusertype( tolua_S, (void*)&CS, "const cPlugin::CommandStruct" );
		lua_rawseti(tolua_S, newTable, index);
		++iter;
		++index;
	}
	return 1;
}





static int tolua_cPluginManager_GetAllPlugins(lua_State* tolua_S)
{
	cPluginManager* self = (cPluginManager*)  tolua_tousertype(tolua_S,1,0);

	const cPluginManager::PluginList & AllPlugins = self->GetAllPlugins();

	lua_createtable(tolua_S, AllPlugins.size(), 0);
	int newTable = lua_gettop(tolua_S);
	int index = 1;
	cPluginManager::PluginList::const_iterator iter = AllPlugins.begin();
	while(iter != AllPlugins.end())
	{
		const cPlugin* Plugin = *iter;
		tolua_pushusertype( tolua_S, (void*)Plugin, "const cPlugin" );
		lua_rawseti(tolua_S, newTable, index);
		++iter;
		++index;
	}
	return 1;
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





static int tolua_cPlugin_BindCommand(lua_State* tolua_S)
{
	cPlugin* self = (cPlugin*)  tolua_tousertype(tolua_S,1,0);
	cPluginManager* PluginManager = cRoot::Get()->GetPluginManager();
	cLuaCommandBinder* CommandBinder = PluginManager->GetLuaCommandBinder();

	tolua_Error tolua_err;
	tolua_err.array = 0;
	tolua_err.index = 0;
	tolua_err.type = 0;

	std::string Permission = "";
	std::string Command = "";
	int Reference = LUA_REFNIL;

	if( tolua_isstring( tolua_S, 2, 0, &tolua_err ) &&
		lua_isfunction( tolua_S, 3 ) )
	{
		Reference = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		Command = ((std::string)  tolua_tocppstring(tolua_S,2,0));
	}
	else if( tolua_isstring( tolua_S, 2, 0, &tolua_err ) &&
			tolua_isstring( tolua_S, 3, 0, &tolua_err ) &&
			lua_isfunction( tolua_S, 4 ) )
	{
		Reference = luaL_ref(tolua_S, LUA_REGISTRYINDEX);
		Command = ((std::string)  tolua_tocppstring(tolua_S,2,0));
		Permission = ((std::string)  tolua_tocppstring(tolua_S,3,0));
	}
	else
	{
		if( tolua_err.type == 0 )
		{
			tolua_err.type = "function";
		}
		tolua_error(tolua_S,"#ferror in function 'BindCommand'.",&tolua_err);
		return 0;
	}

	if( Reference != LUA_REFNIL )
	{
		if( !CommandBinder->BindCommand( Command, Permission, self, tolua_S, Reference ) )
		{
			luaL_unref( tolua_S, LUA_REGISTRYINDEX, Reference );
		}
	}
	else
	{
		LOGERROR("ERROR: cPlugin:BindCommand invalid function reference in 2nd argument (Command: \"%s\")", Command.c_str() );
	}

	return 0;
}





static int tolua_cPlugin_NewLua_AddWebTab(lua_State* tolua_S)
{
	cPlugin_NewLua* self = (cPlugin_NewLua*)  tolua_tousertype(tolua_S,1,0);

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
		if( tolua_err.type == 0 )
		{
			tolua_err.type = "function";
		}
		tolua_error(tolua_S,"#ferror in function 'AddWebTab'.",&tolua_err);
		return 0;
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
			tolua_function(tolua_S, "ForEachWorld", tolua_cRoot_ForEachWorld);
			tolua_function(tolua_S, "FindAndDoWithPlayer", tolua_cRoot_FindAndDoWithPlayer);
			tolua_function(tolua_S, "ForEachPlayer", tolua_cRoot_ForEachPlayer);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cWorld");
			tolua_function(tolua_S, "ForEachPlayer",         tolua_cWorld_ForEachPlayer);
			tolua_function(tolua_S, "ForEachEntity",         tolua_cWorld_ForEachEntity);
			tolua_function(tolua_S, "ForEachEntityInChunk",  tolua_cWorld_ForEachEntityInChunk);
			tolua_function(tolua_S, "ForEachChestInChunk",   tolua_cWorld_ForEachChestInChunk);
			tolua_function(tolua_S, "ForEachFurnaceInChunk", tolua_cWorld_ForEachFurnaceInChunk);
			tolua_function(tolua_S, "DoWithPlayer",          tolua_cWorld_DoWithPlayer);
			tolua_function(tolua_S, "FindAndDoWithPlayer",   tolua_cWorld_FindAndDoWithPlayer);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPlugin");
			tolua_function(tolua_S, "GetCommands", tolua_cPlugin_GetCommands);
			tolua_function(tolua_S, "BindCommand", tolua_cPlugin_BindCommand);
		tolua_endmodule(tolua_S);
		
		tolua_beginmodule(tolua_S, "cPluginManager");
			tolua_function(tolua_S, "GetAllPlugins", tolua_cPluginManager_GetAllPlugins);
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

		tolua_function(tolua_S, "md5", tolua_md5);

	tolua_endmodule(tolua_S);
}




