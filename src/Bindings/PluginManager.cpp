
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PluginManager.h"
#include "PluginLua.h"
#include "../Item.h"
#include "../Root.h"
#include "../Server.h"
#include "../CommandOutput.h"

#include "../IniFile.h"
#include "../Entities/Player.h"

#define FIND_HOOK(a_HookName) HookMap::iterator Plugins = m_Hooks.find(a_HookName);
#define VERIFY_HOOK \
	if (Plugins == m_Hooks.end()) \
	{ \
		return false; \
	}





cPluginManager * cPluginManager::Get(void)
{
	return cRoot::Get()->GetPluginManager();
}





cPluginManager::cPluginManager(void) :
	m_bReloadPlugins(false)
{
}





cPluginManager::~cPluginManager()
{
	UnloadPluginsNow();
}





void cPluginManager::ReloadPlugins(void)
{
	m_bReloadPlugins = true;
}





void cPluginManager::FindPlugins(void)
{
	AString PluginsPath = GetPluginsPath() + "/";

	// First get a clean list of only the currently running plugins, we don't want to mess those up
	for (PluginMap::iterator itr = m_Plugins.begin(); itr != m_Plugins.end();)
	{
		if (itr->second == nullptr)
		{
			PluginMap::iterator thiz = itr;
			++thiz;
			m_Plugins.erase( itr);
			itr = thiz;
			continue;
		}
		++itr;
	}

	AStringVector Files = cFile::GetFolderContents(PluginsPath.c_str());
	for (AStringVector::const_iterator itr = Files.begin(); itr != Files.end(); ++itr)
	{
		if ((*itr == ".") || (*itr == "..") || (!cFile::IsFolder(PluginsPath + *itr)))
		{
			// We only want folders, and don't want "." or ".."
			continue;
		}

		// Add plugin name/directory to the list
		if (m_Plugins.find(*itr) == m_Plugins.end())
		{
			m_Plugins[*itr] = nullptr;
		}
	}
}





void cPluginManager::ReloadPluginsNow(void)
{
	cIniFile a_SettingsIni;
	a_SettingsIni.ReadFile("settings.ini");
	ReloadPluginsNow(a_SettingsIni);
}





void cPluginManager::ReloadPluginsNow(cIniFile & a_SettingsIni)
{
	LOG("-- Loading Plugins --");
	m_bReloadPlugins = false;
	UnloadPluginsNow();

	FindPlugins();

	cServer::BindBuiltInConsoleCommands();

	// Check if the Plugins section exists.
	int KeyNum = a_SettingsIni.FindKey("Plugins");

	if (KeyNum == -1)
	{
		InsertDefaultPlugins(a_SettingsIni);
		KeyNum = a_SettingsIni.FindKey("Plugins");
	}

	// How many plugins are there?
	int NumPlugins = a_SettingsIni.GetNumValues(KeyNum);

	for (int i = 0; i < NumPlugins; i++)
	{
		AString ValueName = a_SettingsIni.GetValueName(KeyNum, i);
		if (ValueName.compare("Plugin") == 0)
		{
			AString PluginFile = a_SettingsIni.GetValue(KeyNum, i);
			if (!PluginFile.empty())
			{
				if (m_Plugins.find(PluginFile) != m_Plugins.end())
				{
					LoadPlugin(PluginFile);
				}
			}
		}
	}


	// Remove invalid plugins from the PluginMap.
	for (PluginMap::iterator itr = m_Plugins.begin(); itr != m_Plugins.end();)
	{
		if (itr->second == nullptr)
		{
			PluginMap::iterator thiz = itr;
			++thiz;
			m_Plugins.erase(itr);
			itr = thiz;
			continue;
		}
		++itr;
	}

	size_t NumLoadedPlugins = GetNumPlugins();
	if (NumLoadedPlugins == 0)
	{
		LOG("-- No Plugins Loaded --");
	}
	else if (NumLoadedPlugins == 1)
	{
		LOG("-- Loaded 1 Plugin --");
	}
	else
	{
		LOG("-- Loaded %i Plugins --", (int)NumLoadedPlugins);
	}
	CallHook(HOOK_PLUGINS_LOADED);
}





