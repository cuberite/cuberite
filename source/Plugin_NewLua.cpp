
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#define LUA_USE_POSIX
#include "Plugin_NewLua.h"
#include "MCLogger.h"

extern "C"
{
#include "lualib.h"
}

#include "tolua++.h"
#include "Bindings.h"
#include "ManualBindings.h"





bool report_errors(lua_State * lua, int status)
{
	if (status == 0)
	{
		// No error to report
		return false;
	}
	
	LOGERROR("LUA: %s", lua_tostring(lua, -1));
	lua_pop(lua, 1);
	return true;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPlugin_NewLua:

cPlugin_NewLua::cPlugin_NewLua(const AString & a_PluginDirectory)
	: m_LuaState( 0 )
	, cWebPlugin()
	, cPlugin(a_PluginDirectory)
{
}





cPlugin_NewLua::~cPlugin_NewLua()
{
	cCSLock Lock(m_CriticalSection);

	if( m_LuaState )
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
	}
}





bool cPlugin_NewLua::Initialize(void)
{
	cCSLock Lock(m_CriticalSection);
	if (m_LuaState == NULL)
	{	
		m_LuaState = lua_open();
		luaL_openlibs(m_LuaState);
		tolua_AllToLua_open(m_LuaState);
		ManualBindings::Bind(m_LuaState);
		
		// Inject the identification global variables into the state:
		lua_pushlightuserdata(m_LuaState, this);
		lua_setglobal(m_LuaState, LUA_PLUGIN_INSTANCE_VAR_NAME);
		lua_pushstring(m_LuaState, GetName().c_str());
		lua_setglobal(m_LuaState, LUA_PLUGIN_NAME_VAR_NAME);
	}

	std::string PluginPath = FILE_IO_PREFIX + GetLocalDirectory() + "/";

	// Load all files for this plugin, and execute them
	AStringList Files = GetDirectoryContents(PluginPath.c_str());
	for (AStringList::const_iterator itr = Files.begin(); itr != Files.end(); ++itr)
	{
		if (itr->rfind(".lua") == AString::npos)
		{
			continue;
		}
		AString Path = PluginPath + *itr;
		int s = luaL_loadfile(m_LuaState, Path.c_str() );
		if( report_errors( m_LuaState, s ) )
		{
			LOGERROR("Can't load plugin %s because of an error in file %s", GetLocalDirectory().c_str(), Path.c_str() );
			lua_close( m_LuaState );
			m_LuaState = 0;
			return false;
		}

		s = lua_pcall(m_LuaState, 0, LUA_MULTRET, 0);
		if( report_errors( m_LuaState, s ) )
		{
			LOGERROR("Error in plugin %s in file %s", GetLocalDirectory().c_str(), Path.c_str() );
			lua_close( m_LuaState );
			m_LuaState = 0;
			return false;
		}
	}  // for itr - Files[]

	// Call intialize function
	if (!PushFunction("Initialize"))
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	tolua_pushusertype(m_LuaState, this, "cPlugin_NewLua");
	
	if (!CallFunction(1, 1, "Initialize"))
	{
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	if( !lua_isboolean( m_LuaState, -1 ) )
	{
		LOGWARN("Error in plugin %s Initialize() must return a boolean value!", GetLocalDirectory().c_str() );
		lua_close( m_LuaState );
		m_LuaState = 0;
		return false;
	}

	bool bSuccess = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	return bSuccess;
}





void cPlugin_NewLua::OnDisable()
{
	cCSLock Lock(m_CriticalSection);
	if (!PushFunction("OnDisable", false)) // false = don't log error if not found
	{
		return;
	}

	CallFunction(0, 0, "OnDisable");
}





void cPlugin_NewLua::Tick(float a_Dt)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_TICK);
	if (!PushFunction(FnName))
	{
		return;
	}

	tolua_pushnumber( m_LuaState, a_Dt );

	CallFunction(1, 0, FnName);
}





