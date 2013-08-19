
// PluginLua.cpp

// Implements the cPluginLua class representing a plugin written in Lua

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#define LUA_USE_POSIX
#include "PluginLua.h"
#include "CommandOutput.h"

extern "C"
{
	#include "lualib.h"
}

#include "tolua++.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cPluginLua:

cPluginLua::cPluginLua(const AString & a_PluginDirectory) :
	cPlugin(a_PluginDirectory),
	m_LuaState(Printf("plugin %s", a_PluginDirectory.c_str()))
{
}





cPluginLua::~cPluginLua()
{
	cCSLock Lock(m_CriticalSection);
	m_LuaState.Close();
}





bool cPluginLua::Initialize(void)
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
	bool res = false;
	if (!m_LuaState.Call("Initialize", this, cLuaState::Return, res))
	{
		LOGWARNING("Error in plugin %s: Cannot call the Initialize() function. Plugin is temporarily disabled.", GetName().c_str());
		m_LuaState.Close();
		return false;
	}

	if (!res)
	{
		LOGINFO("Plugin %s: Initialize() call failed, plugin is temporarily disabled.", GetName().c_str());
		m_LuaState.Close();
		return false;
	}

	return true;
}





void cPluginLua::OnDisable(void)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.HasFunction("OnDisable"))
	{
		return;
	}
	m_LuaState.Call("OnDisable");
}





void cPluginLua::Tick(float a_Dt)
{
	cCSLock Lock(m_CriticalSection);
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_TICK), a_Dt);
}





bool cPluginLua::OnBlockToPickups(cWorld * a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_BLOCK_TO_PICKUPS), a_World, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, &a_Pickups, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnChat(cPlayer * a_Player, AString & a_Message)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_CHAT), a_Player, a_Message, cLuaState::Return, res, a_Message);
	return res;
}





bool cPluginLua::OnChunkAvailable(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_CHUNK_AVAILABLE), a_World, a_ChunkX, a_ChunkZ, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnChunkGenerated(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_CHUNK_GENERATED), a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnChunkGenerating(cWorld * a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_CHUNK_GENERATING), a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnChunkUnloaded(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_CHUNK_UNLOADED), a_World, a_ChunkX, a_ChunkZ, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnChunkUnloading(cWorld * a_World, int a_ChunkX, int a_ChunkZ)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_CHUNK_UNLOADING), a_World, a_ChunkX, a_ChunkZ, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnCollectingPickup(cPlayer * a_Player, cPickup * a_Pickup)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_COLLECTING_PICKUP), a_Player, a_Pickup, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnCraftingNoRecipe(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_CRAFTING_NO_RECIPE), (cPlayer *)a_Player, a_Grid, a_Recipe, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnDisconnect(cPlayer * a_Player, const AString & a_Reason)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_DISCONNECT), a_Player, a_Reason, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnExecuteCommand(cPlayer * a_Player, const AStringVector & a_Split)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_EXECUTE_COMMAND), a_Player, a_Split, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnExploded(cWorld & a_World, double a_ExplosionSize, bool a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	const char * FnName = GetHookFnName(cPluginManager::HOOK_EXPLODED);
	switch (a_Source)
	{
		case esOther:            m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData,               cLuaState::Return, res); break;
		case esPrimedTNT:        m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, (cTNTEntity *)a_SourceData, cLuaState::Return, res); break;
		case esCreeper:          m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, (cCreeper *)a_SourceData,   cLuaState::Return, res); break;
		case esBed:              m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, (Vector3i *)a_SourceData,   cLuaState::Return, res); break;
		case esEnderCrystal:     m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, (Vector3i *)a_SourceData,   cLuaState::Return, res); break;
		case esGhastFireball:    m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData,               cLuaState::Return, res); break;
		case esWitherSkullBlack:
		case esWitherSkullBlue:  m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData, cLuaState::Return, res); break;
		case esWitherBirth:      m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData, cLuaState::Return, res); break;
		case esPlugin:           m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData, cLuaState::Return, res); break;
		default:
		{
			ASSERT(!"Unhandled ExplosionSource");
			return false;
		}
	}
	return res;
}





