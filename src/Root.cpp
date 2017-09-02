
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Root.h"

// STD lib hreaders:
#include <iostream>

// OS-specific headers:
#if defined(_WIN32)
	#include <psapi.h>
#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
	#include <signal.h>
	#if defined(__linux__)
		#include <fstream>
	#elif defined(__APPLE__)
		#include <mach/mach.h>
	#endif
#endif

#include "Server.h"
#include "World.h"
#include "WebAdmin.h"
#include "BrewingRecipes.h"
#include "FurnaceRecipe.h"
#include "CraftingRecipes.h"
#include "Bindings/PluginManager.h"
#include "MonsterConfig.h"
#include "Entities/Player.h"
#include "Blocks/BlockHandler.h"
#include "Items/ItemHandler.h"
#include "Chunk.h"
#include "Protocol/ProtocolRecognizer.h"  // for protocol version constants
#include "CommandOutput.h"
#include "DeadlockDetect.h"
#include "LoggerListeners.h"
#include "BuildInfo.h"
#include "IniFile.h"
#include "SettingsRepositoryInterface.h"
#include "OverridesSettingsRepository.h"
#include "Logger.h"
#include "ClientHandle.h"





cRoot * cRoot::s_Root = nullptr;





cRoot::cRoot(void) :
	m_pDefaultWorld(nullptr),
	m_Server(nullptr),
	m_MonsterConfig(nullptr),
	m_CraftingRecipes(nullptr),
	m_FurnaceRecipe(nullptr),
	m_BrewingRecipes(nullptr),
	m_WebAdmin(nullptr),
	m_PluginManager(nullptr),
	m_MojangAPI(nullptr)
{
	s_Root = this;
	m_InputThreadRunFlag.clear();
}





cRoot::~cRoot()
{
	s_Root = 0;
}





void cRoot::InputThread(cRoot & a_Params)
{
	cLogCommandOutputCallback Output;

	while (a_Params.m_InputThreadRunFlag.test_and_set() && std::cin.good())
	{
		AString Command;
		std::getline(std::cin, Command);
		if (!Command.empty())
		{
			// Execute and clear command string when submitted
			a_Params.ExecuteConsoleCommand(TrimString(Command), Output);
		}
	}

	// We have come here because the std::cin has received an EOF / a terminate signal has been sent, and the server is still running
	// Ignore this when running as a service, cin was never opened in that case
	if (!std::cin.good() && !m_RunAsService)
	{
		// Stop the server:
		a_Params.QueueExecuteConsoleCommand("stop");
	}
}





