
// PluginLua.cpp

// Implements the cPluginLua class representing a plugin written in Lua

#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#ifdef __APPLE__
	#define LUA_USE_MACOSX
#else
	#define LUA_USE_POSIX
#endif

#include "PluginLua.h"
#include "../CommandOutput.h"
#include "PluginManager.h"
#include "../Item.h"
#include "../Root.h"
#include "../WebAdmin.h"

extern "C"
{
	#include "lua/src/lauxlib.h"
}

#undef TOLUA_TEMPLATE_BIND
#include "tolua++/include/tolua++.h"





////////////////////////////////////////////////////////////////////////////////
// cPluginLua:

cPluginLua::cPluginLua(const AString & a_PluginDirectory) :
	cPlugin(a_PluginDirectory),
	m_LuaState(Printf("plugin %s", a_PluginDirectory.c_str()))
{
}





cPluginLua::~cPluginLua()
{
	cCSLock Lock(m_CriticalSection);
	Close();
}





void cPluginLua::Close(void)
{
	cCSLock Lock(m_CriticalSection);

	// If already closed, bail out:
	if (!m_LuaState.IsValid())
	{
		ASSERT(m_Resettables.empty());
		ASSERT(m_HookMap.empty());
		return;
	}

	// Remove the command bindings and web tabs:
	ClearCommands();
	ClearConsoleCommands();
	ClearWebTabs();

	// Notify and remove all m_Resettables (unlock the m_CriticalSection while resetting them):
	cResettablePtrs resettables;
	std::swap(m_Resettables, resettables);
	{
		cCSUnlock Unlock(Lock);
		for (auto resettable: resettables)
		{
			resettable->Reset();
		}
		m_Resettables.clear();
	}  // cCSUnlock (m_CriticalSection)

	// Release all the references in the hook map:
	m_HookMap.clear();

	// Close the Lua engine:
	m_LuaState.Close();
}





bool cPluginLua::Load(void)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		m_LuaState.Create();
		m_LuaState.RegisterAPILibs();

		// Inject the identification global variables into the state:
		lua_pushlightuserdata(m_LuaState, this);
		lua_setglobal(m_LuaState, LUA_PLUGIN_INSTANCE_VAR_NAME);
		lua_pushstring(m_LuaState, GetName().c_str());
		lua_setglobal(m_LuaState, LUA_PLUGIN_NAME_VAR_NAME);

		// Add the plugin's folder to the package.path and package.cpath variables (#693):
		m_LuaState.AddPackagePath("path", FILE_IO_PREFIX + GetLocalFolder() + "/?.lua");
		#ifdef _WIN32
			m_LuaState.AddPackagePath("cpath", GetLocalFolder() + "\\?.dll");
		#else
			m_LuaState.AddPackagePath("cpath", FILE_IO_PREFIX + GetLocalFolder() + "/?.so");
		#endif

		tolua_pushusertype(m_LuaState, this, "cPluginLua");
		lua_setglobal(m_LuaState, "g_Plugin");
	}

	std::string PluginPath = FILE_IO_PREFIX + GetLocalFolder() + "/";

	// List all Lua files for this plugin. Info.lua has a special handling - make it the last to load:
	AStringVector Files = cFile::GetFolderContents(PluginPath.c_str());
	AStringVector LuaFiles;
	bool HasInfoLua = false;
	for (AStringVector::const_iterator itr = Files.begin(), end = Files.end(); itr != end; ++itr)
	{
		if (itr->rfind(".lua") != AString::npos)
		{
			if (*itr == "Info.lua")
			{
				HasInfoLua = true;
			}
			else
			{
				LuaFiles.push_back(*itr);
			}
		}
	}
	std::sort(LuaFiles.begin(), LuaFiles.end());

	// Warn if there are no Lua files in the plugin folder:
	if (LuaFiles.empty())
	{
		SetLoadError("No lua files found, plugin is probably missing.");
		LOGWARNING("No lua files found: plugin %s is missing.", GetName().c_str());
		Close();
		return false;
	}

	// Load all files in the list, including the Info.lua as last, if it exists:
	for (AStringVector::const_iterator itr = LuaFiles.begin(), end = LuaFiles.end(); itr != end; ++itr)
	{
		AString Path = PluginPath + *itr;
		if (!m_LuaState.LoadFile(Path))
		{
			SetLoadError(Printf("Failed to load file %s.", itr->c_str()));
			Close();
			return false;
		}
	}  // for itr - Files[]
	if (HasInfoLua)
	{
		AString Path = PluginPath + "Info.lua";
		if (!m_LuaState.LoadFile(Path))
		{
			SetLoadError("Failed to load file Info.lua.");
			m_Status = cPluginManager::psError;
			Close();
			return false;
		}
	}

	// Call the Initialize function:
	bool res = false;
	if (!m_LuaState.Call("Initialize", this, cLuaState::Return, res))
	{
		SetLoadError("Cannot call the Initialize() function.");
		LOGWARNING("Error in plugin %s: Cannot call the Initialize() function. Plugin is temporarily disabled.", GetName().c_str());
		Close();
		return false;
	}
	if (!res)
	{
		SetLoadError("The Initialize() function failed.");
		LOGINFO("Plugin %s: Initialize() call failed, plugin is temporarily disabled.", GetName().c_str());
		Close();
		return false;
	}

	m_Status = cPluginManager::psLoaded;
	return true;
}





