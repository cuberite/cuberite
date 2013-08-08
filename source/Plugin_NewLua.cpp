
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#define LUA_USE_POSIX
#include "Plugin_NewLua.h"
#include "CommandOutput.h"

extern "C"
{
	#include "lualib.h"
}

#include "tolua++.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPlugin_NewLua:

cPlugin_NewLua::cPlugin_NewLua(const AString & a_PluginDirectory) :
	cPlugin(a_PluginDirectory),
	m_LuaState(Printf("plugin %s", a_PluginDirectory.c_str()))
{
}





cPlugin_NewLua::~cPlugin_NewLua()
{
	cCSLock Lock(m_CriticalSection);
	m_LuaState.Close();
}





bool cPlugin_NewLua::Initialize(void)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{	
		m_LuaState.Create();
		
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
		if (!m_LuaState.LoadFile(Path))
		{
			m_LuaState.Close();
			return false;
		}
	}  // for itr - Files[]

	// Call intialize function
	if (!m_LuaState.PushFunction("Initialize"))
	{
		m_LuaState.Close();
		return false;
	}

	m_LuaState.PushUserType(this, "cPlugin_NewLua");
	
	if (!m_LuaState.CallFunction(1))
	{
		m_LuaState.Close();
		return false;
	}

	if (!lua_isboolean(m_LuaState, -1))
	{
		LOGWARNING("Error in plugin %s: Initialize() must return a boolean value!", GetName().c_str());
		m_LuaState.Close();
		return false;
	}

	bool bSuccess = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	return bSuccess;
}





void cPlugin_NewLua::OnDisable()
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.PushFunction("OnDisable", false)) // false = don't log error if not found
	{
		return;
	}

	m_LuaState.CallFunction(0);
}





void cPlugin_NewLua::Tick(float a_Dt)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_TICK);
	if (!m_LuaState.PushFunction(FnName))
	{
		return;
	}
	m_LuaState.PushNumber(a_Dt);
	m_LuaState.CallFunction(0);
}





bool cPlugin_NewLua::OnBlockToPickups(cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_BLOCK_TO_PICKUPS);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_World);
	m_LuaState.PushObject(a_Digger);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockType);
	m_LuaState.PushNumber(a_BlockMeta);
	m_LuaState.PushObject(&a_Pickups);

	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnChat(cPlayer * a_Player, AString & a_Message)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHAT);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_Player);
	m_LuaState.PushString(a_Message.c_str());

	if (!m_LuaState.CallFunction(2))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -2, 0) > 0);
	if (lua_isstring(m_LuaState, -1))
	{
		a_Message = tolua_tostring(m_LuaState, -1, "");
	}
	lua_pop(m_LuaState, 2);
	return bRetVal;
}