void cRoot::Start(std::unique_ptr<cSettingsRepositoryInterface> a_OverridesRepo)
{
	#ifdef _WIN32
		HMENU ConsoleMenu = GetSystemMenu(GetConsoleWindow(), FALSE);
		EnableMenuItem(ConsoleMenu, SC_CLOSE, MF_GRAYED);  // Disable close button when starting up; it causes problems with our CTRL-CLOSE handling
	#endif

	auto consoleLogListener = MakeConsoleListener(m_RunAsService);
	auto consoleAttachment = cLogger::GetInstance().AttachListener(std::move(consoleLogListener));

	cLogger::cAttachment fileAttachment;
	if (!a_OverridesRepo->HasValue("Server","DisableLogFile"))
	{
		auto fileLogListenerRet = MakeFileListener();
		if (!fileLogListenerRet.first)
		{
			m_TerminateEventRaised = true;
			LOGERROR("Failed to open log file, aborting");
			return;
		}
		fileAttachment = cLogger::GetInstance().AttachListener(std::move(fileLogListenerRet.second));
	}

	LOG("--- Started Log ---");

	#ifdef BUILD_ID
		LOG("Cuberite " BUILD_SERIES_NAME " build id: " BUILD_ID);
		LOG("from commit id: " BUILD_COMMIT_ID " built at: " BUILD_DATETIME);
	#endif

	cDeadlockDetect dd;
	auto BeginTime = std::chrono::steady_clock::now();

	LoadGlobalSettings();

	LOG("Creating new server instance...");
	m_Server = new cServer();

	LOG("Reading server config...");

	m_SettingsFilename = "settings.ini";
	if (a_OverridesRepo->HasValue("Server","ConfigFile"))
	{
		m_SettingsFilename = a_OverridesRepo->GetValue("Server","ConfigFile");
	}

	auto IniFile = cpp14::make_unique<cIniFile>();
	bool IsNewIniFile = !IniFile->ReadFile(m_SettingsFilename);

	if (IsNewIniFile)
	{
		LOGWARN("Regenerating settings.ini, all settings will be reset");
		IniFile->AddHeaderComment(" This is the main server configuration");
		IniFile->AddHeaderComment(" Most of the settings here can be configured using the webadmin interface, if enabled in webadmin.ini");
	}

	auto settingsRepo = cpp14::make_unique<cOverridesSettingsRepository>(std::move(IniFile), std::move(a_OverridesRepo));

	LOG("Starting server...");

	// cClientHandle::FASTBREAK_PERCENTAGE = settingsRepo->GetValueSetI("AntiCheat", "FastBreakPercentage", 97) / 100.0f;
	cClientHandle::FASTBREAK_PERCENTAGE = 0;  // AntiCheat disabled due to bugs. We will enabled it once they are fixed. See #3506.

	m_MojangAPI = new cMojangAPI;
	bool ShouldAuthenticate = settingsRepo->GetValueSetB("Authentication", "Authenticate", true);
	m_MojangAPI->Start(*settingsRepo, ShouldAuthenticate);  // Mojang API needs to be started before plugins, so that plugins may use it for DB upgrades on server init
	if (!m_Server->InitServer(*settingsRepo, ShouldAuthenticate))
	{
		settingsRepo->Flush();
		LOGERROR("Failure starting server, aborting...");
		return;
	}

	m_WebAdmin = new cWebAdmin();
	m_WebAdmin->Init();

	LOGD("Loading settings...");
	m_RankManager.reset(new cRankManager());
	m_RankManager->Initialize(*m_MojangAPI);
	m_CraftingRecipes = new cCraftingRecipes();
	m_FurnaceRecipe   = new cFurnaceRecipe();
	m_BrewingRecipes.reset(new cBrewingRecipes());

	LOGD("Loading worlds...");
	LoadWorlds(*settingsRepo, IsNewIniFile);

	LOGD("Loading plugin manager...");
	m_PluginManager = new cPluginManager(dd);
	m_PluginManager->ReloadPluginsNow(*settingsRepo);

	LOGD("Loading MonsterConfig...");
	m_MonsterConfig = new cMonsterConfig;

	// This sets stuff in motion
	LOGD("Starting Authenticator...");
	m_Authenticator.Start(*settingsRepo);

	LOGD("Starting worlds...");
	StartWorlds(dd);

	if (settingsRepo->GetValueSetB("DeadlockDetect", "Enabled", true))
	{
		LOGD("Starting deadlock detector...");
		dd.Start(settingsRepo->GetValueSetI("DeadlockDetect", "IntervalSec", 20));
	}

	settingsRepo->Flush();

	LOGD("Finalising startup...");
	if (m_Server->Start())
	{
		m_WebAdmin->Start();

		LOGD("Starting InputThread...");
		try
		{
			m_InputThreadRunFlag.test_and_set();
			m_InputThread = std::thread(InputThread, std::ref(*this));
		}
		catch (std::system_error & a_Exception)
		{
			LOGERROR("cRoot::Start (std::thread) error %i: could not construct input thread; %s", a_Exception.code().value(), a_Exception.what());
		}

		LOG("Startup complete, took %ldms!", static_cast<long int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - BeginTime).count()));

		// Save the current time
		m_StartTime = std::chrono::steady_clock::now();

		#ifdef _WIN32
			EnableMenuItem(ConsoleMenu, SC_CLOSE, MF_ENABLED);  // Re-enable close button
		#endif

		for (;;)
		{
			m_StopEvent.Wait();

			break;
		}

		// Stop the server:
		m_WebAdmin->Stop();

		LOG("Shutting down server...");
		m_Server->Shutdown();
	}  // if (m_Server->Start()
	else
	{
		cRoot::m_TerminateEventRaised = true;
		#ifdef _WIN32
			EnableMenuItem(ConsoleMenu, SC_CLOSE, MF_ENABLED);  // Re-enable close button
		#endif
	}

	delete m_MojangAPI; m_MojangAPI = nullptr;

	LOGD("Shutting down deadlock detector...");
	dd.Stop();

	LOGD("Stopping world threads...");
	StopWorlds(dd);

	LOGD("Stopping authenticator...");
	m_Authenticator.Stop();

	LOGD("Freeing MonsterConfig...");
	delete m_MonsterConfig; m_MonsterConfig = nullptr;
	delete m_WebAdmin; m_WebAdmin = nullptr;

	LOGD("Unloading recipes...");
	delete m_FurnaceRecipe;   m_FurnaceRecipe = nullptr;
	delete m_CraftingRecipes; m_CraftingRecipes = nullptr;

	LOG("Unloading worlds...");
	UnloadWorlds();

	LOGD("Stopping plugin manager...");
	delete m_PluginManager; m_PluginManager = nullptr;

	cItemHandler::Deinit();

	LOG("Cleaning up...");
	delete m_Server; m_Server = nullptr;

	m_InputThreadRunFlag.clear();
	#ifdef _WIN32
		DWORD Length;
		INPUT_RECORD Record
		{
			KEY_EVENT,
			{
				{
					TRUE,
					1,
					VK_RETURN,
					static_cast<WORD>(MapVirtualKey(VK_RETURN, MAPVK_VK_TO_VSC)),
					{ { VK_RETURN } },
					0
				}
			}
		};

		// Can't kill the input thread since it breaks cin (getline doesn't block / receive input on restart)
		// Apparently no way to unblock getline
		// Only thing I can think of for now
		if (WriteConsoleInput(GetStdHandle(STD_INPUT_HANDLE), &Record, 1, &Length) == 0)
		{
			LOGWARN("Couldn't notify the input thread; the server will hang before shutdown!");
			m_TerminateEventRaised = true;
			m_InputThread.detach();
		}
		else
		{
			m_InputThread.join();
		}
	#else
		if (m_InputThread.get_id() != std::thread::id())
		{
			if (pthread_kill(m_InputThread.native_handle(), SIGKILL) != 0)
			{
				LOGWARN("Couldn't notify the input thread; the server will hang before shutdown!");
				m_TerminateEventRaised = true;
				m_InputThread.detach();
			}
		}
	#endif

	if (m_TerminateEventRaised)
	{
		LOG("Shutdown successful!");
	}
	else
	{
		LOG("Shutdown successful - restarting...");
	}
	LOG("--- Stopped Log ---");
}