void cPluginLua::Unload(void)
{
	ClearWebTabs();
	super::Unload();
	Close();
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
	CallSimpleHooks(cPluginManager::HOOK_TICK, a_Dt);
}





bool cPluginLua::OnBlockSpread(cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source)
{
	return CallSimpleHooks(cPluginManager::HOOK_BLOCK_SPREAD, &a_World, a_BlockX, a_BlockY, a_BlockZ, a_Source);
}





bool cPluginLua::OnBlockToPickups(cWorld & a_World, cEntity * a_Digger, int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, cItems & a_Pickups)
{
	return CallSimpleHooks(cPluginManager::HOOK_BLOCK_TO_PICKUPS, &a_World, a_Digger, a_BlockX, a_BlockY, a_BlockZ, a_BlockType, a_BlockMeta, &a_Pickups);
}





bool cPluginLua::OnBrewingCompleted(cWorld & a_World, cBrewingstandEntity & a_Brewingstand)
{
	return CallSimpleHooks(cPluginManager::HOOK_BREWING_COMPLETED, &a_World, &a_Brewingstand);
}





bool cPluginLua::OnBrewingCompleting(cWorld & a_World, cBrewingstandEntity & a_Brewingstand)
{
	return CallSimpleHooks(cPluginManager::HOOK_BREWING_COMPLETING, &a_World, &a_Brewingstand);
}





bool cPluginLua::OnChat(cPlayer & a_Player, AString & a_Message)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_CHAT];
	for (auto hook: hooks)
	{
		hook->Call(&a_Player, a_Message, cLuaState::Return, res, a_Message);
		if (res)
		{
			return true;
		}
	}
	return false;
}





bool cPluginLua::OnChunkAvailable(cWorld & a_World, int a_ChunkX, int a_ChunkZ)
{
	return CallSimpleHooks(cPluginManager::HOOK_CHUNK_AVAILABLE, &a_World, a_ChunkX, a_ChunkZ);
}





bool cPluginLua::OnChunkGenerated(cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	return CallSimpleHooks(cPluginManager::HOOK_CHUNK_GENERATED, &a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc);
}





bool cPluginLua::OnChunkGenerating(cWorld & a_World, int a_ChunkX, int a_ChunkZ, cChunkDesc * a_ChunkDesc)
{
	return CallSimpleHooks(cPluginManager::HOOK_CHUNK_GENERATING, &a_World, a_ChunkX, a_ChunkZ, a_ChunkDesc);
}





bool cPluginLua::OnChunkUnloaded(cWorld & a_World, int a_ChunkX, int a_ChunkZ)
{
	return CallSimpleHooks(cPluginManager::HOOK_CHUNK_UNLOADED, &a_World, a_ChunkX, a_ChunkZ);
}





bool cPluginLua::OnChunkUnloading(cWorld & a_World, int a_ChunkX, int a_ChunkZ)
{
	return CallSimpleHooks(cPluginManager::HOOK_CHUNK_UNLOADING, &a_World, a_ChunkX, a_ChunkZ);
}





bool cPluginLua::OnCollectingPickup(cPlayer & a_Player, cPickup & a_Pickup)
{
	return CallSimpleHooks(cPluginManager::HOOK_COLLECTING_PICKUP, &a_Player, &a_Pickup);
}