bool cPlugin_NewLua::OnBlockToPickups(cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_BLOCK_TO_PICKUPS);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_World, "cWorld");
	tolua_pushusertype(m_LuaState, a_Digger, "cEntity");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockType);
	tolua_pushnumber  (m_LuaState, a_BlockMeta);
	tolua_pushusertype(m_LuaState, &a_Pickups, "cItems");

	if (!CallFunction(8, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnChat(cPlayer * a_Player, const AString & a_Message)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHAT);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	tolua_pushstring  (m_LuaState, a_Message.c_str());

	if (!CallFunction(2, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnChunkAvailable(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHUNK_AVAILABLE);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}
	
	tolua_pushusertype(m_LuaState, a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_ChunkX);
	tolua_pushnumber  (m_LuaState, a_ChunkZ);
	
	if (!CallFunction(3, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHUNK_GENERATED);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}
	
	tolua_pushusertype(m_LuaState, a_World,     "cWorld");
	tolua_pushnumber  (m_LuaState, a_ChunkX);
	tolua_pushnumber  (m_LuaState, a_ChunkZ);
	tolua_pushusertype(m_LuaState, a_ChunkDesc, "cChunkDesc");
	
	if (!CallFunction(4, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_pLuaChunk)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHUNK_GENERATING);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_World,     "cWorld");
	tolua_pushnumber  (m_LuaState, a_ChunkX);
	tolua_pushnumber  (m_LuaState, a_ChunkZ);
	tolua_pushusertype(m_LuaState, a_pLuaChunk, "cChunkDesc");

	if (!CallFunction(4, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnChunkUnloaded(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHUNK_UNLOADED);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}
	
	tolua_pushusertype(m_LuaState, a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_ChunkX);
	tolua_pushnumber  (m_LuaState, a_ChunkZ);
	
	if (!CallFunction(3, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnChunkUnloading(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHUNK_UNLOADING);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}
	
	tolua_pushusertype(m_LuaState, a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_ChunkX);
	tolua_pushnumber  (m_LuaState, a_ChunkZ);
	
	if (!CallFunction(3, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnCollectingPickup(cPlayer * a_Player, cPickup * a_Pickup)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_COLLECTING_PICKUP);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	tolua_pushusertype(m_LuaState, a_Pickup, "cPickup");

	if (!CallFunction(2, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CRAFTING_NO_RECIPE);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");
	tolua_pushusertype(m_LuaState, (void *)a_Grid,   "cCraftingGrid");
	tolua_pushusertype(m_LuaState, (void *)a_Recipe, "cCraftingRecipe");

	if (!CallFunction(3, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnDisconnect(cPlayer * a_Player, const AString & a_Reason)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_DISCONNECT);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	tolua_pushstring  (m_LuaState, a_Reason.c_str());

	if (!CallFunction(2, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnExecuteCommand(cPlayer * a_Player, const AStringVector & a_Split)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_EXECUTE_COMMAND);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");

	// Push the split:
	lua_createtable(m_LuaState, a_Split.size(), 0);
	int newTable = lua_gettop(m_LuaState);
	int index = 1;
	std::vector<std::string>::const_iterator iter = a_Split.begin(), end = a_Split.end();
	while(iter != end)
	{
		tolua_pushstring(m_LuaState, (*iter).c_str());
		lua_rawseti(m_LuaState, newTable, index);
		++iter;
		++index;
	}

	if (!CallFunction(2, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnHandshake(cClientHandle * a_Client, const AString & a_Username)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_HANDSHAKE);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, a_Client, "cClientHandle");
	tolua_pushstring  (m_LuaState, a_Username.c_str());

	if (!CallFunction(2, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnKilling(cPawn & a_Victim, cEntity * a_Killer)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_KILLING);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Victim, "cPawn");
	tolua_pushusertype(m_LuaState, a_Killer,  "cEntity");

	if (!CallFunction(2, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnLogin(cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_LOGIN);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype (m_LuaState, a_Client, "cClientHandle");
	tolua_pushnumber   (m_LuaState, a_ProtocolVersion);
	tolua_pushcppstring(m_LuaState, a_Username);

	if (!CallFunction(3, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_BREAKING_BLOCK);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_BlockType);
	tolua_pushnumber  (m_LuaState, a_BlockMeta);

	if (!CallFunction(7, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerBrokenBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_BROKEN_BLOCK);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_BlockType);
	tolua_pushnumber  (m_LuaState, a_BlockMeta);

	if (!CallFunction(7, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerEating(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_EATING);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");

	if (!CallFunction(1, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerJoined(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_JOINED);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");

	if (!CallFunction(1, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerLeftClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_LEFT_CLICK);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_Status);

	if (!CallFunction(6, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerMoved(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_MOVING);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");

	if (!CallFunction(1, 1, FnName))
	{
		return false;
	}
	
	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerPlacedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_PLACED_BLOCK);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_CursorX);
	tolua_pushnumber  (m_LuaState, a_CursorY);
	tolua_pushnumber  (m_LuaState, a_CursorZ);
	tolua_pushnumber  (m_LuaState, a_BlockType);
	tolua_pushnumber  (m_LuaState, a_BlockMeta);

	if (!CallFunction(10, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerPlacingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_PLACING_BLOCK);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_CursorX);
	tolua_pushnumber  (m_LuaState, a_CursorY);
	tolua_pushnumber  (m_LuaState, a_CursorZ);
	tolua_pushnumber  (m_LuaState, a_BlockType);
	tolua_pushnumber  (m_LuaState, a_BlockMeta);

	if (!CallFunction(10, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerRightClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_RIGHT_CLICK);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_CursorX);
	tolua_pushnumber  (m_LuaState, a_CursorY);
	tolua_pushnumber  (m_LuaState, a_CursorZ);

	if (!CallFunction(8, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerShooting(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_SHOOTING);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");

	if (!CallFunction(1, 1, FnName))
	{
		return false;
	}
	
	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerSpawned(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_SPAWNED);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");

	if (!CallFunction(1, 1, FnName))
	{
		return false;
	}
	
	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerTossingItem(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_TOSSING_ITEM);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");

	if (!CallFunction(1, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerUsedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_USED_BLOCK);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_CursorX);
	tolua_pushnumber  (m_LuaState, a_CursorY);
	tolua_pushnumber  (m_LuaState, a_CursorZ);
	tolua_pushnumber  (m_LuaState, a_BlockType);
	tolua_pushnumber  (m_LuaState, a_BlockMeta);

	if (!CallFunction(10, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerUsedItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) 
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_USED_ITEM);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_CursorX);
	tolua_pushnumber  (m_LuaState, a_CursorY);
	tolua_pushnumber  (m_LuaState, a_CursorZ);

	if (!CallFunction(8, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerUsingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_USING_BLOCK);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_CursorX);
	tolua_pushnumber  (m_LuaState, a_CursorY);
	tolua_pushnumber  (m_LuaState, a_CursorZ);
	tolua_pushnumber  (m_LuaState, a_BlockType);
	tolua_pushnumber  (m_LuaState, a_BlockMeta);

	if (!CallFunction(10, 1, "OnPlayerUsingBlock"))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerUsingItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_USING_ITEM);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Player, "cPlayer");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushnumber  (m_LuaState, a_BlockFace);
	tolua_pushnumber  (m_LuaState, a_CursorX);
	tolua_pushnumber  (m_LuaState, a_CursorY);
	tolua_pushnumber  (m_LuaState, a_CursorZ);

	if (!CallFunction(8, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_POST_CRAFTING);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");
	tolua_pushusertype(m_LuaState, (void *)a_Grid,   "cCraftingGrid");
	tolua_pushusertype(m_LuaState, (void *)a_Recipe, "cCraftingRecipe");

	if (!CallFunction(3, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPreCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PRE_CRAFTING);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");
	tolua_pushusertype(m_LuaState, (void *)a_Grid,   "cCraftingGrid");
	tolua_pushusertype(m_LuaState, (void *)a_Recipe, "cCraftingRecipe");

	if (!CallFunction(3, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnTakeDamage(cPawn & a_Receiver, TakeDamageInfo & a_TDI)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_TAKE_DAMAGE);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_Receiver, "cPawn");
	tolua_pushusertype(m_LuaState, &a_TDI,      "TakeDamageInfo");

	if (!CallFunction(2, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) != 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnUpdatedSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4,
	cPlayer * a_Player
)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_UPDATED_SIGN);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, (void *)a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushstring  (m_LuaState, a_Line1.c_str());
	tolua_pushstring  (m_LuaState, a_Line2.c_str());
	tolua_pushstring  (m_LuaState, a_Line3.c_str());
	tolua_pushstring  (m_LuaState, a_Line4.c_str());
	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");

	if (!CallFunction(9, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnUpdatingSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4,
	cPlayer * a_Player
)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_UPDATING_SIGN);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, (void *)a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_BlockX);
	tolua_pushnumber  (m_LuaState, a_BlockY);
	tolua_pushnumber  (m_LuaState, a_BlockZ);
	tolua_pushstring  (m_LuaState, a_Line1.c_str());
	tolua_pushstring  (m_LuaState, a_Line2.c_str());
	tolua_pushstring  (m_LuaState, a_Line3.c_str());
	tolua_pushstring  (m_LuaState, a_Line4.c_str());
	tolua_pushusertype(m_LuaState, (void *)a_Player, "cPlayer");

	if (!CallFunction(9, 5, "OnUpdatingSign"))
	{
		return false;
	}

	
	bool bRetVal = (tolua_toboolean(m_LuaState, -5, 0) > 0);
	if (lua_isstring(m_LuaState, -4))
	{
		a_Line1 = tolua_tostring(m_LuaState, -4, "");
	}
	if (lua_isstring(m_LuaState, -3))
	{
		a_Line2 = tolua_tostring(m_LuaState, -3, "");
	}
	if (lua_isstring(m_LuaState, -2))
	{
		a_Line3 = tolua_tostring(m_LuaState, -2, "");
	}
	if (lua_isstring(m_LuaState, -1))
	{
		a_Line4 = tolua_tostring(m_LuaState, -1, "");
	}
	return bRetVal;
}





bool cPlugin_NewLua::OnWeatherChanged(cWorld & a_World)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_WEATHER_CHANGED);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_World, "cWorld");

	if (!CallFunction(1, 1, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnWeatherChanging(cWorld & a_World, eWeather & a_NewWeather)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_WEATHER_CHANGED);
	ASSERT(FnName != NULL);
	if (!PushFunction(FnName))
	{
		return false;
	}

	tolua_pushusertype(m_LuaState, &a_World, "cWorld");
	tolua_pushnumber  (m_LuaState, a_NewWeather);

	if (!CallFunction(2, 2, FnName))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	if (lua_isnumber(m_LuaState, -2))
	{
		a_NewWeather = (eWeather)lua_tointeger(m_LuaState, -2);
	}
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::HandleCommand(const AStringVector & a_Split, cPlayer * a_Player)
{
	ASSERT(!a_Split.empty());
	CommandMap::iterator cmd = m_Commands.find(a_Split[0]);
	if (cmd == m_Commands.end())
	{
		LOGWARNING("Command handler is registered in cPluginManager but not in cPlugin, wtf? Command \"%s\".", a_Split[0].c_str());
		return false;
	}
	
	cCSLock Lock(m_CriticalSection);
	
	// Push the function to be called:
	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, cmd->second);  // same as lua_getref()
	
	// Push the split:
	lua_createtable(m_LuaState, a_Split.size(), 0);
	int newTable = lua_gettop(m_LuaState);
	int index = 1;
	std::vector<std::string>::const_iterator iter = a_Split.begin(), end = a_Split.end();
	while(iter != end)
	{
		tolua_pushstring(m_LuaState, (*iter).c_str());
		lua_rawseti(m_LuaState, newTable, index);
		++iter;
		++index;
	}
	
	// Push player:
	tolua_pushusertype(m_LuaState, a_Player, "cPlayer");
	
	// Call function:
	int s = lua_pcall(m_LuaState, 2, 1, 0);
	if (report_errors(m_LuaState, s))
	{
		LOGERROR("LUA error in %s. Stack size: %i", __FUNCTION__, lua_gettop(m_LuaState));
		return false;
	}
	
	// Handle return value:
	bool RetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);  // Pop return value
	
	return RetVal;
}





bool cPlugin_NewLua::HandleConsoleCommand(const AStringVector & a_Split)
{
	ASSERT(!a_Split.empty());
	CommandMap::iterator cmd = m_ConsoleCommands.find(a_Split[0]);
	if (cmd == m_ConsoleCommands.end())
	{
		LOGWARNING("Console command handler is registered in cPluginManager but not in cPlugin, wtf? Console command \"%s\".", a_Split[0].c_str());
		return false;
	}
	
	cCSLock Lock(m_CriticalSection);
	
	// Push the function to be called:
	lua_rawgeti(m_LuaState, LUA_REGISTRYINDEX, cmd->second);  // same as lua_getref()
	
	// Push the split:
	lua_createtable(m_LuaState, a_Split.size(), 0);
	int newTable = lua_gettop(m_LuaState);
	int index = 1;
	std::vector<std::string>::const_iterator iter = a_Split.begin(), end = a_Split.end();
	while(iter != end)
	{
		tolua_pushstring(m_LuaState, (*iter).c_str());
		lua_rawseti(m_LuaState, newTable, index);
		++iter;
		++index;
	}
	
	// Call function:
	int s = lua_pcall(m_LuaState, 1, 1, 0);
	if (report_errors(m_LuaState, s))
	{
		LOGERROR("Lua error. Stack size: %i", lua_gettop(m_LuaState));
		return false;
	}
	
	// Handle return value:
	bool RetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);  // Pop return value
	
	return RetVal;
}





void cPlugin_NewLua::ClearCommands(void)
{
	cCSLock Lock(m_CriticalSection);

	// Unreference the bound functions so that Lua can GC them
	if (m_LuaState != NULL)
	{
		for (CommandMap::iterator itr = m_Commands.begin(), end = m_Commands.end(); itr != end; ++itr)
		{
			luaL_unref(m_LuaState, LUA_REGISTRYINDEX, itr->second);
		}
	}
	m_Commands.clear();
}





void cPlugin_NewLua::ClearConsoleCommands(void)
{
	cCSLock Lock(m_CriticalSection);

	// Unreference the bound functions so that Lua can GC them
	if (m_LuaState != NULL)
	{
		for (CommandMap::iterator itr = m_ConsoleCommands.begin(), end = m_ConsoleCommands.end(); itr != end; ++itr)
		{
			luaL_unref(m_LuaState, LUA_REGISTRYINDEX, itr->second);
		}
	}
	m_ConsoleCommands.clear();
}





bool cPlugin_NewLua::CanAddHook(cPluginManager::PluginHook a_Hook)
{
	const char * FnName = GetHookFnName(a_Hook);
	if (FnName == NULL)
	{
		// Unknown hook ID
		LOGWARNING("Plugin %s wants to add an unknown hook ID (%d). The plugin need not work properly.", GetName().c_str(), a_Hook);
		return false;
	}
	
	// Check if the function is available
	lua_getglobal(m_LuaState, FnName);
	bool res = lua_isfunction(m_LuaState, -1);
	lua_pop(m_LuaState, 1);
	
	if (res)
	{
		return true;
	}
	
	LOGWARNING("Plugin %s wants to add a hook (%d), but it doesn't provide the callback function \"%s\" for it. The plugin need not work properly.", 
		GetName().c_str(), a_Hook, FnName
	);
	
	// Lua stacktrace:
	LOGWARNING("Stack trace:");
	lua_Debug entry;
	int depth = 0; 
	while (lua_getstack(m_LuaState, depth, &entry))
	{
		int status = lua_getinfo(m_LuaState, "Sln", &entry);
		assert(status);

		LOGWARNING("  %s(%d): %s", entry.short_src, entry.currentline, entry.name ? entry.name : "?");
		depth++;
	}
	LOGWARNING("Stack trace end");

	return false;
}





const char * cPlugin_NewLua::GetHookFnName(cPluginManager::PluginHook a_Hook)
{
	switch (a_Hook)
	{
		case cPluginManager::HOOK_BLOCK_TO_PICKUPS:      return "OnBlockToPickups";
		case cPluginManager::HOOK_CHAT:                  return "OnChat";
		case cPluginManager::HOOK_CHUNK_AVAILABLE:       return "OnChunkAvailable";
		case cPluginManager::HOOK_CHUNK_GENERATED:       return "OnChunkGenerated";
		case cPluginManager::HOOK_CHUNK_GENERATING:      return "OnChunkGenerating";
		case cPluginManager::HOOK_CHUNK_UNLOADED:        return "OnChunkUnloaded";
		case cPluginManager::HOOK_CHUNK_UNLOADING:       return "OnChunkUnloading";
		case cPluginManager::HOOK_COLLECTING_PICKUP:     return "OnCollectingPickup";
		case cPluginManager::HOOK_CRAFTING_NO_RECIPE:    return "OnCraftingNoRecipe";
		case cPluginManager::HOOK_DISCONNECT:            return "OnDisconnect";
		case cPluginManager::HOOK_EXECUTE_COMMAND:       return "OnExecuteCommand";
		case cPluginManager::HOOK_HANDSHAKE:             return "OnHandshake";
		case cPluginManager::HOOK_KILLING:               return "OnKilling";
		case cPluginManager::HOOK_LOGIN:                 return "OnLogin";
		case cPluginManager::HOOK_PLAYER_BREAKING_BLOCK: return "OnPlayerBreakingBlock";
		case cPluginManager::HOOK_PLAYER_BROKEN_BLOCK:   return "OnPlayerBrokenBlock";
		case cPluginManager::HOOK_PLAYER_EATING:         return "OnPlayerEating";
		case cPluginManager::HOOK_PLAYER_JOINED:         return "OnPlayerJoined";
		case cPluginManager::HOOK_PLAYER_LEFT_CLICK:     return "OnPlayerLeftClick";
		case cPluginManager::HOOK_PLAYER_MOVING:         return "OnPlayerMoving";
		case cPluginManager::HOOK_PLAYER_PLACED_BLOCK:   return "OnPlayerPlacedBlock";
		case cPluginManager::HOOK_PLAYER_PLACING_BLOCK:  return "OnPlayerPlacingBlock";
		case cPluginManager::HOOK_PLAYER_RIGHT_CLICK:    return "OnPlayerRightClick";
		case cPluginManager::HOOK_PLAYER_SHOOTING:       return "OnPlayerShooting";
		case cPluginManager::HOOK_PLAYER_SPAWNED:        return "OnPlayerSpawned";
		case cPluginManager::HOOK_PLAYER_TOSSING_ITEM:   return "OnPlayerTossingItem";
		case cPluginManager::HOOK_PLAYER_USED_BLOCK:     return "OnPlayerUsedBlock";
		case cPluginManager::HOOK_PLAYER_USED_ITEM:      return "OnPlayerUsedItem";
		case cPluginManager::HOOK_PLAYER_USING_BLOCK:    return "OnPlayerUsingBlock";
		case cPluginManager::HOOK_PLAYER_USING_ITEM:     return "OnPlayerUsingItem";
		case cPluginManager::HOOK_POST_CRAFTING:         return "OnPostCrafting";
		case cPluginManager::HOOK_PRE_CRAFTING:          return "OnPreCrafting";
		case cPluginManager::HOOK_TAKE_DAMAGE:           return "OnTakeDamage";
		case cPluginManager::HOOK_TICK:                  return "OnTick";
		case cPluginManager::HOOK_UPDATED_SIGN:          return "OnUpdatedSign";
		case cPluginManager::HOOK_UPDATING_SIGN:         return "OnUpdatingSign";
		case cPluginManager::HOOK_WEATHER_CHANGED:       return "OnWeatherChanged";
		case cPluginManager::HOOK_WEATHER_CHANGING:      return "OnWeatherChanging";
		default: return NULL;
	}  // switch (a_Hook)
}





AString cPlugin_NewLua::HandleWebRequest( HTTPRequest * a_Request )
{
	cCSLock Lock(m_CriticalSection);
	std::string RetVal = "";

	std::pair< std::string, std::string > TabName = GetTabNameForRequest(a_Request);
	std::string SafeTabName = TabName.second;
	if( SafeTabName.empty() )
		return "";

	sWebPluginTab* Tab = 0;
	for( TabList::iterator itr = GetTabs().begin(); itr != GetTabs().end(); ++itr )
	{
		if( (*itr)->SafeTitle.compare( SafeTabName ) == 0 ) // This is the one! Rawr
		{
			Tab = *itr;
			break;
		}
	}

	if( Tab )
	{
		//LOGINFO("1. Stack size: %i", lua_gettop(m_LuaState) );
		lua_rawgeti( m_LuaState, LUA_REGISTRYINDEX, Tab->UserData); // same as lua_getref()

		//LOGINFO("2. Stack size: %i", lua_gettop(m_LuaState) );
		// Push HTTPRequest
		tolua_pushusertype( m_LuaState, a_Request, "HTTPRequest" );
		//LOGINFO("Calling bound function! :D");
		int s = lua_pcall( m_LuaState, 1, 1, 0);

		if ( s != 0 )
		{
			std::string err = lua_tostring(m_LuaState, -1);
			LOGERROR("-- %s", err.c_str() );
			lua_pop(m_LuaState, 1);
			LOGINFO("error. Stack size: %i", lua_gettop(m_LuaState) );
			return err;	// Show the error message in the web page, looks cool
		}

		if( !lua_isstring( m_LuaState, -1 ) )
		{
			LOGWARN("WARNING: WebPlugin tab '%s' did not return a string!", Tab->Title.c_str() );
			lua_pop(m_LuaState, 1); // Pop return value
			return std::string("WARNING: WebPlugin tab '") + Tab->Title + std::string("' did not return a string!");
		}

		RetVal += tolua_tostring(m_LuaState, -1, 0);
		lua_pop(m_LuaState, 1); // Pop return value
		//LOGINFO("ok. Stack size: %i", lua_gettop(m_LuaState) );
	}

	return RetVal;
}





bool cPlugin_NewLua::AddWebTab( const AString & a_Title, lua_State * a_LuaState, int a_FunctionReference )
{
	cCSLock Lock(m_CriticalSection);
	if (a_LuaState != m_LuaState)
	{
		LOGERROR("Only allowed to add a tab to a WebPlugin of your own Plugin!");
		return false;
	}
	sWebPluginTab * Tab = new sWebPluginTab();
	Tab->Title = a_Title;
	Tab->SafeTitle = SafeString(a_Title);

	Tab->UserData = a_FunctionReference;

	GetTabs().push_back( Tab );
	return true;
}





void cPlugin_NewLua::BindCommand(const AString & a_Command, int a_FnRef)
{
	ASSERT(m_Commands.find(a_Command) == m_Commands.end());
	m_Commands[a_Command] = a_FnRef;
}





void cPlugin_NewLua::BindConsoleCommand(const AString & a_Command, int a_FnRef)
{
	ASSERT(m_ConsoleCommands.find(a_Command) == m_ConsoleCommands.end());
	m_ConsoleCommands[a_Command] = a_FnRef;
}





// Helper functions
bool cPlugin_NewLua::PushFunction(const char * a_FunctionName, bool a_bLogError /* = true */)
{
	if (m_LuaState == NULL)
	{
		// This happens if Initialize() fails with an error
		return false;
	}
	
	lua_getglobal(m_LuaState, a_FunctionName);
	if (!lua_isfunction(m_LuaState, -1))
	{
		if (a_bLogError)
		{
			LOGWARN("Error in plugin %s: Could not find function %s()", GetName().c_str(), a_FunctionName);
		}
		lua_pop(m_LuaState, 1);
		return false;
	}
	return true;
}





bool cPlugin_NewLua::CallFunction( int a_NumArgs, int a_NumResults, const char * a_FunctionName)
{
	ASSERT(lua_isfunction(m_LuaState, -a_NumArgs - 1));
	
	int s = lua_pcall(m_LuaState, a_NumArgs, a_NumResults, 0);
	if (report_errors(m_LuaState, s))
	{
		LOGWARN("Error in plugin %s calling function %s()", GetName().c_str(), a_FunctionName);
		return false;
	}
	return true;
}