void cRoot::StopServer()
{
	// Kick all players from the server with custom disconnect message
	class cPlayerCallback : public cPlayerListCallback
	{
		AString m_ShutdownMessage;
		virtual bool Item(cPlayer * a_Player)
		{
			a_Player->GetClientHandlePtr()->Kick(m_ShutdownMessage);
			m_HasSentDisconnect = true;
			return false;
		}
	public:
		bool m_HasSentDisconnect;
		cPlayerCallback(AString a_ShutdownMessage) : m_ShutdownMessage(a_ShutdownMessage) { m_HasSentDisconnect = false; }
	} PlayerCallback(m_Server->GetShutdownMessage());

	cRoot::Get()->ForEachPlayer(PlayerCallback);
	if (PlayerCallback.m_HasSentDisconnect)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	m_TerminateEventRaised = true;
	m_StopEvent.Set();
	m_InputThreadRunFlag.clear();
}





void cRoot::LoadGlobalSettings()
{
	// Nothing needed yet
}





void cRoot::LoadWorlds(cSettingsRepositoryInterface & a_Settings, bool a_IsNewIniFile)
{
	if (a_IsNewIniFile)
	{
		a_Settings.AddValue("Worlds", "DefaultWorld", "world");
		a_Settings.AddValue("Worlds", "World", "world_nether");
		a_Settings.AddValue("Worlds", "World", "world_the_end");
		m_pDefaultWorld = new cWorld("world");
		m_WorldsByName["world"] = m_pDefaultWorld;
		m_WorldsByName["world_nether"] = new cWorld("world_nether", dimNether, "world");
		m_WorldsByName["world_the_end"] = new cWorld("world_the_end", dimEnd, "world");
		return;
	}

	// First get the default world
	AString DefaultWorldName = a_Settings.GetValueSet("Worlds", "DefaultWorld", "world");
	m_pDefaultWorld = new cWorld(DefaultWorldName.c_str());
	m_WorldsByName[ DefaultWorldName ] = m_pDefaultWorld;
	auto Worlds = a_Settings.GetValues("Worlds");

	// Then load the other worlds
	if (Worlds.size() <= 0)
	{
		return;
	}

	/* Here are the world creation rules. Note that these only apply for a world which is in settings.ini but has no world.ini file.
	If an ini file is present, it overrides the world linkages and the dimension type in cWorld::start()
	The creation rules are as follows:

	- If a world exists in settings.ini but has no world.ini, then:
		- If the world name is x_nether, create a world.ini with the dimension type "nether".
			- If a world called x exists, set it as x_nether's overworld.
			- Otherwise set the default world as x_nether's overworld.

		- If the world name is x_the_end or x_end, create a world.ini with the dimension type "end".
			- If a world called x exists, set it as x_the_end's overworld.
			- Otherwise set the default world as x_the_end's overworld.

		- If the world name is x (and doesn't end with _the_end, _end or _nether)
			- Create a world.ini with a dimension type of "overworld".
			- If a world called x_nether exists, set it as x's nether world.
			- Otherwise set x's nether world to blank.h
			- If a world called x_the_end or x_end exists, set it as x's end world.
			- Otherwise set x's nether world to blank.

	*/

	bool FoundAdditionalWorlds = false;
	for (auto WorldNameValue : Worlds)
	{
		AString ValueName = WorldNameValue.first;
		if (ValueName.compare("World") != 0)
		{
			continue;
		}
		AString WorldName = WorldNameValue.second;
		if (WorldName.empty())
		{
			continue;
		}
		FoundAdditionalWorlds = true;
		cWorld * NewWorld;
		AString LowercaseName = StrToLower(WorldName);
		AString NetherAppend = "_nether";
		AString EndAppend1 = "_the_end";
		AString EndAppend2 = "_end";

		// if the world is called x_nether
		if ((LowercaseName.size() > NetherAppend.size()) && (LowercaseName.substr(LowercaseName.size() - NetherAppend.size()) == NetherAppend))
		{
			// The world is called x_nether, see if a world called x exists. If yes, choose it as the linked world,
			// otherwise, choose the default world as the linked world.
			// As before, any ini settings will completely override this if an ini is already present.

			AString LinkTo = WorldName.substr(0, WorldName.size() - NetherAppend.size());
			if (GetWorld(LinkTo) == nullptr)
			{
				LinkTo = DefaultWorldName;
			}
			NewWorld = new cWorld(WorldName.c_str(), dimNether, LinkTo);
		}
		// if the world is called x_the_end
		else if ((LowercaseName.size() > EndAppend1.size()) && (LowercaseName.substr(LowercaseName.size() - EndAppend1.size()) == EndAppend1))
		{
			// The world is called x_the_end, see if a world called x exists. If yes, choose it as the linked world,
			// otherwise, choose the default world as the linked world.
			// As before, any ini settings will completely override this if an ini is already present.

			AString LinkTo = WorldName.substr(0, WorldName.size() - EndAppend1.size());
			if (GetWorld(LinkTo) == nullptr)
			{
				LinkTo = DefaultWorldName;
			}
			NewWorld = new cWorld(WorldName.c_str(), dimEnd, LinkTo);
		}
		// if the world is called x_end
		else if ((LowercaseName.size() > EndAppend2.size()) && (LowercaseName.substr(LowercaseName.size() - EndAppend2.size()) == EndAppend2))
		{
			// The world is called x_end, see if a world called x exists. If yes, choose it as the linked world,
			// otherwise, choose the default world as the linked world.
			// As before, any ini settings will completely override this if an ini is already present.

			AString LinkTo = WorldName.substr(0, WorldName.size() - EndAppend2.size());
			if (GetWorld(LinkTo) == nullptr)
			{
				LinkTo = DefaultWorldName;
			}
			NewWorld = new cWorld(WorldName.c_str(), dimEnd, LinkTo);
		}
		else
		{
			NewWorld = new cWorld(WorldName.c_str());
		}
		m_WorldsByName[WorldName] = NewWorld;
	}  // for i - Worlds

	if (!FoundAdditionalWorlds)
	{
		if (a_Settings.GetKeyComment("Worlds", 0) != " World=secondworld")
		{
			a_Settings.DeleteKeyComment("Worlds", 0);
			a_Settings.AddKeyComment("Worlds", " World=secondworld");
		}
	}
}