bool cPluginLua::OnCraftingNoRecipe(cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe)
{
	return CallSimpleHooks(cPluginManager::HOOK_CRAFTING_NO_RECIPE, &a_Player, &a_Grid, &a_Recipe);
}





bool cPluginLua::OnDisconnect(cClientHandle & a_Client, const AString & a_Reason)
{
	return CallSimpleHooks(cPluginManager::HOOK_DISCONNECT, &a_Client, a_Reason);
}





bool cPluginLua::OnEntityAddEffect(cEntity & a_Entity, int a_EffectType, int a_EffectDurationTicks, int a_EffectIntensity, double a_DistanceModifier)
{
	return CallSimpleHooks(cPluginManager::HOOK_ENTITY_ADD_EFFECT, &a_Entity, a_EffectType, a_EffectDurationTicks, a_EffectIntensity, a_DistanceModifier);
}





bool cPluginLua::OnEntityChangingWorld(cEntity & a_Entity, cWorld & a_World)
{
	return CallSimpleHooks(cPluginManager::HOOK_ENTITY_CHANGING_WORLD, &a_Entity, &a_World);
}





bool cPluginLua::OnEntityChangedWorld(cEntity & a_Entity, cWorld & a_World)
{
	return CallSimpleHooks(cPluginManager::HOOK_ENTITY_CHANGED_WORLD, &a_Entity, &a_World);
}





bool cPluginLua::OnExecuteCommand(cPlayer * a_Player, const AStringVector & a_Split, const AString & a_EntireCommand, cPluginManager::CommandResult & a_Result)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_EXECUTE_COMMAND];
	for (auto & hook: hooks)
	{
		hook->Call(a_Player, a_Split, a_EntireCommand, cLuaState::Return, res, a_Result);
		if (res)
		{
			return true;
		}
	}
	return false;
}





bool cPluginLua::OnExploded(cWorld & a_World, double a_ExplosionSize, bool a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_EXPLODED];
	for (auto & hook: hooks)
	{
		switch (a_Source)
		{
			case esBed:           hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<Vector3i *>            (a_SourceData), cLuaState::Return, res); break;
			case esEnderCrystal:  hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cEntity *>             (a_SourceData), cLuaState::Return, res); break;
			case esGhastFireball: hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cGhastFireballEntity *>(a_SourceData), cLuaState::Return, res); break;
			case esMonster:       hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cMonster *>            (a_SourceData), cLuaState::Return, res); break;
			case esOther:         hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, cLuaState::Return, res); break;
			case esPlugin:        hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, cLuaState::Return, res); break;
			case esPrimedTNT:     hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cTNTEntity *>          (a_SourceData), cLuaState::Return, res); break;
			case esWitherBirth:   hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cMonster *>            (a_SourceData), cLuaState::Return, res); break;
			case esWitherSkull:   hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cWitherSkullEntity *>  (a_SourceData), cLuaState::Return, res); break;
			case esMax:
			{
				ASSERT(!"Invalid explosion source");
				return false;
			}
		}
		if (res)
		{
			return true;
		}
	}
	return false;
}