bool cPluginManager::CallHookExploded(cWorld & a_World, double a_ExplosionSize, bool a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	FIND_HOOK(HOOK_EXPLODED);
	VERIFY_HOOK;

	switch (a_Source)
	{
		case esPrimedTNT:
		{
			for (const auto & itr : Plugins->second)
			{
				if (itr->CallHook(HOOK_EXPLODED, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cTNTEntity *>(a_SourceData)))
				{
					return true;
				}
			}
			break;
		}
		case esMonster:
		{
			for (const auto & itr : Plugins->second)
			{
				if (itr->CallHook(HOOK_EXPLODED, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cMonster *>(a_SourceData)))
				{
					return true;
				}
			}
			break;
		}
		case esBed:
		case esEnderCrystal:
		{
			for (const auto & itr : Plugins->second)
			{
				if (itr->CallHook(HOOK_EXPLODED, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<Vector3i *>(a_SourceData)))
				{
					return true;
				}
			}
			break;
		}
		case esPlugin:
		case esGhastFireball:
		case esWitherSkullBlack:
		case esWitherSkullBlue:
		case esWitherBirth:
		case esOther:
		{
			for (const auto & itr : Plugins->second)
			{
				if (itr->CallHook(HOOK_EXPLODED, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData))
				{
					return true;
				}
			}
			break;
		}
		default:
		{
			ASSERT(!"Unhandled ExplosionSource");
		}
	}
	
	return false;
}





bool cPluginManager::CallHookExploding(cWorld & a_World, double & a_ExplosionSize, bool & a_CanCauseFire, double a_X, double a_Y, double a_Z, eExplosionSource a_Source, void * a_SourceData)
{
	FIND_HOOK(HOOK_EXPLODING);
	VERIFY_HOOK;

	switch (a_Source)
	{
		case esPrimedTNT:
		{
			for (const auto & itr : Plugins->second)
			{
				if (itr->CallHook(HOOK_EXPLODED, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cTNTEntity *>(a_SourceData)))
				{
					return true;
				}
			}
			break;
		}
		case esMonster:
		{
			for (const auto & itr : Plugins->second)
			{
				if (itr->CallHook(HOOK_EXPLODED, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<cMonster *>(a_SourceData)))
				{
					return true;
				}
			}
			break;
		}
		case esBed:
		case esEnderCrystal:
		{
			for (const auto & itr : Plugins->second)
			{
				if (itr->CallHook(HOOK_EXPLODED, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, static_cast<Vector3i *>(a_SourceData)))
				{
					return true;
				}
			}
			break;
		}
		case esPlugin:
		case esGhastFireball:
		case esWitherSkullBlack:
		case esWitherSkullBlue:
		case esWitherBirth:
		case esOther:
		{
			for (const auto & itr : Plugins->second)
			{
				if (itr->CallHook(HOOK_EXPLODED, &a_World, a_ExplosionSize, a_CanCauseFire, a_X, a_Y, a_Z, a_Source, a_SourceData))
				{
					return true;
				}
			}
			break;
		}
		default:
		{
			ASSERT(!"Unhandled ExplosionSource");
		}
	}

	return false;
}





void cPluginManager::InsertDefaultPlugins(cIniFile & a_SettingsIni)
{
	a_SettingsIni.AddKeyName("Plugins");
	a_SettingsIni.AddKeyComment("Plugins", " Plugin=Debuggers");
	a_SettingsIni.AddKeyComment("Plugins", " Plugin=HookNotify");
	a_SettingsIni.AddKeyComment("Plugins", " Plugin=ChunkWorx");
	a_SettingsIni.AddKeyComment("Plugins", " Plugin=APIDump");
	a_SettingsIni.AddValue("Plugins", "Plugin", "Core");
	a_SettingsIni.AddValue("Plugins", "Plugin", "TransAPI");
	a_SettingsIni.AddValue("Plugins", "Plugin", "ChatLog");
}





void cPluginManager::Tick(float a_Dt)
{
	while (!m_DisablePluginList.empty())
	{
		RemovePlugin(m_DisablePluginList.front());
		m_DisablePluginList.pop_front();
	}

	if (m_bReloadPlugins)
	{
		ReloadPluginsNow();
	}

	FIND_HOOK(HOOK_TICK);
	if (Plugins != m_Hooks.end())
	{
		for (PluginList::iterator itr = Plugins->second.begin(); itr != Plugins->second.end(); ++itr)
		{
			(*itr)->Tick(a_Dt);
		}
	}
}