void cRoot::StartWorlds(cDeadlockDetect & a_DeadlockDetect)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->Start(a_DeadlockDetect);
		itr->second->InitializeSpawn();
		m_PluginManager->CallHookWorldStarted(*itr->second);
	}
}





void cRoot::StopWorlds(cDeadlockDetect & a_DeadlockDetect)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->Stop(a_DeadlockDetect);
	}
}





void cRoot::UnloadWorlds(void)
{
	m_pDefaultWorld = nullptr;
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		delete itr->second;
	}
	m_WorldsByName.clear();
}





cWorld * cRoot::GetDefaultWorld()
{
	return m_pDefaultWorld;
}





cWorld * cRoot::GetWorld(const AString & a_WorldName)
{
	WorldMap::iterator itr = m_WorldsByName.find(a_WorldName);
	if (itr != m_WorldsByName.end())
	{
		return itr->second;
	}

	return nullptr;
}





bool cRoot::ForEachWorld(cWorldListCallback & a_Callback)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(), itr2 = itr; itr != m_WorldsByName.end(); itr = itr2)
	{
		++itr2;
		if (itr->second != nullptr)
		{
			if (a_Callback.Item(itr->second))
			{
				return false;
			}
		}
	}
	return true;
}





void cRoot::TickCommands(void)
{
	// Execute any pending commands:
	cCommandQueue PendingCommands;
	{
		cCSLock Lock(m_CSPendingCommands);
		std::swap(PendingCommands, m_PendingCommands);
	}
	for (cCommandQueue::iterator itr = PendingCommands.begin(), end = PendingCommands.end(); itr != end; ++itr)
	{
		ExecuteConsoleCommand(itr->m_Command, *(itr->m_Output));
	}
}