bool cPlugin_NewLua::OnChunkAvailable(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHUNK_AVAILABLE);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}
	
	m_LuaState.PushObject(a_World);
	m_LuaState.PushNumber(a_ChunkX);
	m_LuaState.PushNumber(a_ChunkZ);
	
	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}
	
	m_LuaState.PushObject(a_World);
	m_LuaState.PushNumber(a_ChunkX);
	m_LuaState.PushNumber(a_ChunkZ);
	m_LuaState.PushUserType(a_ChunkDesc, "cChunkDesc");
	
	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_CHUNK_GENERATING);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_World);
	m_LuaState.PushNumber(a_ChunkX);
	m_LuaState.PushNumber(a_ChunkZ);
	m_LuaState.PushUserType(a_ChunkDesc, "cChunkDesc");

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}
	
	m_LuaState.PushObject(a_World);
	m_LuaState.PushNumber(a_ChunkX);
	m_LuaState.PushNumber(a_ChunkZ);
	
	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}
	
	m_LuaState.PushObject(a_World);
	m_LuaState.PushNumber(a_ChunkX);
	m_LuaState.PushNumber(a_ChunkZ);
	
	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_Player);
	m_LuaState.PushObject(a_Pickup);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushUserType((void *)a_Player, "cPlayer");
	m_LuaState.PushUserType((void *)a_Grid,   "cCraftingGrid");
	m_LuaState.PushUserType((void *)a_Recipe, "cCraftingRecipe");

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_Player);
	m_LuaState.PushString(a_Reason.c_str());

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_Player);
	m_LuaState.PushStringVector(a_Split);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushUserType(a_Client, "cClientHandle");
	m_LuaState.PushString  (a_Username.c_str());

	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnKilling(cEntity & a_Victim, cEntity * a_Killer)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_KILLING);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Victim);
	m_LuaState.PushObject(a_Killer);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_Client);
	m_LuaState.PushNumber(a_ProtocolVersion);
	m_LuaState.PushString(a_Username.c_str());

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_BlockType);
	m_LuaState.PushNumber(a_BlockMeta);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_BlockType);
	m_LuaState.PushNumber(a_BlockMeta);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_Status);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_CursorX);
	m_LuaState.PushNumber(a_CursorY);
	m_LuaState.PushNumber(a_CursorZ);
	m_LuaState.PushNumber(a_BlockType);
	m_LuaState.PushNumber(a_BlockMeta);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_CursorX);
	m_LuaState.PushNumber(a_CursorY);
	m_LuaState.PushNumber(a_CursorZ);
	m_LuaState.PushNumber(a_BlockType);
	m_LuaState.PushNumber(a_BlockMeta);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_CursorX);
	m_LuaState.PushNumber(a_CursorY);
	m_LuaState.PushNumber(a_CursorZ);

	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_PLAYER_RIGHT_CLICKING_ENTITY);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushObject(&a_Entity);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_CursorX);
	m_LuaState.PushNumber(a_CursorY);
	m_LuaState.PushNumber(a_CursorZ);
	m_LuaState.PushNumber(a_BlockType);
	m_LuaState.PushNumber(a_BlockMeta);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_CursorX);
	m_LuaState.PushNumber(a_CursorY);
	m_LuaState.PushNumber(a_CursorZ);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_CursorX);
	m_LuaState.PushNumber(a_CursorY);
	m_LuaState.PushNumber(a_CursorZ);
	m_LuaState.PushNumber(a_BlockType);
	m_LuaState.PushNumber(a_BlockMeta);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushNumber(a_BlockFace);
	m_LuaState.PushNumber(a_CursorX);
	m_LuaState.PushNumber(a_CursorY);
	m_LuaState.PushNumber(a_CursorZ);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushUserType((void *)a_Player, "cPlayer");
	m_LuaState.PushUserType((void *)a_Grid,   "cCraftingGrid");
	m_LuaState.PushUserType((void *)a_Recipe, "cCraftingRecipe");

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushUserType((void *)a_Player, "cPlayer");
	m_LuaState.PushUserType((void *)a_Grid,   "cCraftingGrid");
	m_LuaState.PushUserType((void *)a_Recipe, "cCraftingRecipe");

	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnSpawnedEntity(cWorld & a_World, cEntity & a_Entity)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_SPAWNED_ENTITY);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_World);
	m_LuaState.PushObject(&a_Entity);

	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





bool cPlugin_NewLua::OnSpawnedMonster(cWorld & a_World, cMonster & a_Monster)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_SPAWNED_MONSTER);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_World);
	m_LuaState.PushObject(&a_Monster);

	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
	return false;
}





bool cPlugin_NewLua::OnSpawningEntity(cWorld & a_World, cEntity & a_Entity)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_SPAWNING_ENTITY);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_World);
	m_LuaState.PushObject(&a_Entity);

	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
	return false;
}





bool cPlugin_NewLua::OnSpawningMonster(cWorld & a_World, cMonster & a_Monster)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_SPAWNING_MONSTER);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_World);
	m_LuaState.PushObject(&a_Monster);

	if (!m_LuaState.CallFunction(1))
	{
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
	return false;
}