bool cPluginLua::OnExploding(cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	const char * FnName = GetHookFnName(cPluginManager::HOOK_EXPLODING);
	switch (a_Source)
	{
		case esOther:            m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData,               cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		case esPrimedTNT:        m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, (cTNTEntity *)a_SourceData, cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		case esCreeper:          m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, (cCreeper *)a_SourceData,   cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		case esBed:              m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, (Vector3i *)a_SourceData,   cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		case esEnderCrystal:     m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, (Vector3i *)a_SourceData,   cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		case esGhastFireball:    m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData,               cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		case esWitherSkullBlack:
		case esWitherSkullBlue:  m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData,               cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		case esWitherBirth:      m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData,               cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		case esPlugin:           m_LuaState.Call(FnName, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData,               cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
		default:
		{
			ASSERT(!"Unhandled ExplosionSource");
			return false;
		}
	}
	return res;
}





bool cPluginLua::OnHandshake(cClientHandle * a_Client, const AString & a_Username)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_HANDSHAKE), a_Client, a_Username, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnHopperPullingItem(cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_HOPPER_PULLING_ITEM), &a_World, &a_Hopper, a_DstSlotNum, &a_SrcEntity, a_SrcSlotNum, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnHopperPushingItem(cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_HOPPER_PUSHING_ITEM), &a_World, &a_Hopper, a_SrcSlotNum, &a_DstEntity, a_DstSlotNum, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnKilling(cEntity & a_Victim, cEntity * a_Killer)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_KILLING), &a_Victim, a_Killer, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnLogin(cClientHandle * a_Client, int a_ProtocolVersion, const AString & a_Username)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_LOGIN), a_Client, a_ProtocolVersion, a_Username, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerAnimation(cPlayer & a_Player, int a_Animation)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_ANIMATION), &a_Player, a_Animation, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_BREAKING_BLOCK), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerBrokenBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_BROKEN_BLOCK), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerEating(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_EATING), &a_Player, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerJoined(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_JOINED), &a_Player, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerLeftClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_LEFT_CLICK), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerMoved(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_MOVING), &a_Player, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerPlacedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_PLACED_BLOCK), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerPlacingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_PLACING_BLOCK), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerRightClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_RIGHT_CLICK), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_RIGHT_CLICKING_ENTITY), &a_Player, &a_Entity, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerShooting(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_SHOOTING), &a_Player, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerSpawned(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_SPAWNED), &a_Player, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerTossingItem(cPlayer & a_Player)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_TOSSING_ITEM), &a_Player, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerUsedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_USED_BLOCK), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerUsedItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ) 
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_USED_ITEM), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerUsingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_USING_BLOCK), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPlayerUsingItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PLAYER_USING_ITEM), &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPostCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_POST_CRAFTING), a_Player, a_Grid, a_Recipe, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnPreCrafting(const cPlayer * a_Player, const cCraftingGrid * a_Grid, cCraftingRecipe * a_Recipe)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_PRE_CRAFTING), a_Player, a_Grid, a_Recipe, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnSpawnedEntity(cWorld & a_World, cEntity & a_Entity)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_SPAWNED_ENTITY), &a_World, &a_Entity, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnSpawnedMonster(cWorld & a_World, cMonster & a_Monster)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_SPAWNED_MONSTER), &a_World, &a_Monster, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnSpawningEntity(cWorld & a_World, cEntity & a_Entity)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_SPAWNING_ENTITY), &a_World, &a_Entity, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnSpawningMonster(cWorld & a_World, cMonster & a_Monster)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_SPAWNING_MONSTER), &a_World, &a_Monster, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnTakeDamage(cEntity & a_Receiver, TakeDamageInfo & a_TDI)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_TAKE_DAMAGE), &a_Receiver, &a_TDI, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnUpdatedSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4,
	cPlayer * a_Player
)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_UPDATED_SIGN), a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnUpdatingSign(
	cWorld * a_World, 
	int a_BlockX, int a_BlockY, int a_BlockZ, 
	AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4,
	cPlayer * a_Player
)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_UPDATING_SIGN), a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player, cLuaState::Return, res, a_Line1, a_Line2, a_Line3, a_Line4);
	return res;
}





bool cPluginLua::OnWeatherChanged(cWorld & a_World)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_WEATHER_CHANGED), &a_World, cLuaState::Return, res);
	return res;
}





bool cPluginLua::OnWeatherChanging(cWorld & a_World, eWeather & a_NewWeather)
{
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	int NewWeather = a_NewWeather;
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_WEATHER_CHANGING), &a_World, a_NewWeather, cLuaState::Return, res, NewWeather);
	a_NewWeather = (eWeather)NewWeather;
	return res;
}





bool cPluginLua::OnWorldTick(cWorld & a_World, float a_Dt)
{
	cCSLock Lock(m_CriticalSection);
	m_LuaState.Call(GetHookFnName(cPluginManager::HOOK_WORLD_TICK), &a_World, a_Dt);
	return false;
}