void cRoot::QueueExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output)
{
	// Put the command into a queue (Alleviates FS #363):
	cCSLock Lock(m_CSPendingCommands);
	m_PendingCommands.emplace_back(a_Cmd, &a_Output);
}





void cRoot::QueueExecuteConsoleCommand(const AString & a_Cmd)
{
	// Put the command into a queue (Alleviates FS #363):
	cCSLock Lock(m_CSPendingCommands);
	m_PendingCommands.push_back(cCommand(a_Cmd, new cLogCommandDeleteSelfOutputCallback));
}





void cRoot::ExecuteConsoleCommand(const AString & a_Cmd, cCommandOutputCallback & a_Output)
{
	// Some commands are built-in:
	if (a_Cmd == "stop")
	{
		StopServer();
		return;
	}
	else if (a_Cmd == "restart")
	{
		m_StopEvent.Set();
		m_InputThreadRunFlag.clear();
		return;
	}

	LOG("Executing console command: \"%s\"", a_Cmd.c_str());
	m_Server->ExecuteConsoleCommand(a_Cmd, a_Output);
}





void cRoot::KickUser(int a_ClientID, const AString & a_Reason)
{
	m_Server->KickUser(a_ClientID, a_Reason);
}





void cRoot::AuthenticateUser(int a_ClientID, const AString & a_Name, const cUUID & a_UUID, const Json::Value & a_Properties)
{
	m_Server->AuthenticateUser(a_ClientID, a_Name, a_UUID, a_Properties);
}