bool cPlugin_NewLua::OnTakeDamage(cEntity & a_Receiver, TakeDamageInfo & a_TDI)
{
	cCSLock Lock(m_CriticalSection);
	const char * FnName = GetHookFnName(cPluginManager::HOOK_TAKE_DAMAGE);
	ASSERT(FnName != NULL);
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_Receiver);
	m_LuaState.PushUserType(&a_TDI, "TakeDamageInfo");

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_World);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushString(a_Line1.c_str());
	m_LuaState.PushString(a_Line2.c_str());
	m_LuaState.PushString(a_Line3.c_str());
	m_LuaState.PushString(a_Line4.c_str());
	m_LuaState.PushObject(a_Player);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(a_World);
	m_LuaState.PushNumber(a_BlockX);
	m_LuaState.PushNumber(a_BlockY);
	m_LuaState.PushNumber(a_BlockZ);
	m_LuaState.PushString(a_Line1.c_str());
	m_LuaState.PushString(a_Line2.c_str());
	m_LuaState.PushString(a_Line3.c_str());
	m_LuaState.PushString(a_Line4.c_str());
	m_LuaState.PushObject(a_Player);

	if (!m_LuaState.CallFunction(5))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_World);

	if (!m_LuaState.CallFunction(1))
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
	if (!m_LuaState.PushFunction(FnName))
	{
		return false;
	}

	m_LuaState.PushObject(&a_World);
	m_LuaState.PushNumber(a_NewWeather);

	if (!m_LuaState.CallFunction(2))
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
	if (!m_LuaState.PushFunctionFromRegistry(cmd->second))
	{
		LOGWARNING("Command handler function for \"%s\" is invalid", cmd->first.c_str());
		return false;
	}
	
	m_LuaState.PushStringVector(a_Split);
	m_LuaState.PushObject(a_Player);
	
	// Call function:
	if (!m_LuaState.CallFunction(1))
	{
		LOGWARNING("LUA error in %s. Stack size: %i", __FUNCTION__, lua_gettop(m_LuaState));
		return false;
	}
	
	// Handle return value:
	bool RetVal = (tolua_toboolean(m_LuaState, -1, 0) > 0);
	lua_pop(m_LuaState, 1);  // Pop return value
	
	return RetVal;
}