bool cPluginLua::OnExploding(cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_EXPLODING];
	for (auto & hook: hooks)
	{
		switch (a_Source)
		{
			case esBed:           hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<Vector3i *>            (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esEnderCrystal:  hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cEntity *>             (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esGhastFireball: hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cGhastFireballEntity *>(a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esMonster:       hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cMonster *>            (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esOther:         hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source,                                                         cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esPlugin:        hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source,                                                         cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esPrimedTNT:     hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cTNTEntity *>          (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esWitherBirth:   hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cMonster *>            (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esWitherSkull:   hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, reinterpret_cast<cWitherSkullEntity *>  (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esMax:
			{
				ASSERT(!"Invalid explosion source");
				return false;
			}
		}
		if (res)
		{
			return true;
		}
	}
	return false;
}





bool cPluginLua::OnHandshake(cClientHandle & a_Client, const AString & a_Username)
{
	return CallSimpleHooks(cPluginManager::HOOK_HANDSHAKE, &a_Client, a_Username);
}





bool cPluginLua::OnHopperPullingItem(cWorld & a_World, cHopperEntity & a_Hopper, int a_DstSlotNum, cBlockEntityWithItems & a_SrcEntity, int a_SrcSlotNum)
{
	return CallSimpleHooks(cPluginManager::HOOK_HOPPER_PULLING_ITEM, &a_World, &a_Hopper, a_DstSlotNum, &a_SrcEntity, a_SrcSlotNum);
}





bool cPluginLua::OnHopperPushingItem(cWorld & a_World, cHopperEntity & a_Hopper, int a_SrcSlotNum, cBlockEntityWithItems & a_DstEntity, int a_DstSlotNum)
{
	return CallSimpleHooks(cPluginManager::HOOK_HOPPER_PUSHING_ITEM, &a_World, &a_Hopper, a_SrcSlotNum, &a_DstEntity, a_DstSlotNum);
}





bool cPluginLua::OnKilled(cEntity & a_Victim, TakeDamageInfo & a_TDI, AString & a_DeathMessage)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_KILLED];
	for (auto & hook: hooks)
	{
		hook->Call(&a_Victim, &a_TDI, a_DeathMessage, cLuaState::Return, res, a_DeathMessage);
		if (res)
		{
			return true;
		}
	}
	return false;
}





bool cPluginLua::OnKilling(cEntity & a_Victim, cEntity * a_Killer, TakeDamageInfo & a_TDI)
{
	return CallSimpleHooks(cPluginManager::HOOK_KILLING, &a_Victim, a_Killer, &a_TDI);
}





bool cPluginLua::OnLogin(cClientHandle & a_Client, UInt32 a_ProtocolVersion, const AString & a_Username)
{
	return CallSimpleHooks(cPluginManager::HOOK_LOGIN, &a_Client, a_ProtocolVersion, a_Username);
}





bool cPluginLua::OnPlayerAnimation(cPlayer & a_Player, int a_Animation)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_ANIMATION, &a_Player, a_Animation);
}





bool cPluginLua::OnPlayerBreakingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_BREAKING_BLOCK, &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta);
}





bool cPluginLua::OnPlayerBrokenBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_BROKEN_BLOCK, &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_BlockType, a_BlockMeta);
}





bool cPluginLua::OnPlayerDestroyed(cPlayer & a_Player)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_DESTROYED, &a_Player);
}





bool cPluginLua::OnPlayerEating(cPlayer & a_Player)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_EATING, &a_Player);
}





bool cPluginLua::OnPlayerFoodLevelChange(cPlayer & a_Player, int a_NewFoodLevel)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_FOOD_LEVEL_CHANGE, &a_Player, a_NewFoodLevel);
}





bool cPluginLua::OnPlayerFished(cPlayer & a_Player, const cItems & a_Reward)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_FISHED, &a_Player, a_Reward);
}





bool cPluginLua::OnPlayerFishing(cPlayer & a_Player, cItems & a_Reward)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_FISHING, &a_Player, &a_Reward);
}





bool cPluginLua::OnPlayerJoined(cPlayer & a_Player)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_JOINED, &a_Player);
}





bool cPluginLua::OnPlayerLeftClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, char a_Status)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_LEFT_CLICK, &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_Status);
}





bool cPluginLua::OnPlayerMoving(cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_MOVING, &a_Player, a_OldPosition, a_NewPosition);
}





bool cPluginLua::OnEntityTeleport(cEntity & a_Entity, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition)
{
	return CallSimpleHooks(cPluginManager::HOOK_ENTITY_TELEPORT, &a_Entity, a_OldPosition, a_NewPosition);
}





bool cPluginLua::OnPlayerPlacedBlock(cPlayer & a_Player, const sSetBlock & a_BlockChange)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_PLACED_BLOCK,
		&a_Player,
		a_BlockChange.GetX(), a_BlockChange.GetY(), a_BlockChange.GetZ(),
		a_BlockChange.m_BlockType, a_BlockChange.m_BlockMeta
	);
}





bool cPluginLua::OnPlayerPlacingBlock(cPlayer & a_Player, const sSetBlock & a_BlockChange)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_PLACING_BLOCK,
		&a_Player,
		a_BlockChange.GetX(), a_BlockChange.GetY(), a_BlockChange.GetZ(),
		a_BlockChange.m_BlockType, a_BlockChange.m_BlockMeta
	);
}





bool cPluginLua::OnPlayerRightClick(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_RIGHT_CLICK, &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
}