int cRoot::GetTotalChunkCount(void)
{
	int res = 0;
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		res += itr->second->GetNumChunks();
	}
	return res;
}





void cRoot::SaveAllChunks(void)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->QueueSaveAllChunks();
	}
}



void cRoot::SendPlayerLists(cPlayer * a_DestPlayer)
{
	for (const auto & itr : m_WorldsByName)
	{
		itr.second->SendPlayerList(a_DestPlayer);
	}  // for itr - m_WorldsByName[]
}



void cRoot::BroadcastPlayerListsAddPlayer(const cPlayer & a_Player, const cClientHandle * a_Exclude)
{
	for (const auto & itr : m_WorldsByName)
	{
		itr.second->BroadcastPlayerListAddPlayer(a_Player);
	}  // for itr - m_WorldsByName[]
}


void cRoot::BroadcastChat(const AString & a_Message, eMessageType a_ChatPrefix)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(), end = m_WorldsByName.end(); itr != end; ++itr)
	{
		itr->second->BroadcastChat(a_Message, nullptr, a_ChatPrefix);
	}  // for itr - m_WorldsByName[]
}





void cRoot::BroadcastChat(const cCompositeChat & a_Message)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(), end = m_WorldsByName.end(); itr != end; ++itr)
	{
		itr->second->BroadcastChat(a_Message);
	}  // for itr - m_WorldsByName[]
}



bool cRoot::ForEachPlayer(cPlayerListCallback & a_Callback)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(), itr2 = itr; itr != m_WorldsByName.end(); itr = itr2)
	{
		++itr2;
		if (!itr->second->ForEachPlayer(a_Callback))
		{
			return false;
		}
	}
	return true;
}





bool cRoot::FindAndDoWithPlayer(const AString & a_PlayerName, cPlayerListCallback & a_Callback)
{
	class cCallback : public cPlayerListCallback
	{
		size_t        m_BestRating;
		size_t        m_NameLength;
		const AString m_PlayerName;

		virtual bool Item (cPlayer * a_pPlayer)
		{
			size_t Rating = RateCompareString (m_PlayerName, a_pPlayer->GetName());
			if ((Rating > 0) && (Rating >= m_BestRating))
			{
				m_BestMatch = a_pPlayer->GetName();
				if (Rating > m_BestRating)
				{
					m_NumMatches = 0;
				}
				m_BestRating = Rating;
				++m_NumMatches;
			}
			if (Rating == m_NameLength)  // Perfect match
			{
				return true;
			}
			return false;
		}

	public:
		cCallback (const AString & a_CBPlayerName) :
			m_BestRating(0),
			m_NameLength(a_CBPlayerName.length()),
			m_PlayerName(a_CBPlayerName),
			m_BestMatch(),
			m_NumMatches(0)
		{}

		AString m_BestMatch;
		unsigned  m_NumMatches;
	} Callback (a_PlayerName);
	ForEachPlayer(Callback);

	if (Callback.m_NumMatches == 1)
	{
		return DoWithPlayer(Callback.m_BestMatch, a_Callback);
	}
	return false;
}