bool cPluginLua::HandleCommand(const AStringVector & a_Split, cPlayer * a_Player)
{
	ASSERT(!a_Split.empty());
	CommandMap::iterator cmd = m_Commands.find(a_Split[0]);
	if (cmd == m_Commands.end())
	{
		LOGWARNING("Command handler is registered in cPluginManager but not in cPlugin, wtf? Command \"%s\".", a_Split[0].c_str());
		return false;
	}
	
	cCSLock Lock(m_CriticalSection);
	bool res = false;
	m_LuaState.Call(cmd->second, a_Split, a_Player, cLuaState::Return, res);
	return res;
}





bool cPluginLua::HandleConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output)
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
	bool res = false;
	AString str;
	m_LuaState.Call(cmd->second, a_Split, cLuaState::Return, res, str);
	if (res && !str.empty())
	{
		a_Output.Out(str);
	}
	return res;
}





void cPluginLua::ClearCommands(void)
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





void cPluginLua::ClearConsoleCommands(void)
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





bool cPluginLua::CanAddHook(cPluginManager::PluginHook a_Hook)
{
	const char * FnName = GetHookFnName(a_Hook);
	if (FnName == NULL)
	{
		// Unknown hook ID
		LOGWARNING("Plugin %s wants to add an unknown hook ID (%d). The plugin need not work properly.", GetName().c_str(), a_Hook);
		return false;
	}
	
	// Check if the function is available
	if (m_LuaState.HasFunction(FnName))
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





const char * cPluginLua::GetHookFnName(cPluginManager::PluginHook a_Hook)
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
		case cPluginManager::HOOK_PLAYER_ANIMATION:             return "OnPlayerAnimation";
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
		case cPluginManager::HOOK_WORLD_TICK:                   return "OnWorldTick";
		default: return NULL;
	}  // switch (a_Hook)
}





AString cPluginLua::HandleWebRequest(const HTTPRequest * a_Request )
{
	cCSLock Lock(m_CriticalSection);
	std::string RetVal = "";

	std::pair< std::string, std::string > TabName = GetTabNameForRequest(a_Request);
	std::string SafeTabName = TabName.second;
	if (SafeTabName.empty())
	{
		return "";
	}

	sWebPluginTab * Tab = 0;
	for (TabList::iterator itr = GetTabs().begin(); itr != GetTabs().end(); ++itr)
	{
		if ((*itr)->SafeTitle.compare(SafeTabName) == 0) // This is the one! Rawr
		{
			Tab = *itr;
			break;
		}
	}

	if (Tab != NULL)
	{
		AString Contents = Printf("WARNING: WebPlugin tab '%s' did not return a string!", Tab->Title.c_str());
		if (!m_LuaState.Call(Tab->UserData, a_Request, cLuaState::Return, Contents))
		{
			return "Lua encountered error while processing the page request";
		}

		RetVal += Contents;
	}

	return RetVal;
}





bool cPluginLua::AddWebTab(const AString & a_Title, lua_State * a_LuaState, int a_FunctionReference)
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





void cPluginLua::BindCommand(const AString & a_Command, int a_FnRef)
{
	ASSERT(m_Commands.find(a_Command) == m_Commands.end());
	m_Commands[a_Command] = a_FnRef;
}





void cPluginLua::BindConsoleCommand(const AString & a_Command, int a_FnRef)
{
	ASSERT(m_ConsoleCommands.find(a_Command) == m_ConsoleCommands.end());
	m_ConsoleCommands[a_Command] = a_FnRef;
}





void cPluginLua::Unreference(int a_LuaRef)
{
	cCSLock Lock(m_CriticalSection);
	luaL_unref(m_LuaState, LUA_REGISTRYINDEX, a_LuaRef);
}





bool cPluginLua::CallbackWindowClosing(int a_FnRef, cWindow & a_Window, cPlayer & a_Player, bool a_CanRefuse)
{
	ASSERT(a_FnRef != LUA_REFNIL);
	
	cCSLock Lock(m_CriticalSection);
	bool res;
	m_LuaState.Call(a_FnRef, &a_Window, &a_Player, a_CanRefuse, cLuaState::Return, res);
	return res;
}





void cPluginLua::CallbackWindowSlotChanged(int a_FnRef, cWindow & a_Window, int a_SlotNum)
{
	ASSERT(a_FnRef != LUA_REFNIL);
	
	cCSLock Lock(m_CriticalSection);
	m_LuaState.Call(a_FnRef, &a_Window, a_SlotNum);
}