bool cPluginLua::OnPlayerRightClickingEntity(cPlayer & a_Player, cEntity & a_Entity)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_RIGHT_CLICKING_ENTITY, &a_Player, &a_Entity);
}





bool cPluginLua::OnPlayerShooting(cPlayer & a_Player)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_SHOOTING, &a_Player);
}





bool cPluginLua::OnPlayerSpawned(cPlayer & a_Player)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_SPAWNED, &a_Player);
}





bool cPluginLua::OnPlayerTossingItem(cPlayer & a_Player)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_TOSSING_ITEM, &a_Player);
}





bool cPluginLua::OnPlayerUsedBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_USED_BLOCK, &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta);
}





bool cPluginLua::OnPlayerUsedItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_USED_ITEM, &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
}





bool cPluginLua::OnPlayerUsingBlock(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_USING_BLOCK, &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ, a_BlockType, a_BlockMeta);
}





bool cPluginLua::OnPlayerUsingItem(cPlayer & a_Player, int a_BlockX, int a_BlockY, int a_BlockZ, char a_BlockFace, int a_CursorX, int a_CursorY, int a_CursorZ)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_USING_ITEM, &a_Player, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace, a_CursorX, a_CursorY, a_CursorZ);
}





bool cPluginLua::OnPluginMessage(cClientHandle & a_Client, const AString & a_Channel, const AString & a_Message)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLUGIN_MESSAGE, &a_Client, a_Channel, a_Message);
}





bool cPluginLua::OnPluginsLoaded(void)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_PLUGINS_LOADED];
	for (auto & hook: hooks)
	{
		bool ret = false;
		hook->Call(cLuaState::Return, ret);
		res = res || ret;
	}
	return res;
}





bool cPluginLua::OnPostCrafting(cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe)
{
	return CallSimpleHooks(cPluginManager::HOOK_POST_CRAFTING, &a_Player, &a_Grid, &a_Recipe);
}





bool cPluginLua::OnPreCrafting(cPlayer & a_Player, cCraftingGrid & a_Grid, cCraftingRecipe & a_Recipe)
{
	return CallSimpleHooks(cPluginManager::HOOK_PRE_CRAFTING, &a_Player, &a_Grid, &a_Recipe);
}





bool cPluginLua::OnProjectileHitBlock(cProjectileEntity & a_Projectile, int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_Face, const Vector3d & a_BlockHitPos)
{
	return CallSimpleHooks(cPluginManager::HOOK_PROJECTILE_HIT_BLOCK, &a_Projectile, a_BlockX, a_BlockY, a_BlockZ, a_Face, a_BlockHitPos);
}





bool cPluginLua::OnProjectileHitEntity(cProjectileEntity & a_Projectile, cEntity & a_HitEntity)
{
	return CallSimpleHooks(cPluginManager::HOOK_PROJECTILE_HIT_ENTITY, &a_Projectile, &a_HitEntity);
}





bool cPluginLua::OnServerPing(cClientHandle & a_ClientHandle, AString & a_ServerDescription, int & a_OnlinePlayersCount, int & a_MaxPlayersCount, AString & a_Favicon)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_SERVER_PING];
	for (auto & hook: hooks)
	{
		hook->Call(&a_ClientHandle, a_ServerDescription, a_OnlinePlayersCount, a_MaxPlayersCount, a_Favicon, cLuaState::Return, res, a_ServerDescription, a_OnlinePlayersCount, a_MaxPlayersCount, a_Favicon);
		if (res)
		{
			return true;
		}
	}
	return false;
}





bool cPluginLua::OnSpawnedEntity(cWorld & a_World, cEntity & a_Entity)
{
	return CallSimpleHooks(cPluginManager::HOOK_SPAWNED_ENTITY, &a_World, &a_Entity);
}





bool cPluginLua::OnSpawnedMonster(cWorld & a_World, cMonster & a_Monster)
{
	return CallSimpleHooks(cPluginManager::HOOK_SPAWNED_MONSTER, &a_World, &a_Monster);
}





bool cPluginLua::OnSpawningEntity(cWorld & a_World, cEntity & a_Entity)
{
	return CallSimpleHooks(cPluginManager::HOOK_SPAWNING_ENTITY, &a_World, &a_Entity);
}