bool cRoot::DoWithPlayerByUUID(const cUUID & a_PlayerUUID, cPlayerListCallback & a_Callback)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr !=  m_WorldsByName.end(); ++itr)
	{
		if (itr->second->DoWithPlayerByUUID(a_PlayerUUID, a_Callback))
		{
			return true;
		}
	}
	return false;
}





bool cRoot::DoWithPlayer(const AString & a_PlayerName, cPlayerListCallback & a_Callback)
{
	for (auto World : m_WorldsByName)
	{
		if (World.second->DoWithPlayer(a_PlayerName, a_Callback))
		{
			return true;
		}
	}
	return false;
}





AString cRoot::GetProtocolVersionTextFromInt(int a_ProtocolVersion)
{
	return cProtocolRecognizer::GetVersionTextFromInt(a_ProtocolVersion);
}





int cRoot::GetVirtualRAMUsage(void)
{
	#ifdef _WIN32
		PROCESS_MEMORY_COUNTERS_EX pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc)))
		{
			return (int)(pmc.PrivateUsage / 1024);
		}
		return -1;
	#elif defined(__linux__)
		// Code adapted from https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		std::ifstream StatFile("/proc/self/status");
		if (!StatFile.good())
		{
			return -1;
		}
		while (StatFile.good())
		{
			AString Line;
			std::getline(StatFile, Line);
			if (strncmp(Line.c_str(), "VmSize:", 7) == 0)
			{
				int res = atoi(Line.c_str() + 8);
				return (res == 0) ? -1 : res;  // If parsing failed, return -1
			}
		}
		return -1;
	#elif defined (__APPLE__)
		// Code adapted from https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		struct task_basic_info t_info;
		mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

		if (KERN_SUCCESS == task_info(
			mach_task_self(),
			TASK_BASIC_INFO,
			reinterpret_cast<task_info_t>(&t_info),
			&t_info_count
		))
		{
			return static_cast<int>(t_info.virtual_size / 1024);
		}
		return -1;
	#else
		LOGINFO("%s: Unknown platform, cannot query memory usage", __FUNCTION__);
		return -1;
	#endif
}





int cRoot::GetPhysicalRAMUsage(void)
{
	#ifdef _WIN32
		PROCESS_MEMORY_COUNTERS pmc;
		if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc)))
		{
			return (int)(pmc.WorkingSetSize / 1024);
		}
		return -1;
	#elif defined(__linux__)
		// Code adapted from https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		std::ifstream StatFile("/proc/self/status");
		if (!StatFile.good())
		{
			return -1;
		}
		while (StatFile.good())
		{
			AString Line;
			std::getline(StatFile, Line);
			if (strncmp(Line.c_str(), "VmRSS:", 6) == 0)
			{
				int res = atoi(Line.c_str() + 7);
				return (res == 0) ? -1 : res;  // If parsing failed, return -1
			}
		}
		return -1;
	#elif defined (__APPLE__)
		// Code adapted from https://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
		struct task_basic_info t_info;
		mach_msg_type_number_t t_info_count = TASK_BASIC_INFO_COUNT;

		if (KERN_SUCCESS == task_info(
			mach_task_self(),
			TASK_BASIC_INFO,
			reinterpret_cast<task_info_t>(&t_info),
			&t_info_count
		))
		{
			return static_cast<int>(t_info.resident_size / 1024);
		}
		return -1;
	#else
		LOGINFO("%s: Unknown platform, cannot query memory usage", __FUNCTION__);
		return -1;
	#endif
}