bool cPluginManager::CallHookChat(cPlayer & a_Player, AString & a_Message)
{
	// Check if the message contains a command, execute it:
	switch (HandleCommand(a_Player, a_Message, true))
	{
		case crExecuted:
		{
			// The command has executed successfully
			return true;
		}
		
		case crBlocked:
		{
			// The command was blocked by a plugin using HOOK_EXECUTE_COMMAND
			// The plugin has most likely sent a message to the player already
			return true;
		}
		
		case crError:
		{
			// An error in the plugin has prevented the command from executing. Report the error to the player:
			a_Player.SendMessageFailure(Printf("Something went wrong while executing command \"%s\"", a_Message.c_str()));
			return true;
		}
		
		case crNoPermission:
		{
			// The player is not allowed to execute this command
			a_Player.SendMessageFailure(Printf("Forbidden command; insufficient privileges: \"%s\"", a_Message.c_str()));
			return true;
		}

		case crUnknownCommand:
		{
			// This was not a known command, keep processing as a message
			break;
		}
	}

	// Check if the message is a command (starts with a slash). If it is, we know that it wasn't recognised:
	if (!a_Message.empty() && (a_Message[0] == '/'))
	{
		AStringVector Split(StringSplit(a_Message, " "));
		ASSERT(!Split.empty());  // This should not happen - we know there's at least one char in the message so the split needs to be at least one item long
		a_Player.SendMessageInfo(Printf("Unknown command: \"%s\"", a_Message.c_str()));
		LOGINFO("Player %s issued an unknown command: \"%s\"", a_Player.GetName().c_str(), a_Message.c_str());
		return true;  // Cancel sending
	}

	FIND_HOOK(HOOK_CHAT);
	VERIFY_HOOK;

	for (const auto & itr : Plugins->second)
	{
		if (itr->CallHook(HOOK_CHAT, &a_Player, a_Message))
		{
			return true;
		}
	}

	return false;
}





cPluginManager::CommandResult cPluginManager::HandleCommand(cPlayer & a_Player, const AString & a_Command, bool a_ShouldCheckPermissions)
{
	AStringVector Split(StringSplit(a_Command, " "));
	if (Split.empty())
	{
		return crUnknownCommand;
	}

	CommandMap::iterator cmd = m_Commands.find(Split[0]);
	if (cmd == m_Commands.end())
	{
		// Command not found
		return crUnknownCommand;
	}

	// Ask plugins first if a command is okay to execute the command:
	if (CallHook(HOOK_EXECUTE_COMMAND, &a_Player, Split))
	{
		LOGINFO("Player %s tried executing command \"%s\" that was stopped by the HOOK_EXECUTE_COMMAND hook", a_Player.GetName().c_str(), Split[0].c_str());
		return crBlocked;
	}

	if (
		a_ShouldCheckPermissions &&
		!cmd->second.m_Permission.empty() &&
		!a_Player.HasPermission(cmd->second.m_Permission)
	)
	{
		LOGINFO("Player %s tried to execute forbidden command: \"%s\"", a_Player.GetName().c_str(), Split[0].c_str());
		return crNoPermission;
	}

	ASSERT(cmd->second.m_Plugin != nullptr);

	if (!cmd->second.m_Plugin->HandleCommand(Split, a_Player, a_Command))
	{
		return crError;
	}

	return crExecuted;
}





cPluginLua * cPluginManager::GetPlugin(const AString & a_Plugin) const
{
	for (PluginMap::const_iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr)
	{
		if (itr->second == nullptr)
		{
			// The plugin is currently unloaded
			continue;
		}

		if (itr->second->GetName().compare(a_Plugin) == 0)
		{
			return itr->second;
		}
	}
	return 0;
}





const cPluginManager::PluginMap & cPluginManager::GetAllPlugins() const
{
	return m_Plugins;
}





void cPluginManager::UnloadPluginsNow()
{
	m_Hooks.clear();

	while (!m_Plugins.empty())
	{
		RemovePlugin(m_Plugins.begin()->second);
	}

	m_Commands.clear();
	m_ConsoleCommands.clear();
}





bool cPluginManager::DisablePlugin(const AString & a_PluginName)
{
	PluginMap::iterator itr = m_Plugins.find(a_PluginName);
	if (itr == m_Plugins.end())
	{
		return false;
	}

	if (itr->first.compare(a_PluginName) == 0)  // _X 2013_02_01: wtf? Isn't this supposed to be what find() does?
	{
		m_DisablePluginList.push_back(itr->second);
		itr->second = nullptr;  // Get rid of this thing right away
		return true;
	}
	return false;
}





