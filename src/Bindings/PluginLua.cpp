
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

extern "C"
{
	#include "lua/src/lauxlib.h"
}

#undef TOLUA_TEMPLATE_BIND
#include "tolua++/include/tolua++.h"





////////////////////////////////////////////////////////////////////////////////
// cPluginLua:

cPluginLua::cPluginLua(const AString & a_PluginDirectory) :
	m_Directory(a_PluginDirectory),
	m_LuaState(Printf("plugin %s", a_PluginDirectory.c_str()))
{
}





cPluginLua::~cPluginLua()
{
	LOGD("Destroying plugin \"%s\".", m_Name.c_str());
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
	for (cHookMap::iterator itrH = m_HookMap.begin(), endH = m_HookMap.end(); itrH != endH; ++itrH)
	{
		for (cLuaRefs::iterator itrR = itrH->second.begin(), endR = itrH->second.end(); itrR != endR; ++itrR)
		{
			delete *itrR;
		}  // for itrR - itrH->second[]
	}  // for itrH - m_HookMap[]
	m_HookMap.clear();

	// Close the Lua engine:
	m_LuaState.Close();
}





bool cPluginLua::Initialize(void)
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
			Close();
			return false;
		}
	}  // for itr - Files[]
	if (HasInfoLua)
	{
		AString Path = PluginPath + "Info.lua";
		if (!m_LuaState.LoadFile(Path))
		{
			Close();
			return false;
		}
	}

	// Call the Initialize function:
	bool res = false;
	if (!m_LuaState.Call("Initialize", this, cLuaState::Return, res))
	{
		LOGWARNING("Error in plugin %s: Cannot call the Initialize() function. Plugin is temporarily disabled.", GetName().c_str());
		Close();
		return false;
	}
	if (!res)
	{
		LOGINFO("Plugin %s: Initialize() call failed, plugin is temporarily disabled.", GetName().c_str());
		Close();
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
	if (!m_LuaState.IsValid())
	{
		return;
	}
	cLuaRefs & Refs = m_HookMap[cPluginManager::HOOK_TICK];
	for (cLuaRefs::iterator itr = Refs.begin(), end = Refs.end(); itr != end; ++itr)
	{
		m_LuaState.Call((int)(**itr), a_Dt);
	}
}





bool cPluginLua::HandleCommand(const AStringVector & a_Split, cPlayer & a_Player, const AString & a_FullCommand)
{
	ASSERT(!a_Split.empty());
	CommandMap::iterator cmd = m_Commands.find(a_Split[0]);
	if (cmd == m_Commands.end())
	{
		LOGWARNING("Command handler is registered in cPluginManager but not in cPluginLua, wtf? Command \"%s\".", a_Split[0].c_str());
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
		LOGWARNING("Console command handler is registered in cPluginManager but not in cPluginLua, wtf? Console command \"%s\", plugin \"%s\".",
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





bool cPluginLua::AddHookRef(int a_HookType, int a_FnRefIdx)
{
	ASSERT(m_CriticalSection.IsLockedByCurrentThread());  // It probably has to be, how else would we have a LuaState?
	
	// Check if the function reference is valid:
	cLuaState::cRef * Ref = new cLuaState::cRef(m_LuaState, a_FnRefIdx);
	if ((Ref == nullptr) || !Ref->IsValid())
	{
		LOGWARNING("Plugin %s tried to add a hook %d with bad handler function.", GetName().c_str(), a_HookType);
		m_LuaState.LogStackTrace();
		delete Ref;
		Ref = nullptr;
		return false;
	}
	
	m_HookMap[a_HookType].push_back(Ref);
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





AString cPluginLua::HandleWebRequest(const HTTPRequest * a_Request)
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
		if ((*itr)->SafeTitle.compare(SafeTabName) == 0)  // This is the one! Rawr
		{
			Tab = *itr;
			break;
		}
	}

	if (Tab != nullptr)
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




