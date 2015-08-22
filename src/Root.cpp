
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Root.h"
#include "Server.h"
#include "World.h"
#include "WebAdmin.h"
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
#include "SelfTests.h"

#include <iostream>

#ifdef _WIN32
	#include <psapi.h>
#elif defined(__linux__)
	#include <fstream>
	#include <signal.h>
#elif defined(__APPLE__)
	#include <mach/mach.h>
#endif





cRoot * cRoot::s_Root = nullptr;





cRoot::cRoot(void) :
	m_pDefaultWorld(nullptr),
	m_Server(nullptr),
	m_MonsterConfig(nullptr),
	m_CraftingRecipes(nullptr),
	m_FurnaceRecipe(nullptr),
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
	if (!std::cin.good())
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

	cLogger::cListener * consoleLogListener = MakeConsoleListener(m_RunAsService);
	cLogger::cListener * fileLogListener = new cFileListener();
	cLogger::GetInstance().AttachListener(consoleLogListener);
	cLogger::GetInstance().AttachListener(fileLogListener);

	LOG("--- Started Log ---");

	#ifdef BUILD_ID
		LOG("MCServer " BUILD_SERIES_NAME " build id: " BUILD_ID);
		LOG("from commit id: " BUILD_COMMIT_ID " built at: " BUILD_DATETIME);
	#endif

	// Run the self-tests registered previously via cSelfTests::Register():
	#ifdef SELF_TEST
		cSelfTests::ExecuteAll();
	#endif

	cDeadlockDetect dd;
	auto BeginTime = std::chrono::steady_clock::now();

	LoadGlobalSettings();

	LOG("Creating new server instance...");
	m_Server = new cServer();

	LOG("Reading server config...");

	auto IniFile = cpp14::make_unique<cIniFile>();
	if (!IniFile->ReadFile("settings.ini"))
	{
		LOGWARN("Regenerating settings.ini, all settings will be reset");
		IniFile->AddHeaderComment(" This is the main server configuration");
		IniFile->AddHeaderComment(" Most of the settings here can be configured using the webadmin interface, if enabled in webadmin.ini");
		IniFile->AddHeaderComment(" See: http://wiki.mc-server.org/doku.php?id=configure:settings.ini for further configuration help");
	}
	auto settingsRepo = cpp14::make_unique<cOverridesSettingsRepository>(std::move(IniFile), std::move(a_OverridesRepo));

	LOG("Starting server...");
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

	LOGD("Loading worlds...");
	LoadWorlds(*settingsRepo);

	LOGD("Loading plugin manager...");
	m_PluginManager = new cPluginManager();
	m_PluginManager->ReloadPluginsNow(*settingsRepo);

	LOGD("Loading MonsterConfig...");
	m_MonsterConfig = new cMonsterConfig;

	// This sets stuff in motion
	LOGD("Starting Authenticator...");
	m_Authenticator.Start(*settingsRepo);

	LOGD("Starting worlds...");
	StartWorlds();

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

		#if !defined(ANDROID_NDK)
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
		#endif

		LOG("Startup complete, took %ldms!", static_cast<long int>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - BeginTime).count()));

		// Save the current time
		m_StartTime = std::chrono::steady_clock::now();

		#ifdef _WIN32
			EnableMenuItem(ConsoleMenu, SC_CLOSE, MF_ENABLED);  // Re-enable close button
		#endif

		for (;;)
		{
			m_StopEvent.Wait();

			if (m_TerminateEventRaised && m_RunAsService)
			{
				// Dont kill if running as a service
				m_TerminateEventRaised = false;
			}
			else
			{
				break;
			}
		}

		// Stop the server:
		m_WebAdmin->Stop();

		LOG("Shutting down server...");
		m_Server->Shutdown();
	}  // if (m_Server->Start()

	delete m_MojangAPI; m_MojangAPI = nullptr;

	LOGD("Shutting down deadlock detector...");
	dd.Stop();

	LOGD("Stopping world threads...");
	StopWorlds();

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
		if (pthread_kill(m_InputThread.native_handle(), SIGKILL) != 0)
		{
			LOGWARN("Couldn't notify the input thread; the server will hang before shutdown!");
			m_TerminateEventRaised = true;
			m_InputThread.detach();
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

	cLogger::GetInstance().DetachListener(consoleLogListener);
	delete consoleLogListener;
	cLogger::GetInstance().DetachListener(fileLogListener);
	delete fileLogListener;
}





void cRoot::LoadGlobalSettings()
{
	// Nothing needed yet
}





void cRoot::LoadWorlds(cSettingsRepositoryInterface & a_Settings)
{
	// First get the default world
	AString DefaultWorldName = a_Settings.GetValueSet("Worlds", "DefaultWorld", "world");
	m_pDefaultWorld = new cWorld(DefaultWorldName.c_str());
	m_WorldsByName[ DefaultWorldName ] = m_pDefaultWorld;

	// Then load the other worlds
	auto Worlds = a_Settings.GetValues("Worlds");
	if (Worlds.size() <= 0)
	{
		return;
	}

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
		cWorld * NewWorld = new cWorld(WorldName.c_str());
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





cWorld * cRoot::CreateAndInitializeWorld(const AString & a_WorldName, eDimension a_Dimension, const AString & a_OverworldName, bool a_InitSpawn)
{
	cWorld * World = m_WorldsByName[a_WorldName];
	if (World != nullptr)
	{
		return World;
	}

	cWorld * NewWorld = new cWorld(a_WorldName.c_str(), a_Dimension, a_OverworldName);
	m_WorldsByName[a_WorldName] = NewWorld;
	NewWorld->Start();
	if (a_InitSpawn)
	{
		NewWorld->InitializeSpawn();
	}
	m_PluginManager->CallHookWorldStarted(*NewWorld);
	return NewWorld;
}





void cRoot::StartWorlds(void)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->Start();
		itr->second->InitializeSpawn();
		m_PluginManager->CallHookWorldStarted(*itr->second);
	}
}





void cRoot::StopWorlds(void)
{
	for (WorldMap::iterator itr = m_WorldsByName.begin(); itr != m_WorldsByName.end(); ++itr)
	{
		itr->second->Stop();
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





cWorld * cRoot::GetWorld(const AString & a_WorldName, bool a_SearchForFolder)
{
	WorldMap::iterator itr = m_WorldsByName.find(a_WorldName);
	if (itr != m_WorldsByName.end())
	{
		return itr->second;
	}

	if (a_SearchForFolder && cFile::IsFolder(FILE_IO_PREFIX + a_WorldName))
	{
		return CreateAndInitializeWorld(a_WorldName);
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
		m_TerminateEventRaised = true;
		m_StopEvent.Set();
		m_InputThreadRunFlag.clear();
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





void cRoot::AuthenticateUser(int a_ClientID, const AString & a_Name, const AString & a_UUID, const Json::Value & a_Properties)
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
		cCallback (const AString & a_PlayerName) :
			m_BestRating(0),
			m_NameLength(a_PlayerName.length()),
			m_PlayerName(a_PlayerName),
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





bool cRoot::DoWithPlayerByUUID(const AString & a_PlayerUUID, cPlayerListCallback & a_Callback)
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
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
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
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
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
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
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
		// Code adapted from http://stackoverflow.com/questions/63166/how-to-determine-cpu-and-memory-consumption-from-inside-a-process
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