bool cPluginManager::LoadPlugin(const AString & a_PluginName)
{
	return AddPlugin(new cPluginLua(a_PluginName.c_str()));
}





void cPluginManager::RemoveHooks(cPluginLua * a_Plugin)
{
	for (HookMap::iterator itr = m_Hooks.begin(), end = m_Hooks.end(); itr != end; ++itr)
	{
		itr->second.remove(a_Plugin);
	}
}





void cPluginManager::RemovePlugin(cPluginLua * a_Plugin)
{
	for (PluginMap::iterator itr = m_Plugins.begin(); itr != m_Plugins.end(); ++itr)
	{
		if (itr->second == a_Plugin)
		{
			m_Plugins.erase(itr);
			break;
		}
	}

	RemovePluginCommands(a_Plugin);
	RemovePluginConsoleCommands(a_Plugin);
	RemoveHooks(a_Plugin);
	if (a_Plugin != nullptr)
	{
		a_Plugin->OnDisable();
	}
	delete a_Plugin;
	a_Plugin = nullptr;
}





void cPluginManager::RemovePluginCommands(cPluginLua * a_Plugin)
{
	if (a_Plugin != nullptr)
	{
		a_Plugin->ClearCommands();
	}

	for (CommandMap::iterator itr = m_Commands.begin(); itr != m_Commands.end();)
	{
		if (itr->second.m_Plugin == a_Plugin)
		{
			CommandMap::iterator EraseMe = itr;  // Stupid GCC doesn't have a std::map::erase() that would return the next iterator
			++itr;
			m_Commands.erase(EraseMe);
		}
		else
		{
			++itr;
		}
	}  // for itr - m_Commands[]
}





bool cPluginManager::BindCommand(const AString & a_Command, cPluginLua * a_Plugin, const AString & a_Permission, const AString & a_HelpString)
{
	CommandMap::iterator cmd = m_Commands.find(a_Command);
	if (cmd != m_Commands.end())
	{
		LOGWARNING("Command \"%s\" is already bound to plugin \"%s\".", a_Command.c_str(), cmd->second.m_Plugin->GetName().c_str());
		return false;
	}

	m_Commands[a_Command].m_Plugin     = a_Plugin;
	m_Commands[a_Command].m_Permission = a_Permission;
	m_Commands[a_Command].m_HelpString = a_HelpString;
	return true;
}





bool cPluginManager::ForEachCommand(cCommandEnumCallback & a_Callback)
{
	for (CommandMap::iterator itr = m_Commands.begin(), end = m_Commands.end(); itr != end; ++itr)
	{
		if (a_Callback.Command(itr->first, itr->second.m_Plugin, itr->second.m_Permission, itr->second.m_HelpString))
		{
			return false;
		}
	}  // for itr - m_Commands[]
	return true;
}





bool cPluginManager::IsCommandBound(const AString & a_Command)
{
	return (m_Commands.find(a_Command) != m_Commands.end());
}





AString cPluginManager::GetCommandPermission(const AString & a_Command)
{
	CommandMap::iterator cmd = m_Commands.find(a_Command);
	return (cmd == m_Commands.end()) ? "" : cmd->second.m_Permission;
}





cPluginManager::CommandResult cPluginManager::ExecuteCommand(cPlayer & a_Player, const AString & a_Command)
{
	return HandleCommand(a_Player, a_Command, true);
}





cPluginManager::CommandResult cPluginManager::ForceExecuteCommand(cPlayer & a_Player, const AString & a_Command)
{
	return HandleCommand(a_Player, a_Command, false);
}





void cPluginManager::RemovePluginConsoleCommands(cPluginLua * a_Plugin)
{
	if (a_Plugin != nullptr)
	{
		a_Plugin->ClearConsoleCommands();
	}

	for (CommandMap::iterator itr = m_ConsoleCommands.begin(); itr != m_ConsoleCommands.end();)
	{
		if (itr->second.m_Plugin == a_Plugin)
		{
			CommandMap::iterator EraseMe = itr;  // Stupid GCC doesn't have a std::map::erase() that would return the next iterator
			++itr;
			m_ConsoleCommands.erase(EraseMe);
		}
		else
		{
			++itr;
		}
	}  // for itr - m_Commands[]
}