bool cPlugin_NewLua::HandleConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output)
{
	ASSERT(!a_Split.empty());
	CommandMap::iterator cmd = m_ConsoleCommands.find(a_Split[0]);
	if (cmd == m_ConsoleCommands.end())
	{
		LOGWARNING("Console command handler is registered in cPluginManager but not in cPlugin, wtf? Console command \"%s\", plugin \"%s\".",
			a_Split[0].c_str(), GetName().c_str()
		);
		return false;
	}
	
	cCSLock Lock(m_CriticalSection);
	
	// Push the function to be called:
	m_LuaState.PushFunctionFromRegistry(cmd->second);
	
	m_LuaState.PushStringVector(a_Split);
	
	// Call function:
	if (!m_LuaState.CallFunction(2))
	{
		LOGWARNING("Lua error in %s. Stack size: %i", __FUNCTION__, lua_gettop(m_LuaState));
		return false;
	}
	
	// Handle return values:
	if (lua_isstring(m_LuaState, -1))
	{
		AString str = tolua_tocppstring(m_LuaState, -1, "");
		a_Output.Out(str);
	}
	bool RetVal = (tolua_toboolean(m_LuaState, -2, 0) > 0);
	lua_pop(m_LuaState, 2);  // Pop return values
	
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
		case cPluginManager::HOOK_BLOCK_TO_PICKUPS:             return "OnBlockToPickups";
		case cPluginManager::HOOK_CHAT:                         return "OnChat";
		case cPluginManager::HOOK_CHUNK_AVAILABLE:              return "OnChunkAvailable";
		case cPluginManager::HOOK_CHUNK_GENERATED:              return "OnChunkGenerated";
		case cPluginManager::HOOK_CHUNK_GENERATING:             return "OnChunkGenerating";
		case cPluginManager::HOOK_CHUNK_UNLOADED:               return "OnChunkUnloaded";
		case cPluginManager::HOOK_CHUNK_UNLOADING:              return "OnChunkUnloading";
		case cPluginManager::HOOK_COLLECTING_PICKUP:            return "OnCollectingPickup";
		case cPluginManager::HOOK_CRAFTING_NO_RECIPE:           return "OnCraftingNoRecipe";
		case cPluginManager::HOOK_DISCONNECT:                   return "OnDisconnect";
		case cPluginManager::HOOK_EXECUTE_COMMAND:              return "OnExecuteCommand";
		case cPluginManager::HOOK_HANDSHAKE:                    return "OnHandshake";
		case cPluginManager::HOOK_KILLING:                      return "OnKilling";
		case cPluginManager::HOOK_LOGIN:                        return "OnLogin";
		case cPluginManager::HOOK_PLAYER_BREAKING_BLOCK:        return "OnPlayerBreakingBlock";
		case cPluginManager::HOOK_PLAYER_BROKEN_BLOCK:          return "OnPlayerBrokenBlock";
		case cPluginManager::HOOK_PLAYER_EATING:                return "OnPlayerEating";
		case cPluginManager::HOOK_PLAYER_JOINED:                return "OnPlayerJoined";
		case cPluginManager::HOOK_PLAYER_LEFT_CLICK:            return "OnPlayerLeftClick";
		case cPluginManager::HOOK_PLAYER_MOVING:                return "OnPlayerMoving";
		case cPluginManager::HOOK_PLAYER_PLACED_BLOCK:          return "OnPlayerPlacedBlock";
		case cPluginManager::HOOK_PLAYER_PLACING_BLOCK:         return "OnPlayerPlacingBlock";
		case cPluginManager::HOOK_PLAYER_RIGHT_CLICK:           return "OnPlayerRightClick";
		case cPluginManager::HOOK_PLAYER_RIGHT_CLICKING_ENTITY: return "OnPlayerRightClickingEntity";
		case cPluginManager::HOOK_PLAYER_SHOOTING:              return "OnPlayerShooting";
		case cPluginManager::HOOK_PLAYER_SPAWNED:               return "OnPlayerSpawned";
		case cPluginManager::HOOK_PLAYER_TOSSING_ITEM:          return "OnPlayerTossingItem";
		case cPluginManager::HOOK_PLAYER_USED_BLOCK:            return "OnPlayerUsedBlock";
		case cPluginManager::HOOK_PLAYER_USED_ITEM:             return "OnPlayerUsedItem";
		case cPluginManager::HOOK_PLAYER_USING_BLOCK:           return "OnPlayerUsingBlock";
		case cPluginManager::HOOK_PLAYER_USING_ITEM:            return "OnPlayerUsingItem";
		case cPluginManager::HOOK_POST_CRAFTING:                return "OnPostCrafting";
		case cPluginManager::HOOK_PRE_CRAFTING:                 return "OnPreCrafting";
		case cPluginManager::HOOK_SPAWNED_ENTITY:               return "OnSpawnedEntity";
		case cPluginManager::HOOK_SPAWNED_MONSTER:              return "OnSpawnedMonster";
		case cPluginManager::HOOK_SPAWNING_ENTITY:              return "OnSpawningEntity";
		case cPluginManager::HOOK_SPAWNING_MONSTER:             return "OnSpawningMonster";
		case cPluginManager::HOOK_TAKE_DAMAGE:                  return "OnTakeDamage";
		case cPluginManager::HOOK_TICK:                         return "OnTick";
		case cPluginManager::HOOK_UPDATED_SIGN:                 return "OnUpdatedSign";
		case cPluginManager::HOOK_UPDATING_SIGN:                return "OnUpdatingSign";
		case cPluginManager::HOOK_WEATHER_CHANGED:              return "OnWeatherChanged";
		case cPluginManager::HOOK_WEATHER_CHANGING:             return "OnWeatherChanging";
		default: return NULL;
	}  // switch (a_Hook)
}