bool cPluginLua::OnSpawningMonster(cWorld & a_World, cMonster & a_Monster)
{
	return CallSimpleHooks(cPluginManager::HOOK_SPAWNING_MONSTER, &a_World, &a_Monster);
}





bool cPluginLua::OnTakeDamage(cEntity & a_Receiver, TakeDamageInfo & a_TDI)
{
	return CallSimpleHooks(cPluginManager::HOOK_TAKE_DAMAGE, &a_Receiver, &a_TDI);
}





bool cPluginLua::OnUpdatedSign(
	cWorld & a_World,
	int a_BlockX, int a_BlockY, int a_BlockZ,
	const AString & a_Line1, const AString & a_Line2, const AString & a_Line3, const AString & a_Line4,
	cPlayer * a_Player
)
{
	return CallSimpleHooks(cPluginManager::HOOK_UPDATED_SIGN, &a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player);
}





bool cPluginLua::OnUpdatingSign(
	cWorld & a_World,
	int a_BlockX, int a_BlockY, int a_BlockZ,
	AString & a_Line1, AString & a_Line2, AString & a_Line3, AString & a_Line4,
	cPlayer * a_Player
)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_UPDATING_SIGN];
	for (auto & hook: hooks)
	{
		hook->Call(&a_World, a_BlockX, a_BlockY, a_BlockZ, a_Line1, a_Line2, a_Line3, a_Line4, a_Player, cLuaState::Return, res, a_Line1, a_Line2, a_Line3, a_Line4);
		if (res)
		{
			return true;
		}
	}
	return false;
}





bool cPluginLua::OnWeatherChanged(cWorld & a_World)
{
	return CallSimpleHooks(cPluginManager::HOOK_WEATHER_CHANGED, &a_World);
}





bool cPluginLua::OnWeatherChanging(cWorld & a_World, eWeather & a_NewWeather)
{
	cCSLock Lock(m_CriticalSection);
	if (!m_LuaState.IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_WEATHER_CHANGING];
	for (auto & hook: hooks)
	{
		hook->Call(&a_World, a_NewWeather, cLuaState::Return, res, a_NewWeather);
		if (res)
		{
			return true;
		}
	}
	return false;
}





bool cPluginLua::OnWorldStarted(cWorld & a_World)
{
	return CallSimpleHooks(cPluginManager::HOOK_WORLD_STARTED, &a_World);
}





bool cPluginLua::OnWorldTick(cWorld & a_World, std::chrono::milliseconds a_Dt, std::chrono::milliseconds a_LastTickDurationMSec)
{
	return CallSimpleHooks(cPluginManager::HOOK_WORLD_TICK, &a_World, a_Dt, a_LastTickDurationMSec);
}





bool cPluginLua::HandleCommand(const AStringVector & a_Split, cPlayer & a_Player, const AString & a_FullCommand)
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
	m_LuaState.Call(cmd->second, a_Split, &a_Player, a_FullCommand, cLuaState::Return, res);
	return res;
}





bool cPluginLua::HandleConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output, const AString & a_FullCommand)
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
	m_LuaState.Call(cmd->second, a_Split, a_FullCommand, cLuaState::Return, res, str);
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
	if (m_LuaState != nullptr)
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
	if (m_LuaState != nullptr)
	{
		for (CommandMap::iterator itr = m_ConsoleCommands.begin(), end = m_ConsoleCommands.end(); itr != end; ++itr)
		{
			luaL_unref(m_LuaState, LUA_REGISTRYINDEX, itr->second);
		}
	}
	m_ConsoleCommands.clear();
}





bool cPluginLua::CanAddOldStyleHook(int a_HookType)
{
	const char * FnName = GetHookFnName(a_HookType);
	if (FnName == nullptr)
	{
		// Unknown hook ID
		LOGWARNING("Plugin %s wants to add an unknown hook ID (%d). The plugin need not work properly.",
			GetName().c_str(), a_HookType
		);
		m_LuaState.LogStackTrace();
		return false;
	}

	// Check if the function is available
	if (m_LuaState.HasFunction(FnName))
	{
		return true;
	}

	LOGWARNING("Plugin %s wants to add a hook (%d), but it doesn't provide the callback function \"%s\" for it. The plugin need not work properly.",
		GetName().c_str(), a_HookType, FnName
	);
	m_LuaState.LogStackTrace();
	return false;
}





