
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

cPluginLua::cPluginLua(const AString & a_PluginDirectory, cDeadlockDetect & a_DeadlockDetect) :
	cPlugin(a_PluginDirectory),
	m_LuaState(Printf("plugin %s", a_PluginDirectory.c_str())),
	m_DeadlockDetect(a_DeadlockDetect)
{
	m_LuaState.TrackInDeadlockDetect(a_DeadlockDetect);
}





cPluginLua::~cPluginLua()
{
	Close();
	m_LuaState.UntrackInDeadlockDetect(m_DeadlockDetect);
}





void cPluginLua::Close(void)
{
	cOperation op(*this);
	// If already closed, bail out:
	if (!op().IsValid())
	{
		ASSERT(m_HookMap.empty());
		return;
	}

	// Remove the web tabs:
	ClearWebTabs();

	// Release all the references in the hook map:
	m_HookMap.clear();

	// Close the Lua engine:
	op().Close();
}





bool cPluginLua::Load(void)
{
	cOperation op(*this);
	if (!op().IsValid())
	{
		m_LuaState.Create();
		m_LuaState.RegisterAPILibs();

		// Inject the identification global variables into the state:
		lua_pushlightuserdata(m_LuaState, this);
		lua_setglobal(m_LuaState, LUA_PLUGIN_INSTANCE_VAR_NAME);
		lua_pushstring(m_LuaState, GetName().c_str());
		lua_setglobal(m_LuaState, LUA_PLUGIN_NAME_VAR_NAME);

		// Add the plugin's folder to the package.path and package.cpath variables (#693):
		m_LuaState.AddPackagePath("path", GetLocalFolder() + "/?.lua");
		#ifdef _WIN32
			m_LuaState.AddPackagePath("cpath", GetLocalFolder() + "\\?.dll");
		#else
			m_LuaState.AddPackagePath("cpath", GetLocalFolder() + "/?.so");
		#endif

		tolua_pushusertype(m_LuaState, this, "cPluginLua");
		lua_setglobal(m_LuaState, "g_Plugin");
	}

	std::string PluginPath = GetLocalFolder() + "/";

	// List all Lua files for this plugin. Info.lua has a special handling - make it the last to load:
	AStringVector Files = cFile::GetFolderContents(PluginPath);
	AStringVector LuaFiles;
	bool HasInfoLua = false;
	for (AStringVector::const_iterator itr = Files.begin(), end = Files.end(); itr != end; ++itr)
	{
		size_t ExtensionStart = itr->find_last_of('.');
		if ((ExtensionStart != std::string::npos) && (itr->substr(ExtensionStart) == ".lua"))
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
	Super::Unload();
	Close();
}





void cPluginLua::OnDisable(void)
{
	cOperation op(*this);
	if (!op().HasFunction("OnDisable"))
	{
		return;
	}
	op().Call("OnDisable");
}





void cPluginLua::Tick(float a_Dt)
{
	CallSimpleHooks(cPluginManager::HOOK_TICK, a_Dt);
}





bool cPluginLua::OnBlockSpread(cWorld & a_World, int a_BlockX, int a_BlockY, int a_BlockZ, eSpreadSource a_Source)
{
	return CallSimpleHooks(cPluginManager::HOOK_BLOCK_SPREAD, &a_World, a_BlockX, a_BlockY, a_BlockZ, a_Source);
}





bool cPluginLua::OnBlockToPickups(
	cWorld & a_World,
	Vector3i a_BlockPos,
	BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta,
	const cBlockEntity * a_BlockEntity,
	const cEntity * a_Digger,
	const cItem * a_Tool,
	cItems & a_Pickups
)
{
	// TODO: Change the hook signature to reflect the real parameters to this function, once we are allowed to make breaking API changes
	return CallSimpleHooks(
		cPluginManager::HOOK_BLOCK_TO_PICKUPS,
		&a_World,
		a_Digger,
		a_BlockPos.x, a_BlockPos.y, a_BlockPos.z,
		a_BlockType, a_BlockMeta, &a_Pickups
	);
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
	cOperation op(*this);
	if (!op().IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_CHAT];
	for (auto & hook: hooks)
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
	cOperation op(*this);
	if (!op().IsValid())
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
	cOperation op(*this);
	if (!op().IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_EXPLODED];
	for (auto & hook: hooks)
	{
		switch (a_Source)
		{
			case esBed:           hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<Vector3i *>            (a_SourceData), cLuaState::Return, res); break;
			case esEnderCrystal:  hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cEntity *>             (a_SourceData), cLuaState::Return, res); break;
			case esGhastFireball: hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cGhastFireballEntity *>(a_SourceData), cLuaState::Return, res); break;
			case esMonster:       hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cMonster *>            (a_SourceData), cLuaState::Return, res); break;
			case esOther:         hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, cLuaState::Return, res); break;
			case esPlugin:        hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, cLuaState::Return, res); break;
			case esPrimedTNT:     hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cTNTEntity *>          (a_SourceData), cLuaState::Return, res); break;
			case esWitherBirth:   hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cMonster *>            (a_SourceData), cLuaState::Return, res); break;
			case esWitherSkull:   hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cWitherSkullEntity *>  (a_SourceData), cLuaState::Return, res); break;
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
	cOperation op(*this);
	if (!op().IsValid())
	{
		return false;
	}
	bool res = false;
	auto & hooks = m_HookMap[cPluginManager::HOOK_EXPLODING];
	for (auto & hook: hooks)
	{
		switch (a_Source)
		{
			case esBed:           hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<Vector3i *>            (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esEnderCrystal:  hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cEntity *>             (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esGhastFireball: hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cGhastFireballEntity *>(a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esMonster:       hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cMonster *>            (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esOther:         hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source,                                                         cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esPlugin:        hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source,                                                         cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esPrimedTNT:     hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cTNTEntity *>          (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esWitherBirth:   hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cMonster *>            (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
			case esWitherSkull:   hook->Call(&a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cWitherSkullEntity *>  (a_SourceData), cLuaState::Return, res, a_CanCauseFire, a_ExplosionSize); break;
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





bool cPluginLua::OnDropSpense(cWorld & a_World, cDropSpenserEntity & a_DropSpenser, int a_SlotNum)
{
	return CallSimpleHooks(cPluginManager::HOOK_DROPSPENSE, &a_World, &a_DropSpenser, a_SlotNum);
}





bool cPluginLua::OnKilled(cEntity & a_Victim, TakeDamageInfo & a_TDI, AString & a_DeathMessage)
{
	cOperation op(*this);
	if (!op().IsValid())
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





bool cPluginLua::OnLoginForge(cClientHandle & a_Client, const AStringMap & a_Mods)
{
	return CallSimpleHooks(cPluginManager::HOOK_LOGIN_FORGE, &a_Client, a_Mods);
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
	cItems reward(a_Reward);
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_FISHED, &a_Player, &reward);
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





bool cPluginLua::OnPlayerMoving(cPlayer & a_Player, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition, bool a_PreviousIsOnGround)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_MOVING, &a_Player, a_OldPosition, a_NewPosition, a_PreviousIsOnGround);
}





bool cPluginLua::OnEntityTeleport(cEntity & a_Entity, const Vector3d & a_OldPosition, const Vector3d & a_NewPosition)
{
	return CallSimpleHooks(cPluginManager::HOOK_ENTITY_TELEPORT, &a_Entity, a_OldPosition, a_NewPosition);
}





bool cPluginLua::OnPlayerOpeningWindow(cPlayer & a_Player, cWindow & a_Window)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_OPENING_WINDOW, &a_Player, &a_Window);
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





bool cPluginLua::OnPlayerCrouched(cPlayer & a_Player)
{
	return CallSimpleHooks(cPluginManager::HOOK_PLAYER_CROUCHED,
		&a_Player);
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
	cOperation op(*this);
	if (!op().IsValid())
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
	cOperation op(*this);
	if (!op().IsValid())
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
	cOperation op(*this);
	if (!op().IsValid())
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
	cOperation op(*this);
	if (!op().IsValid())
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
		case cPluginManager::HOOK_LOGIN_FORGE:                  return "OnLoginForge";
		case cPluginManager::HOOK_PLAYER_BREAKING_BLOCK:        return "OnPlayerBreakingBlock";
		case cPluginManager::HOOK_PLAYER_BROKEN_BLOCK:          return "OnPlayerBrokenBlock";
		case cPluginManager::HOOK_PLAYER_EATING:                return "OnPlayerEating";
		case cPluginManager::HOOK_PLAYER_JOINED:                return "OnPlayerJoined";
		case cPluginManager::HOOK_PLAYER_LEFT_CLICK:            return "OnPlayerLeftClick";
		case cPluginManager::HOOK_PLAYER_MOVING:                return "OnPlayerMoving";
		case cPluginManager::HOOK_PLAYER_OPENING_WINDOW:        return "OnPlayerOpeningWindow";
		case cPluginManager::HOOK_PLAYER_PLACED_BLOCK:          return "OnPlayerPlacedBlock";
		case cPluginManager::HOOK_PLAYER_PLACING_BLOCK:         return "OnPlayerPlacingBlock";
		case cPluginManager::HOOK_PLAYER_CROUCHED:        		return "OnPlayerCrouched";
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





bool cPluginLua::AddHookCallback(int a_HookType, cLuaState::cCallbackPtr && a_Callback)
{
	m_HookMap[a_HookType].push_back(std::move(a_Callback));
	return true;
}





int cPluginLua::CallFunctionFromForeignState(
	const AString & a_FunctionName,
	cLuaState & a_ForeignState,
	int a_ParamStart,
	int a_ParamEnd
)
{
	cOperation op(*this);

	// Call the function:
	int NumReturns = op().CallFunctionWithForeignParams(a_FunctionName, a_ForeignState, a_ParamStart, a_ParamEnd);
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





void cPluginLua::ClearWebTabs(void)
{
	auto webAdmin = cRoot::Get()->GetWebAdmin();
	if (webAdmin != nullptr)  // can be nullptr when shutting down the server
	{
		webAdmin->RemoveAllPluginWebTabs(m_Name);
	}
}