AString cPlugin_NewLua::HandleWebRequest(const HTTPRequest * a_Request )
{
	cCSLock Lock(m_CriticalSection);
	std::string RetVal = "";

	std::pair< std::string, std::string > TabName = GetTabNameForRequest(a_Request);
	std::string SafeTabName = TabName.second;
	if( SafeTabName.empty() )
		return "";

	sWebPluginTab* Tab = 0;
	for (TabList::iterator itr = GetTabs().begin(); itr != GetTabs().end(); ++itr)
	{
		if ((*itr)->SafeTitle.compare(SafeTabName) == 0) // This is the one! Rawr
		{
			Tab = *itr;
			break;
		}
	}

	if( Tab )
	{
		m_LuaState.PushFunctionFromRegistry(Tab->UserData);

		// Push HTTPRequest
		m_LuaState.PushUserType((void*)a_Request, "const HTTPRequest");
		
		if (!m_LuaState.CallFunction(1))
		{
			return "Lua encountered error while processing the page request";
		}

		if (!lua_isstring(m_LuaState, -1))
		{
			LOGWARNING("WebPlugin tab '%s' did not return a string!", Tab->Title.c_str());
			lua_pop(m_LuaState, 1);  // Pop return value
			return Printf("WARNING: WebPlugin tab '%s' did not return a string!", Tab->Title.c_str());
		}

		RetVal += tolua_tostring(m_LuaState, -1, 0);
		lua_pop(m_LuaState, 1); // Pop return value
		// LOGINFO("ok. Stack size: %i", lua_gettop(m_LuaState) );
	}

	return RetVal;
}





bool cPlugin_NewLua::AddWebTab(const AString & a_Title, lua_State * a_LuaState, int a_FunctionReference)
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

	GetTabs().push_back(Tab);
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





void cPlugin_NewLua::Unreference(int a_LuaRef)
{
	cCSLock Lock(m_CriticalSection);
	luaL_unref(m_LuaState, LUA_REGISTRYINDEX, a_LuaRef);
}





bool cPlugin_NewLua::CallbackWindowClosing(int a_FnRef, cWindow & a_Window, cPlayer & a_Player, bool a_CanRefuse)
{
	ASSERT(a_FnRef != LUA_REFNIL);
	
	cCSLock Lock(m_CriticalSection);
	m_LuaState.PushFunctionFromRegistry(a_FnRef);
	m_LuaState.PushUserType(&a_Window, "cWindow");
	m_LuaState.PushObject(&a_Player);
	m_LuaState.PushBool(a_CanRefuse);

	// Call function:
	if (!m_LuaState.CallFunction(1))
	{
		LOGWARNING("LUA error in %s. Stack size: %i", __FUNCTION__, lua_gettop(m_LuaState));
		return false;
	}

	bool bRetVal = (tolua_toboolean(m_LuaState, -1, false) > 0);
	lua_pop(m_LuaState, 1);
	return bRetVal;
}





void cPlugin_NewLua::CallbackWindowSlotChanged(int a_FnRef, cWindow & a_Window, int a_SlotNum)
{
	ASSERT(a_FnRef != LUA_REFNIL);
	
	cCSLock Lock(m_CriticalSection);
	m_LuaState.PushFunctionFromRegistry(a_FnRef);
	m_LuaState.PushUserType(&a_Window, "cWindow");
	m_LuaState.PushNumber(a_SlotNum);

	// Call function:
	if (!m_LuaState.CallFunction(0))
	{
		LOGWARNING("LUA error in %s. Stack size: %i", __FUNCTION__, lua_gettop(m_LuaState));
	}
}




