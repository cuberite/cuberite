#include "ManualBindings.h"
#include "tolua++.h"
#include "cMCLogger.h"

#include "cRoot.h"
#include "cWorld.h"
#include "cPlugin.h"
#include "cPluginManager.h"
#include "cWebPlugin_Lua.h"
#include "cLuaCommandBinder.h"
#include "cPlayer.h"
#include "md5/md5.h"

extern std::vector<std::string> StringSplit(std::string str, std::string delim);

/****************************
 * Lua bound functions with special return types
 **/

static int tolua_StringSplit(lua_State* tolua_S)
{
	std::string str = ((std::string)  tolua_tocppstring(tolua_S,1,0));
	std::string delim = ((std::string)  tolua_tocppstring(tolua_S,2,0));

	std::vector<std::string> Split = StringSplit( str, delim );

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

static int tolua_cWorld_GetAllPlayers(lua_State* tolua_S)
{
	cWorld* self = (cWorld*)  tolua_tousertype(tolua_S,1,0);
	lua_State* L =  tolua_S;

	self->GetAllPlayers(L);

	return 1;
}

static int tolua_cPlugin_GetCommands(lua_State* tolua_S)
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

static int tolua_cWebPlugin_Lua_AddTab(lua_State* tolua_S)
{
	cWebPlugin_Lua* self = (cWebPlugin_Lua*)  tolua_tousertype(tolua_S,1,0);

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
		tolua_error(tolua_S,"#ferror in function 'AddTab'.",&tolua_err);
		return 0;
	}

	if( Reference != LUA_REFNIL )
	{
		if( !self->AddTab( Title.c_str(), tolua_S, Reference ) )
		{
			luaL_unref( tolua_S, LUA_REGISTRYINDEX, Reference );
		}
	}
	else
	{
		LOGERROR("ERROR: cWebPlugin_Lua:AddTab invalid function reference in 2nd argument (Title: \"%s\")", Title.c_str() );
	}

	return 0;
}

static int tolua_md5(lua_State* tolua_S)
{
	std::string SourceString = tolua_tostring(tolua_S, 1, 0);
	std::string CryptedString = md5( SourceString );
	tolua_pushstring( tolua_S, CryptedString.c_str() );
	return 1;
}

void ManualBindings::Bind( lua_State* tolua_S )
{
	tolua_beginmodule(tolua_S,NULL);
		tolua_function(tolua_S,"StringSplit",tolua_StringSplit);
		tolua_function(tolua_S,"LOG",tolua_LOG);
		tolua_function(tolua_S,"LOGINFO",tolua_LOGINFO);
		tolua_function(tolua_S,"LOGWARN",tolua_LOGWARN);
		tolua_function(tolua_S,"LOGERROR",tolua_LOGERROR);
		tolua_function(tolua_S,"Log",tolua_LOG); // Deprecated

		tolua_beginmodule(tolua_S,"cWorld");
			tolua_function(tolua_S,"GetAllPlayers",tolua_cWorld_GetAllPlayers);
		tolua_endmodule(tolua_S);
		tolua_beginmodule(tolua_S,"cPlugin");
			tolua_function(tolua_S,"GetCommands",tolua_cPlugin_GetCommands);
			tolua_function(tolua_S,"BindCommand",tolua_cPlugin_BindCommand);
		tolua_endmodule(tolua_S);
		tolua_beginmodule(tolua_S,"cPluginManager");
			tolua_function(tolua_S,"GetAllPlugins",tolua_cPluginManager_GetAllPlugins);
		tolua_endmodule(tolua_S);
		tolua_beginmodule(tolua_S,"cPlayer");
			tolua_function(tolua_S,"GetGroups",tolua_cPlayer_GetGroups);
		tolua_endmodule(tolua_S);
		tolua_beginmodule(tolua_S,"cWebPlugin_Lua");
			tolua_function(tolua_S,"AddTab",tolua_cWebPlugin_Lua_AddTab);
		tolua_endmodule(tolua_S);
			
		tolua_function(tolua_S,"md5",tolua_md5);

	tolua_endmodule(tolua_S);
}