const char * cPluginLua::GetHookFnName(int a_HookType)
{
	switch (a_HookType)
	{
		case cPluginManager::HOOK_BLOCK_SPREAD:                 return "OnBlockSpread";
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
		case cPluginManager::HOOK_PLAYER_ANIMATION:             return "OnPlayerAnimation";
		case cPluginManager::HOOK_ENTITY_ADD_EFFECT:            return "OnEntityAddEffect";
		case cPluginManager::HOOK_ENTITY_CHANGING_WORLD:        return "OnEntityChangingWorld";
		case cPluginManager::HOOK_ENTITY_CHANGED_WORLD:         return "OnEntityChangedWorld";
		case cPluginManager::HOOK_ENTITY_TELEPORT:              return "OnEntityTeleport";
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
		case cPluginManager::HOOK_PLUGIN_MESSAGE:               return "OnPluginMessage";
		case cPluginManager::HOOK_PLUGINS_LOADED:               return "OnPluginsLoaded";
		case cPluginManager::HOOK_POST_CRAFTING:                return "OnPostCrafting";
		case cPluginManager::HOOK_PRE_CRAFTING:                 return "OnPreCrafting";
		case cPluginManager::HOOK_SERVER_PING:                  return "OnServerPing";
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

		case cPluginManager::HOOK_NUM_HOOKS:
		{
			// Satisfy a warning that all enum values should be used in a switch
			// but don't want a default branch, so that we catch new hooks missing from this list.
			break;
		}
	}  // switch (a_Hook)
	LOGWARNING("Requested name of an unknown hook type function: %d (max is %d)", a_HookType, cPluginManager::HOOK_MAX);
	ASSERT(!"Unknown hook requested!");
	return nullptr;
}





bool cPluginLua::AddHookCallback(int a_HookType, cLuaState::cCallbackPtr a_Callback)
{
	ASSERT(m_CriticalSection.IsLockedByCurrentThread());  // It probably has to be, how else would we have a LuaState?

	m_HookMap[a_HookType].push_back(a_Callback);
	return true;
}





int cPluginLua::CallFunctionFromForeignState(
	const AString & a_FunctionName,
	cLuaState & a_ForeignState,
	int a_ParamStart,
	int a_ParamEnd
)
{
	cCSLock Lock(m_CriticalSection);

	// Call the function:
	int NumReturns = m_LuaState.CallFunctionWithForeignParams(a_FunctionName, a_ForeignState, a_ParamStart, a_ParamEnd);
	if (NumReturns < 0)
	{
		// The call has failed, an error has already been output to the log, so just silently bail out with the same error
		return NumReturns;
	}

	// Copy all the return values:
	int Top = lua_gettop(m_LuaState);
	int res = a_ForeignState.CopyStackFrom(m_LuaState, Top - NumReturns + 1, Top);

	// Remove the return values off this stack:
	if (NumReturns > 0)
	{
		lua_pop(m_LuaState, NumReturns);
	}

	return res;
}





void cPluginLua::AddResettable(cPluginLua::cResettablePtr a_Resettable)
{
	cCSLock Lock(m_CriticalSection);
	m_Resettables.push_back(a_Resettable);
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
	bool res = false;
	m_LuaState.Call(a_FnRef, &a_Window, &a_Player, a_CanRefuse, cLuaState::Return, res);
	return res;
}





void cPluginLua::CallbackWindowSlotChanged(int a_FnRef, cWindow & a_Window, int a_SlotNum)
{
	ASSERT(a_FnRef != LUA_REFNIL);

	cCSLock Lock(m_CriticalSection);
	m_LuaState.Call(a_FnRef, &a_Window, a_SlotNum);
}





void cPluginLua::ClearWebTabs(void)
{
	auto webAdmin = cRoot::Get()->GetWebAdmin();
	if (webAdmin != nullptr)  // can be nullptr when shutting down the server
	{
		webAdmin->RemoveAllPluginWebTabs(m_Name);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cPluginLua::cResettable:

cPluginLua::cResettable::cResettable(cPluginLua & a_Plugin):
	m_Plugin(&a_Plugin)
{
}





void cPluginLua::cResettable::Reset(void)
{
	cCSLock Lock(m_CSPlugin);
	m_Plugin = nullptr;
}