bool cPluginManager::BindConsoleCommand(const AString & a_Command, cPluginLua * a_Plugin, const AString & a_HelpString)
{
	CommandMap::iterator cmd = m_ConsoleCommands.find(a_Command);
	if (cmd != m_ConsoleCommands.end())
	{
		if (cmd->second.m_Plugin == nullptr)
		{
			LOGWARNING("Console command \"%s\" is already bound internally by MCServer, cannot bind in plugin \"%s\".", a_Command.c_str(), a_Plugin->GetName().c_str());
		}
		else
		{
			LOGWARNING("Console command \"%s\" is already bound to plugin \"%s\", cannot bind in plugin \"%s\".", a_Command.c_str(), cmd->second.m_Plugin->GetName().c_str(), a_Plugin->GetName().c_str());
		}
		return false;
	}

	m_ConsoleCommands[a_Command].m_Plugin     = a_Plugin;
	m_ConsoleCommands[a_Command].m_Permission = "";
	m_ConsoleCommands[a_Command].m_HelpString = a_HelpString;
	return true;
}





bool cPluginManager::ForEachConsoleCommand(cCommandEnumCallback & a_Callback)
{
	for (CommandMap::iterator itr = m_ConsoleCommands.begin(), end = m_ConsoleCommands.end(); itr != end; ++itr)
	{
		if (a_Callback.Command(itr->first, itr->second.m_Plugin, "", itr->second.m_HelpString))
		{
			return false;
		}
	}  // for itr - m_Commands[]
	return true;
}





bool cPluginManager::IsConsoleCommandBound(const AString & a_Command)
{
	return (m_ConsoleCommands.find(a_Command) != m_ConsoleCommands.end());
}





bool cPluginManager::ExecuteConsoleCommand(const AStringVector & a_Split, cCommandOutputCallback & a_Output, const AString & a_Command)
{
	if (a_Split.empty())
	{
		return false;
	}

	CommandMap::iterator cmd = m_ConsoleCommands.find(a_Split[0]);
	if (cmd == m_ConsoleCommands.end())
	{
		// Command not found
		return false;
	}

	if (cmd->second.m_Plugin == nullptr)
	{
		// This is a built-in command
		return false;
	}

	// Ask plugins first if a command is okay to execute the console command:
	if (CallHook(HOOK_EXECUTE_COMMAND, static_cast<cPlayer *>(nullptr), a_Split))
	{
		a_Output.Out("Command \"%s\" was stopped by the HOOK_EXECUTE_COMMAND hook", a_Split[0].c_str());
		return false;
	}

	return cmd->second.m_Plugin->HandleConsoleCommand(a_Split, a_Output, a_Command);
}





void cPluginManager::TabCompleteCommand(const AString & a_Text, AStringVector & a_Results, cPlayer * a_Player)
{
	for (CommandMap::iterator itr = m_Commands.begin(), end = m_Commands.end(); itr != end; ++itr)
	{
		if (NoCaseCompare(itr->first.substr(0, a_Text.length()), a_Text) != 0)
		{
			// Command name doesn't match
			continue;
		}
		if ((a_Player != nullptr) && !a_Player->HasPermission(itr->second.m_Permission))
		{
			// Player doesn't have permission for the command
			continue;
		}
		a_Results.push_back(itr->first);
	}
}





bool cPluginManager::IsValidHookType(int a_HookType)
{
	return ((a_HookType >= 0) && (a_HookType <= HOOK_MAX));
}





bool cPluginManager::DoWithPlugin(const AString & a_PluginName, cPluginCallback & a_Callback)
{
	// TODO: Implement locking for plugins
	PluginMap::iterator itr = m_Plugins.find(a_PluginName);
	if ((itr == m_Plugins.end()) || (itr->second == nullptr))
	{
		return false;
	}
	return a_Callback.Item(itr->second);
}





bool cPluginManager::AddPlugin(cPluginLua * a_Plugin)
{
	m_Plugins[a_Plugin->GetDirectory()] = a_Plugin;

	if (a_Plugin->Initialize())
	{
		// Initialization OK
		return true;
	}

	// Initialization failed
	RemovePlugin(a_Plugin);  // Also undoes any registrations that Initialize() might have made
	return false;
}





void cPluginManager::AddHook(cPluginLua * a_Plugin, int a_Hook)
{
	if (!a_Plugin)
	{
		LOGWARN("Called cPluginManager::AddHook() with a_Plugin == nullptr");
		return;
	}
	auto & Plugins = m_Hooks[a_Hook];
	Plugins.remove(a_Plugin);
	Plugins.push_back(a_Plugin);
}





size_t cPluginManager::GetNumPlugins() const
{
	return m_Plugins.size();
}