void cRoot::LogChunkStats(cCommandOutputCallback & a_Output)
{
	int SumNumValid = 0;
	int SumNumDirty = 0;
	int SumNumInLighting = 0;
	int SumNumInGenerator = 0;
	int SumMem = 0;
	for (WorldMap::iterator itr = m_WorldsByName.begin(), end = m_WorldsByName.end(); itr != end; ++itr)
	{
		cWorld * World = itr->second;
		int NumInGenerator = World->GetGeneratorQueueLength();
		int NumInSaveQueue = static_cast<int>(World->GetStorageSaveQueueLength());
		int NumInLoadQueue = static_cast<int>(World->GetStorageLoadQueueLength());
		int NumValid = 0;
		int NumDirty = 0;
		int NumInLighting = 0;
		World->GetChunkStats(NumValid, NumDirty, NumInLighting);
		a_Output.Out("World %s:", World->GetName().c_str());
		a_Output.Out("  Num loaded chunks: %d", NumValid);
		a_Output.Out("  Num dirty chunks: %d", NumDirty);
		a_Output.Out("  Num chunks in lighting queue: %d", NumInLighting);
		a_Output.Out("  Num chunks in generator queue: %d", NumInGenerator);
		a_Output.Out("  Num chunks in storage load queue: %d", NumInLoadQueue);
		a_Output.Out("  Num chunks in storage save queue: %d", NumInSaveQueue);
		int Mem = NumValid * static_cast<int>(sizeof(cChunk));
		a_Output.Out("  Memory used by chunks: %d KiB (%d MiB)", (Mem + 1023) / 1024, (Mem + 1024 * 1024 - 1) / (1024 * 1024));
		a_Output.Out("  Per-chunk memory size breakdown:");
		a_Output.Out("    block types:    " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", sizeof(cChunkDef::BlockTypes), (sizeof(cChunkDef::BlockTypes) + 1023) / 1024);
		a_Output.Out("    block metadata: " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", sizeof(cChunkDef::BlockNibbles), (sizeof(cChunkDef::BlockNibbles) + 1023) / 1024);
		a_Output.Out("    block lighting: " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", 2 * sizeof(cChunkDef::BlockNibbles), (2 * sizeof(cChunkDef::BlockNibbles) + 1023) / 1024);
		a_Output.Out("    heightmap:      " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", sizeof(cChunkDef::HeightMap), (sizeof(cChunkDef::HeightMap) + 1023) / 1024);
		a_Output.Out("    biomemap:       " SIZE_T_FMT_PRECISION(6)  " bytes (" SIZE_T_FMT_PRECISION(3)  " KiB)", sizeof(cChunkDef::BiomeMap), (sizeof(cChunkDef::BiomeMap) + 1023) / 1024);
		SumNumValid += NumValid;
		SumNumDirty += NumDirty;
		SumNumInLighting += NumInLighting;
		SumNumInGenerator += NumInGenerator;
		SumMem += Mem;
	}
	a_Output.Out("Totals:");
	a_Output.Out("  Num loaded chunks: %d", SumNumValid);
	a_Output.Out("  Num dirty chunks: %d", SumNumDirty);
	a_Output.Out("  Num chunks in lighting queue: %d", SumNumInLighting);
	a_Output.Out("  Num chunks in generator queue: %d", SumNumInGenerator);
	a_Output.Out("  Memory used by chunks: %d KiB (%d MiB)", (SumMem + 1023) / 1024, (SumMem + 1024 * 1024 - 1) / (1024 * 1024));
}





int cRoot::GetFurnaceFuelBurnTime(const cItem & a_Fuel)
{
	cFurnaceRecipe * FR = Get()->GetFurnaceRecipe();
	return FR->GetBurnTime(a_Fuel);
}





AStringVector cRoot::GetPlayerTabCompletionMultiWorld(const AString & a_Text)
{
	AStringVector Results;
	class cWorldCallback : public cWorldListCallback
	{
	public:
		cWorldCallback(AStringVector & a_Results, const AString & a_Search) :
			m_Results(a_Results),
			m_Search(a_Search)
		{
		}

		virtual bool Item(cWorld * a_World) override
		{
			a_World->TabCompleteUserName(m_Search, m_Results);
			return false;
		}
	private:
		AStringVector & m_Results;
		const AString & m_Search;
	} WC(Results, a_Text);

	Get()->ForEachWorld(WC);
	return Results;
}
